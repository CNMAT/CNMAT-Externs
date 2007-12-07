/*
Copyright (c) 2006.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by John MacCallum, The Center for New Music and Audio Technologies,
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


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: roughness
DESCRIPTION: Roughness estimate based on Richard Parncutt's algorithm.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006
DRUPAL_NODE: /patch/4018
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: First version
VERSION 1.1: Added choice of diffent formulas for calculating the cbw
VERSION 1.2: Reads the contents of SDIF-buffers
VERSION 1.2.1: Fixed a bug where in the computation of the denominator in rho_process_parncutt
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "math.h"

#define CBW_HUTCHINSON_KNOPOFF 1
#define CBW_MOORE_GLASBERG 2

#define PARNCUTT 1
#define KK 2

#define VERY_SMALL 0.00000000001

#include "SDIF-buffer.h"  //  includes sdif.h, sdif-mem.h, sdif-buf.h
#include "sdif-util.h"

typedef struct _rho
{
	t_object r_ob;
	long r_in0;
	void *r_out1;
	float *r_buffer;
	long r_bufferLen;
	long r_cbwType;
	int r_method;
	float r_scIndex;
	
	t_symbol *m_bufferSym;
	SDIFBuffer *m_SDIFbuffer;
	SDIFbuf_Buffer m_buf;
	Boolean m_complainedAboutEmptyBufferAlready;
	int r_maxNumRows;
} t_rho;

void *rho_class;

void rho_anything(t_rho *x, t_symbol *msg, short argc, t_atom *argv);
void rho_list(t_rho *x, t_symbol *msg, short argc, t_atom *argv);
void rho_process(t_rho *x);
void rho_process_parncutt(t_rho *x);
void rho_process_kk(t_rho *x);
void rho_processSDIF(t_rho *x);
float rho_a2db(float a);
void rho_deswizzle(float *l, int n, float l1[], float l2[]);
void rho_set_cbw_type(t_rho *x, long t);
void rho_standardCurveIndex(t_rho *x, double i);
float rho_cbw(t_rho *x, float f);
void rho_bang(t_rho *x);
void rho_assist(t_rho *x, void *b, long m, long a, char *s);
void *rho_new(void);
void rho_free(t_rho *x);
void rho_tellmeeverything(t_rho *x);

static void LookupMyBuffer(t_rho *x);
static void rho_setSDIFbuffer(t_rho *x, Symbol *bufName);
static void *my_getbytes(int numBytes);
static void my_freebytes(void *bytes, int size);
static void rho_GetMaxNumRows(t_rho *x);
static void SetAtomFromMatrix(Atom *a, SDIFmem_Matrix m, sdif_int32 column, sdif_int32 row);

void rho_SDIFtime(t_rho *x, double t);

static Symbol *ps_SDIFbuffer, *ps_SDIF_buffer_lookup;

//--------------------------------------------------------------------------

int main(void)
{
	setup((t_messlist **)&rho_class, (method)rho_new, (method)rho_free, (short)sizeof(t_rho), 0L, 0); 
	
	version(0);

	addmess((method) version, "version", 0);
	addbang((method)rho_bang);
	addmess((method)rho_anything, "anything", A_GIMME, 0);
	addmess((method)rho_list, "list", A_GIMME, 0);
	addmess((method)rho_assist, "assist", A_CANT, 0);
	addmess((method)rho_set_cbw_type, "set-cbw-type", A_LONG, 0);
	addmess((method)rho_standardCurveIndex, "scIndex", A_FLOAT, 0);
	addmess((method)rho_tellmeeverything, "tellmeeverything", 0L, 0);
	addmess((method)rho_SDIFtime, "SDIFtime", A_FLOAT, 0);
	addmess((method)rho_setSDIFbuffer, "set", A_SYM, 0);
	addmess((method)rho_processSDIF, "process_SDIF", 0);
	
	SDIFresult r;
	
	if (r = SDIF_Init()) {
		ouchstring("%s: Couldn't initialize SDIF library! %s", 
		           NAME,
		           SDIF_GetErrorString(r));
    return 1;
	}
	
	if (r = SDIFmem_Init(my_getbytes, my_freebytes)) {
		post("¥ %s: Couldn't initialize SDIF memory utilities! %s", 
		     NAME,
		     SDIF_GetErrorString(r));
    return 1;
	}
		
	if (r = SDIFbuf_Init()) {
		post("¥ %s: Couldn't initialize SDIF buffer utilities! %s", 
		     NAME,
		     SDIF_GetErrorString(r));
		return 1;
	}
	
	/* list object in the new object list */
	//finder_addclass("Data", NAME);
	
	ps_SDIFbuffer = gensym("SDIF-buffer");
	ps_SDIF_buffer_lookup = gensym("##SDIF-buffer-lookup");
	
	return 0;
}

