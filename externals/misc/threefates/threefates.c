/**************************************************************************************
 * Deal with track birth/death so sinusoids~ works.
 *
 * How it works:
 * 		Take input list of (index,freq,amp) triplets.
 *
 *		Use "slots" which are position in the list to sinusoids~.
 *
 *		Assume all tracks died, set slots to "dead." and zero amplitude.
 *
 *		Search input list, put into the slots, matching by the track index.
 *		Set that slot to "alive."
 *		(If new track, get a fresh slot)
 *
 *		Output slots to sinusoids~.
 *
 *		Free up dead slots (all slots not set to "alive"). these tracks really died.
 *		 		
 *************************************************************************************/
/*
 * Includes.
 */
 
#include <SetUpA4.h>
#include <A4Stuff.h>

#include <Navigation.h>
#include <FSp_fopen.h>
#include "ext.h"
/* Undo ext.h's macro versions of some of stdio.h: */
#undef fopen
#undef fclose
#undef fprintf
#undef fscanf
#undef fseek
#undef sprintf
#undef sscanf

#include <stdio.h>


// Define the max number of args.
#define MAX_INARGS 768
#define MAX_OUTARGS 512
#define MAX_OSC 256


// We consider each duple sent to sinusoids~ to be a "slot". Each slot
// goes to an oscillator. We dole out slots to tracks from an SDIF file.
// Here is a "slot."
typedef struct t_slot_
{
	// the frequency of sin wave.
	float freq;
	// amplitude of sine.
	float amp;
	// Track index of this sine from SDIF data.
	int index;
	// A 1 or 0, used for finding track deaths. Set to 1 (assume death) before getting 
	// all the SDIF freq data. If this index is in the SDIF data, we are alive
	// so we set to 0. If all SDIF data is processed, and we are still at 1, we died,
	// so set amp to zero, release this slot.
	int is_dead;
} t_slot;


// Structure for this type of object.
typedef struct t_trackdeath_
{
	struct object t_obj;
	void *t_out;
	// Output list.	
	Atom t_list_out[MAX_OUTARGS];
	int t_outsize;

	// Frame0 num args.
	int ac0;
	// Frame 0 Arguments.
	float frame0[MAX_INARGS];

	// Frame 1 num args.
	int ac1;
	// Frame 1 Arguments.
	float frame1[MAX_INARGS];
		
	// toggle which is present frame (0 or 1). the other is future. 
	int toggle_frame_present;
	
	// List of slots.
	t_slot t_slotlist[MAX_OSC];
	// Number of used slots.
	int num_slots;
	// Max index in used slot list.
	int max_slot_index;
} t_trackdeath;

Symbol *ps_list;

void *trackdeath_class;

/*
 * Function Prototypes.
 */

// Make new object.
void *trackdeath_new(
	Symbol s, 
	short ac,
	Atom *av);


