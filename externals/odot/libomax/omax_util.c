#include <inttypes.h>
#include <Carbon.h>
#include <CoreServices.h>
#include "omax_util.h"
#include "osc.h"

#define __ODOT_PROFILE__
#include "profile.h"

#ifdef __USE_OMAX_ATOM_GETTERS__
inline long omax_atom_getlong(t_atom *ap){
	switch(ap->a_type){
	case A_LONG:
		return ap->a_w.w_long;
	case A_FLOAT:
		return (long)(ap->a_w.w_float);
	default:
		return 0;
		break;
	}
}

inline float omax_atom_getfloat(t_atom *ap){
	switch(ap->a_type){
	case A_LONG:
		return (float)(ap->a_w.w_long);
	case A_FLOAT:
		return ap->a_w.w_float;
	default:
		return 0;
		break;
	}
}

inline t_symbol *omax_atom_getsym(t_atom *ap){
	switch(ap->a_type){
	case A_LONG:
		return NULL;
	case A_FLOAT:
		return NULL;
	default:
		return ap->a_w.w_sym;
		break;
	}
}
#endif

void atom64_setlong(t_atom64 *a, int32_t l){
	if(!a){
		return;
	}
	a->a64_word.w_long = l;
	a->type = A64_LONG;
}

void atom64_setfloat(t_atom64 *a, double d){
	if(!a){
		return;
	}
	a->a64_word.w_float = d;
	a->type = A64_FLOAT;
}

void atom64_setsym(t_atom64 *a, t_symbol *s){
	if(!a){
		return;
	}
	a->a64_word.w_sym = s;
	a->type = A64_SYM;
}

void atom64_setptr(t_atom64 *a, void *p){
	if(!a){
		return;
	}
	a->a64_word.w_ptr = p;
	a->type = A64_PTR;
}

int32_t atom64_getlong(t_atom64 *a){
	switch(a->type){
	case A64_LONG:
		return a->a64_word.w_long;
	case A64_FLOAT:
		return (int32_t)(a->a64_word.w_float);
	default:
		return 0;
	}
}

double atom64_getfloat(t_atom64 *a){
	switch(a->type){
	case A64_LONG:{
		return (double)(a->a64_word.w_long);
	}
	case A64_FLOAT:
		return a->a64_word.w_float;
	default:
		return 0;
	}
}

t_symbol *atom64_getsym(t_atom64 *a){
	switch(a->type){
	case A64_LONG:
		{
			char buf[64];
			sprintf(buf, "%ld", (long int)(a->a64_word.w_long));
			return gensym(buf);
		}
	case A64_FLOAT:
		{
			char buf[64];
			sprintf(buf, "%f", a->a64_word.w_float);
			return gensym(buf);
		}
	case A64_SYM:
		return a->a64_word.w_sym;
	}
}

void *atom64_getptr(t_atom64 *a){

}

typedef struct _context{
	t_object *ob;
	t_symbol *osc_classname;
	t_symbol *classname;
	char *buf;
	int bufpos;
	int should_output_state;
} t_context;
	
typedef struct _charbuffer{
	char *buf;
	int bufpos;
	int buflen;
} t_charbuffer;

typedef struct _schemalist{
	struct _charbuffer *addresses;
	int address_buf_pos;
	int address_buf_len;
	int nbytes; // the sum of the addresses[i].bufpos
} t_schemalist;

static t_symbol *ps_oscschemalist, *ps_oscioreport;

t_class *omax_class_new(char *name, method ctor, method dtor, size_t structsize, size_t osc_ob_offset, method fullpacket_method){
	method fp;
	if(fullpacket_method){
		fp = fullpacket_method;
	}else{
		fp = (method)omax_FullPacket;
	}
	t_class *c = class_new(name, ctor, dtor, structsize, 0L, A_GIMME, 0);
	t_hashtab *ht = hashtab_new(0);
	omax_class_makeHashtabName(name)->s_thing = (t_object *)ht;
	hashtab_store(ht, gensym("cnmat_internal_osc_ob_offset"), (t_object *)osc_ob_offset);
	class_addmethod(c, fp, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)omax_tellmeeverything, "tellmeeverything", 0);
	//OMAX_ADDMETHOD(c, (method)omax_tellmeeverything, "tellmeeverything", 0);
	//OMAX_ADDMETHOD(c, (method)omax_probe, "probe", 0);
	OMAX_ADDMETHOD_OSCNAME(c, (method)omax_schemaList, "oscschemalist", "/osc/schema/list", 0);
	OMAX_ADDMETHOD_OSCNAME(c, (method)omax_object_ioReport, "oscioreport", "/osc/io/report", A_GIMME, 0);
	class_addmethod(c, (method)omax_notify, "notify", A_CANT, 0);

	ps_oscschemalist = gensym("/osc/schema/list");
	ps_oscioreport = gensym("/osc/io/report");

	return c;
}

