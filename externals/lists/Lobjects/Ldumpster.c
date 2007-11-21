// ldumpster file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1995, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include "ext_common.h"
#include <string.h>
#include "ext_path.h"
#include "ext_critical.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

enum types {FLOATS,INTS,BYTES,TEXT};
typedef struct ldumpster
{
	struct object ob;
	Atom theList[MAXSIZE];  //local copy of the input list
	int listsize;			// size of theList
	void *proxyR;   		// Right inlet is Proxy type
	//long inletNumber;
	long ArraySize;         //number of entries in array
	long realArraySize;		//number of bytes in array
	long index;				// for accessing array
	long nextslot;			// for storing dumps
	int waitingForDump;
	long dumplength;
	long nextout;
	Atom outList[MAXSIZE];		// copy of last output
	long outsize;			// size of outList
	float fvalue;			// value to store in array when index arrrives
	long  lvalue;
	short bvalue;
	int storeflag;			// value needs to be stored
	int listflag;			// list needs to be stored
	char **aHandle;			// handle to array
	void *out;				// outlet
	void *outR;
	enum types theType;
	
	/* from coll.c filename, volume, and MAX type */
	Symbol *fn;
	short pathID;
} Ldumpster;

void  ldumpster_Lists();
void *ldumpster_new();
void ldumpster_free();
void  ldumpster_setConstant();
void  ldumpster_floatConstant();
void  ldumpster_clear();
void  ldumpster_ready();
void  ldumpster_done();
void  ldumpster_dump();
void  ldumpster_continue();
void  ldumpster_listout();
void ldumpster_assist();
void ldumpster_read();
void ldumpster_write();
void ldumpster_readfile();
void ldumpster_writefile();
void  doStoreOrRead();

void *Ldumpster_class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&Ldumpster_class,(method)ldumpster_new,(method)ldumpster_free,(short)sizeof(struct ldumpster),0L,A_GIMME,0);
	addint((method)ldumpster_setConstant);
	addfloat((method)ldumpster_floatConstant);
	addmess((method)ldumpster_listout, "peek",A_GIMME,0);
	addmess((method)ldumpster_Lists, "poke",A_GIMME,0);
	addmess((method)ldumpster_read, "read",A_DEFSYM,0);
	addmess((method)ldumpster_write, "write",A_GIMME,0);
	addmess((method)ldumpster_assist, "assist",A_CANT,0);
	addmess((method)ldumpster_clear, "clear",0);
	addmess((method)ldumpster_ready, "ready",0);
	addmess((method)ldumpster_done, "done",0);
	addmess((method)ldumpster_dump, "dump",A_GIMME,0);
	addmess((method)ldumpster_continue, "continue",0);
	//finder_addclass("Data","Ldumpster");
	

	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void ldumpster_listout(x,s,argc,argv)
Ldumpster *x;
Symbol *s;
int argc;
Atom *argv;

{
	float *floatArray;  // data is accessed as Array[x->index]
	long *longArray;
	short *shortArray;
	short temp;
	long i;
	void *voidPtr;		// voidPtr is always needed to equate bigArray to x->aHandle
	
	
	if(argc > 1)  						// length argument to peek command
		x->outsize = argv[1].a_w.w_long;
		else
		x->outsize = 1;
	if(argc > 0)						// index argument to peek command
		x->index = argv[0].a_w.w_long;
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
				else   			// odd adress
					{
					x->outList[i].a_w.w_long = 0L | ((temp >> 8) & 0xFF);
					}
				break;
				
				case TEXT:
				temp = shortArray[(x->index + i)/2];
				if((x->index + i) % 2  == 1) // odd address 
					{
					x->outList[i].a_w.w_long = 0L | (temp & 0xFF);
					}
				else   			// even adress, take
					{
					x->outList[i].a_w.w_long = 0L | ((temp >> 8) & 0xFF);
					}
				
				break;
				}
				
				
			
			}
			
		outlet_list(x->out,0L,(int)x->outsize,x->outList);
		
		x->index += x->outsize;
		if(x->index > x->ArraySize) x->index = 0;
	}
	else
	{
	post("Ldumpster: index value out of range");
	x->index = 0;
	}

	HUnlock(x->aHandle);
	
}

void ldumpster_read(x,s)  // file operations must be deferred
Ldumpster *x;
Symbol *s;

{

	
	defer_low(x,(method)ldumpster_readfile,s,0,NULL);
	
}

void ldumpster_write(x,s,argc,argv)  // file operations must be deferred
Ldumpster *x;
Symbol *s;
int argc;
Atom *argv;

{
	
	
	defer_low(x,(method)ldumpster_writefile,s,argc, argv);
	
}

