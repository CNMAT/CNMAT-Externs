/********************************************************************
 *                                                                  
 *  (c) Tactex Controls Inc., 1998-1999                             
 *  All rights reserved.                                            
 *                                                                  
 *  File:    mtc_err.h
 *
 *  Release: 1.0.ALPHA
 *
 ********************************************************************/

#ifndef	MTC_ERR
#define MTC_ERR


typedef enum {
		MTCErrNone,
		MTCErrFailOpenDriver,
		MTCErrFailSetHandshake,
		MTCErrFailSetConfigure,
		MTCErrBufferNotEmpty,
		MTCErrModuleNotInit,
		MTCErrNoMemory,
		MTCErrMapFile,
		MTCErrNormalFile,
		MTCErrConnectFailed,
		MTCErrComBufferNotAvail,
		MTCErrPktBfrNotAvail,
		MTCErrEndOfPkt,
		MTCErrPktSyncErr,
		MTCErrNoDescriptor,
		MTCErrStopDataNotAck,
		MTCErrStartDataFailed,
		MTCErrGetDescrFailed,
		MTCErrDEBUG,
		MTCErrCOUNT
		} _MTC_Err;


extern int MTCErrAccum[MTCErrCOUNT];


#endif