/* kmeans object summer 1999, Dudas, Chaudhary
    PPC only - sorrie charlie.
    
    Amar Chaudhary provided the kmeans code for centroid tracking


to do: 

right now this is really a mess. I have to clean it up - 
  like why bother to copy the incoming list before running the thresh?
  
maybe make taxpoint struct = (x, y) and centroid struct = taxpoint + amp + age. ??

*/

#include "ext.h"
#include "ext_common.h"
#include "ext_strings.h"

#include <math.h>
#include <stdlib.h> // for rand() function

#define VERSION "kmenas v0.1 dudas mess + chauhary hack"

#define RES_ID 16656	// for assistance
#define MAXTAXELS 256 	// maximum 256 taxels right now
#define DEFCOLS 14		// default 14x9 taxel grid (126 taxels)
#define DEFROWS 9
#define MAXCENTROIDS 10 // maximum of 10 centroids
#define DEFCENTROIDS 5	// default ncentroids to look for
#define DEFTHRESHOLD 64 // default threshold for peak detection

#define HACKVAL 0		// hack val (heuristic) for kmeans

typedef struct _taxpoint {
	float x;
	float y;
	float weight;
	int age;
} Taxpoint;

typedef struct _kmeans
{
	Object x_ob;
	int x_ntaxels;			// Number of taxel pressure values reported
	int x_ncols;			// The REAL numbers, not the -1 Tactex numbers
	int x_nrows;
	
	int x_taxels[MAXTAXELS]; // ordered taxel array
	
	int x_ncentroids; 		// number of centroids to look for
	int x_xused; 			// number of centroids actually found/used
	
	int x_threshold; 		// threshold for peak detection thing
	int x_heuristic;  		// hack number 0 1 or 2
	
	Taxpoint x_centroids[MAXCENTROIDS]; 
	//float x_amplitudes[MAXCENTROIDS];
	//int x_age[MAXCENTROIDS];
	
	Atom x_clist[MAXCENTROIDS*4]; // output atom for taxel array (x, y, amplitude, age)
	
	void *x_outlet0; // tracked centroids
	void *x_outlet1; // ncentroids found

} t_kmeans;


void *kmeans_class;


void kmeans_list(t_kmeans *x, Symbol *s, int ac, Atom *av);
void kmeans_outputcentroids(t_kmeans *x);
void kmeans_ncentroids(t_kmeans *x, long n);
void kmeans_dimensions(t_kmeans *x, Symbol *s, int ac, Atom *av);
void kmeans_threshold(t_kmeans *x, long n);
void kmeans_heuristic(t_kmeans *x, long n);
void kmeans_assist(t_kmeans *x, void *b, long m, long a, char *s);
void kmeans_free (t_kmeans *x);
void *kmeans_new(Symbol *s, int ac, Atom *av);

float kmeans_distance(Taxpoint *p1, Taxpoint *p2);
void kmeans_init(t_kmeans *x);
void kmeans_thresh(t_kmeans *x);
void kmeans_test(t_kmeans *x, Taxpoint *coords, int coordssize, int n, int hack); //temporary
void kmeans_run (t_kmeans *x, Taxpoint *coords, int coordssize, int n, int hack);

void main(void)
{			
	setup(&kmeans_class, kmeans_new, (method)kmeans_free, (short)sizeof(t_kmeans), 0L, A_GIMME, 0);

	addinx((method)kmeans_ncentroids, 1); // ncentroids right inlet
	addmess((method)kmeans_ncentroids, "ncentroids", A_LONG, 0);
	addmess((method)kmeans_list, "list", A_GIMME, 0);
	addmess((method)kmeans_assist, "assist", A_CANT, 0);
	addmess((method)kmeans_threshold, "threshold", A_LONG, 0);
	addmess((method)kmeans_heuristic, "heuristic", A_LONG, 0);
	addmess((method)kmeans_dimensions, "dimensions", A_GIMME, 0);
	
	rescopy('STR#',RES_ID);
	post(VERSION,0);
}