void *rho_new(void){
	t_rho *x;

	x = (t_rho *)newobject(rho_class); // create a new instance of this object
	
	x->r_out1 = floatout(x);	
	
	x->r_buffer = NULL;
	x->r_bufferLen = 0;
	
	x->r_cbwType = CBW_MOORE_GLASBERG;
	x->r_method = PARNCUTT;
	//if(argc > 0) rho_set_cbw_type(x, argv[0].a_w.w_long);
	
	x->m_complainedAboutEmptyBufferAlready = FALSE;
	x->m_SDIFbuffer = 0;
	x->m_buf = NULL;
	x->r_scIndex = 2.0; // parncutt's original value
	
	/*
	if(argc == 2){
		rho_setSDIFbuffer(x, argv[1].a_w.w_sym);
		post("%s", argv[1].a_w.w_sym->s_name);
	}
	*/
	
	return(x);
}

void rho_anything(t_rho *x, t_symbol *msg, short argc, t_atom *argv){
	if(!strcmp(msg->s_name, "parncutt"))
		x->r_method = PARNCUTT;
	else if(!strcmp(msg->s_name, "kk"))
		x->r_method = KK;
}

void rho_list(t_rho *x, t_symbol *msg, short argc, t_atom *argv){
	if(argc > x->r_bufferLen)
		x->r_buffer = (float *)realloc(x->r_buffer, argc * sizeof(float));
	
	x->r_bufferLen = (long)argc;
	int i;
	for(i = 0; i < argc; i++){
		x->r_buffer[i] = (float)argv[i].a_w.w_float;
	}
		
	rho_process(x);
}

void rho_process(t_rho *x){
	switch(x->r_method){
		case PARNCUTT:
			rho_process_parncutt(x);
			break;
		case KK:
			rho_process_kk(x);
			break;
	}
}

void rho_processSDIF(t_rho *x){
	int i;
	int numRows;
	
	SDIFmem_Frame f = SDIFbuf_GetFirstFrame(x->m_buf);
	char frameType[4];
	SDIF_Copy4Bytes(frameType, f->header.frameType);
	SDIFmem_Matrix m = SDIFbuf_GetMatrixInFrame(f, frameType);
	
	numRows = m->header.rowCount;
	if(numRows > x->r_bufferLen) 
		x->r_buffer = (float *)realloc(x->r_buffer, (numRows * 2) * sizeof(float));
		
	for(i = 0; i < m->header.rowCount; i++){
		x->r_buffer[i * 2] = SDIFutil_GetMatrixCell(m, 1, i);
		x->r_buffer[(i * 2) + 1] = SDIFutil_GetMatrixCell(m, 2, i);
	}
	x->r_bufferLen = numRows;
	rho_process(x);
	
	while(f){
		f = SDIFbuf_GetNextFrame(f);
		if(!f) break;

		SDIF_Copy4Bytes(frameType, f->header.frameType);
		m = SDIFbuf_GetMatrixInFrame(f, frameType);
		
		numRows = m->header.rowCount;
		if(numRows > x->r_bufferLen)
			x->r_buffer = (float *)realloc(x->r_buffer, (numRows * 2) * sizeof(float));
		
		for(i = 0; i < numRows; i++){
			x->r_buffer[i * 2] = SDIFutil_GetMatrixCell(m, 1, i);
			x->r_buffer[(i * 2) + 1] = SDIFutil_GetMatrixCell(m, 2, i);
		}
		x->r_bufferLen = numRows;
		rho_process(x);
	}
}

