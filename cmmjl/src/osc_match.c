#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <regex.h>

void random_osc(int len, char *pattern, char *address);
int rand_int(int min, int max);

static const char *match_pattern(const char *pattern, const char *address);
static int match_star(const char *pattern, const char *address);
static int match_star_r(const char *pattern, const char *address);
static int match_singleChar(const char *pattern, const char *address);
static int match_bracket(const char *pattern, const char *address);
static int match_curlyBrace(const char *pattern, const char *address);

static const char *theWholePattern;
int PatternMatch (const char *  pattern, const char * test) ;
static int MatchBrackets (const char *pattern, const char *test);
static int MatchList (const char *pattern, const char *test);


int cmmjl_osc_hasPattern(int len, char *st);
int cmmjl_osc2regex(char *osc_string, regex_t *re);
int cmmjl_osc_match(void *x, 
		    char *st1, 
		    char *st2);
int cmmjl_osc_match_re(regex_t *re, char *st);
int cmmjl_osc_hasPattern(int len, char *st);

typedef struct _range{
	char min, max;
} t_range;

static t_range ranges[] = {(t_range){48, 57}, (t_range){65, 90}, (t_range){97, 122}};
static numRanges = 3;

int main(int argc, const char **argv){
	unsigned long long t = mach_absolute_time();
	srand((unsigned long)(t & 0x00000000fffffffflu));
	char p[1024], a[1024];

	if(argc == 3){
		unsigned long long start, end, elapsed;
		start = mach_absolute_time();
		const char *result1 = match_pattern(argv[1], argv[2]);
		end = mach_absolute_time();
		elapsed = end - start;
		Nanoseconds elapsedNano = AbsoluteToNanoseconds(*(AbsoluteTime *)&elapsed);
		printf("done in %llu nanoseconds\n", *(unsigned long long *)&elapsedNano);
		//printf("%s (%d)\n", result1 > 0 ? "match" : "no match", result1);
		char *match_type;
		if(!result1){
			match_type = "no match";
		}else if(result1 == argv[2] + strlen(argv[2])){
			match_type = "full match";
		}else{
			match_type = "partial match";
		}
		printf("%s: %s\n", match_type, result1);

		start = mach_absolute_time();
		int result2 = PatternMatch(argv[1], argv[2]);
		end = mach_absolute_time();
		elapsed = end - start;
		elapsedNano = AbsoluteToNanoseconds(*(AbsoluteTime *)&elapsed);
		printf("done in %llu nanoseconds\n", *(unsigned long long *)&elapsedNano);
		printf("%s\n", result2 ? "match" : "no match");
		return;
	}

	int n = 128;
	int i;
	Nanoseconds e1[n], e2[n];
	Nanoseconds total_nano;
	unsigned long long total;
	unsigned long long total_start, total_end, total_elapsed;
	total_start = mach_absolute_time();
	for(i = 0; i < n; i++){
		printf("__________________________________________________\n");
		memset(p, '\0', 1024);
		memset(a, '\0', 1024);
		random_osc(20, p, a);

		printf("%s %s\n", p, a);
		unsigned long long start, end, elapsed;
		start = mach_absolute_time();
		const char *result1 = match_pattern(p, a);
		end = mach_absolute_time();
		elapsed = end - start;
		e1[i] = AbsoluteToNanoseconds(*(AbsoluteTime *)&elapsed);
		printf("done in %llu nanoseconds\n", *(unsigned long long *)&elapsed);
		//printf("%s\n", result ? "match" : "no match");
		char *match_type;
		if(!result1){
			match_type = "no match";
		}else if(result1 == a + strlen(a)){
			match_type = "full match";
		}else{
			match_type = "partial match";
		}
		printf("%s: %s\n", match_type, result1);

		start = mach_absolute_time();
		int result2 = PatternMatch(p, a);
		end = mach_absolute_time();
		elapsed = end - start;
		e2[i] = AbsoluteToNanoseconds(*(AbsoluteTime *)&elapsed);
		printf("done in %llu nanoseconds\n", *(unsigned long long *)&elapsed);

		start = mach_absolute_time();
		int result3 = cmmjl_osc_match(NULL, p, a);
		end = mach_absolute_time();
		elapsed = end - start;
		e2[i] = AbsoluteToNanoseconds(*(AbsoluteTime *)&elapsed);
		printf("done in %llu nanoseconds\n", *(unsigned long long *)&elapsed);
		//printf("%s\n", result ? "match" : "no match");

		/*
		if(result1 != result2){
			printf("matching %s against %s %s in the new code and %s in the old\n", p, a, result1 ? "SUCCEEDED" : "FAILED", result2 ? "SUCCEEDED" : "FAILED");
		}
		*/
	}
	total_end = mach_absolute_time();
	total_elapsed = total_end - total_start;
	total_nano = AbsoluteToNanoseconds(*(AbsoluteTime *)&total_elapsed);
	total = *(unsigned long long *)&total_nano;
	printf("matched %d patterns in %llu nanoseconds (%f seconds)\n", n, total, total / 1e9);
}

