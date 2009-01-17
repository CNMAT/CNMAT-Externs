/*
  Copyright (c) 1996-2008.  The Regents of the University of California (Regents).
  All Rights Reserved.

  Permission to use, copy, modify, and distribute this software and its
  documentation for educational, research, and not-for-profit purposes, without
  fee and without a signed licensing agreement, is hereby granted, provided that
  the above copyright notice, this paragraph and the following two paragraphs
  appear in all copies, modifications, and distributions.  Contact The Office of
  Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
  CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

  Written by Matt Wright, John MacCallum, and Andy Schmeder, The Center for New Music and
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

#include "cmmjl_osc.h"
#include "cmmjl_osc_schedule.h"
#include "cmmjl_osc_timetag.h"
#include "cmmjl.h"
#include "timeval.h"

t_cmmjl_error cmmjl_osc_schedule_init(void *x, t_cmmjl_obj *o){
	t_cmmjl_osc_scheduler *osc = (t_cmmjl_osc_scheduler *)malloc(sizeof(t_cmmjl_osc_scheduler));
	osc->packets_max = CMMJL_OSC_SCHEDULE_DEFAULT_QUEUE_SIZE;
	osc->packet_size = CMMJL_OSC_SCHEDULE_DEFAULT_PACKET_SIZE;
	cmmjl_osc_timetag_float_to_ntp(0.003, &(osc->precision));
	cmmjl_osc_timetag_float_to_ntp(1000.0, &(osc->max_delay));
	osc->id = 0;
	osc->clock = clock_new(x, (method)cmmjl_osc_schedule_tick);
	critical_new(&(osc->lock));

	// packet data buffer
	osc->buf = (char *)malloc(osc->packets_max * osc->packet_size);

	// nodes
	heap_initialize(&(osc->q), osc->packets_max);

	// free slots
	osc->packets_free = (int *)malloc(sizeof(int) * osc->packets_max);
	int i;
	for(i = 0; i < osc->packets_max; i++){
		osc->packets_free[i] = 1;
	}
	osc->scheduler_cb = cmmjl_osc_schedule_def_cb;
	o->osc_scheduler = osc;
	return CMMJL_SUCCESS;
}

t_cmmjl_error cmmjl_osc_schedule(void *x, long length, long ptr){
	struct ntptime now;
	struct ntptime nowp1;
	char *data;
	int i;
	node n;
	float dt;
	node *p_n;

	t_cmmjl_obj *o = cmmjl_obj_get(x);
	t_cmmjl_osc_scheduler *osc = (t_cmmjl_osc_scheduler *)(o->osc_scheduler);

	if(length < 16){
		CMMJL_ERROR(x, CMMJL_OSC_EBADBNDL, cmmjl_strerror(CMMJL_OSC_EBADBNDL));
		return CMMJL_OSC_EBADBNDL;
	}
	n.length = length;
	data = (char *)ptr;

	// we should consider a callback here to pass the original args 
	// or something somewhere if one of these fails.
        // check for queue full condition
        if(osc->q.heap_size == osc->packets_max) {
		CMMJL_ERROR(x, CMMJL_OSC_EOVRFLW, "Queue overflow");
		return CMMJL_OSC_EOVRFLW;
        }
        
        // check for length condition
        if(n.length >= osc->packet_size) {
		CMMJL_ERROR(x, CMMJL_OSC_EOVRFLW, "packet length %d exceeds maximum", n.length);
            	return CMMJL_OSC_EOVRFLW;
        }
        
        // make sure its a bundle
        if(strcmp(data, "#bundle") != 0) {
		CMMJL_ERROR(x, CMMJL_OSC_EBADBNDL, "input is not a bundle");
		return CMMJL_OSC_EBADBNDL;
        }

        n.timestamp.sec = ntohl(*((unsigned long *)(data + 8)));
        n.timestamp.frac_sec = ntohl(*((unsigned long *)(data + 12)));
        n.timestamp.sign = 1;

	// timetag == immediate, so don't schedule--just call the parser.
	if(n.timestamp.sec == 0 && n.timestamp.frac_sec == 1){
		o->osc_parser(x, length, ptr, o->osc_parser_cb);
		return;
	}else{
		n.timestamp.type = TIME_STAMP;
	}

	// now
	cmmjl_osc_timetag_now_to_ntp(&now);

	// now plus scheduler precision interval -> nowp1
	cmmjl_osc_timetag_add(&now, &(osc->precision), &nowp1);

	switch(cmmjl_osc_timetag_cmp(&nowp1, &(n.timestamp))){
	case 0: // on time
		o->osc_parser(x, length, ptr, o->osc_parser_cb);
		return CMMJL_SUCCESS;
	case 1: // deadline miss or on time
		switch(cmmjl_osc_timetag_cmp(&now, &(n.timestamp))){
		case -1: // within scheduler boundry, on time
		case 0:
			o->osc_parser(x, length, ptr, o->osc_parser_cb);
			return CMMJL_SUCCESS;
		case 1: // deadline missed
			CMMJL_ERROR(x, CMMJL_OSC_ELATE, cmmjl_strerror(CMMJL_OSC_ELATE));
			return CMMJL_OSC_ELATE;
		}
	}

	// message is candidate for future scheduling.  check that delay is less than max_delay.
	cmmjl_osc_timetag_add(&now, &(osc->max_delay), &nowp1);

	if(cmmjl_osc_timetag_cmp(&nowp1, &(n.timestamp)) < 0){
		CMMJL_ERROR(x, CMMJL_OSC_EMAXDEL, strerror(CMMJL_OSC_EMAXDEL));
		return CMMJL_OSC_EMAXDEL;
	}

	critical_enter(osc->lock);

	while(osc->soft_lock == 1){
		critical_exit(osc->lock);
		critical_enter(osc->lock);
	}
	osc->soft_lock = 1;
	clock_unset(osc->clock);

	while(osc->packets_free[osc->id] != 1){
		osc->id = ((osc->id + 1) % osc->packets_max);
	}
	n.id = osc->id;
	osc->id++;

	// mark slot as busy
	osc->packets_free[n.id] = 0;

	// add to queue
	i = heap_insert(&(osc->q), n);

	// copy data
	memcpy(osc->buf + (osc->packet_size * n.id), data, n.length);

	// check for new scheduling target delay

	p_n = heap_max(&(osc->q));

	cmmjl_osc_timetag_now_to_ntp(&now);
	now.sign = -1;
	cmmjl_osc_timetag_add(&(p_n->timestamp), &now, &nowp1);
	dt = cmmjl_osc_timetag_ntp_to_float(&nowp1);
	if(dt < 0.001){
		osc->soft_lock = 0;
		cmmjl_osc_schedule_tick(x);
	}else{
		clock_fdelay(osc->clock, dt * 1000);
		osc->soft_lock = 0;
	}
}

void cmmjl_osc_schedule_tick(void *x){
	struct ntptime now;
	struct ntptime nowp1;
	double dt;
	node n;
	node_ptr p_n;
	int t;
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	t_cmmjl_osc_scheduler *osc = o->osc_scheduler;
	critical_enter(osc->lock);
	while(osc->soft_lock == 1){
		critical_exit(osc->lock);
		critical_enter(osc->lock);
	}
	clock_unset(osc->clock);
	osc->soft_lock = 1;
	p_n = heap_max(&(osc->q));
	cmmjl_osc_timetag_now_to_ntp(&now);
	cmmjl_osc_timetag_add(&now, &(osc->precision), &(nowp1));

	while(p_n != NULL){
		t = cmmjl_osc_timetag_cmp(&nowp1, &((*p_n).timestamp));
		if(t == 1 || t == 0){
			// dequeue the message
			n = heap_extract_max(&(osc->q));

			// dequeue and pass on the next scheduled packet
			// exit critical region just to be safe
			// soft lock is not released
			critical_exit(osc->lock);
			osc->scheduler_cb(x, n.length, 
					  (unsigned long int)((osc->buf + (osc->packet_size * n.id))));
			critical_enter(osc->lock);
			osc->packets_free[n.id] = 1;
		}else{
			break;
		}
		// get next node
		p_n = heap_max(&(osc->q));
	}
	critical_exit(osc->lock);
	if(p_n != NULL){
		cmmjl_osc_timetag_now_to_ntp(&now);
		now.sign = -1;
		cmmjl_osc_timetag_add(&(p_n->timestamp), &now, &nowp1);
		dt = cmmjl_osc_timetag_ntp_to_float(&nowp1);

		if(dt < 0.001){
			osc->soft_lock = 1;
			cmmjl_osc_schedule_tick(x); // re-enter on self
		}else{
			osc->soft_lock = 0;
			clock_fdelay(osc->clock, dt * 1000.);
		}
	}else{
		osc->soft_lock = 0;
	}
}

t_cmmjl_error cmmjl_osc_schedule_def_cb(void *x, long n, long ptr){
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return CMMJL_ENOOBJ;
	}
	cmmjl_osc_parse(x, n, (char *)ptr, true, o->osc_parser_cb);
	return CMMJL_SUCCESS;
}

bool cmmjl_osc_should_schedule(void *x){
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return false;
	}
	return (bool)(o->osc_should_schedule);
}

void cmmjl_osc_should_schedule_set(void *x, bool b){
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return;
	}
	o->osc_should_schedule = b;
}
