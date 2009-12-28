/** 	@file cmmjl_osc.h Utilites for handling OpenSoundControl data in Max. 
	@authors John MacCallum, Matt Wright, Andy Schmeder
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

Written by John MacCallum, Matt Wright and Andy Schmeder, The Center for New Music and 
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

#include "cmmjl_osc_pattern.h"
#include "cmmjl_osc_timetag.h"
#include "cmmjl_osc_schedule.h"
#include "ext.h"

#define CMMJL_GEN_INT 0x100
#define CMMJL_GEN_FLOAT 0x200
#define CMMJL_GEN_OTHER 0x300

typedef enum _cmmjl_osc_type{
	CMMJL_CHAR = 0x100,
	CMMJL_UCHAR,
	CMMJL_SHORT,
	CMMJL_USHORT,
	CMMJL_LONG,
	CMMJL_ULONG,
	CMMJL_LLONG,
	CMMJL_ULLONG,
	CMMJL_FLOAT = 0x200,
	CMMJL_DOUBLE,
	CMMJL_LDOUBLE,
	CMMJL_PTR = 0x300,
	CMMJL_TIMETAG
} t_cmmjl_osc_type;

union _cmmjl_osc_word{
	char w_char;
	unsigned char w_uchar;
	short w_short;
	unsigned short w_ushort;
	long w_long;
	unsigned long w_ulong;
	long long w_llong;
	unsigned long long w_ullong;
	float w_float;
	double w_double;
	long double w_ldouble;
	char *w_ptr;
	unsigned long long w_timetag;
};

typedef struct _cmmjl_osc_atom{
	union _cmmjl_osc_word a_w;
	t_cmmjl_osc_type a_type;
	int nbytes;
	int npaddingbytes;
	char osc_typetag;
} t_cmmjl_osc_atom;

typedef struct _cmmjl_osc_message{
	char *address;
	char *typetags;
	int argc;
	char *argv;
	int size;
} t_cmmjl_osc_message;

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
		class_addattr(c,attr_offset_new("OSCaddress",USESYM(symbol),0L,(method)0L,(method)0L,0L)); \
		CLASS_ATTR_ACCESSORS(c, "OSCaddress", cmmjl_osc_address_get, cmmjl_osc_address_set); \
		class_addmethod(c, (method)cmmjl_osc_setDefaultAddress, "reset_OSCaddress", 0); \
	}while(0)

/**	Initialize the OSC part of the lib.  This should be done in the object's
	new instance routine.

	@param	x	A pointer to your newly allocated object.
	@returns	An error code if one was encountered, or CMMJL_SUCCESS.
*/
t_cmmjl_error cmmjl_osc_init(void *x);

/**	Get the OSC address of this object.  This should only be called by the max system
	when the attribute is querried.  Use cmmjl_osc_getAddress() instead.
	@param	x	A pointer to the object.
	@param	attr	The attribute object.
	@param	argc	Arg count.
	@param	argv	The argument vector.

	@returns	An error code if one is encountered.
	@see		cmmjl_osc_getAddress()
*/
t_max_err cmmjl_osc_address_get(void *x, t_object *attr, long *argc, t_atom **argv);

/**	Set the OSC address of this object.  This should only be called by the max system
	when the attribute is set.  Use cmmjl_osc_setAddress() instead.
	@param	x	A pointer to the object.
	@param	attr	The attribute object.
	@param	argc	Arg count.
	@param	argv	The argument vector.

	@returns	An error code if one is encountered.
	@see		cmmjl_osc_setAddress()
*/
t_max_err cmmjl_osc_address_set(void *x, t_object *attr, long argc, t_atom *argv);

/**	Get the OSC address associated with an object.

	@param	x	The object
	@returns	The OSC address.
*/
t_symbol *cmmjl_osc_getAddress(void *x);

/**	Set the OSC address associated with an object.

	@param	x	The object
	@param address	The address to set.
*/
void cmmjl_osc_setAddress(void *x, t_symbol *address);

