/********************************************************************
 *   
 *  (c) 1999, Tactex Controls Inc.
 *  All rights reserved.
 *  
 *  File:    con.c                Date:       May 17, 1999          
 *  Module:  API                                                    
 *  Project: MTC Express          Programmer: DL        
 *
 *  Release: 1.0.ALPHA
 *                                                                  
 ********************************************************************
 *                                                                  
 *  Description:													
 *		This module handles the connection protocol for 
 *		communication with the MTC.
 *																    
 ********************************************************************/
#include "ext.h"

#include <stdlib.h>
#include <string.h>

#include "mtc_err.h"
#include "con.h"
#include "com.h"
#include "com_low.h"


#define EndOfPacket 	0x0d
#define MaxDataLen		256

typedef enum {
	TDFlgEmpty,
	TDFlgRxInProgress,
	TDFlgFull
	} TabletDataFlg;
	
typedef enum {
	TDKindRawData,
	TDKindCompRawData,
	TDKindUnitDesc,
	TDKindAck
	} TabletDataKind;

typedef struct TABLETDATA {
	TabletDataFlg			flg;
	TabletDataKind			kind;
	DWORD					sampleNum;
	int						dataLen;
	unsigned char			data[MaxDataLen];
	struct TABLETDATA		*next;
	} TabletData;



/* receive parser state definitions */	
typedef enum {
	rxCmdCode, 
	rxSampleNum, 
	rxNumBytes, 
	rxTaxelNum, 
	rxTaxelData,
	rxCompTaxelData, 
	rxDescStr,
	rxEndCmd,
	rxWaitForAck
	} rxParserStateT;

/* receive parser control block */
struct PARSERCTL {
	rxParserStateT		state;   	// current state of data reception
	int					index;		// index of data currently being filled
	int					numBytes;	// number of bytes to expect
	unsigned char		lastRxChar; // last processed character
	BOOL				abort;		// flag to cancel current receive
	BOOL				reset;		// flag to reset the receive parser
	TabletData			*current;  	// ptr to data structure
	} rxParserCtl;

/* connection to MTC object */
typedef struct CON_MTC {
	TabletData 		*currentPkt;
	//hComIn		comInput;
	//hComOut		comOutput;
	//rxParserCtl	parserCtl;
	} con_MTC;


	
static TabletData 	*initRxParser(void);
/*static*/ void 	parseRxBfr(unsigned char*,int);



hCon con_OpenConnection(char *inputPort, char *outputPort)
	{
	con_MTC 		*cc;
	ComPB			pb;					
	_MTC_Err 		err = MTCErrNone;
	
	cc = (con_MTC *)getbytes(sizeof(con_MTC));
	
	if (cc) {
		/* initialize the com module */
		pb.inDriver  = (unsigned char *)inputPort;
		pb.outDriver = (unsigned char *)outputPort;
		pb.baudRate  = Com115kBaud;
		err = com_s_init(&pb);
		if (err) {
			++MTCErrAccum[err];
			freebytes(cc, sizeof(con_MTC));
			return NULL;
			}
		
		/* set up the parser */
		cc->currentPkt = initRxParser();

		/* start the receive process */
		err = com_s_init_thread();
		if (err) {
			++MTCErrAccum[err];
			freebytes(cc, sizeof(con_MTC));
			return NULL;
			}
				
		return (hCon)cc;
		}
		
	return NULL;
	}


BOOL  con_CloseConnection(hCon hc)
	{
	if (!hc)
		return FALSE;
	
	/* stop the communications thread */
	com_s_end_thread();

	/* close the serial driver */
	closeSerialDriver();
	
	/* free the connection object memory */
	freebytes(hc, sizeof(con_MTC));

	return TRUE;	
	}


BOOL con_Send_StartSendingData(hCon hc, char comp)
	{
	 char data[4];
	
	if (!hc)
		return FALSE;
	
	/* The only valid compession types are 'n'=none, 'o'=offset, 'd'=difference. */
	if ((comp != 'o') || (comp != 'd'))
		comp = 'n';
		
	data[0] = 'b';
	data[1] = comp;
	data[2] = 0x0d;
	sendMessage(data, 3);
	
	return TRUE;
	}

