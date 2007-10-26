#include "ext.h"
#include <stdio.h>
#include <math.h>
#include "p5glover.h"


//#include <iostream>
#include <string.h>

//#include "OscOutboundPacketStream.h"

//#include "NetworkingUtils.h"

//#include "UdpSocket.h"
//#include "IpEndpointName.h"

#define IP_MTU_SIZE 1536

P5Glove theGlove;
struct p5glove_data theData;
int visibleLEDs[8];


/* structure definition of object */
typedef struct p5glove
{
	t_object l_ob;		
	double r_x;					// stored values
	double r_y;
	double r_z;
	double r_h;
	double r_a;
	
	
	void *p_out;				// outlets
					
} t_p5glove;

/* global pointer that holds the class definition */
void *p5glove_class;

/* prototypes for methods */

void p5glove_bang(t_p5glove *x);
void *p5glove_new(t_symbol *s, short argc, t_atom *argv);

/* initialization routine */

int main(void)
{
	/* define class. */
	setup((t_messlist **)&p5glove_class, (method)p5glove_new, (method)0L, (short)sizeof(t_p5glove), 0L, A_GIMME,0);
	
	/* bind methods to symbols */
	addbang((method)p5glove_bang);
	
	post("p5glove object by Michael F. Zbyszynski, 25.x.07",0);
	
	/* list object in the new object list */
	finder_addclass("Controllers","p5glove");
	
	
	
	return 0;
}
//--------------------------------------------------------------------------

void p5glove_bang(t_p5glove *x)
{
	int visibleCount = 0;
	int gloveX = 0;
	int y = 0;
	int z = 0;
	int i = 0;
	
	//  Sample the glove
	p5glove_sample(theGlove, &theData);
	p5glove_process_sample(theGlove, &theData);
	
	// Output buttons and fingers
	
	int outvalue = 0;
	outvalue = (theData.buttons & 1);
	post("/p5glove/button/1 %ld", outvalue, 0);
	outlet_list(x->p_out,0L,1,outvalue);
	
	
	// Establish which LEDs are visible
	for(i=0;i<8;i++)
	{
		if(theData.ir[i].visible)
		{
			visibleLEDs[visibleCount] = i;
			visibleCount++;
			
			gloveX += theData.ir[i].x;
			y += theData.ir[i].y;
			z += theData.ir[i].z;
			
		}
		
	}
	
	// Simple Averaging of visible leds gives a centroid of sorts
				
	if(visibleCount>0)
	{
		// If there are visisble LEDs,  average LEDS 
		gloveX = gloveX/visibleCount;
		y = y/visibleCount;
		z = z/visibleCount;
		
		// Output X Y Z
		
	}	
					
}			
		

//--------------------------------------------------------------------------


//--------------------------------------------------------------------------

/* instance creation routine */

void *p5glove_new(t_symbol *s, short argc, t_atom *argv)
{
	t_p5glove *x;
	
	x = (t_p5glove *)newobject(p5glove_class);		// get memory for a new object & initialize 
	

	
	// Open the Glove
	
	theGlove = p5glove_open();
			
	if(theGlove != NULL)
		post("\n Glove Connect Succeeded\n");
	else
	{
		post("Glove Connect Failed");
	}
	  										
	floatin(x,1); // create inlet
	
	x->p_out = listout (x); // create outlet
	
	return (x);									// return newly created object to caller
}

	
	

	