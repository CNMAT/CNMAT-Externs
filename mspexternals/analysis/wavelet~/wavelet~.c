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
NAME: wavelet~
DESCRIPTION: Discrete Wavelet Transform (DWT)
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.1: Universal Binary
VERSION 1.1.1: GPL compatible license
VERSION 1.1.2: Added tellmeeverything function
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "z_dsp.h"
#include "math.h"
#include <gsl/gsl_wavelet.h>

typedef struct _wlet
{
	t_pxobject w_ob;
	void *w_proxy[2];
	long w_inletNumber;
	double *w_tmp;
	gsl_wavelet *w_wavelet;
	gsl_wavelet_type *w_waveletType;
	gsl_wavelet_workspace *w_workspace;
	gsl_wavelet_direction w_direction;
	int w_waveletLength;
	size_t w_stride;
	size_t w_k;
} t_wlet;

void *wlet_class;

void wlet_anything(t_wlet *x, t_symbol *msg, short argc, t_atom *argv);
void wlet_int(t_wlet *x, long n);
void wlet_assist(t_wlet *x, void *b, long m, long a, char *s);
void *wlet_new(t_symbol *dir, t_symbol *wlet, long k, long n);//, long stride);
t_int *wlet_perform(t_int *w);
void wlet_setupWavelet(t_wlet *x, t_symbol *msg, size_t k);
void wlet_dsp(t_wlet *x, t_signal **sp, short *count);
void wlet_free(t_wlet *x);
void wlet_tellmeeverything(t_wlet *x);


//--------------------------------------------------------------------------

int main(void)
{
	setup((t_messlist **)&wlet_class, (method)wlet_new, (method)wlet_free, (short)sizeof(t_wlet), 0L, A_SYM, A_DEFSYM, A_DEFLONG, A_DEFLONG, A_DEFLONG, 0); 
	
	version(0);

	addmess((method) version, "version", 0);
	addmess((method)wlet_dsp, "dsp", A_CANT, 0);
	addmess((method)wlet_anything, "anything", A_GIMME, 0);
	addint((method)wlet_int);
	addmess((method)wlet_assist, "assist", A_CANT, 0);
	addmess((method)wlet_tellmeeverything, "tellmeeverything", 0);
	
	dsp_initclass();
	
	return 0;
}

//--------------------------------------------------------------------------

void wlet_anything(t_wlet *x, t_symbol *msg, short argc, t_atom *argv)
{	
	if(proxy_getinlet((t_object *)x) != 1)
		return;
		
	size_t k = 0;
	if(argc){
		if(argv[0].a_type == A_LONG)
			k = (size_t)(argv[0].a_w.w_long);
		else if(argv[0].a_type == A_FLOAT)
			k = (size_t)(argv[0].a_w.w_float);
	}
	
	wlet_setupWavelet(x, msg, k);
}

void wlet_int(t_wlet *x, long n)
{
	float f = (float)n;
	int i = (int)n;
	
	if(proxy_getinlet((t_object *)x) == 1){
		wlet_setupWavelet(x, gensym((char *)gsl_wavelet_name(x->w_wavelet)), (size_t)n);
	}else if(proxy_getinlet((t_object *)x) == 2){
		if(n > sys_getblksize()){
			error("wavelet: size must be smaller than the signal vector %ld", sys_getblksize());
			return;
		}
		while(i > 1){
			f = f / 2.0;
			if(f - round(f)){
				error("wavelet: size must be a multiple of 2");
				return;
			}
			i = (int)f;
		}
		x->w_waveletLength = (int)n;
		
		gsl_wavelet_workspace *oldWspace = x->w_workspace;
		x->w_workspace = gsl_wavelet_workspace_alloc((size_t)n);
		gsl_wavelet_workspace_free(oldWspace);
		
		double *oldTmp = x->w_tmp;
		x->w_tmp = (double *)calloc((int)n, sizeof(double));
		free(oldTmp);
	}//else if(proxy_getinlet((t_object *)x) == 3){
	//	x->w_stride = (size_t)n;
	//}
}

