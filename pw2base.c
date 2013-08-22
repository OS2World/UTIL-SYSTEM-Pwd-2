/*
 * pw2base.c
 *
 * Basic password entry manipulation.
 *
 * Copyright (c) 2003, 2007 Nicholas Paul Sheppard (nps@zeta.org.au)
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

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include "pwd2_int.h"

/*******************************************************************************
 *
 * Internal function prototypes
 */

/*******************************************************************************
 *
 * Static buffers. These buffers will be over-written and returned by every
 * call to getpwnam() or getpwuid().
 */

char		_pw_buf_name[_PW_BUF_LEN];
char		_pw_buf_passwd[_PW_BUF_LEN];
int		_pw_buf_uid;
int		_pw_buf_gid;
char		_pw_buf_age[_PW_BUF_LEN];
char		_pw_buf_comment[_PW_BUF_LEN];
char		_pw_buf_gecos[_PW_BUF_LEN];
char		_pw_buf_dir[_PW_BUF_LEN];
char		_pw_buf_shell[_PW_BUF_LEN];

struct passwd	_pw_buf;


FILE *		_pw_file = NULL;


/*******************************************************************************
 *
 * Basic passwd parsing routines. These do most of the real work.
 */

struct passwd *fgetpwent_base(FILE *f)
/*
 * Get the next entry in a file stream. Return NULL if finished or parse
 * error.
 */
{
	int	ch;	/* buffer */
	char *	pch;	/* counter */

	/* initialise buffer */
	_pw_buf_name[0] = 0;
	_pw_buf_passwd[0] = 0;
	_pw_buf_uid = 0;
	_pw_buf_gid = 0;
	_pw_buf_age[0] = 0;
	_pw_buf_comment[0] = 0;
	_pw_buf_gecos[0] = 0;
	_pw_buf_dir[0] = 0;
	_pw_buf_shell[0] = 0;

	/* skip whitespace */
	while (((ch = getc(f)) != EOF) && isspace(ch))
		;

	/* get user name */
	if (ch != EOF) {
		ungetc(ch, f);
		pch = _pw_buf_name;
		while (((ch = getc(f)) != EOF) && (ch != ':') && (ch != '\r') && (ch != '\n') &&
				((pch - _pw_buf_name) < (sizeof(_pw_buf_name) - 1))) {
			*pch = ch;
			pch++;
		}
		*pch = 0;
	} else {
		return (NULL);
	}

	/* get password */
	if (ch == ':') {
		pch = _pw_buf_passwd;
		while (((ch = getc(f)) != EOF) && (ch != ':') && (ch != '\r') && (ch != '\n') &&
				((pch - _pw_buf_passwd) < (sizeof(_pw_buf_passwd) - 1))) {
			*pch = ch;
			pch++;
		}
		*pch = 0;
	} else {
		return (NULL);
	}

	/* get user id */
	if (ch == ':') {
		_pw_buf_uid = 0;
		while (((ch = getc(f)) != EOF) && isdigit(ch))
			_pw_buf_uid = _pw_buf_uid * 10 + ch - '0';
	} else {
		return (NULL);
	}

	/* get group id */
	if (ch == ':') {
		_pw_buf_gid = 0;
		while (((ch = getc(f)) != EOF) && isdigit(ch))
			_pw_buf_gid = _pw_buf_gid * 10 + ch - '0';
	} else {
		return (NULL);
	}

	/* get full name */
	if (ch == ':') {
		pch = _pw_buf_gecos;
		while (((ch = getc(f)) != EOF) && (ch != ':') && (ch != '\r') && (ch != '\n') &&
				((pch - _pw_buf_gecos) < (sizeof(_pw_buf_gecos) - 1))) {
			*pch = ch;
			pch++;
		}
		*pch = 0;
	} else {
		return (NULL);
	}

	/* get age */
	if (ch == ':') {
		pch = _pw_buf_age;
		while (((ch = getc(f)) != EOF) && (ch != ':') && (ch != '\r') && (ch != '\n') &&
				((pch - _pw_buf_age) < (sizeof(_pw_buf_age) - 1))) {
			*pch = ch;
			pch++;
		}
		*pch = 0;
	} else {
		return (NULL);
	}

	/* get comment */
	if (ch == ':') {
		pch = _pw_buf_comment;
		while (((ch = getc(f)) != EOF) && (ch != ':') && (ch != '\r') && (ch != '\n') &&
				((pch - _pw_buf_comment) < (sizeof(_pw_buf_comment) - 1))) {
			*pch = ch;
			pch++;
		}
		*pch = 0;
	} else {
		return (NULL);
	}

	if (ch == ':') {
		/* get home directory */
		pch = _pw_buf_dir;
		while (((ch = getc(f)) != EOF) && (ch != ':') && (ch != '\r') && (ch != '\n') &&
				((pch - _pw_buf_dir) < (sizeof(_pw_buf_dir) - 1))) {
			*pch = ch;
			pch++;
		}
		*pch = 0;

		/* get shell */
		if (ch == ':') {
			pch = _pw_buf_shell;
			while (((ch = getc(f)) != EOF) && (ch != ':') && (ch != '\r') && (ch != '\n') &&
					((pch - _pw_buf_shell) < (sizeof(_pw_buf_shell) - 1))) {
				*pch = ch;
				pch++;
			}
			*pch = 0;
		} else {
			return (NULL);
		}
	} else {
		/* no age and comment fields; they were actually dir and shell */
		strcpy(_pw_buf_dir, _pw_buf_age);
		strcpy(_pw_buf_shell, _pw_buf_comment);
		_pw_buf_age[0] = 0;
		_pw_buf_comment[0] = 0;
	}


	/* skip to end of line */
	if ((ch != EOF) && (ch != '\r') && (ch != '\n')) {
		while (((ch = getc(f)) != EOF) && (ch != '\r') && (ch != '\n'));
			;
	}

	/* set up the password structure for whichever version of pwd.h we are using */
	_pw_buf.pw_name = _pw_buf_name;
	_pw_buf.pw_passwd = _pw_buf_passwd;
	_pw_buf.pw_uid = _pw_buf_uid;
	_pw_buf.pw_gid = _pw_buf_gid;
#ifndef __INNOTEK_LIBC__
	_pw_buf.pw_age = _pw_buf_age;
	_pw_buf.pw_comment = _pw_buf_comment;
#else
	_pw_buf.pw_change = atoi(_pw_buf_age);
	_pw_buf.pw_class = _pw_buf_comment;
#endif
	_pw_buf.pw_gecos = _pw_buf_gecos;
	_pw_buf.pw_dir = _pw_buf_dir;
	_pw_buf.pw_shell = _pw_buf_shell;
#ifdef __INNOTEK_LIBC__
	_pw_buf.pw_expire = 0;
	_pw_buf.pw_fields = 0;
#endif

	return (&_pw_buf);
}


