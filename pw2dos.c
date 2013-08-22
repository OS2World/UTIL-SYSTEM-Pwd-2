/*
 * pw2dos.c
 *
 * Dos-format wrapper around pw2base.c
 *
 * Copyright (c) 2003 Nicholas Paul Sheppard (nps@zeta.org.au)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include "pwd2_int.h"


/*******************************************************************************
 *
 * Internal function prototypes
 */

const char *	_pw_fputdos(const char *, FILE *);	/* write out DOS-style path */
char *		_pw_mkdos(char *);			/* convert to DOS-style path */


/*******************************************************************************
 *
 * These are the routines that are visible to applications via pwd.h.
 */

struct passwd *fgetpwent(FILE *f)
/*
 * getpwent() with DOS-style path names
 */
{
	struct passwd *	pw;	/* passwd entry */

	if ((pw = fgetpwent_base(f))) {
		_pw_mkdos(pw->pw_dir);
		_pw_mkdos(pw->pw_shell);
	}

	return (pw);
}


struct passwd *getpwent(void)
/*
 * getpwent() with DOS-style path names
 */
{
	struct passwd *	pw;	/* passwd entry */

	if ((pw = getpwent_base())) {
		_pw_mkdos(pw->pw_dir);
		_pw_mkdos(pw->pw_shell);
	}

	return (pw);
}


struct passwd *getpwnam(const char *name)
/*
 * getpwnam() with DOS-style path names
 */
{
	struct passwd *	pw;	/* passwd entry */

	if ((pw = getpwnam_base(name))) {
		_pw_mkdos(pw->pw_dir);
		_pw_mkdos(pw->pw_shell);
	}

	return (pw);
}


struct passwd *getpwuid(uid_t uid)
/*
 * getpwuid() with DOS-style path names
 */
{
	struct passwd *	pw;	/* passwd entry */

	if ((pw = getpwuid_base(uid))) {
		_pw_mkdos(pw->pw_dir);
		_pw_mkdos(pw->pw_shell);
	}

	return (pw);
}


struct passwd *putpwent(struct passwd *pw, FILE *f)
/*
 * putpwent() with DOS-style path names
 */
{
	fprintf(f, "%s:%s:%d:%d:%s:", pw->pw_name, pw->pw_passwd, pw->pw_uid, pw->pw_gid, pw->pw_gecos);
	_pw_fputdos(pw->pw_dir, f);
	fputc(':', f);
	_pw_fputdos(pw->pw_shell, f);
	fputc('\n', f);

	return (pw);
}


/*******************************************************************************
 *
 * Path name conversion routines. We know of three formats used in OS/2 passwd
 * files, each dealing with drive letters in a different way:
 *
 *  1. The /dev/x format (from discussion in UnixOS/2 community)
 *  2. The $x format (from Andy Zabalotny)
 *  3. The x; format (from Kai Uwe Rommel)
 *
 * These routines will convert any of these formats into normal path names.
 * The fname buffer is assumed to be at least _PW_BUF_LEN bytes long; this is
 * okay since these functions are only ever called on the static buffers
 * declared in this file.
 */

const char *_pw_fputdos(const char *fname, FILE *f)
/*
 * Write out DOS-style path name.
 */
{
	const char *	pch;	/* counter */

	if (fname[0] && (fname[1] == ':')) {
		/* write out drive name */
		fprintf(f, "/dev/%c", fname[0]);
		pch = fname + 2;
	} else {
		pch = fname;
	}

	/* convert slashes */
	while (*pch) {
		putc((*pch == '\\')? '/' : *pch, f);
		pch++;
	}

	return (fname);
}


char *_pw_mkdos(char *fname)
/*
 * Convert to DOS format.
 */
{
	char *	pch;	/* counter */

	if (strncmp(fname, "/dev/", 5) == 0) {

		/* /dev/x format -- prepend drive letter and convert slashes */
		fname[0] = fname[5];
		fname[1] = ':';
		pch = fname + 6;
		do {
			*(pch - 4) = (*pch == '/')? '\\' : *pch;
			pch++;
		} while (*pch);
		*(pch - 4) = 0;

	} else {

		/* fix up drive letter */
		if (fname[0] == '$') {
			/* $x format */
			fname[0] = fname[1];
			fname[1] = ':';
		} else if (fname[0] && (fname[1] == ';')) {
			/* x; format */
			fname[1] = ':';
		}

		/* convert slashes */
		pch = fname;
		do {
			if (*pch == '/')
				*pch = '\\';
			pch++;
		} while (*pch);
	}

	return (fname);
}