void kmeans_list(t_kmeans *x, Symbol *s, int ac, Atom *av)
{
	int i;
	
	for (i=0; i< ac; i++) {
		if (av[i].a_type == A_LONG)
			x->x_taxels[i] = (int)av[i].a_w.w_long;	
		else if (av[i].a_type == A_FLOAT)
			x->x_taxels[i] = (int)av[i].a_w.w_float;
		else 
			x->x_taxels[i] = 0;
	}
	kmeans_thresh(x);
	kmeans_outputcentroids(x);
}

void kmeans_outputcentroids(t_kmeans *x) 
{
	int i;
	for (i=0; i<x->x_xused; i++) {
		SETFLOAT(&x->x_clist[i*4], x->x_centroids[i].x);
		SETFLOAT(&x->x_clist[i*4+1], x->x_centroids[i].y);
		SETFLOAT(&x->x_clist[i*4+2], x->x_centroids[i].weight);
		SETLONG(&x->x_clist[i*4+3], x->x_centroids[i].age);
		//post("%f %f %f" %d, x->x_centroids[i].x, x->x_centroids[i].y, x->x_centroids[i].weight, x->x_centroids[i].age);
	}
	outlet_int(x->x_outlet1, (long)x->x_xused);
	outlet_list(x->x_outlet0, 0L, x->x_xused*4, x->x_clist);
}

void kmeans_ncentroids(t_kmeans *x, long n)
{
	x->x_ncentroids = MIN(MAX(n, 0), MAXCENTROIDS);
}

void kmeans_dimensions(t_kmeans *x, Symbol *s, int ac, Atom *av)
{
	if (ac>0 && av[0].a_type == A_LONG) {
		x->x_ncols = av[0].a_w.w_long;
	}
	if (ac>1 && av[1].a_type == A_LONG) {
		x->x_nrows = av[1].a_w.w_long;
	}
	x->x_ntaxels = MAX(0, MIN(MAXTAXELS, x->x_ncols*x->x_nrows));
	post("dimensions: %d x %d = %d taxels", x->x_ncols, x->x_nrows, x->x_ntaxels);
}

void kmeans_threshold(t_kmeans *x, long n)
{
	x->x_threshold = n;
}

void kmeans_heuristic(t_kmeans *x, long n)
{
	x->x_heuristic = MAX(0, MIN(2, n)); // 0, 1 or 2
}

void kmeans_assist(t_kmeans *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,2,s);
}

void kmeans_free (t_kmeans *x)
{
	// free anything needing freeing... 
	// ...like if we ever dynamically allocate space for the taxel lists or something
}

void *kmeans_new(Symbol *s, int ac, Atom *av)
{
	int i;
	t_kmeans *x;

	x = (t_kmeans *)newobject(kmeans_class);
	
	intin(x,1); 				// ncentroids to look for
	x->x_outlet1 = intout(x); 	// ncentroids actually found
	x->x_outlet0 = listout(x); 	// centroid list


	x->x_ncentroids = DEFCENTROIDS; 	// ncentroids to look for
	x->x_xused = 0; 					// ncentroids actually found
	if (ac>0 && av[0].a_type == A_LONG) {
		x->x_ncentroids = MAX(0, MIN(MAXCENTROIDS, av[0].a_w.w_long));
	}
	
	x->x_ncols = DEFCOLS;				// ncols in taxel list
	if (ac>1 && av[1].a_type == A_LONG) {
		x->x_ncols = av[1].a_w.w_long;
	}
	
	x->x_nrows = DEFROWS;				// nrows in taxel list
	if (ac>2 && av[2].a_type == A_LONG) {
		x->x_nrows = av[2].a_w.w_long;
	}
	x->x_ntaxels = MAX(0, MIN(MAXTAXELS, x->x_ncols*x->x_nrows));
	
	
	x->x_threshold = DEFTHRESHOLD;		// noise threshold for taxel elimination + peak detection
	if (ac>3 && av[3].a_type == A_LONG) {
		x->x_threshold = av[3].a_w.w_long;
	}	
	
	x->x_heuristic = HACKVAL; 			// 0, 1 or 2
	if (ac>4 && av[4].a_type == A_LONG) {
		x->x_heuristic = MAX(0, MIN(2, av[4].a_w.w_long));
	}
	
	for (i = 0; i < MAXCENTROIDS; ++i) {
		x->x_centroids[i].weight = 0.0f;
		x->x_centroids[i].x = rand() % x->x_ncols;
		x->x_centroids[i].y = rand() % x->x_nrows;
		x->x_centroids[i].age = 0;
	}
	
	return(x);
}


