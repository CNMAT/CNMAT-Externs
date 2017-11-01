
// need sprintf, sscanf
#include <stdio.h>

// need math.h
#include <math.h>

// need strcmp
#include <string.h>

// need htonl
#ifdef WIN_VERSION
#include <Winsock.h>
#else
#include <arpa/inet.h>
#endif

// crossplatform gettimeofday
#include "timeval.h"

// own header
#include "OSC-timetag-ops.h"

// add or subtract

void OSCTimeTag_add(struct ntptime* a, struct ntptime* b, struct ntptime* r) {

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
    OSCTimeTag_add(b, a, r);
  }

  r->type = TIME_STAMP;

}

int OSCTimeTag_cmp(struct ntptime* a, struct ntptime* b) {

  if(a->sec < b->sec || (a->sec == b->sec && a->frac_sec < b->frac_sec)) {
    return -1;
  }

  if(a->sec > b->sec || (a->sec == b->sec && a->frac_sec > b->frac_sec)) {
    return 1;
  }

  return 0;

}

// conversion functions
/*
static void osc_timetag_setenv(const char *name, const char *value)
{
#if !(defined _WIN32) || defined HAVE_SETENV
  	setenv(name, value, 1);
#else
  	int len = strlen(name) + 1 + strlen(value) + 1;
  	char str[len];
  	sprintf(str, "%s=%s", name, value);
  	putenv(str);
#endif
}

static void osc_timetag_unsetenv(const char *name)
{
#if !(defined _WIN32) || defined HAVE_SETENV
	unsetenv(name);
#else
    	int len = strlen(name) + 2;
  	char str[len];
  	sprintf(str, "%s=", name);
  	putenv(str);
#endif
}


time_t osc_timetag_timegm (struct tm *tm)
{
	time_t ret;
	char *tz;

	tz = getenv("TZ");
#ifdef WIN_VERSION
	osc_timetag_setenv("TZ", "UTC");
#else
	osc_timetag_setenv("TZ", "");
#endif
	tzset();
	ret = mktime(tm);
	if(tz){
		osc_timetag_setenv("TZ", tz);
	}else{
		osc_timetag_unsetenv("TZ");
	}
	tzset();
	return ret;
}


void OSCTimeTag_iso8601_to_ntp(char* s, struct ntptime* n) {

    struct tm t;
    float sec;
    char s1[32];

    // read out the fractions part
    sscanf(s, "%*d-%*d-%*dT%*d:%*d:%fZ", &sec);

    // null-terminate the string
    //strncat(s1, s, 19);
    snprintf(s1, 20, "%s", s);

    // parse the time
    strptime(s1, "%Y-%m-%dT%H:%M:%S", &t);

    time_t time = timegm(&t);
    OSCTimeTag_ut_to_ntp(time, n);
    n->frac_sec = (unsigned long int)(fmod(sec, 1.0) * 4294967295.0);

    n->sign = 1;
}

void OSCTimeTag_ntp_to_iso8601(struct ntptime* n, char* s) {

    time_t i;
    struct tm* t;
    char s1[24];
    char s2[10];
    double d;

    i = (time_t)OSCTimeTag_ntp_to_ut(n);
    d = OSCTimeTag_ntp_to_float(n);
    t = gmtime(&i);

    strftime(s1, 24, "%Y-%m-%dT%H:%M:%S", t);
    sprintf(s2, "%05fZ", fmod(d, 1.0));
    sprintf(s, "%s.%s", s1, s2+2);
}
*/

// from osc_timetag.c


uint32_t osc_timetag_ntp_getSeconds(struct ntptime* t)
{
    return t->sec;
}

uint32_t osc_timetag_ntp_getFraction(struct ntptime* t)
{
    return t->frac_sec;
}

//Function isleap:  Determines if year is a leap year or not, without using modulo.
//Year is a leap year if year mod 400 = 0, OR if (year mod 4 = 0 AND year mod 100 <> 0).
int osc_timetag_isleap(unsigned short int year)
{
    short int yrtest;
    yrtest=year;

    //year modulo 400 = 0? if so, it's a leap year
    while(yrtest>0){
        yrtest-=400;
    }
    if(yrtest==0){ //then year modulo 400 = 0
        return 1; //it's a leap year
    }
    yrtest=year;

    //year modulo 4 = 0 and year modulo 100 <>0? if so, it's a leap year
    while(yrtest>0){
        yrtest-=4;
    }
    if(yrtest==0){ //then year modulo 4 = 0
        yrtest=year;
        while(yrtest>0){  //so test for modulo 100
            yrtest-=100;
        }
        if(yrtest<0){  //then year modulo 100 <>0
            return 1; //it's a leap year
        }else{
            return 0; //not a leap year
        }
    }else{
        return 0;  //year modulo 4 <> 0, not a leap year
    }
}