void omax_init(t_object *x){
	t_omax *osc = omax_object_getOmaxObj(x);
	if(!osc){
		return;
	}
	osc->continuation_outlet = outlet_new(x, "FullPacket");
	osc->info_outlet = outlet_new(x, "FullPacket");
	object_attach_byptr_register(x, x, CLASS_BOX);
}

void omax_addnotification(t_class *c, method fp){
	t_symbol *classname = class_nameget(c);
	if(!classname){
		return;
	}

	t_hashtab *ht = omax_class_getHashtab(classname->s_name);
	if(!ht){
		return;
	}
	hashtab_store(ht, gensym("cnmat_internal_osc_notification_function"), (t_object *)fp);
}

void omax_FullPacket(t_object *ob, long len, long ptr){
	t_symbol *classname = object_classname(ob);
	if(!classname){
		// if we can't get our classname, there's no way we can match anything
		return;
	}
	char buf[len];
	sprintf(buf, "/%s", classname->s_name);
	t_context c;
	c.ob = ob;
	c.classname = classname;
	c.osc_classname = gensym(buf);

	memset(buf, '\0', len);
	c.buf = buf;
	osc_bundle_setBundleID(buf);
	c.bufpos = OSC_HEADER_SIZE;
	c.should_output_state = 0;
	t_osc_err e = osc_bundle_getMessagesWithCallback(len, (char *)ptr, omax_FullPacketCallback, (void *)(&c));
	if(e){
		object_error(ob, "%s (%d)", osc_error_string(e), e);
		return;
	}
	if(c.bufpos > OSC_HEADER_SIZE){
		void *cout = omax_object_getContinuationOutlet(ob);
		if(cout){
			t_atom out[2];
			atom_setlong(out, c.bufpos);
			atom_setlong(out + 1, (long)(c.buf));
			outlet_anything(cout, gensym("FullPacket"), 2, out);
		}
	}
	if(c.should_output_state > 0){
		omax_outputState(ob);
	}
}

void omax_FullPacketCallback(t_osc_msg msg, void *context){
	t_context *c = (t_context *)context;
	int ao, po;
	//if(osc_match(msg.address, c->osc_classname->s_name, &po, &ao)){
	if(1){
		// the first part of the address matches our class name, so this message is meant for us
		// now iterate through all of our message names and see if we get a match
		t_hashtab *ht = omax_class_getHashtab(c->classname->s_name);
		if(!ht){
			// oops...
			return;
		}
		long nkeys;
		t_symbol **keys = NULL;		
		hashtab_getkeys(ht, &nkeys, &keys);		
		int i;
		int matched = 0;
		for(i = 0; i < nkeys; i++){
			int r = osc_match(msg.address, keys[i]->s_name, &po, &ao);
			if(r == (OSC_MATCH_ADDRESS_COMPLETE | OSC_MATCH_PATTERN_COMPLETE)){
				matched++;
				// match
				// t_max_err object_method_typed(void *x, t_symbol *s, long ac, t_atom *av, t_atom *rv);
				t_atom rv;
				t_omax_method *m = NULL;
				long argc = 0;
				hashtab_lookup(ht, keys[i], (t_object **)(&m));
				if(m){
					switch(m->type){
					case OMAX_METHOD:
						{
							t_atom argv[msg.argc + 1];
							omax_util_oscMsg2MaxAtoms(&msg, &argc, argv);
							object_method_typedfun(c->ob, (t_messlist *)(m->thing), ((t_messlist *)(m->thing))->m_sym, argc - 1, argv + 1, &rv);
						}
						break;
					case OMAX_PARAMETER:
						{
							t_object *attr = NULL;
							long get;
							method f = object_attr_method(c->ob, m->sym, (void **)(&attr), &get);
							t_atom argv[msg.argc + 1];
							omax_util_oscMsg2MaxAtoms(&msg, &argc, argv);
							f(c->ob, attr, msg.argc, argv + 1);
							c->should_output_state = 1;
						}
						break;
					}
				}
			}
		}
		if(!matched){
			memcpy(c->buf + c->bufpos, msg.address - 4, msg.size + 4);
			c->bufpos += (msg.size + 4);
		}
	}else{
		memcpy(c->buf + c->bufpos, msg.address - 4, msg.size + 4);
		c->bufpos += (msg.size + 4);
	}
	
}

