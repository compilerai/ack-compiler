/*
 * doscan.c - scan formatted input
 */
/* $Header$ */

#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<stdarg.h>
#include	"loc_incl.h"

#define	NUMLEN	512
#define	NR_CHARS	256

static char	Xtable[NR_CHARS];
static char	inp_buf[NUMLEN];

/* Collect a number of characters which constitite an ordinal number.
 * When the type is 'i', the base can be 8, 10, or 16, depending on the
 * first 1 or 2 characters. This means that the base must be adjusted
 * according to the format of the number. At the end of the function, base
 * is then set to 0, so strtol() will get the right argument.
 */
static char *
o_collect(register int c, register FILE *stream, char type,
			int width, int *basep)
{
	register char *bufp = inp_buf;
	register int base;

	switch (type) {
	case 'i':	/* i means octal, decimal or hexadecimal */
	case 'p':
	case 'x':
	case 'X':	base = 16;	break;
	case 'd':
	case 'u':	base = 10;	break;
	case 'o':	base = 8;	break;
	case 'b':	base = 2;	break;
	}

	if (c == '-' || c == '+') {
		*bufp++ = c;
		if (--width);
		    c = getc(stream);
	}

	if (width && c == '0' && base == 16) {
		*bufp++ = c;
		if (--width)
			c = getc(stream);
		if (c != 'x' && c != 'X') {
			if (type == 'i') base = 8;
		}
		else if (width) {
			*bufp++ = c;
			if (--width)
				c = getc(stream);
		}
	}
	else if (type == 'i') base = 10;

	while (width) {
		if (((base == 10) && isdigit(c))
		    || ((base == 16) && isxdigit(c))
		    || ((base == 8) && isdigit(c) && (c < '8'))
		    || ((base == 2) && isdigit(c) && (c < '2'))) {
			*bufp++ = c;
			if (--width)
				c = getc(stream);
		}
		else break;
	}

	if (width && c != EOF) ungetc(c, stream);
	if (type == 'i') base = 0;
	*basep = base;
	*bufp = '\0';
	return bufp - 1;
}

#ifndef	NOFLOAT
/* The function f_collect() reads a string that has the format of a
 * floating-point number. The function returns as soon as a format-error
 * is encountered, leaving the offending character in the input. This means
 * that 1.el leaves the 'l' in the input queue. Since all detection of
 * format errors is done here, _doscan() doesn't call strtod() when it's
 * not necessary, although the use of the width field can cause incomplete
 * numbers to be passed to strtod(). (e.g. 1.3e+)
 */
static char *
f_collect(register int c, register FILE *stream, register int width)
{
	register char *bufp = inp_buf;
	int digit_seen = 0;

	if (c == '-' || c == '+') {
		*bufp++ = c;
		if (--width)
			c = getc(stream);
	}

	while (width && isdigit(c)) {
		digit_seen++;
		*bufp++ = c;
		if (--width)
			c = getc(stream);
	}
	if (width && c == '.') {
		*bufp++ = c;
		if(--width)
			c = getc(stream);
		while (width && isdigit(c)) {
			digit_seen++;
			*bufp++ = c;
			if (--width)
				c = getc(stream);
		}
	}

	if (!digit_seen) {
		if (width && c != EOF) ungetc(c, stream);
		return inp_buf - 1;
	}
	else digit_seen = 0;

	if (width && (c == 'e' || c == 'E')) {
		*bufp++ = c;
		if (--width)
			c = getc(stream);
		if (width && (c == '+' || c == '-')) {
			*bufp++ = c;
			if (--width)
				c = getc(stream);
		}
		while (width && isdigit(c)) {
			digit_seen++;
			*bufp++ = c;
			if (--width)
				c = getc(stream);
		}
		if (!digit_seen) {
			if (width && c != EOF) ungetc(c,stream);
			return inp_buf - 1;
		}
	}

	if (width && c != EOF) ungetc(c, stream);
	*bufp = '\0';
	return bufp - 1;
}
#endif	/* NOFLOAT */


/*
 * the routine that does the scanning 
 */

