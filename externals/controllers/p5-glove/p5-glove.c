#include "ext.h"
#include <stdio.h>
#include <math.h>
#include "p5glove.h"




//#include <iostream>
#include <string.h>

//#include "OscOutboundPacketStream.h"

//#include "NetworkingUtils.h"

//#include "UdpSocket.h"
//#include "IpEndpointName.h"

#define IP_MTU_SIZE 1536

P5Glove theGlove;
//struct p5glove_data theData;
int visibleLEDs[8];


/* structure definition of object */
typedef struct p5_glove
{
	t_object l_ob;
	void *p_out3;
	void *p_out2;
	void *p_out1;		
	void *p_out0;
	
} t_p5_glove;

/* global pointer that holds the class definition */
void *p5_glove_class;

/* prototypes for methods */

void p5_glove_bang(t_p5_glove *x);
void *p5_glove_new(t_symbol *s, short argc, t_atom *argv);
void p5_glove_disconnect(t_p5_glove *x);
void p5_glove_connect(t_p5_glove *x);


/* initialization routine */

int main(void)
{
	/* define class. */
	setup((t_messlist **)&p5_glove_class, (method)p5_glove_new, (method)0L, (short)sizeof(t_p5_glove), 0L, A_GIMME,0);
	
	/* bind methods to symbols */
	addbang((method)p5_glove_bang);
	addmess((method)p5_glove_connect, "connect", 0);
	addmess((method)p5_glove_disconnect, "disconnect", 0);
	
	post("p5_glove object version 0.2 by Michael F. Zbyszynski, 5.xi.07",0);
	
	/* list object in the new object list */
	finder_addclass("Controllers","p5_glove");
	
	
	
	return 0;
}
//--------------------------------------------------------------------------

void p5_glove_bang(t_p5_glove *x)
{
	int visibleCount = 0;
	int gloveX = 0;
	int gloveY = 0;
	int gloveZ = 0;
	int i = 0;
	
	if(theGlove != NULL)
	{
		//  Sample the glove
		
		int err;
		err=p5glove_sample(theGlove, -1);
		if (err < 0 && errno == EAGAIN)
			return;
		if (err < 0) 
		{
			post("Glove Failure");
				return;
		}
			
		//p5glove_process_sample(theGlove, &theData);
	
		// Output buttons
		uint32_t buttons;
		p5glove_get_buttons(theGlove,&buttons);
	
		t_atom Buttonlist[3]; 
		int theButtons[3];
		theButtons[0] = (buttons & P5GLOVE_BUTTON_A);
		theButtons[1] = (buttons & P5GLOVE_BUTTON_B);
		theButtons[2] = (buttons & P5GLOVE_BUTTON_C);
		for (i=0; i < 3; i++) 
		{ 
			SETLONG(Buttonlist+i,theButtons[i]); 
		} 
		outlet_list(x->p_out1,0L,3,&Buttonlist);
			
		// Output fingers
		double clench;
		t_atom Fingerlist[5];
		for (i=0; i < 5; i++)
		{
			p5glove_get_finger(theGlove,i,&clench);
			SETFLOAT(Fingerlist+i,clench);
		}
		outlet_list(x->p_out0,0L,5,&Fingerlist);
						
		// Output X Y Z
		double pos[3];
		p5glove_get_position(theGlove, pos);
		t_atom posList[3];
		for (i=0; i < 3; i++)
		{
			SETFLOAT(posList+i,pos[i]);
		}
		outlet_list(x->p_out2,0L,3,&posList);
		
		//Output axis andangle
		double axis[3],angle;
		p5glove_get_rotation(theGlove, &angle, axis);
		t_atom tiltList[4];
		for (i=0; i < 3; i++)
		{
			SETFLOAT(tiltList+i, axis[i]);
		}
		i = 3;
		SETFLOAT(tiltList+i, angle);
		outlet_list(x->p_out3,0L,4,&tiltList);
		
		
	}	
	else 
	{
		post("No glove, no love. :-(");
	}
	
}			
		

//--------------------------------------------------------------------------

void p5_glove_connect(t_p5_glove *x)
{
	// Open the Glove

	theGlove = p5glove_open(0);
	
	if(theGlove != NULL)
		post("Glove Connect Succeeded",theGlove);
	
	else
	{
		post("Glove Connect Failed");
	}
}


void p5_glove_disconnect(t_p5_glove *x)
{
	p5glove_close(theGlove);
	theGlove = NULL;
	post("Glove Disconnected");
}

void p5_glove_free(t_p5_glove *x)
{
	p5glove_close(theGlove);
	theGlove = NULL;
	post("Glove Disconnected");
}

void *p5_glove_new(t_symbol *s, short argc, t_atom *argv)
{
	t_p5_glove *x;
	x = (t_p5_glove *)newobject(p5_glove_class);		// get memory for a new object & initialize 
	
	// Open the Glove
	
	theGlove = p5glove_open(0);
	
	if(theGlove != NULL)
		post("\n Glove Connect Succeeded\n");
	else
	{
		post("Glove Connect Failed");
	}
	
	x->p_out3 = listout(x); // create outlets
	x->p_out2 = listout(x);
	x->p_out1 = listout(x);
	x->p_out0 = listout(x);



	return (x);						
}

	
	

	