/********************************************************************
 *                                                                  
 *  (c) Tactex Controls Inc., 1998-1999                             
 *  All rights reserved.                                            
 *                                                                  
 *  File:    mtc.c                
 *
 *  Release: 1.0.ALPHA
 *                                                                  
 ********************************************************************/

#define NORMALIZATION_FILENAME "MTCnormal.txt" // added these -rd
#define TAXELMAPPING_FILENAME "MTCmapping.txt"


#include "ext.h"

#include <string.h>
#include <time.h>

#include "mtc.h"
#include "mtc_err.h"
#include "cfg.h"
#include "con.h"

////
#define REPLACETHIS128  128
////

typedef struct MTC_OBJECT {
	    int			nTaxels;
        int			xIndexMax;
        int			yIndexMax;
        float		xDimension;
        float		yDimension;
        char        descriptor[80];
		hCon 		connection;
		MTCErr   	lastErr;
		hCFG 		config; 
		BOOL		normalOn;
		} MTC_object;


/* global error accumulator */
int MTCErrAccum[MTCErrCOUNT];

static char *errorString[MTCErrCOUNT + 1] = {
		"MTCErrNone",
		"MTCErrFailOpenDriver",
		"MTCErrFailSetHandshake",
		"MTCErrFailSetConfigure",
		"MTCErrBufferNotEmpty",
		"MTCErrModuleNotInit",
		"MTCErrNoMemory",
		"MTCErrMapFile",
		"MTCErrNormalFile",
		"MTCErrConnectFailed",
		"MTCErrComBufferNotAvail",
		"MTCErrPktBfrNotAvail",
		"MTCErrEndOfPkt",
		"MTCErrPktSyncErr",
		"MTCErrNoDescriptor",
		"MTCErrStopDataNotAck",
		"MTCErrStartDataFailed",
		"MTCErrGetDescrFailed",
		"MTCErrDEBUG",
		"MTCErrCOUNT"
		};
	
static char			*MTC_VersionString = "Tactex Multi-Touch Controller API, V1.00ALPHA";
static hCon 		mtcConnection;
/*static*/ hCFG 	mtcConfigPtr; 
static BOOL			mtcNormalInProgress = FALSE;

static MTC_object   mtcObj;

/********************************************************************
Function:
	MTC_New()

Description:
	This function creates an instance of a Tactex MTC object.  In 
	the process, this function does the following:
	a.	allocates memory for the MTC object;
	b.	reads the mapping (configuration) file from disk;
	c.	reads the normalization file from disk.  If a 
		normalization file with the name specified does not 
		exist, then one is automatically created;
	d.	opens the serial communication driver;
	e.	"pings" the MTC, and verifies the reponse against 
		the information in the mapping file.

Returns:
	MTC_New() returns a handle to a MTC instance.  It 
	returns NULL if the function failed to create a new instance 
	of an MTC.  MTC_GetLastError() can be called with a NULL 
	argument to determine the cause of failure.
 ********************************************************************/
MTCHandle  MTC_New(MTCCreate  mtcCreate)
{
	int e;
	//post("1 Initialize the error counter",0);
	/* Initialize the error counter. */
	for (e = MTCErrNone; e < MTCErrCOUNT; e++)
		MTCErrAccum[e] = 0;
	//post("2 set up the connection to the MTC",0);
	/* set up the connection to the MTC */
	mtcConnection = con_OpenConnection(mtcCreate.inputPort, mtcCreate.outputPort);
	if (mtcConnection) 
		{
		//post("3 allocate memory for the configuration",0);
		/* allocate memory for the configuration */
		mtcConfigPtr = cfg_new_tablet(REPLACETHIS128);
		if (mtcConfigPtr) 
			{
			//post("4 read the mapping file",0);
			/* read the mapping file */
			if (cfg_s_read_map(mtcConfigPtr, TAXELMAPPING_FILENAME)) 
				{
				//post("5 read the normalization file",0);
				/* read the normalization file */
				if (cfg_s_read_norm(mtcConfigPtr, NORMALIZATION_FILENAME)) 
					{
					//post("6 Request a unit descriptor string from MTC",0);
					/* Request a unit descriptor string from MTC.*/
					//if (MTC_GetDescriptorString(1, mtcObj.descriptor, 2)) 
					//	{
						/* Everything was successful, so return a handle. */
						return (MTCHandle)1;	
					//	}
					
					//else
					//	++MTCErrAccum[MTCErrNoDescriptor];
					}	
				
				else
					++MTCErrAccum[MTCErrNormalFile];
	
				}
			
			else
				++MTCErrAccum[MTCErrMapFile];
			
			cfg_delete_tablet(mtcConfigPtr);
			mtcConfigPtr = NULL;		
			}
		
		else 
			++MTCErrAccum[MTCErrNoMemory];

		con_CloseConnection(mtcConnection);
		mtcConnection = NULL;
		}	
	
	else
		++MTCErrAccum[MTCErrConnectFailed];
		
	return FALSE;
}
	


