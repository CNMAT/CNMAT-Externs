/*
 Quick hack Maxification of the printing part of dumpOSC
*/


#ifdef WIN_VERSION
// To get ntohl() on Windows
#include </usr/include/w32api/winsock2.h>
#endif

//#define COMPLAIN error
//#define PRINTF post

#include "../../../CNMAT-OSC/dumpOSC/printOSCpacket.c"

#undef COMPLAIN
#define COMPLAIN error

#undef PRINTF
#define PRINTF post
