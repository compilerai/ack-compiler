/*
 * (c) copyright 1987 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Copyright".
 */
/* $Header$ */
/*	Lint outdef construction	*/

#include	"lint.h"

#ifdef	LINT

#include	<alloc.h>
#include	"arith.h"
#include	"type.h"
#include	"LLlex.h"
#include	"Lpars.h"
#include	"stack.h"
#include	"def.h"
#include	"struct.h"
#include	"field.h"
#include	"idf.h"
#include	"level.h"
#include	"label.h"
#include	"expr.h"
#include	"l_lint.h"
#include	"l_outdef.h"
#include	"l_class.h"

extern char *symbol2str();

extern int f_VARARGSn, LINTLIB;
int stat_number = 9999;			/* static scope number */
struct outdef OutDef, OutCall;

lint_declare_idf(idf, sc)
	struct idf *idf;
	int sc;
{
	register struct def *def = idf->id_def;
	register int is_function = def->df_type->tp_fund == FUNCTION;

	if (level == L_GLOBAL) {
		lint_ext_def(idf, sc);
		if (is_function)
			def2decl(sc);
		if (sc != TYPEDEF)
			outdef();
	}
	else
	if (level >= L_LOCAL && sc != STATIC && is_function) {
		local_EFDC(idf);
	}
}

lint_ext_def(idf, sc)
	struct idf *idf;
{
/* At this place the following fields of the outputdefinition can be
 * filled:
 *		name, stat_number, class, file, line, type.
 * For variable definitions and declarations this will be all.
 * For functions the fields nrargs and argtps are filled after parsing
 * the arguments.
 * The returns-field is known at the end of the function definition.
 * sc indicates the storage class defined by the declaration specifier.
 */
	register struct def *def = idf->id_def;
	register struct type *type = def->df_type;

	OutDef.od_name = idf->id_text;
	OutDef.od_statnr = (sc == STATIC ? stat_number : 0);

	switch (type->tp_fund) {
	case ERRONEOUS:
		OutDef.od_class = XXDF;
		break;
	case FUNCTION:
		/* For the moment assume it will be a definition.
		 * If no compound_statement follows, it is a declaration,
		 * in which case the class will be adjusted by def2decl().
		 */
		OutDef.od_class = (sc == STATIC ? SFDF : EFDF);
		break;
	default:	/* a variable */
		OutDef.od_class =
			sc == EXTERN ? EVDC :
			sc == STATIC ? SVDF : EVDF;
		break;
	}
	OutDef.od_file = def->df_file;
	OutDef.od_line = def->df_line;
	OutDef.od_type = (type->tp_fund == FUNCTION ? type->tp_up : type);
	OutDef.od_valreturned = NORETURN;
}

def2decl(sc)
	int sc;
{
/* It was assumed we were parsing a function definition.
 * There was no compound statement following, so actually it was a
 * declaration. This function updates the class.
 */
	OutDef.od_class = (sc == STATIC ? XXDF : EFDC);
}

set_od_valreturned(n)
{
	OutDef.od_valreturned = n;
}

local_EFDC(idf)
	struct idf *idf;
{
	struct outdef od;

	od.od_class = EFDC;
	od.od_statnr = 0;
	od.od_name = idf->id_text;
	od.od_file = idf->id_def->df_file;
	od.od_line = idf->id_def->df_line;
	od.od_type = idf->id_def->df_type->tp_up;
	output_def(&od);
	/* The other fields are not used for this class. */
}

lint_formals()
{
/* Make a list of tp_entries containing the types of the formal
 * parameters of the function definition currently parsed.
 */
	register struct stack_entry *se = stack_level_of(L_FORMAL1)->sl_entry;
	register struct tp_entry **hook = &OutDef.od_entry;

	OutDef.od_nrargs = 0;
	while (se) {
		register struct type *type = se->se_idf->id_def->df_type;
		register struct tp_entry *te = new_tp_entry();

		switch (type->tp_fund) {
		/*	Do the conversions on the formals that could not be
			done in declare_idf().
			It is, unfortunately, impossible not to do them,
			since the corresponding actuals will have been
			converted to generate proper code and we do not
			want to duplicate the whole of expression handling
			for lint.
		*/
		case CHAR:
		case SHORT:
			type = int_type;
			break;
		case FLOAT:
			type = double_type;
			break;
		}
		te->te_type = type;
		te->te_class = !Const;
		*hook = te;
		hook = &te->next;

		OutDef.od_nrargs++;
		se = se->next;
	}
	if (f_VARARGSn > OutDef.od_nrargs) {
		warning("VARARGS%d function has only %d arguments",
			f_VARARGSn, OutDef.od_nrargs);
		f_VARARGSn = OutDef.od_nrargs;
	}
}

output_use(idf)
	struct idf *idf;
{
/* Output the usage-definition of the variable described by idf.
 */
	OutDef.od_name = idf->id_text;
	OutDef.od_statnr = (idf->id_def->df_sc == STATIC ? stat_number : 0);
	OutDef.od_class = VU;
	OutDef.od_file = FileName;
	OutDef.od_line = LineNumber;
	OutDef.od_type = idf->id_def->df_type;
	outdef();
}

outdef()
{
	output_def(&OutDef);
}

outcall()
{
	output_def(&OutCall);
}

output_def(od)
	struct outdef *od;
{
/* As the types are output the tp_entries are removed, because they
 * are then not needed anymore.
 */
	if (od->od_class == XXDF)
		return;

