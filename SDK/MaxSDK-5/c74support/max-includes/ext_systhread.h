#ifndef _EXT_SYSTHREAD_H_
#define _EXT_SYSTHREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

/** An opaque thread instance pointer. 
	@ingroup threading
*/
typedef void *t_systhread;


/** An opaque mutex handle. 
	@ingroup threading
*/
typedef void *t_systhread_mutex;


/** An opaque cond handle. 
	@ingroup threading
*/
typedef void *t_systhread_cond;


/** systhread_mutex_new() flags
	@ingroup threading
*/
typedef enum {
	SYSTHREAD_MUTEX_NORMAL =		0x00000000,	///< Normal
	SYSTHREAD_MUTEX_ERRORCHECK =	0x00000001,	///< Error-checking
	SYSTHREAD_MUTEX_RECURSIVE =		0x00000002	///< Recursive
} e_max_systhread_mutex_flags;


/**
 	Create a new thread.
	@ingroup threading
	
	@param	entryproc	A method to call in the new thread when the thread is created.
	@param	arg			An argument to pass to the method specified for entryproc.  
						Typically this might be a pointer to your object's struct.
	@param	stacksize	Not used.  Pass 0 for this argument.
	@param	priority	Not used.  Pass 0 for this argument.
	@param	flags		Not used.  Pass 0 for this argument.
	@param	thread		The address of a #t_systhread where this thread's instance pointer will be stored.
	@return				A Max error code as defined in #e_max_errorcodes.
*/
long systhread_create(method entryproc, void *arg, long stacksize, long priority, long flags, t_systhread *thread);


/**
 	Forcefully kill a thread -- not recommended.
	@ingroup threading
	
	@param	thread	The thread to kill.
	@return			A Max error code as defined in #e_max_errorcodes.
*/
long systhread_terminate(t_systhread thread);


/**
 	Suspend the execution of the calling thread.
	@ingroup threading
	
	@param	milliseconds	The number of milliseconds to suspend the execution of the calling thread.
							The actual amount of time may be longer depending on various factors.
*/
void systhread_sleep(long milliseconds);


/**
 	Exit the calling thread.
	Call this from within a thread made using systhread_create() when the thread is no longer needed.

	@ingroup threading
	@param	status		You will typically pass 0 for status.  
						This value will be accessible by systhread_join(), if needed.
*/
void systhread_exit(long status);


/**
	Wait for thread to quit and get return value from systhread_exit().
	
	@ingroup threading
	@param	thread		The thread to join.
	@param	retval		The address of a long to hold the return value (status) from systhread_exit().
	@return				A Max error code as defined in #e_max_errorcodes.
		
	@remark	If your object is freed, and your thread function accesses memory from your object,
			then you will obviously have a memory violation.
			A common use of systhread_join() is to prevent this situation by waiting (in your free method)
			for the thread to exit.
*/
long systhread_join(t_systhread thread, unsigned int* retval);	// 


/**
	Return the thread instance pointer for the calling thread.
	@ingroup	threading
	@return		The thread instance pointer for the thread from which this function is called.
*/
t_systhread systhread_self(void);


char *systhread_getstackbase(void);


// private
void systhread_init(void);
void systhread_mainstacksetup(void);
void systhread_timerstacksetup(void);
short systhread_stackcheck(void);


/** Check to see if the function currently being executed is in the main thread.
	@ingroup	threading
	@return		Returns true if the function is being executed in the main thread, otherwise false.
*/
short systhread_ismainthread(void);


/** Check to see if the function currently being executed is in the scheduler thread.
	@ingroup	threading
	@return		Returns true if the function is being executed in the main thread, otherwise false.
*/
short systhread_istimerthread(void);




/**
	Create a new mutex, which can be used to place thread locks around critical code.
	The mutex should be freed with systhread_mutex_free().
	@ingroup mutex

	@param	pmutex	The address of a variable to store the mutex pointer.
	@param	flags	Flags to determine the behaviour of the mutex, as defined in #e_max_systhread_mutex_flags.
	@return			A Max error code as defined in #e_max_errorcodes.
	
	@remark			One reason to use systhread_mutex_new() instead of @ref critical is to 
					create non-recursive locks, which are lighter-weight than recursive locks.
*/
long systhread_mutex_new(t_systhread_mutex *pmutex,long flags);


/**
	Free a mutex created with systhread_mutex_new().
	@ingroup mutex
	@param	pmutex	The mutex instance pointer.
	@return			A Max error code as defined in #e_max_errorcodes.
*/
long systhread_mutex_free(t_systhread_mutex pmutex);


/**
	Enter block of locked code code until a systhread_mutex_unlock() is reached.
	It is important to keep the code in this block as small as possible.
	@ingroup mutex
	@param	pmutex	The mutex instance pointer.
	@return			A Max error code as defined in #e_max_errorcodes.
	@see			systhread_mutex_trylock()
*/
long systhread_mutex_lock(t_systhread_mutex pmutex);


/**
	Exit a block of code locked with systhread_mutex_lock().
	@ingroup mutex
	@param	pmutex	The mutex instance pointer.
	@return			A Max error code as defined in #e_max_errorcodes.
*/
long systhread_mutex_unlock(t_systhread_mutex pmutex);


/**
	Try to enter block of locked code code until a systhread_mutex_unlock() is reached.
	If the lock cannot be entered, this function will return non-zero.

	@ingroup mutex
	@param	pmutex	The mutex instance pointer.
	@return			Returns non-zero if there was a problem entering.
	@see			systhread_mutex_lock()
*/
long systhread_mutex_trylock(t_systhread_mutex pmutex);


/**
	Convenience utility that combines systhread_mutex_new() and systhread_mutex_lock().
	@ingroup mutex
	@param	pmutex	The address of a variable to store the mutex pointer.
	@param	flags	Flags to determine the behaviour of the mutex, as defined in #e_max_systhread_mutex_flags.
	@return			A Max error code as defined in #e_max_errorcodes.
*/
long systhread_mutex_newlock(t_systhread_mutex *pmutex,long flags);



long systhread_cond_new(t_systhread_cond *pcond, long flags);
long systhread_cond_free(t_systhread_cond pcond);
long systhread_cond_wait(t_systhread_cond pcond, t_systhread_mutex pmutex);
long systhread_cond_signal(t_systhread_cond pcond);
long systhread_cond_broadcast(t_systhread_cond pcond);


#ifdef __cplusplus
}
#endif

#endif // _EXT_SYSTHREAD_H_

