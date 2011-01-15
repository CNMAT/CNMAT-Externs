//-------------------------------------------------------------------------------
//	Copyright (c) 2004-2008 Randall Jones

//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:

//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.

//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

//-------------------------------------------------------------------------------
// 	2up.jit.centroids - generate centroids of intensity from a 1D float matrix.
//  suitable for use with 2up.jit.tactex.
// 
// 	author: randy jones rej@2uptech.com
// 	created 23 Mar 2004
// 	UB version Feb 2007
//  fixes and improved match algorithm October 2008

#include "jit.common.h"
#include <math.h>

#define	MAX_POSSIBLE_CENTROIDS	64			// number of potential centroids gathered before sorting.
#define REALLY_LARGE_NUMBER 999999999
#define Z_BIAS	1.0f						// multiplier for z component in distance calc

// e_pixdata-- stores which adjacent pixels are less in intensity. 
// 
typedef unsigned char e_pixdata;
#define	PIX_RIGHT 	0x01
#define	PIX_UP		0x02
#define	PIX_LEFT	0x04
#define	PIX_DOWN	0x08
#define	PIX_ALL		0x0F
#define PIX_DONE	0x10

typedef struct _centroid_info
{
	float					fp_sum;
	float					fx;
	float					fy;
	float					x_sum;
	float					y_sum;
	float					matchDist;
	long					match;
	long					exists;
	long					matchesPrevious;
	long					index;
}	t_centroid_info;

t_centroid_info gZeroCentroid = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0, 0, 0, 0};

typedef struct _jit_centroids 
{
	t_object				ob;
	void					*obex; 
	void 					*matrixout;
	void 					*listout;
	float					oneOverPixels;		
	float					oneOverWidth;		
	float					oneOverHeight;		
	float					threshold;				// only include pixels above this value
	float					mMatchDistance;			// maximum distance to match centroids
	float xscale;	
	float yscale;	
	float zscale;	
	long					match;					// match old centroids with new at each frame
	int						width;
	int						height;
	e_pixdata *				mInputMap;
	
	t_centroid_info	*		mpCurrentCentroids;
	t_centroid_info *		mpNewCentroids;
	t_centroid_info *		mpPreviousCentroids;
	
	long					mSubtractThreshold;
	long					mMaxCentroids;			// max centroids to report 
	long					mNewCentroids;			
	long					mCurrentCentroids;			
	int						mPreviousCentroids;
	long					mpInData;				// ptr to input matrix base addr
	long					in_rowbytes;
} 	t_jit_centroids;


void *jit_centroids_new(t_symbol *s, long argc, t_atom *argv);
void jit_centroids_free(t_jit_centroids *x);
void jit_centroids_assist(t_jit_centroids *x, void *b, long msg, long arg, char *dst);
void free_map(e_pixdata *x, int width, int height);
e_pixdata * new_map(int width, int height);

void jit_centroids_set_max(t_jit_centroids *x, void *attr, long argc, t_atom *argv);
void jit_centroids_jit_matrix(t_jit_centroids *x, t_symbol *s, int argc, t_atom *argv);
void mapImage(t_jit_centroids *x);
void gatherCentroids(t_jit_centroids *x);
void gather_centroid(t_jit_centroids *x, int i, int j, t_centroid_info * c);
void gather_centroid_recursive(t_jit_centroids *x, int i, int j, t_centroid_info * c);
void matchCentroids(t_jit_centroids *x);
void reportCentroids(t_jit_centroids *x);

#pragma mark -

t_class *jit_centroids_class;

