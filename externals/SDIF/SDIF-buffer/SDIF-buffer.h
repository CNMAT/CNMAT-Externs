/*
Copyright (c) 1999, 2004.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley. Maintenance by Ben "Jacobs".

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

*/

/* SDIF-buffer.h
   Structure definition of an MSP SDIF buffer
 */


#include "sdif.h"
#include "sdif-mem.h"
#include "sdif-buf.h"

/* An "SDIF Frame Lookup Function" looks for a frame at or near a given time.
   If there is a frame at the exact requested time, then of course that frame
   is returned.  If direction < 0, it "searches backwards", returning
   the frame with the highest time <= to the given time (i.e., the frame just 
   before the requested time.)  If direction > 0 it "searches forwards".
   Note that the SDIF_Frame type is actually a doubly linked list node (defined in
   sdif.h), so you can traverse the list of frames in the stream like that if after
   you call the SDIF Frame Lookup Function once.  
   Returns 0 if it doesn't find a frame. */
typedef SDIFmem_Frame (SDIFFrameLookupFn)(struct _SDIFbuffer *buf, 
									    sdif_float64 time,
									    long direction);

/* An "SDIF Frame Insert Function" inserts a new frame into an SDIF-buffer.  It
   automatically updates the doubly linked list.  
   Returns 0 for success, nonzero for failure.
   */
typedef int (SDIFFrameInsertFn)(SDIFmem_Frame f, struct _SDIFbuffer *buf);


/* An "SDIF Frame Delete Function" deletes the given frame from the given SDIF-buffer,
   automatically updating the linked list connections as well as any internal state,
   and freeing the memory taken by that frame. 
   NOTE: this function has never actually been implemented (0.8.0) */
typedef void (SDIFFrameDeleteFn)(SDIFmem_Frame f, struct _SDIFbuffer *buf);

/* An "SDIF Buffer Accessor Function" returns the SDIFbuf_Buffer instance which
   contains the SDIF data for a given max object instance. This provides the
   recipient full access to frame data through the API defined in sdif-buf.h,
   essentially replacing SDIFFrameLookupFn, SDIFFrameInsertFn, SDIFFrameDeleteFn.
   Returns 0 only if the SDIFbuf_Buffer wasn't successfully initialized during
   the max new object constructor (unlikely). */
typedef SDIFbuf_Buffer (SDIFBufferAccessorFn)(struct _SDIFbuffer *buf);


typedef struct _SDIFbuffer {
 	t_object s_obj;
 	t_symbol *s_myname;
 	void *internal;			/* Users of SDIF-buffers don't get to use this */
 	SDIFFrameLookupFn *FrameLookup;
 	SDIFFrameInsertFn *FrameInsert;
 	SDIFFrameDeleteFn *FrameDelete;  //  NOTE: not implemented (0.8.0)
 	
 	// Info about the stream as a whole
 	char *fileName;
 	sdif_int32   streamID;

  //  direct access to buffer data though API defined in sdif-buf.h
 	SDIFBufferAccessorFn *BufferAccessor;

  //  only add new stuff at bottom, so we don't have to recompile existing client objects
} SDIFBuffer;


/* The "SDIF buffer lookup function" turns a max symbol into a pointer to an 
   SDIFBuffer.  A pointer to this function will be in the s_thing field of the
   symbol "##SDIF-buffer-lookup". */
   
typedef SDIFBuffer *(*SDIFBufferLookupFunction)(t_symbol *name);
