/*
 * (c) copyright 1983 by the Vrije Universiteit, Amsterdam, The Netherlands.
 *
 *          This product is part of the Amsterdam Compiler Kit.
 *
 * Permission to use, sell, duplicate or disclose this software must be
 * obtained in writing. Requests for such permissions may be sent to
 *
 *      Dr. Andrew S. Tanenbaum
 *      Wiskundig Seminarium
 *      Vrije Universiteit
 *      Postbox 7161
 *      1007 MC Amsterdam
 *      The Netherlands
 *
 */

#include        "ass00.h"
#include        "assex.h"

#ifndef NORCSID
static char rcs_id[] = "$Header$" ;
#endif

/*
** utilities of EM1-assembler/loader
*/

static int globstep;

/*
 * glohash returns an index in table and leaves a stepsize in globstep
 *
 */

static int glohash(aname,size) char *aname; {
	register char *p;
	register i;
	register sum;

	/*
	 * Computes a hash-value from a string.
	 * Algorithm is adding all the characters after shifting some way.
	 */

	for(sum=i=0,p=aname;*p;i += 3)
		sum += (*p++)<<(i&07);
	sum &= 077777;
	globstep = (sum / size) + 7;
	return(sum % size);
}

/*
 * lookup idname in labeltable , if it is not there enter it
 * return index in labeltable
 */

glob_t *glo2lookup(name,status) char *name; {

	return(glolookup(name,status,mglobs,oursize->n_mlab));
}

glob_t *xglolookup(name,status) char *name; {

	return(glolookup(name,status,xglobs,oursize->n_glab));
}

static void findext(g) glob_t *g ; {
	glob_t *x;

	x = xglolookup(g->g_name,ENTERING);
	if (x && (x->g_status&DEF)) {
		g->g_status |= DEF;
		g->g_val.g_addr = x->g_val.g_addr;
	}
	g->g_status |= EXT;
}

glob_t *glolookup(name,status,table,size)
char *name;     /* name */
int status;     /* kind of lookup */
glob_t *table;  /* which table to use */
int size;       /* size for hash */
{
	register glob_t *g;
	register rem,j;
	int new;

	/*
	 * lookup global symbol name in specified table.
	 * Various actions are taken depending on status.
	 *
	 * DEFINING:
	 *      Lookup or enter the symbol, check for mult. def.
	 * OCCURRING:
	 *      Lookup the symbol, export if not known.
	 * INTERNING:
	 *      Enter symbol local to the module.
	 * EXTERNING:
	 *      Enter symbol visable from every module.
	 * SEARCHING:
	 *      Lookup the symbol, return 0 if not found.
	 * ENTERING:
	 *      Lookup or enter the symbol, don't check
	 */

	rem = glohash(name,size);
	j = 0; new=0;
	g = &table[rem];
	while (g->g_name[0] != 0 && strcmp(name,g->g_name) != 0) {
		j++;
		if (j>size)
			fatal("global label table overflow");
		rem = (rem + globstep) % size;
		g = &table[rem];
	}
	if (g->g_name[0] == 0) {
		/*
		 * This symbol is shining new.
		 * Enter it in table except for status = SEARCHING
		 */
		if (status == SEARCHING)
			return(0);
		strcpy(g->g_name,name);
		g->g_status = 0;
		g->g_val.g_addr=0;
		new++;
	}
	switch(status) {
	case SEARCHING: /* nothing special */
	case ENTERING:
		break;
	case INTERNING:
		if (!new)
			werror("INA must be first occurrence of '%s'",name);
		break;
	case EXTERNING:          /* lookup in other table */
		/*
		 * The If statement is removed to be friendly
		 * to Backend writers having to deal with assemblers
		 * not following our conventions.
		if (!new)
			error("EXA must be first occurrence of '%s'",name);
		*/
		findext(g);
		break;
	case DEFINING:  /* Thou shalt not redefine */
		if (g->g_status&DEF)
			error("global symbol '%s' redefined",name);
		g->g_status |= DEF;
		break;
	case OCCURRING:
		if ( new )
			findext(g);
		g->g_status |= OCC;
		break;
	default:
		fatal("bad status in glolookup");
	}
	return(g);
}