//Function getmonth computes the month and day of month given the day of the year,
//accounting for leap years
unsigned short int osc_timetag_getmonth(unsigned short int *day, unsigned short int leap)
{
    const unsigned short int JAN = 1;
    const unsigned short int FEB = 2;
    const unsigned short int MAR = 3;
    const unsigned short int APR = 4;
    const unsigned short int MAY = 5;
    const unsigned short int JUN = 6;
    const unsigned short int JUL = 7;
    const unsigned short int AUG = 8;
    const unsigned short int SEP = 9;
    const unsigned short int OCT = 10;
    const unsigned short int NOV = 11;
    const unsigned short int DEC = 12;

    unsigned short int JAN_LAST = 31;
    unsigned short int FEB_LAST = 59;
    unsigned short int MAR_LAST = 90;
    unsigned short int APR_LAST = 120;
    unsigned short int MAY_LAST = 151;
    unsigned short int JUN_LAST = 181;
    unsigned short int JUL_LAST = 212;
    unsigned short int AUG_LAST = 243;
    unsigned short int SEP_LAST = 273;
    unsigned short int OCT_LAST = 304;
    unsigned short int NOV_LAST = 334;
    unsigned short int DEC_LAST = 365;

    //correct monthly end dates for leap years (leap=1=leap year, 0 otherwise)
    if(leap > 0){
        if(leap <= 1){
            FEB_LAST += leap;
            MAR_LAST += leap;
            APR_LAST += leap;
            MAY_LAST += leap;
            JUN_LAST += leap;
            JUL_LAST += leap;
            AUG_LAST += leap;
            SEP_LAST += leap;
            OCT_LAST += leap;
            NOV_LAST += leap;
            DEC_LAST += leap;

        }else if(leap > 1){ //error condition
            return 0;
        }
    }else if(leap < 0){  //error condition
        return 0;
    }
    //Determine month & day of month from day of year
    if(*day <= JAN_LAST){
        return JAN;  //day is already correct
    }else if(*day <= FEB_LAST){
        *day -= JAN_LAST;
        return FEB;
    }else if(*day <= MAR_LAST){
        *day -= (FEB_LAST);
        return MAR;
    }else if(*day <= APR_LAST){
        *day -= (MAR_LAST);
        return APR;
    }else if(*day <= MAY_LAST){
        *day -= (APR_LAST);
        return MAY;
    }else if(*day <= JUN_LAST){
        *day -= (MAY_LAST);
        return JUN;
    }else if(*day <= JUL_LAST){
        *day -= (JUN_LAST);
        return JUL;
    }else if(*day <= AUG_LAST){
        *day -= (JUL_LAST);
        return AUG;
    }else if(*day <= SEP_LAST){
        *day -= (AUG_LAST);
        return SEP;
    }else if(*day <= OCT_LAST){
        *day -= (SEP_LAST);
        return OCT;
    }else if(*day <= NOV_LAST){
        *day -= (OCT_LAST);
        return NOV;
    }else if(*day <= DEC_LAST){
        *day -= (NOV_LAST);
        return DEC;
    }else{
        return 0;
    }
}

