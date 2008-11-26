// ext_atomic.h copyright 2008 cycling '74

#ifndef __EXT_ATOMIC_H__
#define __EXT_ATOMIC_H__

#ifdef MAC_VERSION

#include <libkern/OSAtomic.h>

#define ATOMIC_INCREMENT(pv) OSAtomicIncrement32(pv)
#define ATOMIC_DECREMENT(pv) OSAtomicDecrement32(pv)

#else // WIN_VERSION

#include <intrin.h>

#pragma intrinsic (_InterlockedIncrement)
#pragma intrinsic (_InterlockedDecrement)

/**	Use this routine for incrementing a global counter using a threadsafe and multiprocessor safe method.
	@ingroup	threading
	@param	pv	pointer to the (int) counter.
*/
#define ATOMIC_INCREMENT(pv)      (_InterlockedIncrement(pv))


/**	Use this routine for decrementing a global counter using a threadsafe and multiprocessor safe method.
	@ingroup	threading
	@param	pv	pointer to the (int) counter.
*/
#define ATOMIC_DECREMENT(pv)      (_InterlockedDecrement(pv))

#endif // WIN_VERSION


#endif // __EXT_ATOMIC_H__
