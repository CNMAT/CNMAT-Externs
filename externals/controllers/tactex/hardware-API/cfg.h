/********************************************************************
 *                                                                  
 *  (c) Tactex Controls Inc., 1998-1999                             
 *  All rights reserved.                                            
 *                                                                  
 *  File:    cfg.h
 *
 *  Release: 1.0.ALPHA
 *
 ********************************************************************/


typedef Boolean 		BOOL;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;

#ifndef	CFG
#define	CFG

/*
 *	Include files
 */

/*
 *	Definitions
 */
#define	DEFAULT_TACTEX_NAME				"\t\tTactex Controls Inc."
#define	DEFAULT_COPYRIGHT_INFO		"\t\t   Copyright 1998"
#define	DEFAULT_CONTACT_INFO			"\t   www.tactex.com tci@tactex.com"
#define	DEFAULT_NORM_HEADER				"\tTouch Tablet Normalization File"
#define	DEFAULT_NORM_INFO					"\tMin\tScale"
#define	DEFAULT_NORM_SCALE				1.0
#define	DEFAULT_NORM_MIN					0

#define	CFG_FULL_SCALE  					255		//	 8 bit
#define	CFG_MAX_PRESSURE					255
#define	CFG_MIN_PRESSURE					0		  //	 8 bit


#define	CFG_THRESH								30
/*
 *	Support for other digitizers.
 *
#define	CFG_MAX_PRESSURE					0x3ff	//	10 bit
#define	CFG_MAX_PRESSURE					0xfff	//	12 bit
#define	CFG_MAX_PRESSURE					0xffff	//	16 bit
#define	CFG_MAX_PRESSURE					0x7fff	//	16 bit signed
 *
 */

/*
 *	Types and Structures
 */
 
/* Handle of a MTC */
typedef void *hCFG;


/*
 *	Function prototypes
 */

hCFG		cfg_new_tablet(int size);

BOOL	cfg_s_read_map(hCFG hConfig, char * file);
BOOL	cfg_delete_tablet(hCFG hConfig);



void	cfg_g_xy_map_data(int sample_num,
					float * x_coord_ptr,
					float * y_coord_ptr);
float	cfg_g_x_map_data(int x_index, int y_index);
float	cfg_g_y_map_data(int x_index, int y_index);

BOOL	cfg_s_read_norm(hCFG hConfig, char * file);
BOOL	cfg_s_write_norm_data(hCFG hConfig, char * file);

int		cfg_g_norm_min(int x_index, int y_index);
float	cfg_g_norm_scale(int x_index, int y_index);
void	cfg_s_norm_min(int x_index, int y_index, int min);
void	cfg_s_norm_scale(int x_index, int y_index, float scale);

WORD	cfg_g_norm_min_taxel(hCFG hConfig, int taxel);
WORD	cfg_g_norm_max_taxel(hCFG hConfig, int taxel);
float	cfg_g_norm_scale_taxel(int taxel);
void	cfg_s_norm_min_taxel(hCFG hConfig, int taxel, WORD min);
void	cfg_s_norm_max_taxel(hCFG hConfig, int taxel, WORD max);
void	cfg_s_norm_scale_taxel(hCFG hConfig, int taxel, float scale);
int		cfg_g_x_index_taxel(hCFG hConfig, int taxel_num);
int		cfg_g_y_index_taxel(hCFG hConfig, int taxel_num);
float	cfg_g_x_coord_taxel(hCFG hConfig, int taxel_num);
float	cfg_g_y_coord_taxel(hCFG hConfig, int taxel_num);
WORD	cfg_g_normalized_press(hCFG hConfig, int taxel_num, WORD rawPress);
void	cfg_s_incr_normalize_taxel(hCFG hConfig, int taxel_num, WORD rawPress);
BOOL 	cfg_s_reset_norm_param(hCFG hConfig);
int		cfg_g_index_data(hCFG hConfig, int taxel_num);
int		cfg_g_x_index_max(hCFG hConfig);
int		cfg_g_y_index_max(hCFG hConfig);
float	cfg_g_x_coord_max(hCFG hConfig);
float	cfg_g_y_coord_max(hCFG hConfig);


unsigned short *cfg_g_thresh_ptr(int unit_num);


#endif