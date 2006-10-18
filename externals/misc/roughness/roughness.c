#include "ext.h"
#include "math.h"

#define R_MAX_LEN 8192

typedef struct _mig
{
	t_object r_ob;
	long r_in0;
	void *r_out1;
	float *r_buffer;
	long r_bufferLen;
} t_rho;

void *rho_class;

void rho_anything(t_rho *x, t_symbol *msg, short argc, t_atom *argv);
void rho_list(t_rho *x, t_symbol *msg, short argc, t_atom *argv);
void rho_process(t_rho *x);
void rho_bang(t_rho *x);
void rho_assist(t_rho *x, void *b, long m, long a, char *s);
void *rho_new(double var, long nOsc, double oscamp);
void rho_free(t_rho *x);
void rho_tellmeeverything(t_rho *x);

//--------------------------------------------------------------------------

int main(void)
{
	setup((t_messlist **)&rho_class, (method)rho_new, (method)rho_free, (short)sizeof(t_rho), 0L, 0); 
	
	addbang((method)rho_bang);
	addmess((method)rho_anything, "anything", A_GIMME, 0);
	addmess((method)rho_list, "list", A_GIMME, 0);
	addmess((method)rho_assist, "assist", A_CANT, 0);
	addmess((method)rho_tellmeeverything, "tellmeeverything", 0L, 0);
	
	return 0;
}

void *rho_new(double var, long nOsc, double oscamp)
{
	t_rho *x;

	x = (t_rho *)newobject(rho_class); // create a new instance of this object
	
	x->r_out1 = floatout(x);	
	
	x->r_buffer = NULL;
	x->r_bufferLen = 0;
	   	
	return(x);
}

void rho_anything(t_rho *x, t_symbol *msg, short argc, t_atom *argv)
{
}

void rho_list(t_rho *x, t_symbol *msg, short argc, t_atom *argv)
{
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
	int i, j;
	float numerator, denominator, meanFreq, cbw, cbInterval, ratio, standardCurve, roughness;
	float cbInterval0 = 0.25;
	float index = 2; // the bigger index, the narrower the curve
	float e = expf(1); // 2.7182818=base of natural logs
	
	numerator = denominator = 0;	
	float f1, f2, a1, a2;
	
	for(i = 0; i < (x->r_bufferLen / 2) - 1; i++){
		f1 = x->r_buffer[i * 2];
		a1 = x->r_buffer[(i * 2) + 1];
		if(a1 > 0.){
		
			for(j = i + 1; j < (x->r_bufferLen / 2); j++){
				f2 = x->r_buffer[j * 2];
				a2 = x->r_buffer[(j * 2) + 1];
				if(a2 > 0.){
					//post("%.10f %.10f %.10f %.10f", f1, a1, f2, a2);
					
					meanFreq = (f1 + f2) / 2;
					cbw = 1.72 * powf(meanFreq, 0.65);
					cbInterval = (fabsf(f2 - f1)) / cbw;
					ratio = cbInterval / cbInterval0;
					//post( "%f, %f, %f\n", ( e * ratio ), exp ( -1 * ratio ), ratio );
					standardCurve = powf((e * ratio) * expf(-1 * ratio), index);
					//printf ( "%f\n", standardCurve );
					numerator += a1 * a2 * standardCurve;
					//printf ( "%f, %f, %f\n", gAmpData[i][j], gAmpData[k][l], numerator );
					
				}
			}
			denominator += a1 * a1;	
		}
	}
	//post("numerator: %f\ndenominator: %f\n", numerator, denominator);
	if(denominator > 0)
		roughness = numerator / denominator;
	else roughness = 0;
	outlet_float(x->r_out1, roughness);
	//post("roughness: %f", roughness);
	
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

void rho_tellmeeverything(t_rho *x)
{
}