/********************************************************************
Function:
	MTC_Delete()

Description:
	This function stops the communication with the MTC identified 
	by hMTC and frees the memory allocated with the particular MTC.
 ********************************************************************/
BOOL   MTC_Delete(MTCHandle hMTC)
{
	if (!hMTC)
		return FALSE;
	
	/* stop the data flow */
	MTC_StopSendingData(hMTC, 2);
	
	/* close the connection */
	con_CloseConnection(mtcConnection);
	mtcConnection = NULL;
	
	/* release the configuration */
	cfg_delete_tablet(mtcConfigPtr);
	mtcConfigPtr = NULL;
	
	/* turn off normalization */
	mtcNormalInProgress = FALSE;
	
	return TRUE;
}



/********************************************************************
Function:
	MTC_GetLastError()

Description:
	This function returns the error code of the last error.  If hMTC 
	is NULL, then it returns errors unassociated with a specific MTC.  
	If hMTC is assigned a valid handle, then it returns the last 
	error associated with that MTC.  Once an error has been reported, 
	it is cleared (i.e. subsequent calls to MTC_GetLastError() will 
	not report the same event).  However, some errors are accumulated 
	and MTC_GetLastError() may be called successively in order to 
	retreive them.
 ********************************************************************/
MTCErr   MTC_GetLastError(MTCHandle hMTC)
{
	_MTC_Err e;
	
	for (e = MTCErrNone; e < MTCErrCOUNT; e++) {
		if (MTCErrAccum[e]) {
			MTCErrAccum[e] = 0;
			return (MTCErr)e;
			}
		}
		
	return (MTCErr)MTCErrNone;
}


/********************************************************************
Function:
	MTC_GetErrorString()

Description:
	This function returns a pointer to a constant char string
	describing the error.
 ********************************************************************/
char *MTC_GetErrorString(MTCErr errCode)
{	
	return errorString[errCode];
}




/********************************************************************
Function:
	MTC_GetAPIVersion()
	
Description:
	This function fills the buffer pointed to by apiVer with the API 
	version string.  

Parameters:
	apiVer - pointer to a buffer which must be at least 50 bytes long

Returns:
	MTC_GetAPIVersion() returns TRUE.
 ********************************************************************/
BOOL   MTC_GetAPIVersion(char *apiVer)
{
	strcpy(apiVer, MTC_VersionString);
	return TRUE;
}
	
	
	

/********************************************************************
Function:
	MTC_GetConfig()

Description:
	This function fills the structure with the configuration data of 
	the specified MTC.

Parameters:
	hMTC - 	handle of the MTC from which the configuration 
			data is requested.
	config - pointer to a MTCConfig struct to be filled 
			with the configuration data.
Returns:
	MTC_GetVersion() returns TRUE if successful, FALSE otherwise.
 ********************************************************************/
BOOL   MTC_GetConfig(MTCHandle hMTC, MTCConfig *config)
{
	if (!hMTC)
		return FALSE;
		
	config->nTaxels = REPLACETHIS128;
	config->nCols = cfg_g_x_index_max(mtcConfigPtr);
	config->nRows = cfg_g_y_index_max(mtcConfigPtr);
	config->xDimension = cfg_g_x_coord_max(mtcConfigPtr) * (float)(1 + config->nCols)/config->nCols;
	config->yDimension = cfg_g_y_coord_max(mtcConfigPtr) * (float)(1 + config->nRows)/config->nRows;
	config->unitDescriptor = mtcObj.descriptor;
	
	return TRUE;
}
	
	

