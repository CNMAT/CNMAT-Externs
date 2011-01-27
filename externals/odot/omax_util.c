#include "omax_util.h"

void omax_util_oscMsg2MaxAtoms(t_osc_msg *msg, long *ac, t_atom *av){
	t_osc_msg m = *msg; // make a copy
	*ac = osc_util_getArgCount(msg) + 1; 
	//t_atom a[n + 1], *ptr = a + 1;
	t_atom *ptr = av;
	if(msg->address){
		atom_setsym(ptr, gensym(msg->address));
	}else{
		return;
	}
	ptr++;
	//printf("%s: ac = %d\n", __PRETTY_FUNCTION__, *ac);

	while(osc_util_incrementArg(&m)){
		switch(*(m.typetags)){
		case 'i':
			atom_setlong(ptr++, ntoh32(*((int32_t *)m.argv)));
			break;
		case 'f':
			{
				uint32_t l = ntoh32(*((int32_t *)m.argv));
				atom_setfloat(ptr++, *((float *)&l));
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
			}
			break;
		case 's':
		case 'S':
			atom_setsym(ptr++, gensym(m.argv));
			break;
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
				*((long *)ptr) = htonl(atom_getlong(args[i] + j));
				ptr += 4;
				break;
			case 'f':
				{
					float f = atom_getfloat(args[i] + j);
					*((long *)ptr) = htonl(*((long *)(&f)));
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
		*((long *)sizeptr) = htonl(ptr - sizeptr - 4);
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
				*((uint32 *)ptr) = hton32(*((uint32 *)(&f)));
				ptr += 4;
			}
			break;
		case A_LONG:
			{
				uint32 l = atom_getlong(argv + i);
				*((uint32 *)ptr) = hton32(l);
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
