// critical.h -- platform-independent way to define critical regions

#ifndef _CRITICAL_H_
#define _CRITICAL_H_

#ifdef MAC_VERSION

#ifndef __MULTIPROCESSING__
#include <Multiprocessing.h>
#endif

#ifndef __THREADS__
#include <Threads.h>
#endif

#define CRITICAL_DEFAULT 2000 * kDurationMillisecond

typedef MPCriticalRegionID t_critical;
typedef ThreadID t_thread;

#if TARGET_API_MAC_CARBON
#define critical_new(x) (MPCreateCriticalRegion(x))
#define critical_enter(x) (MPEnterCriticalRegion(x,CRITICAL_DEFAULT))
#define critical_enterwith(x,y) (MPEnterCriticalRegion(x,y))
#define critical_exit(x) (MPExitCriticalRegion(x))
#define critical_free(x) (MPDeleteCriticalRegion(x))
#define thread_getcurrent(x) (MacGetCurrentThread(x))
#else
#define critical_new(x)
#define critical_enter(x)
#define critical_enterwith(x,y)
#define critical_exit(x)
#define critical_free(x)
#define thread_getcurrent(x) (*(x) = 0)
#endif

#endif // MAC_VERSION

#ifdef WIN_VERSION

#define CRITICAL_DEFAULT 0

typedef void *t_critical;
typedef long t_thread;

#define critical_new(x)
#define critical_enter(x)
#define critical_enterwith(x,y)
#define critical_exit(x)
#define critical_free(x)
#define thread_getcurrent(x) (*(x) = 0)

#endif // WIN_VERSION

#endif // _CRITICAL_H_