int con_Recv_PressureData(hCon hc, WORD *newPress)
	{
	int i;
	con_MTC *c;
	
	if (!hc)
		return 0;
	
	c = (con_MTC *)hc;
	
	/* get any bytes remaining in the input buffer */
	comFlushInput();
	
	/* skip to the latest available data */
	while (c->currentPkt->next->flg == TDFlgFull) {
		c->currentPkt->flg = TDFlgEmpty;
		c->currentPkt = c->currentPkt->next;
		}
		
	/* If a packet has been received... */
	if  (c->currentPkt->flg == TDFlgFull) {
		/* Initialize the return value. */
		i = 0;
		
		/* Grab the rec'd data. Only look at data packets. */
		if ((c->currentPkt->kind == TDKindRawData) ||
		    (c->currentPkt->kind == TDKindCompRawData)) {

		 	for (; i < c->currentPkt->dataLen; i++)
		 		newPress[i] = (WORD)(0x00FF & c->currentPkt->data[i]);
			}
		
		/* Release the buffer. */
		c->currentPkt->flg = TDFlgEmpty;
		c->currentPkt = c->currentPkt->next;
	
		/* Return the number of bytes read. */
		return i;
		}
		
	else
		return 0;
	}


BOOL con_Recv_Ack(hCon hc)
	{
	con_MTC *c;
	BOOL	isAck = FALSE;
	
	if (!hc)
		return FALSE;
	
	c = (con_MTC *)hc;
	
	/* get any bytes remaining in the input buffer */
	comFlushInput();
	
	/* skip to last rec'd packet */
	while (c->currentPkt->next->flg == TDFlgFull) {
		c->currentPkt->flg = TDFlgEmpty;
		c->currentPkt = c->currentPkt->next;
		}
	
	if (c->currentPkt->flg == TDFlgFull) {	
		/* If it's ACK, success. */
		if (c->currentPkt->kind == TDKindAck) 
			isAck = TRUE;
					
		/* release the buffer */	
		c->currentPkt->flg = TDFlgEmpty;
		c->currentPkt = c->currentPkt->next;
		}
			
	return isAck;
	}

BOOL con_Send_ReqUnitDescriptor(hCon hc)
	{
	 char data[4];
	
	if (!hc)
		return FALSE;
	
	data[0] = 'u';
	data[1] = 0x0d;
	sendMessage(data, 2);
	
	return TRUE;
	}

	
BOOL con_Recv_UnitDescriptor(hCon hc, char *descStr, int *strLen)
	{
	con_MTC *c;
	BOOL	isUnitDes = FALSE;
	
	if (!hc)
		return FALSE;
	
	c = (con_MTC *)hc;
	
	/* get any bytes remaining in the input buffer */
	comFlushInput();
	
	/* skip to last rec'd packet */
	while (c->currentPkt->next->flg == TDFlgFull) {
		c->currentPkt->flg = TDFlgEmpty;
		c->currentPkt = c->currentPkt->next;
		}
		
	/* If it's a Unit Descriptor, success. */
	if (c->currentPkt->flg == TDFlgFull) {
		if (c->currentPkt->kind == TDKindUnitDesc) {
			isUnitDes = TRUE;
			strcpy(descStr, (char *)c->currentPkt->data);
			*strLen = c->currentPkt->dataLen;
			}
					
		/* release the buffer */	
		c->currentPkt->flg = TDFlgEmpty;
		c->currentPkt = c->currentPkt->next;
		}
			
	return isUnitDes;
	}

BOOL con_Send_StopSendingData(hCon hc)
	{
	 char data[4];
	
	if (!hc)
		return FALSE;
	/* signal the parser to kill the current packet & look for an ACK */
	rxParserCtl.abort = TRUE;
	
	data[0] = 's';
	data[1] = 0x0d;
	sendMessage(data, 2);
	
	
	return TRUE;
	}
	
BOOL con_ResetReceiver(hCon hc)
	{	
	if (!hc)
		return FALSE;
	
	rxParserCtl.reset = TRUE;
	return TRUE;
	}


static TabletData	*initRxParser(void)
	{
	#define NumTabData	10
	static  TabletData 	d[NumTabData];
	int i;
	
	for (i = 0; i < NumTabData - 1; i++) {
		d[i].next = &d[i+1]; 
		d[i].flg = TDFlgEmpty;
		}
	d[i].next = &d[0];
	d[i].flg = TDFlgEmpty;
	
	rxParserCtl.abort = FALSE;
	rxParserCtl.reset = FALSE;
	rxParserCtl.state = rxCmdCode;
	rxParserCtl.current = d;
	rxParserCtl.current->flg = TDFlgRxInProgress;
	
	return rxParserCtl.current;
	}
	
	


/////
///// This is the callback function 
/////

