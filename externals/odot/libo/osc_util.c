#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>
#include "osc_util.h"

int osc_util_parseMessage(int n, char *buf, t_osc_msg *osc_msg){
	if(!osc_msg){
		return 1;
	}
	char *ptr = buf;
	// this call could contain a message that's inside a bundle in which case the size will be the first 4 bytes of buf
	// or it could be a naked message in which case n will probably be valid.
	if(n <= 0){
		// assume the first 4 bytes are the size
		n = ntoh32(*((uint32_t *)ptr));
		if(n > 1024 && *ptr == '/'){
			// what are the chances?  better bail out
			return 1;
		}
		ptr += 4;
	}

	osc_msg->address = NULL;
	osc_msg->typetags = NULL;
	osc_msg->argv = NULL;

	osc_msg->size = n;
	osc_msg->address = ptr;
	while(*ptr++){}
	while((ptr - buf) % 4){
		ptr++;
	}
	if(ptr - buf == n){
		return 0;
	}
	// typetags are required
	if(*ptr != ','){
		return 0;
	}
	// we set the typetags pointer to point to the , so that the first call to osc_util_incrementArg will set it to the first
	// actual argument so that you can just go while(osc_util_incrementArg(...)){}
	osc_msg->typetags = ptr;
	ptr++;

	while(*ptr){
		ptr++;
	}
	osc_msg->argc = ptr - osc_msg->typetags - 1;
	if(osc_msg->argc == 0){
		return 0;
	}
	ptr++;
	while((ptr - buf) % 4){
		ptr++;
	}
	osc_msg->argv = ptr;
	return 0;
}

int osc_util_getArgCount(t_osc_msg *msg){
	if(msg->typetags){
		return strlen(msg->typetags) - 1;
	}
	return 0;
}

int osc_util_getMsgCount(int n, char *osc_bndl){
	if(!osc_bndl){
		return 0;
	}
	char *ptr = osc_bndl;
	if(strncmp(ptr, "#bundle\0", 8)){
		return 1;
	}
	ptr += 16;
	int count = 0;
	while((ptr - osc_bndl) < n){
		int size = ntoh32(*((uint32_t *)ptr));
		count++;
		ptr += size + 4;
	}
	return count;
}

int osc_util_parseBundle(int n, char *osc_bndl, t_osc_msg *msg){
	char *ptr = osc_bndl;
	if(strncmp(ptr, "#bundle\0", 8)){
		return 1;
	}
	ptr += 16;
	int i = 0;
	while((ptr - osc_bndl) < n){
		int size = ntoh32(*((uint32_t *)ptr));
		osc_util_parseMessage(-1, ptr, msg + i);
		ptr += size + 4;
		i++;
	}
	return 0;
}

// this is useful when you don't know anything about the contents of the bundle and don't want to 
// have to iterate through things more than necessary.
int osc_util_parseBundleWithCallback(int n, char *osc_bndl, void (*f)(t_osc_msg, void *), void *context){
	char *ptr = osc_bndl;
	if(strncmp(ptr, "#bundle\0", 8)){
		return 1;
	}
	ptr += 16;
	while((ptr - osc_bndl) < n){
		int size = ntoh32(*((uint32_t *)ptr));
		t_osc_msg m;
		osc_util_parseMessage(-1, ptr, &m);
		f(m, context);
		ptr += size + 4;
	}
	return 0;
}

int osc_util_incrementArg(t_osc_msg *msg){
	if(!msg->typetags){
		return 0;
	}
	char tt = *(msg->typetags);
	if(tt == ','){
		msg->typetags += 1;
		return 1;
	}
	msg->typetags += 1;
	msg->argc--;
	if(msg->typetags == '\0' || msg->argc == 0){
		return 0;
	}
	int size = osc_sizeof(tt, msg->argv);
	msg->argv += size;
	return 1;
}

void osc_util_printBundle(int len, char *bundle, int (*p)(const char *, ...)){
	if(strncmp(bundle, "#bundle\0", 8)){
		return;
	}
	p("[ #bundle %llu (0x%x)\n");
	osc_util_parseBundleWithCallback(len, bundle, osc_util_printBundleCbk, (void *)p);
	p("]\n");
}

void osc_util_printBundleCbk(t_osc_msg msg, void *context){
	osc_util_printMsg(&msg, context);
}