void rho_process_parncutt(t_rho *x){
	int i, j;
	float numerator, denominator, meanFreq, cbw, cbInterval, ratio, standardCurve, roughness;
	float cbInterval0 = 0.25;
	float index = x->r_scIndex;//2; // the bigger index, the narrower the curve
	float e = expf(1); // 2.7182818=base of natural logs
	
	numerator = denominator = 0;	
	float f1, f2, a1, a2;
	int counter = 0;
	
	for(i = 0; i < (x->r_bufferLen / 2) - 1; i++){
		f1 = x->r_buffer[i * 2];
		a1 = x->r_buffer[(i * 2) + 1];
		counter = 0;
		if(a1 > 0.){
		
			for(j = i + 1; j < (x->r_bufferLen / 2); j++){
				f2 = x->r_buffer[j * 2];
				a2 = x->r_buffer[(j * 2) + 1];
				if(a2 > 0.){
					counter++;
					//post("i: %d, j: %d, %.10f %.10f %.10f %.10f", i, j, f1, a1, f2, a2);
					
					meanFreq = (f1 + f2) / 2;
					cbw = rho_cbw(x, meanFreq);
					cbInterval = (fabsf(f2 - f1)) / cbw;
					ratio = cbInterval / cbInterval0;
					//post( "%f, %f, %f\n", ( e * ratio ), exp ( -1 * ratio ), ratio );
					standardCurve = powf((e * ratio) * expf(-1 * ratio), index);
					//printf ( "%f\n", standardCurve );
					numerator += a1 * a2 * standardCurve;
					//printf ( "%f, %f, %f\n", gAmpData[i][j], gAmpData[k][l], numerator );
				}
			}
			// this counter is to make sure that a1 was compared to something.
			if(counter) denominator += a1 * a1;	
		}
	}
	//post("numerator: %f\ndenominator: %f\n", numerator, denominator);
	if(denominator > 0)
		roughness = numerator / denominator;
	else roughness = 0;
	outlet_float(x->r_out1, roughness);
	//post("roughness: %f", roughness);
	
}

void rho_process_kk(t_rho *x){
	float k0 = 1.0;
	float C0 = 65;
	float B = 0.25;		// Kameoka & Kuriyagawa's "beta" constant.
	float n_e = 0.20;
	float n_h = 0.15;
	float n_l = 0.32;
	float p0 = 2 * pow(10, -1.15);	// The standard dissonance values are based on two equal-
						// -loudness tones having a combined loudness of 60 dB,
						// i.e. 57 dB SPL for each tone.
	float fb;
	float DI2ei, DIn, D2i, DI2i, DIt, D2ei;
	float f1, f2, a1, a2;
	int i, j;
	
	float *freq = (float *)calloc(x->r_bufferLen / 2, sizeof(float));
	float *amp = (float *)calloc(x->r_bufferLen / 2, sizeof(float));
	rho_deswizzle(x->r_buffer, x->r_bufferLen, freq, amp);
		
	for(i = 0; i < (x->r_bufferLen / 2) - 1; i++){
		for(j = i + 1; j < (x->r_bufferLen / 2); j++){
			if(freq[i] < freq[j]){
				f1 = freq[i];
				f2 = freq[j];
				a1 = rho_a2db(amp[i]);
				a2 = rho_a2db(amp[j]);
			} else {
				f2 = freq[i];
				f1 = freq[j];
				a2 = rho_a2db(amp[i]);
				a1 = rho_a2db(amp[j]);
			}
			//if(fabsf(a1 - a2) > 25) post("%d %d, a1 - a2 is greater than 25, so we're going to break", i, j);
			//if(a1 <= 17) post("%d %d, a1 is greater than 17, so we're going to break", i, j);
			if(fabsf(a1 - a2) > 25) break;
			if(a1 <= 17) break;
			if(f1 < 20 || f2 < 20) break;
			fb = 2.27 * (((a1 - 57) / 40) + 1) * powf(f1, 0.477); // Equation 6.  This should go into the rho_cbw routine.
			
			post("%d %d, f1 = %f f2 = %f a1 = %f a2 = %f", i, j, f1, f2, a1, a2);
			post("fb = %f", fb);
			
			// Supra-octave domain -- and near unison domain.
			// (We avoid calculations near zero frequency difference in order
			//  to avoid log(0) when (f2-f1)/f1 is <=0.01)
			//
			// Assign only dissonance arising from ambient noise.
			if ( f2 >= 2*f1 || (f2-f1)/f1 <= 0.01 )
				D2ei = k0*C0;  // Equation (9).
			else{
				// Dynamic Domain.  Note that the case where (f2-f1)/f1 is close to 0
				// is taken care of above to avoid log(0).  
				if ( f2-f1 <= fb )
					D2ei = k0*( 100*( 2+log10((f2-f1)/f1) )/( 2+log10(fb/f1) ) + C0 ); //Equation (7).
				else if ( f2-f1 > fb )
					D2ei = k0*( 90*( log10((f2-f1)/f1) )/( log10(fb/f1) ) + 10 + C0 ); // Static Domain.
			}
			// Compute the Absolute Dissonance Intensity (DI) for the F1-F2 dyad.

			DI2ei = powf((D2ei/k0), (1/B));	// Equation (10).

			// Compute dissonance intensity of noise DIn = (Dn0/k0)^(1/B) = C0^(1/B)

			DIn = powf(C0, (1/B)); // Equation (11).

			// Subtract noises from dissonance.

			DI2ei = DI2ei - DIn;

			// Real absolute dissonance of dyads.

			D2ei = powf(k0*DI2ei, B); // Equation (12)
		 
			// Account for SPL levels.

			// First change the sound pressure levels from dB SPL to microbars, as
			// noted in K&K Part I (after equation (8)).
			a1 = powf(10, (a1/20)/5000);
			a2 = powf(10, (a2/20)/5000);

			if ( a1 == a2 )
				D2i = powf(D2ei * (a1/p0), n_e);					// Equation (13)
			else if ( a1 > a2 )
				D2i = powf( D2ei*(a1/p0), n_e ) * powf((a2/a1), n_h);	// Equation (14)
			else // p1 < p2
				D2i = powf(D2ei*(a2/p0), n_e ) * powf((a1/a2), n_l);	// Equation (15)
		 
			// Dissonance Intensity.
			DI2i = powf((D2i/k0), (1/B)); // Equation (16).
		 
			// Total dissonance intensity is the sum of all DI2i for all combinations
			// of the partials plus the noise:  DIt = sum(DI2i) + DIn
			// where the sum goes from 1 to M=m*(m-1)/2, m is the number of partials
			DIt += DI2i;
		}
	}
	// Add in the ambient noise.
	DIt += powf((k0*C0), (1/B));
 
	// The total absolute dissonance of the complex tone. 
	outlet_float(x->r_out1,  powf(k0*DIt, B));
	
	free(freq);
	free(amp);
}

