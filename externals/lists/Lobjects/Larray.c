// larray file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1995, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#define XVERSION 1  // hide for classic

#include "ext.h"
#include "ext_path.h"
#include <string.h>
#include "ext_common.h"
#include "ext_critical.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

enum types {FLOATS,INTS,BYTES,TEXT};
typedef struct larray
{
	struct object ob;
	Atom theList[MAXSIZE];  //local copy of the input list
	int listsize;			// size of theList
	void *proxyR;   		// Right inlet is Proxy type
	long inletNumber;
	long ArraySize;         //number of entries in array
	long realArraySize;		//number of bytes in array
	long index;				// for accessing array
	Atom outList[MAXSIZE];		// copy of last output
	long outsize;			// size of outList
	double fvalue;			// value to store in array when index arrrives
	long  lvalue;
	short bvalue;
	int storeflag;			// value needs to be stored
	int listflag;			// list needs to be stored
	char **aHandle;			// handle to array
	void *out;				// outlet
	enum types theType;
	
	/* from coll.c filename, pathIDume, and MAX type */
	Symbol *fn;
	short pathID;
//	short bin;
} Larray;

void  larray_Lists();
void *larray_new();
void larray_free();
void  larray_setConstant();
void  larray_floatConstant();
void  larray_clear();
void  larray_listout();
void larray_assist();
void larray_read();
void larray_write();
void larray_readfile();
void larray_writefile();
void  doStoreOrRead();
int check_inlet();
void *Larray_class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&Larray_class,(method)larray_new,(method)larray_free,(short)sizeof(struct larray),0L,A_GIMME,0);
	addint((method)larray_setConstant);
	addfloat((method)larray_floatConstant);
	addmess((method)larray_listout, "get",A_GIMME,0);
	addmess((method)larray_Lists, "list",A_GIMME,0);
	addmess((method)larray_read, "read",A_DEFSYM,0);
	addmess((method)larray_write, "write",A_GIMME,0);
	addmess((method)larray_assist, "assist",A_CANT,0);
	addmess((method)larray_clear, "clear",0);
	//finder_addclass("Data","Larray");
	
	

	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
	}

void larray_listout(x,s,argc,argv)
Larray *x;
Symbol *s;
int argc;
Atom *argv;

{
	double *floatArray;  // data is accessed as Array[x->index]
	long *longArray;
	short *shortArray;
	short temp;
	long i;
	void *voidPtr;		// voidPtr is always needed to equate bigArray to x->aHandle
	
	
	if(argc > 1)  						// index argument to get command
		x->index = argv[1].a_w.w_long;
	if(argc > 0)						// length argument to get command
		x->outsize = argv[0].a_w.w_long;
	if(x->outsize > MAXSIZE)
		x->outsize = MAXSIZE;
	HLock(x->aHandle);  	// always lock x->aHandle before assigning it to bigArray
	voidPtr = *x->aHandle;  // fool the compiler
	floatArray = voidPtr;
	longArray = voidPtr;
	shortArray = voidPtr;
	
	
	if((x->index >= 0) && ((x->index + x->outsize) < x->ArraySize))
	{
		for(i = 0; i < x->outsize; ++i)
			{
			switch(x->theType)
				{
				case FLOATS:
				x->outList[i].a_w.w_float = floatArray[x->index + i];
				break;
				
				case INTS:
				x->outList[i].a_w.w_long = longArray[x->index + i];
				break;
				
				case BYTES:
				temp = shortArray[(x->index + i)/2];
				if((x->index + i) % 2  == 0) // even address 
					{
					x->outList[i].a_w.w_long = 0L | (temp & 0xFF);
					}
				else   			// odd adress, take
					{
					x->outList[i].a_w.w_long = 0L | ((temp >> 8) & 0xFF);
					}
					break;
					
					
				case TEXT:
				temp = shortArray[(x->index + i)/2];
				if((x->index + i) % 2  == 0) // even address 
					{
					x->outList[i].a_w.w_long = 0L | ((temp >> 8) & 0xFF);
					}
				else   			// odd adress, take
					{
					x->outList[i].a_w.w_long = 0L | (temp & 0xFF);
					}
				}
			
			}
			
		outlet_list(x->out,0L,(int)x->outsize,x->outList);
		
		x->index += x->outsize;
		if(x->index > x->ArraySize) x->index = 0;
	}
	else
	{
	post("Larray: index value out of range");
	x->index = 0;
	}

	HUnlock(x->aHandle);
	
}

void larray_read(x,s)  // file operations must be deferred
Larray *x;
Symbol *s;

