/*
 * pw2unix.c
 *
 * Unix-format wrapper around pw2base.c
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

const char *	_pw_fputunix(const char *, FILE *);	/* write out Unix-style path */
char *		_pw_mkunix(char *);			/* convert to Unix-style path */


/*******************************************************************************
 *
 * These are the routines that are visible to applications via pwd.h.
 */

struct passwd *fgetpwent(FILE *f)
/*
 * getpwent() with Unix-style path names
 */
{
	struct passwd *	pw;	/* passwd entry */

	if ((pw = fgetpwent_base(f))) {
		_pw_mkunix(pw->pw_dir);
		_pw_mkunix(pw->pw_shell);
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
		_pw_mkunix(pw->pw_dir);
		_pw_mkunix(pw->pw_shell);
	}

	return (pw);
}


struct passwd *getpwnam(const char *name)
/*
 * getpwnam() with Unix-style path names
 */
{
	struct passwd *	pw;	/* passwd entry */

	if ((pw = getpwnam_base(name))) {
		_pw_mkunix(pw->pw_dir);
		_pw_mkunix(pw->pw_shell);
	}

	return (pw);
}


struct passwd *getpwuid(uid_t uid)
/*
 * getpwuid() with Unix-style path names]
 */
{
	struct passwd *	pw;	/* passwd entry */

	if ((pw = getpwuid_base(uid))) {
		_pw_mkunix(pw->pw_dir);
		_pw_mkunix(pw->pw_shell);
	}

	return (pw);
}


struct passwd *putpwent(struct passwd *pw, FILE *f)
/*
 * putpwent() with DOS-style path names
 */
{
	fprintf(f, "%s:%s:%d:%d:%s:", pw->pw_name, pw->pw_passwd, pw->pw_uid, pw->pw_gid, pw->pw_gecos);
	_pw_fputunix(pw->pw_dir, f);
	fputc(':', f);
	_pw_fputunix(pw->pw_shell, f);
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

const char *_pw_fputunix(const char *fname, FILE *f)
/*
 * Write out Unix-style path name.
 */
{
	fputs(fname, f);

	return (fname);
}


char *_pw_mkunix(char *fname)
/*
 * Convert to Unix format.
 */
{
	char *	pch;	/* counter */
	char	x;	/* drive letter */

	if (strncmp(fname, "/dev/", 5) == 0) {

		/* /dev/x format -- nothing to do */

	} else {

		/* determine drive letter */
		if (fname[0] == '$') {
			/* $x format */
			x = tolower(fname[1]);
		} else if (fname[0] && (fname[1] == ';')) {
			/* x; format */
			x = tolower(fname[0]);
		} else {
			/* no drive letter */
			x = 0;
		}

		/* convert slashes */
		if (x) {

			/* need to make space for /dev/x prefix */
			pch = fname + strlen(fname);
			if ((pch - fname) >= _PW_BUF_LEN)
				pch = fname + _PW_BUF_LEN - 1;
			while (pch > (fname + 1)) {
				*(pch + 4) = (*pch  == '\\')? '/' : *pch;
				pch--;
			}

			/* prepend /dev/x */
			fname[0] = '/';
			fname[1] = 'd';
			fname[2] = 'e';
			fname[3] = 'v';
			fname[4] = '/';
			fname[5] = x;
			fname[6] = '/';

		} else {
			/* just convert slashes */
			pch = fname;
			while (*pch) {
				if (*pch == '\\')
					*pch = '/';
				pch++;
			}
		}

	}

	return (fname);
}
