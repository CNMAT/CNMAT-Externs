/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2010, The Regents of
the University of California (Regents). 
Permission to use, copy, modify, distribute, and distribute modified versions
of this software and its documentation without fee and without a signed
licensing agreement, is hereby granted, provided that the above copyright
notice, this paragraph and the following two paragraphs appear in all copies,
modifications, and distributions.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: o.printbytes
DESCRIPTION: Post each byte of an OSC bundle to the Max window
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2010
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "../odot_version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"

typedef struct _opbytes{
	t_object ob;
} t_opbytes;

void *opbytes_class;

void opbytes_fullPacket(t_opbytes *x, long len, long ptr);
void opbytes_free(t_opbytes *x);
void opbytes_assist(t_opbytes *x, void *b, long m, long a, char *s);
void *opbytes_new(t_symbol *msg, short argc, t_atom *argv);

void opbytes_fullPacket(t_opbytes *x, long len, long ptr){
	unsigned char *buf = (unsigned char *)ptr;
	int i;
	post("%-12s%-12s%s", "Byte #", "ASCII", "Decimal");
	for(i = 0; i < len; i++){
		if(buf[i] == '\0'){
			post("%05d       %-14s%d", i, "'\\0'", buf[i]);
		}else{
			char b[32];
			sprintf(b, "'%c'", buf[i]);
			post("%05d       %-14s%d", i, b, buf[i]);
		}
	}
}

void opbytes_assist(t_opbytes *x, void *b, long m, long a, char *s){
	if(m == ASSIST_INLET){
		sprintf(s,"Print each byte of an OSC FullPacket to the Max window");
	}
}

void opbytes_free(t_opbytes *x){
}

void *opbytes_new(t_symbol *msg, short argc, t_atom *argv){
	t_opbytes *x;
	if(x = (t_opbytes *)object_alloc(opbytes_class)){
		//attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.printbytes", (method)opbytes_new, (method)opbytes_free, sizeof(t_opbytes), 0L, A_GIMME, 0);

	class_addmethod(c, (method)opbytes_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)opbytes_assist, "assist", A_CANT, 0);

	class_register(CLASS_BOX, c);
	opbytes_class = c;

	common_symbols_init();
	ODOT_PRINT_VERSION;
	return 0;
}