int main()
{	
	long attrflags;
	void *classex, *attr;
	
	setup((t_messlist **)&jit_centroids_class, (method)jit_centroids_new, (method)jit_centroids_free, (short)sizeof(t_jit_centroids), 
		0L, A_GIMME, 0);

	classex = max_jit_classex_setup(calcoffset(t_jit_centroids, obex));
	
 	attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW ;

	// attributes	
	attr = jit_object_new(_jit_sym_jit_attr_offset, "threshold", _jit_sym_float32, attrflags,
		(method)0L, (method)0L, calcoffset(t_jit_centroids, threshold));	
	max_jit_classex_addattr(classex, attr);	
	
	attr = jit_object_new(_jit_sym_jit_attr_offset, "match_distance", _jit_sym_float32, attrflags,
		(method)0L, (method)0L, calcoffset(t_jit_centroids, mMatchDistance));	
	max_jit_classex_addattr(classex, attr);	
	
	attr = jit_object_new(_jit_sym_jit_attr_offset, "xscale", _jit_sym_float32, attrflags,
		(method)0L, (method)0L, calcoffset(t_jit_centroids, xscale));	
	max_jit_classex_addattr(classex, attr);	
	
	attr = jit_object_new(_jit_sym_jit_attr_offset, "yscale", _jit_sym_float32, attrflags,
		(method)0L, (method)0L, calcoffset(t_jit_centroids, yscale));	
	max_jit_classex_addattr(classex, attr);	
	
	attr = jit_object_new(_jit_sym_jit_attr_offset, "zscale", _jit_sym_float32, attrflags,
		(method)0L, (method)0L, calcoffset(t_jit_centroids, zscale));	
	max_jit_classex_addattr(classex, attr);	
	
	attr = jit_object_new(_jit_sym_jit_attr_offset, "max", _jit_sym_long, attrflags,
		(method)0L, (method)jit_centroids_set_max, calcoffset(t_jit_centroids, mMaxCentroids));	
	max_jit_classex_addattr(classex, attr);	
	
	attr = jit_object_new(_jit_sym_jit_attr_offset, "match", _jit_sym_long, attrflags,
		(method)0L, (method)0L, calcoffset(t_jit_centroids, match));	
	max_jit_classex_addattr(classex, attr);	
	
	attr = jit_object_new(_jit_sym_jit_attr_offset, "subtract_threshold", _jit_sym_long, attrflags,
		(method)0L, (method)0L, calcoffset(t_jit_centroids, mSubtractThreshold));	
	max_jit_classex_addattr(classex, attr);	
	
	max_addmethod_usurp_low((method)jit_centroids_jit_matrix, "jit_matrix");
	
	addmess((method)jit_centroids_assist, "assist", A_GIMME, 0L);
	
	max_jit_classex_standard_wrap(classex,NULL,0);	
    max_jit_class_addmethods(jit_class_findbyname(gensym("2up_jit_centroids")));
	
	return (0);
}


void *jit_centroids_new(t_symbol *s, long argc, t_atom *argv)
{
	t_jit_centroids *x;
	long i;
	const int m = MAX_POSSIBLE_CENTROIDS + 1;
		
	x = (t_jit_centroids *)max_jit_obex_new(jit_centroids_class,gensym("2up_jit_centroids"));
	max_jit_obex_dumpout_set(x, outlet_new(x,0L)); 			// general purpose outlet(rightmost)
	x->listout = outlet_new(x,0L); 							// list output

	// init vars
	x->threshold = 0.01f;
	x->xscale = 1.f;
	x->yscale = 1.f;
	x->zscale = 1.f;
	x->mMatchDistance = 2.0f;
	x->mSubtractThreshold = true;
	x->match = 1;
	x->mInputMap = 0;
	x->mpInData = 0;
	x->in_rowbytes = 0;
	x->width = 0;
	x->height = 0;
	x->mPreviousCentroids = 0;
	x->mCurrentCentroids = 0;
	x->mNewCentroids = 0;
	x->mMaxCentroids = 8;	// default

	// allocate data for gatherAndReportCentroids
	x->mpCurrentCentroids = jit_getbytes(sizeof(t_centroid_info) * m);
	x->mpNewCentroids = jit_getbytes(sizeof(t_centroid_info) * m);
	x->mpPreviousCentroids = jit_getbytes(sizeof(t_centroid_info) * m);
	
	if ((!x->mpCurrentCentroids) || (!x->mpNewCentroids) || (!x->mpPreviousCentroids)) 
	{
		error("2up.jit.centroids: out of memory!");
		x = 0;
		goto out;
	}
	
	for (i=0;i<m;i++)
	{
		x->mpCurrentCentroids[i] = gZeroCentroid;
		x->mpNewCentroids[i] = gZeroCentroid;
		x->mpPreviousCentroids[i] = gZeroCentroid;
	}
	
	max_jit_attr_args(x,argc,argv); 						// handle attribute args

out:	
	return (x);
}

