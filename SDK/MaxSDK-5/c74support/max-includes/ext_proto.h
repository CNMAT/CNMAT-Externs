/* ext_proto.h -- prototypes for MAX external methods */
/* copyright 1996 Opcode/IRCAM */
#ifndef _EXT_PROTO_H_
#define _EXT_PROTO_H_

#include "ext_types.h"
#include "ext_maxtypes.h"	// contains box, patcher, wind, atombuf
#include "ext_sysmem.h"
#include "ext_sysfile.h"
#include "ext_systime.h"
#include "ext_expr.h"
#include "ext_path.h"
#include "ext_qtimage.h"
#include "ext_wind.h"
#ifdef WIN_VERSION
#include "ext_proto_win.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


// object/class functions

/**	Use the setup() function to initialize your class by informing Max of its size, 
	the name of your functions that create and destroy instances, 
	and the types of arguments passed to the instance creation function.
	
	@ingroup class_old
			
	@param	ident		A global variable in your code that points to the initialized class.
	@param	makefun		Your instance creation function. 
	@param	freefun		Your instance free function (see Chapter 7). 
	@param	size		The size of your objects data structure in bytes. 
						Usually you use the C sizeof operator here. 
	@param	menufun		No longer used.  You should pass NULL for this parameter.
	@param	type		The first of a list of arguments passed to makefun when an object is created.
	@param	...			Any additional arguments passed to makefun when an object is created.
						Together with the type parameter, this creates a standard Max type list 
						as enumerated in #e_max_atomtypes.
						The final argument of the type list should be a 0.
	@see @ref chapter_anatomy
*/
void setup(t_messlist **ident, method makefun, method freefun, short size, method menufun, short type, ...);


/**	Use addmess() to bind a function to a message other than the standard ones 
	covered by addbang(), addint(), etc.
	
	@ingroup class_old
	@param	f 		Function you want to be the method. 
	@param	s 		C string defining the message. 
	@param	type	The first of one or more integers from #e_max_atomtypes specifying the arguments to the message.
	@param	...		Any additional types from #e_max_atomtypes for additonal arguments.
	@see @ref chapter_anatomy
*/
void addmess(method f, char *s, short type, ...);


/**
	Used to bind a function to the common triggering message bang. 

	@ingroup class_old
	@param	f	Function to be the bang method. 
*/
void addbang(method f);


/**
	Use addint() to bind a function to the int message received in the leftmost inlet.
	@ingroup class_old
	@param	f Function to be the int method.
*/
void addint(method f);


/**
	Use addfloat() to bind a function to the float message received in the leftmost inlet.
	@ingroup class_old
	@param	f Function to be the int method.
*/
void addfloat(method f);


/**
	Use addinx() to bind a function to a int message that will be received in 
		an inlet other than the leftmost one. 

	@ingroup class_old
	@param	f	Function to be the int method. 
 	@param	n	Number of the inlet connected to this method. 
				1 is the first inlet to the right of the left inlet. 

	@remark	This correspondence between inlet locations and messages is not 
			automatic, but it is strongly suggested that you follow existing practice. 
			You must set the correspondence up when creating an object of your 
			class with proper use of intin and floatin in your instance creation 
			function @ref chapter_anatomy_object_new. 
*/
void addinx(method f, short n);


/**
	Use addftx() to bind a function to a float message that will be received in 
		an inlet other than the leftmost one. 

	@ingroup class_old
	@param	f	Function to be the float method. 
	@param	n	Number of the inlet connected to this method. 
				1 is the first inlet to the right of the left inlet. 

	@remark	This correspondence between inlet locations and messages is not 
			automatic, but it is strongly suggested that you follow existing practice. 
			You must set the correspondence up when creating an object of your 
			class with proper use of intin and floatin in your instance creation 
			function @ref chapter_anatomy_object_new. 
*/
void addftx(method f, short n);


/**
	Use newobject to allocate the space for an instance of your class and 
	initialize its object header. 

	@ingroup class_old
	@param	maxclass	The global class variable initialized in your main routine by the setup function. 
	@return 			A pointer to the new instance.
	
	@remark				You call newobject() when creating an instance of your class in your 
						creation function. newobject allocates the proper amount of memory 
						for an object of your class and installs a pointer to your class in the 
						object, so that it can respond with your class’s methods if it receives a 
						message.
*/
void *newobject(void *maxclass);


/**
	Release the memory used by a Max object.
	freeobject() calls an object’s free function, if any, then disposes the 
	memory used by the object itself. freeobject() should be used on any 
	instance of a standard Max object data structure, with the exception of 
	Qelems and Atombufs. Clocks, Binbufs, Proxies, Exprs, etc. should be freed with freeobject().
	
	@ingroup	class_old
	@param		op	The object instance pointer to free.
	
	@remark		This function can be replaced by the use of object_free().
				Unlike freeobject(), object_free() checkes to make sure the pointer is 
				not NULL before trying to free it.
	
	@see newobject()
	@see object_free()
*/
void freeobject(t_object *op);


/** Make a new instance of an existing Max class.
	@ingroup class_old
	
	@param s	className Symbol specifying the name of the class of the instance to be created. 
	@param argc	Count of arguments in argv. 
	@param argv	Array of t_atoms; arguments to the class’s instance creation function. 

	@return 	A pointer to the created object, or 0 if the class 
				didn’t exist or there was another type of error in creating the instance. 
	
	@remark This function creates a new instance of the specified class. Using 
	newinstance is equivalent to typing something in a New Object box 
	when using Max. The difference is that no object box is created in any 
	Patcher window, and you can send messages to the object directly 
	without connecting any patch cords. The messages can either be type- 
	checked (using typedmess) or non-type-checked (using the members 
	of the getfn family).
	
	This function is useful for taking advantage of other already-defined 
	objects that you would like to use “privately” in your object, such as 
	tables. See the source code for the coll object for an example of using a 
	privately defined class. 
*/
void *newinstance(t_symbol *s, short argc, t_atom *argv);


/**
	@ingroup class_old
*/
void finder_addclass(char *category, char *classString);


/**
	Use the alias function to allow users to refer to your object by a 
	name other than that of your shared library.

	@ingroup class_old
	@param	name	An alternative name for the user to use to make an object of your class. 
*/
void alias(char *name);


/**
	@ingroup class_old
*/
void class_noinlet(t_messlist *m);


/**
	Use class_setname() to associate you object’s name with it’s filename 
	on disk. 

	@ingroup class_old
	@param	obname		A character string with the name of your object class as it appears in Max. 
	@param	filename	A character string with the name of your external’s file as it appears on disk. 
*/
void class_setname(char *obname, char *filename);


short force_install(char *classname);
void loader_setpath(long type, short path);



// memory functions

/**
	Allocate small amounts of non-relocatable memory. 
	As of Max 5 it is unified with sysmem_newptr(), which is the preferred method for allocating memory.
	@ingroup memory
	@param	size	The size to allocate in bytes (up to 32767 bytes).
	@return			A pointer to the allocated memory.
*/
char *getbytes(short size);


/**
	Free memory allocated with getbytes().
	As of Max 5 it is unified with sysmem_newptr(), which is the preferred method for allocating memory.
	@ingroup memory
	@param	b		A pointer to the block of memory previously allocated that you want to free.
	@param	size	The size the block specified (as parameter b) in bytes.
*/
void freebytes(void *b, short size);


/**
	Use getbytes16() to allocate small amounts of non-relocatable 
	memory that is aligned on a 16-byte boundary for use with vector optimization.
	@ingroup memory
	@param	size	The size to allocate in bytes (up to 32767 bytes).
	@return			A pointer to the allocated memory.
	
	@remark		getbytes16() is identical to getbytes except that it returns memory 
				that is aligned to a 16-byte boundary. This allows you to allocate 
				storage for vector-optimized memory at interrupt level. Note that any 
				memory allocated with getbytes16() must be freed with 
				freebytes16(), not freebytes().
*/
char *getbytes16(short size);


/**
	Free memory allocated with getbytes16().
	As of Max 5 it is unified with sysmem_newptr(), which is the preferred method for allocating memory.
	@ingroup memory
	@param	mem		A pointer to the block of memory previously allocated that you want to free.
	@param	size	The size the block specified (as parameter b) in bytes.
	
	@remark		Note that freebytes16() will cause memory corruption if you pass it 
				memory that was allocated with getbytes(). Use it only with memory 
				allocated with getbytes16().
*/
void freebytes16(char *mem, short size);


/**
	Allocate relocatable memory.
	
	@ingroup memory
	@param	size	The size to allocate in bytes.
	@return			The allocated handle.
	@see			sysmem_newhandle()
*/
char **newhandle(long size);


/**
	Change the size of a handle. 
	
	@ingroup memory
	@param	h		The handle to resize.
	@param	size	The new size to allocate in bytes.
	@return			Ignored.
	@see			sysmem_resizehandle()
*/
short growhandle(void *h, long size);


/**
	Free the memory used by a handle you no longer need.
	
	@ingroup memory
	@param	h		The handle to dispose.
	@see			sysmem_freehandle()
*/
void disposhandle(char **h);



