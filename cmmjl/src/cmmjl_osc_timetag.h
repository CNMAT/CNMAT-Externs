#ifndef __CMMJL_OSC_TIMETAG_H__
#define __CMMJL_OSC_TIMETAG_H__

#define TIME_NULL       0 
#define TIME_NOW        1
#define TIME_IMMEDIATE  2
#define TIME_STAMP      3

struct ntptime 
{
  unsigned long int sec;
  unsigned long int frac_sec;
  int sign;
  int type;
};

// conversions 
void cmmjl_osc_timetag_iso8601_to_ntp(char* s, struct ntptime* n);
void cmmjl_osc_timetag_ntp_to_iso8601(struct ntptime* n, char* s);

void cmmjl_osc_timetag_float_to_ntp(double d, struct ntptime* n);
double cmmjl_osc_timetag_ntp_to_float(struct ntptime* n);

void cmmjl_osc_timetag_ut_to_ntp(long int ut, struct ntptime* n);
long int cmmjl_osc_timetag_ntp_to_ut(struct ntptime* n);

// operations
void cmmjl_osc_timetag_add(struct ntptime* a, struct ntptime* b, struct ntptime* r);
int cmmjl_osc_timetag_cmp(struct ntptime* a, struct ntptime* b);

// generation
void cmmjl_osc_timetag_now_to_ntp(struct ntptime* n);

#endif
