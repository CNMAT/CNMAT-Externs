#ifndef _EXT_SYSTHREAD_H_
#define _EXT_SYSTHREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void *t_systhread;

// Generic x-platform thread API
long systhread_create(method entryproc, void *arg, long stacksize, long priority, long flags, t_systhread *thread);
long systhread_terminate(t_systhread thread);
void systhread_sleep(long milliseconds);
void systhread_exit(long status);
t_systhread systhread_self(void);
long systhread_join(t_systhread thread, unsigned int* retval);

// Useful thread functions for working within max environment
short systhread_ismainthread(void);
short systhread_istimerthread(void);

#ifdef __cplusplus
}
#endif

#endif // _EXT_SYSTHREAD_H_

