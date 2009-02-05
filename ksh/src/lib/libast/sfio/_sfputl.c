/*******************************************************************
*                                                                  *
*             This software is part of the ast package             *
*                Copyright (c) 1985-2004 AT&T Corp.                *
*        and it may only be used by you under license from         *
*                       AT&T Corp. ("AT&T")                        *
*         A copy of the Source Code Agreement is available         *
*                at the AT&T Internet web site URL                 *
*                                                                  *
*       http://www.research.att.com/sw/license/ast-open.html       *
*                                                                  *
*    If you have copied or used this software without agreeing     *
*        to the terms of the license you are infringing on         *
*           the license and copyright and are violating            *
*               AT&T's intellectual property rights.               *
*                                                                  *
*            Information and Software Systems Research             *
*                        AT&T Labs Research                        *
*                         Florham Park NJ                          *
*                                                                  *
*               Glenn Fowler <gsf@research.att.com>                *
*                David Korn <dgk@research.att.com>                 *
*                 Phong Vo <kpv@research.att.com>                  *
*                                                                  *
*******************************************************************/
#include	"sfhdr.h"

/*	Write out a long value in a portable format
**
**	Written by Kiem-Phong Vo.
*/

#if __STD_C
int _sfputl(reg Sfio_t* f, Sflong_t v)
#else
int _sfputl(f,v)
reg Sfio_t*	f;	/* write a portable long to this stream */
Sflong_t	v;	/* the value to be written */
#endif
{
#define N_ARRAY		(2*sizeof(Sflong_t))
	reg uchar	*s, *ps;
	reg ssize_t	n, p;
	uchar		c[N_ARRAY];

	SFMTXSTART(f,-1);
	if(f->mode != SF_WRITE && _sfmode(f,SF_WRITE,0) < 0)
		SFMTXRETURN(f, -1);
	SFLOCK(f,0);

	s = ps = &(c[N_ARRAY-1]);
	if(v < 0)
	{	/* add 1 to avoid 2-complement problems with -SF_MAXINT */
		v = -(v+1);
		*s = (uchar)(SFSVALUE(v) | SF_SIGN);
	}
	else	*s = (uchar)(SFSVALUE(v));
	v = (Sfulong_t)v >> SF_SBITS;

	while(v > 0)
	{	*--s = (uchar)(SFUVALUE(v) | SF_MORE);
		v = (Sfulong_t)v >> SF_UBITS;
	}
	n = (ps-s)+1;

	if(n > 8 || SFWPEEK(f,ps,p) < n)
		n = SFWRITE(f,(Void_t*)s,n); /* write the hard way */
	else
	{	switch(n)
		{
		case 8 : *ps++ = *s++;
		case 7 : *ps++ = *s++;
		case 6 : *ps++ = *s++;
		case 5 : *ps++ = *s++;
		case 4 : *ps++ = *s++;
		case 3 : *ps++ = *s++;
		case 2 : *ps++ = *s++;
		case 1 : *ps++ = *s++;
		}
		f->next = ps;
	}

	SFOPEN(f,0);
	SFMTXRETURN(f, n);
}
