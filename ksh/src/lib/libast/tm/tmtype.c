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
#pragma prototyped
/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * time conversion support
 */

#include <ast.h>
#include <tm.h>

/*
 * return the tm_data.zone[] time zone entry for type s
 *
 * if e is non-null then it will point to the first
 * unmatched char in s
 *
 * 0 returned for no match
 */

Tm_zone_t*
tmtype(register const char* s, char** e)
{
	register Tm_zone_t*	zp;
	register char*		t;

	tmset(tm_info.zone);
	zp = tm_info.local;
	do
	{
		if ((t = zp->type) && tmword(s, e, t, NiL, 0)) return(zp);
		if (zp == tm_info.local) zp = tm_data.zone;
		else zp++;
	} while (zp->standard);
	return(0);
}
