/*
 * pw2add.c
 *
 * Add user.
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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/des.h>
#include "pwd2_int.h"


int main(int argc, char *argv[])
{
	FILE *		f;		/* passwd file */
	struct passwd	pw;		/* new password structure */
	char		name[256];	/* user name */
	char		pwd[256];	/* password */
	char		gecos[256];	/* full name */
	char		dir[256];	/* home directory */
	char		shell[256];	/* shell */
	char		salt[3];	/* salt for DES_crypt() */
	char *		s;		/* working variable */
	struct passwd *	pw1;		/* working variable */

	/* initialise variables */
	memset(pw.pw_name = name, 0, sizeof(name));
	memset(pw.pw_passwd = pwd, 0, sizeof(pwd));
	memset(pw.pw_gecos = gecos, 0, sizeof(gecos));
	memset(pw.pw_dir = dir, 0, sizeof(dir));
	memset(pw.pw_shell = shell, 0, sizeof(shell));

	/* get user name */
	if (argc > 1) {
		strncpy(name, argv[1], sizeof(name) - 1);
	} else {
		printf("User name: ");
		fgets(name, sizeof(name), stdin);
		if ((s = strchr(name, '\n'))) 
			*s = '\0';
	}
	if (!*name) {
		return (0);
	}
	if (getpwnam(name)) {
		fprintf(stderr, "User %s already exists.\n", name);
		return (1);
	}

	/* get password */
	s = getpass("Enter password: ");
	if (*s) {
		strncpy(pwd, s, sizeof(pwd) - 1);
		s = getpass("Verify new password: ");
		if (strcmp(pwd, s)) {
			printf("Password not verified.\n");
			return (1);
		}
	} else {
		printf("You must enter a password.\n");
		return (1);
	}

	/* get full name */
	printf("Full name: ");
	fgets(gecos, sizeof(gecos), stdin);
	if ((s = strchr(gecos, '\n'))) 
		*s = '\0';

	/* get home directory */
	printf("Home directory: ");
	fgets(dir, sizeof(dir), stdin);
	if ((s = strchr(dir, '\n'))) 
		*s = '\0';

	/* get shell */
	printf("Shell [%s]: ", getenv("COMSPEC"));
	fgets(shell, sizeof(shell), stdin);
	if ((s = strchr(shell, '\n'))) 
		*s = '\0';
	if (!*shell)
		strncpy(shell, getenv("COMSPEC"), sizeof(shell) - 1);

	/* encrypt password */
	s = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
	srand(time(NULL));
	salt[0] = s[rand() % strlen(s)];
	salt[1] = s[rand() % strlen(s)];
	strcpy(pwd, DES_crypt(pwd, salt));

	/* find an unused user and group id */
	pw.pw_uid = 100;
	pw.pw_gid = 100;
	setpwent();
	while ((pw1 = getpwent())) {
		if (pw1->pw_uid > pw.pw_uid)
			pw.pw_uid = pw1->pw_uid;
		if (pw1->pw_gid > pw.pw_gid)
			pw.pw_gid = pw1->pw_gid;
	}
	pw.pw_uid++;
	pw.pw_gid++;
	endpwent();

	/* append the new entry to the passwd file */
	if (!(f = pwfopen("a"))) {
 		fprintf(stderr, "Could not open passwd.\n");
		return (1);
	}
	putpwent(&pw, f);
	fclose(f);

	return (0);
}
