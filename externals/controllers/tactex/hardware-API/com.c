/********************************************************************
 *                                                                  
 *  (c) 1999, Tactex Controls Inc.
 *  All rights reserved.                                 
 *                                                                  
 *                                                                  
 *  File:    com.c                Date:       May 17, 1999          
 *  Module:  tactex                                                 
 *  Project: MCU_pad              Programmer: DL        
 *
 *  Release: 1.0.ALPHA
 *                                                                  
 ********************************************************************
 *                                                                  
 *  Description:													
 *		This module Macintosh-specific details of the serial
 *		communication.
 *																    
 ********************************************************************/


#include "mtc_err.h"
#include "com_low.h"
#include "com.h"


/*#ifndef MAC_PPC
	Crash the compiler here! This file for Macintosh Only.
#endif*/


/* constant definitions */
#define RX_COUNT	140


/* type definitions */
typedef enum {
	ComBfrEmpty,
	ComBfrInProgress,
	ComBfrFull
	} ComBfrBlkFlg;
	
typedef struct COMBFRBLK {
	unsigned char			bfr[1024];
	int						count;
	ComBfrBlkFlg			flg;
	struct COMBFRBLK		*next;
	} ComBfrBlk;


/* global variables */
ComBfrBlk	rBlk1, rBlk2, rBlk3, rBlk4, rBlk5;


/* local variables */
static ComBfrBlk	*receiveBlock;
static Boolean		isInit = FALSE;


/* local function prototypes */
static long		receiveProcess(void);

	

/********************************************************************
 *                                                                  *
 *  Function:			s_init                                      *
 *                                                                  *
 *  Description:                                                    *
 *                                                                  *
 *  Effects:                                                        *
 *                                                                  *
 ********************************************************************/
_MTC_Err com_s_init(ComPB *pb)
	{
	OSErr err;
	
	err = openSerialDriver(pb->inDriver, pb->outDriver);
	if (err != noErr)
		return MTCErrFailOpenDriver;
					
	err = setHandshakeOptions();
	if (err != noErr)
		return MTCErrFailSetHandshake;
					
	err = configureSerialPort(pb->baudRate);
	if (err != noErr)
		return MTCErrFailSetConfigure;
	
	/* set up the receive blocks */
	rBlk1.next = &rBlk2;
	rBlk2.next = &rBlk3;
	rBlk3.next = &rBlk4;
	rBlk4.next = &rBlk5;
	rBlk5.next = &rBlk1;
	
	rBlk1.flg = rBlk2.flg = rBlk3.flg = rBlk4.flg = rBlk5.flg = ComBfrEmpty;
	
			
	/* pick the first rx buffer */
	receiveBlock = &rBlk1;

	/* assign the callback function */
	setReceiveCallback(receiveProcess);

	isInit = TRUE;
	
	return MTCErrNone;
	}	


/********************************************************************
 *                                                                  *
 *  Function:			s_end                                             *
 *                                                                  *
 *  Description:                                                    *
 *		This function uninitializes all support modules.              *
 *                                                                  *
 *  Effects:                                                        *
 *		All memory allocations are freed.
 *                                                                  *
 ********************************************************************/
_MTC_Err com_s_end(void)
	{
	/*	Don't uninitialize if this module has not been initialized. */
	if (isInit != TRUE)
		return(MTCErrModuleNotInit);

	/*	Uninitialize the support module. */
	closeSerialDriver();

	isInit = FALSE;
	
	return(MTCErrNone);
	}


/********************************************************************
 *                                                                  *
 *  Function:			s_command                                         *
 *                                                                  *
 *  Description:                                                    *
 *                                                                  *
 *  Effects:                                                        *
 *                                                                  *
 ********************************************************************/
BOOL	com_s_command(unsigned char *cmd_string, DWORD cmd_length)
	{
	if (isInit != TRUE)
		return(FALSE);

	sendMessage((char *)cmd_string, cmd_length);

	return(TRUE);
	}


/********************************************************************
 *                                                                  *
 *  Function:			s_init_thread                               *
 *                                                                  *
 *  Description:                                                    *
 *                                                                  *
 *  Effects:                                                        *
 *                                                                  *
 ********************************************************************/
_MTC_Err com_s_init_thread(void)
	{
	if (isInit != TRUE)
		return(MTCErrModuleNotInit);

	if (receiveBlock->flg == ComBfrEmpty) {
		/* initiate the receiver */
		receiveBlock->flg = ComBfrInProgress;
		rxMessageAsync(RX_COUNT, (char *)receiveBlock->bfr, &receiveBlock->count);
		return MTCErrNone;
		}
		
	else {
		MTCErrAccum[MTCErrComBufferNotAvail];
		return MTCErrBufferNotEmpty;
		}	
	}


/********************************************************************
 *                                                                  *
 *  Function:			s_end_thread                                      *
 *                                                                  *
 *  Description:                                                    *
 *                                                                  *
 *  Effects:                                                        *
 *                                                                  *
 ********************************************************************/
ComErr		com_s_end_thread(void)
	{	
	return(MTCErrNone);
	}



/********************************************************************
 *                                                                  *
 *  Function:			s_input_buf_size                            *
 *                                                                  *
 *  Description:                                                    *
 *                                                                  *
 *  Effects:                                                        *
 *                                                                  *
 ********************************************************************/
void	com_s_input_buf_size(int size)
	{
	(size);
	return;
	}


	
/* ¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥ */	
/* This is the function which is called by the com module when a buffer
// has been filled. */

extern void parseRxBfr(unsigned char *rxBfr, int rxCount);
	
	
long  receiveProcess(void)
	{
	
	if (receiveBlock->count > 0) {
		/////////// 
		/////////// This is where we can process the received
		/////////// buffer immediately, if we desire.
		///////////

		parseRxBfr(receiveBlock->bfr, receiveBlock->count);
		
		/* signal that the buffer has been processed */
		receiveBlock->flg = ComBfrEmpty;

		///////////
		///////////
		///////////
		
		/* switch buffers */
		receiveBlock = receiveBlock->next;		
		}
	
	else
		receiveBlock->flg = ComBfrEmpty;

	/* If we have an available buffer, ask for more characters. */
	if (receiveBlock->flg == ComBfrEmpty) {
		receiveBlock->flg = ComBfrInProgress; 
		rxMessageAsync(RX_COUNT, (char *)receiveBlock->bfr, &receiveBlock->count);
		}
	else
		MTCErrAccum[MTCErrComBufferNotAvail]++;
	
	return 0;
	}



