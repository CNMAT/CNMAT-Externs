/********************************************************************
 *                                                                  
 *  (c) Tactex Controls Inc., 1998-1999                             
 *  All rights reserved.                                            
 *                                                                  
 *  File:    com_low.h
 *
 *  Release: 1.0.ALPHA
 *
 ********************************************************************/

extern OSErr openSerialDriver(unsigned char*, unsigned char*);
extern OSErr closeSerialDriver(void);
extern OSErr setHandshakeOptions(void);
extern OSErr configureSerialPort(long);
extern void rxMessageAsync(int,char*,int*);
extern long rxMessageWait(char*,int);
extern void sendMessage(char*, int);
extern void comFlushInput(void);
extern OSErr setReceiveCallback(ProcPtr);

