/********************************************************************
 *                                                                  
 *  (c) Tactex Controls Inc., 1998-1999                             
 *  All rights reserved.                                           
 *                                                                  
 *  File:    con.c
 *
 *  Release: 1.0.ALPHA
 *
 ********************************************************************/

typedef Boolean 		BOOL;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;

/* handle to the connection */
typedef void *hCon;


hCon con_OpenConnection(char *inputPort, char *outputPort);
BOOL con_CloseConnection(hCon hc);
BOOL con_ResetReceiver(hCon hc);

BOOL con_Send_StartSendingData(hCon hc, char comp);
BOOL con_Send_StopSendingData(hCon hc);
BOOL con_Send_ReqUnitDescriptor(hCon hc);

BOOL con_Recv_UnitDescriptor(hCon hc, char *descStr, int *strLen);
int  con_Recv_PressureData(hCon hc, WORD *newPress);
BOOL con_Recv_Ack(hCon hc);
