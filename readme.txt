===============================================================================
                                     Pwd/2 
                          A /etc/passwd emulator for OS/2
                                     Beta 1
===============================================================================

Introduction
------------

This package is designed to emulate the functionality of <pwd.h> on Unix
systems, to provide basic multiple user authentication for OS/2, and to
ease porting of Unix applications that require /etc/passwd. The functions
in this library are intended to be used in place of the dummy functions
defined in the EMX run-time libraries.

This is a beta version. Bugs and comments, etc., can be submitted to me.
Up-to-date information about Pwd/2 can be found at

    http://www.zeta.org.au/~nps/software/pwd2/en/index.html


Installation
------------

The libraries in this file will work with the <pwd.h> header distributed with
EMX, or the one distributed with Posix/2, and undoubtedly many other ones, so
you should have a <pwd.h> already.

The functions are implemented in pwd2dos.* (DOS-path versions) and pwd2unix.*
(Unix-path versions). Copy these into your EMX `lib' directory.

Copy the pw2add.exe, pw2ch.exe pw2fn.exe and pw2test.exe files into a
directory on your PATH. These utilities can be used for adding users,
changing passwords and testing passwords, respectively.


Developing Applications with Pwd/2
----------------------------------

In one line: #include <pwd.h> and link with

 * pwd2dos.a or pwd2dos.lib, if you want DOS-style paths (e.g. C:\OS2\CMD.EXE)
 * pwd2unix.a or pwd2unix.lib, if you want Unix-style paths (e.g. /bin/sh.exe)

Not all the functions in EMX or Posix/2 <pwd.h> are implemented by Pwd/2. Pwd/2
implements the following functions which have their normal Unix/Posix
behaviour:

void		setpwent(void);
void		endpwent(void);
struct passwd *	fgetpwent(FILE *);
struct passwd *	getpwent(void);
struct passwd *	getpwnam(const char *);
struct passwd *	getpwuid(uid_t);
struct passwd *	putpwent(struct passwd *, FILE *);

The passwd structure has a slightly different definition in the EMX headers
than it does in the Innotek ones. Pwd/2 will work with both.

If you are using the EMX or Posix/2 headers, the passwd structure is:

	struct passwd {
		char *	pw_name;	/* user name */
		char *	pw_passwd;	/* encrypted password */
		uid_t	pw_uid;		/* user id */
		gid_t	pw_gid;		/* group id */
		char *	pw_age;		/* not used */
		char *	pw_comment;	/* not used */
		char *	pw_gecos;	/* full name */
		char *	pw_dir;		/* home directory */
		char *	pw_shell;	/* user shell */
	};


If you are using the Innotek headers, the passwd structure is:

	struct passwd {
		char	*pw_name;		/* user name */
		char	*pw_passwd;		/* encrypted password */
		uid_t	pw_uid;			/* user id */
		gid_t	pw_gid;			/* user id */
		time_t	pw_change;		/* password change time */
		char	*pw_class;		/* not used */
		char	*pw_gecos;		/* full name */
		char	*pw_dir;		/* home directory */
		char	*pw_shell;		/* default shell */
		time_t	pw_expire;		/* not used */
		int	pw_fields;		/* not used */
	};



To test a password string `pwd' given a pointer to the password entry `pw', do

	if (strcmp(pw->pw_passwd, crypt(pwd, pw->pw_passwd)) == 0) {
		/* password is correct */
	} else {
		/* password is not correct */
	}

See pw2add.c, pw2ch.c, pw2fn.c and pw2test.c for further examples.


User Management
---------------

Pwd/2 is supplied with four simple utilities for user management:

    pw2add [name]

    Add a user with username `name'. If `name' is not on the command line,
    prompt for one. All the other entries will be prompted for.

    pw2ch [name]

    Change the password for a user, clone of the Unix `passwd' command. If
    `name' is not on the command line, use the value of the USER environment
    variable. If USER does not exist, change the password for root.

    pw2fn [name]

    List the password entry for a user (a la `finger'). If `name' is not on
    the command line, use the value of the USER environment variable. If USER
    does not exist, list the entry for root.

    pw2test [name]

    Test the password for a user. If `name' is not on the command line, use
    the value of the USER environment variable. If USER does not exist,
    test the password for root.


The /etc/passwd File
--------------------

Unlike Unix, OS/2 does not have any fixed location for its ETC directory;
most OS/2 users have an ETC environment variable that points to this
directory. However, using an environment variable to locate passwords is
a potential security hole since a malicious user may be able to set ETC
to point to his or own directory. Therefore Pwd/2 searches for the passwd
file in the following order:

  1. x:\etc\passwd
  2. x:\mptn\etc\passwd
  3. x:\tcpip\etc\passwd

where `x' is the boot drive.

The format of each line of the passwd file is as follows:

<pw_name>:<pw_passwd>:<pw_uid>:<pw_gid>:<pw_gecos>:<pw_dir>:<pw_shell>

The <pw_passwd> entry is the output of crypt() when called on the plaintext
password.

Unfortunately, several different methods of dealing with drive letters in the
<pw_dir> and <pw_shell> fields have arisen, since the colon following drive
letter would be confused with a colon separating the fields. Pwd/2 can
interpret three formats:

 * Kai Uwe Rommel's format in which the colon is replaced by a semi-colon;

 * Andy Zabalotny's format in which the drive letter is prefixed by a
   dollar sign and the colon deleted; and

 * Pwd/2's format in which drives are represented by the `/dev/x' directory,
   where x is the drive letter.

All new entries will be written in the Pwd/2 format; this format was chosen
after discussion on the UnixOS/2 list and recognition that both of the older
formats are troublesome in shell scripts because they introduce characters
with special meaning to the shell. Slashes in both directions will be
converted to the appropriate directory separator.

Also, some passwd files may have the <pw_age> and <pw_comment> fields between
<pw_gecos> and <pw_dir>. Pwd/2 can detect this, and copies their contents into
the `pw_age' and `pw_comment' fields of the password structure.


Legal Stuff
-----------

The executables in this product include software developed by the OpenSSL
Project for use in the OpenSSL Toolkit (http://www.openssl.org/), and 
also cryptographic software written by Eric Young (eay@cryptsoft.com).


Everything else is (c) 2003, 2007 Nicholas Paul Sheppard (nps@zeta.org.au)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


Changes
-------

 - since alpha 3

   * added support for Innotek's <pwd.h>

 - since alpha 2

   * fixed pw2ch.exe

 - since alpha 1

   * pw_age and pw_comment fields added

   * now uses EMX or Posix/2 <pwd.h> instead of home-grown version

   * library split into DOS-format and Unix-format versions

   * GNU ultra-fast crypt() replace by OpenSSL DES_crypt()