void random_osc(int len, char *pattern, char *address){
	int i;
	char *a = address, *p = pattern;
	int star = 0;
	*a++ = '/';
	*p++ = '/';
	for(i = 0; i < len; i++){
		if(*(a - 1) != '/'){
			if((double)rand() / RAND_MAX < .05){
				*a++ = '/';
				*p++ = '/';
				continue;
			}
		}
		int r = rand_int(0, 2);
		switch(r){
		case 0:
			r = rand_int(48, 57);
			break;
		case 1:
			r = rand_int(65, 90);
			break;
		case 2:
			r = rand_int(97, 122);
			break;
		}
		*a++ = (char)r;

		if(star){
			if((double)rand() / RAND_MAX < .1){
				continue;
			}else{
				star = 0;
			}
		}
		if(r = (double)rand() / RAND_MAX < .1){
			r = rand_int(0, 3);
			switch(r){
			case 0:
				// *
				*p++ = '*';
				star = 1;
				break;
			case 1:
				// ?
				*p++ = '?';
				break;
			case 2:
				// []
				{
					*p++ = '[';
					int n = rand_int(1, 5);
					int match = rand_int(0, n - 1);
					//int match = -1;
					int j;
					for(j = 0; j < n; j++){
						if(j == match){
							if(rand_int(0, 1)){
								*p++ = *(a - 1);
							}else{
								int k = 0;
								while(k < numRanges){
									if(*(a - 1) >= ranges[k].min && *(a - 1) <= ranges[k].max){
										*p++ = (char)rand_int(ranges[k].min, *(a - 1));
										*p++ = '-';
										*p++ = (char)rand_int(*(a - 1), ranges[k].max);
										break;
									}
									k++;
								}
							}
						}else{
							t_range range = ranges[rand_int(0, numRanges - 1)];
							if(rand_int(0, 1)){
								*p++ = rand_int(range.min, range.max);
							}else{
								*p++ = rand_int(range.min, range.max - 1);
								*p++ = '-';
								*p++ = rand_int(*(p - 2), range.max);
							}
						}
					}
					*p++ = ']';
				}
				break;
			case 3:
				// {}
				{
					*p++ = '{';
					int n = rand_int(1, 7);
					int match = rand_int(0, n - 1);
					//int match = -1;
					int i;
					for(i = 0; i < n; i++){
						if(match == i){
							*p++ = *(a - 1);
						}else{
							t_range range = ranges[rand_int(0, numRanges - 1)];
							*p++ = rand_int(range.min, range.max);
						}
						*p++ = ',';
					}
					*(p - 1) = '}';
				}
				break;
			}
		}else{
			*p++ = *(a - 1);
		}
	}
	
}

int rand_int(int min, int max){
	return round(((double)rand() / RAND_MAX) * (max - min)) + min;
}

static const char *match_pattern(const char *pattern, const char *address){
	int i;
	while(*address != '\0' && *pattern != '\0'){
		if(*pattern == '*'){
			if(!match_star(pattern, address)){
				//printf("match failed\n");
				//return 0;
				return NULL;
			}
			while(*pattern != '/' && *pattern != '\0'){
				pattern++;
			}
			while(*address != '/' && *address != '\0'){
				address++;
			}
		}else{
			if(!match_singleChar(pattern, address)){
				//printf("match failed\n");
				//return 0;
				return NULL;
			}
			if(*pattern == '[' || *pattern == '{'){
				while(*pattern != ']' && *pattern != '}'){
					pattern++;
				}
			}
			pattern++;
			address++;
		}
	}

	if(*address == '\0'){
		//printf("complete match\n");
		//return 1;
		return address;
	}else if(*address == '/'){
		//printf("partial match: %s\n", address);
		//return 1;
		return address;
	}else{
		//printf("match failed\n");
		//return 0;
		return NULL;
	}
}