void omax_class_addToSchemaList(t_class *c, const char *address){
	t_symbol *classname = class_nameget(c);
	if(!classname){
		return;
	}
	t_hashtab *ht = omax_class_getHashtab(classname->s_name);
	if(!ht){
		return;
	}
	t_schemalist *sl = NULL;
	hashtab_lookup(ht, gensym("cnmat_internal_osc_schema_list"), (t_object **)(&sl));
	if(!sl){
		sl = (t_schemalist *)sysmem_newptr(sizeof(t_schemalist));
		sl->address_buf_len = 64;
		sl->addresses = (t_charbuffer *)sysmem_newptr(sl->address_buf_len * sizeof(t_charbuffer));
		sl->address_buf_pos = 0;
		sl->nbytes = 0;
		hashtab_store(ht, gensym("cnmat_internal_osc_schema_list"), (t_object *)sl);
	}
	int len = strlen(address);
	len++;
	while(len % 4){
		len++;
	}
	if(sl->address_buf_pos >= sl->address_buf_len){
		sl->addresses = (t_charbuffer *)sysmem_resizeptr(sl->addresses, sizeof(t_charbuffer) * (sl->address_buf_len + 16));
		if(!sl->addresses){
			return;
		}
		sl->address_buf_len += 16;
	}
	t_charbuffer *cb = sl->addresses + sl->address_buf_pos;
	cb->buf = (char *)sysmem_newptr(len);
	memset(cb->buf, '\0', len);
	strcpy(cb->buf, address);
	cb->bufpos = len;
	cb->buflen = len;
	sl->address_buf_pos++;
	sl->nbytes += len;
}

void omax_schemaList(t_object *ob, t_symbol *msg, int argc, t_atom *argv){
	t_symbol *classname = object_classname(ob);
	if(!classname){
		return;
	}

	t_hashtab *ht = omax_class_getHashtab(classname->s_name);
	if(!ht){
		return;
	}
	t_schemalist *sl = NULL;
	hashtab_lookup(ht, gensym("cnmat_internal_osc_schema_list"), (t_object **)&sl);
	if(!sl){
		return;
	}
	int typetaglen = (sl->address_buf_pos + 2);
	while(typetaglen % 4){
		typetaglen++;
	}

	char *address = "/osc/schema/list";
	int addresslen = 20;
	
	char buf[sl->nbytes + OSC_HEADER_SIZE + 4 + typetaglen + addresslen];
	// need to add space for timetag
	memset(buf, '\0', sl->nbytes + OSC_HEADER_SIZE + 4 + typetaglen);

	char *bufptr = buf + OSC_HEADER_SIZE;
	*((uint32_t *)bufptr) = hton32(sl->nbytes + typetaglen + addresslen);
	bufptr += 4;
	strncpy(bufptr, address, 16);
	bufptr += addresslen;
	*bufptr = ',';
	memset(bufptr + 1, 's', sl->address_buf_pos);
	bufptr += typetaglen;
	int i;
	for(i = 0; i < sl->address_buf_pos; i++){
		memcpy(bufptr, sl->addresses[i].buf, sl->addresses[i].bufpos);
		bufptr += sl->addresses[i].bufpos;
	}
	osc_bundle_setBundleID(buf);
	// put timestamp in place...

	/*	
	long nkeys = 0;
	t_symbol **keys = NULL;
	hashtab_getkeys(ht, &nkeys, &keys);
	int buflen = 128;
	char *buf = sysmem_newptr(buflen);
	memset(buf, '\0', buflen);
	int bufpos = 0;
	osc_bundle_setBundleID(buf);
	bufpos += OSC_HEADER_SIZE;
	int i;
	for(i = 0; i < nkeys; i++){
		if(osc_error_validateAddress(keys[i]->s_name)){
			continue;
		}
		t_omax_method *m = NULL;
		hashtab_lookup(ht, keys[i], (t_object **)(&m));
		if(!m){
			continue;
		}

		if(m->type == OMAX_PARAMETER){
			t_object *attr = object_attr_get(ob, m->sym);
			long argc = 0;
			t_atom *argv = NULL;
			//m->m.m_fun(ob, attr, &argc, &argv);
			char getter[128];
			sprintf(getter, "get%s", m->sym->s_name);
			long get;
			method f = object_attr_method(ob, gensym(getter), (void **)(&attr), &get);
			if(f){
				f(ob, attr, &argc, &argv);
				if(argv){
					char address[128];
					sprintf(address, "/parameter/%s", m->sym->s_name);
					int msglen = omax_util_get_bundle_size_for_atoms(gensym(address), argc, argv);
					if(msglen + bufpos >= buflen){
						buf = sysmem_resizeptr(buf, buflen + msglen);
						if(!buf){
							object_error(ob, "out of memory!");
							return;
						}
					}
					bufpos += omax_util_encode_atoms(buf + bufpos, gensym(address), argc, argv);
					sysmem_freeptr(argv);
				}
			}
		}else

		{
			int msglen = omax_util_get_bundle_size_for_atoms(keys[i], 0, NULL);
			if(msglen + bufpos >= buflen){
				buf = sysmem_resizeptr(buf, buflen + msglen);
				if(!buf){
					object_error(ob, "out of memory!");
					return;
				}
				memset(buf + buflen, '\0', msglen);
				buflen += msglen;
			}
			char methodname[128];
			sprintf(methodname, "/method/%s", m->sym->s_name);
			bufpos += omax_util_encode_atoms(buf + bufpos, gensym(methodname), 0, NULL);
		}
	}
	*/

	void *infoout = omax_object_getInfoOutlet(ob);
	if(infoout){
		t_atom out[2];
		atom_setlong(out, bufptr - buf);
		atom_setlong(out + 1, (long)buf);
		outlet_anything(infoout, gensym("FullPacket"), 2, out);
	}
}

