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
DRUPAL_NODE: /patch/4015
SVN_REVISION: $LastChangedRevision$
VERSION 0.0: first version 050405, derived from SDIF-menu
VERSION 0.1: Force Package Info Generation
VERSION 0.1.1: Updated for Max5
VERSION 0.2: 1NVT outlet
VERSION 0.2.1: Minor improvements to the 1NVT outlet
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


To-Do:  use opendialog to present a dialog box to the user

*/
#define NAME "SDIF-fileinfo"
#define DESCRIPTION "Read info about SDIF streams from an SDIF file"
#define AUTHORS "Matt Wright"
#define COPYRIGHT_YEARS "2005,12,13"



/* SDIF-fileinfo
*/


#include "version.h" // make sure not to get ../SDIF-buffer/version.h

#include "ext.h"
#include "ext_obex.h"

/* Undo ext.h's macro versions of some of stdio.h: */
	/*
#undef fopen
#undef fclose
#undef fprintf
#undef fscanf
#undef fseek
#undef sprintf
#undef sscanf
	*/



#include <stdio.h>
#include <string.h>
#include "sdif.h"
#include "open-sdif-file.h"


t_symbol *ps_file, *ps_stream, *ps_no_file, *ps_1NVT;

#define MAX_STREAMS 100  // Most SDIF streams any file should have

typedef struct sdif_fileinfo {
	struct object t_ob;

	t_symbol			*x_filenameSymbol;
	
	int			    x_ns; // number of streams seen
	sdif_int32 		x_streamID[MAX_STREAMS];
	char 			x_frameType[MAX_STREAMS][4];
	t_symbol			*x_frameTypeSymbol[MAX_STREAMS];
    sdif_float64 	x_starttime[MAX_STREAMS];
	sdif_float64 	x_endtime[MAX_STREAMS];
	sdif_int32		x_numframes[MAX_STREAMS];
	
	int				print_NVT_matrices;
	void		*outlet;	// outlet.
	void *outlet2;
} t_sdif_fileinfo;




void *sdif_fileinfo_new(t_symbol *s, int ac, t_atom *av);
void sdif_fileinfo_bang(t_sdif_fileinfo *x);
void sdif_fileinfo_clear(t_sdif_fileinfo *x);

void sdif_fileinfo_scan(t_sdif_fileinfo *x, t_symbol *s);
int Read1NVTFrame(t_sdif_fileinfo *x, FILE *f, char *name, SDIF_FrameHeader *fhp);
void do_scan(t_sdif_fileinfo *x, FILE *f, char *name);

void SDIFfileinfo_output(t_sdif_fileinfo *x);
void SDIFfileinfo_output1NVT(t_sdif_fileinfo *x, char *buf, SDIF_FrameHeader *fhp);


t_class *sdif_fileinfo_class;


/*==========================================================================*/

int main(void){	
	SDIFresult r;

	version_post_copyright();

	if (r = SDIF_Init()) {
		ouchstring(NAME ": Couldn't initialize SDIF library! %s",
		           SDIF_GetErrorString(r));
	return 0;
}

	sdif_fileinfo_class = class_new("SDIF-fileinfo", (method) sdif_fileinfo_new, 0,
		  (short)sizeof(t_sdif_fileinfo), 0L, A_GIMME, 0);
	class_addmethod(sdif_fileinfo_class, (method)sdif_fileinfo_bang, "bang", 0);
	class_addmethod(sdif_fileinfo_class, (method)sdif_fileinfo_clear, "clear", 0);
	class_addmethod(sdif_fileinfo_class, (method)version, "version", 0);
	class_addmethod(sdif_fileinfo_class, (method)sdif_fileinfo_scan, "scan", A_SYM, 0);

	
	ps_file = gensym("/file");
	ps_stream = gensym("/stream");
	ps_no_file = gensym("<no SDIF file read>");
	ps_1NVT = gensym("/1NVT");

	class_register(CLASS_BOX, sdif_fileinfo_class);
}