#define DWORD uint64_t
// osc_timetag_toISO8601
void OSCTimeTag_ntp_to_iso8601(struct ntptime* timetag, char *buf, long n)
{

    if(!buf){
        return;
    }
    char s1[20];

    uint32_t secs = osc_timetag_ntp_getSeconds(timetag);
    unsigned short int year, month, day, hour, minute, leap = 0;

    const DWORD SEC_PER_YEAR = 31536000;
    const DWORD SEC_PER_DAY = 86400;
    const unsigned short int SEC_PER_HR = 3600;
    const unsigned short int SEC_PER_MIN = 60;


    //secs=-2208988800;//SNTPGetUTCSeconds();
    //secs = abs(secs);
    for(year = 0; secs >= SEC_PER_YEAR; year++){ //determine # years elapse since epoch
        secs -= SEC_PER_YEAR;
        if(osc_timetag_isleap(year)){
            secs -= SEC_PER_DAY;
        }
    }
    //year+=1970;  //1/1/1970, 00:00 is epoch
    year += 1900;  //1/1/1900, 00:00 is ntp epoch
    //for (yrcount=1970;yrcount<year;yrcount++) //scroll from 1970 to last year to find leap yrs.
    /*
     for(yrcount = 1900; yrcount < year; yrcount++){
     leap = osc_timetag_isleap(yrcount);
     if(leap == 1){
     secs -= SEC_PER_DAY;  //if it's a leap year, subtract a day's worth of seconds
     }
     }
     */
    leap = osc_timetag_isleap(year); //Is this a leap year?


    for(day = 1; secs >= SEC_PER_DAY; day++){ //determine # days elapsed in current year
        secs -= SEC_PER_DAY;
    }
    for(hour = 0; secs >= SEC_PER_HR; hour++){  //determine hours elapsed in current day
        secs -= SEC_PER_HR;
    }
    for(minute = 0; secs >= SEC_PER_MIN; minute++){  //determine minutes elapsed in current hour
        secs -= SEC_PER_MIN;
    }

    //The value of secs at the end of the minutes loop is the seconds elapsed in the
    //current minute.
    //Given the year & day of year, determine month & day of month
    month = osc_timetag_getmonth(&day, leap);

    //convert it all to the iso-8601 string
    {
        char yearstr[5], monthstr[3], daystr[3], hourstr[3], minutestr[3], secstr[3];
        /*
         itoa(year,yearstr);
         itoa(month,monthstr);
         itoa(day,daystr);
         itoa(hour,hourstr);
         itoa(minute,minutestr);
         itoa(sec,secstr);
         */
        snprintf(yearstr, sizeof(yearstr), "%04u", year);
        snprintf(monthstr, sizeof(monthstr), "%02u", month);
        snprintf(daystr, sizeof(daystr), "%02u", day);
        snprintf(hourstr, sizeof(hourstr), "%02u", hour);
        snprintf(minutestr, sizeof(minutestr), "%02u", minute);
        snprintf(secstr, sizeof(secstr), "%02u", secs);

        //All items except for year can be 1 or two digits, need to adjust accordingly
        /*
         if(month < 10){
         monthstr[1] = monthstr[0];
         monthstr[0] = '0';
         }
         if(day < 10){
         daystr[1] = daystr[0];
         daystr[0] = '0';
         }
         if(hour < 10){
         hourstr[1] = hourstr[0];
         hourstr[0] = '0';
         }
         if(minute < 10){
         minutestr[1] = minutestr[0];
         minutestr[0] = '0';
         }
         if(secs < 10){
         secstr[1] = secstr[0];
         secstr[0] = '0';
         }
         */

        //put it all together
        long i = 0;
        s1[i++] = yearstr[0];
        s1[i++] = yearstr[1];
        s1[i++] = yearstr[2];
        s1[i++] = yearstr[3];
        s1[i++] = '-';
        s1[i++] = monthstr[0];
        s1[i++] = monthstr[1];
        s1[i++] = '-';
        s1[i++] = daystr[0];
        s1[i++] = daystr[1];
        s1[i++] = 'T';
        s1[i++] = hourstr[0];
        s1[i++] = hourstr[1];
        s1[i++] = ':';
        s1[i++] = minutestr[0];
        s1[i++] = minutestr[1];
        s1[i++] = ':';
        s1[i++] = secstr[0];
        s1[i++] = secstr[1];
        s1[i++] = '\0';
    }

    double d = OSCTimeTag_ntp_to_float(timetag);
    double dm1 = fmod(d, 1.0);
    //int l = osc_strfmt_float64(NULL, 0, dm1);
    //int l = snprintf(NULL, 0, "%05fZ", dm1);
    char s2[24];
    snprintf(s2, 24, "%05fZ", dm1);
    //osc_strfmt_float64(s2, l + 1, dm1);
    //long n = 19 + snprintf(NULL, 0, "%05fZ", fmod(d, 1.0));
    snprintf(buf, n, "%s.%s", s1, s2+2);

    /*
     time_t i;
     struct tm *t;
     char s1[24];
     //char s2[10];
     double d;

     t_osc_timetag_ntptime ntptime = *((t_osc_timetag_ntptime *)&timetag);

     i = (time_t)osc_timetag_ntp_to_ut(ntptime);
     d = osc_timetag_ntp_to_float(ntptime);
     t = gmtime(&i);
     //printf("fucked: 0x%llx %f\n", i, d);
    	//t = localtime(&i);

     strftime(s1, 24, "%Y-%m-%dT%H:%M:%S", t);
     double dm1 = fmod(d, 1.0);
     int l = osc_strfmt_float64(NULL, 0, dm1);
     char s2[l + 1];
     //sprintf(s2, "%05fZ", fmod(d, 1.0));
     osc_strfmt_float64(s2, l + 1, dm1);
     return snprintf(buf, n, "%s.%sZ", s1, s2+2);
     */

}