void omax_object_ioReport(t_object *x, t_symbol *msg, int argc, t_atom *argv){
	int buflen;
	char *buf = NULL;

	if(argc == 0){
		t_atom a;
		atom_setsym(&a, gensym("/*"));
		omax_object_createIOReport(x, msg, 1, &a, &buflen, &buf);
	}else{
		omax_object_createIOReport(x, msg, argc, argv, &buflen, &buf);
	}
	if(!buf){
		return;
	}
	char bundle[buflen + OSC_HEADER_SIZE];
	osc_bundle_setBundleID(bundle);
	memcpy(bundle + OSC_HEADER_SIZE, buf, buflen);
	void *outlet = omax_object_getInfoOutlet(x);
	if(outlet){
		t_atom out[2];
		atom_setlong(out, buflen + OSC_HEADER_SIZE);
		atom_setlong(out + 1, (long)bundle);
		outlet_anything(outlet, gensym("FullPacket"), 2, out);
	}
	if(buf){
		sysmem_freeptr(buf);
	}
}

void omax_object_createIOReport(t_object *x, t_symbol *msg, int argc, t_atom *argv, int *buflen, char **buf){
	t_symbol *classname = object_classname(x);
	if(!classname){
		return;
	}
	t_hashtab *ht = omax_class_getHashtab(classname->s_name);
	if(!ht){
		return;
	}
	long nkeys = 0;
	t_symbol **keys = NULL;
	hashtab_getkeys(ht, &nkeys, &keys);

	int len = 0;
	int pos = 0;
	if(!(*buf)){
		len = 128;
		*buf = (char *)sysmem_newptr(len);
	}else{
		len = *buflen;
	}
	char *bufptr = *buf;
	memset(bufptr, '\0', len);

	int i;
	for(i = 0; i < nkeys; i++){
		if(!osc_error_validateAddress(keys[i]->s_name)){
			int j;

			for(j = 0; j < argc; j++){
				t_atom *a = argv + j;
				if(atom_gettype(a) == A_SYM){
					int ret = 0;
					int ao, po;
					if(atom_getsym(a) == gensym("/*")){
						ret = OSC_MATCH_ADDRESS_COMPLETE;
					}else{
						ret = osc_match(atom_getsym(a)->s_name, keys[i]->s_name, &po, &ao);
					}
					if(ret && OSC_MATCH_ADDRESS_COMPLETE){
						t_omax_method *m = NULL;
						hashtab_lookup(ht, keys[i], (t_object **)(&m));
						if(!m){
							continue;
						}
						if(m->type == OMAX_PARAMETER){
							t_object *attr = object_attr_get(x, m->sym);
							long argc = 0;
							t_atom *argv = NULL;
							//m->m.m_fun(ob, attr, &argc, &argv);
							char getter[128];
							sprintf(getter, "get%s", m->sym->s_name);
							long get;
							method f = object_attr_method(x, gensym(getter), (void **)(&attr), &get);
							if(f){
								f(x, attr, &argc, &argv);
								if(argv){
									char address[128];
									sprintf(address, "/%s", m->sym->s_name);
									t_atom a[argc + 1];
									atom_setsym(a, gensym(address));
									memcpy(a + 1, argv, argc * sizeof(t_atom));
									int msglen = omax_util_get_bundle_size_for_atoms(ps_oscioreport, argc + 1, a);
									if(msglen + pos >= len){
										bufptr = sysmem_resizeptr(bufptr, len + msglen);
										if(!bufptr){
											object_error(x, "out of memory!");
											return;
										}
										len += msglen;
									}
									pos += omax_util_encode_atoms(bufptr + pos, ps_oscioreport, argc + 1, a);
									sysmem_freeptr(argv);
								}
							}
						}
					}
				}
			}
		}
	}
	*buflen = pos;
}

