/*

Copyright (c) 2007.  The Regents of the University of California
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
University of California, Berkeley.  Based on sample code from David Zicarelli.

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

     Portions based on timemachine.c from softcast,
     by Adrian Freed and Matt Wright, Copyright (c) 1992-98.


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: timemachine~
DESCRIPTION: Generate "virtual time" signal with control of position, rate, etc.
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2007
SVN_REVISION: $LastChangedRevision$
VERSION 0.1: Matt's initial version 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/

#define DEBUG

#include "ext.h"
#include "version.h"
#include "version.c"

#include "z_dsp.h"
// #include <math.h>

void *timemachine_class;

typedef struct _timemachine
{
  /* Max stuff */
  t_pxobject x_obj;	//  header
  void *t_proxy[1];
  long t_inletNumber;

  float T;              // 1/fs

  /* Time machine stuff */
  double t;		/* current virtual time */
  double t_rate;	/* current virtual time rate of change */
  double t_min;        /* User-set minimum virtual time */
  double t_max;        /* User-set maximum virtual time */
  double t_min_float;  /* User-set minimum virtual time */
  double t_max_float;  /* User-set maximum virtual time */

  /* Stuff for target stuff */
  Boolean gotATarget;	/* Do we have a target we're trying to reach? */
  Boolean gottaMotor;	/* Do we need to hustle to make the target? */
  double lastUserSetRate;	/* Most recent rate the user asked for */
  double timeToTarget;	/* Time left until we need to be at the target */
  double targetVTime;	/* Virtual time that is the target */
  double maxAllowedSpeed; /* Max speed we can go at to reach target */
  double minAllowedSpeed; /* Min speed we can go at to reach target */

  double approachSlew;    /* When we approach, how far into the future do we aim? */
} t_timemachine;



int main(void);
void *timemachine_new(void);
void timemachine_free(t_timemachine *x);
void timemachine_dsp(t_timemachine *x, t_signal **sp, short *count);
t_int *timemachine_perform_signal_position_control(t_int *w);
t_int *timemachine_perform_signal_rate_control(t_int *w);
t_int *timemachine_perform_no_signals_in(t_int *w);
void proxified_float(t_timemachine *x, float f);
void set_rate(t_timemachine *x, float f);
void set_position(t_timemachine *x, float f);
void set_min_vtime(t_timemachine *x, float f);
void set_max_vtime(t_timemachine *x, float f);
void timemachine_approach(t_timemachine *x, float f);
void timemachine_approach_slew(t_timemachine *x, float f);
void timemachine_target(t_timemachine *x, float where, float when);
void timemachine_target_max_speed(t_timemachine *x, float f);
void timemachine_target_min_speed(t_timemachine *x, float f);
void timemachine_timed_goto(t_timemachine *x, float where, float when);

static void CheckConstraints(t_timemachine *x);


#define CLIP_BETWEEN(x, min, max) \
	(((x)<(min))?(min):(((x)>(max))?(max):(x)))

int main(void) {
  version(0);	
  setup((t_messlist **)&timemachine_class, (method)timemachine_new,
	(method)dsp_free, (short)sizeof(t_timemachine), 0L, 0);
  addmess((method)version, "version", 0);
  addmess((method)timemachine_dsp, "dsp", A_CANT, 0);
  addfloat((method)proxified_float);
  addmess((method)set_rate, "rate", A_FLOAT, 0);
  addmess((method)set_position, "position", A_FLOAT, 0);
  addmess((method)set_min_vtime, "min", A_FLOAT, 0);
  addmess((method)set_max_vtime, "max", A_FLOAT, 0);
  addmess ((method)timemachine_approach, "approach", A_FLOAT, 0);
  addmess ((method)timemachine_approach_slew, "approach_slew", A_FLOAT, 0);
  addmess ((method)timemachine_target, "target", A_FLOAT, A_FLOAT, 0);
  addmess ((method)timemachine_target_max_speed, "target_max_speed", A_FLOAT, 0);
  addmess ((method)timemachine_target_min_speed, "target_min_speed", A_FLOAT, 0);
  addmess ((method)timemachine_timed_goto, "timed_goto", A_FLOAT, A_FLOAT, 0);

  dsp_initclass();
}

void *timemachine_new(void) {
  t_timemachine *x;
  
  x = (t_timemachine *)newobject(timemachine_class);
	
  if (x==0) return 0;

  // This call to dsp_setup creates two proxy inlets that can take floats too:
  dsp_setup((t_pxobject *)x,2);  

  outlet_new((t_object *)x,"signal");

  // Defaults
  x->approachSlew = 0.05;     /* Default 50ms, like in softcast */
  x->t = 0.0;   	      /* start at the beginning */
  x->t_rate = 1.0;	      /* forward full ahead */	
  x->lastUserSetRate = 1.0f;
  x->t_min = DBL_MIN;         /* no minimum virtual time */
  x->t_max = DBL_MAX;         /* no maximum virtual time */
  x->gotATarget = FALSE;
  x->gottaMotor = FALSE;
  x->timeToTarget = 0;
  x->targetVTime = 0;
  x->maxAllowedSpeed = 2.0f;
  x->minAllowedSpeed = 0.5f;
  
  return x;   
}

	
/***************************************************
 Control event input part
***************************************************/