//void osc_timetag_fromISO8601(char *s, struct ntptime *timetag)
void OSCTimeTag_iso8601_to_ntp(char* s, struct ntptime* timetag)
{
    struct tm t;
    memset(&t, '\0', sizeof(struct tm));
    double fsec;
    //char s1[];
    //memset(s1, '\0', sizeof(s1));

    // read out the fractions part
    sscanf(s, "%*d-%*d-%*dT%*d:%*d:%lfZ", &fsec);
    /*
     time_t now = time(NULL);
     struct tm *unsafe = localtime(&now);
     if(unsafe){
     t = *unsafe;
     }
     */
    // null-terminate the string
    //strncat(s1, s, OSC_TIMETAG_MAX_STRING_LENGTH - 1);

    // parse the time
    strptime(s, "%Y-%m-%dT%H:%M:%S", &t);
   // printf("year: %d, month: %d, mday: %d, yday: %d, hour: %d, min: %d, sec: %d\n", t.tm_year, t.tm_mon, t.tm_mday, t.tm_yday, t.tm_hour, t.tm_min, t.tm_sec);

    const DWORD SEC_PER_YEAR = 31536000;
    const DWORD SEC_PER_DAY = 86400;
    const unsigned short int SEC_PER_HR = 3600;
    const unsigned short int SEC_PER_MIN = 60;

    uint32_t sec = 0;
    for(int i = 0; i < t.tm_year; i++){
        sec += SEC_PER_YEAR;
        if(osc_timetag_isleap(i + 1900)){
            sec += SEC_PER_DAY;
        }
    }

    int leap = osc_timetag_isleap(t.tm_year + 1900);

    int days_in_months[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

    int offset = leap && (t.tm_mon > 1) ? 1 : 0;
    sec += ((days_in_months[t.tm_mon] + offset + t.tm_mday ) * SEC_PER_DAY);
    sec += (t.tm_hour * SEC_PER_HR);
    sec += (t.tm_min * SEC_PER_MIN);
    sec += t.tm_sec;
    timetag->sec = sec;

    //timetag->sec = (t.tm_year * 3.15569e7) + (t.tm_yday * 86400) + (t.tm_hour * 3600) + (t.tm_min * 60) + t.tm_sec;
    //osc_timetag_ut_to_ntp(mktime(&t), timetag);
    //t_osc_timetag_ntptime n;
    //osc_timetag_ut_to_ntp(mktime(&t), &n);

    /**************************************************
     * The connversion from double precision float to 32-bit int obviously
     * results in a loss of precision that I don't think can be overcome...
     **************************************************/
    timetag->frac_sec = (uint32_t)(fmod(fsec, 1.0) * 4294967295.0);
    //*timetag = *((t_osc_timetag *)(&n));
    timetag->sign = 1;
    timetag->type = TIME_STAMP;

}


// << end osc_timetag.c

void OSCTimeTag_float_to_ntp(double d, struct ntptime* n) {

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

    n->sec = (uint32_t)(sec);
    n->frac_sec= (uint32_t)(frac_sec * 4294967295.0);
    n->type = TIME_STAMP;
}

double OSCTimeTag_ntp_to_float(struct ntptime* n) {
  if(n->sign == 1) {
    return ((double)(n->sec)) + ((double)((uint64_t)(n->frac_sec))) / 4294967295.0;
  } else {
    return -1. * (((double)(n->sec)) + (((double)((uint64_t)(n->frac_sec))) / 4294967295.0));
  }
}

void OSCTimeTag_ut_to_ntp(time_t ut, struct ntptime* n) {

    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz); // this is just to get the timezone...

    n->sec = (uint64_t)2208988800UL +
            (uint32_t)ut -
            (uint64_t)(60 * tz.tz_minuteswest) +
            (uint32_t)(tz.tz_dsttime == 1 ? 3600 : 0);

    n->frac_sec = 0;
}

time_t OSCTimeTag_ntp_to_ut(struct ntptime* n) {

    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz); // this is just to get the timezone...

    return n->sec - (time_t)2208988800UL + (time_t)(60 * tz.tz_minuteswest) - (time_t)(tz.tz_dsttime == 1 ? 3600 : 0);
}

void OSCTimeTag_now_to_ntp(struct ntptime* n) {

    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    n->sec = (uint32_t)2208988800UL +
    (uint32_t) tv.tv_sec;// -
        //(unsigned long)(60 * tz.tz_minuteswest) +
        //(unsigned long)(tz.tz_dsttime == 1 ? 3600 : 0);

    n->frac_sec = (uint32_t)(tv.tv_usec * 4295); // 2^32-1 / 1.0e6
    n->sign = 1;
}
