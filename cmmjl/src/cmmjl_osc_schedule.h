/** 	@file cmmjl_osc_schedule.h 
	OSC scheduling
	@authors Andy Schmeder, John MacCallum
	@addtogroup 	OSC 
@{
*/
/*
Copyright (c) 2008.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Andy Schmeder and John MacCallum, The Center for New Music and 
Audio Technologies, University of California, Berkeley.

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

#ifndef __CMMJL_OSC_SCHEDULE_H__
#define __CMMJL_OSC_SCHEDULE_H__

#include "cmmjl.h"
#include "ext.h"
#include "ext_critical.h"
#include "pqops.h"

//#define CMMJL_OSC_SCHEDULER_ON 1
//#define CMMJL_OSC_SCHEDULER_OFF 0

/**	Default OSC packet size */
#define CMMJL_OSC_SCHEDULE_DEFAULT_PACKET_SIZE 1000
/** 	Default queue size */
#define CMMJL_OSC_SCHEDULE_DEFAULT_QUEUE_SIZE 1500

#define CMMJL_OSC_SCHEDULE_ONTIME 0 /**< OSC packet is on time */
#define CMMJL_OSC_SCHEDULE_LATE 1 /**< OSC packet is on late */
#define CMMJL_OSC_SCHEDULE_EARLY -1 /**< OSC packet is on early */

/**	A data structure to hold all data necessary for scheduling an 
	OSC packet for execution according to its timetag.
*/
typedef struct _cmmjl_osc_scheduler{
	char *buf; /**< buffer for holding scheduled OSC data */
	int buf_size; /**< buffer size */
	t_clock *clock; /**< clock for OSC scheduling */
	t_critical lock; /**< lock */
	int soft_lock; /**< soft lock */
	ntptime precision; /**< scheduler precision */
	ntptime max_delay; /**< max delay permitted */
	binary_heap q; /**< binary heap */
	int packets_max; /**< max number of packets */
	int packet_size; /**< max size of an OSC packet */
	int *packets_free; /**< free packets */
	unsigned int id; /**< current spot in the buffer */
	/** if this is non-zero, treat late packets as if they were on time */
	int late_packets_ok;
	/** The function which will be called when the scheduler executes. */
	t_cmmjl_error (*scheduler_cb)(void*,long,long);
} t_cmmjl_osc_scheduler;

/**	Initialize the data structure required for scheduling OSC messages
	@param	x	The object
	@param	o	The internal object
	@param	flags	A bitfield of flags--see cmmjl_flags
	@returns	An error code or CMMJL_SUCCESS
*/
t_cmmjl_error cmmjl_osc_schedule_init(void *x, t_cmmjl_obj *o, unsigned int flags);

/**	Schedule an OSC message for execution in the future.  
	@param	x	The object
	@param	length	The length of the packet
	@param	ptr	A pointer to the data
	@returns	An error code if an error was encountered
	@callgraph
*/
t_cmmjl_error cmmjl_osc_schedule(void *x, long length, long ptr);

/**	Schedule an OSC message.  This is called by cmmjl_osc_schedule() and 
	probably shouldn't be used by anything else.
	
	@param	x	The object
	@callgraph
*/
void cmmjl_osc_schedule_tick(void *x);

/** 	This is the default callback that cmmjl_osc_schedule() and cmmjl_osc_schedule_tick()
	use to send their data for execution.

	@param	x	Your object
	@param 	n 	The length of the OSC packet
	@param 	ptr	A pointer to an OSC packet.

	@returns	An error if one is encountered, or CMMJL_SUCCESS
*/
t_cmmjl_error cmmjl_osc_schedule_def_cb(void *x, long n, long ptr);

/**	Find out whether the OSC scheduler is on or off.
	
	@param  x	Your object
	@returns	True or false
*/
bool cmmjl_osc_should_schedule(void *x);

/**	Turn the OSC scheduler on or off.

	@param	x	Your object
	@param	b	True or false
*/
void cmmjl_osc_should_schedule_set(void *x, bool b);

/**	Free all resources used by the scheduler.  Don't call this
	directly--it's called by cmmjl_free()
	@param	o	The scheduler struct.
*/
void cmmjl_osc_schedule_free(t_cmmjl_osc_scheduler *o);

/**	Find out whether a packet is on time, late, or early.

	@param	x		Your object
	@param	timestamp	The timestamp to test

	@returns		CMMJL_OSC_SCHEDULE_ONTIME, CMMJL_OSC_SCHEDULE_LATE, or CMMJL_OSC_SCHEDULE_EARLY
*/
long cmmjl_osc_schedule_isontime(void *x, ntptime timestamp);

/**	Get the scheduler precision

	@param	x	Your object
	@returns	The scheduler precision
*/
ntptime cmmjl_osc_schedule_precision_get(void *x);

#endif // __CMMJL_OSC_SCHEDULE_H__

/**@}*/