static int match_star(const char *pattern, const char *address){
	const char *address_start = address;
	const char *pattern_start = pattern;
	int num_stars = 0;
	while(*address != '/' && *address != '\0'){
		address++;
	}
	while(*pattern != '/' && *pattern != '\0'){
		if(*pattern == '*'){
			num_stars++;
		}
		pattern++;
	}
	pattern--;
	address--;
	switch(num_stars){
	case 1:
		{
			const char *pp = pattern, *aa = address;
			while(*pp != '*'){
				//printf("%c %c\n", *pp, *aa);
				if(!(match_singleChar(pp, aa))){
					return 0;
				}
				if(*pp == ']' || *pp == '}'){
					while(*pp != '[' && *pp != '{'){
						pp--;
					}
				}
				pp--;
				aa--;
			}
		}
		break;
	case 2:
		{
			const char *pp = pattern, *aa = address;
			while(*pp != '*'){
				if(!(match_singleChar(pp, aa))){
					return 0;
				}
				if(*pp == ']' || *pp == '}'){
					while(*pp != '[' && *pp != '{'){
						pp--;
					}
				}
				pp--;
				aa--;
			}
			aa++; // we want to start one character forward to allow the star to match nothing
			const char *star2 = pp;
			const char *test = aa;
			int i = 0;
			while(test > address_start){
				pp = star2 - 1;
				aa = test - 1;
				i++;
				while(*pp != '*'){
					if(!match_singleChar(pp, aa)){
						break;
					}
					if(*pp == ']' || *pp == '}'){
						while(*pp != '[' && *pp != '{'){
							pp--;
						}
					}
					pp--;
					aa--;
				}
				if(pp == pattern_start){
					return 1;
				}
				test--;
			}
			return 0;
		}
		break;
	default:
		return match_star_r(pattern_start, address_start);
		break;
	}
	return 1;
}

static int match_star_r(const char *pattern, const char *address){
	if(*address == '/' || *address == '\0'){
		if(*pattern == '/' || *pattern == '\0' || (*pattern == '*' && (*(pattern + 1) == '/') || *(pattern + 1) == '\0')){
			return 1;
		}else{
			return 0;
		}
	}
	if(*pattern == '*'){
		if(match_star_r(pattern + 1, address)){
			return 1;
		}else{
			return match_star_r(pattern, address + 1);
		}
	}else{
		if(!match_singleChar(pattern, address)){
			return 0;
		}
		if(*pattern == '[' || *pattern == '{'){
			while(*pattern != ']' && *pattern != '}'){
				pattern++;
			}
		}
		//pattern++;
		//address++;
		return match_star_r(pattern + 1, address + 1);
	}
}

static int match_singleChar(const char *pattern, const char *address){
	switch(*pattern){
	case '[':
		return match_bracket(pattern, address);
	case ']':
		while(*pattern != '['){
			pattern--;
		}
		return match_bracket(pattern, address);
	case '{':
		return match_curlyBrace(pattern, address);
	case '}':
		while(*pattern != '{'){
			pattern--;
		}
		return match_curlyBrace(pattern, address);
	case '?':
		return 1;
	default:
		if(*pattern == *address){
			return 1;
		}else{
			return 0;
		}
	}
	return 0;
}

static int match_bracket(const char *pattern, const char *address){
	int matched = 0;
	pattern++;
	while(*pattern != ']' && *pattern != '\0'){
		// the character we're on now is the beginning of a range
		if(*(pattern + 1) == '-'){
			if(*address >= *pattern && *address <= *(pattern + 2)){
				matched = 1;
				break;
			}else{
				pattern += 3;
			}
		}else{
			// just test the character
			if(*pattern == *address){
				matched = 1;
				break;
			}
			pattern++;
		}
	}
	return matched;
	/*
	if(matched){
		while(*pattern != ']' && *pattern != '\0'){
			pattern++;
		}
		pattern++;
		address++;
	}else{
		return 0;
	}
	return 1;
	*/
}

static int match_curlyBrace(const char *pattern, const char *address){
	int matched = 0;
	pattern++;
	while(*pattern != '}' && *pattern != '\0' && *pattern != '/'){
		if(*pattern == *address){
			matched = 1;
			break;
		}else{
			pattern++;
			if(*pattern == ','){
				pattern++;
			}
		}
	}

	if(matched){
		while(*pattern != '}' && *pattern != '\0' && *pattern != '/'){
			pattern++;
		}
		pattern++;
		address++;
	}else{
		return 0;
	}

	return 1;
}


/**************************************************
 * Recursive pattern matching
 **************************************************/
