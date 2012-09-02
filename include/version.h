#include "ext.h"
void version(void *x) {
	//post(NAME " Version " VERSION ", by " AUTHORS ".");
	if (x != 0) {
		// Not called from main()
		post("Compiled " __TIME__ " " __DATE__);
	}
	post("Copyright (c) " COPYRIGHT_YEARS " Regents of the University of California.  All rights reserved.");
}
#define version_post_copyright()