/********************************************************************
Function:
	MTC_StartSendingData()

Description:
	This function initiates the stream of data from the MTC.
 ********************************************************************/
BOOL   MTC_StartSendingData(MTCHandle hMTC, MTCCompType compressionType, int waitSeconds)
{
	clock_t start, now, resend;
	WORD	temp[REPLACETHIS128];
	
	if (!hMTC)
		return FALSE;
	
	resend = start = clock();
	
	do {
		/* Return success if data is being received. */
		if (con_Recv_PressureData(mtcConnection,temp) > 0)
			return TRUE;
			
		now = clock();
		
		/* Periodically (i.e. every .25 s) send a request for data */
		if (now > resend) {
			con_Send_StartSendingData(mtcConnection, (char)'n'); // ALPHA: uses only non-compressed mode
			resend += CLOCKS_PER_SEC / 4;
			}
		}
	while ((now - start) < (CLOCKS_PER_SEC * waitSeconds));
	
	/* Log an error. */	
	++MTCErrAccum[MTCErrStartDataFailed];

	return FALSE;
}
	
	
/********************************************************************
Function:
	MTC_StopSendingData()

Description:
	This function stops the stream of data from the specified MTC.
	It does so by periodically sending a Stop Data command, and
	polling for an ACK.
 ********************************************************************/
BOOL  MTC_StopSendingData(MTCHandle hMTC, int waitSeconds)
{
	clock_t	start, now, resend;

	if (!hMTC)
		return FALSE;
	
	resend = start = clock();
			
	do {
		/* Data has stopped if an ACK has been rec'd. */
		if (con_Recv_Ack(mtcConnection))
			return TRUE;
			
		now = clock();
			
		/* Periodically transmit a Stop_send_data command. */
		if (now > resend) {
			con_Send_StopSendingData(mtcConnection);
			resend += CLOCKS_PER_SEC / 4;
			}
		}
	while ((now - start) < (CLOCKS_PER_SEC * waitSeconds));

	/* If an ACK has not been received after a con_Send_StopSendingData(),
	// then we must reset the receive parser. */
	con_ResetReceiver(mtcConnection);
	
	/* log an error */
	++MTCErrAccum[MTCErrStopDataNotAck];
	
	return FALSE;
}



/********************************************************************
Function:
	MTC_GetUnitDescriptorString()

Description:
	This function stops data flow from the MTC and requests the MTC
	to return its Unit Descriptor String.
	
Returns:
	The length of the unit descriptor string.
 ********************************************************************/
int  MTC_GetDescriptorString(MTCHandle hMTC, char *descStr, int waitSeconds)
{
	clock_t		start, now, resend;
	int 		strLen;

	if (!hMTC)
		return 0;

	resend = start = clock();

	if (!MTC_StopSendingData(hMTC, waitSeconds))
		return 0;
		
	do {
		/* return when we've received the unit descriptor */
		if (con_Recv_UnitDescriptor(mtcConnection, descStr, &strLen))
			return strLen;
			
		now = clock();
		
		/* Periodically request the unit descriptor */
		if (now > resend) {
			con_Send_ReqUnitDescriptor(mtcConnection);
			resend += CLOCKS_PER_SEC / 4;
			}
		}
	while ((now - start) < (CLOCKS_PER_SEC * waitSeconds));

	/* Since the time ran out, log an error. */
	++MTCErrAccum[MTCErrGetDescrFailed];

	return 0;
}
	
	
/********************************************************************
Function:
	MTC_BeginNormalization()

Description:
	This function starts the normaliation procedure.
 ********************************************************************/	
BOOL  MTC_BeginNormalization(MTCHandle hMTC)
{
	if (!hMTC)
		return FALSE;
		
	cfg_s_reset_norm_param(mtcConfigPtr);
	mtcNormalInProgress = TRUE;
	return TRUE;
}
	
	
	
/********************************************************************
Function:
	MTC_EndNormalization()

Description:
	This function completes the normaliation.  If saveToDisk is TRUE, 
	then the normalization file is overwriMTCen, otherwise the 
	normalization data continues to be used for this session, but 
	the file is not overwritten.
 ********************************************************************/