//--------------------------------------------------------------------------

void wlet_assist(t_wlet *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET){
		if(x->w_direction == forward){
			sprintf(s,"(Signal) DWT output");
		}
		else sprintf(s, "(Signal) Inverse DWT output");
	}
	else {
		switch (a) {	
		case 0:
			sprintf(s,"(Signal) Input");
			break;
		case 1:
			sprintf(s, "(int) k, index of the coefficient within each level");
			break;
		case 2:
			sprintf(s, "(int) size of the transform");
			break;
		}
	}
}

//--------------------------------------------------------------------------

void *wlet_new(t_symbol *dir, t_symbol *wlet, long k, long n)//, long stride)
{
	t_wlet *x;

	x = (t_wlet *)newobject(wlet_class); // create a new instance of this object
	dsp_setup((t_pxobject *)x, 1);
	//x->w_proxy[2] = proxy_new(x, 3, &x->w_inletNumber);
	x->w_proxy[1] = proxy_new(x, 2, &x->w_inletNumber);
	x->w_proxy[0] = proxy_new(x, 1, &x->w_inletNumber);
	outlet_new((t_pxobject *)x, "signal");
	outlet_new((t_pxobject *)x, "signal");
	
	if(!strcmp(dir->s_name, "forward"))
		x->w_direction = forward;
	else if(!strcmp(dir->s_name, "backward"))
		x->w_direction = backward;
	else{ 
		error("wavelet: direction must be forward or backward--bailing out");
		return 0;
	}
	
	if(n){
		if(sys_getblksize() < n) x->w_waveletLength = sys_getblksize();
		else x->w_waveletLength = n;
	}else{
		if(sys_getblksize() < 512) x->w_waveletLength = sys_getblksize();
		else x->w_waveletLength = 512;
	}
	
	//if(stride) x->w_stride = stride;
	//else 
	x->w_stride = 1;
	
	x->w_tmp = (double *)calloc(x->w_waveletLength, sizeof(double));
	//x->w_wavelet = gsl_wavelet_alloc(gsl_wavelet_daubechies, (k) ? k : 4);
	x->w_workspace = gsl_wavelet_workspace_alloc(x->w_waveletLength);
	
	if(k)
		x->w_k = (size_t)k;
	else x->w_k = 4;
	wlet_setupWavelet(x, (!strcmp(wlet->s_name, " ")) ? wlet : gensym("daubechies"), x->w_k);
		
	return(x);
}

void wlet_free(t_wlet *x)
{
	dsp_free((t_pxobject *)x);
	free(x->w_tmp);
	gsl_wavelet_free(x->w_wavelet);
	gsl_wavelet_workspace_free(x->w_workspace);
}