#ifdef MM_UNIFIED // sysmem and getbytes are unified
#define getbytes(size) ((char *)sysmem_newptr((long)(size)))
#define freebytes(p,size) sysmem_freeptr((char *)(p))
#endif

// symbol/string/text/error functions

/**
	Given a C-string, fetch the matching #t_symbol pointer from the symbol table,
	generating the symbol if neccessary.

	@ingroup symbol
	@param	s	A C-string to be looked up in Max’s symbol table.
	@return		A pointer to the #t_symbol in the symbol table.
*/
t_symbol *gensym(char *s);


/**
	Print text to the Max window.
	Max 5 introduced object_post(), which provides several enhancements to post()
	where a valid #t_object pointer is available.
	
	post() is a printf() for the Max window. It even works from non-main threads, 
	queuing up multiple lines of text to be printed when the main thread processing resumes. 
	post() can be quite useful in debugging your external object.
	
	@ingroup console
	@param	fmt		A C-string containing text and printf-like codes 
					specifying the sizes and formatting of the additional arguments.
	@param	...		Arguments of any type that correspond to the format codes in fmtString.
	
	@remark		Note that post only passes 16 bytes of arguments to sprintf, so if 
				you want additional formatted items on a single line, use postatom().
				 
				Example: 
	@code
	short whatIsIt; 

	whatIsIt = 999; 
	post ("the variable is %ld",(long)whatIsIt); 
	@endcode
	
	@remark		The Max Window output when this code is executed. 
	@code
	the variable is 999
	@endcode
	
	@see object_post()
	@see error()
	@see cpost()
*/
void post(char *fmt, ...);


/**
	Print text to the system console.
	On the Mac this post will be visible by launching Console.app in the /Applications/Utilities folder.
	On Windows this post will be visible by launching the dbgView.exe program, which is a free download
	as a part of Microsoft's SysInternals.
	
	@ingroup console
	@param	fmt		A C-string containing text and printf-like codes 
					specifying the sizes and formatting of the additional arguments.
	@param	...		Arguments of any type that correspond to the format codes in fmtString.
	
	@remark			Particularly on MacOS 10.5, posting to Console.app can be a computationally expensive operation.
					Use with care.
					
	@see post()
	@see object_post()
*/
void cpost(char *fmt, ...);


/**
	Print an error to the Max window.
	Max 5 introduced object_error(), which provides several enhancements to error()
	where a valid #t_object pointer is available.
	
	error() is very similar to post(), thought it offers two additional features:
	- the post to the Max window is highlighted (with a red background).
	- the post can be trapped with the error object in a patcher.
		
	@ingroup console
	@param	fmt		A C-string containing text and printf-like codes 
					specifying the sizes and formatting of the additional arguments.
	@param	...		Arguments of any type that correspond to the format codes in fmtString.
		
	@see object_post()
	@see post()
	@see cpost()
*/
void error(char *fmt, ...);

/**
	Put up an error or advisory alert box on the screen.

	Don't use this function.  Instead use error(), object_error(), or object_error_obtrusive().
	
	This function performs an sprintf() on fmtstring and items, then 
	puts up an alert box. ouchstring() will queue the message to a lower 
	priority level if it’s called in an interrupt and there is no alert box 
	request already pending.
	
	@ingroup console
	@param	s		A C-string containing text and printf-like codes 
					specifying the sizes and formatting of the additional arguments.
	@param	...		Arguments of any type that correspond to the format codes in fmtString.
	
	@see	error()
	@see	object_error()
	@see	object_error_obtrusive()
*/
void ouchstring(char *s, ...);
short advise(char *s, ...);
void t_cpytext(void);
void drawstr(char *s);

/**
	Print multiple items in the same line of text in the Max window.
	This function prints a single #t_atom on a line in the Max window 
	without a carriage return afterwards, as post() does. Each #t_atom printed 
	is followed by a space character. 

	@ingroup console
	@param	ap		The address of a #t_atom to print.

	@see object_post()
	@see post()
	@see cpost()

*/
void postatom(t_atom *ap);

// tap -- this one doesn't do anything any more, right?  at least on windows it doesn't....
//void assist_string(short id,long msg,long arg, short firstin, short firstout,char *dst,...);

void stdinletinfo(t_object *s, void *b, long a, char *t);

void debug_printf(char *,...);


/**	Receive messages from the error handler.
	@ingroup misc
	@param	x	The object to be subscribed to the error handler.
	
	@remark		error_subscribe() enables your object to receive a message (error), 
				followed by the list of atoms in the error message posted to the Max 
				window. 
	
				Prior to calling error_subscribe(), you should bind the error 
				message to an internal error handling routine: 
	@code
	addmess((method)myobject_error, "error", A_GIMME, 0);
	@endcode
				Your error handling routine should be declared as follows:
	@code
	void myobject_error(t_myobject *x, t_symbol *s, short argc, t_atom *argv);
	@endcode
*/
void error_subscribe(t_object *x);


/** Remove an object as an error message recipient.
	@ingroup misc
	@param	x	The object to unsubscribe. 
*/
void error_unsubscribe(t_object *x);


void xsetpost();
void post_getpos(short *row, short *col);
void poststring(char *s);
// not sure where to put these...

enum {
	POSTROW_POST = 0,
	POSTROW_ERROR = 1,
	POSTROW_WARNING = 2,
	POSTROW_BUG = 3
};

enum {
	JMAXWINDOW_ADVANCE = 1,
	JMAXWINDOW_APPEND = 2,
	JMAXWINDOW_WRITE = 4,
	JMAXWINDOW_UNIQUE = 8
};


/**
	Print text to the Max window, linked to an instance of your object.

	Max window rows which are generated using object_post() or object_error() can be
	double-clicked by the user to have Max assist with locating the object in a patcher.
	Rows created with object_post() and object_error() will also automatically provide
	the name of the object's class in the correct column in the Max window.
		
	@ingroup console
	@param	x		A pointer to your object.
	@param	s		A C-string containing text and printf-like codes 
					specifying the sizes and formatting of the additional arguments.
	@param	...		Arguments of any type that correspond to the format codes in fmtString.
	
	@remark			Example: 
	@code
	void myMethod(myObject *x, long someArgument)
	{
		object_post((t_object*)x, "This is my argument: %ld", someArgument);
	}
	@endcode
		
	@see object_error()
*/
void object_post(t_object *x, char *s, ...);


/**
	Print text to the Max window, linked to an instance of your object,
	and flagged as an error (highlighted with a red background).

	Max window rows which are generated using object_post() or object_error() can be
	double-clicked by the user to have Max assist with locating the object in a patcher.
	Rows created with object_post() and object_error() will also automatically provide
	the name of the object's class in the correct column in the Max window.
		
	@ingroup console
	@param	x		A pointer to your object.
	@param	s		A C-string containing text and printf-like codes 
					specifying the sizes and formatting of the additional arguments.
	@param	...		Arguments of any type that correspond to the format codes in fmtString.
		
	@see object_post()
	@see object_warn()
*/
void object_error(t_object *x, char *s, ...);


/**
	Print text to the Max window, linked to an instance of your object,
	and flagged as a warning (highlighted with a yellow background).

	Max window rows which are generated using object_post(), object_error(), or object_warn can be
	double-clicked by the user to have Max assist with locating the object in a patcher.
	Rows created with object_post(), object_error(), or object_warn() will also automatically provide
	the name of the object's class in the correct column in the Max window.
		
	@ingroup console
	@param	x		A pointer to your object.
	@param	s		A C-string containing text and printf-like codes 
					specifying the sizes and formatting of the additional arguments.
	@param	...		Arguments of any type that correspond to the format codes in fmtString.
			
	@see object_post()
	@see object_error()
*/
void object_warn(t_object *x, char *s, ...);

// ?
void object_bug(t_object *x, char *s, ...);

// private?
void object_poststring(t_object *ob, long kind, long flags, char *text);


/**
	Print text to the Max window, linked to an instance of your object,
	and flagged as an error (highlighted with a red background), 
	and grab the user's attention by displaying a banner in the patcher window.
	
	This function should be used exceedingly sparingly, with preference given to 
	object_error() when a problem occurs.
		
	@ingroup console
	@param	x		A pointer to your object.
	@param	s		A C-string containing text and printf-like codes 
					specifying the sizes and formatting of the additional arguments.
	@param	...		Arguments of any type that correspond to the format codes in fmtString.
			
	@see object_post()
	@see object_error()
*/
void object_error_obtrusive(t_object *x, char *s, ...);


long jdialog_showtext(char *prompt, char *deftext, long flags, char **text);


#ifndef WIN_VERSION
int sprintf(char *, const char *, ...);
int sscanf(const char *, const char *, ...);
#endif //WIN_VERSION


// inlet/outlet functions

