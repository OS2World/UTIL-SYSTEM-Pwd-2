/*
 * pw2fn.c
 *
 * List a password entry.
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

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
	char *		name;	/* user name */
	struct passwd *	pw;	/* password entry */
	char *		s;	/* working variable */

	/* get user name */
	if (argc < 2) {
		/* get user from environment */
		if (!(name = getenv("USER")))
			name = "root";
	} else {
		/* get user name from argument list */
		name = argv[1];
	}

	/* get password entry */
	if (!(pw = getpwnam(name))) {
		fprintf(stderr, "User %s does not exist.\n", name);
		return (1);
	}

	printf("          User: %s\n", pw->pw_name);
	printf("       User Id: %d\n", pw->pw_uid);
	printf("      Group Id: %d\n", pw->pw_gid);
	printf("     Full name: %s\n", pw->pw_gecos);
	printf("Home directory: %s\n", pw->pw_dir);
	printf("         Shell: %s\n", pw->pw_shell);

	return (0);
}
