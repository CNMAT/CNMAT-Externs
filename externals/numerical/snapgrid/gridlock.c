#define REALASSERT
#ifdef REALASSERT
#include <assert.h>
#else
#define assert(x) /* Do nothing */
#endif
#include "ext.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#include "gridlock.h"

/****************************************************************
 * Construct an empty grid.
 ***************************************************************/

Grid* MakeGrid(int length)
{
    Grid* grid = (Grid*)getbytes(sizeof(Grid));
    grid->grid = (double*)getbytes(length*sizeof(double));
    grid->length = length;
 
    return grid;
}

/****************************************************************
 * Delete grid, and free its memory.
 ***************************************************************/
void DeleteGrid(Grid* grid)
{
    freebytes((char*)(grid->grid),grid->length);
    freebytes((char*)grid, sizeof(Grid));
}


/****************************************************************
 * Give a resonance map, and grid, and snap resonance freq's
 * to the grid points. grid is simply a list of frequencies. Return
 * a new resonance map.
 ***************************************************************/

Grid* Lock2Grid(
    Grid* res,
    Grid* grid) 
{ 
	int i;
    Grid* result = MakeGrid(res->length);

    /* grid is simply a list of frequencies.*/
    for (i = 0; i < res->length; i++)
    {	
        (result->grid)[i] = GridFreq((res->grid)[i], grid);
    }
    return result;
}


/****************************************************************
 * Given a resonance map, add random values to frequencies. Provide
 * percentage of freq to alter.
 ***************************************************************/

Grid* JerkRes(
    Grid* grid1,
    double  percent) 
{
    /* grid is simply a list of frequencies.*/
	double r;
	double rnd;
	int i;
    Grid* grid2 = MakeGrid(grid1->length);

    for (i = 0; i < grid1->length; i++)
    {
	    rnd = (double)(rand()) / 32768.0;
		r =  1.0 + percent*2.0*(rnd-0.5);
        (grid2->grid)[i] = r * ((grid1->grid)[i]);
    }

    return grid2;
}


/****************************************************************
 * Give a list of frequencies, considered to be pitches. Return 
 * a grid, a list of frequencies made from the pitch freq's and
 * N harmonics.
 ***************************************************************/

Grid* MakeHarmGrid(
    Grid* pitches,
    int num_harmonics)
{
    int index;
    int k, i;
    double p;

    int length = num_harmonics * pitches->length;
    /* Empty grid.*/
   
    Grid* grid = MakeGrid(length);

    index = 0;

    /* Add harmonics of pitches to grid.*/
    for (k = 0; k < pitches->length; k++)
    { 
       p = (pitches->grid)[k];
       for (i = 1; i <= num_harmonics; i++)
       {
            /* Calculate harmonics and add to grid.*/
	    (grid->grid)[index] = (double)i * p;  
            index++;
       }
    }
    return grid;
}


/****************************************************************
 * Make lydian chromatic scale, given lydian tonic and tonal order.
 ***************************************************************/

Grid* MakeLydian(
    double tonic_freq,
    int order)
{

	Grid* scale;
	int i;
    double freq;
    double chart[] = {0.0,7.0,2.0,9.0,4.0,11.0,6.0,8.0,3.0,10.0,5.0,1.0};

    if (order > 12.0) 
    {
		order = 12;
    }
   
    scale = MakeGrid(order);

    for(i = 0; i < order; i++)
    {
		freq = tonic_freq * pow(2.0,(chart[i]/12.0));
		(scale->grid)[i] = freq;
    }

    return scale;
}



/****************************************************************
 * Find Lydian Tonic of a group of pitches 
 * according to Russell's Lydian Chromatic 
 * Concept. REturn lydian tonic, (freq) and its tonal order. Based
 * on combination of pitcnes, and not order. (all 12 pitches present
 * gives useless result.)
 *
 * ARguments:
 *    pitches    Array of frequencys
 *
 * Exit Arguments:
 *    tonic     Lydian tonic of array of pitches.
 *    order     Tonal Order of Parent Lydian Chromatic Scale of array.
 *
 ***************************************************************/

void LydianTonic(
    Grid *pitches, 
    double *lydian_tonic,
    int *tonal_order)
{
    double p, lt;
    int current_order, t_o;
    /* Counters*/
    int i, ii;
    /* Start with 13 ,12 is  the most outgoing tonal order.*/
    *tonal_order = 13;
    /* No tonic found yet.*/
  

    /*Tonal order for each test tonic.*/
    
    /* Step thru all pitches, testing them as lydian tonic.*/
    for (i = 0; i < pitches->length; i++)
    {
        lt = (pitches->grid)[i];
  
        /* Test each tonic to get tonal order. 
           Tonal order for this tonic.*/
	current_order = 7;
	/* Test all pitches against this tonic.*/
	
	for (ii = 0; ii < pitches->length; ii++)
	{
            p = (pitches->grid)[ii];
  
	
	    t_o = GetTonalOrder(lt, p);
	    
	    if (t_o > current_order) 
	    {
		current_order = t_o;
	    }
	}
	
	/* See if this is the tonic.*/
	if (current_order < *tonal_order)
	{
	    *tonal_order = current_order;
	    *lydian_tonic = lt;
	}
    }

}


/****************************************************************
 * Given a lydian tonic, and another pitch, find the tonal
 * order, based on Russell's Lydian Chromatic Concept.
 ***************************************************************/

int GetTonalOrder(
    double lt, 
    double p) 
{
    double log_diff, interval;
    int i;
    /* Make tonal order chart from Russell's Western Tonal Order */
    int chart[] = {7, 12, 7, 9, 7, 11,
		  7, 7, 9, 7, 10, 7, 7};

    /* Make p higher than lt by transposing octaves up.*/
    while(p < lt) 
    {
	p = p * 2.0;
    }

    /* Find the interval between then (in equal tempermant).*/

    log_diff = ((log(p/lt)) / log(2.0));

    /* remove octaves by keeping only decimal.*/
    log_diff = log_diff - floor(log_diff);


    /* find interval by rounding.*/
    interval = floor(12.0 * log_diff + 0.5);
    /* get interval as an int.*/
    i = (int)interval;

    /*return tonal order.*/
    return chart[i];

}



/****************************************************************
 * Give a frequency, and snap it to the closest point in grid.
 * Return the snapped frequency.
 * 
 ***************************************************************/

double GridFreq(double freq, Grid *grid)
{
    /* min difference between freq's.*/
    double min_diff = 100000.0;
    double answer = 0;
    double g,d;
    int i;
 
    for (i = 0; i < grid->length; i++)
    {
         g = (grid->grid)[i];
         d = fabs(g - freq);
         if (d < min_diff)
         {
             min_diff = d;
             answer = g;
         }
    }

    return answer;
}



