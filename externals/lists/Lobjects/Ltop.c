// ltop file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1994, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

#define MAXSIZE 256

typedef struct ltop
{
	struct object ob;
	long dummy;
	Atom outList[MAXSIZE];
	Atom placeList[MAXSIZE];
	int outsize;
	int howMany;
	int sorted;
	int findmid;
	int donegs;
	void *outL;
	void *outR;
} Ltop;


void *ltop_new();
void ltop_free();
void  ltop_List();
void  ltop_doList();
void  ltop_howMany();
void ltop_dumpList();
void assist();
void ltop_constant();
void ltop_Fconstant();
void ltop_setSort();
void ltop_setNegMode();
void ltop_sortoff();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)ltop_new,(method)ltop_free,(short)sizeof(struct ltop),0L,A_DEFLONG,A_DEFLONG,0);
	addint((method)ltop_constant);
	addfloat((method)ltop_Fconstant);
	addinx((method)ltop_howMany,1);
	addbang((method)ltop_dumpList);
	addmess((method)ltop_setSort, "sort",0);
	addmess((method)ltop_setNegMode, "negatives",A_DEFLONG,0);
	addmess((method)ltop_sortoff, "sortoff",0);
	addmess((method)ltop_List, "list",A_GIMME,0);
	addmess((method)assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}



void ltop_constant(x,n)
Ltop *x;
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

void ltop_Fconstant(x,n)
Ltop *x;
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

void ltop_List(x,s,argc,argv)
Ltop *x;
Symbol *s;
int argc;
Atom *argv;
{
	long highestSoFar;
	long where;
	double FhighestSoFar;
	double there;
	int i,k;
	
	if(x->howMany > 1)
		ltop_doList(x,s,argc,argv);  // the hard way
	else{  // quick version
		if(x->donegs)
			highestSoFar = -2147483647;
		else 
			highestSoFar = 0;
		FhighestSoFar = (double)highestSoFar;
		where = 0;
		for(i=0;i<argc;++i)   // find the higest member, put its location in where
			{
				switch(argv[i].a_type)
				{
				case A_LONG:
					if(argv[i].a_w.w_long > highestSoFar)
					{
					highestSoFar = argv[i].a_w.w_long;
					FhighestSoFar = (double)highestSoFar;
					where = i;
					}
					break;
					
				case A_FLOAT:
				if(argv[i].a_w.w_float > FhighestSoFar)
					{
					FhighestSoFar = argv[i].a_w.w_float;
					highestSoFar = (long)FhighestSoFar;
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
	ltop_dumpList(x);
	}
}


void ltop_doList(x,s,argc,argz)
Ltop *x;
Symbol *s;
int argc;
Atom *argz;
{
	int i,j;
	int foundone;
	long howManyFound;
	long highestSoFar;
	double FhighestSoFar;
	long where = 0;
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
		//x->outsize = howManyFound;
	}
	else  // find the top values
	{
		for(i=0;i < argc;++i)  // copy input to argv
		{
			argv[i] = argz[i];
			if(!x->donegs) // remove 0s and lower from consideration
				switch(argv[i].a_type)
				{
					case A_LONG:
					if(argv[i].a_w.w_long <= 0)
					argv[i].a_type = A_SYM;  // syms are ignored in argv
					break;
					
					case A_FLOAT:
					if(argv[i].a_w.w_float <= 0.0)
					argv[i].a_type = A_SYM;
					break;
				}
		}
		howManyFound = 0;
		
		for(j=0;j<x->howMany;++j) // look for up to x->howmany values
		{
			foundone = 0;
			highestSoFar = -2147483647;
			FhighestSoFar = (double)highestSoFar;
			for(i=0;i < argc;++i)   // find the higest member, put its location in tempPlaces[j]
			{
				switch(argv[i].a_type)
				{
					case A_LONG:
					if(argv[i].a_w.w_long > highestSoFar){
						highestSoFar = argv[i].a_w.w_long;
						FhighestSoFar = (double)highestSoFar;
						where = i;
						
						foundone = 1;
					}
					break;
					
					case A_FLOAT:
					if(argv[i].a_w.w_float > FhighestSoFar){
						FhighestSoFar = argv[i].a_w.w_float;
						highestSoFar = (long)FhighestSoFar;
						where = i;
						foundone = 1;
					}
					break;
					// don't test Syms, they've alrady been found
				} // end of switch
			} // end of i loop
			if(foundone){
			++ howManyFound;
			tempPlaces[j] = where;
			argv[where].a_type = A_SYM;
			}
		} // end of J loop
			//post("howmanyfound %ld",howManyFound);
		x->placeList[0].a_type = A_LONG;
			// fill outlist
		if(x->sorted){  // just reverse the list
			for( i = 0; i < howManyFound; ++i){
				j = howManyFound - i -1;
				x->placeList[i].a_w.w_long = tempPlaces[j];
				x->outList[i] = argz[tempPlaces[j]];
			}
		}
		else  // sort by where
		{
			for(j = 0; j < howManyFound; ++ j)
			{
				lowestPlace = MAXSIZE;
				for(i = 0; i < howManyFound; ++i){
					if(tempPlaces[i]<lowestPlace) {
						lowestPlace = tempPlaces[i];
						where = i;
					}
				}
				x->placeList[j].a_w.w_long = tempPlaces[where];
				tempPlaces[where] = MAXSIZE;
				
			}
			for(i = 0; i < howManyFound; ++i){
				x->outList[i] = argz[x->placeList[i].a_w.w_long];
			}
		}
	} 
	x->outsize = howManyFound;
	ltop_dumpList(x);
		
	
}

void ltop_setSort(x)
Ltop *x;
{

	
		x->sorted = 1;
	
}

void ltop_setNegMode(x,n)
Ltop *x;
int n;
{

	
		x->donegs = n;
	
}

void ltop_sortoff(x)
Ltop *x;
{

	
		x->sorted = 0;
	
}


void  ltop_howMany(x,n)
Ltop *x;
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



void ltop_dumpList(x)
Ltop *x;
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
Ltop *x;
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
			strcpy(dst,"Positions of top values");
			break;
			
			default:
			strcpy(dst,"List of top values");
			break;
		}
	}
}


void  *ltop_new(arg1,arg2)
long arg1,arg2;
{

	Ltop  *x;
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
	x = (Ltop *)newobject(class);
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
	x->donegs = 0;
	
	return(x);
}

void ltop_free(x)
Ltop *x;
{
}