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

Written by John MacCallum, The Center for New Music and
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

#include "cmmjl_commonsymbols.h"

t_cmmjl_common_symbols_table *_cmmjl_common_symbols;

static char *cmmjl_common_strings[] = {
	"OSCTimeTag",
	"FullPacket",
	"OSCBlob"
};

void cmmjl_common_symbols_init(void){
	_cmmjl_common_symbols = (t_cmmjl_common_symbols_table *)malloc(sizeof(t_cmmjl_common_symbols_table));
	if(_cmmjl_common_symbols == NULL){
		error("out of memory");
		return;
	}
	int n = sizeof(t_cmmjl_common_symbols_table) / sizeof(t_symbol *);
	int i;
	t_symbol **syms = (t_symbol **)_cmmjl_common_symbols;
	for(i = 0; i < n; i++){
		syms[i] = gensym(cmmjl_common_strings[i]);
	}

}
