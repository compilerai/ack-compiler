/*
 * (c) copyright 1987 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Copyright".
 */
/* $Id$ */

#include <string.h>
#include <ack_string.h>
#include <system.h>
#include "print.h"

static int
integral(int c)
{
	switch (c) {
	case 'b':
		return -2;
	case 'd':
		return 10;
	case 'o':
		return -8;
	case 'u':
		return -10;
	case 'x':
		return -16;
	}
	return 0;
}

/*FORMAT1 $
	%s = char *
	%l = long
	%c = int
	%[uxbo] = unsigned int
	%d = int
$ */
int
_format(char *buf, const char *fmt, va_list argp)
{
	const char *pf = fmt;
	char *pb = buf;

	while (*pf) {
		if (*pf == '%') {
			int width, base, pad, npad;
			char *arg;
			char cbuf[2];
			char *badformat = "<bad format>";
			
			/* get padder */
			if (*++pf == '0') {
				pad = '0';
				++pf;
			}
			else
				pad = ' ';
			
			/* get width */
			width = 0;
			while (*pf >= '0' && *pf <= '9')
				width = 10 * width + *pf++ - '0';
			
			if (*pf == 's') {
				arg = va_arg(argp, char *);
			}
			else
			if (*pf == 'c') {
				cbuf[0] = va_arg(argp, int);
				cbuf[1] = '\0';
				arg = &cbuf[0];
			}
			else
			if (*pf == 'l') {
				/* alignment ??? */
				if (base = integral(*++pf)) {
					arg = long2str(va_arg(argp,long), base);
				}
				else {
					pf--;
					arg = badformat;
				}
			}
			else
			if (base = integral(*pf)) {
				arg = long2str((long)va_arg(argp,int), base);
			}
			else
			if (*pf == '%')
				arg = "%";
			else
				arg = badformat;

			npad = width - strlen(arg);

			while (npad-- > 0)
				*pb++ = pad;
			
			while (*pb++ = *arg++);
			pb--;
			pf++;
		}
		else
			*pb++ = *pf++;
	}
	return pb - buf;
}
