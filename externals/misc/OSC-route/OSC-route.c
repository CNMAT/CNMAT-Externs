/*
Copyright (c) 1999,2000,01,02,03,04,05.  The Regents of the University of California (Regents).
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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: OSC-route ("OpenSoundControl route")
DESCRIPTION: Message dispatching through an <A HREF="http://www.cnmat.berkeley.edu/OSC">OpenSoundControl</A> address space.
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 1999,2000,01,02,03,04,05,06
SVN_REVISION: $LastChangedRevision$
VERSION 1.04: Allows #1 thru #9 as typed-in arguments
VERSION 1.05: Allows "list" messages as well as "message" messages.
VERSION 1.06: Extra outlet, "slash" argument, set method...
VERSION 1.07: Get's "allmessages" right with the extra outlet
VERSION 1.08: 68K maxL code resource named correctly, so usable in collective
VERSION 1.09: Allows special case prefix of /* that matches anything.
VERSION 1.10: Assitance now says "for prefix /%s (subaddress + arguments)"
VERSION 1.10.1: New versioning system
VERSION 1.10.2: Version info in this .c file
VERSION 1.10.3: Just a test of incrementing the version number
VERSION 1.11: Allows multi-level prefixes (e.g., "/foo/bar")
VERSION 1.12: Debugged "slash" argument problem introduced in 1.11
VERSION 1.13: Debugged crash problem introduced in 1.12
VERSION 1.13.1: Force Package Info Generation
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

 */
 
/*

 To-do:
 
 	Match a pattern against a pattern?
 	Declare outlet types / distinguish leaf nodes from other children
 		OSC-route /amp dB /freq Hz
 	
 	More sophisticated (2-pass?) allmessages scheme
*/


/* the required include files */
#include "version.h"
#include "ext.h"
#include "version.c"

#include "OSC-pattern-match.h"

/* structure definition of your object */
#define MAX_NUM 32

typedef struct OSCroute
{
	Object o_ob;				// required header
	int o_num;					// Number of address prefixes we store
	int o_complainmode;			// Do we print a message if no match?
	char *o_prefixes[MAX_NUM];	// Prefixes this object matches, with multiple levels as successive 
								// null-terminated strings
	int prefix_levels[MAX_NUM]; // Number of levels (i.e., of strings) for each prefix
	int prefix_sizes[MAX_NUM];	// # bytes in each o_prefixes[i], for freebytes
	void *o_outlets[MAX_NUM];
	void *o_otheroutlet;		// "none of the above" outlet
} OSCroute;

Symbol *ps_list, *ps_complain, *ps_emptySymbol, *ps_slash;

/* global necessary for 68K function macros to work */
fptr *FNS;

/* globalthat holds the class definition */
void *OSCroute_class;

/* prototypes  */

void OSCroute_free(OSCroute *x);
int RememberPrefix (OSCroute *x, char *prefixWithoutLeadingSlash);
Boolean MyPatternMatch (const char *pattern, const char *test);
void OSCroute_doanything(OSCroute *x, Symbol *s, short argc, Atom *argv);
static void OutputOSCArguments(OSCroute *x, int i, short argc, Atom *argv);
void OSCroute_anything(OSCroute *x, Symbol *s, short argc, Atom *argv);
void OSCroute_list(OSCroute *x, Symbol *s, short argc, Atom *argv);
void *OSCroute_new(Symbol *s, short argc, Atom *argv);
void OSCroute_version (OSCroute *x);
void OSCroute_assist (OSCroute *x, void *box, long msg, long arg, char *dstString);
void OSCroute_allmessages(OSCroute *x, Symbol *s, short argc, Atom *argv);
void OSCroute_set(OSCroute *x, long outlet, Symbol *s);
static char *NextSlashOrNull(char *p);
static int StrCopyUntilSlash(char *target, const char *source);
static int MyStrCopy(char *target, const char *source);
static int MyStrLen(const char *s);


