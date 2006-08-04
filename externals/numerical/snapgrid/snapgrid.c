/*
Copyright (c) 2000,01,02,03,04,05,06.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Tim Madden, The Center for New Music and
Audio Technologies, University of California, Berkeley.

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
NAME: snapgrid
DESCRIPTION: Snap to Grid is used for adding randomness to a list of numbers, making a preset "grid, " making harmonic series, and creating lydian chromatic scales. Useful for tuning bell sounds to a chord, or randomizing amplitudes.
AUTHORS: Tim Madden
COPYRIGHT_YEARS: 1998,99,2000,01,02,03,04,05,06
VERSION 0.1: Tim's initial version
VERSION 0.2: MachO compile, added version message
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/

/*
 * Includes.
 */
 
// #include <Navigation.h>
// #include <FSp_fopen.h>
#include "ext.h"

#include "gridlock.h"
#include "version.h"

// Define the max number of args.
#define MAX_ARGS 2048

// Structure for this type of object.
typedef struct t_TJMsnapgrid
{
	struct object t_TJMobj;
	void *t_TJMout;	
	
	int t_TJMis_grid_set;
	Grid *t_TJMgrid;
	Atom t_TJMlist_out[MAX_ARGS];
	int t_TJMoutsize;
} t_TJMSnapGrid;

Symbol *ps_list;


void *snapgrid_class;

/*
 * Function Prototypes.
 */

// Make new object.
void *snapgrid_new(
	Symbol s, 
	short ac,
	Atom *av);


