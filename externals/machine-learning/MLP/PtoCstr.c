/*
   MW 030627 - Can't figure out how to link correct version of PtoCstr, so here's
   a version I found on the web.
 */

char *PtoCstr(char* src);
char *PtoCstr(char* src) {
	int i;

	/* Hack -- pointer */
	char *s = (char*)(src);

	/* Hack -- convert the string */
	for (i = s[0]; i; i--, s++) s[0] = s[1];

	/* Hack -- terminate the string */
	s[0] = '\0';
	
	return src;
}
