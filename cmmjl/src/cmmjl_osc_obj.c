#include "cmmjl_osc_obj.h"

t_cmmjl_osc_packet *cmmjl_osc_make_packet(long len, char *ptr){
	t_cmmjl_osc_packet *p = (t_cmmjl_osc_packet *)calloc(1, sizeof(t_cmmjl_osc_packet));

	if(!(strncmp(ptr, "#bundle", 8))){
		// this is a bundle
		post("bundle");
		int i = 8;
		t_cmmjl_osc_bundle *b = cmmjl_osc_bundle_new();
		cmmjl_osc_make_bundle(len, ptr, b);
		p->contents = (t_cmmjl_osc_obj *)b;
	}else{
		post("not a bundle");
		t_cmmjl_osc_message *msg = cmmjl_osc_message_new_raw(len, ptr);
		p->contents = (t_cmmjl_osc_obj *)msg;
	}
	return p;
}

void cmmjl_osc_make_bundle(long len, char *ptr, t_cmmjl_osc_bundle *b){
	if(len < 16){
		return;
	}
	if(strncmp(ptr, "#bundle", 8)){
		return;
	}
	int size;
	int i = 0;
	i += 8; // #bundle\0
	b->timetag = ptr + i;
	i += 8; // timetag
	t_cmmjl_osc_obj *obj = &(b->obj);
	t_cmmjl_osc_obj **element = &(b->element);
	while(i < len){
		size = ntohl(*((long *)(ptr + i)));
		if(strncmp(ptr + i + 4, "#bundle", 8) == 0){
			post("found another bundle");
			i += 4;
			t_cmmjl_osc_bundle *bb = cmmjl_osc_bundle_new();
			cmmjl_osc_make_bundle(size, ptr + i, bb);
			obj->next = (t_cmmjl_osc_obj *)bb;
			obj = obj->next;
			obj->size = size;
		}else{
			post("not a bundle: %d %s", i, ptr + i + 4);
			i += 4;
			post("making message for %p size %d", ptr + i, size);
			*element = (t_cmmjl_osc_obj *)cmmjl_osc_message_new_raw(size, ptr + i);
			(*element)->size = size;
			element = &((*element)->next);
		}
		i += size;
	}
}

t_cmmjl_osc_bundle *cmmjl_osc_bundle_new(void){
	t_cmmjl_osc_bundle *b = (t_cmmjl_osc_bundle *)calloc(1, sizeof(t_cmmjl_osc_bundle));
	cmmjl_osc_set_bundle_id(b);
	return b;
}

t_cmmjl_osc_message *cmmjl_osc_message_new(){
	t_cmmjl_osc_message *msg = (t_cmmjl_osc_message *)calloc(1, sizeof(t_cmmjl_osc_message));
	return msg;
}

t_cmmjl_osc_message *cmmjl_osc_message_new_args(char *address, char *typetags, long argc, char *argv){
	t_cmmjl_osc_message *msg = cmmjl_osc_message_new();
	cmmjl_osc_message_set_address(address, msg);
	cmmjl_osc_message_set_typetags(typetags, msg);
	cmmjl_osc_message_set_data(argc, argv, msg);
	return msg;
}

t_cmmjl_osc_message *cmmjl_osc_message_new_raw(long len, char *ptr){
	char *address = ptr;
	post("address %s", ptr);
	char *tt;
	cmmjl_osc_dataAfterAlignedString(ptr, ptr + len, &tt);
	tt++;
	int numArgs = 0;
	while(*(tt + numArgs) != '\0'){
		numArgs++;
	}

	char *args;
	cmmjl_osc_dataAfterAlignedString(tt - 1, ptr + len, &args);
	post("+++++ %p %p", tt, args);

	return cmmjl_osc_message_new_args(address, tt, numArgs, args);
}

void cmmjl_osc_message_set_address(char *address, t_cmmjl_osc_message *msg){
	msg->address = address;
}

