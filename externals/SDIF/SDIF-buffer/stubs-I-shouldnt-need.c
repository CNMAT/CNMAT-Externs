#include "ext.h"

void num2dec(void);
void num2dec(void) {
	post("¥¥ num2dec was called");
}

void DisposeTextToUnicodeInfo(void);
void DisposeTextToUnicodeInfo(void) {
	post("¥¥ DisposeTextToUnicodeInfo was called");
}

/* void __msl_text2unicode(void);
void __msl_text2unicode(void) {
	post("¥¥ __msl_text2unicode was called");
}
*/

void ConvertFromTextToUnicode(void);
void ConvertFromTextToUnicode(void) {
	post("¥¥ ConvertFromTextToUnicode");
}

void CreateTextToUnicodeInfoByEncoding(void);
void CreateTextToUnicodeInfoByEncoding(void) {
	post("¥¥ CreateTextToUnicodeInfoByEncoding");
}

pascal long UpgradeScriptInfoToTextEncoding(short, short, short, const unsigned char *, unsigned long *) {
	post("¥¥ UpgradeScriptInfoToTextEncoding");
}