void proxified_float(t_timemachine *x, float f) {
	int inlet = proxy_getinlet((t_object *)x);
  switch (inlet) {
  case 0:
    set_rate(x,f);
    break;
  case 1:
    set_position(x,f);
    break;
  default:
    error("Float came in unrecognized inlet %ld", inlet);
  }
}

void set_rate(t_timemachine *x, float f) {
  double d = (double) f;

  // post("You passed in float %f for rate", f);
  
  x->lastUserSetRate = d;

  if (x->gotATarget && x->gottaMotor) {
    /* Sorry; can't pay any attention to user rate requests
       until we reach our target */
  } else {
    x->t_rate = d;
  }
}
	
void set_position(t_timemachine *x, float f) {
  //  post("You passed in float %f for position", f);
  double d = (double) f;

  x->t = CLIP_BETWEEN(d, x->t_min, x->t_max);
  CheckConstraints(x);
}

void set_min_vtime(t_timemachine *x, float f) {
  //  post("You passed in float %f for min_vtime", f);

  x->t_min_float = f;
  x->t_min = (double) f;
}

void set_max_vtime(t_timemachine *x, float f) {
  //  post("You passed in float %f for max_vtime", f);

  x->t_max_float = f;
  x->t_max = (double) f;
}

void timemachine_approach(t_timemachine *x, float f) {
  double d = (double) f;
  double slop = x->approachSlew;

  // post("You called timemachine_approach with arg %f", f);

  x->t_rate = (d - x->t + slop) /slop;
}

void timemachine_approach_slew(t_timemachine *x, float f) {
  double d = (double) f;
  // post("You called timemachine_approach_slew with arg %f", f);
}

void timemachine_target_max_speed(t_timemachine *x, float f) {
  double d = (double) f;
  // post("You called timemachine_target_max_speed with arg %f", f);

  x->maxAllowedSpeed = d;
  CheckConstraints(x);
}

void timemachine_target_min_speed(t_timemachine *x, float f) {
  double d = (double) f;
  // post("You called timemachine_target_min_speed with arg %f", f);

  x->minAllowedSpeed = d;
  CheckConstraints(x);
}

void timemachine_target(t_timemachine *x, float where_f, float when_f) {
  /* First arg desired vtime, second arg real-time to get there */
  /* If we're already aimed at a target, just forget the old goal
       and go for the new one. */

  double where = (double) where_f;
  double when = (double) when_f;
  // post("You called timemachine_target with args %f, %f", where, when);

  if (where < x->t_min || where > x->t_max) {
    error("target %d is outside allowable range %d to %d; ignoring.", 
	 where, x->t_min, x->t_max);
  } else {
    x->gotATarget = TRUE;
    x->targetVTime = where;
    x->timeToTarget = when;
    CheckConstraints(x);
  }
}

void timemachine_timed_goto(t_timemachine *x, float where_f, float when_f) {
  /* First arg desired vtime, second arg real-time to get there */
  double where = (double) where_f;
  double when = (double) when_f;
  // post("You called timemachine_timed_goto with args %f, %f", where_f, when_f);

  if (when_f <= 0.0f) {
    error("timed_goto: can't get there in %ld seconds (must be >0); ignoring", when_f);
    return;
  }

  if (where < x->t_min || where > x->t_max) {
    error("timed_goto location %d is outside allowable range %d to %d; ignoring.",
         where, x->t_min, x->t_max);
  } else {
    x->gotATarget = TRUE;
    x->gottaMotor = TRUE;
    x->targetVTime = where;
    x->timeToTarget = when;
    x->t_rate = (where - x->t) / when;
  }
}


/***************************************************
 timed goto (synchronization) stuff:
***************************************************/

static void CheckConstraints(t_timemachine *x) {
  double avgSpeed;	/* Speed we need to average to make our goal */

  if (x->gotATarget) {
    avgSpeed = (x->targetVTime - x->t) / x->timeToTarget;
    
#ifdef DEBUG
    post("timemachine~ is at %d and needs to be at %f in %f seconds; so avg speed %f."
	 x->t, x->targetVTime, x->timeToTarget, avgSpeed);
    post("  Current rate %f. (min %f, max %f)\n",
	 x->t_rate, x->minAllowedSpeed, x->maxAllowedSpeed);
#endif

    if (avgSpeed <= x->minAllowedSpeed || avgSpeed >= x->maxAllowedSpeed) {
      x->t_rate = avgSpeed;
#ifdef DEBUG2
      if (!x->gottaMotor) {
	printf("Now we gotta motor!  t %f, target %f, rate now %f\n",
	       x->t, x->targetVTime, x->t_rate);
      }
#endif
      x->gottaMotor = TRUE;
#ifdef DEBUG
      printf("... GOTTA MOTOR!\n");
#endif
    } else {
      /* TO-DO: compute a measure of how close we are to the extreme allowed
	 values and use that to scale incoming requests */
      x->gottaMotor = FALSE;
    }
  }
}




