// banger  an object to spread bangs around   Oct 1994   
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1994, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct banger
{
	struct object b_ob;
	long lastOutlet;
	long recentOutlet;
	long mode;
	long direction;
	void *b_out[32];
} Banger;

void *banger_new();
void banger_bang();
void banger_select();
void banger_assist();
void banger_mode();
void banger_set();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method) banger_new, 0L, (short)sizeof(Banger), 0L, A_DEFLONG,A_DEFLONG, 0);
	addbang((method)banger_bang);
	addint((method)banger_select);
	addinx((method)banger_mode,1);
	addmess((method)banger_assist, "assist", A_CANT,0);
	addmess((method)banger_set, "set", A_GIMME,0);
	
	finder_addclass("Control", "banger");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}


void banger_mode(x,n)
Banger *x;
long n;
{
	
	x->mode = n;   // modes: 0 = up 1 = down  2 = back and forth
	if(n !=2)  x->direction = n;
	
}

void banger_bang(x)
Banger *x;
{

	
	switch(x->mode)
	{
		case 0:
		if(x->recentOutlet == x->lastOutlet)
			x->recentOutlet = 0;
			else
			++x->recentOutlet;
		break;
		
		case 1:
		if(x->recentOutlet == 0)
			x->recentOutlet = x->lastOutlet;
			else
			--x->recentOutlet;
		break;
		
		case 2:
		if(x->direction==0)  //direction:  0 = up 1 = down
		{
			if(x->recentOutlet == x->lastOutlet)
				{
				--x->recentOutlet;
				x->direction = 1;
				}
			else
				++x->recentOutlet;
		}
		else  // going backwards
		{
		if(x->recentOutlet == 0)
			{
			++x->recentOutlet;
			x->direction = 0;
			}
		else
			--x->recentOutlet;
		}
			
		break;
		
	}
	outlet_bang(x->b_out[x->recentOutlet]);
	
}

void banger_select(x,n)
Banger *x;
long n;
{

	
	if(n > -1)
	{
	if(n <= x->lastOutlet)
		{
		x->recentOutlet = n;
		outlet_bang(x->b_out[x->recentOutlet]);
		}
	}
	
}

void banger_set(x,s,argc,argv)
Banger *x;
Symbol *s;
int argc;
Atom *argv;

{
int n;

	
	n = argv[0].a_w.w_long;
	if(x->direction==1) 
		++n;
	else 
		--n;
	if(n < 0)
		n = x->lastOutlet;
	if(n > x->lastOutlet)
		n = 0;
	x->recentOutlet = n;
		
	
	
}

void banger_assist(x,b,msg,arg,dst)
Banger *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Bang here");
			break;
			
			default:
			strcpy(dst,"List to Store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Bang in sequence");//forgot something
			break;
			
			default:
			strcpy(dst,"Bang in sequence");
			break;
		}
	}
	
}


void *banger_new(numOuts,modearg)
long numOuts;
long modearg;
{
	Banger *x;
	int i;
	
	x = (Banger *)newobject(class);
	intin(x,1);
	x->mode = modearg;
	x->direction = 0;
	if(numOuts < 2) numOuts = 2;
	if(numOuts >32) numOuts = 32;
	for(i= numOuts; i > 0; --i)
		{
			x->b_out[i-1] = bangout(x);
		}
	x->lastOutlet= numOuts-1;
	
	if(modearg==0)x->recentOutlet = numOuts-1;
	if(modearg == 1) x->recentOutlet = 0;
	if(modearg == 2) x->recentOutlet = x->direction=1;
	

	
	return(x);
}