int
_doscan(register FILE *stream, const char *format, va_list ap)
{
	int		done = 0;	/* number of items done */
	int		nrchars = 0;	/* number of characters read */
	int		conv = 0;	/* # of conversions */
	int		base;		/* conversion base */
	unsigned long	val;		/* an integer value */
	char		*str, *tmp_string;	/* temporary pointers */
	unsigned	width;		/* width of field */
	int		flags;		/* some flags */
	int		reverse;	/* reverse the checking in [...] */
	int		kind;
	register int	ic;		/* the input character */
#ifndef	NOFLOAT
	long double	ld_val;
#endif

	if (!*format) return 0;

	while (1) {
		if (isspace(*format)) {
			while (isspace(*format))
				++format;	/* skip whitespace */
			ic = getc(stream);
			nrchars++;
			while (isspace (ic)) {
				ic = getc(stream);
				nrchars++;
			}
			if (ic != EOF) ungetc(ic,stream);
			nrchars--;
		}
		if (!*format) break;	/* end of format */

		ic = getc(stream);
		nrchars++;

		if (ic == EOF)
			return conv ? done : EOF;
		if (*format != '%') {
			if (ic != *format)
				break;		/* matching error */
			++format;
			continue;
		}
		++format;
		if (*format == '%') {
			if (ic == '%') {
				format++;
				continue;
			}
			else break;
		}
		flags = 0;
		if (*format == '*') {
			++format;
			flags |= FL_NOASSIGN;
		}
		if (isdigit (*format)) {
			flags |= FL_WIDTHSPEC;
			for (width = 0; isdigit (*format);)
				width = width * 10 + *format++ - '0';
		}

		switch (*format) {
		case 'h': flags |= FL_SHORT; format++; break;
		case 'l': flags |= FL_LONG; format++; break;
		case 'L': flags |= FL_LONGDOUBLE; format++; break;
		}
		kind = *format;
		if ((kind != 'c') && (kind != '[') && (kind != 'n')) {
			while (ic != EOF && isspace(ic)) {
				ic = getc(stream);
				nrchars++;
			}
			if (ic == EOF) return conv ? done : EOF;
		}
		conv++;
		switch (kind) {
		default:
			if (kind == ic) continue;
			break;
		case 'n':
			if (ic != EOF) ungetc(ic, stream);
			nrchars--;
			if (!(flags & FL_NOASSIGN)) {
				if (flags & FL_SHORT)
					*va_arg(ap, short *) = (short) nrchars;
				else if (flags & FL_LONG)
					*va_arg(ap, long *) = (long) nrchars;
				else
					*va_arg(ap, int *) = (int) nrchars;
			}
			break;
		case 'b':		/* binary */
		case 'd':		/* decimal */
		case 'i':		/* general integer */
		case 'o':		/* octal */
		case 'p':		/* pointer */
		case 'u':		/* unsigned */
		case 'x':		/* hexadecimal */
		case 'X':		/* ditto */
			if (!(flags & FL_WIDTHSPEC))
				width = NUMLEN;
			if (!width) {
				if (ic != EOF) ungetc(ic, stream);
				return done;
			}
			str = o_collect(ic, stream, kind, width, &base);
			if (str < inp_buf
			    || (str == inp_buf && *str == '-')) return done;
			nrchars += str - inp_buf + 1;
			if (!(flags & FL_NOASSIGN)) {
				if (kind == 'd' || kind == 'i')
				    val = strtol(inp_buf, &tmp_string, base);
				else
				    val = strtoul(inp_buf, &tmp_string, base);
				if (flags & FL_LONG)
					*va_arg(ap, unsigned long *) = (unsigned long) val;
				else if (flags & FL_SHORT)
					*va_arg(ap, unsigned short *) = (unsigned short) val;
				else
					*va_arg(ap, unsigned *) = (unsigned) val;
				done++;
			}
			break;
		case 'c':
			if (!(flags & FL_WIDTHSPEC))
				width = 1;
			if (!(flags & FL_NOASSIGN))
				tmp_string = va_arg(ap, char *);
			if (!width) {
				if (ic != EOF) ungetc(ic, stream);
				return done;
			}
			if (!(flags & FL_NOASSIGN) && (ic != EOF))
				++done;
			while (width && ic != EOF) {
				if (!(flags & FL_NOASSIGN))
					*tmp_string++ = (char) ic;
				if (--width) {
					ic = getc(stream);
					nrchars++;
				}
			}

			if (width) {
				if (ic != EOF) ungetc(ic,stream);
				nrchars--;
			}
			break;

		case 's':
			if (!(flags & FL_WIDTHSPEC))
				width = 0xffff;
			if (!(flags & FL_NOASSIGN))
				tmp_string = va_arg(ap, char *);
			if (!width) {
				if (ic != EOF) ungetc(ic,stream);
				return done;
			}
			if (!(flags & FL_NOASSIGN))
				++done;
			while (width && ic != EOF && !isspace(ic)) {
				if (!(flags & FL_NOASSIGN))
					*tmp_string++ = (char) ic;
				if (--width) {
					ic = getc(stream);
					nrchars++;
				}
			}
			/* terminate the string */
			if (!(flags & FL_NOASSIGN))
				*tmp_string = '\0';	
			if (width) {
				if (ic != EOF) ungetc(ic,stream);
				nrchars--;
			}
			break;

		case '[':
		{	int old_nrchars;
			if (!(flags & FL_WIDTHSPEC))
				width = 0xffff;

			if (!width) {
				if (ic != EOF) ungetc(ic, stream);
				return done;
			}
			if ( *(++format) == '^' ) {
				reverse = 1;
				format++;
			} else
				reverse = 0;

			for (tmp_string = Xtable; tmp_string < &Xtable[NR_CHARS]
							; tmp_string++)
				*tmp_string = 0;

			if (*format == ']') Xtable[*format++] = 1;
			while (*format && *format != ']') {
				Xtable[*format++] = 1;
				if (*format == '-') {
					format++;
					if (*format
					    && *format != ']'
					    && *(format) >= *(format -2)) {
						int c;

						for( c = *(format -2) + 1
						    ; c <= *format ; c++)
							Xtable[c] = 1;
						format++;
					}
					else Xtable['-'] = 1;
				}
			}
			if (!*format) {
				if (ic != EOF) ungetc(ic,stream);
				return done;
			}
			
			old_nrchars = nrchars;
			if (!(flags & FL_NOASSIGN))
				tmp_string = va_arg(ap, char *);
			if (ic == EOF || !(Xtable[ic] ^ reverse)) {
				if (ic != EOF) ungetc(ic, stream);
				return done;
			}

			 while (width && ic != EOF && (Xtable[ic] ^ reverse)) {
				if (!(flags & FL_NOASSIGN))
					*tmp_string++ = (char) ic;
				if (--width) {
					ic = getc(stream);
					nrchars++;
				}
			}
			if (width) {
				if (ic != EOF) ungetc(ic, stream);
				nrchars--;
			}
			if (!(flags & FL_NOASSIGN)) {	/* terminate string */
				*tmp_string = '\0';	
				++done;
			}
			break;
		}
#ifndef	NOFLOAT:
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
			if (!(flags & FL_WIDTHSPEC)) width = NUMLEN;
			if (width > NUMLEN) width = NUMLEN;

			if (!width) return done;
			str = f_collect(ic, stream, width);
			if (str < inp_buf
			    || (str == inp_buf && *str == '-')) return done;
			nrchars += str - inp_buf + 1;
			if (!(flags & FL_NOASSIGN)) {
				ld_val = strtod(inp_buf, &tmp_string);
				if (flags & FL_LONGDOUBLE)
					*va_arg(ap, long double *) = (long double) ld_val;
				else
				    if (flags & FL_LONG)
					*va_arg(ap, double *) = (double) ld_val;
				else
					*va_arg(ap, float *) = (float) ld_val;
				done++;
			}
			break;
#endif
		}		/* end switch */
		++format;
	}
	return conv || (ic != EOF) ? done : EOF;
}
