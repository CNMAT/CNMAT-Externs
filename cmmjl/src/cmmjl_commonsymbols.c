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
