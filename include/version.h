#ifndef __CNMAT_VERSION_H__
#define __CNMAT_VERSION_H__ placeholder

#include "ext.h"

#include "current_version.h"
#include "report_compiler.h"


void version(void *x) 
{
	if(x){
		// called in response to the version message
		post("CNMAT Externals %s, compiled %s", CNMAT_EXT_VERSION, CNMAT_EXT_COMPILE_DATE);
		post("%s by %s.", NAME, AUTHORS);
		post("Copyright (c) " COPYRIGHT_YEARS " Regents of the University of California.  All rights reserved.");
		post_compiler();
	}else{
		if(!(gensym("CNMAT_did_post_copyright")->s_thing)){
			post("CNMAT Externals %s, compiled %s", CNMAT_EXT_VERSION, CNMAT_EXT_COMPILE_DATE);
			gensym("CNMAT_did_post_copyright")->s_thing = (void *)1;
		}
		post("%s by %s.", NAME, AUTHORS);
		post("Copyright (c) " COPYRIGHT_YEARS " Regents of the University of California.  All rights reserved.");
	}
}

#define version_post_copyright() version(NULL)

#endif