/**
	Use inlet_new() to create an inlet that can receive a specific message or any message. 

	@ingroup inout
	@param	x	Your object. 
	@param	s	Character string of the message, or NULL to receive any message.
	@return		A pointer to the new inlet.

	@remark		inlet_new() ceates a general purpose inlet. 
				You can use it in circumstances where you would like special messages to be received in 
				inlets other than the leftmost one. 
				To create an inlet that receives a particular message, pass the message’s 
				character string. For example, to create an inlet that receives only bang 
				messages, do the following
	@code
	inlet_new (myObject,"bang"); 
	@endcode

	@remark		To create an inlet that can receive any message, pass NULL for msg
	@code
	inlet_new (myObject, NULL); 
	@endcode

	@remark		Proxies are an alternative method for general-purpose inlets that have 
				a number of advantages. If you create multiple inlets as shown above, 
				there would be no way to figure out which inlet received a message. See 
				the discussion in @ref chapter_inout_proxies. 
*/
void *inlet_new(void *x, char *s);

/**
	Use intin() to create an inlet typed to receive only integers. 

	@ingroup inout
	@param	x	Your object. 
	@param	n	Location of the inlet from 1 to 9. 1 is immediately to the right of the leftmost inlet. 
	@return		A pointer to the new inlet.
	
	@remark		intin creates integer inlets. 
				It takes a pointer to your newly created object and an integer n, from 1 to 9. 
				The number specifies the message 
				type you’ll get, so you can distinguish one inlet from another. For 
				example, an integer sent in inlet 1 will be of message type in1 and a 
				floating point number sent in inlet 4 will be of type ft4. You use 
				addinx() and addftx() to add methods to respond to these messages.
				
				The order you create additional inlets is important. If you want the 
				rightmost inlet to be the have the highest number in- or ft- message 
				(which is usually the case), you should create the highest number 
				message inlet first. 
*/
void *intin(void *x, short n);


/**
	Use floatin() to create an inlet typed to receive only floats. 

	@ingroup inout
	@param	x	Your object. 
	@param	n	Location of the inlet from 1 to 9. 1 is immediately to the right of the leftmost inlet. 
	@return		A pointer to the new inlet.
*/
void *floatin(void *x, short n);


/**
	Use outlet_new() to create an outlet that can send a specific non-standard message, or any message.

	@ingroup	inout
	@param	x	Your object.
	@param	s	A C-string specifying the message that will be sent out this outlet, 
				or NULL to indicate the outlet will be used to send various messages.
				The advantage of this kind of outlet’s flexibility is balanced by the fact that 
				Max must perform a message-lookup in real-time for every message sent through it, 
				rather than when a patch is being constructed, as is true for other types of outlets. 
				Patchers execute faster when outlets are typed, since the message 
				lookup can be done before the program executes.
	@return		A pointer to the new outlet.
*/
void *outlet_new(void *x, char *s);


/**
	Use bangout() to create an outlet that will always send the bang message. 

	@ingroup	inout
	@param	x	Your object.
	@return		A pointer to the new outlet.

	@remark		You can send a bang message out a general purpose outlet, but creating 
				an outlet using bangout() allows Max to type-check the connection a 
				user might make and refuse to connect the outlet to any object that 
				cannot receive a bang message. bangout() returns the created outlet. 
*/
void *bangout(void *x);


/**
	Use intout() to create an outlet that will always send the int message. 

	@ingroup	inout
	@param	x	Your object.
	@return		A pointer to the new outlet.

	@remark		You can send a bang message out a general purpose outlet, but creating 
				an outlet using bangout() allows Max to type-check the connection a 
				user might make and refuse to connect the outlet to any object that 
				cannot receive a bang message. bangout() returns the created outlet. 
*/
void *intout(void *x);


/**
	Use floatout() to create an outlet that will always send the float message. 

	@ingroup	inout
	@param	x	Your object.
	@return		A pointer to the new outlet.
*/
void *floatout(void *x);


/**
	Use listout() to create an outlet that will always send the list message. 
	@ingroup	inout
	@param	x	Your object.
	@return		A pointer to the new outlet.
*/
void *listout(void *x);


/**
	Use outlet_bang() to send a bang message out an outlet. 

	@ingroup inout
	@param	o	Outlet that will send the message.
	@return		Returns 0 if a stack overflow occurred, otherwise returns 1.
*/
void *outlet_bang(void *o);


/**
	Use outlet_int() to send a float message out an outlet. 

	@ingroup inout
	@param	o	Outlet that will send the message.
	@param	n	Integer value to send.
	@return		Returns 0 if a stack overflow occurred, otherwise returns 1.
*/
void *outlet_int(void *o, long n);


/**
	Use outlet_float() to send an int message out an outlet. 

	@ingroup inout
	@param	o	Outlet that will send the message.
	@param	f	Float value to send.
	@return		Returns 0 if a stack overflow occurred, otherwise returns 1.
*/
void *outlet_float(void *o, double f);


/**
	Use outlet_list() to send a list message out an outlet. 

	@ingroup inout
	@param	o		Outlet that will send the message.
	@param	s		Should be NULL, but can be the _sym_list.
	@param	ac		Number of elements in the list in argv. 
	@param	av		Atoms constituting the list. 
	@return			Returns 0 if a stack overflow occurred, otherwise returns 1.
	
	@remark			outlet_list() sends the list specified by argv and argc out the 
					specified outlet. The outlet must have been created with listout or 
					outlet_new in your object creation function (see above). You create 
					the list as an array of Atoms, but the first item in the list must be an 
					integer or float.
					
					Here’s an example of sending a list of three numbers. 
	@code
	t_atom myList[3]; 
	long theNumbers[3]; 
	short i; 
	
	theNumbers[0] = 23; 
	theNumbers[1] = 12; 
	theNumbers[2] = 5;
	for (i=0; i < 3; i++) { 
		atom_setlong(myList+i,theNumbers[i]);
	} 
	outlet_list(myOutlet,0L,3,&myList); 
	@endcode

	@remark			It’s not a good idea to pass large lists to outlet_list that are 
					comprised of local (automatic) variables. If the list is small, as in the 
					above example, there’s no problem. If your object will regularly send 
					lists, it might make sense to keep an array of t_atoms inside your 
					object’s data structure.	
*/
void *outlet_list(void *o, t_symbol *s, short ac, t_atom *av);


/**
	Use outlet_anything() to send any message out an outlet. 

	@ingroup inout
	@param	o		Outlet that will send the message.
	@param	s		The message selector #t_symbol*. 
	@param	ac		Number of elements in the list in argv. 
	@param	av		Atoms constituting the list. 
	@return			Returns 0 if a stack overflow occurred, otherwise returns 1.
	
	@remark			This function lets you send an arbitrary message out an outlet. 
					Here are a couple of examples of its use.
					
					First, here’s a hard way to send the bang message (see outlet_bang() for an easier way):
	@code
	outlet_anything(myOutlet, gensym("bang"), 0, NIL); 
	@endcode
	
	@remark			And here’s an even harder way to send a single integer (instead of using outlet_int()).
	@code
	t_atom myNumber; 

	atom_setlong(&myNumber, 432);
	outlet_anything(myOutlet, gensym("int"), 1, &myNumber);
	@endcode

	@remark			Notice that outlet_anything() expects the message argument as a 
					#t_symbol*, so you must use gensym() on a character string. 
				
					If you’ll be sending the same message a lot, you might call gensym() on the message string at 
					initialization time and store the result in a global variable to save the 
					(significant) overhead of calling gensym() every time you want to send a 
					message.
					
					Also, do not send lists using outlet_anything() with list as 
					the selector argument. Use the outlet_list() function instead. 
*/
void *outlet_anything(void *o, t_symbol *s, short ac, t_atom *av);


void *inlet4(void *x, void *w, char *s, char *s1);


void inlet_to(void *x, void *w);


short outlet_add(void *x, void *ip);


void outlet_rm(void *x, void *ip);


void outlet_atoms(void *out, short argc, t_atom *argv);




// clock functions

/**
	Create a new Clock object. 
	Normally, clock_new() is called in your instance creation 
	function—and it cannot be called from a thread other than the main thread. 
	To get rid of a clock object you created, use freeobject().

	@ingroup clocks
	@param	obj		Argument that will be passed to clock function fn when it is called. 
					This will almost always be a pointer to your object.
	@param	fn		Function to be called when the clock goes off, 
					declared to take a single argument as shown in @ref clocks_using_clocks.
	@return			A pointer to a newly created Clock object.
*/
void *clock_new(void *obj, method fn);


void clock_set(void *obj,long when);


/**
	Schedule the execution of a Clock.
	clock_delay() sets a clock to go off at a certain number of 
	milliseconds from the current logical time.

	@ingroup clocks
	@param	x		Clock to schedule. 
	@param	n		Delay, in milliseconds, before the Clock will execute.
	@see	clock_fdelay()
*/
void clock_delay(void *x, long n);


/**
	Cancel the scheduled execution of a Clock. 
	clock_unset() will do nothing (and not complain) if the Clock passed 
	to it has not been set.
		
	@ingroup clocks
	@param	x		Clock to cancel. 
*/
void clock_unset(void *x);
void clock_xdelay(void *x, long n);
void clock_xset(void *x, long n);
void clock_xunset(void *x);
short clock_getextfmt(void);


