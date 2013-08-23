#ifndef granubuf__granu_h
#define granubuf__granu_h

#include <math.h>
#include <inttypes.h>
#undef PI
#define PI 3.14159265358979323846264338327950288419716939937510

#undef E
#define E  2.71828182845904523536028747135266249775724709369995

#define GRANU_SINCRIPPLES 5  //could probably make this an attribute, or maybe compute sinc in realtime... 5 seems good for now

#define DEFAULTMAXOSCILLATORS 256
#define GRANU_MAX_BUFFERS 128

#define TPOW 16
#define STABSZ (1l<<TPOW) //table size

#define NSHAPINGTABS (1l<<9)  //number of shaping tables

#define GRANUBUF_NUMINTERNAL_WINDOWS 4

typedef enum _granu_window
{
    EXTERNAL = -1,
    COS,
    FOF,//special case, requires cos, and expdecay, so labeling expdecay as fof
    SINC,
    DAMPEDSINE
    //add hanning
} e_granu_window;

//********** globals
int granu_tableExists = 0;
int granu_shapingTableExists = 0;

//window functions
t_float **granu_internal_window;

/*
double granu_wind_costab[STABSZ];
double granu_expdecaytab[STABSZ];
double granu_dampedsinetab[STABSZ];
double granu_sincwindow[STABSZ];
*/

//for granusoids~ and granufm~
double granu_sinetab[STABSZ];

//shaping functions
double granu_exptab[NSHAPINGTABS][STABSZ];
double granu_coefshape[NSHAPINGTABS];


static t_symbol *ps_granu_cos;
static t_symbol *ps_granu_fof;
static t_symbol *ps_granu_dampedsine;
static t_symbol *ps_granu_sinc;
static t_symbol *ps_buffer_modified;

static t_symbol *ps_granu_trigger, *ps_granu_location, *ps_granu_rate, *ps_granu_dur, *ps_granu_shapex, *ps_granu_shapey, *ps_granu_win_idx, *ps_granu_outlet, *ps_granu_buf_idx, *ps_granu_amp, *ps_granu_tex;

//********************** utils

double wrapPhase(double p_current, double tablesize)
{
    if( p_current < 0)
        return fmod(p_current, tablesize) + tablesize;
    else return fmod(p_current, tablesize);
}


double linear_interp( double v0, double v1, double t)
{
    return v0+((v1-v0)*t);
}

double granu_shapeFunction( double *w_tab, double w_frames, uint32_t wpc, uint32_t xshapetab, uint32_t yshapetab, long tabscale )
{
    return granu_exptab[ yshapetab ][
                              (uint32_t)(w_tab[(uint32_t)(granu_exptab[ xshapetab ][ (uint32_t)wpc ] * (w_frames - 1)) ] * tabscale)
                              ];
}


/*
 //maybe something like this for slowed down audio, but need to check more on resampling, and probably needs a filter...
 // from polynomal_interpolation.pdf ( http://yehar.com/blog/wp-content/uploads/2009/08/deip.pdf )
 
double optimal2x_interp(double x, double *y )
{
    // Optimal 2x (6-point, 5th-order) (z-form)
    float z = x - 1/2.0;
    float even1 = y[1]+y[0], odd1 = y[1]-y[0];
    float even2 = y[2]+y[-1], odd2 = y[2]-y[-1];
    float even3 = y[3]+y[-2], odd3 = y[3]-y[-2];
    float c0 = even1*0.40513396007145713 + even2*0.09251794438424393
    + even3*0.00234806603570670;
    float c1 = odd1*0.28342806338906690 + odd2*0.21703277024054901
    + odd3*0.01309294748731515;
    float c2 = even1*-0.191337682540351941 + even2*0.16187844487943592
    + even3*0.02946017143111912;
    float c3 = odd1*-0.16471626190554542 + odd2*-0.00154547203542499
    + odd3*0.03399271444851909;
    float c4 = even1*0.03845798729588149 + even2*-0.05712936104242644
    + even3*0.01866750929921070;
    float c5 = odd1*0.04317950185225609 + odd2*-0.01802814255926417
    + odd3*0.00152170021558204;
    return ((((c5*z+c4)*z+c3)*z+c2)*z+c1)*z+c0;
}
*/

//********************** wave table functions