void omax_outputState(t_object *x){
	t_symbol *classname = object_classname(x);
	if(!classname){
		return;
	}
	t_hashtab *ht = omax_class_getHashtab(classname->s_name);
	if(!ht){
		return;
	}
	long nkeys = 0;
	t_symbol **keys = NULL;
	hashtab_getkeys(ht, &nkeys, &keys);

	int len = 128;
	char *buf = (char *)sysmem_newptr(len);

	char *bufptr = buf;
	memset(bufptr, '\0', len);
	osc_bundle_setBundleID(bufptr);
	bufptr += OSC_HEADER_SIZE;

	int i;
	for(i = 0; i < nkeys; i++){
		if(osc_error_validateAddress(keys[i]->s_name)){
			continue;
		}

		t_omax_method *m = NULL;
		hashtab_lookup(ht, keys[i], (t_object **)(&m));
		if(!m){
			continue;
		}
		if(m->type == OMAX_PARAMETER){
			t_object *attr = object_attr_get(x, m->sym);
			long argc = 0;
			t_atom *argv = NULL;
			//m->m.m_fun(ob, attr, &argc, &argv);
			char getter[128];
			sprintf(getter, "get%s", m->sym->s_name);
			long get;
			method f = object_attr_method(x, gensym(getter), (void **)(&attr), &get);
			if(f){
				f(x, attr, &argc, &argv);
				if(argv){
					char address[128];
					sprintf(address, "/%s", m->sym->s_name);
					t_symbol *addresssym = gensym(address);
					int msglen = omax_util_get_bundle_size_for_atoms(addresssym, argc, argv);
					if(msglen + (bufptr - buf) >= len){
						bufptr = sysmem_resizeptr(bufptr, len + msglen);
						if(!bufptr){
							object_error(x, "out of memory!");
							return;
						}
						len += msglen;
					}
					bufptr += omax_util_encode_atoms(bufptr, addresssym, argc, argv);
					sysmem_freeptr(argv);
				}
			}
		}
	}
	void *outlet = omax_object_getInfoOutlet(x);
	if(outlet){
		t_atom out[2];
		atom_setlong(out, bufptr - buf);
		atom_setlong(out + 1, (long)buf);
		outlet_anything(outlet, gensym("FullPacket"), 2, out);
	}
}

t_max_err omax_notify(t_object *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

	if (msg == gensym("attr_modified")) {
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		omax_outputState(x);
	}
	t_max_err e = MAX_ERR_NONE;
	// big old cast to make an annoying compiler warning go away...
	t_max_err (*f)(t_object*,t_symbol*,t_symbol*,void*,void*) =
		(t_max_err (*)(t_object*,t_symbol*,t_symbol*,void*,void*))omax_object_getNotificationCallback(x);
	if(f){
		e = f(x, s, msg, sender, data);
	}
	
	return e;
}

t_hashtab *omax_class_getHashtab(char *classname){
	if(!classname){
		return NULL;
	}
	t_symbol *htname = omax_class_makeHashtabName(classname);
	if(htname){
		return (t_hashtab *)(htname->s_thing);
	}else{
		return NULL;
	}
}

t_symbol *omax_class_makeHashtabName(char *classname){
	char buf[128];
	sprintf(buf, "cnmat_internal_%s_hashtab", classname);
	return gensym(buf);
}

t_symbol *omax_class_makeMethodNameOSC(char *classname, char *methodname){
	char buf[128];
	//sprintf(buf, "/%s/%s", classname, methodname);
	sprintf(buf, "/%s", methodname);
	return gensym(buf);
}

t_symbol *omax_class_makeParamNameOSC(char *classname, char *paramname){
	char buf[128];
	//sprintf(buf, "/%s/%s", classname, paramname);
	sprintf(buf, "/%s", paramname);
	return gensym(buf);
}

t_symbol *omax_class_makeSetterNameOSC(char *classname, char *methodname){
	return omax_class_makeAccessorNameOSC(classname, methodname, "set");
}

t_symbol *omax_class_makeGetterNameOSC(char *classname, char *methodname){
	return omax_class_makeAccessorNameOSC(classname, methodname, "get");
}

t_symbol *omax_class_makeAccessorNameOSC(char *classname, char *methodname, char *type){
	char buf[128];
	//sprintf(buf, "/%s/%s/%s", classname, methodname, type);
	sprintf(buf, "/%s/%s", methodname, type);
	return gensym(buf);
}

t_symbol *omax_class_makeSetterName(char *methodname){
	return gensym(methodname);
}

t_symbol *omax_class_makeGetterName(char *methodname){
	char buf[128];
	sprintf(buf, "get%s", methodname);
	return gensym(buf);
}