float rho_a2db(float a){
	if(a == 0)
		return 20 * log10f(VERY_SMALL / 0.00001);
	else 
		return 20 * log10f(a / 0.00001);
}

void rho_deswizzle(float *l, int n, float l1[], float l2[]){
	int i;
	for(i = 0; i < n / 2; i++){
		l1[i] = l[i * 2];
		l2[i] = l[(i * 2) + 1];
	}
}

void rho_set_cbw_type(t_rho *x, long t){
	x->r_cbwType = t;
	switch(t){
		case CBW_HUTCHINSON_KNOPOFF:
			post("Using Hutchinson and Knopoff's critical bandwidth formula:");
			post("cbw = 1.72 * powf(f, 0.65)");
			return;
		case CBW_MOORE_GLASBERG:
			post("Using Moore and Glasberg's critical bandwidth formula:");
			post("cbw = (0.108 * f) + 24.7");
			return;
	}
	error("%ld isn't a valid choice for the critical bandwidth formula.  See the help file for more info.", t);
}

void rho_standardCurveIndex(t_rho *x, double i){
	x->r_scIndex = (float)i;
}

float rho_cbw(t_rho *x, float f){
	switch(x->r_cbwType){
		case CBW_HUTCHINSON_KNOPOFF:
			return 1.72 * powf(f, 0.65);
		case CBW_MOORE_GLASBERG:
			return (0.108 * f) + 24.7;
	}
	return (0.108 * f) + 24.7;
}

void rho_bang(t_rho *x)
{
}

void rho_assist(t_rho *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET)
		sprintf(s,"help");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"help");
			break;
		}
	}
}

void rho_free(t_rho *x)
{
	free(x->r_buffer);
}

void rho_tellmeeverything(t_rho *x){
	version(0);
	switch(x->r_cbwType){
		case CBW_HUTCHINSON_KNOPOFF:
			post("Using Hutchinson and Knopoff's critical bandwidth formula:");
			post("cbw = 1.72 * powf(f, 0.65)");
			return;
		case CBW_MOORE_GLASBERG:
			post("Using Moore and Glasberg's critical bandwidth formula:");
			post("cbw = (0.108 * f) + 24.7");
			return;
	}
}

