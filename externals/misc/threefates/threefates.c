/*
Copyright (c) 2000-2004.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Tim Madden and Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.
*/




/**************************************************************************************
 * Deal with track birth/death so sinusoids~ works.
 *
 * How it works:
 * 		Take input list of (index,[other parameters]) tuples.
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
Version 0.2: 041229 MW: Handles arbitrary-length partial parameter lists 
Version 0.2.1: New version/makefile system
*/

#include "VERSION.h"

#include "ext.h"

// Define the max number of parameters per oscillator (not including index).
#define MAX_PARTIAL_PARAMS 5


// We consider each duple sent to sinusoids~ to be a "slot". Each slot
// goes to an oscillator. We dole out slots to tracks from an SDIF file.
// Here is a "slot."
typedef struct t_slot_
{
	// Parameters other than index: frequency, amplitude, bandwidth, etc.
	// The second one (i.e., params[1]) must be amplitude
	float params[MAX_PARTIAL_PARAMS];
#define AMPLITUDE params[1]

	// Track index of this sine from SDIF data.
	int index;
	// A 1 or 0, used for finding track deaths. Set to 1 (assume death) before getting 
	// all the SDIF freq data. If this index is in the SDIF data, we are alive
	// so we set to 0. If all SDIF data is processed, and we are still at 1, we died,
	// so set amp to zero, release this slot.
	Boolean is_dead;
} t_slot;


// Structure for this type of object.
typedef struct t_threefates_
{
	struct object t_obj;
	
	// Maximum number of oscillators (determines memory allocation)
	int max_osc;
	
	// How many parameters besides index (e.g., amp, freq, bw) does each partial have?
	int num_partial_parameters;
	
	// Parameters dervied from those two:
	int max_inargs, max_outargs;
	
	void *t_out;
	Atom *output_list;
	int t_outsize;

	// Frame0 num args.
	int ac0;
	// Frame 0 Arguments - including partial indices
	float *frame0;

	// Frame 1 num args.
	int ac1;
	// Frame 1 Arguments - including partial indices
	float *frame1;
		
	// toggle which is present frame (0 or 1). the other is future. 
	int toggle_frame_present;
	
	// List of slots.
	t_slot *t_slotlist;
	// Number of used slots.
	int num_slots;
	// Max index in used slot list.
	int max_slot_index;
} t_threefates;

Symbol *ps_list;

void *threefates_class;

/*
 * Function Prototypes.
 */

void *threefates_new(long maxpartials, long nPartialParams);
void threefates_free(t_threefates *x);

