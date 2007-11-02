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
	void *p_out2;
	void *p_out1;		
	void *p_out0;
	
} t_p5glove;

/* global pointer that holds the class definition */
void *p5glove_class;

/* prototypes for methods */

void p5glove_bang(t_p5glove *x);
void *p5glove_new(t_symbol *s, short argc, t_atom *argv);
void p5glove_disconnect(t_p5glove *x);
void p5glove_connect(t_p5glove *x);


/* initialization routine */

int main(void)
{
	/* define class. */
	setup((t_messlist **)&p5glove_class, (method)p5glove_new, (method)0L, (short)sizeof(t_p5glove), 0L, A_GIMME,0);
	
	/* bind methods to symbols */
	addbang((method)p5glove_bang);
	addmess((method)p5glove_connect, "connect", 0);
	addmess((method)p5glove_disconnect, "disconnect", 0);
	
	post("p5glove object by Michael F. Zbyszynski, 2.xi.07",0);
	
	/* list object in the new object list */
	finder_addclass("Controllers","p5glove");
	
	
	
	return 0;
}
//--------------------------------------------------------------------------

void p5glove_bang(t_p5glove *x)
{
	int visibleCount = 0;
	int gloveX = 0;
	int gloveY = 0;
	int gloveZ = 0;
	int i = 0;
	
	if(theGlove != NULL)
	{
		//  Sample the glove
		if(p5glove_sample(theGlove, &theData)==0)
		{
			p5glove_process_sample(theGlove, &theData);
	
			// Output buttons
	
			t_atom Buttonlist[3]; 
			int theButtons[3];
			theButtons[0] = (theData.buttons & 1);
			theButtons[1] = ((theData.buttons&2)>>1);
			theButtons[2] = ((theData.buttons&4)>>2);
			for (i=0; i < 3; i++) 
			{ 
				SETLONG(Buttonlist+i,theButtons[i]); 
			} 
			outlet_list(x->p_out1,0L,3,&Buttonlist);
	
			// Output fingers
	
			t_atom Fingerlist[5];
			for (i=0; i < 5; i++)
			{
				SETLONG(Fingerlist+i,theData.finger[i]);
			}
			outlet_list(x->p_out0,0L,5,&Fingerlist);
							
			// Establish which LEDs are visible
			for(i=0;i<8;i++)
			{
				if(theData.ir[i].visible)
				{
					visibleLEDs[visibleCount] = i;
					visibleCount++;
					gloveX += theData.ir[i].x;
					gloveY += theData.ir[i].y;
					gloveZ += theData.ir[i].z;
				}
			}
	
			// Simple Averaging of visible leds gives a centroid of sorts
				
			if(visibleCount>0)
			{
				// If there are visisble LEDs,  average LEDS 
				gloveX = gloveX/visibleCount;
				gloveY = gloveY/visibleCount;
				gloveZ = gloveZ/visibleCount;
		
				// Output X Y Z
				t_atom posList[3];
				int glovePos[3];
				glovePos[0] = gloveX;
				glovePos[1] = gloveY;
				glovePos[2] = gloveZ;
				for (i=0; i < 3; i++)
				{
					SETLONG(posList+i,glovePos[i]);
				}
				outlet_list(x->p_out2,0L,3,&posList);
			}	
		}
	}
	else {post("No glove, no love. :-(");}
}			
		

//--------------------------------------------------------------------------

void p5glove_connect(t_p5glove *x)
{
	// Open the Glove

	theGlove = p5glove_open();
	
	if(theGlove != NULL)
		post("Glove Connect Succeeded",theGlove);
	
	else
	{
		post("Glove Connect Failed");
	}
}


void p5glove_disconnect(t_p5glove *x)
{
	p5glove_close(theGlove);
	theGlove = NULL;
	post("Glove Disconnected");
}

void p5glove_free(t_p5glove *x)
{
	p5glove_close(theGlove);
	theGlove = NULL;
	post("Glove Disconnected");
}

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
	
	x->p_out2 = listout(x); // create outlets
	x->p_out1 = listout(x);
	x->p_out0 = listout(x);



	return (x);						
}

	
	

	