{

	
	defer_low(x,(method)larray_readfile,s,0,NULL);
	
}

void larray_write(x,s,argc,argv)  // file operations must be deferred
Larray *x;
Symbol *s;
int argc;
Atom *argv;

{
	
	
	defer_low(x,(method)larray_writefile,s,argc, argv);
	
}

void larray_readfile(x,s,argc,argv)  // User Interface lifted from coll
Larray *x;
Symbol *s;
int argc;
Atom *argv;
{

	long types[6];
	long typefound;
	t_filehandle theFileHandle;

	long i;
	long type;
	char name[256];
	//Str255 filename;
	double *bigArray;
	void *voidPtr;
	
	long incount;
	
	
	name[0] = 0;
	if(x->theType == TEXT)
	types[0] ='TEXT';
	else
	types[0]='maxb';

	if (s != gensym(""))   // "" is an empty symbol
	{
		if (locatefile_extended (s->s_name, &x->pathID, &typefound, types, 1)) 
			{
			ouchstring("Couldn't find %s. Please locate the file, or an alternate.",
				s->s_name);
			x->fn=gensym("");
			s = gensym("");
			}
		else
			{
			x->fn = s;
			i = 0;
			while(s->s_name[i])
				{
				name[i] = s->s_name[i];
				++i;
				}
			name[i] = 0;
			}
	}
	
	if(s == gensym(""))  // noname or user bailed
	{
		if (open_dialog(name, &x->pathID, &type, types, 1)) 
		{
		
		return;
		}
		else
			x->fn = gensym(name);
			
	}
	post("Larray: reading %s",x->fn->s_name);
	#ifndef XVERSION
	if(path_openfile(name,x->pathID,&refNum,READ_PERM))
		{
		ouchstring("Error Opening %s",x->fn->s_name);
		return;
		}
	
	prevLock = lockout_set(1);  //Stop interrupts
	#else
	if(path_opensysfile(name,x->pathID,&theFileHandle,READ_PERM))
		{
		ouchstring("Error Opening %s",x->fn->s_name);
		
		return;
		}
	critical_enter(0);  //Stop interrupts
	#endif
	
	HLock(x->aHandle);          // and lock memory
	voidPtr = *x->aHandle;		// fool the compiler
	bigArray = voidPtr;
	incount = x->realArraySize;  // read no more than incount bytes
	
	#ifndef XVERSION
	FSRead(refNum,&incount,bigArray);
	FSClose(refNum);
	lockout_set(prevLock);
	#else
	sysfile_read(theFileHandle,&incount,bigArray);
	sysfile_close(theFileHandle);
	critical_exit(0);
	#endif
	HUnlock(x->aHandle);
	switch(x->theType)
	{ 
	case FLOATS:
	post("Larray: %ld floats read",incount/sizeof(double)); // incount is now number of bytes read
	break;
	
	case INTS:
	post("Larray: %ld ints read",incount/sizeof(long)); // incount is now number of bytes read
	break;
	
	case BYTES:case TEXT:
	post("Larray: %ld bytes read",incount); // incount is now number of bytes read
	break;
	}
	
}


