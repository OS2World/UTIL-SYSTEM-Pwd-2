/*
 * pwd2_int.h
 *
 * Pwd/2 internal function header.
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

#ifndef _PWD2_INT_H
#define _PWD2_INT_H

#include <pwd.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _PW_BUF_LEN	256

FILE *	pwfopen(const char *);				/* open passwd file */

struct passwd *	fgetpwent_base(FILE *f);		/* basic fgetpwent() */
struct passwd *	getpwent_base(void);			/* basic getpwent() */
struct passwd *	getpwnam_base(const char *);		/* basic getpwnam() */
struct passwd *	getpwuid_base(uid_t);			/* basic getpwuid() */


#ifdef __cplusplus
}
#endif

#endif
