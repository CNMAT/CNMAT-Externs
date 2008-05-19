/*
Copyright (c) 1999, 2000,01,02,03,04,05.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Based on sample code from David Zicarelli.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

*/

/* open-sdif-file.c : open an SDIF file in Max's search path
   This code was split from SDIF-buffer.c on 050405 by Matt Wright
   On 080519 Matt changed this to work with Max 5, removing lots of stuff having to do with Metrowerks, FSSPEC, etc.

 */
 
 
#include "ext.h"
/* Undo ext.h's macro versions of some of stdio.h: */
#undef fopen
#undef fclose
#undef fprintf
#undef fscanf
#undef fseek
#undef sprintf
#undef sscanf

#define TEST_PLATFORM
#ifdef TEST_PLATFORM
#ifdef __MWERKS__
#warning "__MWERKS__"
#error "Metrowerks no longer supported (try revision 1772 or before)"
#endif
#ifdef __POWERPC__
#warning "__POWERPC__"
#endif
#ifdef __APPLE__
#warning "__APPLE__"
#endif
#ifdef __MACH__
#warning "__MACH__"
#endif
#ifdef WIN_VERSION
#warning "WIN_VERSION"
#endif
#endif // TEST_PLATFORM

 
#include <stdio.h>
#include <string.h>

#include "sdif.h"
#include "open-sdif-file.h"



#ifdef ALWAYS_WANT_TO_LOOK_IN_MAX_FOLDER
/* Life was so simple back in the old days.  
   To open an SDIF file, we just called into the SDIF library and let it
   call the stdio open() procedure.  All OpenSDIFFile() does is turn a
   (user-supplied) filename into a FILE *.  */
static FILE *OpenSDIFFile(char *filename) {
	return SDIF_OpenRead(filename);
}
#else
/* Use the Max API's locatefile_extended() to search for the file by name in the
   Max search path. */
   
#define MAX_FILENAME_LEN 256
#define MAX_FULLPATH_LEN 2000

FILE *OpenSDIFFile(char *filename) {
	char filenamecopy[MAX_FILENAME_LEN];
	short result, pathID;
	long filetype;
	PATH_SPEC ps;	
	FILE *f;
	SDIFresult r;

	
	strncpy(filenamecopy, filename, MAX_FILENAME_LEN);
	
	result = locatefile_extended(filenamecopy, &pathID, &filetype, 0, 0);

	
	if (result != 0) {
		post("¥ SDIF-buffer: couldn't locate alleged SDIF file %s in Max's search path (result %ld)", 
			 filename, result);
		return NULL;
	}
	

	// post("** Got path ID %d,filename %s", pathID, filenamecopy);
	
	/* Turn pathID into a FILE *... */
    

	{
		short maxErr;
		char fullpath[MAX_FULLPATH_LEN];
		char conformed[MAX_FULLPATH_LEN];
		maxErr = path_topathname(pathID, filenamecopy, fullpath);
		if (maxErr) {
			error("path_topathname returned error code %d - can't open %s", maxErr, filename);
			return NULL;
		}
#ifdef DEBUG_PATH_NAMECONFORM
		maxErr = path_nameconform(fullpath, conformed, PATH_STYLE_NATIVE, PATH_TYPE_ABSOLUTE);
		post("NATIVE/ABSOLUTE: %s", conformed);
		maxErr = path_nameconform(fullpath, conformed, PATH_STYLE_SLASH, PATH_TYPE_ABSOLUTE);
		post("SLASH/ABSOLUTE: %s", conformed);
		maxErr = path_nameconform(fullpath, conformed, PATH_STYLE_SLASH, PATH_TYPE_IGNORE);
		post("SLASH/IGNORE: %s", conformed);
		maxErr = path_nameconform(fullpath, conformed, PATH_STYLE_SLASH, PATH_TYPE_RELATIVE);
		post("SLASH/RELATIVE: %s", conformed);
		maxErr = path_nameconform(fullpath, conformed, PATH_STYLE_SLASH, PATH_TYPE_BOOT);
		post("SLASH/BOOT: %s", conformed);
#endif

#ifdef WIN_VERSION
		maxErr = path_nameconform(fullpath, conformed, PATH_STYLE_NATIVE, PATH_TYPE_ABSOLUTE);
#else
		maxErr = path_nameconform(fullpath, conformed, PATH_STYLE_SLASH, PATH_TYPE_BOOT);
#endif

		if (maxErr) {
			error("path_nameconform returned error code %d - can't open %s", maxErr, filename);
			return NULL;
		}
		f = fopen(conformed, "rb");
		if (f == NULL) {
			error("SDIF-buffer: fopen returned NULL; can't open %s", conformed);
			return NULL;
		} 
	}
    
	if (r = SDIF_BeginRead(f)) {
		int ferrno;
		error("SDIF-buffer: error reading header of SDIF file %s:", filename);
		error("  %s", SDIF_GetErrorString(r));
		
		ferrno = ferror(f);
		error("  ferror() returned %ld:", ferrno);
		error("      %s", strerror(ferrno));
		
		fclose(f);
		return NULL;
	}
	
	return f;
}
#endif /* ALWAYS_WANT_TO_LOOK_IN_MAX_FOLDER */

