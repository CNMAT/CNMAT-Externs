/********************************************************************
 *                                                                  
 *  (c) Tactex Controls Inc., 1998-1999                             
 *  All rights reserved.                                            
 *                                                                  
 *  File:    com_low.c               
 *
 *  Release: 1.0.ALPHA
 *
 *  Decription:
 *      This module implements the serial I/O functions for the Macintosh OS.
 *
 *  References:
 *		Inside Macintosh:Devices, Chapter 1 Device Manager
 *		Inside Macintosh:Devices, Chapter 7 Serial Driver
 *		Apple TECHNOTE 1018 - Understanding the DMA Serial Driver
 *		Apple TECHNOTE 1119 - Serial Port Apocrypha
 *  	(Inside the Macintosh Communications Toolbox may also be a good reference
 *		but is impossible to find.)
 */

#include <stdio.h>
#include <string.h>

#include <serial.h>

#include "com.h"
#include "com_low.h"


static pascal void rxCompletion(void);



#define InputBufLen 2048

short	gOutputRefNum; 		/* output driver reference number */
short	gInputRefNum;  		/* input driver reference number */
OSErr	gOSErr;				/* function results */
char 	gInputBuf[InputBufLen + 1];
Boolean gOpenFlg;

static struct 	DeferredTask dt;
static struct 	RoutineDescriptor rxProcDesc = BUILD_ROUTINE_DESCRIPTOR(0,nil);



/* Use the device manager OpenDriver function to open the drivers. */
OSErr openSerialDriver(unsigned char *inName, unsigned char *outName)
	{
	/* open the output driver first */
	gOSErr = OpenDriver(outName, &gOutputRefNum);
	if (gOSErr == noErr) {
		gOSErr = OpenDriver(inName, &gInputRefNum);
		if (gOSErr != noErr)
			CloseDriver(gOutputRefNum);
		}
		
	/* clear the driver reference numbers (according to Technote 1119 */
	if (gOSErr != noErr)
		gInputRefNum = gOutputRefNum = 0;

	/* Replace the default input buffer with a larger buffer. */
	memset(gInputBuf, 0, InputBufLen);
	SerSetBuf(gInputRefNum, gInputBuf, InputBufLen);
	
	/* Set the flag indicating the driver is open. */
	if (gOSErr == noErr)
		gOpenFlg = true;
	else
		gOpenFlg = false;

	/* clear the deferred task address (set in setupDeferredTask() */
	dt.dtAddr = nil;
	
	return gOSErr;
	}
	
	

/* Set the flow control method and other options.  Note that you only need to
   set the output driver; the settings are reflected on the input side. */
OSErr setHandshakeOptions(void)
	{
	SerShk  s; /* serial handshake record */
	
	if (!gOpenFlg)
		return -1;
		
	s.fXOn = 0;
	s.fCTS = 0;
	/* s.xOn  =
	   s.xOff = */
	s.errs = 0;
	s.evts = 0;
	s.fInX = 0;
	s.fDTR = 0;
	
	/* Use control call 14 instead of SerHShake() because it allows control
	   over DTR handshaking */
	gOSErr = Control(gOutputRefNum, kSERDHandshake, &s);
	
	return gOSErr;
	}

	
/* Set the baud rate and data format.  Note that you only need to set the output
   driver; the settings are reflected on the input side. */	
OSErr configureSerialPort(long baudRate)
	{
	if (!gOpenFlg)
		return portNotCf;
		
	gOSErr = SerReset(gOutputRefNum, (baud57600 | data8 | noParity | stop10));
	if (gOSErr)
		return gOSErr;
	
	/* now use the Control function to set higher speeds */
	switch (baudRate) {
		case Com115kBaud:
			gOSErr = Control(gOutputRefNum, kSERD115KBaud, nil);
			break;
		case Com230kBaud:
			gOSErr = Control(gOutputRefNum, kSERD230KBaud, nil);
			break;
		}		
	
	return gOSErr;
	}



/* Fill in the deferred task block. */
OSErr setReceiveCallback(ProcPtr fnPtr)
	{
	if (fnPtr) {
		/* fill in the routine descriptor */
		rxProcDesc.routineRecords[0].procDescriptor = fnPtr;
		
		dt.qType = dtQType; 		/* queue type*/
		dt.dtAddr = &rxProcDesc;	/* pointer to task*/
		dt.dtReserved = 0;			/* reserved; should be 0*/
		}
		
	else
		dt.dtAddr = nil;
	
	return noErr;
	}


	