void jit_centroids_free(t_jit_centroids *x)
{
	const int m = MAX_POSSIBLE_CENTROIDS + 1;
	jit_freebytes(x->mpNewCentroids, sizeof(t_centroid_info)*m);
	jit_freebytes(x->mpCurrentCentroids, sizeof(t_centroid_info)*m);
	jit_freebytes(x->mpPreviousCentroids, sizeof(t_centroid_info)*m);
	free_map(x->mInputMap, x->width, x->height);
	max_jit_obex_free(x);
}

void jit_centroids_assist(t_jit_centroids *x, void *b, long msg, long arg, char *dst)
{
	if (msg==1) 
	{
		if (arg == 0)
		{
			sprintf(dst, "matrix in (float32, 1 plane)");
		}
	} 
	else if (msg==2) 
	{
		if (arg == 0)
		{
			sprintf(dst, "centroids: list (index, x, y, intensity)");
		}
		else if (arg == 1)
		{
			sprintf(dst, "dumpout");
		}
	}
}

void jit_centroids_set_max(t_jit_centroids *x, void *attr, long argc, t_atom *argv)
{
	long m, prev;
	Atom av[6];
	int i;
	short size = 4;

	if (argc&&argv) 
	{
		m = jit_atom_getlong(&argv[0]);
		prev = x->mMaxCentroids;
		x->mMaxCentroids = MIN(m, MAX_POSSIBLE_CENTROIDS);
	
		if (x->mMaxCentroids < prev)
		{
			// write "note-offs" for expired
			for (i = 1; i <= x->mMaxCentroids; i++) 
			{	
				jit_atom_setlong(&av[0], i); 
				jit_atom_setfloat(&av[1], 0.); 
				jit_atom_setfloat(&av[2], 0.); 
				jit_atom_setfloat(&av[3], 0.); 
				outlet_list(x->listout, 0L, size, av);
			}
		}
	}
}


void free_map(e_pixdata * pMap, int width, int height)
{
	if (pMap)
	{	
		//jit_freebytes(pMap, width * height * sizeof(e_pixdata));
		sysmem_freeptr(pMap, width * height * sizeof(e_pixdata));
		pMap = 0;
	}
}


e_pixdata * new_map(int width, int height)
{
	// allocate image map
	e_pixdata * pNew = 0;
	//if (!(pNew = (e_pixdata *)jit_getbytes(width*height*sizeof(e_pixdata))))
	if (!(pNew = (e_pixdata *)sysmem_newptr(width*height*sizeof(e_pixdata))))
	{
		error("2up.jit.centroids: couldn't make image map!");
	}
	return pNew;
}


