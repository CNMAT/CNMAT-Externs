#include "oio_obj.h"
#include "oio_mem.h"
#include "oio_hid.h"

t_oio *oio_obj_alloc(void){
	t_oio *oio = (t_oio *)oio_mem_alloc(1, sizeof(t_oio));
	oio_hid_alloc(oio);
	return oio;
}
