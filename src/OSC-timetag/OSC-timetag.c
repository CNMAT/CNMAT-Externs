
/*

Copyright (c) 2008
The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Andy Schmeder, The Center for New Music and Audio Technologies,
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

 OSC-timetag.c

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: OSC-timetag
DESCRIPTION: Generates, transforms and operates on high-resolution timestamps
AUTHORS: Andy Schmeder
COPYRIGHT_YEARS: 2008
SVN_REVISION: $LastChangedRevision: 1634 $
VERSION 0.1: First public release
VERSION 0.2: Added support for in-place operations on FullPackets
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/
#define NAME "OSC-timetag"
#define DESCRIPTION "Generates, transforms and operates on high-resolution timestamps"
#define AUTHORS "Andy Schmeder and John MacCallum"
#define COPYRIGHT_YEARS "2008,12,13-17"


// max object header
#include "ext.h"
#include "ext_obex.h"


// need a source of time
#include "OSC-timetag-ops.h"

// version info
#include "version.h"


#define OP_TAG 1
#define OP_ADD 2
#define OP_DST 3
#define OP_SUB 4
#define OP_MUL 5
#define OP_DER 6
#define OP_CMP 7

#define TO_T 1
#define TO_F 2
#define TO_I 3
#define TO_S 4
#define TO_A 5
#define TO_P 6

/* structure definition of your object */
typedef struct _OSCTimeTag
{
  t_object o_ob;    // required header

  t_object* in_p[1];  // either a normal inlet or a crazy proxy thing
  long in_i_unsafe;   // which inlet message arrived on, but isn't threadsafe

  t_object* out_p[1]; // outlet

  int op;
  int to;

  struct ntptime a;
  struct ntptime b; // second argument to the operator, if relevant, or the previous value in OP_DER

  float f; // scaling factor if in multiply mode

} OSCTimeTag;

/* global that holds the class definition */
t_class *OSCTimeTag_class;

t_symbol *ps_FullPacket;
t_symbol *ps_OSCTimeTag;
t_symbol *ps_iso8601;
t_symbol *ps_tai;

// basic prototypes
void* OSCTimeTag_new(t_symbol* s, short argc, t_atom* argv);
void OSCTimeTag_free(OSCTimeTag *x);
void OSCTimeTag_assist (OSCTimeTag *x, void *box, long msg, long arg, char *dstString);

// op callback
struct ntptime OSCTimeTag_run(OSCTimeTag *x);

// methods
void OSCTimeTag_now(OSCTimeTag *x);
void OSCTimeTag_int(OSCTimeTag *x, long i);
void OSCTimeTag_float(OSCTimeTag *x, double d);
void OSCTimeTag_immediate(OSCTimeTag *x);
void OSCTimeTag_FullPacket(OSCTimeTag *x, t_symbol* s, int argc, t_atom* argv);
void OSCTimeTag_OSCTimeTag(OSCTimeTag *x, t_symbol* s, int argc, t_atom* argv);
void OSCTimeTag_iso8601(OSCTimeTag *x, t_symbol* s, int argc, t_atom* argv);
//void OSCTimeTag_tai(OSCTimeTag *x, t_symbol* s, int argc, t_atom* argv);

