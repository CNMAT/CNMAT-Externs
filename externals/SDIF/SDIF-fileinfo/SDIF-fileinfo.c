/*
Copyright (c) 2005.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Matt Wright, The Center for New
Music and Audio Technologies, University of California, Berkeley.

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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: SDIF-fileinfo
DESCRIPTION: Read info about SDIF streams from an SDIF file
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2005
VERSION 0.0: first version 050405, derived from SDIF-menu
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


To-Do:  use opendialog to present a dialog box to the user

*/


/* SDIF-fileinfo
*/


#include "version.h"

#include "ext.h"
/* Undo ext.h's macro versions of some of stdio.h: */
#undef fopen
#undef fclose
#undef fprintf
#undef fscanf
#undef fseek
#undef sprintf
#undef sscanf

#include <stdio.h>
#include <string.h>
#include "sdif.h"
#include "open-sdif-file.h"


Symbol *ps_file, *ps_stream, *ps_no_file;

#define MAX_STREAMS 100  // Most SDIF streams any file should have

typedef struct sdif_fileinfo {
	struct object t_ob;

	Symbol			*x_filenameSymbol;
	
	int			    x_ns; // number of streams seen
	sdif_int32 		x_streamID[MAX_STREAMS];
	char 			x_frameType[MAX_STREAMS][4];
	Symbol			*x_frameTypeSymbol[MAX_STREAMS];
    sdif_float64 	x_starttime[MAX_STREAMS];
	sdif_float64 	x_endtime[MAX_STREAMS];
	sdif_int32		x_numframes[MAX_STREAMS];
	
	void		*outlet;	// outlet.
} t_sdif_fileinfo;




void *sdif_fileinfo_new(Symbol *s, int ac, Atom *av);
void sdif_fileinfo_bang(t_sdif_fileinfo *x);
void sdif_fileinfo_clear(t_sdif_fileinfo *x);

void sdif_fileinfo_scan(t_sdif_fileinfo *x, Symbol *s);
void do_scan(t_sdif_fileinfo *x, FILE *f, char *name);

void SDIFfileinfo_output(t_sdif_fileinfo *x);
void SDIFfileinfo_version(t_sdif_fileinfo *x);


void *sdif_fileinfo_class;


/*==========================================================================*/

void main(void)
{	
	SDIFresult r;

	SDIFfileinfo_version(0);

	if (r = SDIF_Init()) {
		ouchstring(NAME ": Couldn't initialize SDIF library! %s",
		           SDIF_GetErrorString(r));
	}

	setup((t_messlist **)&sdif_fileinfo_class, (method) sdif_fileinfo_new, 0, 
		  (short)sizeof(t_sdif_fileinfo), 0L, A_GIMME, 0);
	addbang((method)sdif_fileinfo_bang);
	addmess((method)sdif_fileinfo_clear, "clear", 0);
	addmess((method)SDIFfileinfo_version, "version", 0);
	addmess((method)sdif_fileinfo_scan, "scan", A_SYM, 0);

	
	ps_file = gensym("/file");
	ps_stream = gensym("/stream");
	ps_no_file = gensym("<no SDIF file read>");


}

void *sdif_fileinfo_new(Symbol *s, int ac, Atom *av) {
	t_sdif_fileinfo *x;
	x = newobject(sdif_fileinfo_class);	
    x->outlet = outlet_new(x, 0L);

	sdif_fileinfo_clear(x);
	return (x);
}



void SDIFfileinfo_version (t_sdif_fileinfo *x) {
	post(NAME " Version " VERSION
		  ", by " AUTHORS ". Compiled " __TIME__ " " __DATE__);	
}



void sdif_fileinfo_bang(t_sdif_fileinfo *x) {		
	SDIFfileinfo_output(x);
}

void sdif_fileinfo_clear(t_sdif_fileinfo *x) {	
	x->x_filenameSymbol = ps_no_file;
	x->x_ns = 0;	
}







/*==========================================================================*/
/*   SDIF file reading shit   */
/*==========================================================================*/