void jit_centroids_jit_matrix(t_jit_centroids *x, t_symbol *s, int argc, t_atom *argv)
{
	t_jit_matrix_info 		info;
	t_symbol * 				matrix_name = 0;
	void *					matrix = 0;
	int						width, height, rowbytes;

	// get matrix
	matrix_name = jit_atom_getsym(&argv[0]);
	if (matrix_name != _jit_sym_nothing) 
	{
		matrix = jit_object_findregistered(matrix_name);
	}
	if (!matrix) 
	{
		error ("2up.jit.centroids: couldn't get matrix object %s!", matrix_name->s_name);
		return;
	}
	
	jit_object_method(matrix,_jit_sym_getinfo,&info);
	jit_object_method(matrix,_jit_sym_getdata,&(x->mpInData));

	if (x->mpInData == 0)
	{
		error("2up.jit.centroids: null data ptr for matrix!");
		return;
	}	
	if (info.dimcount != 2)
	{
		error("2up.jit.centroids: input matrix must be 2D.");
		return;
	}	
	if (info.planecount != 1)
	{
		error("2up.jit.centroids: input matrix must be 1 plane.");
		return;
	}	
	if (info.type != _jit_sym_float32)
	{
		error("2up.jit.centroids: sorry, float32 matrix needed.");
		return;
	}
	
	height = info.dim[1];
	width = info.dim[0];
	rowbytes = info.dimstride[1];
	
	if ((width < 2) || (height < 2))
	{
		error("2up.jit.centroids: sorry, matrix must be 2x2 or bigger.");
		return;
	}

	if ((width != x->width) || (height != x->height))
	{
		x->in_rowbytes = rowbytes;
		free_map(x->mInputMap, x->width, x->height);
		
		if ((x->mInputMap = new_map(width, height)))
		{
			x->width = width; 
			x->height = height;
		}
		
		x->oneOverWidth = 1.f / (float)(width);
		x->oneOverHeight = 1.f / (float)(height);
		x->oneOverPixels = 1.f / (float)(width * height);		
	}
	
	mapImage(x);
	gatherCentroids(x);
	matchCentroids(x);
	reportCentroids(x);
}


void mapImage(t_jit_centroids *x)
{
	int						i, j;
	float *					pf_data = 0;
	float *					pf_data_prev = 0;
	float *					pf_data_next = 0;
	e_pixdata *				p_map, * p_map_prev = 0, * p_map_next = 0;
	int						firstrow, lastrow, firstcol, lastcol;
	float					pixel;
	
	const int width = x->width;
	const int height = x->height;
	const int rowbytes = x->in_rowbytes;

	// clear map
	if (x->mInputMap)
	{
		memset(x->mInputMap, 0, width*height*sizeof(e_pixdata));
	}
	else
		return;

	// set direction ptrs for each pixel to point to neighboring pixels of less intensity.
	// PIX_DOWN means that pixel in down direction has less intensity, etc. 

	for (i=0; i < height; i++)
	{
		firstrow = (i==0);
		lastrow = (i==height-1);
		
		pf_data = (float *)(x->mpInData + i*rowbytes);
		pf_data_prev = (float *)(x->mpInData + (i-1)*rowbytes);
		pf_data_next = (float *)(x->mpInData + (i+1)*rowbytes);
		
		p_map = x->mInputMap + i*width;
		p_map_prev = x->mInputMap + (i-1)*width;
		p_map_next = x->mInputMap + (i+1)*width;

		for (j=0; j < width; j++)
		{		
			firstcol = (j==0);
			lastcol = (j==width-1);
			pixel = pf_data[j];

			// right
			if (!lastcol)
			{
				if (pf_data[j+1] > pixel)
					p_map[j+1] |= PIX_LEFT;
			}
			else
			{
				p_map[j] |= PIX_RIGHT;
			}
			// up
			if (!firstrow)
			{
				if (pf_data_prev[j] > pixel)
					p_map_prev[j] |= PIX_DOWN;
			}
			else
			{
				p_map[j] |= PIX_UP;
			}
			// left
			if (!firstcol)
			{
				if (pf_data[j-1] > pixel)
					p_map[j-1] |= PIX_RIGHT;
			}
			else
			{
				p_map[j] |= PIX_LEFT;
			}
			// down
			if (!lastrow)
			{
				if (pf_data_next[j] > pixel)
					p_map_next[j] |= PIX_UP;
			}
			else
			{
				p_map[j] |= PIX_DOWN;
			} 
		}
	}
}