// setup
int main(void)
{

  // post version
  version_post_copyright();

  OSCTimeTag_class = class_new("OSC-timetag", (method)OSCTimeTag_new, (method)OSCTimeTag_free, (short)sizeof(OSCTimeTag), 0L, A_GIMME, 0);

  // unix timestamp
  class_addmethod(OSCTimeTag_class, (method)OSCTimeTag_int, "int", A_LONG, 0);

  // float time
  class_addmethod(OSCTimeTag_class, (method)OSCTimeTag_float, "float", A_FLOAT, 0);

  // now (both inlets via proxy)
  class_addmethod(OSCTimeTag_class, (method)OSCTimeTag_now, "now", 0);

  // bang is equivalent to now
  class_addmethod(OSCTimeTag_class, (method)OSCTimeTag_now, "bang", 0);

  // special immediate handler (both inlets via proxy)
  class_addmethod(OSCTimeTag_class, (method)OSCTimeTag_immediate, "immediate", 0);

  // OSCTimeTag in (both inlets via proxy)
  class_addmethod(OSCTimeTag_class, (method)OSCTimeTag_OSCTimeTag, "OSCTimeTag", A_GIMME, 0);

  // OSCTimeTag in (both inlets via proxy)
  class_addmethod(OSCTimeTag_class, (method)OSCTimeTag_FullPacket, "FullPacket", A_GIMME, 0);

  // supports iso8601 string (both inlets via proxy)
  class_addmethod(OSCTimeTag_class, (method)OSCTimeTag_iso8601, "iso8601", A_GIMME, 0);

  // tooltip helper
  class_addmethod(OSCTimeTag_class, (method)OSCTimeTag_assist, "assist", A_CANT, 0);

  ps_FullPacket = gensym("FullPacket");
  ps_OSCTimeTag = gensym("OSCTimeTag");
  ps_iso8601 = gensym("iso8601");
  ps_tai = gensym("tai");

    class_register(CLASS_BOX, OSCTimeTag_class);
    return 0;

}

/* instance creation routine */

void *OSCTimeTag_new(t_symbol* s, short argc, t_atom *argv)
{

  OSCTimeTag *x;
  int i;

  x = object_alloc(OSCTimeTag_class);
  if(!x){
	  return NULL;
  }

  x->op = OP_TAG;
  x->to = TO_T;

  x->f = 1.0;

  x->a.sec = 0;
  x->a.frac_sec = 1;
  x->a.sign = 1;
  x->a.type = TIME_IMMEDIATE;

  x->b.sec = 0;
  x->b.frac_sec = 1;
  x->b.sign = 1;
  x->b.type = TIME_NULL;

  for(i = 0; i < argc; i++) {

    if(argv[i].a_type == A_SYM) {
      // look for @op
      if(strcmp(argv[i].a_w.w_sym->s_name, "@op") == 0) {

	if(i + 1 < argc) {
	  i++;

	  if(argv[i].a_type == A_SYM) {
	    if(strcmp(argv[i].a_w.w_sym->s_name, "tag") == 0) {
	      x->op = OP_TAG;
	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "+") == 0) {
	      x->op = OP_ADD;
	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "-") == 0) {
	      x->op = OP_DST;
	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "*") == 0) {
	      x->op = OP_MUL;
	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "cmp") == 0) {
	      x->op = OP_CMP;
	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "d") == 0) {
	      x->op = OP_DER;
	    } else {
	      object_post((t_object *)x, "OSC-timetag got unknown operator %s", argv[i].a_w.w_sym->s_name);
	    }
	  } else {
	    object_post((t_object *)x, "OSC-timetag got unexpected type for @op");
	  }

	} else {
	  object_post((t_object *)x, "OSC-timetag expects an operator after @op");
	}

      }

      // look for @to
      if(strcmp(argv[i].a_w.w_sym->s_name, "@to") == 0) {

      	if(i + 1 < argc) {
      	  i++;

      	  if(argv[i].a_type == A_SYM) {
      	    if(strcmp(argv[i].a_w.w_sym->s_name, "t") == 0) {
      	      x->to = TO_T;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "OSCTimeTag") == 0) {
      	      x->to = TO_T;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "i") == 0) {
      	      x->to = TO_I;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "int") == 0) {
      	      x->to = TO_I;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "unix") == 0) {
      	      x->to = TO_I;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "f") == 0) {
      	      x->to = TO_F;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "float") == 0) {
      	      x->to = TO_F;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "s") == 0) {
      	      x->to = TO_S;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "str") == 0) {
      	      x->to = TO_S;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "iso8601") == 0) {
      	      x->to = TO_S;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "FullPacket") == 0) {
      	      x->to = TO_P;
      	    } else if(strcmp(argv[i].a_w.w_sym->s_name, "#bundle") == 0) {
      	      x->to = TO_P;
      	    } else {
      	      object_post((t_object *)x, "OSC-timetag got unknown value for @to: %s", argv[i].a_w.w_sym->s_name);
      	    }
      	  } else {
      	    object_post((t_object *)x, "OSC-timetag got unexpected type for @to");
      	  }

      	} else {
          object_post((t_object *)x, "OSC-timetag expects a type after @to");
      	}

      }
    }

  }

  if(x->op == OP_CMP) {
    x->to = TO_I;
  }

  switch(x->to) {
  case TO_T:
    x->out_p[0] = outlet_new(x, "OSCTimeTag");
    break;
  case TO_F:
    x->out_p[0] = floatout(x);
    break;
  case TO_I:
    x->out_p[0] = intout(x);
    break;
  case TO_S:
    x->out_p[0] = outlet_new(x, "iso8601");
    break;
  case TO_P:
    x->out_p[0] = outlet_new(x, "FullPacket");
    break;
  }

  switch(x->op) {
  case OP_TAG: // only one inlet for these two cases
  case OP_DER:
    break;
  default:
    x->in_p[0] = proxy_new(x, 1, &x->in_i_unsafe);
    break;
  }

  return (x);
}