// Set up the grid.
void List(
	t_trackdeath *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

void SlotsToOutput(t_trackdeath *thisobject);
void RemoveDeadSlots(t_trackdeath *thisobject);
void AddToSlotList(
	t_trackdeath *thisobject,
	int index,
	float freq,
	float amp);
	
int GetFreeSlot(t_trackdeath *thisobject);

int IsInSlotList(
	t_trackdeath *thisobject,
	int index);
	
void SetDeadZero(t_trackdeath *thisobject);
	
void InitializeSlots(t_trackdeath *thisobject);


void BirthSlotList(
	t_trackdeath *thisobject,
	int index,
	float freq,
	float amp);

int InputToFutureFrame(
	t_trackdeath *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);
	
void SetFutureValue(
	t_trackdeath *x,
	int index,
	float value);
	
float GetFutureValue(
	t_trackdeath *x,
	int index);

void SetFutureAC(
	t_trackdeath *x,
	int ac);

int GetFutureAC(t_trackdeath *x);

void SetPresentValue(
	t_trackdeath *x,
	int index,
	float value);
	
float GetPresentValue(
	t_trackdeath *x,
	int index);

void SetPresentAC(
	t_trackdeath *x,
	int ac);

int GetPresentAC(t_trackdeath *x);

void CopyFutureToPresent(t_trackdeath *x);

void PresentFrameToSlots(t_trackdeath *x);

void FutureFrameToSlots(t_trackdeath *x);



/**************************************************************************************
 *
 *************************************************************************************/


void main(fptr *f)
{
	
	post("Running track-assigner main");
	
	/* tell Max about my class. The cast to short is important for 68K */
	setup(&trackdeath_class, trackdeath_new, 0,
			(short)sizeof(t_trackdeath), 0L, 0);
			
			
	addmess((method)List, "list", A_GIMME, 0);
	
	ps_list = gensym("list");


}


/**************************************************************************************
 *
 *************************************************************************************/

void *trackdeath_new(
	Symbol s, 
	short ac,
	Atom *av)
{
	int i;

	t_trackdeath *x;
	
	EnterCallback();
	post("Running track-assigner.new");

	x = (t_trackdeath*)newobject(trackdeath_class);
	
	x->t_out = listout(x);

	// Frame0 num args.
	x->ac0 = 0;

	// Frame 1 num args.
	x->ac1 = 0;
		
	// toggle which is present frame (0 or 1). the other is future. 
	x->toggle_frame_present = 0;
	
	InitializeSlots(x);

	
	ExitCallback();
	
	return(x);
}


/**************************************************************************************
 *
 *************************************************************************************/

void List(
	t_trackdeath *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	EnterCallback();

	// We get FUTURE frame from arguments.
	if (InputToFutureFrame(x, mess, argc, argv) == 0)
	{
		// Assume all tracks are dead. 
		SetDeadZero(x);
		
		// Get args to slots.
		PresentFrameToSlots(x);

		// Get ready for births. Set new freq with zero amp.
		FutureFrameToSlots(x);

		// Make output list.
		SlotsToOutput(x);

		// Set list length. (2 args per slot <amp,freq> pairs).
		x->t_outsize = 2 * x->max_slot_index;
		
		// send to sinusoids~
		outlet_list(x->t_out, ps_list, x->t_outsize, x->t_list_out);
	
		// Take dead tracks out of the picture.
		RemoveDeadSlots(x);
		
		// Shift Register.
		CopyFutureToPresent(x);
	}
	else
	{
		post("¥ Assign-tracks: Need multiple of 3 length input list.");
	}
	 
	ExitCallback();
}


/***************************************************************************************
 *
 **************************************************************************************/

void InitializeSlots(t_trackdeath *thisobject)
{
	int i;
	
	for (i = 0; i < MAX_OSC; i++)
	{
		thisobject->t_slotlist[i].freq = 0.0;
		thisobject->t_slotlist[i].amp = 0.0;
		thisobject->t_slotlist[i].index = -1;
		thisobject->t_slotlist[i].is_dead = 0;		
	}
	
	thisobject->num_slots = 0;
	thisobject->max_slot_index = 0;
	

}


/***************************************************************************************
 *
 **************************************************************************************/

void SetDeadZero(t_trackdeath *thisobject)
{
	int i;
	
	for (i = 0; i < thisobject->max_slot_index; i++)
	{
		thisobject->t_slotlist[i].amp = 0.0;
		thisobject->t_slotlist[i].is_dead = 1;		
	}
}


/***************************************************************************************
 *
 **************************************************************************************/

int IsInSlotList(
	t_trackdeath *thisobject,
	int index)
{
	int i;
	
	for (i = 0; i < thisobject->max_slot_index; i ++)
	{
		if (thisobject->t_slotlist[i].index == index)
		{
			return i;
		}
	}

	return -1;
}

/***************************************************************************************
 *
 **************************************************************************************/

int GetFreeSlot(t_trackdeath *thisobject)
{
	int i;
	
	for (i = 0; i < MAX_OSC; i++)
	{
		if (thisobject->t_slotlist[i].index == -1)
		{
			thisobject->num_slots++;
			
			if (i == thisobject->max_slot_index)
				thisobject->max_slot_index++;
				
			return i;
		}
	}
	
	return -1;
}

/***************************************************************************************
 *
 **************************************************************************************/

void AddToSlotList(
	t_trackdeath *thisobject,
	int index,
	float freq,
	float amp)
{
	int slotindex;
	
	slotindex = IsInSlotList(thisobject, index);
	
	if (slotindex  == -1)
	{
		slotindex = GetFreeSlot(thisobject);
	}
	
	if (slotindex >= 0)
	{
		thisobject->t_slotlist[slotindex].freq = freq;
		thisobject->t_slotlist[slotindex].amp = amp;
		thisobject->t_slotlist[slotindex].index = index;
		thisobject->t_slotlist[slotindex].is_dead = 0;	
	}
	else
	{
		//post("track-assign: OUT OF OSC");	
	}	
}

/***************************************************************************************
 *
 **************************************************************************************/

void BirthSlotList(
	t_trackdeath *thisobject,
	int index,
	float freq,
	float amp)
{
	int slotindex;
	
	slotindex = IsInSlotList(thisobject, index);
	
	if (slotindex  == -1)
	{
		// We have a totally new track.
		slotindex = GetFreeSlot(thisobject);
	
		if (slotindex >= 0)
		{
			thisobject->t_slotlist[slotindex].freq = freq;
			thisobject->t_slotlist[slotindex].amp = 0.0;
			thisobject->t_slotlist[slotindex].index = index;
			thisobject->t_slotlist[slotindex].is_dead = 0;	
		}	
		else
		{
			//post("track-assign: OUT OF OSC");
		}
	}
	else
	{
		// Track in list, but dead for 1 frame, and comming to life again. 
		if (thisobject->t_slotlist[slotindex].is_dead == 1)
		{
			thisobject->t_slotlist[slotindex].freq = freq;
			thisobject->t_slotlist[slotindex].amp = 0.0;
			thisobject->t_slotlist[slotindex].index = index;
			thisobject->t_slotlist[slotindex].is_dead = 0;		
		}
	}
}

/***************************************************************************************
 *
 **************************************************************************************/

void RemoveDeadSlots(t_trackdeath *thisobject)
{

	int i;
	
	for (i = 0; i < thisobject->max_slot_index; i++)
	{
		if (thisobject->t_slotlist[i].is_dead == 1)
		{
			thisobject->t_slotlist[i].freq = 0.0;
			thisobject->t_slotlist[i].amp = 0.0;
			thisobject->t_slotlist[i].index = -1;
			thisobject->t_slotlist[i].is_dead = 0;
			
			thisobject->num_slots+= -1;		
			
			if (i == (thisobject->max_slot_index - 1))
			{
				thisobject->max_slot_index+= -1;
			}
		}
	}
}

/***************************************************************************************
 *
 **************************************************************************************/

void SlotsToOutput(t_trackdeath *thisobject)
{
	int i;
	int counter0 = 0;
	int counter1 = 1;
	
	for (i = 0; i < thisobject->max_slot_index; i++)
	{			
		thisobject->t_list_out[counter0].a_type = A_FLOAT;
		thisobject->t_list_out[counter1].a_type = A_FLOAT;
		
		thisobject->t_list_out[counter0].a_w.w_float = 
			thisobject->t_slotlist[i].freq;
		thisobject->t_list_out[counter1].a_w.w_float = 
			thisobject->t_slotlist[i].amp;
			
		counter0 += 2;
		counter1 += 2;
			
	}
}



/***************************************************************************************
 *
 **************************************************************************************/

int InputToFutureFrame(
	t_trackdeath *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	int i;
	float value;	
	

	if ((argc % 3) == 0)
	{

		SetFutureAC(x, argc);

		// Put args into storage.
		for (i = 0; i < argc; i++)
		{	
			switch (argv[i].a_type)
			{
				case A_FLOAT:
					value = argv[i].a_w.w_float;
					break;
				case A_LONG:
					value = (float)(argv[i].a_w.w_long);
					break;
			}
		
			SetFutureValue(x, i, value);
		}// for ...
	}//if ((argc % 3) == 0)
	else
		return -1;
		
	return 0;		
}

/***************************************************************************************
 *
 **************************************************************************************/

void SetFutureValue(
	t_trackdeath *x, 
	int i,
	float value)
{
	if (x->toggle_frame_present == 0)
	{
		x->frame1[i] = value;
	}
	else
	{
		x->frame0[i] = value;
	}		
}

/***************************************************************************************
 *
 **************************************************************************************/

float GetFutureValue(
	t_trackdeath *x, 
	int i)
{
	if (x->toggle_frame_present == 0)
	{
		return x->frame1[i];
	}
	
	return x->frame0[i];
}

/***************************************************************************************
 *
 **************************************************************************************/

void SetFutureAC(
	t_trackdeath *x, 
	int ac)
{
	if (x->toggle_frame_present == 0)
	{
		x->ac1 = ac;
	}
	else
	{
		x->ac0 = ac;
	}
}

/***************************************************************************************
 *
 **************************************************************************************/

int GetFutureAC(t_trackdeath *x)
{
	if (x->toggle_frame_present == 0)
	{
		return x->ac1;
	}
	
	return x->ac0;
}

/***************************************************************************************
 *
 **************************************************************************************/

void SetPresentValue(
	t_trackdeath *x,
	int i,
	float value)
{
	if (x->toggle_frame_present == 1)
	{
		x->frame1[i] = value;
	}
	else
	{
		x->frame0[i] = value;
	}		
}

/***************************************************************************************
 *
 **************************************************************************************/

float GetPresentValue(
	t_trackdeath *x,
	int i)
{
	if (x->toggle_frame_present == 1)
	{
		return x->frame1[i];
	}
	
	return x->frame0[i];
}

/***************************************************************************************
 *
 **************************************************************************************/

void SetPresentAC(
	t_trackdeath *x,
	int ac)
{
	if (x->toggle_frame_present == 1)
	{
		x->ac1 = ac;
	}
	else
	{
		x->ac0 = ac;
	}
}

/***************************************************************************************
 *
 **************************************************************************************/

int GetPresentAC(t_trackdeath *x)
{
	if (x->toggle_frame_present == 1)
	{
		return x->ac1;
	}
	
	return x->ac0;
}


/***************************************************************************************
 *
 **************************************************************************************/

void CopyFutureToPresent(t_trackdeath *x)
{
	switch (x->toggle_frame_present)
	{
		case 0:
			x->toggle_frame_present = 1;
			break;
		case 1:
			x->toggle_frame_present = 0;
			break;
	}			
}


/***************************************************************************************
 *
 **************************************************************************************/

void PresentFrameToSlots(t_trackdeath *x)
{
	int counter0, counter1, counter2;
	int i;
	int theend;
	int index;
	float freq, amp;
	
	// Set counter for output array.
	counter0 = 0;
	counter1 = 1;
	counter2 = 2;
	
	// count by threes.
	theend = GetPresentAC(x) / 3;
			
	
	// Put args into storage.
	for (i = 0; i < theend; i++)
	{
	
		index = (int)(GetPresentValue(x, counter0));
		freq = GetPresentValue(x, counter1);
		amp = GetPresentValue(x, counter2);
		
		// Add to slots. (These tracks are marked as "alive")
		AddToSlotList(x, index, freq, amp);
					
		// increment counter.
		counter0 += 3;
		counter1 += 3;
		counter2 += 3;
	}// for ...
	
}

/***************************************************************************************
 *
 **************************************************************************************/

void FutureFrameToSlots(t_trackdeath *x)
{
	int counter0, counter1, counter2;
	int i;
	int theend;
	int index;
	float freq, amp;
	
	// Set counter for output array.
	counter0 = 0;
	counter1 = 1;
	counter2 = 2;
	
	// count by threes.
	theend = GetFutureAC(x) / 3;
			
	// Put args into storage.
	for (i = 0; i < theend; i++)
	{
		index = (int)(GetFutureValue(x, counter0));
		freq = GetFutureValue(x, counter1);
		amp = GetFutureValue(x, counter2);
		
		// Add to slots. (These tracks are marked as "alive")
		BirthSlotList(x, index, freq, amp);
					
		// increment counter.
		counter0 += 3;
		counter1 += 3;
		counter2 += 3;
	}// for ...
}

