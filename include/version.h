#ifndef __CNMAT_VERSION_H__
#define __CNMAT_VERSION_H__

#include "ext.h"

#include "current_version.h"
#include "report_compiler.h"


void version(void *x) {
	if(!(gensym("CNMAT_did_post_copyright")->s_thing)){
		post("CNMAT Externals v%s", CNMAT_EXT_VERSION);
		gensym("CNMAT_did_post_copyright")->s_thing = (t_object *)1;
	}
	post("%s Version %s, by %s.", NAME, CNMAT_EXT_VERSION, AUTHORS);
	post("Copyright (c) " COPYRIGHT_YEARS " Regents of the University of California.  All rights reserved.");
	if(x){
		// Not called from main()
		post_compiler();
	}
}

#define version_post_copyright() version(NULL)

#endif