/**
	Schedule the execution of a Clock using a floating-point argument.
	clock_delay() sets a clock to go off at a certain number of 
	milliseconds from the current logical time.

	@ingroup clocks
	@param	c		Clock to schedule. 
	@param	time	Delay, in milliseconds, before the Clock will execute.
	@see	clock_delay()
*/
void clock_fdelay(void *c, double time);
void clock_fset(void *x, double when);
void clock_fset2(void *x, double when, double offset);
void clock_fdelay(void *x, double f);
void clock_fdelay2(void *x, double delay, double offset);


/**
	Find out the current logical time of the scheduler in milliseconds
	as a floating-point number.

	@ingroup clocks
	@param	time	Returns the current time.
	@see	gettime()
	@see	setclock_getftime()
	@see	setclock_gettime()
*/
void clock_getftime(double *time);


/** Schedule a Clock on a scheduler. 
	Schedules the Clock c to execute at time units after the current 
	time. If scheduler x is 0 or does not point to a setclock object, the 
	internal millisecond scheduler is used. Otherwise c is scheduled on 
	the setclock object’s list of Clocks. The Clock should be created with 
	clock_new(), the same as for a Clock passed to clock_delay().

	@ingroup clocks
	@param	x		A setclock object to be used for scheduling this clock.
	@param	c		Clock object containing the function to be executed.
	@param	time	Time delay (in the units of the Setclock) from the 
					current time when the Clock will be executed.
	@see	@ref 	setclock
	@see			setclock_fdelay()
*/
void setclock_delay(t_object *x, void *c, long time);


/** Remove a Clock from a scheduler.
	This function unschedules the Clock c in the list of Clocks in the 
	setclock object x, or the internal millisecond scheduler if scheduler is 0.

	@ingroup clocks
	@param	x		The setclock object that was used to schedule this clock. 
					If 0, the clock is unscheduled from the internal millisecond scheduler.
	@param	c		Clock object to be removed from the scheduler.
	@see	@ref	setclock
*/
void setclock_unset(t_object *x, void *c);


/** Find out the current time value of a setclock object.
	@ingroup clocks
	@param	x		A setclock object. 
	@return			Returns the current time value of the setclock object scheduler. 
					If scheduler is 0, setclock_gettime is equivalent to the function 
					gettime that returns the current value of the internal millisecond clock.
	@see	@ref	setclock
	@see			setclock_getftime()
*/
long setclock_gettime(t_object *x);


/**	Schedule a Clock on a scheduler, using a floating-point time argument.
	@ingroup clocks
	@param	s		A setclock object to be used for scheduling this clock.
	@param	c		Clock object containing the function to be executed.
	@param	time	Time delay (in the units of the Setclock) from the 
					current time when the Clock will be executed.
	@see	@ref	setclock
	@see			setclock_delay()
*/
void setclock_fdelay(t_object *s, void *c, double time);


/** Find out the current time value of a setclock object in floating-point milliseconds.
	@ingroup clocks
	@param	s		A setclock object.
	@param	time	The current time in milliseconds. 
	@see	@ref	setclock
	@see			setclock_gettime()
*/
void setclock_getftime(t_object *s, double *time);



// real-time
double systimer_gettime(void);


// scheduler functions

/**
	Find out the current logical time of the scheduler in milliseconds.
	
	@ingroup	clocks
	@return		Returns the current time.
	@see		clock_getftime()
*/
long gettime(void);
long getschedtime(void);
long getexttime(void);
void sched_suspend(void);
void sched_resume(void);


/**
	Cause a function to be executed at the timer level at some time in the future.

	@ingroup	threading
	@param		ob		First argument passed to the function fun when it executes.
	@param		fun		Function to be called, see below for how it should be declared.
	@param		when	The logical time that the function fun will be executed.
	@param		sym		Second argument passed to the function fun when it executes.
	@param		argc	Count of arguments in argv. argc is also the third argument passed to the function fun when it executes.
	@param		argv	Array containing a variable number of #t_atom function arguments. 
						If this argument is non-zero, defer allocates memory to make a copy of the arguments 
						(according to the size passed in argc) 
						and passes the copied array to the function fun when it executes as the fourth argument.

	@remark				schedule() calls a function at some time in the future. Unlike defer(), 
						the function is called in the scheduling loop when logical time is equal 
						to the specified value when. This means that the function could be 
						called at interrupt level, so it should follow the usual restrictions on
						interrupt-level conduct. The function fun passed to schedule should 
						be declared as follows: 
	
	@code					
	void myobject_do (myObject *client, t_symbol *s, short argc, t_atom *argv); 
	@endcode
	
	@remark				One use of schedule() is as an alternative to using the lockout flag. 	

	@see		defer()
*/
void schedule(void *ob, method fun, long when, t_symbol *sym, short argc, Atom *argv);
void schedulef(void *ob, method fun, double when, t_symbol *sym, short argc, Atom *argv);




/**	Create a new local scheduler.
	@ingroup		clocks
	@return			A pointer to the newly created scheduler.
	@see	@ref	creating_schedulers
*/
void *scheduler_new(void);


/**	Make a scheduler current, so that future related calls (such as 
	clock_delay()) will affect the appropriate scheduler. 

	@ingroup		clocks
	@param	x		The scheduler to make current.
	@return			This routine returns a pointer to the previously current scheduler, 
					wsaved and restored when local scheduling is complete.
	@see	@ref	creating_schedulers
*/
void *scheduler_set(void *x);


/** Run scheduler events to a selected time.
	@ingroup		clocks
	@param	x		The scheduler to advance.
	@param	until	The ending time for this run (in milliseconds). 
	@see	@ref	creating_schedulers
*/
void scheduler_run(void *x, double until);


/** Set the current time of the scheduler. 
	@ingroup		clocks
	@param	x		The scheduler to set.
	@param	time	The new current time for the selected scheduler (in milliseconds). 
	@see	@ref	creating_schedulers
*/
void scheduler_settime(void *x, double time);


/** Retrieve the current time of the selected scheduler.
	@ingroup		clocks
	@param	x		The scheduler to query.
	@param	time	The current time of the selected scheduler.
	@see	@ref	creating_schedulers
*/
void scheduler_gettime(void *x, double *time);




/**
	Cause a function to be executed at the timer level at some time in the future specified by a delay offset.

	@ingroup	threading
	@param		ob		First argument passed to the function fun when it executes.
	@param		fun		Function to be called, see below for how it should be declared.
	@param		delay	The delay from the current time before the function will be executed.
	@param		sym		Second argument passed to the function fun when it executes.
	@param		argc	Count of arguments in argv. argc is also the third argument passed to the function fun when it executes.
	@param		argv	Array containing a variable number of #t_atom function arguments. 
						If this argument is non-zero, defer allocates memory to make a copy of the arguments 
						(according to the size passed in argc) 
						and passes the copied array to the function fun when it executes as the fourth argument.

	@remark				schedule_delay() is similar to schedule but allows you to specify the 
						time as a delay rather than a specific logical time. 
						
						One use of schedule() or schedule_delay() is as an alternative to 
						using the lockout flag. Here is an example click method that calls 
						schedule() instead of outlet_int() surrounded by lockout_set() calls.

	@code
	void myobject_click (t_myobject *x, Point pt, short modifiers) 
	{ 
		t_atom a[1]; 
		a[0].a_type = A_LONG; 
		a[0].a_w.w_long = Random(); 
		schedule_delay(x, myobject_sched, 0 ,0, 1, a); 
	} 

	void myobject_sched (t_myobject *x, t_symbol *s, short ac, t_atom *av) 
	{ 
		outlet_int(x->m_out,av->a_w.w_long); 
	} 
	@endcode

	@see schedule()
*/
void schedule_delay(void *ob, method fun, long delay, t_symbol *sym, short argc, t_atom *argv);
void schedule_fdelay(void *ob, method fun, double delay, t_symbol *sym, short argc, t_atom *argv);
void schedule_defer(void *ob, method fun, long delay, t_symbol *sym, short argc, t_atom *arv);
void schedule_fdefer(void *ob, method fun, double delay, t_symbol *sym, short argc, t_atom *arv);
short lockout_set(short);

/**
	Determine whether your code is executing in the Max scheduler thread.
	
	@ingroup	threading
	@return		This function returns non-zero if you are within Max's scheduler thread, zero otherwise. 
				Note that if your code sets up other types of interrupt-level callbacks, 
				such as for other types of device drivers used in asynchronous mode, isr will return false.
*/
long isr(void);
short isr_set(short way);



// queue functions

/**
	Create a new Qelem.
	The created Qelem will need to be freed using qelem_free(), do not use freeobject().
	
	@ingroup qelems
	@param	obj	Argument to be passed to function fun when the Qelem executes. 
				Normally a pointer to your object.
	@param	fn	Function to execute.
	@return		A pointer to a Qelem instance.  
				You need to store this value to pass to qelem_set().
	
	@remark		Any kind of drawing or calling of Macintosh Toolbox routines that 
				allocate or purge memory should be done in a Qelem function. 
*/
void *qelem_new(void *obj, method fn);