t_omax *omax_object_getOmaxObj(t_object *ob){
	t_symbol *classname = object_classname(ob);
	if(!classname){
		return NULL;
	}
	t_hashtab *ht = NULL;
	ht = omax_class_getHashtab(classname->s_name);
	if(!ht){
		return NULL;
	}
	size_t offset;
	hashtab_lookup(ht, gensym("cnmat_internal_osc_ob_offset"), (t_object **)(&offset));
	t_omax *osc = (t_omax *)(((char *)ob) + offset);
	return osc;
}

void *omax_object_getContinuationOutlet(t_object *ob){
	t_omax *osc = omax_object_getOmaxObj(ob);
	if(osc){
		return osc->continuation_outlet;
	}else{
		return NULL;
	}
}

void *omax_object_getInfoOutlet(t_object *ob){
	t_omax *osc = omax_object_getOmaxObj(ob);
	if(osc){
		return osc->info_outlet;
	}else{
		return NULL;
	}
}

method omax_object_getNotificationCallback(t_object *ob){
	t_symbol *classname = object_classname(ob);
	if(!classname){
		return NULL;
	}

	t_hashtab *ht = omax_class_getHashtab(classname->s_name);
	if(!ht){
		return NULL;
	}
	method f = NULL;
	hashtab_lookup(ht, gensym("cnmat_internal_osc_notification_function"), (t_object **)(&f));
	return f;
}

void omax_util_oscMsg2MaxAtoms(t_osc_msg *msg, long *ac, t_atom *av){
	t_osc_msg m = *msg;
	*ac = osc_message_getArgCount(msg) + 1;
	t_atom *ptr = av;
	if(m.address){
		atom_setsym(ptr, gensym(m.address));
	}else{
		return;
	}
	ptr++;

	int numints = 0, numfloats = 0, numother = 0;
	char *tt1 = m.typetags + 1;
	int i;
	for(i = 0; i < m.argc; i++){
		switch(tt1[i]){
		case 'i':
			numints++;
			break;
		case 'f':
			numfloats++;
			break;
		default:
			numother++;
			break;
		}
	}
	if(numints > 0 && (numfloats == 0 && numother == 0)){
		int32_t *buf = (int32_t *)(m.argv);
		for(i = 0; i < m.argc; i++){
			atom_setlong(ptr++, ntoh32(buf[i]));
		}
		return;
	}else if(numfloats > 0 && (numints == 0 && numother == 0)){
		//float *buf = (float *)(m.argv);
		int32_t *buf = (int32_t *)(m.argv);
		for(i = 0; i < m.argc; i++){
			int32_t ii = ntoh32(buf[i]);
			atom_setfloat(ptr++, *((float *)&ii));
		}
		return;
	}else if(numfloats > 0 && numints > 0 && numother == 0){
		tt1 = m.typetags + 1;
		int32_t *buf = (int32_t *)(m.argv);
		//float *buff = (float *)(m.argv);
		for(i = 0; i < m.argc; i++){
			switch(tt1[i]){
			case 'i':
				atom_setlong(ptr++, ntoh32(buf[i]));
				break;
			case 'f':
				{
					//float f = buff[i];
					int32_t ii = ntoh32(buf[i]);
					float f = *((float *)&ii);
					atom_setfloat(ptr++, f);
				}
				break;
			}
		}
		return;
	}

	while(osc_message_incrementArg(&m)){
		switch(*(m.typetags)){
		case 'i':
			atom_setlong(ptr++, ntoh32(*((int32_t *)m.argv)));
			break;
		case 'f':
			{
				uint32_t l = ntoh32(*((int32_t *)m.argv));
				atom_setfloat(ptr++, *((float *)&l));
				//atom_setfloat(ptr++, *((float *)m.argv));
			}
			break;
		case 'h':
		case 't':
			atom_setlong(ptr++, (long)ntoh64(*((int64_t *)m.argv)));
			break;
		case 'd':
			{
				uint64_t l = ntoh64(*((int64_t *)m.argv));
				atom_setfloat(ptr++, (float)*((double *)&l));
				//atom_setfloat(ptr++, *((double *)m.argv));
			}
			break;
		case 's':
		case 'S':
			atom_setsym(ptr++, gensym(m.argv));
			break;
		case 'H':
			{
				if(!strncmp(m.address, "/osc/tim", 8)){
					// NTP timetag--convert to ISO 8601
					char buf[128];
					ntptime tt = (ntptime){*((uint32_t *)m.argv), *((uint32_t *)(m.argv + 4)), 1, TIME_STAMP};
					osc_timetag_ntp_to_iso8601(&tt, buf);
					atom_setsym(ptr++, gensym(buf));
				}
			}
		case 'c':
			atom_setlong(ptr++, (long)*(m.argv));
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
				int j, n = osc_sizeof(*(m.typetags), m.argv);
				atom_setlong(ptr++, ntoh32(*((uint32_t *)(m.argv))));
				for(j = 0; j < n; j++){
					atom_setlong(ptr++, (long)m.argv[j]);
				}
			}
			break;
		}
	}
	//return atomarray_new(n + 1, a);
}

