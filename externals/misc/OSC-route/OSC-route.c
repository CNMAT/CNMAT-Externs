/*
Copyright (c) 1999, 2000, 2001.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

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

The OpenSound Control WWW page is 
    http://www.cnmat.berkeley.edu/OpenSoundControl

 OSC-route.c
 Max object for OSC-style dispatching
 
 To-do:
 
 	Match a pattern against a pattern?
 	Declare outlet types / distinguish leaf nodes from other children
 		OSC-route /amp dB /freq Hz
 	
 	More sophisticated (2-pass?) allmessages scheme
 
 */
 

#define OSC_ROUTE_VERSION "1.10"

/* Version 1.04: Allows #1 thru #9 as typed-in arguments
   Version 1.05: Allows "list" messages as well as "message" messages.
   Version 1.06: Extra outlet, "slash" argument, set method...
   Version 1.07: Get's "allmessages" right with the extra outlet
   Version 1.08: 68K maxL code resource named correctly, so usable in collective
   Version 1.09: Allows special case prefix of /* that matches anything.
   Version 1.10: Assitance now says "for prefix /%s (subaddress + arguments)"
*/


/* the required include files */
#include "ext.h"
#include "OSC-pattern-match.h"

/* structure definition of your object */
#define MAX_NUM 32

typedef struct OSCroute
{
	Object o_ob;				// required header
	int o_num;					// Number of address prefixes we store
	int o_complainmode;			// Do we print a message if no match?
	char *o_prefixes[MAX_NUM];
	void *o_outlets[MAX_NUM];
	void *o_otheroutlet;		// "none of the above" outlet
} OSCroute;

Symbol *ps_list, *ps_complain, *ps_emptySymbol, *ps_slash;

/* global necessary for 68K function macros to work */
fptr *FNS;

/* globalthat holds the class definition */
void *OSCroute_class;

/* prototypes  */

Boolean MyPatternMatch (const char *pattern, const char *test);
void OSCroute_doanything(OSCroute *x, Symbol *s, short argc, Atom *argv);
void OSCroute_anything(OSCroute *x, Symbol *s, short argc, Atom *argv);
void OSCroute_list(OSCroute *x, Symbol *s, short argc, Atom *argv);
void *OSCroute_new(Symbol *s, short argc, Atom *argv);
void OSCroute_version (OSCroute *x);
void OSCroute_assist (OSCroute *x, void *box, long msg, long arg, char *dstString);
void OSCroute_allmessages(OSCroute *x, Symbol *s, short argc, Atom *argv);
void OSCroute_set(OSCroute *x, long outlet, Symbol *s);
static char *NextSlashOrNull(char *p);
static void StrCopyUntilSlash(char *target, const char *source);


/* initialization routine */

void main(fptr *f)
{
	/* tell Max about your class. The cast to short is important for 68K */
	setup((t_messlist **)&OSCroute_class, (method)OSCroute_new,0L, (short)sizeof(OSCroute), 0L, A_GIMME, 0);
	/* bind your methods to symbols */
	addmess((method)OSCroute_anything, "anything", A_GIMME, 0);
	addmess((method)OSCroute_list, "list", A_GIMME, 0);
	addmess((method)OSCroute_assist, "assist", A_CANT, 0);
	addmess((method)OSCroute_version, "version", 0);
	addmess((method)OSCroute_allmessages, "allmessages", A_GIMME, 0);
	addmess((method)OSCroute_set, "set", A_LONG, A_SYM, 0);

	ps_list = gensym("list");
	ps_complain = gensym("complain");
	ps_slash = gensym("slash");
	ps_emptySymbol = gensym("");
	
	post("OSC-route object version " OSC_ROUTE_VERSION " by Matt Wright. ");
	post("Copyright © 1999,2000,01,02 Regents of the University of California. All Rights Reserved.");

}



/* instance creation routine */

