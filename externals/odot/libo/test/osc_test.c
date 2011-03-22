#include "osc.h"
#include "osc_bundle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int len, char **argv){
	char buf[1024];
	char *bufptr = buf;
	bufptr += 4;
	char *address = "/foo/barf";
	strcpy(bufptr, address);
	bufptr += strlen(address);
	bufptr++;
	while((bufptr - buf) % 4){
		bufptr++;
	}
	*bufptr++ = ',';
	*bufptr++ = 's';
	*bufptr++ = '\0';
	*bufptr++ = '\0';

	char *data = "foo";
	strcpy(bufptr, data);
	bufptr += 3;
	*bufptr++ = '\0';

	*((uint32_t *)buf) = hton32(bufptr - buf - 4);

	char buf2[1024];
	char *bufptr2 = buf;
	int n;
	n = osc_bundle_bundleNakedMessage(bufptr - buf, buf, &bufptr2);
	printf("n = %d\n", n);
	int i;
	for(i = 0; i < n; i++){
		printf("%d: %c\n", i, bufptr2[i]);
	}
	osc_bundle_printBundle(n, bufptr2, printf);
	/*
	char buf[1024];
	memset(buf, '\0', 1024);
	char *ptr = buf;
	osc_bundle_setBundleID(buf);
	//strncpy(buf, "#bundle\0", 8);
	ptr += OSC_HEADER_SIZE;
	//ptr += 16;
	char *sizeptr = ptr;
	ptr += 4;
	strncpy(ptr, "/foo\0\0\0\0", 8);
	ptr += 8;
	strncpy(ptr, ",i\0\0", 4);
	ptr += 4;
	*((uint32_t *)ptr) = hton32(666);
	ptr += 4;
	*((uint32_t *)sizeptr) = hton32((ptr - sizeptr) - 4);

	t_osc_bundle bundle;
	t_osc_err ret;
	ret = osc_bundle_makeBundle(ptr - buf, buf, &bundle);
	if(ret){
		printf("%s (%d)\n", osc_error_string(ret), (int)ret); 
	}

	printf("num_messages = %d\n", bundle.num_messages);
	int i;
	for(i = 0; i < bundle.num_messages; i++){
		printf("msg %d with address %s has size %d\n", i, bundle.messages[i].address, bundle.messages[i].size);
	}

	t_osc_msg *mmmm = NULL;
	osc_bundle_lookupAddress(&bundle, "/foo", &mmmm, 1);
	if(mmmm){
		printf("here's your stupid message: %s\n", mmmm->address);
	}

	char *renamed = NULL;
	int newlen = osc_message_rename(bundle.messages[0].size, (*bundle.messages).address, "/barf", &renamed);
	printf("oldlen = %d, newlen = %d\n", bundle.messages[0].size + 4, newlen);
	t_osc_msg m;
	osc_message_parseMessage(0, renamed, &m);
	printf("%s\n", m.address);

	t_osc_msg *mm = NULL;
	osc_bundle_lookupAddressSerialized(ptr - buf, buf, "/foo", &mm, 0);
	printf("mm = %p\n", mm);
	if(mm){
		printf("address = %s\n", mm->address);
	}

	t_osc_msg *clone = NULL;
	osc_message_deepCopy(&clone, mm);
	if(clone){
		printf("cloned address = %s\n", clone->address);
	}
	osc_message_free(mm);
	osc_message_free(clone);
	*/
}