int osc_util_make_bundle(int numAddresses,
			  t_symbol **addresses, 
			  int *numArgs,
			  char **typetags, 
			  t_atom **args, 
			  int *len, 
			  char *buffer)
{
	char *ptr = buffer;
	if(strncmp(buffer, "#bundle", 8)){
		strncpy(buffer, "#bundle\0", 8);
		ptr += 8;
		*((uint64_t *)ptr) = hton64(1);
		ptr += 8;
	}
	int i, j;
	for(i = 0; i < numAddresses; i++){
		char *sizeptr = ptr;
		ptr += 4; // skip over size
		*len = osc_util_check_pos_and_resize(buffer, *len, ptr);
		int addlen = strlen(addresses[i]->s_name);
		memcpy(ptr, addresses[i]->s_name, addlen);
		ptr += addlen;
		ptr += 4 - (addlen % 4);
		*len = osc_util_check_pos_and_resize(buffer, *len, ptr);
		*ptr = ',';
		char *tt = ++ptr;
		ptr += numArgs[i];
		ptr += 4 - ((numArgs[i] + 1) % 4);
		*len = osc_util_check_pos_and_resize(buffer, *len, ptr);
		for(j = 0; j < numArgs[i]; j++){
			*tt = typetags[i][j];
			switch(*tt){
			case 'i':
				*((long *)ptr) = hton32(atom_getlong(args[i] + j));
				ptr += 4;
				break;
			case 'f':
				{
					float f = atom_getfloat(args[i] + j);
					*((long *)ptr) = hton32(*((uint32_t *)(&f)));
					//*((float *)ptr) = atom_getfloat(args[i] + j);
					ptr += 4;
				}
				break;
			case 's':
				{
					t_symbol *s = atom_getsym(args[i] + j);
					int slen = strlen(s->s_name);
					memcpy(ptr, s->s_name, slen);
					ptr += slen;
					ptr += 4 - (slen % 4);
				}
				break;
			case 'I':
				break;
			}
			tt++;
			*len = osc_util_check_pos_and_resize(buffer, *len, ptr);
		}
		*((long *)sizeptr) = hton32(ptr - sizeptr - 4);
	}
	return ptr - buffer;
}

int osc_util_check_pos_and_resize(char *buf, int len, char *pos){
	if(pos - buf >= len){
		realloc(buf, len * 2);
		return len * 2;
	}
	return len;
}

char omax_util_typetagForAtom(t_atom *a){
	switch(atom_gettype(a)){
	case A_FLOAT:
		return 'f';
	case A_LONG:
		return 'i';
	case A_SYM:
		return 's';
	}
	return '\0';
}

int omax_util_get_bundle_size_for_atoms(t_symbol *address, int argc, t_atom *argv){
	int len = 20;
	len += strlen(address->s_name);
	len++;
	while(len % 4){
		len++;
	}

	len += argc + 2;
	while(len % 4){
		len++;
	}
	int i;
	for(i = 0; i < argc; i++){
		switch(atom_gettype(argv + i)){
		case A_LONG:
		case A_FLOAT:
			len += 4;
			break;
		case A_SYM:
			len += strlen(atom_getsym(argv + i)->s_name);
			len++;
			while(len % 4){
				len++;
			}
			break;
		}
	}
	return len;
}

int omax_util_encode_atoms(char *buf, t_symbol *address, int argc, t_atom *argv){
	char *sizeptr = buf;
	char *ptr = buf + 4;
	strcpy(ptr, address->s_name);
	ptr += strlen(address->s_name);
	ptr++;
	while((ptr - buf) % 4){
		ptr++;
	}

	*ptr++ = ',';
	int i;
	for(i = 0; i < argc; i++){
		*ptr++ = omax_util_typetagForAtom(argv + i);
	}
	ptr++;
	while((ptr - buf) % 4){
		ptr++;
	}
	for(i = 0; i < argc; i++){
		switch(atom_gettype(argv + i)){
		case A_FLOAT:
			{
				float f = atom_getfloat(argv + i);
				*((uint32_t *)ptr) = hton32(*((uint32_t *)(&f)));
				//*((float *)ptr) = atom_getfloat(argv + i);
				ptr += 4;
			}
			break;
		case A_LONG:
			{
				uint32_t l = atom_getlong(argv + i);
				*((uint32_t *)ptr) = hton32(l);
				ptr += 4;
			}
			break;
		case A_SYM:
			strcpy(ptr, atom_getsym(argv + i)->s_name);
			ptr += strlen(atom_getsym(argv + i)->s_name);
			ptr++;
			while((ptr - buf) % 4){
				ptr++;
			}
			break;
		}
	}
	*((long *)sizeptr) = htonl(ptr - buf - 4);
	return ptr - buf;
}