void *OSCroute_new(Symbol *s, short argc, Atom *argv)
{
	OSCroute *x;
	int i, n;
	
	x = newobject(OSCroute_class);		// get memory for a new object & initialize

	x->o_complainmode = 0;
	x->o_num = 0;
	for (i = 0; i < argc; ++i) {
		if (x->o_num >= MAX_NUM) {
			post("¥ OSC-route: too many outlets requested. (max %ld)", MAX_NUM);
			return 0;
		}

		if (argv[i].a_type == A_SYM) {
			if (argv[i].a_w.w_sym->s_name[0] == '/') {
				/* Now that's a nice prefix */
				x->o_prefixes[x->o_num] = argv[i].a_w.w_sym->s_name + 1;
				++(x->o_num);
			} else if (argv[i].a_w.w_sym->s_name[0] == '#' &&
					   argv[i].a_w.w_sym->s_name[1] >= '1' &&
					   argv[i].a_w.w_sym->s_name[1] <= '9') {
				/* The Max programmer is trying to make a patch that will be
				   a subpatch with arguments.  We have to make an outlet for this
				   argument. */
				 x->o_prefixes[x->o_num] = "dummy";
				 ++(x->o_num);
			} else if (argv[i].a_w.w_sym == ps_complain) {
				x->o_complainmode = 1;
			} else if (argv[i].a_w.w_sym == ps_slash) {
				/* Take next argument after slash as if it were a "slashed" argument */
				if (i+1 >= argc) {
					post("¥ OSC-route: \"slash\" argument must be followed by another argument.");
					return 0;
				}
				
				if (argv[i+1].a_type == A_SYM) {
					x->o_prefixes[x->o_num] = argv[i+1].a_w.w_sym->s_name;
					++(x->o_num);
				} else if (argv[i+1].a_type == A_LONG) {
					// Convert to a numeral.  Max ints are -2147483648 to 2147483647
					char *string = getbytes(12);
					// I can't be bothered to plug this 12 byte memory leak
					if (string == 0) {
						post("¥ OSC-route: out of memory!");
						return 0;
					}
					sprintf(string, "%d", argv[i+1].a_w.w_long);
					x->o_prefixes[x->o_num] = string;
					++(x->o_num);
				} else {
					post("¥ OSC-route: unrecognized argument type after \"slash\".");
					return 0;
				}
				++i;		
			} else {
					post("¥ OSC-route: Unrecognized argument %s", argv[i].a_w.w_sym->s_name);
			}
		} else if (argv[i].a_type == A_LONG) {
			post("¥ OSC-route: int arguments are not OK.");
			return 0;
		} else if (argv[i].a_type == A_FLOAT) {
			post("¥ OSC-route: float arguments are not OK.");
			return 0;
		} else {
			post("¥ OSC-route: unrecognized argument type!");
			return 0;
		}
	}
	
	
	/* Have to create the outlets in reverse order */
	
	x->o_otheroutlet = listout(x);
	
	for (i = x->o_num-1; i >= 0; --i) {
		x->o_outlets[i] = listout(x);
	}
		
	return (x);
}


void OSCroute_version (OSCroute *x) {
	post("OSC-route Version " OSC_ROUTE_VERSION
		  ", by Matt Wright. Compiled " __TIME__ " " __DATE__);	
}

/* I don't know why these aren't defined in some Max #include file. */
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void OSCroute_assist (OSCroute *x, void *box, long msg, long arg, char *dstString) {	
	if (msg==ASSIST_INLET) {
		sprintf(dstString, "Incoming OSC messages");
	} else if (msg==ASSIST_OUTLET) {
		if (arg == x->o_num) {
			sprintf(dstString, "Full message if it was not matched");
		} else if (arg < 0 || arg > x->o_num) {
			post("¥ OSCroute_assist: No outlet corresponds to arg %ld!", arg);
		} else {
			sprintf(dstString, "for prefix /%s (subaddress + arguments)", x->o_prefixes[arg]);
		}
	} else {
		post("¥ OSCroute_assist: unrecognized message %ld", msg);
	}
}


void OSCroute_set(OSCroute *x, long outlet, Symbol *s) {	
	// outlet argument is 1-based.
	
	if (outlet <=0 || outlet > x->o_num) {
		post("¥ OSC-route: argument to set must be from one to the number of outlets.");
		goto bag;
	}
	
	if (s->s_name[0] == '/') {
		x->o_prefixes[outlet-1] = s->s_name + 1;
	} else {
		x->o_prefixes[outlet-1] = s->s_name;
	}	
		
bag:
	1+1;
}


void OSCroute_list(OSCroute *x, Symbol *s, short argc, Atom *argv) {
	if (argc > 0 && argv[0].a_type == A_SYM) {
		/* Ignore the fact that this is a "list" */
		OSCroute_doanything(x, argv[0].a_w.w_sym, argc-1, argv+1);
	} else {
		post("¥ OSC-route: invalid list beginning with a number");
	}
}
	
	
void OSCroute_anything(OSCroute *x, Symbol *s, short argc, Atom *argv) {
	OSCroute_doanything(x, s, argc, argv);
}
	
Boolean MyPatternMatch (const char *pattern, const char *test) {
	/* This allows the special case of "OSC-route /*" to be an outlet
	   that matches anything; i.e., it always outputs the input with the first
	   level of the address stripped off. */
	if (test[0] == '*' && test[1] == '\0') {
		return 1;
	} else {
		return PatternMatch(pattern, test);
	}
}