void larray_writefile(x,dummy,argc,argv)
Larray *x;
Symbol *dummy;
int argc;
Atom *argv;
{

	OSType type,creator;
	t_filehandle theFileHandle;
	long i;
	Symbol *s;
	char name[256];
	//Str255 filename;
	double *bigArray;
	void *voidPtr;
	long outcount;
	long result;
	long writesize;
	
	name[0] = 0;
	if(x->theType == TEXT)
	type ='TEXT';
	else
	type ='maxb';
	creator = '????';
	writesize = x->ArraySize;
	s = gensym("");
	if(argc)  // user may enter length, name or name, length
		{
			if(argv[0].a_type == A_LONG)   // just a length specifier
				writesize = argv[0].a_w.w_long;
			if(argv[0].a_type == A_SYM)		// a file name
				s = argv[0].a_w.w_sym;
		}
	if(argc > 1)
		{
			if(argv[1].a_type == A_LONG)  // a length specifier
				writesize = argv[1].a_w.w_long;
			if(argv[1].a_type == A_SYM)  // again file name
				s = argv[1].a_w.w_sym;
		}
	
	if(s == gensym(""))  // no name specified
		{
		if (saveas_dialog(name, &x->pathID, 0L)) // user bailed
			{
			
			return;
			}
		else
			x->fn = gensym(name);
			
		}
	else   // name was specified
		{
		x->fn = s;
		i = 0;
		while(s->s_name[i])
			{
			name[i] = s->s_name[i];
			++i;
			}
		name[i] = 0;
		}
	#ifndef XVERSION
	prevLock = lockout_set(1); //old style
	#else
	critical_enter(0);  // turn off interrupts
	#endif
	HLock(x->aHandle);			// and lock memory
	voidPtr = *x->aHandle;
	bigArray = voidPtr;
	switch(x->theType)
	{
		case BYTES:
		case TEXT:
		if(writesize % 2 == 0)
		outcount = (writesize / 2) * sizeof(short);
		else
		outcount = (writesize / 2 +1)*sizeof(short);
		break;
		
		case FLOATS:
		outcount = writesize*sizeof(double);
		break;
		
		case INTS:
		outcount = writesize*sizeof(long);
		break;
	}
	
	#ifndef XVERSION
	result = path_createfile(name,x->pathID,type,&refNum);
	if(result & (result != dupFNErr))  // does not warn if specified file exists
		{
		post("Could not create %s! error#  %ld",x->fn->s_name,result);
		goto exitHere;  				// these error handlers are not tested
		}
	post("Ldumpster: writing %s",x->fn->s_name);
	
	result = FSWrite(refNum,&outcount,bigArray);
	if(result)  
		{
		post("Could not write to %s! error#  %ld",x->fn->s_name,result);
		}
CloseIt:
	result = FSClose(refNum);
	
	#else
	result = path_createsysfile(name,x->pathID,type,&theFileHandle);
	
	if(result & (result != dupFNErr))  // does not warn if specified file exists
		{
		post("Could not create %s! error#  %ld",x->fn->s_name,result);
		goto exitHere;  				// these error handlers are not tested
		}
	post("Larray: writing %s",x->fn->s_name);
	
	result = sysfile_write(theFileHandle,&outcount,bigArray);
	if(result)  
		{
		post("Could not write to %s! error#  %ld",x->fn->s_name,result);
		}
//CloseIt:
	result = sysfile_close(theFileHandle);
	#endif
	
	if(result)  
		{
		post("Could not close %s! error#  %ld",x->fn->s_name,result);
		goto exitHere;
		}
	switch(x->theType)
	{
	case FLOATS:
	post("Larray: %ld floats saved",outcount/sizeof(double));
	break;
	
	case INTS:
	post("Larray: %ld ints saved",outcount/sizeof(long));
	break;
	
	case BYTES: case TEXT:
	post("Larray: %ld bytes saved",outcount);
	break;
	}
exitHere:	
	HUnlock(x->aHandle);
	critical_exit(0);
	
	
}




void larray_Lists(x,s,argc,argv)  // handles list input
Larray *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet will be stored when index arrives
	{
	for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type)
			{ 
			case A_LONG:
			if(x->theType == FLOATS)
			x->theList[i].a_w.w_float = (double)argv[i].a_w.w_long;
			else
			x->theList[i].a_w.w_long = argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			if(x->theType == FLOATS)
			x->theList[i].a_w.w_float = argv[i].a_w.w_float;
			else
			x->theList[i].a_w.w_long = (long)argv[i].a_w.w_float;
			break;
			
			default:
			post("Invalid List Member");
			}
		}
	x->listflag = 1;
	x->storeflag = 1;
	x->listsize = argc;
	}
	else		// list in left inlet is index, value
		{
		if(argv[0].a_type == A_LONG)
		{
			for(i=1;i<argc;++i)
			{
				switch(argv[i].a_type)
				{ 
					case A_LONG:
					if(x->theType == FLOATS)
					x->theList[i-1].a_w.w_float = (double)argv[i].a_w.w_long;
					else
					x->theList[i-1].a_w.w_long = argv[i].a_w.w_long;
					break;
					
					case A_FLOAT:
					if(x->theType == FLOATS)
					x->theList[i-1].a_w.w_float = argv[i].a_w.w_float;
					else
					x->theList[i-1].a_w.w_long = (long)argv[i].a_w.w_float;
					break;
					
					default:
					post("Invalid List Member");
				}
			}
			x->listflag = 1;
			x->storeflag = 1;
			x->listsize = argc-1;
			
			doStoreOrRead(x,argv[0].a_w.w_long);
			}
		else
			{
				
				return;
			}
			
		x->storeflag = 0;  // list in left overrides any impending stores
		x->listflag = 0;
		
		}
}

