/*
 Quick hack Maxification of the printing part of dumpOSC
*/


#ifdef WIN_VERSION
// To get ntohl() on Windows
#include </usr/include/w32api/winsock2.h>
#endif

#undef COMPLAIN
#undef PRINTF

#define COMPLAIN error
#define PRINTF post

#include "../../../CNMAT-OSC/dumpOSC/printOSCpacket.c"




