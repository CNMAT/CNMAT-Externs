/********************************************************************
 *                                                                  
 *  (c) Tactex Controls Inc., 1998-1999                             
 *  All rights reserved.                                            
 *                                                                  
 *  File:    mtc.h
 *
 *  Release: 1.0.ALPHA
 *
 ********************************************************************/

#ifndef	MTC
#define MTC

typedef Boolean 		BOOL;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;

typedef WORD MTCErr;

typedef struct MTCCREATE {
         char   *inputPort;    // name of the input driver
         char   *outputPort;   // name of the output driver
         char   *mappingFile;  // path and filename of the mapping file
         char   *normalFile;   // path and filename of the normalization file
} MTCCreate;

typedef unsigned long MTCHandle;

typedef enum {
		MTCCompDefault = 0,
        MTCCompNone,
        MTCCompDiff,
        MTCCompThresh
} MTCCompType;

typedef struct MTCCONFIG {
        int          nTaxels;
        int          nRows;
        int          nCols;
        float        xDimension;
        float        yDimension;
        char        *unitDescriptor;
} MTCConfig;


MTCHandle	MTC_New(MTCCreate  mtcCreate);
BOOL		MTC_Delete(MTCHandle hMTC);
MTCErr		MTC_GetLastError(MTCHandle hMTC);
char 		*MTC_GetErrorString(MTCErr errCode);
BOOL		MTC_GetAPIVersion(char *apiVer);
BOOL   		MTC_GetConfig(MTCHandle hMTC, MTCConfig *config);
int  		MTC_GetDescriptorString(MTCHandle hMTC, char *descStr, int waitSeconds);
BOOL		MTC_StartSendingData(MTCHandle hMTC, MTCCompType compressionType, int waitSeconds);
BOOL		MTC_StopSendingData(MTCHandle hMTC, int waitSeconds);
BOOL		MTC_BeginNormalization(MTCHandle hMTC);
BOOL		MTC_EndNormalization(MTCHandle hMTC, BOOL saveToDisk);
BOOL		MTC_GetRawData(MTCHandle hMTC, WORD *dataPtr);
BOOL		MTC_GetNormalizedData(MTCHandle hMTC, WORD *dataPtr);
float		MTC_SetSampleRate(MTCHandle hMTC, float sampleRate);
float		MTC_GetSampleRate(MTCHandle hMTC);
float		MTC_GetTaxelX(MTCHandle hMTC, int taxelNum);
float		MTC_GetTaxelY(MTCHandle hMTC, int taxelNum);
int  		MTC_GetTaxelXIndex(MTCHandle hMTC, int taxelNum);
int  		MTC_GetTaxelYIndex(MTCHandle hMTC, int taxelNum);


#endif