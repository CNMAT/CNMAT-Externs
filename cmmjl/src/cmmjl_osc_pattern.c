#include "cmmjl.h"
#include "cmmjl_osc_pattern.h"
#include <string.h>

int cmmjl_osc2regex(char *osc_string, regex_t *re){
	int i;
	int len = strlen(osc_string);

	// this should be long enough, but we'll keep track and allocate more if necessary
	char write_buf[len * 2]; 
	memset(write_buf, '\0', len * 2);
	char read_buf[len * 2]; 
	memcpy(read_buf, osc_string, len);

	// keep track of our position in the buffer.
	int read_pos = 0;
	int write_pos = 0;

	//////////////////////////////////////////////////
	// replace:
	//	'?' => [^/]
	//	'*' => [^/]*
	//	'{x,y,z}' => (x|y|z)
	for(read_pos = 0; read_pos < len; read_pos++){
		if(read_buf[read_pos] == '?'){
			if(write_pos + 3 > len * 2){
				// allocate some more memory
			}
			write_buf[write_pos++] = '[';
			write_buf[write_pos++] = '^';
			write_buf[write_pos++] = '/';
			write_buf[write_pos++] = ']';
			//++write_pos;
		}else if(read_buf[read_pos] == '*'){
			if(write_pos + 4 > len * 2){
				// mem
			}
			write_buf[write_pos++] = '[';
			write_buf[write_pos++] = '^';
			write_buf[write_pos++] = '/';
			write_buf[write_pos++] = ']';
			write_buf[write_pos++] = '*';
			//++write_pos;
		}else if(read_buf[read_pos] == '{'){
			write_buf[write_pos++] = '(';
			read_pos++;
			while(read_buf[read_pos] != '}' && read_pos < len * 2){
				if(read_buf[read_pos] == ','){
					write_buf[write_pos++] = '|';
				}else{
					write_buf[write_pos++] = read_buf[read_pos];
				}
				read_pos++;
			}
			if(read_buf[read_pos] == '}'){
				write_buf[write_pos++] = ')';
			}
		}else{
			write_buf[write_pos++] = read_buf[read_pos];
		}
	}
	//post("%s => %s", read_buf, write_buf);
	int e;
	if(e = regcomp(re, write_buf, REG_EXTENDED)){
		return e;
	}
	return 0;
}

int cmmjl_osc_match(void *x, char *st1, char *st2){
	regex_t re;
	int e;
	char ebuf[256];
	if(e = cmmjl_osc2regex(st1, &re)){
		regerror(e, &re, ebuf, 256);
		CMMJL_ERROR(x, CMMJL_OSC_EMATCH, "%s:\n\t%s", 
			    cmmjl_strerror(CMMJL_OSC_EMATCH), ebuf);
		return e;
	}
	if(e = cmmjl_osc_match_re(&re, st2)){
		regerror(e, &re, ebuf, 256);
		CMMJL_ERROR(x, CMMJL_OSC_EMATCH, "%s:\n\t%s", 
			    cmmjl_strerror(CMMJL_OSC_EMATCH), ebuf);
		return e;
	}
	return e;
}

int cmmjl_osc_match_re(regex_t *re, char *st){
	return regexec(re, st, 0, NULL, 0);
}
