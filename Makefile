###############################################################################
#
# Makefile for OS/2 password file management.
#
# Copyright (c) 2003 Nicholas Paul Sheppard (nps@zeta.org.au)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

CC      = gcc
CFLAGS  = -O2
LD      = gcc
LDFLAGS = -Zcrtdll
LDLIBS  = -lcrypto
AR      = ar rs
A2OMF   = emxomf
ZIP     = zip -9 -u
PKG     = pwd2b1


###############################################################################
#
# Top-level targets
#

default: \
pwd2dos.a pwd2dos.lib pwd2unix.a pwd2unix.lib \
pw2add.exe pw2ch.exe pw2fn.exe pw2test.exe

package: $(PKG).zip

clean:
	if exist *.o del *.o

spotless: clean
	if exist *.a del *.a
	if exist *.lib del *.lib
	if exist *.exe del *.exe


###############################################################################
#
# Pattern rules
#

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.a:
	$(AR) $@ $^

%.lib: %.a
	$(A2OMF) $<

%.exe:
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.zip:
	$(ZIP) $@ $^


###############################################################################
#
# File dependencies
#

pwd2dos.a: pw2dos.o pw2base.o pw2fopen.o
pwd2unix.a: pw2unix.o pw2base.o pw2fopen.o

pwd2dos.lib: pwd2dos.a
pwd2unix.lib: pwd2unix.a


pw2add.exe: pw2add.o pw2fopen.o pwd2dos.a
pw2ch.exe: pw2ch.o pw2fopen.o pwd2dos.a
pw2fn.exe: pw2fn.c pwd2dos.a
pw2test.exe: pw2test.o pwd2dos.a

pw2add.o: pw2add.c pwd2_int.h
pw2base.o: pw2base.c pwd2_int.h
pw2ch.o: pw2ch.c pwd2_int.h
pw2dos.o: pw2dos.c pwd2_int.h
pw2fn.o: pw2fn.c
pw2fopen.o: pw2fopen.c pwd2_int.h
pw2test.o: pw2test.c
pw2unix.o: pw2unix.c pwd2_int.h

$(PKG).zip: \
readme.txt \
pwd2dos.a pwd2dos.lib pwd2unix.a pwd2unix.lib \
pw2ch.exe pw2add.exe pw2fn.exe pw2test.exe \
Makefile pwd2_int.h \
pw2add.c pw2base.c pw2ch.c pw2dos.c pw2fn.c pw2fopen.c pw2test.c pw2unix.c