void OSCTimeTag_free(OSCTimeTag* x) {

  // this does not work, why?
  // release the proxy
  //freeobject(x->in_p[0]);
    object_free(x->in_p[0]);

}

// tooltip assist

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void OSCTimeTag_assist (OSCTimeTag *x, void *box, long msg, long arg, char *dstString) {
  if (msg == ASSIST_INLET) {
    if(arg == 0) {
      sprintf(dstString, "bang or 'now' for current time, int as unixtime, float to convert to relative time, or message 'OSCTimeTag', 'iso8601' or 'immediate'");
        } else if(arg == 1) {
      if(x->op == OP_MUL) {
	sprintf(dstString, "scaling factor as float");
      } else {
	sprintf(dstString, "bang or 'now' for current time, int as unixtime, relative time as float, or message 'OSCTimeTag', 'iso8601' or 'immediate'");
      }
    }
  } else if (msg == ASSIST_OUTLET) {
    if(x->op == OP_CMP) {
      sprintf(dstString, "int, 0 when equal, -1 for a < b, 1 for a > b");
    } else {
      switch(x->to) {
      case TO_T:
	sprintf(dstString, "ntp time as OSCTimeTag");
	break;
      case TO_I:
	sprintf(dstString, "unix timestamp as int");
	break;
      case TO_F:
	sprintf(dstString, "relative time as float");
	break;
      case TO_S:
	sprintf(dstString, "time in iso8601 string");
	break;
      case TO_P:
	sprintf(dstString, "transformed pacet as FullPacket");
	break;
      }
    }
  } else {
    object_post((t_object *)x, "OSCTimeTag_assist: unrecognized message %ld", msg);
  }
}

// this is where the op executes
struct ntptime OSCTimeTag_run(OSCTimeTag *x)
{

  struct ntptime r; // answer
  char s[32]; // string if we need it
  t_atom tt[2]; // max list output for timetag
  t_atom iso[1]; // iso list output

  double a, c;

  r.sign = 1;