void *sdif_fileinfo_new(t_symbol *s, int ac, t_atom *av) {
	t_sdif_fileinfo *x;
	x = object_alloc(sdif_fileinfo_class);	
	if(!x){
		return NULL;
	}
    x->outlet2 = outlet_new(x, 0L);
    x->outlet = outlet_new(x, 0L);
	x->print_NVT_matrices = 1;
	
	sdif_fileinfo_clear(x);
	return (x);
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

void sdif_fileinfo_scan(t_sdif_fileinfo *x, t_symbol *s)  {	
	SDIFresult r;
	FILE *f;
	char *filename = s->s_name;
	
	
	x->x_ns = 0; // zero streams until a file is read
	x->x_filenameSymbol = s;
		
	f = OpenSDIFFile(filename);
	if (f == NULL) {
		object_error((t_object *)x, NAME ": Couldn't read SDIF file %s", filename);
		return;
	} 
		
	do_scan(x, f, filename);
	
	if ((r = SDIF_CloseRead(f))) {
		object_post((t_object *)x, NAME ": error closing SDIF file %s:", filename);
		object_post((t_object *)x, "%s", SDIF_GetErrorString(r));
	}
	
	SDIFfileinfo_output(x);
}


void do_scan(t_sdif_fileinfo *x, FILE *f, char *name) {
	SDIFresult r;
	SDIF_FrameHeader fh;
	int result, i, sawStreamAlready, needToSkip;
	char frameTypeBuffer[5];

	x->x_ns = 0;
		
	while ((r = SDIF_ReadFrameHeader(&fh, f)) == ESDIF_SUCCESS) {
		/* post("** Read frame header: ID %d, time %g, type %c%c%c%c", fh.streamID, fh.time,
			 fh.frameType[0], fh.frameType[1], fh.frameType[2], fh.frameType[3]); */
		
		needToSkip = 1;
		
		if (x->print_NVT_matrices) {
			if (SDIF_Char4Eq(fh.frameType, "1NVT")) {
				needToSkip = 0;  // Will read the matrices in this frame rather than skipping it	
				if (Read1NVTFrame(x, f, name, &fh) == 0) return;
			}

		}
		
		sawStreamAlready = 0;
		for (i = 0; i < x->x_ns; ++i) {
			if (x->x_streamID[i] == fh.streamID) {
				sawStreamAlready = 1;
				// Already saw this stream, so just make sure type is OK
				if (!SDIF_Char4Eq(fh.frameType, x->x_frameType[i])) {
					object_post((t_object *)x, "� streamlist: Warning: First frame for stream %ld", fh.streamID);
					post("� had type %c%c%c%c, but frame at time %g has type %c%c%c%c",
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
				object_error((t_object *)x, NAME ": SDIF file has more than %ld streams!", MAX_STREAMS);
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

		if (needToSkip) {
			if (r = SDIF_SkipFrame(&fh, f)) {
				object_post((t_object *)x, NAME ": error skipping frame in SDIF file %s:", name);
				object_post((t_object *)x, "   %s", SDIF_GetErrorString(r));
				return;
			}
		}
	}
	
	if (r != ESDIF_END_OF_DATA) {
		object_post((t_object *)x, NAME ": error reading SDIF file %s:", name);
		object_post((t_object *)x, "%s", SDIF_GetErrorString(r));
	}

	outlet_bang(x->outlet2);
	
	return;
}


int Read1NVTFrame(t_sdif_fileinfo *x, FILE *f, char *name, SDIF_FrameHeader *fhp) {
	// Just read the frame header, so go through the matrices looking for 1NVTs to print

    SDIFresult r;
	int i, sz;
	SDIF_MatrixHeader mh;
	char *buf;
	
	for (i = 0; i < fhp->matrixCount; ++i) {
        if (r = SDIF_ReadMatrixHeader(&mh, f)) {
        	object_error((t_object *)x, NAME ": error reading matrix header: %s", SDIF_GetErrorString(r));
   			return 0;     	
		}
		
		if (SDIF_Char4Eq("1NVT", mh.matrixType) && mh.matrixDataType == SDIF_UTF8) {
			sz = SDIF_GetMatrixDataSize(&mh);
			buf = (char *) getbytes(sz);
			if (buf == 0) {
				object_error((t_object *)x, NAME ": out of memory; can't read name/value table");
				return 0;
			}
			if (r = SDIF_ReadMatrixData((void *) buf, f, &mh)) {
			    object_error((t_object *)x, NAME ": error reading 1NVT matrix data: %s", SDIF_GetErrorString(r));
			    return 0;
			}
			//post("Name/value table:");
			//post("%s", buf);
			SDIFfileinfo_output1NVT(x, buf, fhp);
			freebytes(buf, sz);						
		} else {
			if (SDIF_Char4Eq("1NVT", mh.matrixType)) {
				object_post((t_object *)x, NAME ": 1NVT matrix has unexpected matrix data type 0x%x; skipping",  mh.matrixDataType);
			}				
			if (r = SDIF_SkipMatrix(&mh, f)) {
			    object_error((t_object *)x, NAME ": error skipping 1NVT matrix: %s", SDIF_GetErrorString(r));
			    return 0;
			 }
	    }
	  }
}


void SDIFfileinfo_output(t_sdif_fileinfo *x) {
	int i;
	
	t_atom arguments[5];  // ID, type, start, end, #frames
	

	// post("File %s has %d streams:", x->x_filenameSymbol->s_name, x->x_ns);

	atom_setsym(arguments, x->x_filenameSymbol);
	atom_setlong(arguments+1, x->x_ns);
	outlet_anything(x->outlet, ps_file, 2, arguments);

	for (i = 0; i < x->x_ns; ++i) {
		/* post("  ID %d, type %c%c%c%c, start %g end %g, %d frames",
			 x->x_streamID[i], x->x_frameType[i][0], x->x_frameType[i][1],
			 x->x_frameType[i][2], x->x_frameType[i][3], x->x_starttime[i],
			 x->x_endtime[i], x->x_numframes[i]); */
			 
		atom_setlong(arguments, x->x_streamID[i]);
		atom_setsym(arguments+1, x->x_frameTypeSymbol[i]);
		atom_setfloat(arguments+2, x->x_starttime[i]);
		atom_setfloat(arguments+3, x->x_endtime[i]);
		atom_setlong(arguments+4, x->x_numframes[i]);
		outlet_anything(x->outlet, ps_stream, 5, arguments);
	}	
}

void SDIFfileinfo_output1NVT(t_sdif_fileinfo *x, char *buf, SDIF_FrameHeader *fhp){
	char key[256], val[256];
	char *ptr = buf;
	int i = 0;
	char msg_char[16];
	sprintf(msg_char, "/1NVT/%d", fhp->streamID);
	t_symbol *msg = gensym(msg_char);
	while(*ptr != 0x0){
		i = 0;
		while(*ptr != 0x09 && *ptr != 0x0){
			key[i++] = *ptr;
			ptr++;
		}
		key[i++] = 0x0;
		i = 0;
		while(*ptr != 0x0A && *ptr != 0x0){
			val[i++] = *ptr;
			ptr++;
		}
		val[i++] = 0x0;
		//post("%s %s", key, val);
		if(i > 1){
			t_atom out[2];
			atom_setsym(&(out[0]), gensym(key));
			atom_setsym(&(out[1]), gensym(val));
			outlet_anything(x->outlet2, msg, 2, out);
		}else{
			/*
			t_atom out;
			atom_setsym(&out, gensym("bang"));
			outlet_anything(x->outlet2, msg, 1, &out);
			*/
		}
	}
}