void ldumpster_readfile(x,s,argc,argv)  // User Interface lifted from coll
Ldumpster *x;
Symbol *s;
int argc;
Atom *argv;
{

	long types[2];
	
	t_filehandle theFileHandle;
	
	long i;
	long type;
	long typefound;
	char name[256];
	//Str255 filename;
	float *bigArray;
	void *voidPtr;
	
	long incount;
	
	
	name[0] = 0;
	types[0]='maxb';

	if (s != gensym(""))   // "" is an empty symbol
	{
		i = 0;
		while(s->s_name[i])
				{
				name[i] = s->s_name[i];
				++i;
				}
			name[i] = 0;  // name is now a cstring
		
		if (locatefile_extended (name, &x->pathID, &typefound, types, 1)) 
			{
			ouchstring("Couldn't find %s. Please locate the file, or an alternate.",
				s->s_name);
			x->fn=gensym("");
			s = gensym("");
			}
		else
			{
			x->fn = s;
			}
	}
	
	if(s == gensym(""))
	{
		if (open_dialog(name, &x->pathID, &type, types, 1)) 
		{
		
		return;
		}
		else
			x->fn = gensym(name);
			
	}
	post("Ldumpster: reading %s",x->fn->s_name);

	if(path_opensysfile(name,x->pathID,&theFileHandle,READ_PERM))
		{
		ouchstring("Error Opening %s",x->fn->s_name);
		
		return;
		}
	
	critical_enter(0);  //Stop interrupts
	HLock(x->aHandle);          // and lock memory
	voidPtr = *x->aHandle;		// fool the compiler
	bigArray = voidPtr;
	incount = x->realArraySize;  // read no more than incount bytes
	sysfile_read(theFileHandle,&incount,bigArray);
	sysfile_close(theFileHandle);
	HUnlock(x->aHandle);
	critical_exit(0);
	switch(x->theType)
	{ 
	case FLOATS:
	x->dumplength = incount/sizeof(float);
	post("Ldumpster: %ld floats read",x->dumplength); // incount is now number of bytes read
	break;
	
	case INTS:
	x->dumplength = incount/sizeof(long);
	post("Ldumpster: %ld ints read",x->dumplength); // incount is now number of bytes read
	break;
	
	case BYTES:
	x->dumplength =incount;
	post("Ldumpster: %ld bytes read",x->dumplength); // incount is now number of bytes read
	break;
	
	case TEXT:
	x->dumplength =incount;
	post("Ldumpster: %ld bytes read",x->dumplength); // incount is now number of bytes read
	break;
	}
	outlet_int(x->outR,(long)x->dumplength);
	
}


void ldumpster_writefile(x,dummy,argc,argv)
Ldumpster *x;
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
	float *bigArray;
	void *voidPtr;
	long outcount;
	long result;
	long writesize;
	
	name[0] = 0;
	type ='maxb';
	creator = '????';
	writesize = x->dumplength;
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
		
	critical_enter(0);  // turn off interrupts
	HLock(x->aHandle);			// and lock memory
	voidPtr = *x->aHandle;
	bigArray = voidPtr;
	switch(x->theType)
	{
		case BYTES: case TEXT:
		if(writesize % 2 == 0)
		outcount = (writesize / 2) * sizeof(short);
		else
		outcount = (writesize / 2 +1)*sizeof(short);
		break;
		
		case FLOATS:
		outcount = writesize*sizeof(float);
		break;
		
		case INTS:
		outcount = writesize*sizeof(long);
		break;
	}
	
	result = path_createsysfile(name,x->pathID,type,&theFileHandle);
	if(result & (result != dupFNErr))  // does not warn if specified file exists
		{
		post("Could not create %s! error#  %ld",x->fn->s_name,result);
		goto exitHere;  				// these error handlers are not tested
		}
	post("Ldumpster: writing %s",x->fn->s_name);
	
	
	result = sysfile_write(theFileHandle,&outcount,bigArray);
	if(result)  
		{
		post("Could not write to %s! error#  %ld",x->fn->s_name,result);
		}
//CloseIt:
	result = sysfile_close(theFileHandle);
	if(result)  
		{
		post("Could not close %s! error#  %ld",x->fn->s_name,result);
		goto exitHere;
		}
	switch(x->theType)
	{
	case FLOATS:
	post("Ldumpster: %ld floats saved",outcount/sizeof(float));
	break;
	
	case INTS:
	post("Ldumpster: %ld ints saved",outcount/sizeof(long));
	break;
	
	case BYTES: case TEXT:
	post("Ldumpster: %ld bytes saved",outcount);
	break;
	}
exitHere:	
	HUnlock(x->aHandle);
	critical_exit(0);
	
	
}




