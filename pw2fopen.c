/*
 * pw2fopen.c
 *
 * Open the passwd file.
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

#define INCL_DOSMISC
#include <os2.h>
#include <stdio.h>
#include "pwd2_int.h"


FILE *pwfopen(const char *mode)
/*
 * Open the passwd file. In Unix, this file is always /etc/passwd. OS/2
 * installations have a variety of possible ETC directories. We try
 *
 *   1. x:\etc\passwd
 *   2. x:\mptn\etc\passwd
 *   3. x:\tcpip\etc\passwd
 *
 * where x: is the boot drive. If none of these can be opened, fail.
 */
{
	static	int	_pwfname_inited = 0;		/* initialised? */
	static	char	_pwfname_buf[CCHMAXPATH];	/* passwd file name */
		ULONG	ul;				/* buffer for DosQuerySysInfo() */
		FILE *	f;				/* return value */

	if (!_pwfname_inited) {

		/* determine boot drive */
		DosQuerySysInfo(QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ul, sizeof(ul));

		/* try x:\etc\passwd */
		sprintf(_pwfname_buf, "%c:\\etc\\passwd", 'a' + ul - 1);
		if ((f = fopen(_pwfname_buf, mode))) {
			_pwfname_inited = 1;
			return (f);
		}

		/* try x:\mptn\etc\passwd */
		sprintf(_pwfname_buf, "%c:\\mptn\\etc\\passwd", 'a' + ul - 1);
		if ((f = fopen(_pwfname_buf, mode))) {
			_pwfname_inited = 1;
			return (f);
		}

		/* try x:\tcpip\etc\passwd */
		sprintf(_pwfname_buf, "%c:\\tcpip\\etc\\passwd", 'a' + ul - 1);
		if ((f = fopen(_pwfname_buf, mode))) {
			_pwfname_inited = 1;
			return (f);
		}

	} else {
		return (fopen(_pwfname_buf, mode));
	}
}