void sdif_fileinfo_scan(t_sdif_fileinfo *x, Symbol *s)  {	
	SDIFresult r;
	FILE *f;
	char *filename = s->s_name;
	
	
	x->x_ns = 0; // zero streams until a file is read
	x->x_filenameSymbol = s;
		
	f = OpenSDIFFile(filename);
	if (f == NULL) {
		error(NAME ": Couldn't read SDIF file %s", filename);
		return;
	} 
		
	do_scan(x, f, filename);
	
	if ((r = SDIF_CloseRead(f))) {
		post(NAME ": error closing SDIF file %s:", filename);
		post("%s", SDIF_GetErrorString(r));
	}
	
	SDIFfileinfo_output(x);
}

void do_scan(t_sdif_fileinfo *x, FILE *f, char *name) {
	SDIFresult r;
	SDIF_FrameHeader fh;
	int result, i, sawStreamAlready;
	char frameTypeBuffer[5];

	x->x_ns = 0;
		
	while ((r = SDIF_ReadFrameHeader(&fh, f)) == ESDIF_SUCCESS) {
		/* post("** Read frame header: ID %d, time %g, type %c%c%c%c", fh.streamID, fh.time,
			 fh.frameType[0], fh.frameType[1], fh.frameType[2], fh.frameType[3]); */
			 
		sawStreamAlready = 0;
		for (i = 0; i < x->x_ns; ++i) {
			if (x->x_streamID[i] == fh.streamID) {
				sawStreamAlready = 1;
				// Already saw this stream, so just make sure type is OK
				if (!SDIF_Char4Eq(fh.frameType, x->x_frameType[i])) {
					post("¥ streamlist: Warning: First frame for stream %ld", fh.streamID);
					post("¥ had type %c%c%c%c, but frame at time %g has type %c%c%c%c",
						 x->x_frameType[i][0], x->x_frameType[i][1],
						 x->x_frameType[i][2], x->x_frameType[i][3],
						 fh.time, fh.frameType[0], fh.frameType[1], fh.frameType[2],
						 fh.frameType[3]);
				}
				break;
			}
		}
				
		if (!sawStreamAlready) {
			if (x->x_ns >= MAX_STREAMS) {
				error(NAME ": SDIF file has more than %ld streams!", MAX_STREAMS);
				return;
			}
			++(x->x_ns);
			x->x_streamID[i] = fh.streamID;
			SDIF_Copy4Bytes(x->x_frameType[i], fh.frameType);
			SDIF_Copy4Bytes(frameTypeBuffer, fh.frameType);
			frameTypeBuffer[4] = '\0';
			x->x_frameTypeSymbol[i] = gensym(frameTypeBuffer);
			x->x_starttime[i] = fh.time;
			x->x_numframes[i] = 0;
		}
		
		x->x_endtime[i] = fh.time;
		++(x->x_numframes[i]);

		if (r = SDIF_SkipFrame(&fh, f)) {
			post(NAME ": error skipping frame in SDIF file %s:", name);
			post("   %s", SDIF_GetErrorString(r));
			return;
		}
	}
	
	if (r != ESDIF_END_OF_DATA) {
		post(NAME ": error reading SDIF file %s:", name);
		post("%s", SDIF_GetErrorString(r));
	}
	
	return;
}


void SDIFfileinfo_output(t_sdif_fileinfo *x) {
	int i;
	
	Atom arguments[5];  // ID, type, start, end, #frames
	

	// post("File %s has %d streams:", x->x_filenameSymbol->s_name, x->x_ns);

	SETSYM(arguments, x->x_filenameSymbol);
	SETLONG(arguments+1, x->x_ns);
	outlet_anything(x->outlet, ps_file, 2, arguments);

	for (i = 0; i < x->x_ns; ++i) {
		/* post("  ID %d, type %c%c%c%c, start %g end %g, %d frames",
			 x->x_streamID[i], x->x_frameType[i][0], x->x_frameType[i][1],
			 x->x_frameType[i][2], x->x_frameType[i][3], x->x_starttime[i],
			 x->x_endtime[i], x->x_numframes[i]); */
			 
		SETLONG(arguments, x->x_streamID[i]);
		SETSYM(arguments+1, x->x_frameTypeSymbol[i]);
		SETFLOAT(arguments+2, x->x_starttime[i]);
		SETFLOAT(arguments+3, x->x_endtime[i]);
		SETLONG(arguments+4, x->x_numframes[i]);
		outlet_anything(x->outlet, ps_stream, 5, arguments);
	}	
}
