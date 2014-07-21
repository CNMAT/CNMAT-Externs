#ifndef Byte
typedef unsigned char Byte ;
#endif
#ifndef SignedByte
typedef signed char SignedByte ;
#endif

short FSOpenPerm(Byte *fileName, short vol, SignedByte perm, short *refNum);

