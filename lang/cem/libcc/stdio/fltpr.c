#ifndef NOFLOAT
extern char     *fcvt();
extern char     *ecvt();

char *
_pfloat(r,s,n,b)
	double r;
	register char *s;
{
	int sign,dp,i; char *s1;

	if (b == 0)
		n = 6;
	s1 = fcvt(r,n,&dp,&sign);
	if (sign)
		*s++ = '-';
	if (dp<=0)
		*s++ = '0';
	for (i=dp; i>0; i--)
		*s++ = *s1++;
	if ((i=n) > 0)
		*s++ = '.';
	while (++dp <= 0) {
		if (--i<0)
			break;
		*s++ = '0';
	}
	while (--i >= 0)
		*s++ = *s1++;
	return(s);
}

char *_pscien(r,s,n,b) float r; char *s; {
	int sign,dp; char *s1;

	if (b == 0)
		n = 7;
	else    n += 1;
	s1 = ecvt(r,n,&dp,&sign);
	if (sign)
		*s++ = '-';
	*s++ = *s1++;
	*s++ = '.';
	while (--n>0)
		*s++ = *s1++;
	*s++ = 'e';
	if ( r ) --dp ;
	if ( dp<0 ) {
		*s++ = '-' ; dp= -dp ;
	} else {
		*s++ = '+' ;
	}
	*s++ = '0' + (dp/10);
	*s++ = '0' + (dp%10);
	return(s);
}
#endif
