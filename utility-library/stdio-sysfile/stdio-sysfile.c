/*
Copyright (c) 2004.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

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

/*
  stdio-sysfile.c
  attempt to make Max's "sysfile" platform-independent file I/O interface look like stdio.
  
*/


#include "stdio-sysfile.h"
#include <ext.h>

FILE *fopen(char *filename, char *mode) {
	ouchstring("Shouldn't need fopen!");
	return 0;
}

void fflush(FILE *f) {
	ouchstring("Shouldn't need fflush!");
}

int fread(void *ptr, size_t size, size_t nmemb, FILE *f) {
	long count = size * nmemb;
	long errorCode;
	
	errorCode = sysfile_read(f, &count, ptr);
	
	if (errorCode != 0) {
		post("sysfile_read error %d - aren't you glad you asked?", errorCode);
	}
	
	return count/size;
}

size_t fwrite(void *ptr, size_t size, size_t nmemb, FILE *f) {
	long count = size * nmemb;
	long errorCode;

	errorCode = sysfile_write(f, &count, ptr);

	if (errorCode != 0) {
		post("sysfile_write error %d - aren't you glad you asked?", errorCode);
	}
	
	return count/size;
}

int fclose(FILE *f) {
	long errorCode = sysfile_close(f);

	if (errorCode != 0) {
		post("sysfile_close error %d - aren't you glad you asked?", errorCode);
	}
	
	return errorCode;
}

int errno(void) {
	return 0;
}

char *strerror(int i) {
	return "strerror not implemented.";
}

int fseek(FILE *f, long nbytes, int mode) {
	if (mode != SEEK_CUR) {
		ouchstring("Unimplemented fseek mode!");
	} else {
		return sysfile_setpos(f, SYSFILE_FROMMARK, nbytes);
	}
}