/* Use the device manager PBWrite() to send data to the output driver. */
void sendMessage(char *txBfr, int length)
	{
	IOParam			pb;
	
	if (!gOpenFlg)
		return;
		
	pb.ioRefNum   = gOutputRefNum; 	/* write to the output driver */
	pb.ioBuffer   = txBfr;			/* pointer to the data */
	pb.ioReqCount = length;			/* number of bytes to send */
	pb.ioCompletion = nil;			/* no completion routine specified */
	pb.ioVRefNum  = 0;				/* not used by serial driver */
	pb.ioPosMode  = 0;				/* not used by serial driver */
	
	
	gOSErr = PBWrite((ParmBlkPtr)&pb, false); /* synchronous device mangaer request */
	}

		
/* Use the Device Manager PBRead() to read data from the input driver. */
long rxMessageWait(char *buffer, int maxLen)
	{
	IOParam			pb;
	ParmBlkPtr		pbPtr;
	union {
		short		csParam[11];
		long		n;
		} 			count;
	
	if (!gOpenFlg)
		return 0L;
		
	count.n = 0;
	
	/* determine how many bytes are in the input buffer. */
	gOSErr = SerGetBuf(gInputRefNum, (void *)count.csParam);
	/*gOSErr = Status(gInputRefNum, kSERDInputCount, (void *)count.csParam);*/
	
	if (count.n > 0) {
		if (count.n > maxLen)
			count.n = maxLen;
			
		pb.ioRefNum		= gInputRefNum; 	/* read from the input driver */
		pb.ioBuffer		= buffer;			/* pointer to my data buffer */
		pb.ioReqCount	= count.n;			/* number of bytes to read */
		pb.ioCompletion = nil;				/* no completion routine specified */
		pb.ioVRefNum	= 0;				/* not used by serial driver */
		pb.ioPosMode	= 0;				/* not used by serial driver */
		
		pbPtr = (ParmBlkPtr)&pb;
		gOSErr = PBRead(pbPtr, false); /* synchronous Device Manager request */
		}

	if (gOSErr == noErr)
		return pb.ioActCount;
	else
		return -1;
	}



char 	*asyRxBuffer;		// ptr to the user's receive buffer
int		*asyRxActCountPtr;	// ptr to report actual count to the user
IOParam	asyRxPb;
struct 	RoutineDescriptor rxComplDesc = BUILD_ROUTINE_DESCRIPTOR(0,rxCompletion);

/* Use the Device Manager PBRead() to read data from the input driver,
// and call the rxProcess() when complete. */
void rxMessageAsync(int rxReqCount, char *rxBfr, int *rxActCount)
	{
	ParmBlkPtr		pbPtr;
//						++MTCErrAccum[MTCErrDEBUG];
	
	if (!gOpenFlg)
		return;
	
	asyRxBuffer = rxBfr;
	asyRxActCountPtr = rxActCount;
		
	asyRxPb.ioRefNum	= gInputRefNum; 	/* read from the input driver */
	asyRxPb.ioBuffer	= asyRxBuffer;			/* pointer to my data buffer */
	asyRxPb.ioReqCount	= rxReqCount;		/* number of bytes to read */
	asyRxPb.ioCompletion = &rxComplDesc;	/* completion routine descriptor */
	asyRxPb.ioVRefNum	= 0;				/* not used by serial driver */
	asyRxPb.ioPosMode	= 0;				/* not used by serial driver */
		
	pbPtr = (ParmBlkPtr)&asyRxPb;
	gOSErr = PBRead(pbPtr, true);     /* async Device Manager request */
	}



/* When the driver completes an asynchronous receive, it calls this function, which
   in turn initiates a deferred task to process the recieve buffer. */
static pascal void rxCompletion(void)
	{	
	*asyRxActCountPtr = asyRxPb.ioActCount;

	if (dt.dtAddr != nil)
		DTInstall( &dt );
	}



/* Use the Device Manager KillIO() to terminate all current input. */
void comFlushInput(void)
	{
	KillIO(gInputRefNum);
	}


/* Use the Device Manager KillIO() to terminate all current and pending operations,
   then close the drivers.     
   Note: according to Inside Mac:Devices Ch.7, you only need to call KillIO() on the output
   driver to terminate both input and output operations. However, Technote 1119: Serial
   Apocrypha (which is more recent) shows an explicit KillIO to both.
*/
OSErr closeSerialDriver(void)
	{
	gOpenFlg = false;
	
	KillIO(gInputRefNum); 						/* Terminate all pending I/O operations. */
	SerSetBuf(gInputRefNum, gInputBuf, 0);		/* Restore the input buffer. */
	gOSErr = CloseDriver(gInputRefNum); 		/* Close the input driver first... */
	
	if (gOSErr == noErr) {
		KillIO(gOutputRefNum);
		gOSErr = CloseDriver(gOutputRefNum);	/* ...then close the output driver. */
		}
		
	return gOSErr;
	}
	
	
