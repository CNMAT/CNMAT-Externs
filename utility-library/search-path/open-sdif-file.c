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

#ifdef __MWERKS__
#include <FSp_fopen.h>
#endif


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
	
    /* Turning pathID into a FILE * is platform-specific: */
    
#ifdef WIN_VERSION
    {
		short maxErr;
		char fullpath[MAX_FULLPATH_LEN];
		char conformed[MAX_FULLPATH_LEN];
		maxErr = path_topathname(pathID, filenamecopy, fullpath);
		if (maxErr) {
			error("path_topathname returned error code %d - can't open %s", maxErr, filename);
			return NULL;
		}
		maxErr = path_nameconform(fullpath, conformed, PATH_STYLE_NATIVE, PATH_TYPE_ABSOLUTE);
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
#else
/* Macintosh version */

#define PATH_SPEC_MEANS_FSSPEC
#ifdef PATH_SPEC_MEANS_FSSPEC
	result = path_tospec(pathID, filenamecopy, &ps);
	if (result != 0) {
		post("¥ SDIF-buffer: couldn't make PATH_SPEC from SDIF file %s (path_tospec returned %ld)",
			 filenamecopy, result);
		return NULL;
	}

	/* Now what to do with the FSSpec? */
#ifdef __MWERKS__
    f = FSp_fopen (&ps, "rb");
#else
	// No FSp_fopen(), so use the more roundabout route in Carbon that David Zicarelli suggested
	
    {
	   OSErr errorCode;
	   OSStatus fileManagerResultCode;
	   UInt8 path_unsigned[MAX_FULLPATH_LEN];
	   char path[MAX_FULLPATH_LEN];
	   FSRef myFSRef;
	   int i;

	   errorCode =  FSpMakeFSRef (&ps, &myFSRef);
	   if (errorCode) {
	     error("SDIF-buffer: FSpMakeFSRef returned %d; can't open %s", errorCode, filename);
	     return NULL;
	   }

	   fileManagerResultCode =  FSRefMakePath (&myFSRef, path_unsigned, MAX_FULLPATH_LEN);
	   if (fileManagerResultCode) {
	     error("SDIF-buffer: FSRefMakePath returned %d; can't open %s", fileManagerResultCode, filename);
	     return NULL;
	   }

#ifdef NEED_SIGNED_PATH
	   for (i=0; i<MAX_FULLPATH_LEN; ++i) {
	   	   if (path_unsigned[i] >= 128) {
	   	   	    error("Path returned by FSRefMakePath contains illegal character %d (%c); can't open.",
	   	   	    	  path_unsigned[i], path_unsigned[i]);
	   	   	    return NULL;
	   	   }
	   	   path[i] = (char) path_unsigned[i];
	   	   if (path[i] == '\0') break;
	   }
	   
	   post("path: %s", path);

	   f = fopen(path, "rb");
#else	   		
	   f = fopen(path_unsigned, "rb");
#endif // NEED_SIGNED_PATH
	   if (f == NULL) {
	     error("SDIF-buffer: fopen() returned NULL; can't open %s", filename);
	     return NULL;
	   } 
    }
#endif __MWERKS__

#else // PATH_SPEC_MEANS_FSSPEC
#error What do I do with a PATH_SPEC?	
#endif /* PATH_SPEC_MEANS_FSSPEC */
#endif /* WIN_VERSION */

    /* Back to platform-independent code */
    
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

