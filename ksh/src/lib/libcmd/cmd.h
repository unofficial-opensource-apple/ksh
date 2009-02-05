/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*           Copyright (c) 1992-2007 AT&T Knowledge Ventures            *
*                      and is licensed under the                       *
*                  Common Public License, Version 1.0                  *
*                      by AT&T Knowledge Ventures                      *
*                                                                      *
*                A copy of the License is available at                 *
*            http://www.opensource.org/licenses/cpl1.0.txt             *
*         (with md5 checksum 059e8cd6165cb4c31e351f2b69388fd9)         *
*                                                                      *
*              Information and Software Systems Research               *
*                            AT&T Research                             *
*                           Florham Park NJ                            *
*                                                                      *
*                 Glenn Fowler <gsf@research.att.com>                  *
*                  David Korn <dgk@research.att.com>                   *
*                                                                      *
***********************************************************************/
#pragma prototyped
/*
 * AT&T Research
 *
 * builtin cmd definitions
 */

#ifndef _CMD_H
#define _CMD_H

#include <ast.h>
#include <error.h>
#include <stak.h>
#include <shcmd.h>

#define cmdinit			_cmd_init

#if _BLD_cmd && defined(__EXPORT__)
#define extern		__EXPORT__
#endif

#include <cmdext.h>

#undef	extern

#if defined(CMD_BUILTIN) && !defined(CMD_STANDALONE)
#define CMD_STANDALONE	CMD_BUILTIN
#endif

#ifdef CMD_STANDALONE

#if CMD_DYNAMIC

#include <dlldefs.h>

typedef int (*Shbltin_f)(int, char**, void*);

#else

extern int CMD_STANDALONE(int, char**, void*);

#endif

#ifndef CMD_BUILTIN

/*
 * command initialization
 */

static int
cmdinit(int argc, register char** argv, void* context, const char* catalog, int flags)
{
	register char*	cp;
	register char*	pp;

	if (cp = strrchr(argv[0], '/'))
		cp++;
	else
		cp = argv[0];
	if (pp = strrchr(cp, '_'))
		cp = pp + 1;
	error_info.id = cp;
	if (!error_info.catalog)
		error_info.catalog = (char*)catalog;
	opt_info.index = 0;
	if (context)
		error_info.flags |= flags;
	return 0;
}

#endif

int
main(int argc, char** argv)
{
#if CMD_DYNAMIC
	register char*	s;
	register char*	t;
	void*		dll;
	Shbltin_f	fun;
	char		buf[64];

	if (s = strrchr(argv[0], '/'))
		s++;
	else if (!(s = argv[0]))
		return 127;
	if ((t = strrchr(s, '_')) && *++t)
		s = t;
	buf[0] = '_';
	buf[1] = 'b';
	buf[2] = '_';
	strncpy(buf + 3, s, sizeof(buf) - 4);
	buf[sizeof(buf) - 1] = 0;
	if (t = strchr(buf, '.'))
		*t = 0;
	for (;;)
	{
		if (dll = dlopen(NiL, RTLD_LAZY))
		{
			if (fun = (Shbltin_f)dlsym(dll, buf + 1))
				break;
			if (fun = (Shbltin_f)dlsym(dll, buf))
				break;
		}
		if (dll = dllfind("cmd", NiL, RTLD_LAZY))
		{
			if (fun = (Shbltin_f)dlsym(dll, buf + 1))
				break;
			if (fun = (Shbltin_f)dlsym(dll, buf))
				break;
		}
		return 127;
	}
	return (*fun)(argc, argv, NiL);
#else
	return CMD_STANDALONE(argc, argv, NiL);
#endif
}

#else

#define _CMD_CONTEXT_OK(p)	(((Shbltin_t*)(p))->version>=20070511&&((Shbltin_t*)(p))->version<20350101)
#undef	cmdinit
#ifdef _MSC_VER
#define cmdinit(a,b,c,d,e)	do{if(_cmd_init(a,b,c,d,e))return -1;}while(0)
#else
#define cmdinit(a,b,c,d,e)	do{if((c)&&!_CMD_CONTEXT_OK(c))c=0;if(_cmd_init(a,b,c,d,e))return -1;}while(0)
#endif

#if _BLD_cmd && defined(__EXPORT__)
#define extern			extern __EXPORT__
#endif

extern int	_cmd_init(int, char**, void*, const char*, int);

#undef	extern

#endif

#endif