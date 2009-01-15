/** @file /Users/johnmac/Development/cnmat/trunk/max/cmmjl/src/cmmjl_commonsymbols.h
	A set of commonly used predefined t_symbols.

	WARNING: THIS FILE IS GENERATED AUTOMATICALLY BY cmmjl_commonsymbols.pl.
	DO NOT EDIT.

*/

#ifndef __CMMJL_COMMONSYMBOLS_H__
#define __CMMJL_COMMONSYMBOLS_H__

#include "ext.h"

typedef struct _cmmjl_common_symols_table{
	t_symbol *s_OSCaddress;
	t_symbol *s_OSCTimeTag;
	t_symbol *s_FullPacket;
	t_symbol *s_OSCBlob;
} t_cmmjl_common_symbols_table;

#define ps_OSCaddress (_cmmjl_common_symbols->s_OSCaddress)
#define ps_OSCTimeTag (_cmmjl_common_symbols->s_OSCTimeTag)
#define ps_FullPacket (_cmmjl_common_symbols->s_FullPacket)
#define ps_OSCBlob (_cmmjl_common_symbols->s_OSCBlob)

/** The common symbols structure.  This shouldn't be accessed directly, but rather by using the symbols defined above. */
extern t_cmmjl_common_symbols_table *_cmmjl_common_symbols;

/** Initialize the symbol table by calling this. */
void cmmjl_common_symbols_init(void);

#endif
