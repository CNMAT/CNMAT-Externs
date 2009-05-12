
// need sprintf, sscanf
#include <stdio.h>

// need math.h
#include <math.h>

// need strcmp
#include <string.h>

// need htonl
#include <arpa/inet.h>

// crossplatform gettimeofday
#include "timeval.h"

// own header
#include "cmmjl_osc_timetag.h"

// add or subtract

void cmmjl_osc_timetag_add(ntptime* a, ntptime* b, ntptime* r) {
  
  if((a->sign == 1 && b->sign == 1) || (a->sign == -1 && b->sign == -1)) {
    
    r->sec = a->sec + b->sec;
    r->frac_sec = a->frac_sec + b->frac_sec;
    
    if(r->frac_sec < a->frac_sec) { // rollover occurred
      r->sec += 1;
    }
    
    r->sign = a->sign;
    
  } else if((a->sign == 1 && b->sign == -1)) {
    
    if(a->sec > b->sec || (a->sec == b->sec && a->frac_sec >= b->frac_sec)) {
      
      r->sec = a->sec - b->sec;
      r->sign = 1;
      
      if(a->frac_sec >= b->frac_sec) {
	r->frac_sec = a->frac_sec - b->frac_sec;
      } else {
	if(r->sec == 0) {
	  r->frac_sec = b->frac_sec - a->frac_sec;
	  r->sign = -1;
	} else {
	  r->sec--;
	  r->frac_sec = a->frac_sec - b->frac_sec;
	}
      }
      
      
    } else {
      
      r->sec = b->sec - a->sec;
      
      if(a->frac_sec >= b->frac_sec) {
	r->frac_sec = a->frac_sec - b->frac_sec;
      } else {
	r->frac_sec = b->frac_sec - a->frac_sec;
      }
      
      r->sign = -1;
      
    }
    
  } else { // a.sign == -1 and b.sign == 1...
    cmmjl_osc_timetag_add(b, a, r);
  }
  
  r->type = TIME_STAMP;
  
}

int cmmjl_osc_timetag_cmp(ntptime* a, ntptime* b) {
  
  if(a->sec < b->sec || (a->sec == b->sec && a->frac_sec < b->frac_sec)) {
    return -1;
  }
  
  if(a->sec > b->sec || (a->sec == b->sec && a->frac_sec > b->frac_sec)) {
    return 1;
  }
  
  return 0;
  
}

// conversion functions

void cmmjl_osc_timetag_iso8601_to_ntp(char* s, ntptime* n) {
  
    struct tm t;
    float sec;
    char s1[20];
    
    // read out the fractions part
    sscanf(s, "%*d-%*d-%*dT%*d:%*d:%fZ", &sec);
    
    // null-terminate the string
    strncat(s1, s, 19);

    // parse the time
    strptime(s1, "%Y-%m-%dT%H:%M:%S", &t);
    
    cmmjl_osc_timetag_ut_to_ntp(timegm(&t), n);
    n->frac_sec = (unsigned long int)(fmod(sec, 1.0) * 4294967295.0);

    n->sign = 1;
}

void cmmjl_osc_timetag_ntp_to_iso8601(ntptime* n, char* s) {
  
    time_t i;
    struct tm* t;
    char s1[24];
    char s2[10];
    double d;
    
    i = (time_t)cmmjl_osc_timetag_ntp_to_ut(n);
    d = cmmjl_osc_timetag_ntp_to_float(n);
    t = gmtime(&i);
    
    strftime(s1, 24, "%Y-%m-%dT%H:%M:%S", t);
    sprintf(s2, "%05fZ", fmod(d, 1.0));
    sprintf(s, "%s.%s", s1, s2+2);
}

void cmmjl_osc_timetag_float_to_ntp(double d, ntptime* n) {

    double sec;
    double frac_sec;

    if(d > 0) {
      n->sign = 1;
    } else {
      d *= -1;
      n->sign = -1;
    }
    
    frac_sec = fmod(d, 1.0);
    sec = d - frac_sec;
    
    n->sec = (unsigned long int)(sec);
    n->frac_sec= (unsigned long int)(frac_sec * 4294967295.0);
    n->type = TIME_STAMP;
}

double cmmjl_osc_timetag_ntp_to_float(ntptime* n) {
  if(n->sign == 1) {
    return ((double)(n->sec)) + ((double)((unsigned long int)(n->frac_sec))) / 4294967295.0;
  } else {
    return -1. * (((double)(n->sec)) + (((double)((unsigned long int)(n->frac_sec))) / 4294967295.0));
  }
}

void cmmjl_osc_timetag_ut_to_ntp(long int ut, ntptime* n) {

    struct timeval tv;
    struct timezone tz;
    
    gettimeofday(&tv, &tz); // this is just to get the timezone...
    
    n->sec = (unsigned long)2208988800UL + 
            (unsigned long)ut - 
            (unsigned long)(60 * tz.tz_minuteswest) +
            (unsigned long)(tz.tz_dsttime == 1 ? 3600 : 0);

    n->frac_sec = 0;
}

long int cmmjl_osc_timetag_ntp_to_ut(ntptime* n) {

    struct timeval tv;
    struct timezone tz;
    
    gettimeofday(&tv, &tz); // this is just to get the timezone...
    
    return n->sec - (unsigned long)2208988800UL + (unsigned long)(60 * tz.tz_minuteswest) - (unsigned long)(tz.tz_dsttime == 1 ? 3600 : 0);
}

void cmmjl_osc_timetag_now_to_ntp(ntptime* n) {

    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);
    
    n->sec = (unsigned long)2208988800UL + 
        (unsigned long) tv.tv_sec - 
        (unsigned long)(60 * tz.tz_minuteswest) +
        (unsigned long)(tz.tz_dsttime == 1 ? 3600 : 0);
    
    n->frac_sec = (unsigned long)(tv.tv_usec * 4295); // 2^32-1 / 1.0e6

    n->sign = 1;
}

t_cmmjl_error cmmjl_osc_timetag_get(long n, long ptr, ntptime *r){
	if(strcmp((char *)ptr, "#bundle") != 0) {
		r = NULL;
		return CMMJL_OSC_EBADBNDL;
        }
	char *data = (char *)ptr;
	if(n < 16){
		r = NULL;
		return CMMJL_OSC_EBADBNDL;
	}
	r->sec = ntohl(*((unsigned long *)(data + 8)));
	r->frac_sec = ntohl(*((unsigned long *)(data + 12)));
	r->sign = 1;
	r->type = TIME_STAMP;
	return CMMJL_SUCCESS;
}
