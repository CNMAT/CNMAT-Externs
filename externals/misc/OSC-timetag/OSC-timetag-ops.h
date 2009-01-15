#ifndef _OSC_TIMETAG_OPS_H
#define _OSC_TIMETAG_OPS_H

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
void OSCTimeTag_iso8601_to_ntp(char* s, struct ntptime* n);
void OSCTimeTag_ntp_to_iso8601(struct ntptime* n, char* s);

void OSCTimeTag_float_to_ntp(double d, struct ntptime* n);
double OSCTimeTag_ntp_to_float(struct ntptime* n);

void OSCTimeTag_ut_to_ntp(long int ut, struct ntptime* n);
long int OSCTimeTag_ntp_to_ut(struct ntptime* n);

// operations
void OSCTimeTag_add(struct ntptime* a, struct ntptime* b, struct ntptime* r);
int OSCTimeTag_cmp(struct ntptime* a, struct ntptime* b);

// generation
void OSCTimeTag_now_to_ntp(struct ntptime* n);

#endif