void gatherCentroids(t_jit_centroids *x)
{
	int i, j;

	unsigned char * p_map;
	t_centroid_info			c;

	int n = 0;			
	
	// clear new
	for (i=1; i <= MAX_POSSIBLE_CENTROIDS; i++)
	{
		x->mpNewCentroids[i] = gZeroCentroid;
	}
	
	// gathering from peaks collects pixels into new centroids
	for (i=0; i< x->height; i++)
	{
		p_map = x->mInputMap + i*x->width;
		for (j=0; j< x->width; j++)
		{
			// if peak
			if (p_map[j] == PIX_ALL)
			{
				// zero temp
				c = gZeroCentroid;
				
				// gather
				gather_centroid(x, i, j, &c);
				
				// if big enough, calc x and y and add to list
				// first centroid index is 1. 
				if (c.fp_sum > 0.)
				{
					n++;	
					if (n > MAX_POSSIBLE_CENTROIDS) goto done;
					
					// calculate center of mass
					c.fx = (float)c.x_sum / c.fp_sum;
					c.fy = (float)c.y_sum / c.fp_sum;
									
					x->mpNewCentroids[n] = c;
					x->mpNewCentroids[n].exists = true;
				}
			}
		}
	}
	
done:

	// sort new centroids by intensity
	if (n > 1)
	{
		for (i=1; i <= n; i++)
		{
			for (j=i; j <= n; j++)
			{
				if (x->mpNewCentroids[j].fp_sum > x->mpNewCentroids[i].fp_sum)
				{
					c = x->mpNewCentroids[j];
					x->mpNewCentroids[j] = x->mpNewCentroids[i];
					x->mpNewCentroids[i] = c;
				}
			}
		}
	}
	
	x->mNewCentroids = n;
	return;
}

// notes: finish something like this to make a non-recursive version. 
// right now this is not the bottleneck anyway, mapimage() takes more time.
/*
void gather_centroid(t_jit_centroids *x, int i, int j, t_centroid_info * c)
{
	register float h;				// pressure
	register float * pf_data;		// ptr to float data row
	unsigned char * p_map;
	int width = x->width;
	int firstrow, lastrow, firstcol, lastcol;
		
	// init stack. 
	stack.depth = 0;
		
	do
	{
		p_map = x->mInputMap + i*width;
		
		// mark pixel as read
		p_map[j] |= PIX_DONE;

		pf_data = ((float *)(x->mpInData + i*x->in_rowbytes));
		
		h = pf_data[j];
		if (x->mSubtractThreshold)
		{
			// subtracting threshold from pressure prevents discontinuity at zero for controllers
			// but we don't want it in all applications
			h -= x->threshold;		
		}
		
		if (h > 0.)
		{			
			// add pixel to centroid
			c->fp_sum += h;
			c->x_sum += (float)j*h;
			c->y_sum += (float)i*h;
			
			// recurse to any unmarked adjacent pixels of lesser intensity
			firstrow = (i==0);
			lastrow = (i==x->height-1);
			firstcol = (j==0);
			lastcol = (j==x->width-1);
			
			// right
			if ((!lastcol) && (p_map[j] & PIX_RIGHT) && (!(p_map[j+1] & PIX_DONE)))
			{
				stack.push(i, j);
				j++;
			}

			// up
			else if ((!firstrow) && (p_map[j] & PIX_UP) && (!(p_map[j-width] & PIX_DONE)))
			{
				stack.push(i, j);
				i--;
			}

			// left
			else if ((!firstcol) && (p_map[j] & PIX_LEFT) && (!(p_map[j-1] & PIX_DONE)))
			{
				stack.push(i, j);
				j--;
			}
	
			// down
			else if ((!lastrow) && (p_map[j] & PIX_DOWN) && (!(p_map[j+width] & PIX_DONE)))
			{
				stack.push(i, j);
				i++;
			}
				
			else 
				stack.pop(&i, &j);

		}
	} 
	while(stack.depth > 0);

}
*/