void ldumpster_Lists(x,s,argc,argv)  // handles list input
Ldumpster *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;	
	
	//converted to poke
	
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	{// list in left inlet is index, value
		if(argv[0].a_type == A_LONG)
			{
			for(i = 1; i < argc; ++ i)
				{
				switch(argv[i].a_type)
					{ 
					case A_LONG:
					
					x->bvalue = (short) argv[i].a_w.w_long;
					
					break;
					
					case A_FLOAT:
					x->bvalue = (short) argv[i].a_w.w_float;
					break;
					
					default:
					post("Invalid List Member in Ldumpster");
					}
				x->storeflag = 1;  
				x->listflag = 0;
				doStoreOrRead(x,argv[0].a_w.w_long + i -1);
				}
			}
		else
			{
				
				return;
			}
			
		x->storeflag = 0;  // list in left overrides any impending stores
		x->listflag = 0;
		
		}
	//x->inletNumber = 0;
	
	
}

void  ldumpster_clear(x)
Ldumpster *x;
{
	long i;
	float *floatArray;
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
	x->dumplength = 0;
	outlet_int(x->outR,0L);
	
}

void ldumpster_ready(x)
Ldumpster *x;
{
	
	ldumpster_clear(x);
	x->waitingForDump = 1;
	x->nextslot =0;
	

}


void  ldumpster_done(x)
Ldumpster *x;
{
	
	x->waitingForDump = 0;
	x->dumplength = x->nextslot;
	outlet_int(x->outR,(long)x->dumplength);
	

}

void  ldumpster_dump(x,s,argc,argv)
Ldumpster *x;
Symbol *s;
int argc;
Atom *argv;
{
	int start, end;
	short temp;
	long i;
	void *voidPtr;
	short *shortArray;
	float *floatArray;  // data is accessed as Array[x->index]
	long *longArray;
	
	if(argc == 0){
		if(x->dumplength)
		{
			x->nextout = 0;
			ldumpster_continue(x);
		}
	}
	else
	{
		
		HLock(x->aHandle);
		voidPtr = *x->aHandle;
		shortArray = voidPtr;
		floatArray = voidPtr;
		longArray = voidPtr;
		temp = 0;
		start = 0;
		end = x->dumplength;
			
		if(argv[0].a_type == A_LONG) 
			start = argv[0].a_w.w_long;
		if(argc > 1 && argv[1].a_type == A_LONG)
			end = argv[1].a_w.w_long;
		//post(" start %ld  end %ld ",start,end);
		for(i = start; i <= end; ++i){
			switch(x->theType)
			{
				case FLOATS:
				outlet_float(x->out,floatArray[ x->nextout]); 
				break;
				
				case INTS:
				outlet_int(x->out,longArray[ x->nextout]); 
				break;
				
				case BYTES:
				if((i/2) * 2  == i) // even address
					{
					temp = shortArray[i/2] & 0xFF;
					
					}
				else			// odd address
					{
					temp = (shortArray[i/2] >> 8) & 0xFF;
					}
				outlet_int(x->out,(long)temp);
				break;
				
				case TEXT:
				if((i/2) * 2  == i) // even address
					{
					
					temp = (shortArray[i/2] >> 8) & 0xFF;
					}
				else			// odd address
					{
					temp = shortArray[i/2] & 0xFF;
					}
				outlet_int(x->out,(long)temp);
			}
			
			
			
			
			
			
			
			/*if((i/2) * 2  == i) // even address
					{
					temp = shortArray[i/2] & 0xFF;
					}
				else			// odd address
					{
					temp = (shortArray[i/2] >> 8) & 0xFF;
					}
				outlet_int(x->out,(long)temp);*/
		
		
		}
		HUnlock(x->aHandle);
	}

}

void  ldumpster_continue(x)
Ldumpster *x;
{
	float *floatArray;
	long *longArray;
	short *shortArray;
	short temp;
	long n;
	void *voidPtr;
	
	
	HLock(x->aHandle);
	voidPtr = *x->aHandle;
	floatArray = voidPtr;
	longArray = voidPtr;
	shortArray = voidPtr;
	temp = 0;
	
	while(temp != 0xf7)
	{
		if(x->nextout >= x->dumplength)
			break;
			
			switch(x->theType)
			{
				case FLOATS:
				outlet_float(x->out,floatArray[ x->nextout]); 
				break;
				
				case INTS:
				outlet_int(x->out,longArray[ x->nextout]); 
				break;
				
				case BYTES:
				n = x->nextout;
				if((n/2) * 2  == x->nextout) // even address
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
				n = x->nextout;
				if((n/2) * 2  == x->nextout) // even address
					{
					
					temp = (shortArray[n/2] >> 8) & 0xFF;
					}
				else			// odd address
					{
					temp = shortArray[n/2] & 0xFF;
					}
				outlet_int(x->out,(long)temp);
			}
			++x->nextout;
		}
			
	HUnlock(x->aHandle);
	

}