void rho_SDIFtime(t_rho *x, double t){
	int i = 1;
	char frameType[4];
	short numArgs = 0;
	t_atom *outputArgs;
	SDIFmem_Frame f;
	
	if(!(f = SDIFbuf_GetFrame(x->m_buf, t, ESDIF_SEARCH_BACKWARDS)))
		return;
	
	SDIF_Copy4Bytes(frameType, f->header.frameType);
	SDIFmem_Matrix m = SDIFbuf_GetMatrixInFrame(f, frameType);
	outputArgs = (t_atom *)calloc(m->header.rowCount * 2, sizeof(t_atom));
	//x->r_SDIFindexes = (int *)realloc(x->r_SDIFindexes, m->header.rowCount * sizeof(int));
	
	for(i = 0; i < m->header.rowCount; i++){
		//x->r_SDIFindexes[i] = SDIFutil_GetMatrixCell(m, 0, i);
		SetAtomFromMatrix(&(outputArgs[numArgs++]), m, 1, i);
		SetAtomFromMatrix(&(outputArgs[numArgs++]), m, 2, i);
	}
	rho_list(x, 0L, numArgs, outputArgs);
}

////////////////////////////////////////////////////
// SDIF stuff
// the functions for linking to and reading from an SDIF-buffer were taken from SDIF-tuples

static void LookupMyBuffer(t_rho *x) {
#ifdef THE_S_THING_KLUDGE
	void *thing;
	
	thing = x->t_bufferSym->s_thing;
	
	if (thing) {
		if (ob_sym(thing) = ps_SDIFbuffer) {
			x->t_buffer = thing;
			return;
		}
	}
	x->t_buffer = 0;
#else
	SDIFBufferLookupFunction f;
	
	f = (SDIFBufferLookupFunction) ps_SDIF_buffer_lookup->s_thing;
	if (f == 0) {
		/* No SDIF buffer has ever been created yet. */
		x->m_SDIFbuffer = 0;
	} else {
		x->m_SDIFbuffer = (*f)(x->m_bufferSym);
	}
#endif

  //  get access to the SDIFbuf_Buffer instance
  if (x->m_SDIFbuffer)
    x->m_buf = (x->m_SDIFbuffer->BufferAccessor)(x->m_SDIFbuffer);
  else
    x->m_buf = NULL;
}

static void rho_setSDIFbuffer(t_rho *x, Symbol *bufName) {
	x->m_SDIFbuffer = 0;
	x->m_bufferSym = bufName;

	LookupMyBuffer(x);
	if (x->m_SDIFbuffer == 0) {
		error("roughness: there is no SDIF-buffer \"%s\"", bufName->s_name);
		return;
	}
	x->m_complainedAboutEmptyBufferAlready = FALSE;
	
	rho_GetMaxNumRows(x);
}

static void *my_getbytes(int numBytes) {
	if (numBytes > SHRT_MAX) {
			return 0;
	}
	return (void *) getbytes((short) numBytes);
}

static void my_freebytes(void *bytes, int size) {
	freebytes(bytes, (short) size);
}

static void rho_GetMaxNumRows(t_rho *x){
	int index;
	int j;
	
	SDIFmem_Frame f = SDIFbuf_GetFirstFrame(x->m_buf);
	char frameType[4];
	SDIF_Copy4Bytes(frameType, f->header.frameType);
	SDIFmem_Matrix m = SDIFbuf_GetMatrixInFrame(f, frameType);
	
	for(j = 0; j < m->header.rowCount; j++){
		index = (int)SDIFutil_GetMatrixCell(m, 0, j);
		if(index > x->r_maxNumRows)
			x->r_maxNumRows = index;
	}
	
	while(f){
		f = SDIFbuf_GetNextFrame(f);
		if(!f) break;
		m = SDIFbuf_GetMatrixInFrame(f, frameType);
				
		for(j = 0; j < m->header.rowCount; j++){
			index = (int)SDIFutil_GetMatrixCell(m, 0, j);
			if(index > x->r_maxNumRows)
				x->r_maxNumRows = index;
		}
	}
}

static void SetAtomFromMatrix(Atom *a, SDIFmem_Matrix m, sdif_int32 column, sdif_int32 row) {
	if (m->header.matrixDataType == SDIF_INT32) {
		SETLONG(a, SDIFutil_GetMatrixCell_int32(m, column, row));
	} else {
		SETFLOAT(a, SDIFutil_GetMatrixCell(m, column, row));
	}
}