locl_t *loclookup(an,status) {
	register locl_t *lbp,*l_lbp;
	register unsigned num;
	char hinum;

	if ( !pstate.s_locl ) fatal("label outside procedure");
	num = an;
	if ( num/LOCLABSIZE>255 ) fatal("local label number too large");
	hinum = num/LOCLABSIZE;
	l_lbp= lbp= &(*pstate.s_locl)[num%LOCLABSIZE];
	if ( lbp->l_defined==EMPTY ) {
		lbp= lbp_cast 0 ;
	} else {
		while ( lbp!= lbp_cast 0 && lbp->l_hinum != hinum ) {
			l_lbp = lbp ;
			lbp = lbp->l_chain;
		}
	}
	if ( lbp == lbp_cast 0 ) {
		if ( l_lbp->l_defined!=EMPTY ) {
			lbp = lbp_cast getarea(sizeof *lbp);
			l_lbp->l_chain= lbp ;
		} else lbp= l_lbp ;
		lbp->l_chain= lbp_cast 0 ;
		lbp->l_hinum=hinum;
		lbp->l_defined = (status==OCCURRING ? NO : YES);
		lbp->l_min= line_num;
	} else
		if (status == DEFINING) {
			if (lbp->l_defined == YES)
				error("multiple defined local symbol");
			else
				lbp->l_defined = YES;
		}
	if ( status==DEFINING ) lbp->l_min= line_num ;
	return(lbp);
}

proc_t *prolookup(name,status) char *name; {
	register proc_t *p;
	register pstat;

	/*
	 * Look up a procedure name according to status
	 *
	 * PRO_OCC:     Occurrence
	 *      Search both tables, local table first.
	 *      If not found, enter in global table
	 * PRO_INT:     INP
	 *      Enter symbol in local table.
	 * PRO_DEF:     Definition
	 *      Define local procedure.
	 * PRO_EXT:     EXP
	 *      Enter symbol in global table.
	 *
	 *      The EXT bit in this table indicates the the name is used
	 *      as external in this module.
	 */

	switch(status) {
	case PRO_OCC:
		p = searchproc(name,mprocs,oursize->n_mproc);
		if (p->p_name[0]) {
			p->p_status |= OCC;
			return(p);
		}
		p = searchproc(name,xprocs,oursize->n_xproc);
		if (p->p_name[0]) {
			p->p_status |= OCC;
			return(p);
		}
		pstat = OCC|EXT;
		unresolved++ ;
		break;
	case PRO_INT:
		p = searchproc(name,xprocs,oursize->n_xproc);
		if (p->p_name[0] && (p->p_status&EXT) )
			error("pro '%s' conflicting use",name);

		p = searchproc(name,mprocs,oursize->n_mproc);
		if (p->p_name[0])
			werror("INP must be first occurrence of '%s'",name);
		pstat = 0;
		break;
	case PRO_EXT:
		p = searchproc(name,mprocs,oursize->n_mproc);
		if (p->p_name[0])
			error("pro '%s' exists already localy",name);
		p = searchproc(name,xprocs,oursize->n_xproc);
		if (p->p_name[0]) {
			/*
			 * The If statement is removed to be friendly
			 * to Backend writers having to deal with assemblers
			 * not following our conventions.
			if ( p->p_status&EXT )
				werror("EXP must be first occurrence of '%s'",
					name) ;
			 */
			p->p_status |= EXT;
			return(p);
		}
		pstat = EXT;
		unresolved++;
		break;
	case PRO_DEF:
		p = searchproc(name,xprocs,oursize->n_xproc);
		if (p->p_name[0] && (p->p_status&EXT) ) {
			if (p->p_status&DEF)
				error("global pro '%s' redeclared",name);
			else
				unresolved-- ;
			p->p_status |= DEF;
			return(p);
		} else {
			p = searchproc(name,mprocs,oursize->n_mproc);
			if (p->p_name[0]) {
				if (p->p_status&DEF)
					error("local pro '%s' redeclared",
						name);
				p->p_status |= DEF;
				return(p);
			}
		}
		pstat = DEF;
		break;
	default:
		fatal("bad status in prolookup");
	}
	return(enterproc(name,pstat,p));
}

proc_t *searchproc(name,table,size)
	char *name;
	proc_t *table;
	int size;
{
	register proc_t *p;
	register rem,j;

	/*
	 * return a pointer into table to the place where the procedure
	 * name is or should be if in the table.
	 */

	rem = glohash(name,size);
	j = 0;
	p = &table[rem];
	while (p->p_name[0] != 0 && strcmp(name,p->p_name) != 0) {
		j++;
		if (j>size)
			fatal("procedure table overflow");
		rem = (rem + globstep) % size;
		p = &table[rem];
	}
	return(p);
}

proc_t *enterproc(name,status,place)
char *name;
char status;
proc_t *place; {
	register proc_t *p;

	/*
	 * Enter the procedure name into the table at place place.
	 * Place had better be computed by searchproc().
	 *
	 * NOTE:
	 *      At this point the procedure gets assigned a number.
	 *      This number is used as a parameter of cal and in some
	 *      other ways. There exists a 1-1 correspondence between
	 *      procedures and numbers.
	 *      Two local procedures with the same name in different
	 *      modules have different numbers.
	 */

	p=place;
	strcpy(p->p_name,name);
	p->p_status = status;
	if (procnum>=oursize->n_proc)
		fatal("too many procedures");
	p->p_num = procnum++;
	return(p);
}