/* ========================================================================= */ 


float kmeans_distance(Taxpoint *p1, Taxpoint *p2) 
{
    float dx = p1->x - p2->x;
    float dy = p1->y - p2->y;
    // return sqrtf(dx * dx + dy * dy);
    // return dx * dx + dy * dy;
    return (fabs(dx) + fabs(dy)); // cheapest and best for this type of measurement
}

void kmeans_init(t_kmeans *x) 
{
	int i;
	int k = x->x_ncentroids; // ncentroids to look for
	int ncols = x->x_ncols;
	int nrows = x->x_nrows;

	for (i = 0; i < k; ++i) {
		x->x_centroids[i].weight = 0.0f;
		x->x_centroids[i].x = rand() % ncols;
		x->x_centroids[i].y = rand() % nrows;
		x->x_centroids[i].age = 0;
	}
	x->x_xused = k;
}

void kmeans_thresh(t_kmeans *x)
{
	int i;
	int ntax = 0;
	Taxpoint tax[MAXTAXELS];
	int ntaxels = x->x_ntaxels;
	int ncols = x->x_ncols;
	int nrows = x->x_nrows;
	int npoints = 0;
	int dx1, dx2, dy1, dy2;
	int thresh = x->x_threshold;
	int thistaxel;
	
	ntaxels = floor(ntaxels/ncols) * ncols; // eliminate stray taxels at the end
	
	ntax = npoints = 0;
	for (i=0; i<ntaxels; i++) {
		// above threshold?
		thistaxel = x->x_taxels[i];
		if (thistaxel > thresh) {
			// then pack into taxlist (good points which will be sent to kmeans)
			tax[ntax].x = (float)(i%ncols);
			tax[ntax].y = (float)(floor(i/ncols));
			tax[ntax].weight = (float)thistaxel;
			
			// check if maxima (peak) point
			if ((i%ncols) == 0) dx1 = 100;
			else dx1 = thistaxel - x->x_taxels[i-1];
			
			if ((i%ncols) == ncols-1) dx2 = -100;
			else dx2 = x->x_taxels[i+1] - thistaxel;
			
			if (i<ncols) dy1 = 100;
			else dy1 = thistaxel - x->x_taxels[i-14];
			
			// this is a hack need to get the real number of complete rows instead of nrows-2
			if (i>ncols*(nrows-2)) dy2 = -100; 
			else dy2 = x->x_taxels[i+ncols] - thistaxel;
				
			// if maxima then increment npoints
			if ((dx1 >= 0 && dx2 < 0) && (dy1 >= 0 && dy2 < 0))
				 npoints++;
				 
			//increment taxlist index
			ntax++;
		}
	} 
	kmeans_run(x, tax, ntax, npoints, x->x_heuristic);
	//kmeans_test(x, tax, ntax, npoints, x->x_heuristic);
	   
}


// this is temporary and just to test the result of kmeans_thresh()
void kmeans_test(t_kmeans *x, Taxpoint *coords, int coordssize, int n, int hack) 
{
	int i;
	Atom at[4];
	post("tactex: ntax %d npts %d hack %d", coordssize, n, hack);
	
	for (i=0; i<coordssize; i++) {
		SETFLOAT(&at[0], coords[i].x);
		SETFLOAT(&at[1], coords[i].y);
		SETFLOAT(&at[2], coords[i].weight);
		SETFLOAT(&at[3], coords[i].age);
		outlet_list(x->x_outlet0, 0L, 4, at);
	}
	
	
}