/**
	Cause a Qelem to execute. 

	@ingroup qelems
	@param	q	The Qelem whose function will be executed in the main thread.
	
	@remark		The key behavior of qelem_set() is this: if the Qelem object has already 
				been set, it will not be set again. (If this is not what you want, see 
				defer().) This is useful if you want to redraw the state of some 
				data when it changes, but not in response to changes that occur faster 
				than can be drawn. A Qelem object is unset after its queue function has 
				been called.
*/
void qelem_set(void *q);


/**
	Cancel a Qelem’s execution. 
	If the Qelem’s function is set to be called, qelem_unset() will stop it 
	from being called. Otherwise, qelem_unset() does nothing.

	@ingroup qelems
	@param	q	The Qelem whose execution you wish to cancel.
*/
void qelem_unset(void *q);


/**
	Free a Qelem object created with qelem_new().
 	Typically this will be in your object’s free funtion. 

	@ingroup qelems
	@param	x	The Qelem to destroy.
*/
void qelem_free(void *x);


/**
	Cause a Qelem to execute with a higher priority.
	This function is identical to qelem_set(), except that the Qelem’s 
	function is placed at the front of the list of routines to execute in the 
	main thread instead of the back. Be polite and only use 
	qelem_front() only for special time-critical applications.
		
	@ingroup qelems
	@param	x	The Qelem whose function will be executed in the main thread.
*/
void qelem_front(void *x);


/**
	Defer execution of a function to the main thread if (and only if) 
	your function is executing in the scheduler thread.
	
	@ingroup	threading
	@param		ob		First argument passed to the function fun when it executes.
	@param		fn		Function to be called, see below for how it should be declared.
	@param		sym		Second argument passed to the function fun when it executes.
	@param		argc	Count of arguments in argv. argc is also the third argument passed to the function fun when it executes.
	@param		argv	Array containing a variable number of #t_atom function arguments. 
						If this argument is non-zero, defer allocates memory to make a copy of the arguments 
						(according to the size passed in argc) 
						and passes the copied array to the function fun when it executes as the fourth argument.
	@return		Return values is for internal Cycling '74 use only.
	
	@remark		This function uses the isr() routine to determine whether you’re at the 
				Max timer interrupt level (in the scheduler thread). 
				If so, defer() creates a Qelem (see @ref qelems), calls 
				qelem_front(), and its queue function calls the function fn you 
				passed with the specified arguments. 
				If you’re not in the scheduler thread, the function is executed immediately with the 
				arguments. Note that this implies that defer() is not appropriate for 
				using in situations such as Device or File manager I/0 completion routines. 
				The defer_low() function is appropriate however, because it always defers. 

				The deferred function should be declared as follows:
	@code
	void myobject_do (myObject *client, t_symbol *s, short argc, t_atom *argv);
	@endcode
	
	@see		defer_low()
*/
void *defer(void *ob,method fn,t_symbol *sym,short argc,t_atom *argv);


/**
	Defer execution of a function to the back of the queue on the main thread.

	@ingroup	threading
	@param		ob		First argument passed to the function fun when it executes.
	@param		fn		Function to be called, see below for how it should be declared.
	@param		sym		Second argument passed to the function fun when it executes.
	@param		argc	Count of arguments in argv. argc is also the third argument passed to the function fun when it executes.
	@param		argv	Array containing a variable number of #t_atom function arguments. 
						If this argument is non-zero, defer allocates memory to make a copy of the arguments 
						(according to the size passed in argc) 
						and passes the copied array to the function fun when it executes as the fourth argument.
	@return		Return values is for internal Cycling '74 use only.

	@remark		defer_low() always defers a call to the function fun whether you are already 
				in the main thread or not, and uses qelem_set(), not qelem_front(). This 
				function is recommended for responding to messages that will cause 
				your object to open a dialog box, such as read and write. 

				The deferred function should be declared as follows:
	@code
	void myobject_do (myObject *client, t_symbol *s, short argc, t_atom *argv);
	@endcode

	@see		defer()
*/
void *defer_low(void *ob,method fn,t_symbol *sym,short argc,t_atom *argv);

void *defer_medium(void *ob, method fn, t_symbol *sym, short argc, t_atom *argv);

void *defer_front(void *ob, method fn, t_symbol *sym, short argc, t_atom *argv);

// private
void *defer_sys_low(void *ob,method fn,t_symbol *sym,short argc,t_atom *argv);



// binbuf functions

/**
	Use binbuf_new() to create and initialize a Binbuf.
	@ingroup	binbuf
	@return		Returns a new binbuf if successful, otherwise NULL.
*/
void *binbuf_new(void);


/**
	Use binbuf_vinsert() to append a Max message to a Binbuf adding a semicolon.
	@ingroup binbuf
	
	@param	x		Binbuf containing the desired Atom. 
	@param	fmt		A C-string containing one or more letters corresponding to the types of each element of the message. 
					s for #t_symbol*, l for long, or f for float.
	@param	...		Elements of the message, passed directly to the function as Symbols, longs, or floats.
	
	@remark	binbuf_vinsert() works somewhat like a printf() for Binbufs. It 
	allows you to pass a number of arguments of different types and insert 
	them into a Binbuf. The entire message will then be terminated with a 
	semicolon. Only 16 items can be passed to binbuf_vinsert(). 
	
	The example below shows the implementation of a normal object’s 
	save method. The save method requires that you build a message that 
	begins with #N (the new object) , followed by the name of your object 
	(in this case, represented by the #t_symbol myobject), followed by any 
	arguments your instance creation function requires. In this example, 
	we save the values of two fields m_val1 and m_val2 defined as longs. 
	
	@code
	void myobject_save (myObject *x, Binbuf *dstBuf) 
	{ 
		binbuf_vinsert(dstBuf, "ssll", gensym("#N"), 
			gensym("myobject"), 
			x->m_val1, x->m_val2); 
	}
	@endcode
	
	Suppose that such an object had written this data into a file. If you 
	opened the file as text, you would see the following: 
	
	@code
	#N myobject 10 20; 
	#P newobj 218 82 30 myobject; 
	@endcode
	
	The first line will result in a new myobject object to be created; the 
	creation function receives the arguments 10 and 20. The second line 
	contains the text of the object box. The newobj message to a patcher 
	creates the object box user interface object and attaches it to the 
	previously created myobject object. Normally, the newex message is 
	used. This causes the object to be created using the arguments that 
	were typed into the object box.
*/
void binbuf_vinsert(void *x, char *fmt, ...);


/**
	Use binbuf_insert() to append a Max message to a Binbuf adding a semicolon.
	@ingroup binbuf
	
	@param	x		Binbuf to receive the items. 
	@param	s		Ignored. Pass NULL.
	@param	argc	Count of items in the argv array. 
	@param	argv	Array of t_atoms to add to the Binbuf. 
	
	@remark	You’ll use binbuf_insert() instead of binbuf_append() if you were 
	saving your object into a Binbuf and wanted a semicolon at the end. If 
	the message is part of a file that will later be evaluated, such as a 
	Patcher file, the first argument argv[0] will be the receiver of the 
	message and must be a Symbol. binbuf_vinsert() is 
	easier to use than binbuf_insert(), since you don’t have to format 
	your data into an array of Atoms first. 
	
	binbuf_insert() will also convert the t_symbols #1 through #9 into 
	$1 through $9. This is used for saving patcher files that take 
	arguments; you will probably never save these symbols as part of 
	anything you are doing.
*/
void binbuf_insert(void *x, t_symbol *s, short argc, t_atom *argv);


/**
	Use binbuf_eval to evaluate a Max message in a Binbuf, passing it arguments.
	binbuf_eval() is an advanced function that evaluates the message in a 
	Binbuf with arguments in argv, and sends it to receiver.
					
	@ingroup binbuf
	@param	x	Binbuf containing the message.
	@param	ac	Count of items in the argv array.
	@param	av	Array of t_atoms as the arguments to the message. 
	@param	to	Receiver of the message. 

	@return		The result of sending the message.
*/
void *binbuf_eval(void *x, short ac, t_atom *av, void *to);


/**
	Use binbuf_getatom to retrieve a single Atom from a Binbuf.
	
	@ingroup binbuf
	@param	x	Binbuf containing the desired #t_atom. 
	@param	p1	Offset into the Binbuf’s array of types. Modified to point to the next #t_atom.
	@param	p2	Offset into the Binbuf’s array of data. Modified to point to the next #t_atom.
	@param	ap	Location of a #t_atom where the retrieved data will be placed.
	
	@return		1 if there were no t_atoms at the specified offsets, 
				0 if there’s a legitimate t_atom returned in result.
				
	@remark		To get the first t_atom, set both typeOffset and stuffOffset to 0.
				Here’s an example of getting all the items in a Binbuf: 
	@code
	t_atom holder; 
	long to, so; 
	
	to = 0; 
	so = 0; 
	while (!binbuf_getatom(x, &to, &so, &holder)){
		// do something with the t_atom
	}
	@endcode
*/
short binbuf_getatom(void *x, long *p1, long *p2, t_atom *ap);


