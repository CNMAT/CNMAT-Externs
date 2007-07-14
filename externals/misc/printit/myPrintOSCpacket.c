/*
 Quick hack Maxification of the printing part of dumpOSC
*/


#ifdef WIN_VERSION
// To get ntohl() on Windows
#include </usr/include/w32api/winsock2.h>
#endif

#define COMPLAIN error
#define PRINTF post

#include "../../../../OSC/dumpOSC/printOSCpacket.c"