  switch(x->op) {

  case OP_TAG:
    if(x->a.type == TIME_IMMEDIATE) {
      r.sec = 0;
      r.frac_sec = 1;
      r.sign = 1;
      r.type = TIME_IMMEDIATE;
    } else if(x->a.type == TIME_NOW) {
      OSCTimeTag_now_to_ntp(&r);
    } else {
      // TIME_STAMP -- passthru
      r.sec = x->a.sec;
      r.frac_sec = x->a.frac_sec;
      r.sign = x->a.sign;
      r.type = TIME_STAMP;
    }
    break;

  case OP_ADD:
  case OP_SUB:
  case OP_DST:
    if(x->a.type == TIME_NOW) {
      OSCTimeTag_now_to_ntp(&(x->a));
    }
    if(x->b.type == TIME_NOW) {
      OSCTimeTag_now_to_ntp(&(x->b));
    }
    if(x->a.type == TIME_IMMEDIATE || x->b.type == TIME_IMMEDIATE) { // undefined result
      r.sec = 0;
      r.frac_sec = 1;
    } else {
      if(x->op == OP_SUB || x->op == OP_DST) {
        // flip sign
        x->b.sign *= -1;

        // do add
        OSCTimeTag_add(&(x->a), &(x->b), &r);

        // flip sign back
        x->b.sign *= -1;
      } else {
        OSCTimeTag_add(&(x->a), &(x->b), &r);
      }

      // force result unsigned in this mode...
      if(x->op == OP_DST) {
        r.sign = 1;
      }

      if(x->to == TO_F) {
        outlet_float(x->out_p[0], OSCTimeTag_ntp_to_float(&r));
        return r;
      } else {
        if(r.sign == -1) {
          // result undefined
          r.sign = 1;
          r.sec = 0;
          r.frac_sec = 1;
          r.type = TIME_IMMEDIATE;
        }
      }
    }
    break;

  case OP_MUL:
    if(x->a.type == TIME_NOW) {
      OSCTimeTag_now_to_ntp(&(x->a));
    }
    if(x->a.type == TIME_IMMEDIATE) { // undefined result
      r.sec = 0;
      r.frac_sec = 1;
    } else {
      // could do this in two parts to keep better accuracy ?
      a = OSCTimeTag_ntp_to_float(&(x->a));
      c = a * x->f;
      if(x->to == TO_F) {
	outlet_float(x->out_p[0], c);
	return r;
      } else {
	OSCTimeTag_float_to_ntp(c, &r);
      }
    }
    break;

  case OP_CMP:
    if(x->a.type == TIME_NOW) {
      OSCTimeTag_now_to_ntp(&(x->a));
    }
    if(x->b.type == TIME_NOW) {
      OSCTimeTag_now_to_ntp(&(x->b));
    }
    if(x->a.type == TIME_IMMEDIATE || x->b.type == TIME_IMMEDIATE || x->b.type == TIME_NULL) { // undefined result
      outlet_int(x->out_p[0], 0);
    }
    if(x->a.sec == x->b.sec) {
      if(x->a.frac_sec == x->b.frac_sec) {
	outlet_int(x->out_p[0], 0);
      }
      else if(x->a.frac_sec < x->b.frac_sec) {
	outlet_int(x->out_p[0], -1);
      }
      else {
	outlet_int(x->out_p[0], 1);
      }
    }
    else if(x->a.sec < x->b.sec) {
      outlet_int(x->out_p[0], -1);
    }
    else {
      outlet_int(x->out_p[0], 1);
    }
    return r;
    break;

  case OP_DER:
    if(x->a.type == TIME_NOW) {
      OSCTimeTag_now_to_ntp(&(x->a));
    }

    if(x->b.type == TIME_NULL) {

      // propagate current to next
      x->b.type = x->a.type;
      x->b.sign = -1. * x->a.sign;
      x->b.sec = x->a.sec;
      x->b.frac_sec = x->a.frac_sec;

      // stop
      return r;

    }

    if(x->b.type != TIME_NULL && x->a.type != TIME_NULL) {

      // reset
      if(x->a.type == TIME_IMMEDIATE || x->b.type == TIME_IMMEDIATE) {

	// propagate current to next
	x->b.type = x->a.type;
	x->b.sign = -1. * x->a.sign;
	x->b.sec = x->a.sec;
	x->b.frac_sec = x->a.frac_sec;

	// stop
	return r;

      } else {

	OSCTimeTag_add(&(x->a), &(x->b), &r);

	if(x->to == TO_F) {
	  outlet_float(x->out_p[0], OSCTimeTag_ntp_to_float(&r));

	  // propagate current to next
	  x->b.type = x->a.type;
	  x->b.sign = -1. * x->a.sign;
	  x->b.sec = x->a.sec;
	  x->b.frac_sec = x->a.frac_sec;

	  // stop
	  return r;

	} else if(r.sign == -1) { // undefined result for this output mode
	  r.sec = 0;
	  r.frac_sec = 1; // undefined
	  r.type = TIME_IMMEDIATE;
	}

	// propagate current to next
	x->b.type = x->a.type;
	x->b.sign = -1. * x->a.sign;
	x->b.sec = x->a.sec;
	x->b.frac_sec = x->a.frac_sec;

	// return normally (using @to)
      }

    }

    break;

  }