void osc_util_printMsg(t_osc_msg *msg, int (*p)(const char *, ...)){
	p("\t%d ", msg->size);
	p("%s ", msg->address);
	while(osc_util_incrementArg(msg)){
		switch(*(msg->typetags)){
		case 'i':
			p("%d ", ntoh32(*((int32_t *)msg->argv)));
			break;
		case 'f':
			{
				uint32_t l = ntoh32(*((int32_t *)msg->argv));
				p("%f ", *((float *)&l));
			}
			break;
		case 'h':
		case 't':
			p("%lld ", ntoh64(*((int64_t *)msg->argv)));
			break;
		case 'd':
			{
				uint64_t l = ntoh64(*((int64_t *)msg->argv));
				p("%f ", *((double *)&l));
			}
			break;
		case 's':
		case 'S':
			p("%s ", msg->argv);
			break;
		case 'c':
			p("%c ", *(msg->argv));
			break;
		case 'T':
			p("True ");
			break;
		case 'F':
			p("False ");
			break;
		case 'N':
			p("Nil ");
			break;
		case 'I':
			p("Infinitum ");
			break;
		case 'b':
			{
				int j, n = osc_sizeof(*(msg->typetags), msg->argv);
				p("blob (%d bytes): ", n);
				for(j = 0; j < n; j++){
					p("%d ", msg->argv[j]);
				}
			}
			break;
		}
	}
	p("\n");
}

long osc_util_bundle_naked_message(long n, char *ptr, char *out){
	if(!strncmp(ptr, "#bundle\0", 8)){
		return -1;
	}
	if(!out){
		printf("%s line %d called malloc--you should use a memory pool!\n", __PRETTY_FUNCTION__, __LINE__);
		out = malloc(n + 20);
	}
	char buf[n + 20];
	buf[0] = '#';
	buf[1] = 'b';
	buf[2] = 'u';
	buf[3] = 'n';
	buf[4] = 'd';
	buf[5] = 'l';
	buf[6] = 'e';
	buf[7] = '\0';
	*((long long *)(buf + 8)) = 0x000000001; // now
	*((long *)(buf + 16)) = hton32(n);
	memcpy(buf + 20, ptr, n);
	memcpy(out, buf, n + 20);
	return n + 20;
}

long osc_util_flatten(long n, char *ptr, char *out){
	// out could be the same as ptr for an inplace operation, so do everything locally and then copy
	if(strncmp(ptr, "#bundle\0", 8)){
		return -1;
	}
	if(!out){
		printf("%s line %d called malloc--you should use a memory pool!\n", __PRETTY_FUNCTION__, __LINE__);
		out = malloc(n);
	}
	char localptr[n];
	int i = 0, j = 0;
	memcpy(localptr, ptr, 16);
	i = j = 16;
	while(i < n){
		if(!strncmp(ptr + i, "#bundle\0", 8)){
			i += 16; // skip over #bundle\0, timetag, and size
			j -= 4;
		}else{
			*(localptr + (j++)) = *(ptr + i++);
		}
	}
	memset(localptr + j, '\0', n - j);
	memset(out, '\0', n);
	memcpy(out, localptr, j);
	return j;
}

int osc_util_rename(char *buffer, t_osc_msg *msg, char *new_address){
	char *ptr = buffer + 4;
	int new_address_len = strlen(new_address);
	memcpy(ptr, new_address, new_address_len);
	ptr += new_address_len + 1;
	while((ptr - buffer) % 4){
		ptr++;
	}
	int rest_of_msg = msg->size - (msg->typetags - msg->address);
	memcpy(ptr, msg->typetags, rest_of_msg);
	ptr += rest_of_msg;
	*((uint32_t *)buffer) = hton32((ptr - buffer) - 4);
	return ptr - buffer;
}