void SincFunction(int n, t_float *tab, int stride, double from, double to)
{
    int j;
    double f = (to-from) / n;
    double t;
    
	for( j=0; j<n; ++j )
	{
        t = (from + (j * f));
        if (t == 0.0)
        {
            tab[ j * stride ] = 1.0; //avoid divide by zero!
        }
        else
        {
            tab[ j * stride ] = fabs( sin(PI * t) / (PI * t) ); // ABS version
        }
	}
};


void DampedSineFunction(int n, t_float *tab, int stride, double from, double to, double k)
{
    int j;
    double f = (to-from) / n;
    double t;
    
	for( j=0; j<n; ++j )
	{
        t = (from + (j * f));
        
		tab[ j * stride ] = fabs( pow( E, -k * t ) * sin( 2.0 * PI * t ) ); // ABS version
        
	}
};



void CosWindowFunction(int n, t_float *tab, int stride, double from, double to)
{
    int j;
    double f = (to-from) / n;
    double t;
    
	for( j=0; j<n; ++j )
	{
        t = (from + (j * f));
        
		tab[ j * stride ] = 1 - (( cos( t ) * 0.5) + 0.5);
        
	}
}



void SineFunction(int n, double *stab, int stride, double from, double to)
{
	int j;
	double f = (to-from) / n;
    
	for( j=0; j<n; ++j )
	{
		stab[ j * stride ] = sin( from + j * f );
        
	}
}



void ExpDecayFunction(int n, t_float *stab, int stride, double from, double to)
{
    int j;
	double f = (to-from) / n;
    
	for( j=0; j<n; ++j )
	{
		stab[ j * stride ] = exp( -(from + j * f) );
        
	}
    
}

void PowTableFunction(int n, double *tab, int stride, double from, double to, double exp)
{
    int j;
    double f = (to-from) / n;
    
	for( j=0; j<n; ++j )
	{
		tab[ j * stride ] = pow(from + j * f, exp );
	}
    
}

void granu_makeTables()
{
    
    granu_internal_window = (t_float **)malloc(GRANUBUF_NUMINTERNAL_WINDOWS * sizeof(t_float *));

    int i;
    for(i=0; i<GRANUBUF_NUMINTERNAL_WINDOWS; i++)
    {
        granu_internal_window[i] = (t_float *)malloc(STABSZ * sizeof(t_float));
        
        switch (i) {
            case COS:
                CosWindowFunction(STABSZ, granu_internal_window[COS], 1, 0.0, 2.0 * PI);
                break;
            case FOF: //special case, requires cos, and expdecay, so labeling expdecay as fof for simplicity for table lookup
                ExpDecayFunction(STABSZ, granu_internal_window[FOF], 1, 0.0, 5.0);
                break;
            case SINC:
                SincFunction(STABSZ, granu_internal_window[SINC], 1, (double)-(GRANU_SINCRIPPLES), (double)GRANU_SINCRIPPLES);
                break;
            case DAMPEDSINE:
                DampedSineFunction(STABSZ, granu_internal_window[DAMPEDSINE], 1, 0.0, 5.0, 1.0);
                break;
            default:
                break;
        }
        
    }

    SineFunction(STABSZ, granu_sinetab, 1, 0.0, 2.0 * PI);
    PowTableFunction(NSHAPINGTABS, granu_coefshape, 1, 0.0001, 1.0, 2.0);
    
    granu_tableExists = 1;
    
    /*
     ExpDecayFunction(STABSZ, granu_expdecaytab, 1, 0.0, 5.0);
     SincFunction(STABSZ, granu_sincwindow, 1, (double)-(GRANU_SINCRIPPLES), (double)GRANU_SINCRIPPLES);
     DampedSineFunction(STABSZ, granu_dampedsinetab, 1, 0.0, 5.0, 1.0);
     CosWindowFunction(STABSZ, granu_wind_costab, 1, 0.0, 2.0 * PI);
     SineFunction(STABSZ, granu_sinetab, 1, 0.0, 2.0 * PI);
     */

}

void granu_makeShapeTables()
{
    if(!granu_tableExists)
    {
        error("unable to build shaping table");
        return;
    }
    
    long i;
    for(i = 0; i < NSHAPINGTABS; i++){
        PowTableFunction(STABSZ, granu_exptab[i], 1, 0.0001, 1.0, granu_coefshape[i] * 10.0);
    }
    granu_shapingTableExists = 1;
}


#endif