  switch(x->to) {

  case TO_T:
    atom_setlong(&tt[0], r.sec);
    atom_setlong(&tt[1], r.frac_sec);
    outlet_anything(x->out_p[0], ps_OSCTimeTag, 2, tt);
    break;

  case TO_F:
    if(r.sec == 0 && r.frac_sec == 1) { // undefined result; use NAN
      outlet_float(x->out_p[0], NAN);
      break;
    } else {
      outlet_float(x->out_p[0], OSCTimeTag_ntp_to_float(&r));
    }
    break;

  case TO_I:
    if(r.sec == 0 && r.frac_sec == 1) { // undefined result; just use -1
      outlet_int(x->out_p[0], -1);
    } else {
      outlet_int(x->out_p[0], (long int)OSCTimeTag_ntp_to_ut(&r));
    }
    break;

  case TO_S:
    if(r.sec == 0 && r.frac_sec == 1) {
      iso[0].a_type = A_SYM;
      iso[0].a_w.w_sym = gensym("Z");
      outlet_anything(x->out_p[0], ps_iso8601, 1, iso); // undefined result
    } else {
      OSCTimeTag_ntp_to_iso8601(&r, s, 32);
      iso[0].a_type = A_SYM;
      iso[0].a_w.w_sym = gensym(s);
      outlet_anything(x->out_p[0], ps_iso8601, 1, iso);
    }
    break;
  }

  return r;

}

// methods--this copy & convert data, etc

void OSCTimeTag_now(OSCTimeTag *x)
{

    long i;
    i = proxy_getinlet((t_object *)x);

    if(i == 0) {
        x->a.type = TIME_NOW;
        OSCTimeTag_run(x);
    }
    else if(i == 1) {
        x->b.type = TIME_NOW;
    }
}

void OSCTimeTag_immediate(OSCTimeTag *x)
{
    long i;
    i = proxy_getinlet((t_object *)x);

    if(i == 0) {
        x->a.type = TIME_IMMEDIATE;
        OSCTimeTag_run(x);
    }
    else if(i == 1) {
        x->b.type = TIME_IMMEDIATE;
    }

}

void OSCTimeTag_int(OSCTimeTag *x, long int ut)
{

    long i;
    i = proxy_getinlet((t_object *)x);

    if(i == 0) {
        x->a.type = TIME_STAMP;
        OSCTimeTag_ut_to_ntp(ut, &(x->a));
        OSCTimeTag_run(x);
    }
    else if(i == 1) {
        x->b.type = TIME_STAMP;
        OSCTimeTag_ut_to_ntp(ut, &(x->b));
    }
}

void OSCTimeTag_float(OSCTimeTag *x, double d)
{

    long i;

    i = proxy_getinlet((t_object *)x);

    if(i == 0) {
        x->a.type = TIME_STAMP;
        OSCTimeTag_float_to_ntp(d, &(x->a));
        OSCTimeTag_run(x);
    }
    else if(i == 1) {
        if(x->op == OP_MUL) {
            x->f = d;
        } else {
            x->b.type = TIME_STAMP;
            OSCTimeTag_float_to_ntp(d, &(x->b));
        }
    }

}

