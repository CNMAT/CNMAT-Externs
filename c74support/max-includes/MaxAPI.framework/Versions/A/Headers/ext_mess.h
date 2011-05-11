/*************************/
/* Copyright 1988 IRCAM. */
/*************************/

#ifndef _EXT_MESS_H_
#define _EXT_MESS_H_

#ifdef __cplusplus
extern "C" {
#endif

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

/* mess.h -- define a symbol table and message-passing system.  */


/**	Function pointer type for generic methods.
	@ingroup datatypes
*/
typedef void *(*method)(void *, ...);


/**	Function pointer type for methods returning a long.
	@ingroup datatypes
*/
typedef long (*longmethod)(void *, ...);


/**	Function pointer type for a function with no arguments, returning a generic pointer.
	@ingroup datatypes
*/
typedef void *(*voidstarvoid)();


/**	Generic pointer type.
	@ingroup datatypes
*/
typedef char *t_ptr;
typedef char *ptr;


/**	Generic pointer-to-a-pointer type.
	@ingroup datatypes
*/
typedef char **t_handle;


#ifndef VPTR_TYPEDEF
/**	Void pointer type.
	@ingroup datatypes
*/
typedef void *t_vptr;
typedef void *vptr;

#define VPTR_TYPEDEF
#endif 


/** The symbol. 
	
	Note: You should <em>never</em> manipulate the s_name field of the #t_symbol directly!
	Doing so will corrupt Max's symbol table.  
	Instead, <em>always</em> use gensym() to get a symbol with the desired string 
	contents for the s_name field.

	@ingroup symbol	
*/
typedef struct symbol
{
	char *s_name;			///< name: a c-string
	struct object *s_thing;	///< possible binding to a t_object
} Symbol, t_symbol;

#define CAREFUL

/**	Magic number used to determine if memory pointed to by a #t_object* is valid.	
	@ingroup obj
*/
#define MAGIC 1758379419L	/* random (but odd) */

#ifdef  WIN_VERSION
/**	Returns true if a pointer is not a valid object. 
	@ingroup obj
*/
#define NOGOOD(x) ( IsBadReadPtr((void *)(x),sizeof(struct object)) || ((struct object *)(x))->o_magic != MAGIC )
#else
/**	Returns true if a pointer is not a valid object. 
	@ingroup obj
*/
#define NOGOOD(x) (((struct object *)x)->o_magic != MAGIC)
#endif


/**	Maximum number of arguments that can be passed as a typed-list rather than using #A_GIMME.  It is generally recommended to use #A_GIMME. 
	@ingroup obj
*/
#define MAXARG 7


/** A list of symbols and their corresponding methods,
 	complete with typechecking information.	
	@ingroup obj
*/
typedef struct messlist
{
	struct symbol *m_sym;		///< Name of the message
	method m_fun;				///< Method associated with the message
	char m_type[MAXARG + 1];	///< Argument type information
} Messlist, t_messlist;


/**	The tiny object structure sits at the head of any object to which you may
 	pass messages (and which you may feed to freeobject()).
	In general, you should use #t_object instead.
	@ingroup obj
*/
typedef struct tinyobject
{
	struct messlist *t_messlist;	///< list of messages and methods 
									// (also used as freelist link)
#ifdef CAREFUL
	long t_magic;					///< magic number
#endif
} Tinyobject, t_tinyobject;


/** The structure for the head of any object which wants to have inlets or outlets,
	or support attributes.
	@ingroup obj
*/
typedef struct object
{
	struct messlist *o_messlist;	///<  list of messages and methods. The -1 entry of the message list of an object contains a pointer to its #t_class entry.
									// (also used as freelist link)
#ifdef CAREFUL
	long o_magic;					///< magic number
#endif
	struct inlet *o_inlet;			///<  list of inlets
	struct outlet *o_outlet;		///<  list of outlets
} Object, t_object;


/** 
	The data structure for a Max class. This struct is provided for debugging convenience, 
	but should be considered opaque and is subject to change without notice.
					
	@ingroup class
*/
typedef struct maxclass
{
	struct symbol *c_sym;			///< symbol giving name of class
	struct object **c_freelist;		// linked list of free ones
	method c_freefun;				// function to call when freeing
	short c_size;					// size of an instance
	char c_tiny;					// flag indicating tiny header
	char c_noinlet;					// flag indicating no first inlet for patcher
	struct symbol *c_filename;		///< name of file associated with this class
	t_messlist *c_newmess;			// constructor method/type information
	method c_menufun;				// function to call when creating from object pallette (default constructor)
	long c_flags;					// class flags used to determine context in which class may be used
	long c_messcount;				// size of messlist array
	t_messlist *c_messlist;			// messlist arrray
	void *c_attributes;				// t_hashtab object
	void *c_extra;					// t_hashtab object
	long c_obexoffset;				// if non zero, object struct contains obex pointer at specified byte offset
} Maxclass, t_class;


/** Class flags. If not box or polyglot, class is only accessible in C via known interface
	@ingroup class
*/
typedef enum {
	CLASS_FLAG_BOX =					0x00000001L,	///< for use in a patcher
	CLASS_FLAG_POLYGLOT =				0x00000002L,	///< for use by any text language (c/js/java/etc)
	CLASS_FLAG_NEWDICTIONARY =			0x00000004L,	///< dictionary based constructor
	CLASS_FLAG_REGISTERED =				0x00000008L,	///< for backward compatible messlist implementation (once reg'd can't grow)
	CLASS_FLAG_UIOBJECT =				0x00000010L,	///< for objects that don't go inside a newobj box. 
	CLASS_FLAG_ALIAS =					0x00000020L,	///< for classes that are just copies of some other class (i.e. del is a copy of delay)
	CLASS_FLAG_SCHED_PURGE =			0x00000040L,	///< for classes that have called clock_new() or qelem_new() (don't need to set this yourself)
	CLASS_FLAG_DO_NOT_PARSE_ATTR_ARGS =	0x00000080L, 	///< override dictionary based constructor attr arg parsing
	CLASS_FLAG_NOATTRIBUTES =			0x00010000L,	///< for efficiency
	CLASS_FLAG_OWNATTRIBUTES =			0x00020000L		///< for classes which support a custom attr interface (e.g. jitter)
} e_max_class_flags;


/** the list of officially recognized types, including pseudotypes for
	commas and semicolons.  Used in two places: 1. the reader, when it reads
	a string, returns long, float, sym, comma, semi, or dollar;
	and 2. each object method comes with an array of them saying what types
	it needs, from among long, float, sym, obj, gimme, and cant.
	
	@ingroup atom
	
	@remark	While these values are defined in an enum, you should use a long to represent the value.
			Using the enum type creates ambiguity in struct size and is subject to various inconsistent 
			compiler settings.
*/
typedef enum {
	A_NOTHING = 0,	///< no type, thus no atom
	A_LONG,			///< long integer
	A_FLOAT,		///< 32-bit float
	A_SYM,			///< t_symbol pointer
	A_OBJ,			///< t_object pointer (for argtype lists; passes the value of sym)
	A_DEFLONG,		///< long but defaults to zero
	A_DEFFLOAT,		///< float, but defaults to zero
	A_DEFSYM,		///< symbol, defaults to ""
	A_GIMME,		///< request that args be passed as an array, the routine will check the types itself.
	A_CANT,			///< cannot typecheck args
	A_SEMI,			///< semicolon
	A_COMMA,		///< comma
	A_DOLLAR,		///< dollar
	A_DOLLSYM,		///< dollar
	A_GIMMEBACK,	///< request that args be passed as an array, the routine will check the types itself. can return atom value in final atom ptr arg. function returns long error code 0 = no err. see gimmeback_meth typedef

	A_DEFER	=		0x41,	///< A special signature for declaring methods. This is like A_GIMME, but the call is deferred.
	A_USURP =		0x42,	///< A special signature for declaring methods. This is like A_GIMME, but the call is deferred and multiple calls within one servicing of the queue are filtered down to one call.
	A_DEFER_LOW =	0x43,	///< A special signature for declaring methods. This is like A_GIMME, but the call is deferref to the back of the queue.
	A_USURP_LOW =	0x44	///< A special signature for declaring methods. This is like A_GIMME, but the call is deferred to the back of the queue and multiple calls within one servicing of the queue are filtered down to one call.
} e_max_atomtypes;


/**	Defines the largest possible string size for an atom
	@ingroup atom */
#define ATOM_MAX_STRLEN		(32768)

typedef float t_atom_float;			// the type that an A_FLOAT represents

/** Union for packing any of the datum defined in #e_max_atomtypes.
	@ingroup atom
*/
union word		
{
	long w_long;			///< long integer
	t_atom_float w_float;	///< 32-bit float
	struct symbol *w_sym;	///< pointer to a symbol in the Max symbol table
	struct object *w_obj;	///< pointer to a #t_object or other generic pointer
};


/** An atom is a typed datum.
	@ingroup atom
*/
typedef struct atom	
{
	short a_type;			///< a value as defined in #e_max_atomtypes
	union word a_w;			///< the actual data
} Atom, t_atom;



/**	Function pointer type for methods with no arguments.
	@ingroup datatypes
*/
typedef void *(*zero_meth)(void *x);


/**	Function pointer type for methods with a single argument.
	@ingroup datatypes
*/
typedef void *(*one_meth)(void *x, void *z);


/**	Function pointer type for methods with two arguments.
	@ingroup datatypes
*/
typedef void *(*two_meth)(void *x, void *z, void *a);


/**	Function pointer type for methods that pass back a result value through the last parameter as a #t_atom, 
	and return an error.
	@ingroup datatypes
*/
typedef long *(*gimmeback_meth)(void *x, t_symbol *s, long ac, t_atom *av, t_atom *rv);



// legacy macros...

#define BAG(s) {error(s); return (0);}
#define BAG2(s,t) {error(s,t); return (0);}
#define ob_messlist(x) (((struct object *)(x))->o_messlist - 1)
#define ob_class(x) ((struct maxclass *) \
	(((struct object *)(x))->o_messlist[-1].m_sym))
