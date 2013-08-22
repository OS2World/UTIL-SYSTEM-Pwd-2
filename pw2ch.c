/*
 * pw2ch.c
 *
 * Emulation of Unix `passwd' command.
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

#include <ctype.h>
#include <errno.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <openssl/des.h>
#include "pwd2_int.h"


int main(int argc, char *argv[])
{
	FILE *		fpw;		/* passwd file */
	char *		name;		/* user name */
	struct passwd *	pw;		/* passwd entry */
	char		salt[3];	/* salt for DES_crypt() */
	char		pwd[128];	/* password */
	char		line[256];	/* line of file */
	char *		s;		/* working variable */
	char		tmp[L_tmpnam];	/* temporary file name */
	FILE *		ftmp;		/* temporary file */

	/* get user name */
	if (argc < 2) {
		/* get user from environment */
		if (!(name = getenv("USER")))
			name = "root";
	} else {
		/* get user name from argument list */
		name = argv[1];
	}

	if (!(pw = getpwnam(name))) {
		fprintf(stderr, "User %s does not exist.\n", name);
		return (1);
	} else {
		printf("Changing password for %s.\n", name);
	}

	/* check old password */
	s = getpass("Old password: ");
	if ((strcmp(pw->pw_passwd, DES_crypt(s, pw->pw_passwd)))) {
		printf("Invalid password.");
		return (1);
	}

	/* get new password */
	s = getpass("New password: ");
	if (*s) {
		strcpy(pwd, s);
		s = getpass("Verify new password: ");
		if (strcmp(pwd, s)) {
			printf("Password not verified.\n");
			return (1);
		}
	} else {
		printf("Password not changed.\n");
		return (1);
	}

	/* get salt */
	s = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
	srand(time(NULL));
	salt[0] = s[rand() % strlen(s)];
	salt[1] = s[rand() % strlen(s)];

	/* open passwd file */
	if (!(fpw = pwfopen("r"))) {
		fprintf(stderr, "Could not open passwd.\n");
		return (1);
	}

	/* get a temporary file */
	tmpnam(tmp);
	if (!(ftmp = fopen(tmp, "w"))) {
		fprintf(stderr, "Could not open temporary file.\n");
		fclose(fpw);
		return (1);
	}

	/* search for the entry we want */
	while (fgets(line, sizeof(line), fpw)) {
		if ((s = strchr(line, ':'))) {
			*s = '\0';
			if (strcmp(name, line) == 0) {
				/* found it; replace password */
				*s = ':';
				s++;
				strcpy(s, DES_crypt(pwd, salt));
				s = strchr(s, '\0');
			}
			*s = ':';
		}

		/* copy the line to the temp file */
		fputs(line, ftmp);
	}

	/* close files */
	fclose(fpw);
	fclose(ftmp);

	/* copy temp file to password file */
	if (!(fpw = pwfopen("w"))) {
		fprintf(stderr, "Could not write to passwd.\n");
		return (1);
	}
	if (!(ftmp = fopen(tmp, "r"))) {
		fprintf(stderr, "Could not re-open temporary file.\n");
		fprintf(stderr, "Passwords have been moved to %s\n", tmp);
		fclose(fpw);
		return (1);
	}
	while (fgets(line, sizeof(line), ftmp)) {
		fputs(line, fpw);
	}
	fclose(fpw);
	fclose(ftmp);
	remove(tmp);

	printf("Password changed.\n");

	return (0);
}
