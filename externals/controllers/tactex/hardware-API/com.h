/********************************************************************
 *                                                                  
 *  (c) Tactex Controls Inc., 1998-1999                             
 *  All rights reserved.                                            
 *                                                                  
 *  File:    com.h
 *
 *  Release: 1.0.ALPHA
 *
 ********************************************************************/


typedef Boolean 		BOOL;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;

#ifndef	COM
#define COM


/*
 *	Definitions
 */
#define	DEFAULT_IN_QUEUE				1000
#define	DEFAULT_OUT_QUEUE				1000


/*
 *	Configuration
 */


/*
 *	Types and Structures
 */
 
typedef struct {
	unsigned char		*inDriver;
	unsigned char		*outDriver;
	long				baudRate;
	} ComPB, *ComPBPtr;

#define Com230kBaud 230400L
#define Com115kBaud 115200L

//typedef enum {
//	ErrNone = 0,
//	ErrFailOpenDriver,
//	ErrFailSetHandshake,
//	ErrFailSetConfigure,
//	ErrBufferNotEmpty,
//	ErrModuleNotInit
//	} ComErr;
#include "mtc_err.h"
typedef _MTC_Err ComErr;






/*
 *	Function prototypes
 */
 
/* initialization and wind-up */
extern ComErr 	com_s_init(ComPBPtr);
extern ComErr	com_s_end(void);


/* Communication routines */
extern ComErr 	com_s_init_thread(void);
extern ComErr	com_s_end_thread(void);
extern BOOL		com_s_command(unsigned char *cmd_string, DWORD cmd_length);


/* Buffer access routines */
extern BOOL		com_s_buf_release(void);
extern BOOL		com_g_buf_access(void);
extern int		com_g_sample_num(void);
extern DWORD 	*com_g_buf_data(int sample_num);

extern void		com_s_input_buf_size(int size);


#endif
