#include "cmmjl_error.h"
#include "cmmjl_commonsymbols.h"
#include "cmmjl.h"

t_cmmjl_error cmmjl_init(void){
	cmmjl_common_symbols_init();
	return CMMJL_SUCCESS;
}