void  larray_clear(x)
Larray *x;
{
	long i;
	double *floatArray;
	long *longArray;
	short *shortArray;
	void *voidPtr;
	
	HLock(x->aHandle);
	voidPtr = *x->aHandle;
	
	switch(x->theType)
	{
		case FLOATS:
		floatArray = voidPtr;
		for(i = 0; i< x->ArraySize;++i)
			{
				floatArray[i] = 0;
			}
		break;
		
		case INTS:
		longArray = voidPtr;
		for(i = 0; i< x->ArraySize;++i)
			{
				longArray[i] = 0;
			}
		break;
		
		case BYTES: case TEXT:
		shortArray = voidPtr;
		for(i = 0; i< (x->ArraySize/2 +1);++i)
			{
				shortArray[i] = 0;
			}
		break;
	}
	HUnlock(x->aHandle);
	x->index=0;
	x->storeflag = 0;
	
}


void  larray_setConstant(x,n)  // handle int input
Larray *x;
long n;
{

	if(check_inlet(x) )	// input is a number to be stored
		{
		switch (x->theType)
			{
			case INTS:
			x->lvalue = n;  	
			break;
			
			case FLOATS:
			x->fvalue =(double) n;  	
			break;
			
			case BYTES:
			case TEXT:
			x->bvalue = n & 0xFF;  	
			break;
			}
		x->storeflag = 1;
		x->listflag = 0;	
		}
	else					// input is location to store value
		doStoreOrRead(x,n);
	
}

void  larray_floatConstant(x,n)  //handle double inputs
Larray *x;
double n;
{
	
	
	
	if(check_inlet(x))	// n is value to be stored
		{
		switch (x->theType)
			{
			case INTS:
			x->lvalue =(long) n;  	
			break;
			
			case FLOATS:
			x->fvalue = n;  	
			break;
			
			case BYTES:
			case TEXT:
			x->bvalue = (short) n & 0xFF;  	
			break;
			}
		x->storeflag = 1;
		x->listflag = 0;
		}
	else	//  n is index
		doStoreOrRead(x,(long)n);
	
}

void  doStoreOrRead(x,n)  // handle left input
Larray *x;
long n;
{
	
	double *floatArray;
	long *longArray;
	short *shortArray;
	short temp;
	long i;
	void *voidPtr;
	
	
	HLock(x->aHandle);
	voidPtr = *x->aHandle;
	floatArray = voidPtr;
	longArray = voidPtr;
	shortArray = voidPtr;
	
	if((n >= 0) && (n < x->ArraySize))
	{
		x->index = n;
		if(x->storeflag == 1)
			{
			if(x->listflag == 1)  // store list beginning at n
				{
				for(i = 0; i<x->listsize; ++i)
					{
					switch(x->theType)
						{
						case FLOATS:
						floatArray[ x->index+i] =x->theList[i].a_w.w_float; 
						break;
						
						case INTS:
						longArray[ x->index+i] =x->theList[i].a_w.w_long; 
						break;
						
						case BYTES:
						if((x->index + i) % 2  == 0) // even address
							{
							temp = shortArray[(x->index + i)/2] & 0xFF00;
							temp += x->theList[i].a_w.w_long & 0xFF;
							}
						else			// odd address
							{
							temp = shortArray[(x->index + i)/2] & 0x00FF;
							temp += (x->theList[i].a_w.w_long & 0xFF) << 8;
							}
						shortArray[(x->index + i)/2] = temp;
						break;
						
						case TEXT:
						if((x->index + i) % 2  == 1) // odd address
							{
							temp = shortArray[(x->index + i)/2] & 0xFF00;
							temp += x->theList[i].a_w.w_long & 0xFF;
							}
						else			// even address
							{
							temp = shortArray[(x->index + i)/2] & 0x00FF;
							temp += (x->theList[i].a_w.w_long & 0xFF) << 8;
							}
						shortArray[(x->index + i)/2] = temp;
						break;
						}
					}
				x->storeflag = 0;
				x->listflag = 0;
				}
				else     // store value to n
				{
					
					switch(x->theType)
						{
						case FLOATS:
						floatArray[ n] =x->fvalue; 
						break;
						
						case INTS:
						longArray[ n] =x->lvalue; 
						break;
						
						case BYTES:
						if(n % 2 == 0) // even address
							{
							temp = shortArray[n/2] & 0xFF00;
							temp +=x->bvalue;
							shortArray[n/2] = temp;
							}
						else			// odd address
							{
							temp = shortArray[n/2] & 0x00FF;
							temp += x->bvalue << 8;
							shortArray[n/2] = temp;
							}
						break;
						
						case TEXT:
						if(n % 2 == 1) // odd address
							{
							temp = shortArray[n/2] & 0xFF00;
							temp +=x->bvalue;
							shortArray[n/2] = temp;
							}
						else			// even address
							{
							temp = shortArray[n/2] & 0x00FF;
							temp += x->bvalue << 8;
							shortArray[n/2] = temp;
							}
						}
					
					x->storeflag = 0;
				}
			}
		else  // report the value at n
			{
			
					switch(x->theType)
						{
						case FLOATS:
						outlet_float(x->out,floatArray[ n]); 
						break;
						
						case INTS:
						outlet_int(x->out,longArray[ n]); 
						break;
						
						case BYTES:
						if(n% 2  == 0) // even address
							{
							temp = shortArray[n/2] & 0xFF;
							
							}
						else			// odd address
							{
							temp = (shortArray[n/2] >> 8) & 0xFF;
							}
						outlet_int(x->out,(long)temp);
						
						break;
						
						case TEXT:
						if(n % 2  == 1) // odd address
							{
							temp = shortArray[n/2] & 0xFF;
							
							}
						else			// even address
							{
							temp = (shortArray[n/2] >> 8) & 0xFF;
							}
						outlet_int(x->out,(long)temp);
						
						break;
						}
			
			}
	}
	else
	post("Larray: index value out of range");
	
	HUnlock(x->aHandle);
	
}



