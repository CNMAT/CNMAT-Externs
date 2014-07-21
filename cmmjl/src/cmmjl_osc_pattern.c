#include "cmmjl_osc_pattern.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char *cmmjl_osc_pattern_match(const char *pattern, const char *address){
	int i;
	while(*address != '\0' && *pattern != '\0'){
		if(*pattern == '*'){
			if(!cmmjl_osc_pattern_match_star(pattern, address)){
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
			if(!cmmjl_osc_pattern_match_single_char(pattern, address)){
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

int cmmjl_osc_pattern_match_star(const char *pattern, const char *address){
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
				if(!(cmmjl_osc_pattern_match_single_char(pp, aa))){
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
				if(!(cmmjl_osc_pattern_match_single_char(pp, aa))){
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
					if(!cmmjl_osc_pattern_match_single_char(pp, aa)){
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
		return cmmjl_osc_pattern_match_star_r(pattern_start, address_start);
		break;
	}
	return 1;
}

int cmmjl_osc_pattern_match_star_r(const char *pattern, const char *address){
	if(*address == '/' || *address == '\0'){
		if(*pattern == '/' || *pattern == '\0' || (*pattern == '*' && (*(pattern + 1) == '/') || *(pattern + 1) == '\0')){
			return 1;
		}else{
			return 0;
		}
	}
	if(*pattern == '*'){
		if(cmmjl_osc_pattern_match_star_r(pattern + 1, address)){
			return 1;
		}else{
			return cmmjl_osc_pattern_match_star_r(pattern, address + 1);
		}
	}else{
		if(!cmmjl_osc_pattern_match_single_char(pattern, address)){
			return 0;
		}
		if(*pattern == '[' || *pattern == '{'){
			while(*pattern != ']' && *pattern != '}'){
				pattern++;
			}
		}
		//pattern++;
		//address++;
		return cmmjl_osc_pattern_match_star_r(pattern + 1, address + 1);
	}
}

int cmmjl_osc_pattern_match_single_char(const char *pattern, const char *address){
	switch(*pattern){
	case '[':
		return cmmjl_osc_pattern_match_bracket(pattern, address);
	case ']':
		while(*pattern != '['){
			pattern--;
		}
		return cmmjl_osc_pattern_match_bracket(pattern, address);
	case '{':
		return cmmjl_osc_pattern_match_curly_brace(pattern, address);
	case '}':
		while(*pattern != '{'){
			pattern--;
		}
		return cmmjl_osc_pattern_match_curly_brace(pattern, address);
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

int cmmjl_osc_pattern_match_bracket(const char *pattern, const char *address){
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

int cmmjl_osc_pattern_match_curly_brace(const char *pattern, const char *address){
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