void cmmjl_osc_message_set_typetags(char *tt, t_cmmjl_osc_message *msg){
	msg->typetags = tt;
}

void cmmjl_osc_message_set_data(long argc, char *argv, t_cmmjl_osc_message *msg){
	if(msg->argc != argc){
		msg->argc = argc;
		msg->argv = argv;
	}
}

void cmmjl_osc_message_get_address(t_cmmjl_osc_message *msg, char **address){
	*address = msg->address;
}

void cmmjl_osc_message_get_arguments(t_cmmjl_osc_message *msg, int size, t_cmmjl_osc_atom *args){
	int i;
	char *ptr = msg->argv;
	for(i = 0; i < msg->argc; i++){
		post("%d %p", i, ptr);
		args[i].a_type = *(msg->typetags + i);
		post("type = %c", args[i].a_type);
		if(*(msg->typetags + i) == 'i' || *(msg->typetags + i) == 'f'){
			args[i].a_w.w_int = ntohl(*((int *)ptr));
			ptr += 4;
		}else if(*(msg->typetags + i) == 't'){
			long long l1 = ntohl(*((long *)ptr));
			long long l2 = ntohl(*((long *)ptr));
			l2 = (l2 << 32) | l1;
			args[i].a_w.w_timetag = l2;

			ptr += 8;
		}else if(*(msg->typetags + i) == 's' || *(msg->typetags + i) == 'b'){
			args[i].a_w.w_blob = ptr;
			cmmjl_osc_dataAfterAlignedString(ptr, msg->address + size, &ptr);
		}
	}
}

void cmmjl_osc_filter(t_cmmjl_osc_packet *p1, t_cmmjl_osc_packet *p2){

}

void cmmjl_osc_remove_bundle_element(t_cmmjl_osc_bundle *b, t_cmmjl_osc_obj *obj_to_remove){
	t_cmmjl_osc_obj *obj = b;
	t_cmmjl_osc_obj *element;
	if(b == obj_to_remove){
		cmmjl_osc_free_bundle(b);
		return;
	}
	while(obj){

		obj = obj->next;
	}
}

void cmmjl_osc_free_packet(t_cmmjl_osc_packet *p){
	t_cmmjl_osc_obj *obj;
	t_cmmjl_osc_obj *next;
	t_cmmjl_osc_obj *element;
	obj = p->contents;
	while(obj){
		next = obj->next;
		if(cmmjl_osc_is_bundle(obj)){
			cmmjl_osc_free_bundle((t_cmmjl_osc_bundle *)obj);
		}else{
			cmmjl_osc_free_message((t_cmmjl_osc_message *)obj);
		}
		obj = next;
	}
}

void cmmjl_osc_free_bundle(t_cmmjl_osc_bundle *b){
	t_cmmjl_osc_obj *element = b->element;
	t_cmmjl_osc_obj *next;
	while(element){
		next = element->next;
		if(cmmjl_osc_is_bundle(element)){
			cmmjl_osc_free_bundle((t_cmmjl_osc_bundle *)element);
		}else{
			cmmjl_osc_free_message((t_cmmjl_osc_message *)element);
		}
		element = next;
	}
}

void cmmjl_osc_free_message(t_cmmjl_osc_message *m){
	free(m);
}

void cmmjl_osc_set_bundle_id(t_cmmjl_osc_bundle *b){
	b->bundle[0] = '#';
	b->bundle[1] = 'b';
	b->bundle[2] = 'u';
	b->bundle[3] = 'n';
	b->bundle[4] = 'd';
	b->bundle[5] = 'l';
	b->bundle[6] = 'e';
	b->bundle[7] = '\0';
}

int cmmjl_osc_is_bundle(t_cmmjl_osc_obj *obj){
	if(strncmp(((char *)obj) + sizeof(t_cmmjl_osc_obj), "#bundle", 8)){
		return 0;
	}else{
		return 1;
	}
}