/**	Create and set a default OSC address which will be in the form
	/patcher/hierarchy/objectname/instancenumber

	@param	x	The object
*/
void cmmjl_osc_setDefaultAddress(void *x);

/**	Create a default OSC address in the form
	/patcher/hierarchy/objectname/instancenumber

	@param 	x		The object
	@param	instance	The instance number
	@param	buf		A buffer to hold the address.
	@see 	cmmjl_osc_setAddress 
*/
void cmmjl_osc_makeDefaultAddress(void *x, long instance, char *buf);

/**	Calls obj_attr_attr_setvalueof() to notify Max that he should save 
	the value of this attribute with the patcher.
	@param	x 	The object.
	@param	b	true/false
*/
void cmmjl_osc_saveAddressWithPatcher(void *x, bool b);

/**	This function strips an OSC message of everything but the last segment
	(if necessary) and attempts to send it to the object pointed to by x.

	@param 	x	The receiving object.
	@param	msg	The OSC message.
	@param	argc	The number of arguments.
	@param  argv	The arguments.
*/
void cmmjl_osc_sendMsg(void *x, t_symbol *msg, int argc, t_atom *argv);

int cmmjl_osc_init_bundle(int len, char *ptr, char *timetag);
int cmmjl_osc_get_msg_length(char *address, char *tt, int argc, char *argv);
int cmmjl_osc_get_msg_length_max(t_symbol *msg, short argc, t_atom *argv);
int cmmjl_osc_make_bundle_from_atoms(long argc, t_atom *argv, int *len, char *buffer);
int cmmjl_osc_make_bundle(int numAddresses,
			  t_symbol **addresses, 
			  int *numArgs,
			  char **typetags, 
			  t_atom **args, 
			  int *len, 
			  char *buffer);
int cmmjl_osc_check_pos_and_resize(char *buf, int len, char *pos);
int cmmjl_osc_add_to_bundle(int len, char *ptr, t_cmmjl_osc_message *msg);
int cmmjl_osc_make_message(char *address, int argc, t_cmmjl_osc_atom *argv, int len, char *ptr, t_cmmjl_osc_message *msg);
int cmmjl_osc_rename(char *buffer, 
		    int bufferLen, 
		    int bufferPos, 
		    t_cmmjl_osc_message *msg, 
		     char *newAddress);

/** 	Wrap a naked message in a bundle with an ``immediate'' timetag

	@param  n	The length of the message in bytes
	@param 	ptr	A pointer to the message
	@param 	out	A pointer where the new bundle will be put.  
			If NULL, memory will be allocated.

	@returns	The length of the new bundle or -1 if the ptr arg was already a bundle
*/
long cmmjl_osc_bundle_naked_message(long n, char *ptr, char *out);

/**	Flatten a (possibly) nested bundle.

	@param	n	The length of the message in bytes
	@param	ptr 	A pointer to the message
	@param 	out	A pointer to where the new bundle will be put.
			If NULL, memory will be allocated.

	@returns	The length of the new bundle or -1 if the ptr arg was already a bundle
 */
long cmmjl_osc_flatten(long n, char *ptr, char *out);

/**	Handle a FullPacket message.  This is the default function set if 
	CMMJL_ACCEPT_FULLPACKET() is called with NULL for the function arg.
	This function parses the packet and calls cmmjl_osc_sendMsg for
	each OSC message found

	@param	x	The object.
	@param	n	The length of the packet in bytes.
	@param	ptr	The address of the packet.
	@callgraph
 */

int cmmjl_osc_copy_max_messages(t_symbol *msg, short argc, t_atom *argv, int len, char *buf);
void cmmjl_osc_fullPacket(void *x, long n, long ptr);