struct passwd *getpwent_base(void)
/*
 * Basic getpwent().
 */
{
	if (!_pw_file) {
		/* open file */
		setpwent();
		if (!_pw_file)
			return (NULL);
	}

	return (fgetpwent_base(_pw_file));
}


struct passwd *getpwnam_base(const char *name)
/*
 * Basic getpwnam(). This locates and returns the unprocessed passwd entry.
 */
{
	struct passwd *	pw;	/* return value */

	setpwent();
	while (((pw = getpwent_base()) != NULL) && strcmp(pw->pw_name, name))
		;
	endpwent();

	return (pw);
}


struct passwd *getpwuid_base(uid_t uid)
/*
 * Basic getpwuid(). This locates and returns the unprocessed passwd entry.
 */
{
	struct passwd *	pw;	/* return value */

	setpwent();
	while (((pw = getpwent_base()) != NULL) && (pw->pw_uid != uid))
		;
	endpwent();

	return (pw);
}


void setpwent(void)
/*
 * Open or rewind the passwd file.
 */
{
	if (_pw_file) {
		/* file is already open */
		rewind(_pw_file);
	} else {
		/* make pwfopen() do the work */
		_pw_file = pwfopen("r");
	}
}


void endpwent(void)
/*
 * Close the passwd file.
 */
{
	if (_pw_file) {
		fclose(_pw_file);
		_pw_file = NULL;
	}
}