void wlet_setupWavelet(t_wlet *x, t_symbol *msg, size_t k)
{
	gsl_wavelet *oldWlet = x->w_wavelet;
	x->w_k = k;
	
	if(!strcmp(msg->s_name, "daubechies")){
		if(k = -1)
			k = 4;
		if(k < 4 || k > 20 || ((k / 2) - round(k / 2))){
			error("wavelet: k must equal 4, 6, 8, 10, 12, 14, 16, 18, or 20.  Defaulting to k = 4.");
			k = 4;
		}
		x->w_waveletType = (gsl_wavelet_type *)gsl_wavelet_daubechies;
		x->w_wavelet = gsl_wavelet_alloc(gsl_wavelet_daubechies, k);
	}else if(!strcmp(msg->s_name, "daubechies_centered") || !strcmp(msg->s_name, "daubechies-centered")){
		if(k = -1)
			k = 4;
		if(k < 4 || k > 20 || ((k / 2) - round(k / 2))){
			error("wavelet: k must equal 4, 6, 8, 10, 12, 14, 16, 18, or 20.  Defaulting to k = 4.");
			k = 4;
		}
		x->w_waveletType = (gsl_wavelet_type *)gsl_wavelet_daubechies_centered;
		x->w_wavelet = gsl_wavelet_alloc(gsl_wavelet_daubechies_centered, k);
	}else if(!strcmp(msg->s_name, "haar")){
		if(k = -1)
			k = 2;
		if(k != 2){
			error("wavelet: k must equal 2.  Defaulting to k = 2.");
			k = 2;
		}
		x->w_waveletType = (gsl_wavelet_type *)gsl_wavelet_haar;
		x->w_wavelet = gsl_wavelet_alloc(gsl_wavelet_haar, k);
	}else if(!strcmp(msg->s_name, "haar_centered") || !strcmp(msg->s_name, "haar-centered")){
		if(k = -1)
			k = 2;
		if(k != 2){
			error("wavelet: k must equal 2.  Defaulting to k = 2.");
			k = 2;
		}
		x->w_waveletType = (gsl_wavelet_type *)gsl_wavelet_haar_centered;
		x->w_wavelet = gsl_wavelet_alloc(gsl_wavelet_haar_centered, k);
	}else if(!strcmp(msg->s_name, "bspline")){
		if(k = -1)
			k = 103;
		if(k != 103 && k != 105 && k != 202 && k != 204 && k != 206 && k != 208 && k != 301 && k != 303 && k != 305 && k != 307 && k != 309){
			error("wavelet: k must equal 103, 105, 202, 204, 206, 208, 301, 303, 305 307, or 309.  Defaulting to k = 103.");
			k = 103;
		}
		x->w_waveletType = (gsl_wavelet_type *)gsl_wavelet_bspline;
		x->w_wavelet = gsl_wavelet_alloc(gsl_wavelet_bspline, k);
	}else if(!strcmp(msg->s_name, "bspline_centered") || !strcmp(msg->s_name, "bspline-centered")){
		if(k = -1)
			k = 103;
		if(k != 103 && k != 105 && k != 202 && k != 204 && k != 206 && k != 208 && k != 301 && k != 303 && k != 305 && k != 307 && k != 309){
			error("wavelet: k must equal 103, 105, 202, 204, 206, 208, 301, 303, 305 307, or 309.  Defaulting to k = 103.");
			k = 103;
		}
		x->w_waveletType = (gsl_wavelet_type *)gsl_wavelet_bspline_centered;
		x->w_wavelet = gsl_wavelet_alloc(gsl_wavelet_bspline_centered, k);
	}else{
		error("wavelet: wavelet of type %s not implemented", msg->s_name);
		return;
	}
	gsl_wavelet_free(oldWlet);
}

void wlet_dsp(t_wlet *x, t_signal **sp, short *count)
{
	dsp_add(wlet_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

t_int *wlet_perform(t_int *w)
{
	t_wlet *x = (t_wlet *)w[1];
	t_float *inL = (t_float *)w[2];
	t_float *outL = (t_float *)w[3];
	int n = (int)w[4];
	int i, j, result;
	
	long numTransforms = (long)n / (x->w_waveletLength);
	
	//double data = x->w_tmp;
	for(j = 0; j < numTransforms; j++){
		for(i = 0; i < x->w_waveletLength; i++)
			x->w_tmp[i] = (double)inL[i + (x->w_waveletLength * j)];
		
		result = gsl_wavelet_transform(x->w_wavelet, x->w_tmp, x->w_stride, x->w_waveletLength, x->w_direction, x->w_workspace);
		if(result) post("%d", result);
		for(i = 0; i < x->w_waveletLength; i++){
			outL[i + (x->w_waveletLength * j)] = (t_float)x->w_tmp[i];
		}
	}
			
	return (w + 5);
}

void wlet_tellmeeverything(t_wlet *x){
	version(0);
	post("Direction: %s", x->w_direction == forward ? "forward" : "backward");
	post("Wavelet type: %s", gsl_wavelet_name(x->w_wavelet));
	post("k = %ld", (long)x->w_k);
	post("Size: %d", x->w_waveletLength);
}