/***************************************************
 Signal processing part
***************************************************/


void timemachine_dsp(t_timemachine *x, t_signal **sp, short *count) {
  x->T = 1.0f / sp[0]->s_sr;

  if (count[1]) {
    // A signal is connected to the position control
    post("* You're controlling position with a signal (rate signal will be ignored)");
	// So no need to pass the rate signal input; it has no effect.
    dsp_add(timemachine_perform_signal_position_control, 4, x, sp[0]->s_n, 
	    sp[1]->s_vec, sp[2]->s_vec);
  } else if (count[0]) {
    post("* You're controlling rate with a signal");
    dsp_add(timemachine_perform_signal_rate_control, 4, x, sp[0]->s_n,
	    sp[0]->s_vec, sp[2]->s_vec);
  } else {
    post("* No signal inputs");
    dsp_add(timemachine_perform_no_signals_in, 3, x, sp[0]->s_n, sp[2]->s_vec);
  }
}

t_int *timemachine_perform_signal_position_control(t_int *w) {
    t_timemachine *x = (t_timemachine *)(w[1]);  // object
    int size = w[2]; // vector size
    t_float *position_in = (t_float *)(w[3]); 
    t_float *output = (t_float *)(w[4]);
    t_float val, min, max;

    min = x->t_min_float;
    max = x->t_max_float;

    // Trivial: user's position input is what we output (after clipping)
    // XXX actually, we could implement timed_goto and target for this case too...

    while (size--) {
      val = *position_in++;
      *output++ = CLIP_BETWEEN(val, min, max);
    }
    return (w+5);
}


t_int *timemachine_perform_signal_rate_control(t_int *w) {
    t_timemachine *x = (t_timemachine *)(w[1]);  // object
    int size = w[2]; // vector size
    t_float *rate_in = (t_float *)(w[3]); 
    t_float *output = (t_float *)(w[4]);

    double min = x->t_min;
    double max = x->t_max;
    double T = x->T;
    double t = x->t;
    double timeToTarget = x->timeToTarget;
    Boolean gotATarget = x->gotATarget;
    Boolean gottaMotor = x->gottaMotor;

    int i;
	
    for (i = 0; i < size; ++i) {
      t += T * rate_in[i];
      t = CLIP_BETWEEN(t, min, max);
      
#ifdef IMPLEMENTED_TARGET_FOR_SIGNAL_INPUT_CASE
      if (gotATarget) {
	timeToTarget -= T;
	if (timeToTarget <= 0) {
	  // NB: we could have checked outside the inner loop whether
	  // this would occur on this signal vector...

	  /* We made it to the target */
#ifdef DEBUG
	  post("Made the target!  timeToTarget %f, current time %f, rate %f\n",
	       x->timeToTarget, x->t, x->t_rate);
#endif
	  gotATarget = x->gotATarget = FALSE;
	  gottaMotor = x->gottaMotor = FALSE;
	  /* Reset rate to last value set by user */
	  rate = x->t_rate = x->lastUserSetRate;
	} else {
	  // Probably expensive to do this on each signal vector!
	  // CheckConstraints(x);
	}
      }
#endif

      output[i] = t;
    }
    
    if (x->gotATarget) {
      // Check the constraints once per signal vector.
      CheckConstraints(x);
    }

    // Remember state variables that changed
    x->t = t;
    x->timeToTarget = timeToTarget;


    return (w+5);
}

t_int *timemachine_perform_no_signals_in(t_int *w) {
    t_timemachine *x = (t_timemachine *)(w[1]);  // object
    int size = w[2]; // vector size
    t_float *output = (t_float *)(w[3]);
    double min = x->t_min;
    double max = x->t_max;
    double T = x->T;
    double t = x->t;
    double rate = x->t_rate;
    double timeToTarget = x->timeToTarget;
    Boolean gotATarget = x->gotATarget;
    Boolean gottaMotor = x->gottaMotor;

    int i;


	
    for (i = 0; i < size; ++i) {
      t += T * rate;
      t = CLIP_BETWEEN(t, min, max);
      
      if (gotATarget) {
	timeToTarget -= T;
	if (timeToTarget <= 0) {
	  // NB: we could have checked outside the inner loop whether
	  // this would occur on this signal vector...

	  /* We made it to the target */
#ifdef DEBUG
	  post("Made the target!  timeToTarget %f, current time %f, rate %f\n",
	       x->timeToTarget, x->t, x->t_rate);
#endif
	  gotATarget = x->gotATarget = FALSE;
	  gottaMotor = x->gottaMotor = FALSE;
	  /* Reset rate to last value set by user */
	  rate = x->t_rate = x->lastUserSetRate;
	} else {
	  // Probably expensive to do this on each signal vector!
	  // CheckConstraints(x);
	}
      }
      output[i] = t;
    }
    
    if (x->gotATarget) {
      // Check the constraints once per signal vector.
      CheckConstraints(x);
    }

    // Remember state variables that changed
    x->t = t;
    x->timeToTarget = timeToTarget;

    return (w+4);
}