void gather_centroid(t_jit_centroids *x, int i, int j, t_centroid_info * c)
{
	register float h;				// pressure
	register float * pf_data;		// ptr to float data row
	unsigned char * p_map;
	int width = x->width;
	int firstrow, lastrow, firstcol, lastcol;
		
	p_map = x->mInputMap + i*width;
	
	// mark pixel as read
	p_map[j] |= PIX_DONE;

	pf_data = ((float *)(x->mpInData + i*x->in_rowbytes));
	
	h = pf_data[j];
	if (x->mSubtractThreshold)
	{
		// subtracting threshold from pressure prevents discontinuity at zero for controllers
		// but we don't want it in all applications
		h -= x->threshold;		
	}
	
	if (h <= 0.)
	{
		return;
	}
	
	// add pixel to centroid
	c->fp_sum += h;
	c->x_sum += (float)j*h;
	c->y_sum += (float)i*h;
	
	// recurse to any unmarked adjacent pixels of lesser intensity
	firstrow = (i==0);
	lastrow = (i==x->height-1);
	firstcol = (j==0);
	lastcol = (j==x->width-1);
	
	// right
	if (!lastcol)
		if (p_map[j] & PIX_RIGHT)
			if (!(p_map[j+1] & PIX_DONE))
				gather_centroid(x, i, j+1, c);
	// up
	if (!firstrow)
		if (p_map[j] & PIX_UP)
			if (!(p_map[j-width] & PIX_DONE))
				gather_centroid(x, i-1, j, c);
	// left
	if (!firstcol)
		if (p_map[j] & PIX_LEFT)
			if (!(p_map[j-1] & PIX_DONE))
				gather_centroid(x, i, j-1, c);
	// down
	if (!lastrow)
		if (p_map[j] & PIX_DOWN)
			if (!(p_map[j+width] & PIX_DONE))
				gather_centroid(x, i+1, j, c);

}



inline float distSquared(const t_centroid_info * a, const t_centroid_info * b);
inline float distSquared(const t_centroid_info * a, const t_centroid_info * b)
{
	float h, v, z;
	h = fabsf(a->fx - b->fx);
	v = fabsf(a->fy - b->fy);
	z = fabsf(a->fp_sum - b->fp_sum);
	return (h*h + v*v + Z_BIAS*z*z);
}