void OSCroute_doanything(OSCroute *x, Symbol *s, short argc, Atom *argv) {
	char *pattern, *nextSlash;
	int i;
	int matchedAnything;
	// post("*** OSCroute_anything(s %s, argc %ld)", s->s_name, (long) argc);
	
	pattern = s->s_name;
	if (pattern[0] != '/') {
		post("¥ OSC-route: invalid message pattern %s does not begin with /", s->s_name);
		return;
	}
	
	matchedAnything = 0;
	
	nextSlash = NextSlashOrNull(pattern+1);
	if (*nextSlash == '\0') {
		/* last level of the address, so we'll output the argument list */
		

#ifdef NULL_IS_DIFFERENT_FROM_BANG
		if (argc==0) {
			post("¥ OSC-route: why are you matching one level pattern %s with no args?",
				 pattern);
			return;
		}
#endif
		
		for (i = 0; i < x->o_num; ++i) {
			if (MyPatternMatch(pattern+1, x->o_prefixes[i])) {
				++matchedAnything;
				
				// I hate stupid Max lists with a special first element
				if (argc == 0) {
					outlet_bang(x->o_outlets[i]);
				} else if (argv[0].a_type == A_SYM) {
					// Promote the symbol that was argv[0] to the special symbol
					outlet_anything(x->o_outlets[i], argv[0].a_w.w_sym, argc-1, argv+1);
				} else if (argc > 1) {
					// Multiple arguments starting with a number, so naturally we have
					// to use a special function to output this "list", since it's what
					// Max originally meant by "list".
					outlet_list(x->o_outlets[i], 0L, argc, argv);
				} else {
					// There was only one argument, and it was a number, so we output it
					// not as a list
					if (argv[0].a_type == A_LONG) {
						outlet_int(x->o_outlets[i], argv[0].a_w.w_long);
					} else if (argv[0].a_type == A_FLOAT) {
						outlet_float(x->o_outlets[i], argv[0].a_w.w_float);
					} else {
						post("¥ OSC-route: unrecognized atom type!");
					}
				}
			}
		}
	} else {
		/* There's more address after this part, so our output list will begin with
		   the next slash.  */
		Symbol *restOfPattern = 0; /* avoid the gensym unless we have to output */
		char patternBegin[1000];
		
		
		/* Get the first level of the incoming pattern to match against all our prefixes */
		StrCopyUntilSlash(patternBegin, pattern+1);

		for (i = 0; i < x->o_num; ++i) {
			if (MyPatternMatch(patternBegin, x->o_prefixes[i])) {
				++matchedAnything;
				if (restOfPattern == 0) {
					restOfPattern = gensym(nextSlash);
				}
				outlet_anything(x->o_outlets[i], restOfPattern, argc, argv);
			}
		}
	}	
	
	
	if (!matchedAnything) {
		if (x->o_complainmode) {
			post("¥ OSC-route: pattern %s did not match any prefixes", pattern);
		}

		outlet_anything(x->o_otheroutlet, s, argc, argv);
	}
}

static char *NextSlashOrNull(char *p) {
    while (*p != '/' && *p != '\0') {
	p++;
    }
    return p;
}

static void StrCopyUntilSlash(char *target, const char *source) {
	while (*source != '/' && *source != '\0') {
		*target = *source;
		++target;
		++source;
	}
	*target = 0;
}

static int MyStrCopy(char *target, const char *source) {
	int i = 0;
	while (*source != '\0') {
		*target = *source;
		++target;
		++source;
		++i;
	}
	*target = 0;
	return i;
}

	
void OSCroute_allmessages(OSCroute *x, Symbol *s, short argc, Atom *argv) {
	int i;
	Symbol *prefixSymbol = 0;
	char prefixBuf[1000];
	char *endOfPrefix;
	Atom a[1];

	if (argc >= 1 && argv[0].a_type == A_SYM) {
		prefixSymbol = argv[0].a_w.w_sym;
		endOfPrefix = prefixBuf + MyStrCopy(prefixBuf, prefixSymbol->s_name);
	} else {
		prefixSymbol = ps_emptySymbol;
		prefixBuf[0] = '\0';
		endOfPrefix = prefixBuf;
	}
	
	*endOfPrefix = '/';
	++endOfPrefix;

	for (i = 0; i < x->o_num; ++i) {
		post("OSC:  %s/%s", prefixSymbol->s_name, x->o_prefixes[i]);
		MyStrCopy(endOfPrefix, x->o_prefixes[i]);
		SETSYM(a, gensym(prefixBuf));
		outlet_anything(x->o_outlets[i], s, 1, a);
	}
	
	// And now the "none of the above" outlet, which outputs messages at the same
	// level of the hierarchy as the *input* to this OSC-route object:
	SETSYM(a, prefixSymbol);
	outlet_anything(x->o_otheroutlet, s, 1, a);
}


/* The OSC-Kit calls OSCWarning() to print warning messages (encountered during pattern-matching
   of malformed OSC address patterns).  Here we define it to use Max's post() */

#include <stdio.h>
// extern int vsprintf(char *str, const char *format, va_list ap);

void OSCWarning(char *s, ...);

void OSCWarning(char *s, ...) {
	char warningMessage[1000];
	int n;
    va_list ap;
    
    sprintf(warningMessage, "OSC-route: Warning: ");
    n = 20; /* strlen(warningMessage); */
    
    va_start(ap, s);
    vsprintf(warningMessage+n, s, ap);
    va_end(ap);

	post(warningMessage);
}
