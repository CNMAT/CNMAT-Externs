/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2006, The Regents of
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
NAME: stop
DESCRIPTION: Sends a stop message to all objects you specify by type
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2007
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: First version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"

typedef struct _stop
{
	t_object s_ob;
	void *outlet;
	t_atom *s_args;
	short s_numArgs;
} t_stop;

void *stop_class;

void stop_assist(t_stop *x, void *b, long m, long a, char *s);
void *stop_new(t_symbol *m, short argc, t_atom *argv);
void stop_free(t_stop *x);
void stop_bang(t_stop *x);
short lookForDelays(t_patcher *p, void *arg);
void stopDelay(t_object *x);

//--------------------------------------------------------------------------

int main(void){
	setup((t_messlist **)&stop_class, (method)stop_new, (method)stop_free, (short)sizeof(t_stop), 0L, A_GIMME, 0);
	
	version(0);

	addmess((method) version, "version", 0);
	addmess((method)stop_assist, "assist", A_CANT, 0);
	addbang((method)stop_bang);
	
	return 0;
}

//--------------------------------------------------------------------------

void stop_assist(t_stop *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
	}
	else {
	}
}

void *stop_new(t_symbol *m, short argc, t_atom *argv){
	t_stop *x;

	x = (t_stop *)newobject(stop_class);
	//x->outlet = bangout(x);

	int i;
	if(argc){
		x->s_args = (t_atom *)calloc(argc, sizeof(t_atom));
		for(i = 0; i < argc; i++)
			x->s_args[i] = argv[i];
		x->s_numArgs = argc;
	} else {
		x->s_numArgs = 1;
		x->s_args = (t_atom *)calloc(1, sizeof(t_atom));
		SETSYM(x->s_args, gensym("delay"));
	}

	return(x);
}

void stop_free(t_stop *x){}

void stop_bang(t_stop *x){
	patcher_eachdo((method)lookForDelays, x);
}

short lookForDelays(t_patcher *p, void *x){
	t_stop *xx = (t_stop *)x;
	struct box *boxes = p->p_box;
	struct box *b;
	b = boxes;
	int i;
	for(b = boxes; b; b = b->b_next){
		for(i = 0; i < xx->s_numArgs; i++){
			if(strcmp(ob_name(b->b_firstin), xx->s_args[i].a_w.w_sym->s_name) == 0){
				stopDelay(b->b_firstin);
			}
		}
	}
	return 0;
}

void stopDelay(t_object *x){
	mess0(x, gensym("stop"));
}
