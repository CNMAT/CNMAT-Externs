/* Copyright (c) 2003.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
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

param-update.h
	API for geting atomic parameter updates for Max/MSP objects
	Matt Wright, 4/21/2003
		
*/

typedef void *parameters *;

typedef struct PacketBuffer_struct {
} *PacketBuffer;


/* Put one of these in your object */
typedef struct _paramPointers {
	void *params;
	void *oldparams;
	void *newparams;
	void *freeparams;
} paramPointers;


/* You must allocate memory for three blocks of parameters, and pass those
   three pointers to this procedure to initialize your paramPointer struct.
   The first parameter block should contain the initial values for your object;
   the other two should just have enough memory.  */
void InitParamPointers(paramPointers *pp, void *initialValues, void *blank1, void *blank2);


/* To compute parameter values, first "check out" a parameter block to write the new values
   into.  The object will continue to run with the previous values. */
void *CheckOutParamBlock(paramPointers *pp);


/* Once the new parameter values are ready to go, "check in" the new values so that they'll
   take effect on the next perform() */
void CheckInNewParams(paramPointers *pp);


/* In the perform() routine, call this to get the current parameter values */
void *GetCurrentParams(paramPointers *pp);

/* In the perform() routine, call this to get the previous parameter values
   (so you can interpolate from these old values to the new values).  If the parameters
   haven't changed since the last perform() routine, then this will return the same pointer
   that GetCurrentParams() returned. */
void *GetPreviousParams(paramPointers *pp);

/* At the end of the perform() routine, after the interpolation reaches the current
   value and there's no longer any need to remember the "previous" value, call this. */
void DoneWithPreviousParams(paramPointers *pp);
