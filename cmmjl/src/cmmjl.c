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

#include "cmmjl_errno.h"
#include "cmmjl_error.h"
#include "cmmjl_commonsymbols.h"
#include "cmmjl.h"

t_cmmjl_error *cmmjl_init(void){
	cmmjl_common_symbols_init();
	cmmjl_error_callback = cmmjl_default_error_handler;
	return NULL;
}

void cmmjl_post_gimme(void *x, t_symbol *msg, int argc, t_atom *argv){
	int i;
	t_atom a;
	PINFO("x = %p", x);
	PINFO("msg = %s", msg->s_name);
	PINFO("argc = %d", argc);
	PINFO("argv:");
	for(i = 0; i < argc; i++){
		a = argv[i];
		switch(a.a_type){
		case A_FLOAT:
			PINFO("A_FLOAT: %f", a.a_w.w_float);
			break;
		case A_LONG:
			PINFO("A_LONG: %d", a.a_w.w_long);
			break;
		case A_SYM:
			PINFO("A_SYM: %s", a.a_w.w_sym->s_name);
			break;
		default:
			CMMJL_ERROR(CMMJL_EBADTYPE, "Unknown type: %d", a.a_type);
		}
	}
}
