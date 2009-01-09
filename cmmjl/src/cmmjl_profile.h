#ifndef __CMMJL_PROFILE_H__
#define __CMMJL_PROFILE_H__

#ifdef	CMMJL_PROFILE

#include "cycle.h"

#ifndef CMMJL_TIMER
#define CMMJL_TIMER() ticks t
#endif
#ifndef CMMJL_TIMER_START
#define CMMJL_TIMER_START() t = getticks()
#endif
#ifndef CMMJL_TIMER_END
#define CMMJL_TIMER_END() post("elapsed time = %f", elapsed(getticks(), t))
#endif

#ifndef CMMJL_ENTER
#define CMMJL_ENTER(x) if(cmmjl_entrance_count_inc(x, __FUNCTION__) > 1){ \
		error("entrance count for %s: %s() = %d",		\
		      NAME, __FUNCTION__, cmmjl_entrance_count_get(x, __FUNCTION__)); \
	}
#endif
#ifndef CMMJL_EXIT
#define CMMJL_EXIT(x) cmmjl_entrance_count_dec(x, __FUNCTION__)
#endif

#else

#ifndef CMMJL_TIMER
#define CMMJL_TIMER()
#endif
#ifndef CMMJL_TIMER_START
#define CMMJL_TIMER_START()
#endif
#ifndef CMMJL_TIMER_END
#define CMMJL_TIMER_END()
#endif

#endif // CMMJL_PROFILE

int cmmjl_entrance_count_inc(void *x, const char *func);
int cmmjl_entrance_count_dec(void *x, const char *func);
int cmmjl_entrance_count_get(void *x, const char *func);

#endif // __CMMJL_PROFILE_H__
