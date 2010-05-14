#include "oio_osc_util.h"
#include "oio_obj.h"
#include "oio_mem.h"


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

// these have to be implemented...
uint64_t oio_osc_util_machAbsoluteToNTP(uint64_t ma){
	return ma;
}

uint64_t oio_osc_util_NTPToMachAbsolute(uint64_t ntp){
	return ntp;
}

t_oio_err oio_osc_util_parseOSCBundle(int nbytes, char *osc_msg, t_oio_osc_msg **msg){
	char *ptr = osc_msg;
	if(strncmp(ptr, "#bundle\0", 8)){
		return OIO_ERR_OSCBNDL;
	}
	ptr += 16;
	while((ptr - osc_msg) < nbytes){
		int size = ntoh32(*((uint32_t *)ptr));
		ptr += 4;
		t_oio_osc_msg *m = (t_oio_osc_msg *)oio_mem_alloc(1, sizeof(t_oio_osc_msg));
		oio_osc_util_parseOSCMessage(size, ptr, m);
		m->next = *msg;
		*msg = m;
		ptr += size;
	}
	return OIO_ERR_NONE;
}

t_oio_err oio_osc_util_parseOSCMessage(int nbytes, char *osc_msg, t_oio_osc_msg *msg){
	char *ptr = osc_msg;
	// a valid message has to be at least 4 bytes for the address
	if(nbytes < 4 || !ptr){
		return OIO_ERR_OSCMSG;
	}
	// the first char of ptr should be '/'.  if it's not, skip ahead 4 bytes to see if the size was there.
	// if it's still not a '/', bail out
	if(*ptr != '/'){
		ptr += 4;
		if(*ptr != '/'){
			return OIO_ERR_OSCMSG;
		}
		nbytes -= 4; // size was included in the message string
	}
	if(!msg){
		msg = (t_oio_osc_msg *)oio_mem_alloc(1, sizeof(t_oio_osc_msg));
	}
	strcpy(msg->address, ptr);
	while(*ptr++){}
	while((ptr - osc_msg) % 4){
		ptr++;
	}
	if(ptr - osc_msg == nbytes){
		return OIO_ERR_NONE;
	}
	if(*ptr != ','){
		return OIO_ERR_NONE;
	}

	ptr++;
	char *tt = ptr;
	int n = 0;
	while(*ptr){
		ptr++;
	}
	n = ptr - tt;
	if(n == 0){
		return OIO_ERR_NONE;
	}
	while((ptr - osc_msg) % 4){
		ptr++;
	}

	// data
	msg->atoms = (t_oio_osc_atom *)oio_mem_alloc(n, sizeof(t_oio_osc_atom));
	msg->natoms = n;
	int i;
	for(i = 0; i < n; i++){
		t_oio_osc_atom *a = msg->atoms + i;
		oio_osc_util_atomSet(a, ptr, tt[i], 1);
		ptr += sizeofosc(tt[i]);
	}
	return OIO_ERR_NONE;
}

t_oio_err oio_osc_util_atomSet(t_oio_osc_atom *atom, void *buf, char typetag, char byteswap){
	atom->typetag = typetag;
	switch(typetag){
	case 'b':
		{
			int len = ntoh32(*((uint32_t *)buf));
			char *b = (char *)oio_mem_alloc(len, sizeof(char));
			memcpy(b, buf + 4, len);
			*((char **)atom) = b;
		}
		break;
	case 's':
	case 'S':
		{
			int len = strlen(buf);
			char *b = (char *)oio_mem_alloc(len, sizeof(char));
			memcpy(b, buf, len);
			*((char **)atom) = b;
		}
		break;
	case 'T':
	case 'F':
	case 'N':
	case 'I':
		break;
	default:
		{
			int len = sizeofosc(typetag);
			if(len == 8){
				uint64_t l = *((uint64_t *)buf);
				if(byteswap){
					l = OIO_BYTE_SWAP64(l);
				}
				*((uint64_t *)atom) = l;
			}else if(len == 4){
				uint32_t l = *((uint32_t *)buf);
				if(byteswap){
					l = OIO_BYTE_SWAP32(l);
				}
				*((uint32_t *)atom) = l;
			}else if(len == 2){
				uint16_t l = *((uint16_t *)buf);
				if(byteswap){
					l = OIO_BYTE_SWAP16(l);
				}
				*((uint16_t *)atom) = l;
			}else if(len == 1){
				*((uint8_t *)atom) = *((uint8_t *)buf);
			}else{
				return OIO_ERR_OSCTAG;
			}
		}
	}
	return OIO_ERR_NONE;
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

size_t sizeofosc(unsigned char typetag){
	if(typetag > 127){
		return -1;
	}
	return osc_data_lengths[typetag];
}