void larray_assist(x,b,msg,arg,dst)
Larray *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Address to reference");
			break;
			
			default:
			strcpy(dst,"Data to Store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data out");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
}


void  *larray_new(s,argc,theArgs)
Symbol *s;
int argc;
Atom *theArgs;
{

	Larray  *x;
	double *floatArray;
	long *longArray;
	short *shortArray;
	void *voidPtr;
	long i;
	
	
	x = (Larray *)newobject(Larray_class);				// make an object
	x->proxyR = proxy_new(x,2L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	
	x->pathID= defvolume();							// initialize 
	x->fn = gensym("untitled");
	x->index = 0;
	x->storeflag = 0;
	x->listflag = 0;
	x->outsize = 1;
	x->theType = FLOATS;
							// set arraySize to default or arg
	x->ArraySize = MAXSIZE;
	if(argc && (theArgs[0].a_type == A_LONG))				
		x->ArraySize = theArgs[0].a_w.w_long;

	if(argc > 1)
		{
		if(theArgs[1].a_type == A_SYM)
			{
			switch(theArgs[1].a_w.w_sym -> s_name[0])
				{
				case 'l':
				case 'L':
				case 'I':
				case 'i':
				x->theType = INTS;
				break;

				case 'b':
				case 'B':
				x->theType = BYTES;
				break;
				
				case 't':
				case 'T':
				x->theType = TEXT;
				break;
				}
			}
		}
	switch(x->theType)
	{
		case FLOATS:
		x->realArraySize = x->ArraySize * sizeof(double);  
		break;
		
		case INTS:
		x->realArraySize = x->ArraySize * sizeof(long); 
		break;
		
		case BYTES: case TEXT:
		x->realArraySize =( x->ArraySize/2 +1) * sizeof(short); 
		break;
	}
	x->aHandle = newhandle(x->realArraySize);

	if(x->aHandle == NIL)
		{
		ouchstring("The amount of memory requested by Larray is not available");
		if(maxversion() >= 0x300)
		post("Larray %ld Failed: Allocate more memory to Max",x->ArraySize); // seems to be a problem
		x->ArraySize = 0;									// with newhandle
		
		return(x);

		}
		
	HLock(x->aHandle);
	voidPtr = *x->aHandle;
	
	switch(x->theType)
	{
		case FLOATS:
		floatArray = voidPtr;
		for(i = 0; i< x->ArraySize;++i)
			{
				floatArray[i] = 0;
			}
		break;
		
		case INTS:
		longArray = voidPtr;
		for(i = 0; i< x->ArraySize;++i)
			{
				longArray[i] = 0;
			}
		break;
		
		case BYTES: case TEXT:
		shortArray = voidPtr;
		for(i = 0; i< (x->ArraySize/2 +1);++i)
			{
				shortArray[i] = 0;
			}
		break;
	}
	HUnlock(x->aHandle);
	
	if(x->theType == FLOATS)
		for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_FLOAT;
		x->outList[i].a_type = A_FLOAT;
		x->theList[i].a_w.w_float = 0;
		}
	else
		for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->outList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	
	return(x);
}

void larray_free(x)
Larray *x;
{
	
	disposhandle(x->aHandle);
	freeobject(x->proxyR);
}

int check_inlet(x)
Larray *x;
{
	#ifndef XVERSION
	return x->inletNumber;  // classic only
	#else
	return proxy_getinlet((t_object *)x);  // OS X only
	#endif
}