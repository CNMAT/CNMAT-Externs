#ifndef __OSC_PROFILE_H__
#define __OSC_PROFILE_H__

#ifdef __cplusplus
extern "C" {
#endif

//#define RDTSC_CYCLES_PER_SECOND 2.261E09

static __inline__ unsigned long long osc_profile_rdtsc(void){
	unsigned hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

inline double osc_profile_getTime(){
	//output time in seconds
	unsigned long long tsc;
	tsc = osc_profile_rdtsc();
	return (double)tsc;
}

inline double osc_profile_getCyclesPerSecond(){
	double t1 = osc_profile_getTime();
        usleep(1000000);
        double t2 = osc_profile_getTime();
	double cps = floor((t2 - t1) / 10000000.) * 10000000.;
        return cps;
}

#ifdef __OSC_PROFILE__
#define RDTSC_CYCLES_PER_SECOND osc_profile_getCyclesPerSecond();
#define TIMER_START(varname, rdtsc_cps) int varname##_line_start = __LINE__;	\
	double varname##_start = osc_profile_getTime() / rdtsc_cps

#define TIMER_STOP(varname, rdtsc_cps) int varname##_line_stop = __LINE__;	\
	double varname##_stop = osc_profile_getTime() / rdtsc_cps

#define TIMER_PRINTF(varname) printf("%s time elapsed: %f\" (%f ms).  (%s:%d--%d)\n", #varname, varname##_stop - varname##_start, (varname##_stop - varname##_start) * 1000., __PRETTY_FUNCTION__, varname##_line_start, varname##_line_stop);
#define TIMER_SNPRINTF(varname, bufname) char bufname [128]; snprintf(bufname, 128, "%s time elapsed: %f\" (%f ms).  (%s:%d--%d)", #varname, varname##_stop - varname##_start, (varname##_stop - varname##_start) * 1000., __PRETTY_FUNCTION__, varname##_line_start, varname##_line_stop);
#else
#define RDTSC_CYCLES_PER_SECOND 0
#define TIMER_START(varname, rdtsc_cps)
#define TIMER_STOP(varname, rdtsc_cps)
#define TIMER_PRINTF(varname)
#define TIMER_SNPRINTF(varname, bufname)
#endif

#ifdef __cplusplus
}
#endif

#endif //  __OSC_PROFILE_H__