int PatternMatch (const char *  pattern, const char * test) {
	theWholePattern = pattern;
  
	if (pattern == 0 || pattern[0] == 0) {
		return test[0] == 0;
	} 
  
	if (test[0] == 0) {
		if (pattern[0] == '*')
			return PatternMatch (pattern+1,test);
		else
			return 0;
	}

	switch (pattern[0]) {
	case 0      : return test[0] == 0;
	case '?'    : return PatternMatch (pattern + 1, test + 1);
	case '*'    : 
		if (PatternMatch (pattern+1, test)) {
			return 1;
		} else {
			return PatternMatch (pattern, test+1);
		}
	case ']'    :
	case '}'    :
		//OSCWarning("Spurious %c in pattern \".../%s/...\"",pattern[0], theWholePattern);
		return 0;
	case '['    :
		return MatchBrackets (pattern,test);
	case '{'    :
		return MatchList (pattern,test);
	case '\\'   :  
		if (pattern[1] == 0) {
			return test[0] == 0;
		} else if (pattern[1] == test[0]) {
			return PatternMatch (pattern+2,test+1);
		} else {
			return 0;
		}
	default     :
		if (pattern[0] == test[0]) {
			return PatternMatch (pattern+1,test+1);
		} else {
			return 0;
		}
	}
}


/* we know that pattern[0] == '[' and test[0] != 0 */

static int MatchBrackets (const char *pattern, const char *test) {
	int result;
	int negated = 0;
	const char *p = pattern;

	if (pattern[1] == 0) {
		//OSCWarning("Unterminated [ in pattern \".../%s/...\"", theWholePattern);
		return 0;
	}

	if (pattern[1] == '!') {
		negated = 1;
		p++;
	}

	while (*p != ']') {
		if (*p == 0) {
			//OSCWarning("Unterminated [ in pattern \".../%s/...\"", theWholePattern);
			return 0;
		}
		if (p[1] == '-' && p[2] != 0) {
			if (test[0] >= p[0] && test[0] <= p[2]) {
				result = !negated;
				goto advance;
			}
		}
		if (p[0] == test[0]) {
			result = !negated;
			goto advance;
		}
		p++;
	}

	result = negated;

 advance:

	if (!result)
		return 0;

	while (*p != ']') {
		if (*p == 0) {
			//OSCWarning("Unterminated [ in pattern \".../%s/...\"", theWholePattern);
			return 0;
		}
		p++;
	}

	return PatternMatch (p+1,test+1);
}

static int MatchList (const char *pattern, const char *test) {
	const char *restOfPattern, *tp = test;


	for(restOfPattern = pattern; *restOfPattern != '}'; restOfPattern++) {
		if (*restOfPattern == 0) {
			//OSCWarning("Unterminated { in pattern \".../%s/...\"", theWholePattern);
			return 0;
		}
	}

	restOfPattern++; /* skip close curly brace */


	pattern++; /* skip open curly brace */

	while (1) {
		if (*pattern == ',') {
			if (PatternMatch (restOfPattern, tp)) {
				return 1;
			} else {
				tp = test;
				++pattern;
			}
		} else if (*pattern == '}') {
			return PatternMatch (restOfPattern, tp);
		} else if (*pattern == *tp) {
			++pattern;
			++tp;
		} else {
			tp = test;
			while (*pattern != ',' && *pattern != '}') {
				pattern++;
			}
			if (*pattern == ',') {
				pattern++;
			}
		}
	}

}



/**************************************************
 * Regex
 **************************************************/

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
				printf("out of memory!\n");
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
				printf("out of memory!\n");
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
				printf("out of memory!\n");
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
				printf("you can't match a pattern (%s) against another pattern (%s) (yet).\n", buf1, buf2);
				return 0;
			}else{
				regex = buf2;
				st = buf1;
			}
		}

		if(e = cmmjl_osc2regex(regex, &re)){
			regerror(e, &re, ebuf, 256);
			//CMMJL_ERROR(x, CMMJL_OSC_EMATCH, "%s:\n\t%s", 
			//cmmjl_strerror(CMMJL_OSC_EMATCH), ebuf);
			//return e;
			return 0;
		}
		if(e = cmmjl_osc_match_re(&re, st)){
			regerror(e, &re, ebuf, 256);
			//CMMJL_ERROR(x, CMMJL_OSC_EMATCH, "%s:\n\t%s", 
			//cmmjl_strerror(CMMJL_OSC_EMATCH), ebuf);
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