/**
	Use binbuf_text() to convert a text handle to a Binbuf.
	binbuf_text() parses the text in the handle srcText and converts it 
	into binary format. Use it to evaluate a text file or text line entry into a 
	Binbuf.
	
	@ingroup binbuf
	@param	x		Binbuf to contain the converted text. 
					It must have already been created with binbuf_new. 
					Its previous contents are destroyed.
	@param	srcText	Handle to the text to be converted. It need not be terminated with a 0.
	@param	n		Number of characters in the text. 
	@return			If binbuf_text encounters an error during its operation, 
					a non-zero result is returned, otherwise it returns 0.
	
	@remark			Note: Commas, symbols containing a dollar sign followed by a number 
					1-9, and semicolons are identified by special pseudo-type constants for 
					you when your text is binbuf-ized. 
					
					The following constants in the a_type field of Atoms returned by 
					binbuf_getAtom identify the special symbols #A_SEMI, 
					#A_COMMA, and #A_DOLLAR. 
					
					For a #t_atom of the pseudo-type #A_DOLLAR, the a_w.w_long field of 
					the #t_atom contains the number after the dollar sign in the original 
					text or symbol. 
					
					Using these pseudo-types may be helpful in separating “sentences” and 
					“phrases” in the input language you design. For example, the old pop-up 
					umenu object allowed users to have spaces in between words by requiring 
					the menu items be separated by commas. It’s reasonably easy, using 
					binbuf_getatom(), to find the commas in a Binbuf in order to 
					determine the beginning of a new item when reading the atomized text 
					to be displayed in the menu.
					 
					If you want to use a literal comma or semicolon in a symbol, precede it 
					with a backslash (\\) character. The backslash character can be included 
					by using two backslashes in a row.
*/
short binbuf_text(void *x, char **srcText, long n);


/**
	Use binbuf_totext() to convert a Binbuf into a text handle.
	binbuf_totext() converts a Binbuf into text and places it in a handle. 
	Backslashes are added to protect literal commas and semicolons 
	contained in symbols. The pseudo-types are converted into commas, 
	semicolons, or dollar-sign and number, without backslashes preceding 
	them. binbuf_text can read the output of binbuf_totext and 
	make the same Binbuf.
		
	@ingroup binbuf
	@param	x		Binbuf with data to convert to text.
	@param	dstText	Pre-existing handle where the text will be placed. 
					dstText will be resized to accomodate the text. 
	@param	sizep	Where binbuf_totext() returns the number of characters in the converted text handle.
	@return			If binbuf_totext runs out of memory during its operation, it returns a non-zero result, 
					otherwise it returns 0.
*/
short binbuf_totext(void *x, char **dstText, long *sizep);


/**
	Use binbuf_set() to change the entire contents of a Binbuf. 
	The previous contents of the Binbuf are destroyed.

	@ingroup binbuf
	@param	x		Binbuf to receive the items.
	@param	s		Ignored. Pass NULL.
	@param	argc	Count of items in the argv array.
	@param	argv	Array of t_atoms to put in the Binbuf.
*/
void binbuf_set(void *x, t_symbol *s, short argc, t_atom *argv);


/**
	Use binbuf_append to append t_atoms to a Binbuf without modifying them.
	@ingroup binbuf
	@param	x		Binbuf to receive the items.
	@param	s		Ignored.  Pass NULL.
	@param	argc	Count of items in the argv array.
	@param	argv	Array of atoms to add to the Binbuf.
*/
void binbuf_append(void *x, t_symbol *s, short argc, t_atom *argv);

// still supported?
short binbuf_read(void *x, char *name, short volume, short binary);

// still supported?
short binbuf_write(void *x, char *fn, short vol, short binary);

void binbuf_savebox(void *x, void *w, t_symbol *what, short d1, short d2, short d3, long d4, short hidden, short user);
void binbuf_delete(void *x, long fromType, long toType, long fromData, long toData);
void binbuf_addtext(void *x, char **text, long size);


/**
	Use readatom() to read a single Atom from a text buffer.
	@ingroup		binbuf
	@param	outstr	C-string of 256 characters that will receive the next text item read from the buffer.
	@param	text	Handle to the text buffer to be read.
	@param	n		Starts at 0, and is modified by readatom to point to the next item in the text buffer. 
	@param	e		Number of characters in text.
	@param	ap		Where the resulting Atom read from the text buffer is placed.
	@return			readatom() returns non-zero if there is more text to read, 
					and zero if it has reached the end of the text. 
					Note that this return value has the opposite logic from that of binbuf_getatom().
	
	@remark			This function provides access to the low-level Max text evaluator used 
					by binbuf_text(). It is designed to operate on a handle of characters 
					(text) and called in a loop, as in the example shown below.
	@code
	long index = 0; 
	t_atom dst; 
	char outstr[256]; 
	
	while (readatom(outstr,textHandle,&index,textLength,&dst)) 
	{ 
		// do something with the resulting Atom
	} 
	@endcode
	
	@remark			An alternative to using readatom is to turn your text into a Binbuf 
					using binbuf_text(), then call binbuf_getatom() in a loop.
*/
short readatom(char *outstr, char **text, long *n, long e, t_atom *ap);
char *atom_string(t_atom *a);


// atombuf functions

/**
	Use atombuf_new() to create a new Atombuf from an array of t_atoms.
	
	@ingroup atombuf
	@param	argc	Number of t_atoms in the argv array. May be 0.
	@param	argv	Array of t_atoms. If creating an empty Atombuf, you may pass 0.
	@return			atombuf_new() create a new #t_atombuf and returns a pointer to it. 
					If 0 is returned, insufficient memory was available.
*/
void *atombuf_new(long argc, t_atom *argv);


/**
	Use atombuf_free() to dispose of the memory used by a #t_atombuf.

	@ingroup atombuf
	@param	x	The #t_atombuf to free.
*/
void atombuf_free(t_atombuf *x);


/**
	Use atombuf_text() to convert text to a #t_atom array in a #t_atombuf.
	To use this routine to create a new Atombuf from the text buffer, first 
	create a new empty t_atombuf with a call to atombuf_new(0,NULL).
		
	@ingroup atombuf
	@param	x		Pointer to existing atombuf variable. 
					The variable will be replaced by a new Atombuf containing the converted text.
	@param	text	Handle to the text to be converted. It need not be zero-terminated.
	@param	size	Number of characters in the text.
*/
void atombuf_text(t_atombuf **x, char **text, long size);

void atombuf_totext(t_atombuf *x, char **text, long *size);
short atombuf_count(t_atombuf *x);
void atombuf_set(t_atombuf *x, short start, short num);
long atombuf_replacepoundargs(t_atombuf *x, long argc, t_atom *argv);


// message functions

/**	Send a typed message directly to a Max object.
	@ingroup class_old
	
	@param op	Max object that will receive the message. 
	@param msg	The message selector. 
	@param argc	Count of message arguments in argv. 
	@param argp	Array of t_atoms; the message arguments. 
	@return		If the receiver object can respond to the message, 
				typedmess() returns the result. Otherwise, an error message will be 
				seen in the Max window and 0 will be returned.
	
	@remark	typedmess sends a message to a Max object (receiver) a message 
		with arguments. Note that the message 
		must be a #t_symbol, not a character string, so you must call gensym 
		on a string before passing it to typedmess. Also, note that untyped 
		messages defined for classes with the argument list #A_CANT cannot be 
		sent using typedmess. You must use getfn() etc. instead. 
		
		Example: 
	@code
	//If you want to send a bang message to the object bang_me… 
	void *bangResult; 
	bangResult = typedmess(bang_me,gensym("bang"),0,0L);
	@endcode
*/
void *typedmess(t_object *op, t_symbol *msg, short argc, t_atom *argp);


/** Use getfn() to send an untyped message to a Max object with error checking.
	@ingroup class_old
	
	@param op	Receiver of the message.
	@param msg	Message selector.
	@return		getfn returns a pointer to the method bound to the message selector 
				msg in the receiver’s message list. It returns 0 and prints an error 
				message in Max Window if the method can’t be found.
*/
method getfn(t_object *op, t_symbol *msg);


/** Use egetfn() to send an untyped message to a Max object that always works.
	@ingroup class_old
	
	@param op	Receiver of the message.
	@param msg	Message selector.
	@return		egetfn returns a pointer to the method bound to the message selector 
				msg in the receiver’s message list. If the method can’t be found, a 
				pointer to a do-nothing function is returned.
*/
method egetfn(t_object *op, t_symbol *msg);


/** Use zgetfn() to send an untyped message to a Max object without error checking.
	@ingroup class_old
	
	@param op	Receiver of the message.
	@param msg	Message selector.
	@return		zgetfn returns a pointer to the method bound to the message selector 
				msg in the receiver’s message list. It returns 0 but doesn’t print an 
				error message in Max Window if the method can’t be found.
*/
method zgetfn(t_object *op, t_symbol *msg);