#define ob_name(x) (ob_class(x)->c_sym->s_name)
#define ob_sym(x) (ob_class(x)->c_sym)
#define ob_filename(x) (ob_class(x)->c_filename->s_name)
#define ob_filesym(x) (ob_class(x)->c_filename)
#define mess0(x, y)  (*((zero_meth)(getfn(x, y))))(x)
#define mess1(x, y, z1) (*((one_meth)(getfn(x, y))))(x, z1)
#define mess2(x, y, z1,z2) (*((two_meth)(getfn(x, y))))(x, z1,z2)
#define mess3(x, y, z1,z2,z3) (*(getfn(x, y)))(x, z1,z2,z3)
#define mess4(x, y, z1,z2,z3,z4) (*(getfn(x, y)))(x, z1,z2,z3,z4)
#define mess5(x, y, z1,z2,z3,z4,z5) (*(getfn(x, y)))(x, z1,z2,z3,z4,z5)
#define NIL ((void *)0)
#define SETCOMMA(ap) ((ap)->a_type = A_COMMA)
#define SETSEMI(ap) ((ap)->a_type = A_SEMI)
#define SETSYM(ap, x) ((ap)->a_type = A_SYM, (ap)->a_w.w_sym = (x))
#define SETOBJ(ap, x) ((ap)->a_type = A_OBJ, (ap)->a_w.w_obj = (x))
#define SETLONG(ap, x) ((ap)->a_type = A_LONG, (ap)->a_w.w_long = (x))
#define SETFLOAT(ap, x) ((ap)->a_type = A_FLOAT, (ap)->a_w.w_float = (x))
#define SETDOLLAR(ap, x) ((ap)->a_type = A_DOLLAR, (ap)->a_w.w_long = (x))


#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef __cplusplus
}
#endif

#endif /* _EXT_MESS_H_ */
