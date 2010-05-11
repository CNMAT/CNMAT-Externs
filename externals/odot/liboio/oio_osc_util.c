#include "oio_osc_util.h"
#include "oio_obj.h"


t_oio_err oio_osc_util_printMessage(int n, char *buf, int (*f)(const char *,...)){
	char *ptr = buf;
	f("%u ", ntoh32(*((uint32_t *)buf)));
	ptr += 4;
	f(" ");
	while(*ptr != ','){
		f("%c", *ptr++);
	}
	ptr++;
	f(" %c ", *ptr);
	ptr += 3;
	f("%llu (0x%llx)\n", ntoh64(*((uint64_t *)ptr)), ntoh64(*((uint64_t *)ptr)));
	return OIO_ERR_NONE;
}

t_oio_err oio_osc_util_printBundle(int n, char *buf, int (*f)(const char *,...)){
	char *ptr = buf;
	if(strncmp(buf, "#bundle\0", 8)){
		OIO_ERROR(OIO_ERR_OSCBNDL);
		return OIO_ERR_OSCBNDL;
	}
	ptr += 8;
	uint64_t ts = ntoh64(*((uint64_t *)ptr));
	f("[ #bundle timestamp: %llu 0x%llx\n", ts, ts);
	ptr += 8;

	while(ptr - buf < n){
		int nn = ntoh32(*((uint32_t *)ptr)) + 4;
		oio_osc_util_printMessage(nn, ptr, f);
		ptr += nn;
	}
	f("]\n");
	return OIO_ERR_NONE;
}

void oio_osc_util_makenice(char *buf){
	int i, n = strlen(buf);
	for(i = 0; i < n; i++){
		switch(buf[i]){
		case ' ':
		case '/':
		case '*':
		case '?':
			buf[i] = '-';
			break;
		case '[':
			buf[i] = '(';
			break;
		case ']':
			buf[i] = ')';
			break;
		}
	}
}
