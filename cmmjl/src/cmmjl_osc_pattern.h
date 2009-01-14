/**	@file 	cmmjl_osc_pattern.h
	Pattern matching utilities for OSC addresses
	@authors John MacCallum, The Center for New Music and Audio Technologies, University of California, Berkeley.
	@addtogroup OSC
@{
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

Written by John MacCallum and Andy Schmeder, The Center for New Music and 
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

#ifndef __CMMJL_OSC_PATTERN_H__
#define __CMMJL_OSC_PATTERN_H__

#include "regex.h"

/**	Convert an OSC address to a regular expression.  This is done by 
	converting the following OSC expressions to their regex counterparts:\n\n
	\t '*' => [^/]* \n
	\t '?' => [^/] \n
	\t {1,2,3} => (1|2|3) \n
	\t [3-48] => [3-48] (no change) \n

	@param 	osc_string	The OSC address you would like to convert.
	@param	re		A pointer to a regex_t data structure where the
				compiled regex will be stored.

	@returns		0 on success or an error (see regex.h)
*/
int cmmjl_osc2regex(char *osc_string, regex_t *re);

/**	Tests st2 against the OSC pattern in st1.
	
	@param	x	Your object.
	@param	st1	The string containing the OSC pattern.  This will be 
			converted into a regex by calling cmmjl_osc2regex().
	@param	st2	The OSC address to try to match.

	@returns	0 if st2 matches the pattern in st1, or an error code (see regex.h)
*/
int cmmjl_osc_match(void *x, char *st1, char *st2);

/**	Tests st against the regular expression re.
	
	@param	re	The regex.
	@param	st	The OSC address to test.

	@returns	0 if st2 matches the pattern in st1, or an error code (see regex.h)
*/
int cmmjl_osc_match_re(regex_t *re, char *st);

#endif // __CMMJL_OSC_PATTERN_H__

/*@}*/
