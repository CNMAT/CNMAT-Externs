/*
Copyright (c) 2008.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by John MacCallum, The Center for New Music and
Audio Technologies, University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.
 */

#include "cmmjl_errno.h"
#include "cmmjl_error.h"

void cmmjl_default_error_handler(const char *objname, 
				 const char *filename, 
				 const char *function, 
				 int line, 
				 int code, 
				 char *st)
{
	error("%s: %s: %s: %d: %s (%d)", objname, filename, function, line, st, code);
}

void cmmjl_no_error_handler(const char *objname,
			    const char *filename,
			    const char *function,
			    int line,
			    int code,
			    char *str)
{}

void cmmjl_error(const char *objname, 
		 const char *filename, 
		 const char *function, 
		 int line, 
		 int code, 
		 const char *st,
		 ...)
{
	va_list ap;
	va_start(ap, st);
	char buf[256];
	vsprintf(buf, st, ap);
	va_end(ap);
	cmmjl_error_callback(objname, filename, function, line, code, buf);
}