int osc_util_make_bundle_from_atoms(long argc, t_atom *argv, int *len, char *buffer){
	char *bufptr = buffer;
	if(atom_gettype(argv) != A_SYM){
		error("%s: the first argument must be a symbol", __PRETTY_FUNCTION__);
		return 0;
	}
	t_atom *address = argv;
	t_symbol *address_sym = atom_getsym(argv);
	/*
	if(argc == 1){
		return omax_util_encode_atoms(buffer, 0, address_sym, 0, NULL);
	}
	*/
	if(!address_sym){
		return 0;
	}
	if(address_sym->s_name[0] != '/'){
		error("%s is not an OSC address", address_sym->s_name);
	}
	t_atom *ptr = argv + 1;
	//while(1){
		while(ptr - argv < argc){
			if(atom_gettype(ptr) == A_SYM){
				t_symbol *sym = atom_getsym(ptr);
				if(sym){
					if(sym->s_name[0] == '/'){
						bufptr += omax_util_encode_atoms(bufptr, address_sym, ptr - address - 1, address + 1);
						address = ptr;
						address_sym = sym;
					}
				}
			}
			ptr++;
		}
		//}
	bufptr += omax_util_encode_atoms(bufptr, address_sym, ptr - address - 1, address + 1);
	return bufptr - buffer;
	/*
	int i;
	if(atom_gettype(argv) != A_SYM){
		return 0;
	}
	if(*(atom_getsym(argv)->s_name) != '/'){
		return 0;
	}
	int numargs = 0;
	char typetags[argc];
	char argbuf[*len];
	char *bufptr = buffer, *argbufptr = argbuf;
	char *sizeptr = buffer;
	bufptr += 4; // skip over the size
        for(i = 0; i < argc; i++){
		int type;
		if((type = atom_gettype(argv + i)) == A_SYM){
			t_symbol *sym = atom_getsym(argv + i);
			if(*(sym->s_name) == '/'){
				// OSC address
				if(numargs){
					*bufptr++ = ',';
					memcpy(bufptr, typetags, numargs);
					bufptr += numargs;
					*bufptr++ = '\0';
					while((bufptr - buffer) % 4){
						*bufptr++ = '\0';
					}
					memcpy(bufptr, argbuf, argbufptr - argbuf);
					bufptr += (argbufptr - argbuf);
					*((long *)sizeptr) = htonl((bufptr - sizeptr) - 4);
					sizeptr = bufptr;
					bufptr += 4;
				}else{
					*bufptr++ = ',';
					*bufptr++ = '\0';
					*bufptr++ = '\0';
					*bufptr++ = '\0';
					*((long *)sizeptr) = htonl((bufptr - sizeptr) - 4);
					sizeptr = bufptr;
					//bufptr += 4;
				}
				memset(argbuf, '\0', *len);
				memset(typetags, '\0', argc);
				argbufptr = argbuf;

				numargs = 0;
				strcpy(bufptr, sym->s_name);
				bufptr += strlen(sym->s_name) + 1;
				while((bufptr - buffer) % 4){
					*bufptr++ = '\0';
				}
			}else{
				typetags[numargs] = 's';
				strcpy(argbufptr, sym->s_name);
				argbufptr += strlen(sym->s_name) + 1;
				while((argbufptr - buffer) % 4){
					argbufptr++;
				}
				numargs++;
			}
		}else{
			switch(type){
			case A_FLOAT:
				*((long *)argbufptr) = htonl(*((long *)&(argv[i].a_w.w_float)));
				typetags[numargs] = 'f';
				break;
			case A_LONG:
				*((long *)argbufptr) = htonl(argv[i].a_w.w_long);
				typetags[numargs] = 'i';
				break;
			}
			numargs++;
			argbufptr += 4;
		}
        }
	if(numargs){
		*bufptr++ = ',';
		memcpy(bufptr, typetags, numargs);
		bufptr += numargs;
		*bufptr++ = '\0';
		while((bufptr - buffer) % 4){
			*bufptr++ = '\0';
		}
		memcpy(bufptr, argbuf, argbufptr - argbuf);
		bufptr += (argbufptr - argbuf);
		*((long *)sizeptr) = htonl((bufptr - sizeptr) - 4);
	}
	return bufptr - buffer;
	*/
}
