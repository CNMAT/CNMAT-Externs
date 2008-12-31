/** 	@file cmmjl_osc.h Utilites for handling OpenSoundControl data in Max. 
	@defgroup OSC OpenSoundControl
@{
*/

#include "cmmjl_error.h"
#include "ext.h"

#ifndef __CMMJL_OSC_H__
#define __CMMJL_OSC_H__

/** The alignment padding for a string in an OSC packet.  
This is defined in the OSC spec and should never change */
#define CMMJL_OSC_STRING_ALIGN_PAD 4 

/** 	Adds support for the FullPacket OSC message to your object.  If fn is NULL,
	the default function will be used.

	@param 	ob	A pointer to your object.
	@param	fn	The function that will be called when the FullPacket message
			is received, or NULL to use the default cmmjl_osc_fullpacket().
*/
#define CMMJL_ACCEPT_FULLPACKET(ob, fn) class_addmethod(ob, (method)(fn ? fn : cmmjl_osc_fullPacket), \
							"FullPacket", A_LONG, A_LONG, 0);
/**	Handle a FullPacket message.  This is the default function set if 
	CMMJL_ACCEPT_FULLPACKET() is called with NULL for the function arg.
	This function simply parses the packet and posts the contents to 
	the Max window.

	@param	x	The object.
	@param	n	The length of the packet in bytes.
	@param	ptr	The address of the packet.
 */
void cmmjl_osc_fullPacket(void *x, long n, long ptr);

/** 	Parse an OSC packet.  This function recursively parses the 
	packet which can be a bundle or even a nested bundle.  
	For each message (and the timetag), cbk is called.  
	outlet_anything() (max-includes) can be passed to the 
	function as the callback in order to have the messages and 
	timetag output through an outlet, or a user-defined callback 
	can be used for further processing.

	@param 	x		Anything.  This will be passed as the first 
				argument to the callback.  If outlet_anything() 
				is used, x should be a pointer to an outlet, 
				otherwise it can be an object struct, or NULL.
	@param	buf		Pointer to the OSC data to be parsed.
	@param	n		Length in bytes of the OSC data.
	@param	topLevel	Set this to "true" or 1 when calling the function.
	@param 	cbk		The callback to be used to return OSC messages.

	@returns		Any error code or 0 on success.
*/ 
t_cmmjl_error cmmjl_osc_parseFullPacket(void *x, 
					char *buf, 
					long n, 
					bool topLevel,
					void(*cbk)(void *x, t_symbol *sym, int argv, t_atom *argc));

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
	@returns		A pointer to the next byte after the null byte(s) or zero on failure.
*/
char *cmmjl_osc_dataAfterAlignedString(char *string, char *boundary);

/** 	Checks to see if the string is really a null terminated, 
	4-byte aligned isprint()able string.

	@param	string		The null-padded string to be searched.
	@param	boundary	The boundary argument points to the character 
				after the last valid character in the buffer.  
				If the string hasn't ended by there, the function 
				calls cmmjl_error() and returns 0.

	@returns		true or false (1 or 0).
*/
Boolean cmmjl_osc_isNiceString(char *string, char *boundary);

/** 	Tests if the string is the final segment of an OSC path.  "/foo" will return true
	while "/foo/bar" will return false.

	@param	path	The path to test.
	@returns 	True or false
*/
bool cmmjl_osc_isFinalPathSegment(char *path);
#endif

/*@}*/