void List(
	t_threefates *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

void SlotsToOutput(t_threefates *thisobject);
void RemoveDeadSlots(t_threefates *thisobject);
void AddToSlotList(t_threefates *thisobject, int index, int nparams, float *params);
	
int GetFreeSlot(t_threefates *thisobject);

int IsInSlotList(
	t_threefates *thisobject,
	int index);
	
void SetDeadZero(t_threefates *thisobject);
	
void InitializeSlots(t_threefates *thisobject);


void BirthSlotList(t_threefates *thisobject, int index, int nparams, float *params);

int InputToFutureFrame(
	t_threefates *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);
	
void SetFutureValue(
	t_threefates *x,
	int index,
	float value);
	
float GetFutureValue(
	t_threefates *x,
	int index);

void SetFutureAC(
	t_threefates *x,
	int ac);

int GetFutureAC(t_threefates *x);

void SetPresentValue(
	t_threefates *x,
	int index,
	float value);
	
float GetPresentValue(
	t_threefates *x,
	int index);

void SetPresentAC(
	t_threefates *x,
	int ac);

int GetPresentAC(t_threefates *x);

void CopyFutureToPresent(t_threefates *x);

void PresentFrameToSlots(t_threefates *x);

void FutureFrameToSlots(t_threefates *x);

void threefates_tellmeeverything(t_threefates *x);

/**************************************************************************************
 *
 *************************************************************************************/


void main(fptr *f)
{
	
	post("threefates " VERSION " - Tim Madden and Matt Wright");
	post("Copyright ©2000-04 Regents of the University of California.");
	
	/* tell Max about my class. The cast to short is important for 68K */
	setup((t_messlist **)&threefates_class, (method) threefates_new, (method) threefates_free,
			(short)sizeof(t_threefates), 0L, A_DEFLONG, A_DEFLONG, 0);
			
			
	// post("size %ld", sizeof(t_threefates));
	addmess((method)List, "list", A_GIMME, 0);
	addmess((method)threefates_tellmeeverything, "tellmeeverything", 0);
	
	ps_list = gensym("list");


}


/**************************************************************************************
 *
 *************************************************************************************/

void *threefates_new(long maxpartials, long nPartialParams) {
	int i;

	t_threefates *x;
	
	// post("threefates_new(%ld, %ld)", maxpartials, nPartialParams);
	
	
	if (maxpartials == 0) {
		// Default 256 partials for no good reason
		maxpartials = 256;
	}
	
	if (nPartialParams == 0) {
		/* Default two parameters: freq, amplitude */
		nPartialParams = 2;
	}
	if (nPartialParams < 2 || nPartialParams > MAX_PARTIAL_PARAMS) {
		error("threefates: bad second argument %ld (partials must have 2 to %ld parameters)",
			  nPartialParams, MAX_PARTIAL_PARAMS);
		return 0;
	}
	
	x = (t_threefates*)newobject(threefates_class);
	x->num_partial_parameters = nPartialParams;
	x->max_osc = maxpartials;
	
	x->t_out = listout(x);

	// Frame0 num args.
	x->ac0 = 0;

	// Frame 1 num args.
	x->ac1 = 0;
		
	// toggle which is present frame (0 or 1). the other is future. 
	x->toggle_frame_present = 0;
	
	x->max_inargs =  (x->num_partial_parameters + 1) * x->max_osc;
	x->max_outargs = x->num_partial_parameters * x->max_osc;

	x->output_list = (Atom *)   getbytes((short) x->max_outargs * sizeof(Atom));
	x->frame0 =      (float *)  getbytes((short) x->max_inargs * sizeof(float));
	x->frame1 =      (float *)  getbytes((short) x->max_inargs * sizeof(float));
	x->t_slotlist =  (t_slot *) getbytes((short) x->max_osc * sizeof(t_slot));
	
	
	InitializeSlots(x);
	
	// post("max inargs %ld, max_outargs %ld", x->max_inargs, x->max_outargs);
	
	return(x);
}

void threefates_free(t_threefates *x) {
	freebytes(x->output_list, (short) x->max_outargs * sizeof(Atom));
	freebytes(x->frame0,      (short) x->max_inargs * sizeof(float));
	freebytes(x->frame1,      (short) x->max_inargs * sizeof(float));
	freebytes(x->t_slotlist,  (short) x->max_osc * sizeof(t_slot));
}
	

/**************************************************************************************
 *
 *************************************************************************************/

void List(
	t_threefates *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{

	if (argc > x->max_inargs) {
		error("threefates: input list too big (size %ld) --- dropping", argc);
		return;
	}
	
	if (argc %  (1+ x->num_partial_parameters) != 0) {
		error("threefates: input list length not a multiple of %ld - dropping", 1+x->num_partial_parameters);
		return;
	}

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

		// Set list length
		x->t_outsize = x->num_partial_parameters * x->max_slot_index;
		
		// send to sinusoids~
		outlet_list(x->t_out, ps_list, x->t_outsize, x->output_list);
	
		// Take dead tracks out of the picture.
		RemoveDeadSlots(x);
		
		// Shift Register.
		CopyFutureToPresent(x);
	}
	else
	{
		error("threefates: InputToFutureFrame returned a bad value.");
	}
}


/***************************************************************************************
 *
 **************************************************************************************/

void InitializeSlots(t_threefates *x)
{
	int i, j;
	
	for (i = 0; i < x->max_osc; i++) {
		for (j=0; j<MAX_PARTIAL_PARAMS; ++j) {
			x->t_slotlist[i].params[j] = 0.0;
		}
		x->t_slotlist[i].index = -1;
		x->t_slotlist[i].is_dead = 0;		
	}
	
	x->num_slots = 0;
	x->max_slot_index = 0;
	

}


/***************************************************************************************
 *
 **************************************************************************************/

void SetDeadZero(t_threefates *thisobject)
{
	int i;
	
	for (i = 0; i < thisobject->max_slot_index; i++)
	{
		thisobject->t_slotlist[i].AMPLITUDE = 0.0;
		thisobject->t_slotlist[i].is_dead = 1;		
	}
}


/***************************************************************************************
 *
 **************************************************************************************/

int IsInSlotList(
	t_threefates *thisobject,
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

int GetFreeSlot(t_threefates *x)
{
	int i;
	
	for (i = 0; i < x->max_osc; i++)
	{
		if (x->t_slotlist[i].index == -1)
		{
			x->num_slots++;
			
			if (i == x->max_slot_index)
				x->max_slot_index++;
				
			return i;
		}
	}
	
	return -1;
}

/***************************************************************************************
 *
 **************************************************************************************/

void AddToSlotList(t_threefates *thisobject, int index, int nparams, float *params) {
	int slotindex, i;
	
	slotindex = IsInSlotList(thisobject, index);
	
	if (slotindex  == -1) {
		slotindex = GetFreeSlot(thisobject);
	}
	
	if (slotindex >= 0) {
		for (i = 0; i<nparams; ++i) {
			thisobject->t_slotlist[slotindex].params[i] = params[i];
		}
		thisobject->t_slotlist[slotindex].index = index;
		thisobject->t_slotlist[slotindex].is_dead = 0;
		
		//post("** AddToSlotList: index %ld to slotindex %ld, param[0] = %f, param[1] = %f", index, slotindex, params[0], params[1]);
	}
	else
	{
		//post("track-assign: OUT OF OSC");	
	}	
}

/***************************************************************************************
 *
 **************************************************************************************/

void BirthSlotList(t_threefates *thisobject, int index, int nparams, float *params){
	int slotindex, i;
	
	slotindex = IsInSlotList(thisobject, index);
	
	// post("* BirthSlotList index %ld, param[0] %f, param[1] %f, slot index %ld", index, params[0], params[1], slotindex);
	
	if (slotindex  == -1)
	{
		// We have a totally new track.
		slotindex = GetFreeSlot(thisobject);
	
		if (slotindex >= 0)
		{
			for (i = 0; i<nparams; ++i) {
				thisobject->t_slotlist[slotindex].params[i] = params[i];
			}
			thisobject->t_slotlist[slotindex].AMPLITUDE = 0.0;
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
			for (i = 0; i<nparams; ++i) {
				thisobject->t_slotlist[slotindex].params[i] = params[i];
			}
			thisobject->t_slotlist[slotindex].AMPLITUDE = 0.0;
			thisobject->t_slotlist[slotindex].index = index;
			thisobject->t_slotlist[slotindex].is_dead = 0;		
		}
	}
}

/***************************************************************************************
 *
 **************************************************************************************/

void RemoveDeadSlots(t_threefates *thisobject)
{

	int i,j;
	
	for (i = 0; i < thisobject->max_slot_index; i++)
	{
		if (thisobject->t_slotlist[i].is_dead == 1)
		{
			for (j=0; j<MAX_PARTIAL_PARAMS; ++j) {
				thisobject->t_slotlist[i].params[j] = 0.0;
			}
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

void SlotsToOutput(t_threefates *thisobject) {
	int i, j, outindex;
	int nparams = thisobject->num_partial_parameters;

    outindex = 0;
	for (i = 0; i < thisobject->max_slot_index; i++) {
	    // Output everything except index
	    for (j = 0; j<nparams; ++j) {
			SETFLOAT(thisobject->output_list + outindex, thisobject->t_slotlist[i].params[j]);
			++outindex;
		}
	}
}



/***************************************************************************************
 *
 **************************************************************************************/

int InputToFutureFrame(
	t_threefates *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	int i;
	float value;	

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
	}
		
		
	return 0;		
}

/***************************************************************************************
 *
 **************************************************************************************/

void SetFutureValue(
	t_threefates *x, 
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
	t_threefates *x, 
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
	t_threefates *x, 
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

int GetFutureAC(t_threefates *x)
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
	t_threefates *x,
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
	t_threefates *x,
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
	t_threefates *x,
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

int GetPresentAC(t_threefates *x)
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

void CopyFutureToPresent(t_threefates *x)
{
	// post("* CopyFutureToPresent");
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

void PresentFrameToSlots(t_threefates *x) {
	int i, j;
	int index;
	float params[MAX_PARTIAL_PARAMS];
	int nparams = x->num_partial_parameters;
	int npartials = GetPresentAC(x) / (1+nparams);
			
	
	// Put args into storage.
	for (i = 0; i < npartials; i++) {
		// First of each group is index
		index = (int)(GetPresentValue(x, i*(nparams+1)));
		
		// Rest of each group is partial parameters
		for (j = 0; j < nparams; ++j) {
	   		params[j] = GetPresentValue(x, i*(nparams+1) + j + 1);
	   		//post("** PresentFrameToSlots param[%ld] = %f", j, params[j]);
	   	}
			
		
		// Add to slots. (These tracks are marked as "alive")
		AddToSlotList(x, index, nparams, params);				
	}
}

/***************************************************************************************
 *
 **************************************************************************************/

void FutureFrameToSlots(t_threefates *x) {
	int i,j;
	int index;
	float params[MAX_PARTIAL_PARAMS];
	int nparams = x->num_partial_parameters;
	int npartials = GetFutureAC(x) / (1+nparams);

				
	// Put args into storage.
	for (i = 0; i < npartials; i++) {
		// First of each group is index
		index = (int)(GetFutureValue(x, i*(nparams+1)));
		
		// Rest of each group is partial parameters
		for (j = 0; j < nparams; ++j) {
	   		params[j] = GetFutureValue(x, i*(nparams+1) + j + 1);
	   	}
		
		// Add to slots. (These tracks are marked as "alive")
		BirthSlotList(x, index, nparams, params);
	}
}

void threefates_tellmeeverything(t_threefates *x) {
   int i, j;
   Atom a;
   
   post("--- threefates version " VERSION " ---");
   post(" up to %ld oscillators, %ld params each (so input list length <= %ld, output list length <= %ld)",
        x->max_osc, x->num_partial_parameters, x->max_inargs, x->max_outargs);
   post(" Present frame has %ld values:", GetPresentAC(x));
   for (i = 0; i < GetPresentAC(x); i+= x->num_partial_parameters+1) {
   	  post("    ");
   	  for (j = 0; j < x->num_partial_parameters+1; ++j) {
   	  	SETFLOAT(&a, GetPresentValue(x, i+j));
   	  	postatom(&a);
   	  }
   }
   post(" Future frame has %ld values:", GetFutureAC(x));
   for (i = 0; i < GetFutureAC(x); i+= x->num_partial_parameters+1) {
   	  post("    ");
   	  for (j = 0; j < x->num_partial_parameters+1; ++j) {
   	  	SETFLOAT(&a, GetFutureValue(x, i+j));
   	  	postatom(&a);
   	  }
   }
   post(" Slot list has %ld elements, max index %ld", x->num_slots, x->max_slot_index);
   for (i = 0; i<x->max_slot_index; i ++) {
   		post("  index %ld %s ", x->t_slotlist[i].index, x->t_slotlist[i].is_dead ? "DEAD" : "ALIVE");
   		for (j = 0; j<x->num_partial_parameters; ++j) {
   			SETFLOAT(&a, x->t_slotlist[i].params[j]);
   			postatom(&a);
   	  }
   }
}
