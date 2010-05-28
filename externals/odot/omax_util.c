#include "omax_util.h"

void omax_util_oscMsg2MaxAtoms(t_osc_msg *msg, long *ac, t_atom **av){
	t_osc_msg m = *msg;
	*ac = osc_util_getArgCount(msg) + 1; 
	//t_atom a[n + 1], *ptr = a + 1;
	t_atom *ptr = *av;
	if(msg->address){
		atom_setsym(ptr, gensym(msg->address));
	}else{
		return;
	}
	ptr++;
	printf("%s: ac = %d\n", __PRETTY_FUNCTION__, *ac);
	while(osc_util_incrementArg(&m)){
		switch(*(m.typetags)){
		case 'i':
			atom_setlong(ptr++, ntoh32(*((int32_t *)msg->argv)));
			break;
		case 'f':
			{
				uint32_t l = ntoh32(*((int32_t *)msg->argv));
				atom_setfloat(ptr++, *((float *)&l));
			}
			break;
		case 'h':
		case 't':
			atom_setlong(ptr++, (long)ntoh64(*((int64_t *)msg->argv)));
			break;
		case 'd':
			{
				uint64_t l = ntoh64(*((int64_t *)msg->argv));
				atom_setfloat(ptr++, (float)*((double *)&l));
			}
			break;
		case 's':
		case 'S':
			atom_setsym(ptr++, gensym(msg->argv));
			break;
		case 'c':
			atom_setlong(ptr++, (long)*(msg->argv));
			break;
		case 'T':
			atom_setsym(ptr++, gensym("True"));
			break;
		case 'F':
			atom_setsym(ptr++, gensym("False"));
			break;
		case 'N':
			atom_setsym(ptr++, gensym("NIL"));
			break;
		case 'I':
			atom_setsym(ptr++, gensym("bang"));
			break;
		case 'b':
			{
				int j, n = osc_sizeof(*(msg->typetags), msg->argv);
				atom_setlong(ptr++, ntoh32(*((uint32_t *)(msg->argv))));
				for(j = 0; j < n; j++){
					atom_setlong(ptr++, (long)msg->argv[j]);
				}
			}
			break;
		}
	}
	//return atomarray_new(n + 1, a);
}