BOOL  MTC_EndNormalization(MTCHandle hMTC, BOOL saveToDisk)
{
	if (!hMTC)
		return FALSE;
		
	mtcNormalInProgress = FALSE;
  	
  	if (saveToDisk)
	  	cfg_s_write_norm_data(mtcConfigPtr, NORMALIZATION_FILENAME);
	  	
	return TRUE;
}


	
/********************************************************************
Function:
	MTC_GetRawData()

Description:
	This function fills the array pointed to by newPress with the 
	taxel raw pressure data received from the MTC.  The data is not 
	normalized.
 ********************************************************************/
BOOL  MTC_GetRawData(MTCHandle hMTC, WORD *newPress)
{
	int taxelsRead, i;
	
	if (!hMTC)
		return FALSE;
	
	taxelsRead = con_Recv_PressureData(mtcConnection, newPress);
	
	if (taxelsRead) {
		/* if normalization is turned on, then update the parameters */
		if (mtcNormalInProgress) {	
			for (i = 0; i < taxelsRead; i++)	
	 			cfg_s_incr_normalize_taxel(mtcConfigPtr, i, newPress[i]);
			}
			
		return TRUE;
		}
		
	else
		return FALSE;
}



/********************************************************************
Function:
	MTC_GetNormalizedData()

Description:
	This function fills the array pointed to by dataPtr with taxel 
	normalized pressure data.
	
Returns:
	TRUE if new data is available.
	FALSE if there is no new data.
 ********************************************************************/
BOOL  MTC_GetNormalizedData(MTCHandle hMTC, WORD *newPress)
{
	int taxelsRead, i;
	
	if (!hMTC)
		return FALSE;
		
	taxelsRead = con_Recv_PressureData(mtcConnection, newPress);
	
	if (taxelsRead) {
		for (i = 0; i < taxelsRead; i++) {		
			/* if normalization is turned on, then update the parameters */
			if (mtcNormalInProgress)
	 			cfg_s_incr_normalize_taxel(mtcConfigPtr, i, newPress[i]);
	 				
		 	/* convert raw to normalized pressure */
		 	newPress[i] = cfg_g_normalized_press(mtcConfigPtr, i, newPress[i]);
			}
		return TRUE;
		}
		
	else
		return FALSE;
}



/********************************************************************
Function:
	MTC_SetSampleRate()

Description:
	This function is used to alter the frequency with which the MTC 
	reports data to the host computer.

Parameters:
	hMTC - 	handle of the MTC from which the configuration 
			data is requested.
	sampleRate - the fequency, in Hz, at which the MTC is 
			requested to report.
Returns:
	This function  returns the actual sample rate that was set, 
	in Hz (samples per second).
 ********************************************************************/
float  MTC_SetSampleRate(MTCHandle hMTC, float sampleRate)
{
	if (!hMTC)
		return 0.0;
		
	return 0.0;
}


	
/********************************************************************
Function:
	MTC_GetSampleRate()

Description:

Returns: 
	This function returns the sample rate, in Hz (samples per second).
 ********************************************************************/
float  MTC_GetSampleRate(MTCHandle hMTC)
{
	if (!hMTC)
		return 0.0;
		
	return 0.0;
}
	

	
/********************************************************************
Functions:
	MTC_GetTaxelX()
	MTC_GetTaxelY()

Description:
	These functions return the coordinates of the specified taxel.
 ********************************************************************/
float  MTC_GetTaxelX(MTCHandle hMTC, int taxelNum)
{
	if (!hMTC)
		return 0.0;
		
	return cfg_g_x_coord_taxel(mtcConfigPtr, taxelNum);
}

float  MTC_GetTaxelY(MTCHandle hMTC, int taxelNum)
{
	if (!hMTC)
		return 0.0;
		
	return cfg_g_y_coord_taxel(mtcConfigPtr, taxelNum);
}



/********************************************************************
Functions:
	MTC_GetTaxelXIndex()
	MTC_GetTaxelYIndex()

Description:
	These functions return the indices of the specified taxel.
 ********************************************************************/
int  MTC_GetTaxelXIndex(MTCHandle hMTC, int taxelNum)
{
	if (!hMTC)
		return 0;
		
	return cfg_g_x_index_taxel(mtcConfigPtr, taxelNum);
}
	
int  MTC_GetTaxelYIndex(MTCHandle hMTC, int taxelNum)
{	
	if (!hMTC)
		return 0;
		
	return cfg_g_y_index_taxel(mtcConfigPtr, taxelNum);
}
