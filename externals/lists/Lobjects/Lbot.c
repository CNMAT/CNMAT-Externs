// lbot file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2004, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

#define MAXSIZE 256

typedef struct lbot
{
	struct object ob;
	long dummy;
	Atom outList[MAXSIZE];
	Atom placeList[MAXSIZE];
	int outsize;
	int howMany;
	int sorted;
	int findmid;
	void *outL;
	void *outR;
} Lbot;

void *lbot_new();
void lbot_free();
void  lbot_List();
void  lbot_doList();
void  lbot_howMany();
void lbot_dumpList();
void assist();
void lbot_constant();
void lbot_Fconstant();
void lbot_setSort();
void lbot_sortoff();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lbot_new,(method)lbot_free,(short)sizeof(struct lbot),0L,A_DEFLONG,A_DEFLONG,0);
	addint((method)lbot_constant);
	addfloat((method)lbot_Fconstant);
	addinx((method)lbot_howMany,1);
	addbang((method)lbot_dumpList);
	addmess((method)lbot_setSort, "sort",0);
	addmess((method)lbot_sortoff, "sortoff",0);
	addmess((method)lbot_List, "list",A_GIMME,0);
	addmess((method)assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lbot_constant(x,n)
Lbot *x;
long n;
{
	if(n > 0)
	{
		x->outList[0].a_type = A_LONG;
		x->outList[0].a_w.w_long = n;
		x->outsize = 1;
		
		x->placeList[0].a_w.w_long = 0;
		outlet_int(x->outR,n);
		
		outlet_int(x->outL,0L);
	}
}

void lbot_Fconstant(x,n)
Lbot *x;
double n;
{
	if(n > 0)
	{
		x->outList[0].a_type = A_FLOAT;
		x->outList[0].a_w.w_float = n;
		x->outsize = 1;
		
		x->outList[0].a_type = A_LONG;
		x->placeList[0].a_w.w_long = 0;
		outlet_float(x->outR,n);
		outlet_int(x->outL,0L);
	}
}

void lbot_List(x,s,argc,argv)
Lbot *x;
Symbol *s;
int argc;
Atom *argv;
{
	long lowestSoFar;
	long where;
	double FlowestSoFar;
	double there;
	int i,k;
	
	if(x->howMany > 1)
		lbot_doList(x,s,argc,argv);
	else{  // quick version
		lowestSoFar = 2147483647;
		FlowestSoFar = (double)lowestSoFar;
		where = 0;
		for(i=0;i<argc;++i)   // find the higest member, put its location in where
			{
				switch(argv[i].a_type)
				{
				case A_LONG:
					if(argv[i].a_w.w_long < lowestSoFar)
					{
					lowestSoFar = argv[i].a_w.w_long;
					FlowestSoFar = (double)lowestSoFar;
					where = i;
					}
					break;
					
				case A_FLOAT:
				if(argv[i].a_w.w_float < FlowestSoFar)
					{
					FlowestSoFar = argv[i].a_w.w_float;
					lowestSoFar = (long)FlowestSoFar;
					where = i;
					
					}
					break;
					
				} // end of switch
			} // end of i loop
		x->outList[0]= argv[where];
			
		if(x->findmid)
			{
			there = (double)where;
			k = 1; //where points to first of the equal values, set k to last of them
			for(k = 1; where + k < argc; ++k)
				{
				if(argv[where].a_type == A_LONG)
					{
					if(argv[where+k].a_w.w_long != argv[where].a_w.w_long)
					break;
					}
				else
					{
					if(argv[where + k].a_w.w_float != argv[where].a_w.w_float)
					break;
					}
				}
				
			if(where+k < argc) // if equal values are at either end report "plateau point"
				{
				if(there == 0)
					there = k-1;
				else
					there += (double)(k-1)/2;
				}
			x->placeList[0].a_type = A_FLOAT;
			x->placeList[0].a_w.w_float = there;
		} // end of findmid
		else{
			x->placeList[0].a_type = A_LONG;
			x->placeList[0].a_w.w_long = where;
		}
		x->outsize = 1;
	lbot_dumpList(x);
	}
}


void lbot_doList(x,s,argc,argz)
Lbot *x;
Symbol *s;
int argc;
Atom *argz;
{
	int i,j;
	long howManyFound;
	long lowestSoFar;
	double FlowestSoFar;
	long where = 0;
	//Atom templist[MAXSIZE];
	Atom argv[MAXSIZE];
	int tempPlaces[MAXSIZE];
	int lowestPlace;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	howManyFound = 0;
	if(argc <= x->howMany)  // only one pass through list is necessary, discard 0s
	{
		for(i=0;i < argc;++i)
		{
			switch(argz[i].a_type)
			{
			case A_LONG:
				if(argz[i].a_w.w_long)
				{
				x->outList[howManyFound].a_type = A_LONG;
				x->outList[howManyFound].a_w.w_long = argz[i].a_w.w_long;
				x->placeList[howManyFound].a_type = A_LONG;
				x->placeList[howManyFound].a_w.w_long = i;
				++howManyFound;
				}
				break;
				
			case A_FLOAT:
			if(argz[i].a_w.w_float)
				{
				x->outList[howManyFound].a_type = A_FLOAT;
				x->outList[howManyFound].a_w.w_float = argz[i].a_w.w_float;
				x->placeList[howManyFound].a_type = A_LONG;
				x->placeList[howManyFound].a_w.w_long = i;
				++howManyFound;
				}
				break;
			}
		}
		x->outsize = howManyFound;
	}
	else  // go through once per output member
	{
		for(i=0;i < argc;++i)  // copy input to argv
		{
			argv[i] = argz[i];
		}
			
		
		for(j=0;j<x->howMany;++j)
		{
			lowestSoFar = 2147483647;
			FlowestSoFar = (double)lowestSoFar;
			for(i=0;i<argc;++i)   // find the higest member, put its location in tempPlaces[j]
				{
					switch(argv[i].a_type)
					{
					case A_LONG:
						if(argv[i].a_w.w_long < lowestSoFar)
						{
							lowestSoFar = argv[i].a_w.w_long;
							FlowestSoFar = (double)lowestSoFar;
							where = i;
							
						}
						break;
						
					case A_FLOAT:
					if(argv[i].a_w.w_float < FlowestSoFar)
						{
							FlowestSoFar = argv[i].a_w.w_float;
							lowestSoFar = (long)FlowestSoFar;
							where = i;
							
						}
						break;
						// don't test Syms
					} // end of switch
			} // end of i loop
		tempPlaces[j] = where;
		argv[where].a_type = A_SYM;
		} // end of J loop
			//post("howmanyfound %ld",howManyFound);
		x->placeList[0].a_type = A_LONG;
			
		if(x->sorted){  // just copy the list
			for( i = 0; i < x->howMany; ++i){
				
				x->placeList[i].a_w.w_long = tempPlaces[i];
				x->outList[i] = argz[tempPlaces[i]];
			}
		}
		else  // sort by where
		{
			for(j = 0; j < x->howMany; ++ j)
			{
				lowestPlace = MAXSIZE;
				for(i = 0; i < x->howMany; ++i){
					if(tempPlaces[i]<lowestPlace) {
						lowestPlace = tempPlaces[i];
						where = i;
					}
				}
				x->placeList[j].a_w.w_long = tempPlaces[where];
				tempPlaces[where] = MAXSIZE;
				
			}
			for(i = 0; i < x->howMany; ++i){
				x->outList[i] = argz[x->placeList[i].a_w.w_long];
			}
		
		}
		x->outsize = x->howMany;
	} // end of once per member else 	
	
	lbot_dumpList(x);
}

void lbot_setSort(x)
Lbot *x;
{
	x->sorted = 1;
}

void lbot_sortoff(x)
Lbot *x;
{
	x->sorted = 0;
}


void  lbot_howMany(x,n)
Lbot *x;
long n;
{
	if(n < 1)
		{
		x->findmid = 1;
		n = 1;
		}
		else
		x->findmid = 0;
	x->howMany = n;
}



void lbot_dumpList(x)
Lbot *x;
{
	if(x->outsize){
		if(x->outsize == 1){
			switch(x->outList[0].a_type)
			{
			 case A_LONG:
				outlet_int(x->outR,x->outList[0].a_w.w_long);
				break;
				
			case A_FLOAT:
				outlet_float(x->outR,x->outList[0].a_w.w_float);
				break;
			
			}
			switch(x->placeList[0].a_type)
			{
			 case A_LONG:
				outlet_int(x->outL,x->placeList[0].a_w.w_long);
				break;
				
			case A_FLOAT:
				outlet_float(x->outL,x->placeList[0].a_w.w_float);
				break;
			
			}
		}
		else{
		outlet_list(x->outR,0L,x->outsize,x->outList);	
		outlet_list(x->outL,0L,x->outsize,x->placeList);
		}
	}
}

void assist(x,b,msg,arg,dst)
Lbot *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to Process");
			break;
			
			default:
			strcpy(dst,"Number of values to find");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Positions of lowest values");
			break;
			
			default:
			strcpy(dst,"List of lowest values");
			break;
		}
	}
}


void  *lbot_new(arg1,arg2)
long arg1,arg2;
{
	Lbot  *x;
	int i;
	int midfind;
	
	if(arg1 <= 0) 
		{
		arg1 = 1;
		midfind =1;
		}
		else
		midfind = 0;
	if(arg1 > MAXSIZE) arg1 = MAXSIZE;
	x = (Lbot *)newobject(class);
	intin(x,1);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	
	for (i = 0; i < MAXSIZE; ++ i){
	x->outList[i].a_type = A_LONG;
	x->outList[i].a_w.w_long = 0;
	x->placeList[i].a_type = A_LONG;
	x->placeList[i].a_w.w_long = 0;
	
	}
	x->outsize = 1;
	x->howMany = arg1;
	x->sorted = arg2;
	x->findmid = midfind;
	
	
	return(x);
}

void lbot_free(x)
Lbot *x;
{
	
	

}