// commenting out -- some of these are probably still good, we need to investigate -- TAP
void patcher_eachdo(eachdomethod fun, void *arg);	// this one is still legit
void loadbang_suspend(void);  // used by poly~ on windows
void loadbang_resume(void);  // used by poly~ on windows
/*
// patcher functions
GrafPtr patcher_setport(t_patcher *p);
#ifdef WIN_VERSION
void patcher_restoreport(GrafPtr gp);
#endif
#ifdef MAC_VERSION
#define patcher_restoreport  SetPort
#endif
void *patchbox(void *p, method fn, short ac, t_atom *av, short defwidth);
void patcher_dirty(t_patcher *p);
void patcher_selectbox(t_patcher *p, t_box *b);
void patcher_deselectbox(t_patcher *p, t_box *b);
void patcher_okclose(t_patcher *p, void *x);
void patcher_eachdo(eachdomethod fun, void *arg);
void patcher_avoidrect(t_patcher *p, Rect *r2, short id);
short patcher_boxname(t_patcher *p, t_box *b, t_symbol **s);
void patcher_setboxname(t_patcher *p, t_box *b, t_symbol *s);
void patcher_nohilite(t_patcher *p);
void hilite_settarget(t_patcher *p, t_box *b);
void *patcher_parentpatcher(t_patcher *p);

// box functions 
void box_new(t_box *b, t_patcher *patcher, short flags, short x1, short y1, short x2, short y2);
void box_free(t_box *b);
void box_ready(t_box *b);
short box_ownerlocked(t_box *b);
void box_size(void *b, short x, short y);
void *textbox(void *p, method fn, short ac, t_atom *av);
short box_nodraw(t_box *b);
void box_erase(t_box *b);
short box_invalnow(t_box *b);
void *box_clip(t_box *b);
void box_enddraw(t_box *b);
short box_visible(t_box *b);
void box_hilite(t_box *b, short way);
void box_color(t_box *b);
void box_getcolor(t_box *b,short index, RGBColor *rgb);
long usecolor(t_box *b);
void palette_getcolor(t_box *b, short index, RGBColor *rgb);
long boxcolor_rgb2index(RGBColor *n);
*/



// table functions

/**	Get a handle to the data in a named table object.
	@ingroup tables
	
	@param s	Symbol containing the name of the table object to find.
	@param hp	Address of a handle where the table’s data will be returned if the named table object is found.
	@param sp	Number of elements in the table (its size in longs). 
	@return		If no table object is associated with the symbol tableName, table_get() returns a non-zero result.
	
	@remark		table_get searches for a table associated with the t_symbol 
				tableName. If one is found, a Handle to its elements (stored as an 
				array of long integers) is returned and the function returns 0. 
				Never count on a table to exist across calls to 
				one of your methods. Call table_get and check the result each time 
				you wish to use a table. 
	
				Here is an example of retrieving the 40th element of a table: 
	@code
	long **storage,size,value; 
	if (!table_get(gensym("somename"),&storage,&size)) { 
		if (size > 40) 
			value = *((*storage)+40); 
	}
	@endcode
*/
short table_get(t_symbol *s, long ***hp, long *sp);


/**	Mark a table object as having changed data.
	@ingroup tables
	@param s	Symbol containing the name of a table object.
	@return		If no table is associated with tableName, table_dirty returns a non-zero result.
*/
short table_dirty(t_symbol *s);

// commenting out, where are the replacements? -- tap
/*
// ed functions
void *ed_new(t_object *assoc);
void ed_vis(t_ed *x);
void ed_settext(t_ed *x, t_handle text, long len);
*/


// file functions

/** Load a data file into a handle.
	This is a low-level routine used for reading text and data files. You 
		specify the file’s name and Path ID, as well as a pointer to a Handle.

	@ingroup loading_max_files
	@param	name	Name of the patcher file to load. 
	@param	volume	Path ID specifying the location of the file.
	@param	h		Pointer to a handle variable that will receive the handle 
					that contains the data in the file.
	@param	sizep	Size of the handle returned in h. 
	@return			If the file is found, readtohandle creates a Handle, reads all the data in 
					the file into it, assigns the handle to the variable hp, and returns the 
					size of the data in size. readtohandle returns 0 if the file was 
					opened and read successfully, and non-zero if there was an error.
*/
short readtohandle(char *name, short volume, char ***h, long *sizep);


/** Load a patcher file by name and volume reference number.
	@ingroup loading_max_files
	@param	name	Filename of the patcher file to load (C string). 
	@param	vol		Path ID specifying the location of the file.
	@return			If the file is found, fileload tries to open 
					the file, evaluate it, open a window, and bring it to the front. A pointer 
					to the newly created Patcher is returned if loading is successful, 
					otherwise, if the file is not found or there is insufficient memory, zero 
					is returned.
*/
void *fileload(char *name, short vol);


/** Pass arguments to Max files when you open them.
	This function loads the specified file and returns a pointer to the 
	created object. Historically, intload() is was used to open patcher files, 
	whether they are in text or Max binary format. 
	It could also open table files whose contents begin with the word “table.” 
	
	@ingroup loading_max_files
	@param	name		Name of the file to open. 
	@param	volume		Path ID specifying the location of the file. 
	@param	s			A symbol.
	@param	ac			Count of t_atoms in av. To properly open a patcher file, ac should be 9. 
	@param	av			Array of t_atoms that will replace the changeable 
						arguments 1-9. The default behavior could be to set 
						all these to t_atoms of type #A_LONG with a value of 0.
	@param	couldedit	If non-zero and the file is not a patcher file, the file is opened as a text file.
	@return				If couldedit is non-zero and the file is not a patcher file, it is made into 
						a text editor, and lowload returns 0. If couldedit is non-zero, lowload
						will just alert the user to an error and return 0. If there is no error, the 
						value returned will be a pointer to a patcher or table object. 
*/
void *intload(char *name, short volume, t_symbol *s, short ac, t_atom *av, short couldedit);


/** Load a patcher file located in the Max search path by name.
	This function searches for a patcher file, opens it, 
	evaluates it as a patcher file, opens a window for the patcher and brings 
	it to the front. You need only specify a filename and Max will look 
	through its search path for the file. The search path begins with the 
	current “default volume” that is often the volume of the last opened 
	patcher file, then the folders specified in the File Preferences dialog, 
	searched depth first, then finally the folder that contains the Max 
	application.

	@ingroup loading_max_files
	@param	name	Filename of the patcher file to load (C string).
	@return			If stringload() returns a non-zero result, you can later 
					use freeobject() to close the patcher, or just let users do it themselves. 
					If stringload() returns zero, no file with the specified name was 
					found or there was insufficient memory to open it.
*/
void *stringload(char *name);

void *resource_install(char *name, long rsrc);
void *toolfile_new(char *name, short vol, long type);
long toolfile_fread(void *t, char *buf, long n);
long toolfile_fwrite(void *t, char *buf, long n);
short toolfile_getc(void *t);
short collective_load(char *name, short vol, short argc, t_atom *argv);
void *onecopy_fileload(char *s, short path);

// resource functions
short rescopy(long type,short id);
short resnamecopy(long type, char *name);



// preset functions

/** Give the preset object a general message to restore the current state of your object.
	This is a general preset function for use when your object’s state 
	cannot be restored with a simple int or set message. The example 
	below shows the expected format for specifying what your current 
	state is to a preset object. The first thing you supply is your object itself, 
	followed by the symbol that is the name of your object’s class (which 
	you can retrieve from your object using the macro ob_sym, declared in 
	ext_mess.h). Next, supply the symbol that specifies the message you 
	want receive (a method for which had better be defined in your class), 
	followed by the arguments to this message—the current values of your 
	object’s fields.

	@ingroup presets
	@param	fmt		C string containing one or more letters corresponding 
					to the types of each element of the message. s for 
					Symbol, l for long, or f for float.
	@param	...		Elements of the message used to restore the state of 
					your object, passed directly to the function as Symbols, 
					longs, or floats. See below for an example that 
					conforms to what the preset object expects.
*/
void preset_store(char *fmt, ... /*struct b100 arg1 */);


/** Restore the state of your object with a set message.
	This function causes a set message with the argument value to be sent 
	to your object from the preset object when the user executes a preset.

	@ingroup presets	
	@param	obj		Your object.
	@param	val		Current value of your object.
	
*/
void preset_set(t_object *obj, long val);


/**	Restore the state of your object with an int message.
	This function causes an int message with the argument value to be 
	sent to your object from the preset object when the user executes a 
	preset. All of the existing user interface objects use the int message for 
	restoring their state when a preset is executed. 

	@ingroup presets	
	@param	x	Your object.
	@param	n	Current value of your object.
*/
void preset_int(void *x,long n);



// num functions

/** Increment the event serial number.
	@ingroup evnum
*/
void evnum_incr(void);


/** Get the current value of the event serial number.
	@ingroup evnum
	@return	The current value of the event serial number.
*/
long evnum_get(void);


// commenting out -- tap
//t_numerical *num_new(short top, short left, short bottom, short right, method draw,
//	longmethod inc, long flags, long min, long max, long val, short font, short fsize);
//void num_draw(t_numerical *x);
//void num_hilite(t_numerical *x, short way);
//long num_test(t_numerical *x, Point pt);
//void num_track(t_numerical *x, Point pt, method track, void *arg);
//void num_setvalue(t_numerical *x, long newval, short redraw);



