/** 	@file cmmjl_osc.h Utilites for handling OpenSoundControl data in Max. 
	@addtogroup 	OSC 
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

#ifndef __CMMJL_OSC_H__
#define __CMMJL_OSC_H__

#include "cmmjl_error.h"
#include "OSC-client.h"
#include "ext.h"

/** The alignment padding for a string in an OSC packet.  
This is defined in the OSC spec and should never change */
#define CMMJL_OSC_STRING_ALIGN_PAD 4 

/** 	Adds support for the FullPacket OSC message to your object.  If fn is NULL,
	the default function cmmjl_osc_fullPacket() will be used with 
	cmmjl_osc_sendMsg() as the callback.

	@param 	c		A pointer to your object's class allocated with class_new().
	@param	structname	Your class's type.
*/
#define CMMJL_ACCEPT_FULLPACKET(c, structname)				\
	do{							\
		class_addmethod(c, (method)cmmjl_osc_fullPacket, "FullPacket", A_LONG, A_LONG, 0); \
		class_addattr(c,attr_offset_new("OSC-address",USESYM(symbol),0L,(method)0L,(method)0L,0L)); \
		CLASS_ATTR_ACCESSORS(c, "OSC-address", cmmjl_osc_address_get, cmmjl_osc_address_set); \
		CLASS_ATTR_SAVE(c, "OSC-address", 0L); \
	}while(0)

/**	Initialize the OSC part of the lib.  This should be done in the object's
	new instance routine.

	@param	x	A pointer to your newly allocated object.
	@returns	An error code if one was encountered, or CMMJL_SUCCESS.
*/
t_cmmjl_error cmmjl_osc_init(void *x);

/**	Get the OSC address of this object
	@param	x	A pointer to the object.
	@param	attr	The attribute object.
	@param	argc	Arg count.
	@param	argv	The argument vector.

	@returns	An error code if one is encountered.
*/
t_max_err cmmjl_osc_address_get(void *x, t_object *attr, long *argc, t_atom **argv);

/**	Set the OSC address of this object
	@param	x	A pointer to the object.
	@param	attr	The attribute object.
	@param	argc	Arg count.
	@param	argv	The argument vector.

	@returns	An error code if one is encountered.
*/
t_max_err cmmjl_osc_address_set(void *x, t_object *attr, long argc, t_atom *argv);

/**	Convenience function for getting the OSC address of an object.  Simply
	calls cmmjl_osc_address_get() and returns the result after unpacking 
	the t_atom.

	@param	x	The object
	@returns	The OSC address.
*/
char *cmmjl_osc_getAddress(void *x);

/**	This function strips an OSC message of everything but the last segment
	(if necessary) and attempts to send it to the object pointed to by x.

	@param 	x	The receiving object.
	@param	msg	The OSC message.
	@param	argc	The number of arguments.
	@param  argv	The arguments.
*/
void cmmjl_osc_sendMsg(void *x, t_symbol *msg, int argc, t_atom *argv);

/**	Handle a FullPacket message.  This is the default function set if 
	CMMJL_ACCEPT_FULLPACKET() is called with NULL for the function arg.
	This function parses the packet and calls cmmjl_osc_sendMsg for
	each OSC message found

	@param	x	The object.
	@param	n	The length of the packet in bytes.
	@param	ptr	The address of the packet.
 */
void cmmjl_osc_fullPacket(void *x, long n, long ptr);

/** 	Parse an OSC packet.  This function recursively parses the 
	packet which can be a bundle or even a nested bundle.  
	For each message (and the timetag), cbk is called.  

	@param 	x		A pointer to your object.  This will be passed
				to the callback as the first argument.
	@param	buf		Pointer to the OSC data to be parsed.
	@param	n		Length in bytes of the OSC data.
	@param	topLevel	Set this to "true" or 1 when calling the function.
	@param 	cbk		The callback to be used to return OSC messages.  
				If cbk is NULL, the messages will be printed to 
				the Max window.

	@returns		Any error code or 0 on success.
*/ 
t_cmmjl_error cmmjl_osc_parseFullPacket(void *x, 
					char *buf, 
					long n, 
					bool topLevel,
					void (*cbk)(void *x, t_symbol *sym, int argv, t_atom *argc));

/** 	Format an OSC message as a Max message.  This function is 
	called by cmmjl_osc_parseFullPacket().

	@param 	x		Anything.  This will be passed as the first 
				argument to the callback.  If outlet_anything() 
				is used, x should be a pointer to an outlet, 
				otherwise it can be an object struct, or NULL.
	@param	address		The OSC message address.
	@param	v		The data as a void* pointer.
	@param  n		The length of the data in bytes.
	@param 	cbk		The callback to be used to return OSC messages.
	@returns		Any error code or 0 on success.
*/
t_cmmjl_error cmmjl_osc_formatMessage(void *x, 
				      char *address, 
				      void *v, 
				      long n, 
				      void (*cbk)(void *x, t_symbol *msg, int argc, t_atom *argv));

/** 	Returns a pointer (or 0 if the function fails) to the next 
	byte after a null padded string.  The string must be padded 
	to a multiple of CMMJL_OSC_STRING_ALIGN_PAD or the function will fail.  

	@param	string		The null-padded string to be searched.
	@param	boundary	The boundary argument points to the 
				character after the last valid character 
				in the buffer.  If the string hasn't ended 
				by there, the function calls cmmjl_error() and returns 0.
	@param	result		The address of a pointer which will point to the first byte
				after the end of the null padding

	@returns		A pointer to the next byte after the null byte(s) or zero on failure.
*/
t_cmmjl_error cmmjl_osc_dataAfterAlignedString(char *string, char *boundary, char **result);

/** 	Checks to see if the string is really a null terminated, 
	4-byte aligned isprint()able string.

	@param	string		The null-padded string to be searched.
	@param	boundary	The boundary argument points to the character 
				after the last valid character in the buffer.  
				If the string hasn't ended by there, the function 
				calls cmmjl_error() and returns 0.

	@returns		An error code if the string looks bad, or CMMJL_SUCCESS.
*/
t_cmmjl_error cmmjl_osc_isNiceString(char *string, char *boundary);

/** 	Tests if the string is the final segment of an OSC path.  "/foo" will return true
	while "/foo/bar" will return false.

	@param	path	The path to test.
	@returns 	CMMJL_SUCCESS on success, or an error code
*/
bool cmmjl_osc_isFinalPathSegment(char *path);
#endif

/*@}*/