	if (LINTLIB) {
		switch (od->od_class) {
		case EFDF:
			od->od_class = LFDF;
			break;
		case EVDF:
			od->od_class = LVDF;
			break;
		case SFDF:
			/* remove tp_entries */
			while (od->od_entry) {
				register struct tp_entry *tmp = od->od_entry;
				od->od_entry = od->od_entry->next;
				free_tp_entry(tmp);
			}
			return;
		default:
			return;
		}
	}
	printf("%s:%d:%c", od->od_name, od->od_statnr, od->od_class);
	switch (od->od_class) {
	case EFDF:
	case SFDF:
	case LFDF:
		if (f_VARARGSn != -1) {
			printf(":%d", -1 - f_VARARGSn);
			outtypes(od->od_entry, f_VARARGSn);
		}
		else {
			printf(":%d", od->od_nrargs);
			outtypes(od->od_entry, od->od_nrargs);
		}
		od->od_entry = 0;
		printf(":%d", od->od_valreturned);
		break;
	case FC:
		printf(":%d", od->od_nrargs);
		outtypes(od->od_entry, od->od_nrargs);
		od->od_entry = 0;
		printf(":%d", od->od_valused);
		break;
	case EVDF:
	case SVDF:
	case LVDF:
	case EFDC:
	case EVDC:
	case IFDC:
	case VU:
		break;
	default:
		crash("(output_def) illegal class");
		/*NOTREACHED*/
	}
	printf(":");
	outtype(od->od_type);
	printf(":%u:%s\n", od->od_line, od->od_file);
}

outtypes(te, n)
	struct tp_entry *te;
{
/* Output n types in the tp_entry-list and remove all the entries */

	register struct tp_entry *tmp;

	while (n--) {
		if (!te) {
			crash("(outtypes) not enough tp_entries");
			/*NOTREACHED*/
		}
		printf(":");
		if (te->te_class == Const && te->te_value >= 0) {
			/* constant non-negative actual parameter */
			printf("+");
		}
		outtype(te->te_type);
		if (te->te_type->tp_fund == FUNCTION) {
			/* UGLY PATCH !!! ??? */
			/*	function names as operands are sometimes
				FUNCTION and sometimes POINTER to FUNCTION,
				depending on opaque circumstances.  E.g., in
					f(main, main);
				the first main is PtF and the second is F.
			*/
			printf("*");
		}
		tmp = te;
		te = te->next;
		free_tp_entry(tmp);
	}
	/* remove the remaining entries */
	while (te) {
		tmp = te;
		te = te->next;
		free_tp_entry(tmp);
	}
}

outtype(tp)
	struct type *tp;
{
	switch (tp->tp_fund) {
	case POINTER:
		outtype(tp->tp_up);
		printf("*");
		break;

	case ARRAY:
		outtype(tp->tp_up);
		printf("*");	/* compatible with [] */
		break;

	case FUNCTION:
		outtype(tp->tp_up);
		printf("()");
		break;

	case STRUCT:
	case UNION:
	case ENUM:
		printf("%s %s", symbol2str(tp->tp_fund), tp->tp_idf->id_text);
		break;

	case CHAR:
	case INT:
	case SHORT:
	case LONG:
	case FLOAT:
	case DOUBLE:
	case VOID:
	case ERRONEOUS:
		if (tp->tp_unsigned)
			printf("unsigned ");
		printf("%s", symbol2str(tp->tp_fund));
		break;
	default:
		crash("(outtype) illegal tp_fund");
		/*NOTREACHED*/
	}
}

implicit_func_decl(idf, file, line)
	struct idf *idf;
	char *file;
	unsigned int line;
{
	struct outdef od;

	od.od_class = IFDC;
	od.od_statnr = 0;
	od.od_name = idf->id_text;
	od.od_file = file;
	od.od_line = line;
	od.od_type = idf->id_def->df_type->tp_up;
	output_def(&od);
	/* The other fields are not used for this class. */
}

fill_outcall(ex, used)
	struct expr *ex;
	int used;
{
	register struct idf *idf = ex->OP_LEFT->VL_IDF;
	register struct def *def = idf->id_def;

	if (def->df_sc == IMPLICIT) {
		implicit_func_decl(idf, ex->ex_file, ex->ex_line);
	}

	OutCall.od_type = def->df_type->tp_up;
	OutCall.od_statnr = (def->df_sc == STATIC ? stat_number : 0);
	OutCall.od_class = FC;
	OutCall.od_name = idf->id_text;
	OutCall.od_file = ex->ex_file;
	OutCall.od_line = ex->ex_line;
	OutCall.od_entry = (struct tp_entry *)0;
	OutCall.od_nrargs = 0;

	if ((ex = ex->OP_RIGHT) != 0) {	/* function call with arguments */
		/* store types of argument expressions in tp_entries */
		while (ex->ex_class == Oper && ex->OP_OPER == PARCOMMA) {
			fill_arg(ex->OP_RIGHT);
			ex = ex->OP_LEFT;
		}
		fill_arg(ex);
	}
	OutCall.od_valused = used;	/* USED, IGNORED or VOIDED */
}

fill_arg(e)
	struct expr *e;
{
	register struct tp_entry *te;

	te = new_tp_entry();
	te->te_type = e->ex_type;
	if (is_cp_cst(e)) {
		te->te_class = Const;
		te->te_value = e->VL_VALUE;
	}
	else {
		te->te_class = !Const;
		te->te_value = (arith) 0;
	}
	te->next = OutCall.od_entry;
	OutCall.od_entry = te;
	OutCall.od_nrargs++;
}

#endif	LINT