/*static*/ void parseRxBfr(unsigned char *rxBfr, int rxCount)
	{
	TabletData		*d;
	int				iRx;
	unsigned char	rxByte;
	
	d = rxParserCtl.current;
	iRx = 0;
	if (d->flg == TDFlgEmpty)
		d->flg = TDFlgRxInProgress;
	
	// If a reset has been signalled, turn off "abort", and wait 
	// for a command.
	if (rxParserCtl.reset) {
		rxParserCtl.state = rxCmdCode;
		rxParserCtl.lastRxChar = EndOfPacket;
		rxParserCtl.abort = FALSE;
		rxParserCtl.reset = FALSE; /// AF
		}
		
	// If an abort has been signalled, change to a state
	// which waits for an Ack. 
	if (rxParserCtl.abort) {
		rxParserCtl.state = rxWaitForAck;
		rxParserCtl.abort = FALSE;
		}
		
	while ((rxCount > 0) && (d->flg == TDFlgRxInProgress)) {
		
		// get the next byte from the buffer
		rxByte = rxBfr[iRx++]; 
				
		switch (rxParserCtl.state) {
			case rxCmdCode:
				// in this state, we are expecting a command code.  If the
				// rxByte is a valid command move to the next state.
				if (rxParserCtl.lastRxChar == EndOfPacket) {	
					switch (rxByte) {
						case 'd': // Sending_data
							rxParserCtl.state = rxSampleNum;
							d->kind = TDKindRawData;
							break;
						case 'c': // Sending_comp_data
							rxParserCtl.state = rxTaxelNum;
							d->kind = TDKindCompRawData;
							break;
						case 'a': // Ack
							rxParserCtl.state = rxEndCmd;
							d->kind = TDKindAck;
							break;
						case 'u': // Sending_unit_desc
							rxParserCtl.state = rxDescStr;
							rxParserCtl.index = 0;
							d->kind = TDKindUnitDesc;
							break;			
						default: // unexpected character... 
								 // wait for the next command
							break;
						}
					}
				
				else
					/* indicate a synchronization error */
					++MTCErrAccum[MTCErrPktSyncErr];
					
				break;

			case rxSampleNum:
				d->sampleNum = (int)rxByte;
				rxParserCtl.state = rxNumBytes;
				break;
				
			case rxNumBytes:
				if (rxByte > 0) {
					if (rxByte > MaxDataLen)
						rxByte = MaxDataLen;
					d->dataLen = rxParserCtl.numBytes = rxByte;
					rxParserCtl.index = 0;
					rxParserCtl.state = rxTaxelData;
					}
				else
					rxParserCtl.state = rxEndCmd;
				break;
				 
			case rxTaxelNum:
				rxParserCtl.index = rxByte;
				rxParserCtl.state = rxCompTaxelData;
				break;
				
			case rxTaxelData:
				d->data[rxParserCtl.index] = rxByte;
				++rxParserCtl.index;
				if (rxParserCtl.index == rxParserCtl.numBytes)
					rxParserCtl.state = rxEndCmd;
				break;
				
			case rxCompTaxelData:
				d->data[rxParserCtl.index] = rxByte;
				rxParserCtl.state = rxEndCmd;
				break;
				
			case rxDescStr:
				// if it's not the end, continue to fill the string
				if (rxByte != 0x0d) {
					d->data[rxParserCtl.index++] = rxByte;
					break;
					}

				// ...else, end of command was rec'd so terminate the string
				d->data[rxParserCtl.index] = '\0';
				d->dataLen = rxParserCtl.index;
				
				// ... and fall through to finish off...
			
			case rxEndCmd:
				/* if the packet seems valid, signal that the Tablet Data 
				// packet is complete and available */
				if (rxByte == EndOfPacket) {
					d->flg = TDFlgFull;
				
					/* switch to the next packet */
					d = rxParserCtl.current = d->next;
					if (d->flg == TDFlgEmpty)
						d->flg = TDFlgRxInProgress;
					else
						++MTCErrAccum[MTCErrPktBfrNotAvail];
					}
				
				/* ...else it wasn't a valid packet */
				else
					++MTCErrAccum[MTCErrEndOfPkt];
				
				/* set up for the next stream of data */
				rxParserCtl.index = 0;
				rxParserCtl.state = rxCmdCode;
				break;
				
			case rxWaitForAck:
				/* This state occurs when we are trying to sync on
				// an ACK in the middle of a stream of characters.
				// Look for successive characters being 'a',0x0d
				// (There is a 1 in 64k chance of error here.) */
				
				if ((rxByte == EndOfPacket) && (rxParserCtl.lastRxChar == 'a'))
					{
					d->kind = TDKindAck;
					d->flg = TDFlgFull;
				
					/* switch to the next packet */
					d = rxParserCtl.current = d->next;
					if (d->flg == TDFlgEmpty)
						d->flg = TDFlgRxInProgress;
					else
						++MTCErrAccum[MTCErrPktBfrNotAvail];
					
					/* set up for the next stream of data */
					rxParserCtl.index = 0;
					rxParserCtl.state = rxCmdCode;		
					}
				break;
			}
			
		rxParserCtl.lastRxChar = rxByte;
		--rxCount;
		}

	return;
	}
	