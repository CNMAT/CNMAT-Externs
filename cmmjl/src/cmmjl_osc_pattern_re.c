#include "cmmjl.h"
#include "cmmjl_osc_pattern_re.h"
#include <string.h>

int cmmjl_osc_hasPattern(int len, char *st);

int cmmjl_osc2regex(char *osc_string, regex_t *re){
	int i;
	int len = strlen(osc_string);
	//post("turning %s into a regex pattern", osc_string);
	//post("len = %d", len);

	// this should be long enough, but we'll keep track and allocate more if necessary
	char write_buf[len * 8]; 
	memset(write_buf, '\0', len * 8);
	char read_buf[len + 1]; 
	memcpy(read_buf, osc_string, len);
	read_buf[len] = '\0';

	// keep track of our position in the buffer.
	int read_pos = 0;
	int write_pos = 0;

	write_buf[0] = '^';
	write_pos++;

	//////////////////////////////////////////////////
	// replace:
	//	'?' => [^/]
	//	'*' => [^/]*
	//	'{x,y,z}' => (x|y|z)
	for(read_pos = 0; read_pos < len; read_pos++){
		if(read_buf[read_pos] == '?'){
			if(write_pos + 3 > (len * 8)){
				error("out of memory!");
				// allocate some more memory
			}
			write_buf[write_pos++] = '[';
			write_buf[write_pos++] = '^';
			write_buf[write_pos++] = '\\';
			write_buf[write_pos++] = '/';
			write_buf[write_pos++] = ']';
			//++write_pos;
		}else if(read_buf[read_pos] == '*'){
			if(write_pos + 4 > (len * 8)){
				error("out of memory!");
				// mem
			}
			write_buf[write_pos++] = '[';
			write_buf[write_pos++] = '^';
			write_buf[write_pos++] = '\\';
			write_buf[write_pos++] = '/';
			write_buf[write_pos++] = ']';
			write_buf[write_pos++] = '*';
			//++write_pos;
		}else if(read_buf[read_pos] == '{'){
			if(write_pos + 20 > (len * 8)){
				error("out of memory!");
			}
			write_buf[write_pos++] = '(';
			read_pos++;
			while(read_buf[read_pos] != '}' && read_pos < len){
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
	write_buf[write_pos++] = '$';

	write_buf[write_pos++] = '\0';
	//post("cmmjl_osc2regex: %s => %s", read_buf, write_buf);
	int e;
	if(e = regcomp(re, write_buf, REG_EXTENDED)){
		return e;
	}
	return 0;
}

int cmmjl_osc_match(void *x, 
		      char *st1, 
		      char *st2){
	regex_t re;
	int e;
	char ebuf[256];

	int len1, len2;
	len1 = strlen(st1);
	len2 = strlen(st2);
	char buf1[len1];
	char buf2[len2];

	char *ptr1_l = st1, *ptr1_r = st1;
	char *ptr2_l = st2, *ptr2_r = st2;

	ptr1_l++;
	ptr1_r++;
	ptr2_l++;
	ptr2_r++;

	while((ptr1_r - st1) < len1 && (ptr2_r - st2) < len2){
		int st1_haspattern = 0, st2_haspattern = 0;
		while(*ptr1_r != '/' && (ptr1_r - st1) < len1){
			buf1[ptr1_r - ptr1_l] = *ptr1_r;
			ptr1_r++;
		}
		buf1[ptr1_r - ptr1_l] = '\0';
		while(*ptr2_r != '/' && (ptr2_r - st2) < len2){
			buf2[ptr2_r - ptr2_l] = *ptr2_r;
			ptr2_r++;
		}
		buf2[ptr2_r - ptr2_l] = '\0';

		if(!strcmp(buf1, buf2)){
			ptr1_l = ++ptr1_r;
			ptr2_l = ++ptr2_r;
			continue;
		}

		char *regex = buf1, *st = buf2;
		if(cmmjl_osc_hasPattern(strlen(buf2), buf2)){
			//post("buf2 has pattern");
			if(cmmjl_osc_hasPattern(strlen(buf1), buf1)){
				error("you can't match a pattern (%s) against another pattern (%s) (yet).", buf1, buf2);
				return 0;
			}else{
				regex = buf2;
				st = buf1;
			}
		}

		if(e = cmmjl_osc2regex(regex, &re)){
			regerror(e, &re, ebuf, 256);
			CMMJL_ERROR(x, CMMJL_OSC_EMATCH, "%s:\n\t%s", 
				    cmmjl_strerror(CMMJL_OSC_EMATCH), ebuf);
			//return e;
			return 0;
		}
		if(e = cmmjl_osc_match_re(&re, st)){
			regerror(e, &re, ebuf, 256);
			CMMJL_ERROR(x, CMMJL_OSC_EMATCH, "%s:\n\t%s", 
				    cmmjl_strerror(CMMJL_OSC_EMATCH), ebuf);
			//return e;
			//if(e == REG_NOMATCH){
				//post("return");
				return ptr2_l - st2 - 1;
				//}
		}
		ptr1_l = ++ptr1_r;
		ptr2_l = ++ptr2_r;
	}

	if(ptr1_r - st1 - 1 == len1 && ptr2_r - st2 - 1 == len2){
		return -1;
	}else{
		return ptr2_r - st2 - 1;
	}

	/*
	char st1_cpy[strlen(st1) + 1];
	char st2_cpy[strlen(st2) + 1];

	strcpy(st1_cpy, st1);
	strcpy(st2_cpy, st2);

	char *ptr1, *ptr2;
	char *lasts1, *lasts2;
	ptr1 = strtok_r(st1_cpy, "/", &lasts1);
	ptr2 = strtok_r(st2_cpy, "/", &lasts2);
	post("ptr2 = %p", ptr2);

	while(ptr1 && ptr2){
		//if(e = cmmjl_osc2regex(st1, &re)){
		if(e = cmmjl_osc2regex(ptr1, &re)){
			regerror(e, &re, ebuf, 256);
			CMMJL_ERROR(x, CMMJL_OSC_EMATCH, "%s:\n\t%s", 
				    cmmjl_strerror(CMMJL_OSC_EMATCH), ebuf);
			//return e;
			return NULL;
		}
		if(e = cmmjl_osc_match_re(&re, ptr2)){
			regerror(e, &re, ebuf, 256);
			CMMJL_ERROR(x, CMMJL_OSC_EMATCH, "%s:\n\t%s", 
				    cmmjl_strerror(CMMJL_OSC_EMATCH), ebuf);
			//return e;
			return ptr2 - 1;
		}
		ptr1 = strtok_r(NULL, "/", &lasts1);
		ptr2 = strtok_r(NULL, "/", &lasts2);
		post("ptr2 = %p", ptr2);
	}
	//return e;
	post("ptr2 = %p", ptr2);
	return ptr2;
	*/
}

int cmmjl_osc_match_re(regex_t *re, char *st){
	return regexec(re, st, 0, NULL, 0);
}

int cmmjl_osc_hasPattern(int len, char *st){
	int i;
	char *ptr = st;
	for(i = 0; i < len; i++){
		switch(*ptr++){
		case '{':
			return 1;
		case '}':
			return 1;
		case '[':
			return 1;
		case ']':
			return 1;
		case '*':
			return 1;
		case '?':
			return 1;
		}

	}
	return 0;
}
