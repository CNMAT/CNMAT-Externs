/* tactex object summer 1999, Dudas hack 
    PPC only - sorrie charlie.
    
    parts of this code were shamelessly copied from the tactex guy's code
    other parts were kluge-ily written by Matt Wright 
    
    Amar Chaudhary provided the kmeans code for centroid tracking
	as of version 0.2, the centroid tracking has been removed and put in another dedicated object

DESCRIPTION: Richard Dudas' obsolete object to read data from a Tactex MTC, superceded by an object written by the Tactex folks.
*/

#include "ext.h"
#include "ext_common.h"
#include "ext_strings.h"

#include "mtc.h"
#include <math.h>
#include <stdlib.h> // for rand() function

#define VERSION "tactex v0.2 dudas mess + wright kluge"
#define NORMALIZATION_FILENAME "MTCnormal.txt"
#define TAXELMAPPING_FILENAME "MTCmapping.txt"

#define RES_ID 16655	// for assistance
#define MAXSIZE 256 	// maximum 256 taxels right now


typedef struct _tactex
{
	Object x_ob;
	int x_connected; 	// is a tablet currently connected?
	int x_normalizing;	// are we normalizing right now?
	int x_port;			// which serial port do we connect through?
 	
	MTCHandle x_MTC; 
	MTCErr *x_press;
	int x_presssize;		// Num bytes in x_press so we can freebytes()
	int x_ntaxels;			// Number of taxel pressure values reported
	int x_ncols;			// The REAL numbers, not the funky -1 Tactex numbers
	int x_nrows;
	
	int x_reordering_array[MAXSIZE]; 
	int x_taxels[MAXSIZE]; // ordered taxel array
	
	int x_ac;
	Atom x_av[MAXSIZE]; // Atom versioon of x_taxels above (for output)
	
	void *x_outlet0; // array of taxels
	void *x_outlet1; // info- numtaxels, nrows, ncols, stragglers

} t_tactex;

void *tactex_class;

void tactex_bang(t_tactex *x);
void tactex_padinfo(t_tactex *x); 
void tactex_assist(t_tactex *x, void *b, long m, long a, char *s);
void tactex_free (t_tactex *x);
void *tactex_new(Symbol *s, int ac, Atom *av);

void tactex_connect(t_tactex *x);
void tactex_disconnect(t_tactex *x);
void tactex_startnormalize(t_tactex *x);
void tactex_stopnormalize(t_tactex *x);

void tactex_makeReorderingArray(t_tactex *x);


void main(void)
{			
	setup(&tactex_class, tactex_new, (method)tactex_free, (short)sizeof(t_tactex), 0L, A_GIMME, 0);

	addbang((method)tactex_bang);
	addmess((method)tactex_assist, "assist", A_CANT, 0);
	addmess((method)tactex_connect, "connect", 0);
	addmess((method)tactex_disconnect, "disconnect", 0);
	addmess((method)tactex_padinfo, "getinfo", 0);
	addmess((method)tactex_startnormalize, "startnormalize", 0);
	addmess((method)tactex_stopnormalize, "stopnormalize", 0);
	
	rescopy('STR#',RES_ID);
	post(VERSION,0);
}


void tactex_bang(t_tactex *x)
{
	BOOL dataAvail;
	int i, ntaxels;
	
	if (x->x_connected && x->x_press) {
		dataAvail = MTC_GetNormalizedData(x->x_MTC, x->x_press);
		
		if (dataAvail){
			//post("got normalized data %ld",x->x_ntaxels);
			ntaxels = x->x_ntaxels;
			if (ntaxels > MAXSIZE) 
				ntaxels = MAXSIZE;
			for (i=0; i<ntaxels; i++) 
				x->x_taxels[x->x_reordering_array[i]] = x->x_press[i];
			
			for (i=0; i<ntaxels; i++) 
				SETLONG(&x->x_av[i], (long)x->x_taxels[i]);
				
			x->x_ac = ntaxels;
			
			outlet_list(x->x_outlet0, 0L, x->x_ac, x->x_av); // output the taxel array
		}
	}
}


void tactex_padinfo(t_tactex *x) 
{
	int taxels = x->x_ntaxels;
	int cols = x->x_ncols;
	int rows = x->x_nrows;
	int rtaxels, rrows, stragglers;
	Atom at[4];
	
	rtaxels = taxels-(taxels%cols); 	// real (i.e. useable) taxels in 2d grid
	rrows = rtaxels/cols;				// real number of rows
	stragglers = taxels - rtaxels;		// leftover taxels (that don't make a complete row
	
	SETLONG(&at[0], (long)rtaxels);
	SETLONG(&at[1], (long)cols);
	SETLONG(&at[2], (long)rrows);
	SETLONG(&at[3], (long)stragglers);
	
	outlet_list(x->x_outlet1, 0L, 6, at); // useable taxels, ncolumns, nrows, stragglers
}

void tactex_assist(t_tactex *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,2,s);
}

void tactex_free (t_tactex *x)
{
	//free anything needing freeing
	tactex_stopnormalize(x);
	tactex_disconnect(x);
}

void *tactex_new(Symbol *s, int ac, Atom *av)
{
	int i;
	t_tactex *x;

	x = (t_tactex *)newobject(tactex_class);
	
	x->x_outlet1 = listout(x); // info list
	x->x_outlet0 = listout(x); // taxel list

	x->x_port = 0;
	if (ac>0 && av[0].a_type == A_SYM) {
		if (av[0].a_w.w_sym == gensym("a")) x->x_port = 0;
		if (av[0].a_w.w_sym == gensym("b")) x->x_port = 1;
	}
	
	x->x_normalizing = 0;
	x->x_connected = 0;
	x->x_MTC = 0L;
	x->x_press = 0L;
	x->x_presssize = 0;
	x->x_ntaxels = 0;
	x->x_ncols = 0;
	x->x_nrows = 0;

	x->x_ac = 0;
	
	return (x);
}