// swap new centroids to match order of current centroids close to them. 
void matchCentroids(t_jit_centroids *x)
{
	int i, j, m;
	float d, dMin;
	t_centroid_info * centroidList[MAX_POSSIBLE_CENTROIDS];
	int reserved[MAX_POSSIBLE_CENTROIDS];
	t_centroid_info	* t;		
	t_centroid_info * pCurrent, * pNew, * pClosest;
	const int new = MIN(x->mMaxCentroids, x->mNewCentroids);
	const int current = x->mCurrentCentroids;

	x->mNewCentroids = new;
	
	// copy current to previous
	for (i=1; i <= x->mMaxCentroids; i++)
	{
		x->mpPreviousCentroids[i] = x->mpCurrentCentroids[i];
	}
	x->mPreviousCentroids = x->mCurrentCentroids;

	if ((x->match) && (current > 0))
	{
		// clear matches from current
		for (i=1; i <= x->mMaxCentroids; i++)
		{
			x->mpCurrentCentroids[i].match = 0;
		}
		
		// for each new centroid, get closest current centroid
		for (i=1; i <= new; i++)
		{
			pNew = &x->mpNewCentroids[i];
			pNew->index = i;
			pNew->match = i; // default match for no current
			dMin = REALLY_LARGE_NUMBER;
			for(j=1; j<= x->mMaxCentroids; j++)
			{
				pCurrent = &x->mpCurrentCentroids[j];
				if(pCurrent->exists)
				{
					d = distSquared(pNew, pCurrent);
					if (d < dMin)
					{
						dMin = d;
						pNew->matchDist = d;
						pNew->match = j;
					}
				}
			}
			pClosest = &x->mpCurrentCentroids[pNew->match];
		}

		// sort centroids into a list by distance to closest current value
		for (i=1; i <= new; i++)
		{
			pNew = &x->mpNewCentroids[i];
			centroidList[i] = pNew;
		}
		if (new > 1)
		{
			for (i=1; i <= new; i++)
			{
				pNew = &x->mpNewCentroids[i];
				
				for (j=i; j <= new; j++)
				{
					if (centroidList[i]->matchDist > centroidList[j]->matchDist)
					{
						t = centroidList[j];
						centroidList[j] = centroidList[i];
						centroidList[i] = t;
					}
				}
			}
		}

		// mark existing as free
		for (i=1; i <= x->mMaxCentroids; i++)
		{
			reserved[i] = false;
		}


		// in order of distance, try to put centroids where they match 
		for(i=1; i<=new; i++)
		{
			m = centroidList[i]->match;
			d = centroidList[i]->matchDist;
			if (!reserved[m]) // desired match is not reserved
			{			
				// centroid matches with previous.
				reserved[m] = true;
				x->mpCurrentCentroids[m].match = centroidList[i]->index;
			
				// mark continuous centroids.
				centroidList[i]->matchesPrevious = (x->mpCurrentCentroids[m].exists) && (d < x->mMatchDistance);
			}
			else	// not a continued centroid, just find room anywhere. 
			{
				centroidList[i]->matchesPrevious = false; 
				for(j=1; j<=x->mMaxCentroids; j++)
				{
					if (!reserved[j])
					{
						reserved[j] = true;
						x->mpCurrentCentroids[j].match = centroidList[i]->index;

						centroidList[i]->match = j; // reassign
						break;
					}
				}
			}
		}		
	}
	else // no matching
	{
		for (i=1; i <= new; i++)
		{
			x->mpCurrentCentroids[i].match = i;
			// to avoid note offs every frame, we assume connected centroids when not doing matching 	
			x->mpNewCentroids[i].matchesPrevious = x->mpCurrentCentroids[i].exists;	
		}
	}
		
	// copy new centroids to current
	for (i=1; i <= x->mMaxCentroids; i++)
	{
		m = x->mpCurrentCentroids[i].match;
		if (m)
		{
			x->mpCurrentCentroids[i] = x->mpNewCentroids[m];
		}
		else
		{
			x->mpCurrentCentroids[i] = gZeroCentroid;
		}
	}
	x->mCurrentCentroids = x->mNewCentroids;
}	
	
	

void reportCentroids(t_jit_centroids *x)
{
	Atom av[6];
	int i;
	int noteoff;
	float fx, fy, fp;
	short size = 4;

	// write "note-offs" for expired
	for (i = 1; i <= x->mMaxCentroids; i++) 
	{	
		noteoff = false;
		if(x->mpCurrentCentroids[i].exists)
		{
			if(x->mpPreviousCentroids[i].exists && !x->mpCurrentCentroids[i].matchesPrevious)
			{
				noteoff = true;
			}
		}
		else
		{
			if(x->mpPreviousCentroids[i].exists)
				noteoff = true;
		}
				
		if (noteoff)
		{
			jit_atom_setlong(&av[0], i); 
			jit_atom_setfloat(&av[1], 0.); 
			jit_atom_setfloat(&av[2], 0.); 
			jit_atom_setfloat(&av[3], 0.); 
			outlet_list(x->listout, 0L, size, av);
		}
	}
	
	// report each new centroid as list of index, x, y, pressure.
	// loop is to max number, not new, because we might have, say one centroid in slot 4.
	for (i = 1; i <= x->mMaxCentroids; i++)
	{
		if (x->mpCurrentCentroids[i].exists)
		{
			fp = (x->mpCurrentCentroids[i]).fp_sum * x->oneOverPixels * x->zscale;
			fx = ((x->mpCurrentCentroids[i]).fx + 0.5f) * x->oneOverWidth * x->xscale;
			fy = ((x->mpCurrentCentroids[i]).fy + 0.5f) * x->oneOverHeight * x->yscale;
			
			jit_atom_setlong(&av[0], i); 
			jit_atom_setfloat(&av[1], fx); 
			jit_atom_setfloat(&av[2], fy); 
			jit_atom_setfloat(&av[3], fp); 
			outlet_list(x->listout, 0L, 4, av);
		}	
	}
}