// proxy functions

/**
	Use proxy_new to create a new Proxy object. 
	
	@ingroup inout
	@param	x			Your object. 
	@param	id			A non-zero number to be written into your object when a message is received in this particular Proxy.
						Normally, id will be the inlet “number” analogous to in1, in2 etc. 
	@param	stuffloc	A pointer to a location where the id value will be written.
	@return				A pointer to the new proxy inlet.
	
	@remark		This routine creates a new Proxy object (that includes an inlet). It 
				allows you to identify messages based on an id value stored in the 
				location specified by stuffLoc. You should store the pointer 
				returned by proxy_new() because you’ll need to free all Proxies in your 
				object’s free function using object_free(). 
				
				After your method has finished, Proxy sets the stuffLoc location 
				back to 0, since it never sees messages coming in an object’s leftmost 
				inlet. You’ll know you received a message in the leftmost inlet if the 
				contents of stuffLoc is 0. As of Max 4.3, stuffLoc is not always 
				guaranteed to be a correct indicator of the inlet in which a message was 
				received. Use proxy_getinlet() to determine the inlet number. 	
*/
void *proxy_new(void *x,long id,long *stuffloc);


/**
	Use proxy_getinlet to get the inlet number in which a message was received.
	Note that the “owner” argument should point to your external object’s instance, not a proxy object.

	@ingroup inout
	@param	master	Your object. 
	@return			The index number of the inlet that received the message.
*/
long proxy_getinlet(t_object *master);


//void *gwind_new(t_object *assoc, t_symbol *s, short style, short left, short top, short bottom, short right);


// connection functions
void *connection_client(t_object *cli, t_symbol *name, t_symbol *classname, method traverse);
void connection_server(t_object *obj, t_symbol *name);
void connection_send(t_object *server, t_symbol *name, t_symbol *mess, void *arg);
void connection_delete(t_object *ob, t_symbol *name);


// quittask functions

/**
	Register a function that will be called when Max exits.

	@ingroup misc
	@param	m	A function that will be called on Max exit.
	@param	a	Argument to be used with method m.
	
	@remark		quittask_install() provides a mechanism for your external to 
	register a routine to be called prior to Max shutdown. This is useful for 
	objects that need to provide disk-based persistance outside the 
	standard Max storage mechanisms, or need to shut down hardware or 
	their connection to system software and cannot do so in the 
	termination routine of a code fragment.
*/
void quittask_install(method m, void *a);


/**
	Unregister a function previously registered with quittask_install().

	@ingroup misc
	@param	m	Function to be removed as a shutdown method.
*/
void quittask_remove(method m);
void quittask_remove2(method m, void *a);


// notify functions
void *notify_new(t_object *owner);
void notify_enlist(t_object *dependent, t_object *owner);
void notify_update(void *xx);
void notify_free(t_object *owner);

// commenting out -- tap
/*
// syswind functions
Boolean syswindow_inlist(t_syswind w);
Boolean syswindow_isvisible(t_syswind w);
Boolean syswindow_isfloating(t_syswind w);
void syswindow_show(t_syswind w);
void syswindow_hide(t_syswind w);
void syswindow_move(t_syswind w, long x, long y, Boolean front);
void syswindow_size(t_syswind w, long width, long height, Boolean update);
t_wind *syswindow_wind(t_syswind w);
*/



// miscellaneous functions

/**
	Determine version information about the current Max environment.

	This function returns the version number of Max. In Max versions 
	2.1.4 and later, this number is the version number of the Max kernel 
	application in binary-coded decimal. Thus, 2.1.4 would return 214 hex 
	or 532 decimal. Version 3.0 returns 300 hex. 
	
	Use this to check for the existence of particular function macros that are only present in more 
	recent Max versions. Versions before 2.1.4 returned 1, except for 
	versions 2.1.1 - 2.1.3 which returned 2. 
	
	Bit 14 (counting from left) will 
	be set if Max is running as a standalone application, so you should 
	mask the lower 12 bits to get the version number.
		
	@ingroup	misc
	@return		The Max environment's version number.
*/
short maxversion(void);


/** Get a unique number for each Patcher file saved.
	This function returns a serial number that is incremented each time a 
	Patcher file is saved. This routine is useful for objects like table and coll 
	that have multiple objects that refer to the same data, and can “embed” 
	the data inside a Patcher file. If the serial number hasn’t changed since 
	your object was last saved, you can detect this and avoid saving 
	multiple copies of the object’s data.

	@ingroup evnum
	@return	The serial number.
*/
long serialno(void);


char **palette(void);
short ispatcher(t_object *x);
short isnewex(t_object *x);
// commenting out -- tap
//short newex_knows(t_box *x, t_symbol *selector);
void colorinfo(void *r);
void *callback_new(void *assoc, ProcPtr proc, long refCon, short offset, ProcPtr *callfun);


/**
	Use open_promptset() to add a prompt message to the open file dialog displayed by open_dialog().

	Calling this function before open_dialog() permits a string to 
	displayed in the dialog box instructing the user as to the purpose of the 
	file being opened. It will only apply to the call of open_dialog() that 
	immediately follows open_promptset().

	@ingroup files
	@param	s		A C-string containing the prompt you wish to display in the dialog box.
	@return			Ignore.
	
	@see open_dialog()
*/
void open_promptset(char *s);


/**
	Use saveas_promptset() to add a prompt message to the open file dialog displayed by saveas_dialog()
	or saveasdialog_extended().

	Calling this function before saveasdialog_extended() permits a string to 
	displayed in the dialog box instructing the user as to the purpose of the 
	file being opened. It will only apply to the call of saveasdialog_extended() that 
	immediately follows saveas_promptset().

	@ingroup files
	@param	s		A C-string containing the prompt you wish to display in the dialog box.
	@return			Ignore.
	
	@see open_dialog()
*/
void saveas_promptset(char *s);


void dialog_setkey(long type);
void saveasdialog_pathset(short path, short force);
void dialog_poll(short dosched, short doevent, unsigned short evtMask);
void forecolor(short index, short way);
void backcolor(short index, short way);
void *tabfromhandle(long **handle, long size);
void stdlist(t_object *x, t_symbol *s, short ac, t_atom *av);
void assist_queue(t_object *x, method fun);
void inspector_open(t_object *x, void *p, void *b);
void *object_subpatcher(t_object *x, long *index, void *arg);



// filewatch functions

/**	Create a new filewatcher.
	The file will not be actively watched until filewatcher_start() is called.
	The filewatcher can be freed using object_free().
	
	@ingroup			files
	@param	owner		Your object.  
						This object will receive the message "filechanged" when the watcher sees a change in the file or folder.
	@param	path		The path in which the file being watched resides, or the path of the folder being watched.
	@param	filename	The name of the file being watched, or an empty string if you are simply watching the folder specified by path.
	@return				A pointer to the new filewatcher.
	@remark				The "filechanged" method should have the prototype:
	@code
	void myObject_filechanged(t_myObject *x, char *filename, short path);
	@endcode
	*/
void *filewatcher_new(t_object *owner, short path, char *filename);

/**	Start watching a file using a filewatcher created with filewatcher_new().
	@param	x			A filewatcher pointer, as returned by filewatcher_new().	*/
void filewatcher_start(void *x);

/**	Stop watching a file using a filewatcher created with filewatcher_new().
	@param	x			A filewatcher pointer, as returned by filewatcher_new().	*/
void filewatcher_stop(void *x);



// fileusage functions

/**	Add a file to a collective.
	@ingroup		files
	@param	w		Handle for the collective builder.
	@param	flags	If flags == 1, copy this file to support folder of an app instead of to the collective in an app.
	@param	name	The name of the file.
	@param	path	The path of the file to add.
*/
void fileusage_addfile(void *w, long flags, char *name, short path);

void fileusage_addfilename(void *w, long flags, char *name);
void fileusage_addpathname(void *w, long flags, char *name);
void fileusage_copyfolder(void *w, char *name, long recursive);
void fileusage_makefolder(void *w, char *name);


// fileformat functions
void fileformat_stripsuffix(char *name, long *types, short numtypes);


#ifdef WIN_VERSION
// systext functions
void systext_mactoansi(char *sz);
void systext_ansitomac(char *sz);
#endif


#ifdef MAC_VERSION
long fontinfo_getencoding(long id);
long fontinfo_convert(t_object *x, char *src, long srclen, long encoding, char **out);
long fontinfo_reconvert(t_object *x, char *src, long srclen, long encoding, char **out);
void fontinfo_reconverthandle(t_object *x, char **h, long encoding);
#endif


long fontinfo_prefcheckencoding(void);

t_atom *atom_dynamic_start(const t_atom *static_array, long static_count, long request_count);
void atom_dynamic_end(const t_atom *static_array, t_atom *request_array);

#ifdef __cplusplus
}
#endif

#endif // _EXT_PROTO_H_