void OSCTimeTag_FullPacket(OSCTimeTag *x, t_symbol* s, int argc, t_atom* argv) {

  long i;
  unsigned long sec;
  unsigned long frac_sec;

  struct ntptime r;

  char* data;

  sec = 0;
  frac_sec = 0;

  i = proxy_getinlet((t_object *)x);
  if(argc == 2 && argv[0].a_type == A_LONG && argv[0].a_w.w_long >= 16 && argv[1].a_type == A_LONG && argv[1].a_w.w_long != 0) {

    data = (char*)argv[1].a_w.w_long;
    if(strcmp(data, "#bundle") == 0) {

      sec =      ntohl(*((unsigned long *)(data+8)));
      frac_sec = ntohl(*((unsigned long *)(data+12)));

      if(i == 0) {
	if(sec == 0 && frac_sec == 1) {
	  if(x->op == OP_TAG) {
	    x->a.type = TIME_NOW;
	  } else {
	    x->a.type = TIME_IMMEDIATE;
	  }
	} else {
	  x->a.type = TIME_STAMP;
	  x->a.sec = sec;
	  x->a.frac_sec = frac_sec;
	}

	r = OSCTimeTag_run(x);

	if(x->to == TO_P) {
	  // write result into packet...
	  (*((unsigned long *)(data+8))) = htonl(r.sec);
	  (*((unsigned long *)(data+12))) = htonl(r.frac_sec);

	  // output transformed FullPacket
	  outlet_anything(x->out_p[0], ps_FullPacket, 2, argv);
	}

      }
      else if(i == 1) {
	if(sec == 0 && frac_sec == 1) {
	  x->b.type = TIME_IMMEDIATE;
	} else {
	  x->b.type = TIME_STAMP;
	  x->b.sec = sec;
	  x->b.frac_sec = frac_sec;
	}
      }
    }
  }

}


void OSCTimeTag_OSCTimeTag(OSCTimeTag *x, t_symbol* s, int argc, t_atom* argv) {

    long i;
    unsigned long sec;
    unsigned long frac_sec;

    i = proxy_getinlet((t_object *)x);

    if(argc == 2 && argv[0].a_type == A_LONG && argv[1].a_type == A_LONG) {

        sec = (unsigned long)argv[0].a_w.w_long;
        frac_sec = (unsigned long)argv[1].a_w.w_long;

        if(i == 0) {
	  x->a.sec = sec;
	  x->a.frac_sec = frac_sec;
            if(sec == 0 && frac_sec == 1) {
                x->a.type = TIME_IMMEDIATE;
            } else {
                x->a.type = TIME_STAMP;
            }
	    OSCTimeTag_run(x);
        }
        else if(i == 1) {
	  x->b.sec = sec;
	  x->b.frac_sec = frac_sec;
            if(sec == 0 && frac_sec == 1) {
                x->b.type = TIME_IMMEDIATE;
            } else {
                x->b.type = TIME_STAMP;
            }
	    // passive inlet
        }
    } else {
        object_post((t_object *)x, "OSC-timetag: got invalid OSCTimeTag");
    }

}

void OSCTimeTag_iso8601(OSCTimeTag *x, t_symbol* s, int argc, t_atom* argv) {

    long i;
    i = proxy_getinlet((t_object *)x);

    if(argc == 1 && argv[0].a_type == A_SYM) {

        if(i == 0) {
            x->a.type = TIME_STAMP;
            OSCTimeTag_iso8601_to_ntp(argv[0].a_w.w_sym->s_name, &(x->a));
            OSCTimeTag_run(x);
        }
        else if(i == 1) {
            x->b.type = TIME_STAMP;
            OSCTimeTag_iso8601_to_ntp(argv[0].a_w.w_sym->s_name, &(x->b));
        }

    }

}