//list of coords, num coords, num maxima (peaks), hack
void kmeans_run(t_kmeans *x, Taxpoint *coords, int coordssize, int n, int hack)
{
	int i, j, index;
	int k = x->x_ncentroids; // ncentroids to look for
	int xused = x->x_xused;  // ncentroids actually found
	int maxx = x->x_ncols;
	int maxy = x->x_nrows;
	
    Taxpoint old_centroids[MAXCENTROIDS];
    Boolean inPreviousFrame[MAXCENTROIDS];

	int bins[MAXCENTROIDS][MAXTAXELS];
	int binssize[MAXCENTROIDS];
	Boolean go_again = false;
	int maxruns = k;
	
	
    for (i = 0; i < MAXCENTROIDS; ++i) {
    	binssize[i] = 0;
    	inPreviousFrame[i] = false;
    }
    
	for (i = 0; i < k; ++i) {
		if (x->x_centroids[i].weight < 0.0f) {
			if (xused < n) { // if cur ncentroids < npeaks
				x->x_centroids[i].x = rand() % maxx;
				x->x_centroids[i].y = rand() % maxy;
				x->x_centroids[i].weight = 0.0f;
				x->x_centroids[i].age = 0;
				++xused;
	  
			}
		} else {
			inPreviousFrame[i] = true;
		}
	}
	
	do {
		for (i = 0; i < k; ++i) {
			old_centroids[i].x = x->x_centroids[i].x;
			old_centroids[i].y = x->x_centroids[i].y;
			old_centroids[i].weight = x->x_centroids[i].weight;
			old_centroids[i].age = x->x_centroids[i].age;
		}
      
		for (i = 0; i < coordssize; ++i) {
			float mindistance = 100000.0f;
			int closest = -1; 
			for (j = 0; j < k; ++j) {
				if (x->x_centroids[j].weight >= 0.0f) {
					float d = kmeans_distance(&coords[i], &x->x_centroids[j]); 
					//post("d %f, mindist %f", d, mindistance); 
					if (d < mindistance) {
						if ((closest != -1) && (kmeans_distance(&x->x_centroids[j], &x->x_centroids[closest]) < 2)) {
							switch (hack) {
								case 0 :
									mindistance = d;
									closest = j;
									break;
								case 1 :
									if (binssize[j] > binssize[closest]) {
										mindistance = d;
										closest = j;
									}
									break;
								case 2 :
									if (x->x_centroids[j].age > x->x_centroids[closest].age) {
										mindistance = d;
										closest = j;
									}
									break;
							}
						} else {
							mindistance = d;
							closest = j;
						}
					}
				}
			}
			//post("closest: %d", closest);
			bins[closest][binssize[closest]] = i;
			binssize[closest]++;

		}
	      
		// calculate new centroids / amplitudes
	      
		go_again = false;
		for (i = 0; i < k; ++i) {
			float X = 0.0f; 
			float Y = 0.0f;
			x->x_centroids[i].weight = 0.0f;
			for (index = 0; index < binssize[i]; ++index) {
				int thisindex = bins[i][index];
				x->x_centroids[i].weight += coords[thisindex].weight;
				X += coords[thisindex].x * coords[thisindex].weight;
				Y += coords[thisindex].y * coords[thisindex].weight;
			}
			if (x->x_centroids[i].weight > 0.0f) {
				x->x_centroids[i].x = X / x->x_centroids[i].weight;
				x->x_centroids[i].y = Y / x->x_centroids[i].weight;
				//amplitudes[i] /= float(bins[i].size());
				if ((x->x_centroids[i].x != old_centroids[i].x) && (x->x_centroids[i].y != old_centroids[i].y)) {
					go_again = true;
				}
			} else {
				x->x_centroids[i].weight = -1.0f;
			}
		}
		--maxruns;
	} while (go_again && maxruns);
	
	
	xused = 0;
	for (i = 0; i < k; ++i) {
		if (x->x_centroids[i].weight >= 0.0f) {
			++xused;
			++x->x_centroids[i].age;
		}
	}
	while (xused > n) {
		float minamp = 10000000.0;
		int minindex = -1;
		for (i = 0; i < k; ++i) {
			if ((x->x_centroids[i].weight >= 0.0f) && (x->x_centroids[i].weight < minamp)) {
				minamp = x->x_centroids[i].weight;
				minindex = i;
			}
		}
		x->x_centroids[minindex].weight = -1.0f;
		x->x_centroids[i].age = 0;
		--xused;
	}
	
	// restore updated values to obj instance
	x->x_xused = xused;
}