/* ========================================================================= */ 

void tactex_connect(t_tactex *x) 
{		
	MTCCreate 	mtcCreate;
	MTCConfig	mtcConfig;
	MTCErr		err;
	Str255		str;
	
	Str255 in, out;
	
	/* 
		this nonsense of specifying the port by a string is so we will remain compatible 
			with new versions of the tactex guy's mtc.c code
	*/
	if (x->x_port == 0){
		strcpy((char*)in, ".AIn");
		CtoPstr((char *)in);
		strcpy((char*)out, ".AOut");
		CtoPstr((char *)out);
	} else {
		strcpy((char*)in, ".BIn");
		CtoPstr((char *)in);
		strcpy((char*)out, ".BOut");
		CtoPstr((char *)out);
	}
	
	// connect("\p.AIn","\p.AOut");
	// connect("\p.BIn","\p.BOut");
	
	if (!x->x_connected) {
		post("taxtex: Trying to connect to MTC through: %ld",  x->x_port);				
		
		mtcCreate.inputPort = (char *)in;
		mtcCreate.outputPort = (char *)out;
		mtcCreate.mappingFile = TAXELMAPPING_FILENAME;
		mtcCreate.normalFile = NORMALIZATION_FILENAME;

		x->x_MTC = MTC_New(mtcCreate);
		
		if (x->x_MTC) {
			post("tactex: Communications initialized. ", 0);
	
			/* allocate memory to store the pressure data */
			MTC_GetConfig(x->x_MTC, &mtcConfig);
			x->x_ntaxels = mtcConfig.nTaxels;
			
			/* Stupid Tactex can't count.  The highest index of the zero-based
			   numbering is actually one less than the number, duh. */
			x->x_ncols = mtcConfig.nCols+1;
			x->x_nrows = mtcConfig.nRows+1;
			x->x_presssize = mtcConfig.nTaxels * sizeof(MTCErr);
			x->x_press = (MTCErr *)getbytes(x->x_presssize);
			if (!x->x_press)
				post("tactex: Memory allocation failed...unable to display pressure.", 0);
			
			/* display the MTC firmware version */
			strcpy((char *)(str + 1), mtcConfig.unitDescriptor);
			str[0] = strlen(mtcConfig.unitDescriptor);
			post((char *)str, 0);
			
				
			/* start the data flow */
			if (!MTC_StartSendingData(x->x_MTC, MTCCompNone, 1))
				post("tactex: MTC did not respond to StartSendingData", 0);
			
			x->x_connected = 1;
		}
		else {
			post("tactex: communications not initialized ...",0);
			err = MTC_GetLastError(NULL);
			strcpy((char *)(str + 1), MTC_GetErrorString(err));
			str[0] = strlen(MTC_GetErrorString(err));
			post((char *)str, 0);
		}
		
		tactex_makeReorderingArray(x);
		tactex_padinfo(x);
	}
	else
		post("tactex: Already connected.");
		
	return;
}


void tactex_disconnect(t_tactex *x) 
{
	if (x->x_connected) {
		if (MTC_Delete(x->x_MTC))
			post("tactex: Connection to MTC closed.", 0);
		else
			post("tactex: ¥error closing connection¥", 0);
			
		x->x_MTC = NULL;
		x->x_connected = false;
		freebytes(x->x_press, x->x_presssize);
	}
	return;
}
	

void tactex_startnormalize(t_tactex *x)
{
	if (x->x_connected && !x->x_normalizing) {
		x->x_normalizing = 1;
		MTC_BeginNormalization(x->x_MTC);
	}
}

void tactex_stopnormalize(t_tactex *x)
{
	if (x->x_connected && x->x_normalizing) {
		MTC_EndNormalization(x->x_MTC, true);
		x->x_normalizing = 0;
	}
}

// ---------------------------------

void tactex_makeReorderingArray(t_tactex *x) { // thanx for figuring this out, Matt !!!!
	int i, xx, yy, goodindex;
	int reverseForParanoia[MAXSIZE];
	int taxels = x->x_ntaxels;
	int cols = x->x_ncols;
	
	//post("%ld taxels, %ld rows, %ld columns", x->x_ntaxels, x->x_nrows, x->x_ncols);
	for (i = 0; i < taxels; ++i) {
		reverseForParanoia[i] = -666;
	}
	
	for (i = 0; i < taxels; ++i) {
		xx = MTC_GetTaxelXIndex(x->x_MTC, i);
		yy = MTC_GetTaxelYIndex(x->x_MTC, i);
		goodindex = (yy * cols) + xx;
		x->x_reordering_array[i] = goodindex;
		// post("taxel %ld: x %ld, y %ld", i, xx, yy);
		if (reverseForParanoia[goodindex] != -666) {
			post("tactex: More than one taxel wants to map to logical position %ld!", goodindex);
		} else {
			reverseForParanoia[goodindex] = i;
		}
	}
	
	for (i = 0; i < taxels; ++i) {
		if (reverseForParanoia[i] == -666) {
			post("tactex: No taxel wants to map to logical position %ld", i);
		}
	}
/* 	
	post("** The reordering array: ");
	for (i = 0; i < taxels; ++i) {
		post("  reorder[%ld] = %ld", i, x->x_reordering_array[i]);
	}
*/
}
