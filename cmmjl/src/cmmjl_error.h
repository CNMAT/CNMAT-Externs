/** @file cmmjl_error.h
	Error handling macros, functions, and codes.
	@authors John MacCallum and Andy Schmeder, The Center for New Music and Audio Technologies, University of California, Berkeley.

Copyright (c) 2008.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

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

#ifndef __CMMJL_ERROR_H__
#define __CMMJL_ERROR_H__

#include "ext.h"

#ifndef PINFO
#define PINFO(str, args...) post(NAME ": " str, ##args)
#endif

#ifndef PERROR
#define PERROR(str, args...) error(NAME ": ERROR at %s:%d: " str, __FILE__, __LINE__, ##args)
#endif

#ifdef DEBUG
#ifndef PDEBUG
#define PDEBUG(str, args...) post(NAME ": " str, ##args)
#endif
#else
#ifndef PDEBUG
#define PDEBUG(str, args...)
#endif
#endif

#ifndef CMMJL_PINFO
#define CMMJL_PINFO(str, args...) post("cmmjl: " str, ##args)
#endif

#ifndef CMMJL_PERROR
#define CMMJL_PERROR(str, args...) error("cmmjl: ERROR at %s:%d: " str, __FILE__, __LINE__, ##args)
#endif

#ifdef CMMJL_DEBUG
#ifndef CMMJL_PDEBUG
#define CMMJL_PDEBUG(str, args...) post("cmmjl: " str, ##args)
#endif
#else
#ifndef CMMJL_PDEBUG
#define CMMJL_PDEBUG(str, args...)
#endif
#endif

enum{
	CMMJL_SUCCESS = 0,
	CMMJL_END,

	CMMJL_OSC_SUCCESS = 0x100,
	CMMJL_OSC_PACKET_NOT_MULTIPLE_OF_4BYTES = 0x101,
	CMMJL_OSC_BAD_PACKET_SIZE = 0x102,
	CMMJL_OSC_BAD_BUFFER_SIZE = 0x103,
	CMMJL_OSC_NULL_BUFFER = 0x104,
	CMMJL_OSC_NO_TIME_TAG = 0x105,
	CMMJL_OSC_BAD_SIZE_COUNT_IN_BUNDLE = 0x106,
	CMMJL_OSC_ERROR_IN_ENCAPSULATED_MESSAGE = 0x107,
	CMMJL_OSC_FAILED_TO_PROCESS_ENTIRE_PACKET = 0x108,
	CMMJL_OSC_BAD_MESSAGE_NAME_STRING = 0x109,
	CMMJL_OSC_END,

	CMMJL_SDIF_SUCCESS = 0x10000,
	CMMJL_SDIF_END
};

typedef int t_cmmjl_error;

char *cmmjl_error_getString(t_cmmjl_error e);

#endif
