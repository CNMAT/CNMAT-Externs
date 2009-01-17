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

#define CMMJL_OSC_SCHEDULE_DEFAULT_PACKET_SIZE 1000
#define CMMJL_OSC_SCHEDULE_DEFAULT_QUEUE_SIZE 1500

typedef struct _cmmjl_osc_scheduler{
	char *buf; /**< buffer for holding scheduled OSC data */
	int buf_size; /**< buffer size */
	t_clock *clock; /**< clock for OSC scheduling */
	t_critical lock; /**< lock */
	int soft_lock; /**< soft lock */
	struct ntptime precision; /**< scheduler precision */
	struct ntptime max_delay; /**< max delay permitted */
	binary_heap q; /**< binary heap */
	int packets_max; /**< max number of packets */
	int packet_size; /**< max size of an OSC packet */
	int *packets_free; /**< free packets */
	unsigned int id; /**< current spot in the buffer */
	/** The function which will be called when the scheduler executes. */
	t_cmmjl_error (*scheduler_cb)(void*,long,long);
} t_cmmjl_osc_scheduler;

/**	Initialize the data structure required for scheduling OSC messages
	@param	x	The object
	@returns	An error code or CMMJL_SUCCESS
*/
t_cmmjl_error cmmjl_osc_schedule_init(void *x, t_cmmjl_obj *o);

/**	Schedule an OSC message for execution in the future.  
	@param	x	The object
	@param	length	The length of the packet
	@param	ptr	A pointer to the data
	@returns	An error code if an error was encountered
*/
t_cmmjl_error cmmjl_osc_schedule(void *x, long length, long ptr);

void cmmjl_osc_schedule_tick(void *x);

t_cmmjl_error cmmjl_osc_schedule_def_cb(void *x, long n, long ptr);

bool cmmjl_osc_should_schedule(void *x);
void cmmjl_osc_should_schedule_set(void *x, bool b);

#endif // __CMMJL_OSC_SCHEDULE_H__