/* initialization routine */

void main(fptr *f) {
	setup((t_messlist **)&OSCroute_class, (method)OSCroute_new, (method)OSCroute_free, 
		  (short)sizeof(OSCroute), 0L, A_GIMME, 0);
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
	
	post(NAME " object version " VERSION " by " AUTHORS ".");
	post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");

}



/* instance creation routine */


int RememberPrefix (OSCroute *x, char *prefixWithoutLeadingSlash) {
	char *s;
		
	if (x->o_num >= MAX_NUM) {
		error("OSC-route: too many outlets requested. (max %ld)", MAX_NUM);
		return 0;
	}

	x->prefix_sizes[x->o_num] = MyStrLen(prefixWithoutLeadingSlash)+1;
	x->o_prefixes[x->o_num] = getbytes(x->prefix_sizes[x->o_num]);
	if (x->o_prefixes[x->o_num] == 0) {
		error("Out of memory saving string %s", prefixWithoutLeadingSlash);
		return 0;
	}
	
	MyStrCopy(x->o_prefixes[x->o_num], prefixWithoutLeadingSlash);
	x->prefix_levels[x->o_num] = 1;

	// Convert all other slashes to nulls so as to
	// store each "part" (between slashes) as consecutive strings.
	for (s = x->o_prefixes[x->o_num]; *s != '\0'; ++s) {
		if (*s == '/') {
			*s = '\0';
			++(x->prefix_levels[x->o_num]);
		}
	}
				
	++(x->o_num);
	return 1;
}