/**	Call the OSC FullPacket parser
	@param	x	The object.
	@param	n	The length of the packet in bytes.
	@param	cbk	The callback that will be used when messages get parsed
	@param	ptr	The address of the packet.
*/
t_cmmjl_error cmmjl_osc_parseFullPacket(void *x, 
					long n, 
					long ptr, 
					void (*cbk)(void*, t_symbol*, int, t_atom*));

t_cmmjl_error cmmjl_osc_extract_messages(long n, 
					 char *buf,
					 bool topLevel,
					 void (*cbk)(t_cmmjl_osc_message msg, void *v), 
					 void *v);

/** 	Parse an OSC packet.  This function recursively parses the 
	packet which can be a bundle or even a nested bundle.  
	For each message (and the timetag), cbk is called.  

	@param 	x		A pointer to your object.  This will be passed
				to the callback as the first argument.
	@param	n		Length in bytes of the OSC data.
	@param	buf		Pointer to the OSC data to be parsed.
	@param	topLevel	Set this to "true" or 1 when calling the function.
	@param 	cbk		The callback to be used to return OSC messages.  
				If cbk is NULL, the messages will be printed to 
				the Max window.

	@returns		Any error code or 0 on success.
*/ 
t_cmmjl_error cmmjl_osc_parse(void *x, 
			      long n, 
			      char *buf,
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

/**	Returns the number of bytes needed to hold a blob:  The length of the 4-byte
	size count, plus the number of bytes in the blob, plus 0-3 null bytes needed
	for the 4-byte alignment.

	@param 	blobDataSize	The length of the blob data
	@returns		The number of bytes needed to hold the blob data.
*/
int cmmjl_osc_effectiveBlobLength(int blobDataSize);

int cmmjl_osc_get_tree_length(int len, char *msg);
void cmmjl_osc_address_to_array(int len, char *msg, char **ar, int *lengths);

void cmmjl_osc_atom2maxatom(t_cmmjl_osc_atom *a, t_atom *ma);
void cmmjl_osc_get_data_atoms(t_cmmjl_osc_message *msg, t_atom *atoms);
void cmmjl_osc_get_data(t_cmmjl_osc_message *msg, t_cmmjl_osc_atom *a);

int cmmjl_osc_atom_setchar(t_cmmjl_osc_atom *a, char *ptr);
char cmmjl_osc_atom_getchar(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setuchar(t_cmmjl_osc_atom *a, char *ptr);
unsigned char cmmjl_osc_atom_getuchar(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setshort(t_cmmjl_osc_atom *a, char *ptr);
short cmmjl_osc_atom_getshort(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setushort(t_cmmjl_osc_atom *a, char *ptr);
unsigned short cmmjl_osc_atom_getushort(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setlong(t_cmmjl_osc_atom *a, char *ptr);
long cmmjl_osc_atom_getlong(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setulong(t_cmmjl_osc_atom *a, char *ptr);
unsigned long cmmjl_osc_atom_getulong(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setllong(t_cmmjl_osc_atom *a, char *ptr);
long long cmmjl_osc_atom_getllong(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setullong(t_cmmjl_osc_atom *a, char *ptr);
unsigned long long cmmjl_osc_atom_getullong(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setfloat(t_cmmjl_osc_atom *a, char *ptr);
float cmmjl_osc_atom_getfloat(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setdouble(t_cmmjl_osc_atom *a, char *ptr);
double cmmjl_osc_atom_getdouble(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setldouble(t_cmmjl_osc_atom *a, char *ptr);
long double cmmjl_osc_atom_getldouble(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_setptr(t_cmmjl_osc_atom *a, char *ptr);
char *cmmjl_osc_atom_getptr(t_cmmjl_osc_atom *a);
void cmmjl_osc_atom_settypetag(t_cmmjl_osc_atom *a, char tt);
char cmmjl_osc_atom_gettypetag(t_cmmjl_osc_atom *a);
int cmmjl_osc_atom_tostring(t_cmmjl_osc_atom *a, char *buf);
#endif

/*@}*/
