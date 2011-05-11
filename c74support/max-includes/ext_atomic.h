// ext_atomic.h copyright 2008 cycling '74

#ifndef __EXT_ATOMIC_H__
#define __EXT_ATOMIC_H__

#ifdef MAC_VERSION

#include <libkern/OSAtomic.h>
typedef volatile int32_t t_int32_atomic;

#define ATOMIC_INCREMENT(pv) OSAtomicIncrement32((int32_t *)pv)
#define ATOMIC_INCREMENT_BARRIER(pv) OSAtomicIncrement32Barrier((int32_t *)pv)

#define ATOMIC_DECREMENT(pv) OSAtomicDecrement32((int32_t *)pv)
#define ATOMIC_DECREMENT_BARRIER(pv) OSAtomicDecrement32Barrier((int32_t *)pv)

#else // WIN_VERSION

#include <intrin.h>
typedef volatile long t_int32_atomic;

#pragma intrinsic (_InterlockedIncrement)
#pragma intrinsic (_InterlockedDecrement)

/**	Use this routine for incrementing a global counter using a threadsafe and multiprocessor safe method.
	@ingroup	threading
	@param	pv	pointer to the (int) counter.
*/

// on windows I don't think there are non-barrier atomic increment / decrement functions
// perhaps could be done with inline assembly?

#define ATOMIC_INCREMENT(pv)			  (_InterlockedIncrement(pv))
#define ATOMIC_INCREMENT_BARRIER(pv)      (_InterlockedIncrement(pv))


/**	Use this routine for decrementing a global counter using a threadsafe and multiprocessor safe method.
	@ingroup	threading
	@param	pv	pointer to the (int) counter.
*/
#define ATOMIC_DECREMENT(pv)			  (_InterlockedDecrement(pv))
#define ATOMIC_DECREMENT_BARRIER(pv)      (_InterlockedDecrement(pv))

#endif // WIN_VERSION


#endif // __EXT_ATOMIC_H__