void *OSCroute_new(Symbol *s, short argc, Atom *argv)
{
	OSCroute *x;
	int i, j, n;
	char *str;
	
	x = newobject(OSCroute_class);		// get memory for a new object & initialize

	x->o_complainmode = 0;
	x->o_num = 0;
	for (i = 0; i < argc; ++i) {

		if (argv[i].a_type == A_SYM) {
			if (argv[i].a_w.w_sym->s_name[0] == '/') {
				/* Now that's a nice prefix. */
				if (RememberPrefix(x, (argv[i].a_w.w_sym->s_name)+1) == 0) return 0;
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
					if (RememberPrefix(x, argv[i+1].a_w.w_sym->s_name) == 0) return 0;
				} else if (argv[i+1].a_type == A_LONG) {
					// Convert to a numeral.  Max ints are -2147483648 to 2147483647
					char string[12];
					sprintf(string, "%d", argv[i+1].a_w.w_long);
					if (RememberPrefix(x, string) == 0) return 0;
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
	
	/*
	for (i = 0; i<x->o_num; ++i) {
		post("prefix[%d] has %d levels:", i, x->prefix_levels[i]);
		for (j = 0, str=x->o_prefixes[i]; j<x->prefix_levels[i]; ++j, str=NextSlashOrNull(str)+1) {
			post("   %d: %s", j, str);
		}
	}
	*/
	
	/* Have to create the outlets in reverse order */
	
	x->o_otheroutlet = listout(x);
	
	for (i = x->o_num-1; i >= 0; --i) {
		x->o_outlets[i] = listout(x);
	}
		
	return (x);
}



void OSCroute_free(OSCroute *x) {
	int i;
	for (i = 0; i<x->o_num; ++i) {
		freebytes(x->o_prefixes[i], x->prefix_sizes[i]);
	}
}

void OSCroute_version (OSCroute *x) {
	post(NAME " Version " VERSION
		  ", by " AUTHORS ". Compiled " __TIME__ " " __DATE__);
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
			char *str;
			int j;
			int i = MyStrCopy(dstString, "for prefix ");
			for (j = 0, str=x->o_prefixes[arg]; j<x->prefix_levels[arg]; ++j, str=NextSlashOrNull(str)+1) {
				i += MyStrCopy(dstString+i, "/");
				i += MyStrCopy(dstString+i, str);
			}
			MyStrCopy(dstString+i, " (subaddress + arguments)");
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
	if (test[0] == '*' && test[1] == '\0') {
		/* This allows the special case of "OSC-route /*" to be an outlet
		   that matches anything; i.e., it always outputs the input with the first
		   level of the address stripped off. */
		return 1;
	} else {
		return PatternMatch(pattern, test);
	}
}


#define MAX_PREFIX_LEVELS 20  // e.g., "/a/b/c/d/e/f/ ... /t"
#define MAX_PREFIX_PART_LENGTH 50
void OSCroute_doanything(OSCroute *x, Symbol *s, short argc, Atom *argv) {
	char *pattern, *nextSlash, *str;
	int i,j,k;
	int matchedAnything;
	char patternParts[MAX_PREFIX_LEVELS][MAX_PREFIX_PART_LENGTH];
	int numPatternParts;
	
	// post("*** OSCroute_anything(s %s, argc %ld)", s->s_name, (long) argc);
	
	
	// Decompose incoming pattern into its parts
	pattern = s->s_name;
	if (pattern[0] != '/') {
		post("¥ OSC-route: invalid message pattern %s does not begin with /", s->s_name);
		return;
	}
	pattern++; // Skip opening slash

	numPatternParts = 0;
	while (numPatternParts < MAX_PREFIX_LEVELS) {
		pattern += StrCopyUntilSlash(patternParts[numPatternParts], pattern);
		++numPatternParts;
		if (*pattern == '\0') break;
		pattern++; // skip this slash
	}
	
	if (numPatternParts == MAX_PREFIX_LEVELS) {
		error("OSC-route: exceeded MAX_PREFIX_LEVELS");
		return;
	}
	
	/*
	post("Pattern has %d parts:", numPatternParts);
	for (i=0; i<numPatternParts; ++i) {
		post("  %s", patternParts[i]);
	}
	*/
	
	/* Go through all our prefixes looking for matches */
	matchedAnything = 0;
	
	for (i = 0; i < x->o_num; ++i) {
		str = x->o_prefixes[i];
		for (j = 0; j < x->prefix_levels[i] && j < numPatternParts; ++j) {
			if (MyPatternMatch(patternParts[j], str)) {
				// post("matched %s against %s", patternParts[j], str);
				str = NextSlashOrNull(str) + 1;
			} else {
				// post("didn't match %s against %s", patternParts[j], str);
				break;
			}
		}
		// post("i %d j %d numPatternParts %d previxLevels", i, j, numPatternParts, x->prefix_levels[i]);
		
		if (j == x->prefix_levels[i]) {
			// We matched all levels of the prefix...
			++matchedAnything;
			if (j == numPatternParts) {
				// ...against all parts of the pattern, so output the arguments
				OutputOSCArguments(x, i, argc, argv);
			} else {
				// ...against the beginning of the pattern, so output the rest of the pattern plus arguments
				char dstString[1000];
				Symbol *restOfPattern;
				k = 0;
				for (/* j already first unmatched pattern part */; j<numPatternParts; ++j) {
					k += MyStrCopy(dstString+k, "/");
					k += MyStrCopy(dstString+k, patternParts[j]);
				}
				restOfPattern = gensym(dstString);
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


static void OutputOSCArguments(OSCroute *x, int i, short argc, Atom *argv) {
	// We've matched  the entire OSC address pattern, so output the OSC arguments
	// as a Max list.
	 
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

static char *NextSlashOrNull(char *p) {
    while (*p != '/' && *p != '\0') {
	p++;
    }
    return p;
}

static int StrCopyUntilSlash(char *target, const char *source) {
	int i = 0;
	while (*source != '/' && *source != '\0') {
		*target = *source;
		++target;
		++source;
		++i;
	}
	*target = 0;
	return i;
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

static int MyStrLen(const char *s) {
	int i = 0;
	while (*s != '\0') {
		++s;
		++i;
	}
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