// Set up the grid.
void SetGrid(
	t_TJMSnapGrid *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// Snap frequencies to the grid.
void Snap2Grid(
	t_TJMSnapGrid *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// Make a new grid from arguments.	
Grid* Arg2Grid(
	int argc, 
	Atom *argv);

// Put grid into a list.
void Grid2Atom(Grid* grid, Atom *list);
	
	
// Make a grid of harmonics of list of freq. Send to output.
void OutputHarmGrid(
	t_TJMSnapGrid *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// Alter freqs by given percent.
void OutputJerkedFreq(
	t_TJMSnapGrid *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);
	
// Give a chord, return Lydian Chromatic Parent Scale.
void OutputLydianChScale(
	t_TJMSnapGrid *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);	

void version(t_TJMSnapGrid *x);


/**************************************************************************************
 *
 *************************************************************************************/


void main(fptr *f)
{
	
	//post("Running snapgrid main");
	
	/* tell Max about my class. The cast to short is important for 68K */
	setup((t_messlist **)&snapgrid_class, (method) snapgrid_new, 0,
			(short)sizeof(t_TJMSnapGrid), 0L, 0);
			
	// Old Lame messages for backward compatibility.		
	addmess((method)SetGrid, "SetGrid", A_GIMME, 0);
	addmess((method)Snap2Grid, "Snap2Grid", A_GIMME, 0);
	addmess((method)OutputJerkedFreq, "JerkFreq", A_GIMME, 0);
	addmess((method)OutputHarmGrid, "HarmGrid", A_GIMME, 0);
	addmess((method)OutputLydianChScale, "Lydian", A_GIMME, 0);

	// New improved messages for Ali.
	addmess((method)SetGrid, "setgrid", A_GIMME, 0);
	addmess((method)Snap2Grid, "snap2grid", A_GIMME, 0);
	addmess((method)OutputJerkedFreq, "jitter", A_GIMME, 0);
	addmess((method)OutputHarmGrid, "harmgrid", A_GIMME, 0);
	addmess((method)OutputLydianChScale, "lydian", A_GIMME, 0);

	addmess((method) version, "version", 0);

	
	ps_list = gensym("list");
	version(0);
}


/**************************************************************************************
 *
 *************************************************************************************/

void *snapgrid_new(
	Symbol s, 
	short ac,
	Atom *av)
{

	t_TJMSnapGrid *x;
	
	// post("Running snapgrid new");

	x = (t_TJMSnapGrid*)newobject(snapgrid_class);
	
	x->t_TJMis_grid_set = 0;

	x->t_TJMout = listout(x);
	
	
	
	return(x);
}


/**************************************************************************************
 *
 * Provide a list of floats to represent the grid. Store the grid in 
 * this object. Grid is an array of frequencies.
 *************************************************************************************/

void SetGrid(
	t_TJMSnapGrid *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{		
	
	//post("Running snapgrid setgrid");

	if (x->t_TJMis_grid_set)
	{	
		DeleteGrid(x->t_TJMgrid);
	}

	x->t_TJMgrid = Arg2Grid(argc, argv);
	x->t_TJMis_grid_set = 1;

	
	
}


/**************************************************************************************
 *  Make a grid of harmonics of list of freq. Send to output.
 * First arg is num harmonics, rest are frequencies.
 *************************************************************************************/

void OutputHarmGrid(
	t_TJMSnapGrid *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	// Store frequencies from arguments.
	Grid *freqs, *freqs2;
	int harms;
	
	
	

		harms = argv[0].a_w.w_long;
		freqs = Arg2Grid((argc-1), (argv+1));
		freqs2 = MakeHarmGrid(freqs,harms);
		
		
		Grid2Atom(freqs2, x->t_TJMlist_out);
		x->t_TJMoutsize = freqs2->length;
		
		
		outlet_list(x->t_TJMout, ps_list, x->t_TJMoutsize, x->t_TJMlist_out);
		 
		DeleteGrid(freqs2);
		DeleteGrid(freqs);
	
	
}

/**************************************************************************************
 * Alter freqs by given percent.
 *************************************************************************************/

void OutputJerkedFreq(
	t_TJMSnapGrid *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	// Store frequencies from arguments.
	Grid *freqs, *freqs2;
	float percent;
	
	
	

		percent = argv[0].a_w.w_float;
		freqs = Arg2Grid((argc-1), (argv+1));
		freqs2 = JerkRes(freqs,percent);
		
		
		Grid2Atom(freqs2, x->t_TJMlist_out);
		x->t_TJMoutsize = freqs2->length;
		
		
		outlet_list(x->t_TJMout, ps_list, x->t_TJMoutsize, x->t_TJMlist_out);
		 
		DeleteGrid(freqs2);
		DeleteGrid(freqs);
	
	
}


/**************************************************************************************
 * Give list of freqs, output parent Lydian Ch. Scale.
 *************************************************************************************/

void OutputLydianChScale(
	t_TJMSnapGrid *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	// Store frequencies from arguments.
	Grid *freqs, *freqs2;
    double lydian_tonic;
    int tonal_order;
	
	
	

		
		freqs = Arg2Grid(argc, argv);
		LydianTonic(freqs,&lydian_tonic,&tonal_order);
		
		freqs2 = MakeLydian(lydian_tonic, tonal_order);
		
		Grid2Atom(freqs2, x->t_TJMlist_out);
		x->t_TJMoutsize = freqs2->length;
		
		
		outlet_list(x->t_TJMout, ps_list, x->t_TJMoutsize, x->t_TJMlist_out);
		 
		DeleteGrid(freqs2);
		DeleteGrid(freqs);
	
	
}


/**************************************************************************************
 * 
 * Snap the frequencies (list of arguments) to the grid (list of frequencies).
 *
 *************************************************************************************/

void Snap2Grid(
	t_TJMSnapGrid *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	// Store frequencies from arguments.
	Grid *freqs, *freqs2;
	
	
	
	if (x->t_TJMis_grid_set)
	{
		freqs = Arg2Grid(argc, argv);
		freqs2 = Lock2Grid(freqs, x->t_TJMgrid);
		
		
		Grid2Atom(freqs2, x->t_TJMlist_out);
		x->t_TJMoutsize = freqs2->length;
		
		
		outlet_list(x->t_TJMout, ps_list, x->t_TJMoutsize, x->t_TJMlist_out);
		 
		DeleteGrid(freqs2);
		DeleteGrid(freqs);
	}
	
}

/**************************************************************************************
 * Put arguments into a newly created grid.
 *************************************************************************************/

Grid* Arg2Grid(
	int argc, 
	Atom *argv)
{
	int length, i;
	Grid* grid;
	// argc is length of list.
	length = argc;
	grid = MakeGrid(length);
	
	// Insert arguments (frequencies) into grid.
	for (i = 0; i < length; i++)
	{
		if (argv[i].a_type == A_FLOAT)
			(grid->grid)[i] =  argv[i].a_w.w_float;
		else if (argv[i].a_type == A_LONG)
			(grid->grid)[i] =  (float)(argv[i].a_w.w_long);
		else
			post("Only ints of floats in the list.");
				
	}
	
	return grid;
	
}


/**************************************************************************************
 * put grid freqs into a MAX list.
 *************************************************************************************/

void Grid2Atom(Grid* grid, Atom *list)
{

	int i;
	
	for (i = 0; i < grid->length; i++)
	{
		list[i].a_type = A_FLOAT;
		list[i].a_w.w_float = (grid->grid)[i];
	}

}

void version(t_TJMSnapGrid *x) {
    post(NAME " object version " VERSION " by " AUTHORS );
    if (x) {
      /* Not called from main(); */
      post("  compiled " __TIME__ " " __DATE__);
    }
}

