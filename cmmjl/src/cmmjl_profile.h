/** @file cmmjl_profile.h
	Some convenient profiling macros and functions
	@author John MacCallum
*/
/*
Copyright (c) 2008.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by John MacCallum, The Center for New Music and
Audio Technologies, University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.
 */


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