void  ldumpster_setConstant(x,n)  // handle int input
Ldumpster *x;
long n;
{

	
	
	if( x->waitingForDump)	// input is a number to be stored
		{
		switch (x->theType)
			{
			case INTS:
			x->lvalue = n;  	
			break;
			
			case FLOATS:
			x->fvalue =(float) n;  	
			break;
			
			case BYTES: case TEXT:
			x->bvalue = n & 0xFF;  	
			break;
			}
		x->storeflag = 1;
		x->listflag = 0;
		if(x->waitingForDump)
			doStoreOrRead(x,x->nextslot++);
		}
	
}

void  ldumpster_floatConstant(x,n)  //handle float inputs
Ldumpster *x;
float n;
{
	
	
	
	if( x->waitingForDump)	// n is value to be stored
		{
		switch (x->theType)
			{
			case INTS:
			x->lvalue =(long) n;  	
			break;
			
			case FLOATS:
			x->fvalue = n;  	
			break;
			
			case BYTES: case TEXT:
			x->bvalue = (short) n & 0xFF;  	
			break;
			}
		x->storeflag = 1;
		x->listflag = 0;
		if(x->waitingForDump)
			doStoreOrRead(x,x->nextslot++);
		}
	
	
}

void  doStoreOrRead(x,n)  // handle left input
Ldumpster *x;
long n;
{
	
	float *floatArray;
	long *longArray;
	UInt16 *shortArray;
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
							temp += x->bvalue;
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
							temp += x->bvalue;
							shortArray[n/2] = temp;
							}
						else			// even address
							{
							temp = shortArray[n/2] & 0x00FF;
							temp += x->bvalue << 8;
							shortArray[n/2] = temp;
							
							}
						break;
						
						}
					//post("store %ld  %ld",(long)temp,x->bvalue);
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
						if(n%2  == n) // even address
							{
							temp = shortArray[n/2] & 0xFF;
							
							}
						else			// odd address
							{
							temp = (shortArray[n/2] >> 8) & 0xFF;
							}
						outlet_int(x->outR,(long)n);
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
						outlet_int(x->outR,(long)n);
						outlet_int(x->out,(long)temp);
						break;
						
						}
					
			
			}
	}
	else
	post("Ldumpster: index value out of range");
	
	HUnlock(x->aHandle);
	
}



void ldumpster_assist(x,b,msg,arg,dst)
Ldumpster *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data and commands");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"data peeked or dumped");
			break;
			
			default:
			strcpy(dst,"size of data block");
			break;
		}
	}
	
}


void  *ldumpster_new(s,argc,theArgs)
Symbol *s;
int argc;
Atom *theArgs;
{

	Ldumpster  *x;
	float *floatArray;
	long *longArray;
	short *shortArray;
	void *voidPtr;
	long i;
	
	
	x = (Ldumpster *)newobject(Ldumpster_class);				// make an object
	//x->proxyR = proxy_new(x,2L,&x->inletNumber);  // Patch out R inlet for Ldumpster
	x->outR= outlet_new(x,0L);
	x->out= outlet_new(x,0L);
	x->pathID= defvolume();							// initialize 
	x->fn = gensym("untitled");
	x->index = 0;
	x->nextslot = 0;
	x->nextout = 0;
	x->waitingForDump = 0;
	x->dumplength = 0;
	x->storeflag = 0;
	x->listflag = 0;
	x->outsize = 1;
	x->theType = TEXT;
	if(argc== 0)						// set arraySize to default or arg
		x->ArraySize = MAXSIZE;
	else				
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
		x->realArraySize = x->ArraySize * sizeof(float);  
		break;
		
		case INTS:
		x->realArraySize = x->ArraySize * sizeof(long); 
		break;
		
		case BYTES: case TEXT:
		 
		break;
	}
	x->realArraySize =( x->ArraySize/2 +1) * sizeof(short);
	x->aHandle = NewHandle(x->realArraySize);

	if(x->aHandle == NIL)
		{
		ouchstring("The amount of memory requested by Ldumpster is not available");
		if(maxversion() >= 0x300)
		post("Ldumpster %ld Failed: Allocate more memory to Max",x->ArraySize); // seems to be a problem
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

void ldumpster_free(x)
Ldumpster *x;
{
	
	DisposeHandle(x->aHandle);
	//freeobject(x->proxyR); Proxy removed from Ldumpster
}