char osc_data_lengths[128] = {
	/*	0		*/	-1	,
	/*	1		*/	-1	,
	/*	2		*/	-1	,
	/*	3		*/	-1	,
	/*	4		*/	-1	,
	/*	5		*/	-1	,
	/*	6		*/	-1	,
	/*	7		*/	-1	,
	/*	8		*/	-1	,
	/*	9		*/	-1	,
	/*	10		*/	-1	,
	/*	11		*/	-1	,
	/*	12		*/	-1	,
	/*	13		*/	-1	,
	/*	14		*/	-1	,
	/*	15		*/	-1	,
	/*	16		*/	-1	,
	/*	17		*/	-1	,
	/*	18		*/	-1	,
	/*	19		*/	-1	,
	/*	20		*/	-1	,
	/*	21		*/	-1	,
	/*	22		*/	-1	,
	/*	23		*/	-1	,
	/*	24		*/	-1	,
	/*	25		*/	-1	,
	/*	26		*/	-1	,
	/*	27		*/	-1	,
	/*	28		*/	-1	,
	/*	29		*/	-1	,
	/*	30		*/	-1	,
	/*	31		*/	-1	,
	/*	32		*/	-1	,
	/*	33		*/	-1	,
	/*	34		*/	-1	,
	/*	35		*/	-1	,
	/*	36		*/	-1	,
	/*	37		*/	-1	,
	/*	38		*/	-1	,
	/*	39		*/	-1	,
	/*	40		*/	-1	,
	/*	41		*/	-1	,
	/*	42		*/	-1	,
	/*	43		*/	-1	,
	/*	44		*/	-1	,
	/*	45		*/	-1	,
	/*	46		*/	-1	,
	/*	47		*/	-1	,
	/*	48		*/	-1	,
	/*	49		*/	-1	,
	/*	50		*/	-1	,
	/*	51		*/	-1	,
	/*	52		*/	-1	,
	/*	53		*/	-1	,
	/*	54		*/	-1	,
	/*	55		*/	-1	,
	/*	56		*/	-1	,
	/*	57		*/	-1	,
	/*	58		*/	-1	,
	/*	59		*/	-1	,
	/*	60		*/	-1	,
	/*	61		*/	-1	,
	/*	62		*/	-1	,
	/*	63		*/	-1	,
	/*	64		*/	-1	,
	/*	65	A	*/	-1	,
	/*	66	B	*/	-1	,
	/*	67	C	*/	-1	,
	/*	68	D	*/	-1	,
	/*	69	E	*/	-1	,
	/*	70	F	*/	0	,
	/*	71	G	*/	-1	,
	/*	72	H	*/	-1	,
	/*	73	I	*/	0	,
	/*	74	J	*/	-1	,
	/*	75	K	*/	-1	,
	/*	76	L	*/	-1	,
	/*	77	M	*/	-1	,
	/*	78	N	*/	0	,
	/*	79	O	*/	-1	,
	/*	80	P	*/	-1	,
	/*	81	Q	*/	-1	,
	/*	82	R	*/	-1	,
	/*	83	S	*/	sizeof(char *)	,
	/*	84	T	*/	0	,
	/*	85	U	*/	-1	,
	/*	86	V	*/	-1	,
	/*	87	W	*/	-1	,
	/*	88	X	*/	-1	,
	/*	89	Y	*/	-1	,
	/*	90	Z	*/	-1	,
	/*	91		*/	-1	,
	/*	92		*/	-1	,
	/*	93		*/	-1	,
	/*	94		*/	-1	,
	/*	95		*/	-1	,
	/*	96		*/	-1	,
	/*	97	a	*/	-1	,
	/*	98	b	*/	sizeof(char *)	,
	/*	99	c	*/	1	,
	/*	100	d	*/	8	,
	/*	101	e	*/	-1	,
	/*	102	f	*/	4	,
	/*	103	g	*/	-1	,
	/*	104	h	*/	8	,
	/*	105	i	*/	4	,
	/*	106	j	*/	-1	,
	/*	107	k	*/	-1	,
	/*	108	l	*/	-1	,
	/*	109	m	*/	4	,
	/*	110	n	*/	-1	,
	/*	111	o	*/	-1	,
	/*	112	p	*/	-1	,
	/*	113	q	*/	-1	,
	/*	114	r	*/	4	,
	/*	115	s	*/	sizeof(char *)	,
	/*	116	t	*/	8	,
	/*	117	u	*/	-1	,
	/*	118	v	*/	-1	,
	/*	119	w	*/	-1	,
	/*	120	x	*/	-1	,
	/*	121	y	*/	-1	,
	/*	122	z	*/	-1	,
	/*	123		*/	-1	,
	/*	124		*/	-1	,
	/*	125		*/	-1	,
	/*	126		*/	-1	,
	/*	127		*/	-1		
};

size_t osc_sizeof(unsigned char typetag, char *data){
	if(typetag > 127){
		return -1;
	}
	switch(typetag){
	case 'b':
	case 's':
	case 'S':
		{
			int size = strlen(data);
			size++;
			while(size % 4){
				size++;
			}
			return size;
		}
	default:
		return osc_data_lengths[typetag];
	}
}
