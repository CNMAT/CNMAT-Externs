/********************************************************************
 *                                                                  *
 *  (c) Tactex Controls Inc., 1998-1999                             *
 *  All rights reserved.                                            *
 *                                                                  *
 *  File:    cfg.c                Date:       Sept. 14, 1998        *
 *
 *  Release: 1.0.ALPHA
 *                                                                  *
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "cfg.h"

/* Taxel configuration structure */
typedef struct CFG_TAXEL {
	int				xIndex;
	int				yIndex;
	float			xCoord;
	float			yCoord;
	WORD			rawMin;
	WORD			rawMax;
	float			scale;
	} cfg_Taxel;

/* Multi-touch controller configuration structure */
typedef struct CFG_MTC {
	int				numTaxels;
	int				xIndexMax;
	int				yIndexMax;
	float			xCoordMax;
	float			yCoordMax;
	cfg_Taxel		*taxel;
	} cfg_MTC;


hCFG		cfg_new_tablet(int size)
	{
	cfg_MTC *tt;
	
	tt = malloc(sizeof(cfg_MTC));
	if (!tt)
		return NULL;
		
	tt->numTaxels = size;
	tt->taxel = malloc(sizeof(cfg_Taxel) * size);
	
	if (tt->taxel)
		return (hCFG)tt;
	
	else {
		free(tt);	
		return NULL;
		}
	}

	
BOOL	cfg_delete_tablet(hCFG hConfig)
	{
	cfg_MTC *tt;
	
	tt = (cfg_MTC *)hConfig;
	
	if (tt) {
		if (tt->taxel)
			free(tt->taxel);
			
		free(tt);
		
		return TRUE;
		}
	
	return FALSE;
	}
	
BOOL cfg_s_read_map(hCFG hConfig, char *file)
	{
  	int			line_num, results;
  	char  		buffer[200];
  	FILE		*mapping_stream;
	cfg_MTC *tt;
	
	if (!hConfig)
		return FALSE;
	
	tt = (cfg_MTC *)hConfig;

	/* Open the mapping file. */
	mapping_stream = fopen(file, "r");
	if(mapping_stream == NULL)
		return(FALSE);

	/* Initialize the maximum values. */
	tt->xIndexMax = tt->yIndexMax = 0;
	tt->xCoordMax = tt->yCoordMax = 0.0;

	/* Start at the beginning of file. */
	fseek(mapping_stream, 0L, SEEK_SET);

	/* Retrieve data from the mapping file. */
	for(line_num = 0; line_num < tt->numTaxels; line_num++) {
	
		/* Once a comment has been found, remove the rest of the line. */
		while(fgetc(mapping_stream) == '#')
			fgets(buffer, 200 , mapping_stream);

		/* Restore the last character taken by fgetc(). */
		fseek(mapping_stream, -1L, SEEK_CUR);

		/* For a touch tablet connected to the NIDAQ board. */
		results = fscanf(mapping_stream, "%d%d%f%f",
					&tt->taxel[line_num].yIndex, &tt->taxel[line_num].xIndex, 
					&tt->taxel[line_num].xCoord, &tt->taxel[line_num].yCoord);
		
		/* Throw away extra data to the end of line char. */
		fgets(buffer, 200 , mapping_stream);

		/* Decrement the x and y index by one. The x and y index (taxel 
		// coords sys) starts at 1 while arrays in 'c' start at zero. */
		tt->taxel[line_num].xIndex--;
		tt->taxel[line_num].yIndex--;
		
		/* assign the maximum values */
		if (tt->taxel[line_num].xIndex > tt->xIndexMax)
			tt->xIndexMax = tt->taxel[line_num].xIndex;
		if (tt->taxel[line_num].yIndex > tt->yIndexMax)
			tt->yIndexMax = tt->taxel[line_num].yIndex;
		if (tt->taxel[line_num].xCoord > tt->xCoordMax)
			tt->xCoordMax = tt->taxel[line_num].xCoord;
		if (tt->taxel[line_num].yCoord > tt->yCoordMax)
			tt->yCoordMax = tt->taxel[line_num].yCoord;
			
  		}

	/* Close the mapping file. */
	fclose(mapping_stream);

	return(TRUE);
	}


BOOL cfg_s_read_norm(hCFG hConfig, char * file)
	{
  	int 	line_num, results, tempMin;
  	char  	buffer[200];
	FILE	*norm_stream;
	cfg_MTC *tt;
	
	if (!hConfig)
		return FALSE;
		
	tt = (cfg_MTC *)hConfig;

	/* Read the normalization file. */
	norm_stream = fopen(file, "r");
	if (norm_stream == NULL) {
		/* Attempt to open and write a normalization file. */
		norm_stream = fopen(file, "w+");
		if (norm_stream == NULL)
			return(FALSE);
	
		/* Create default normalization file. */
		fprintf(norm_stream, "#%s\n", DEFAULT_TACTEX_NAME);
		fprintf(norm_stream, "#%s\n", DEFAULT_COPYRIGHT_INFO);
		fprintf(norm_stream, "#%s\n", DEFAULT_CONTACT_INFO);
		fprintf(norm_stream, "#\n");
		fprintf(norm_stream, "#%s\n", DEFAULT_NORM_HEADER);
		fprintf(norm_stream, "#\n");
		fprintf(norm_stream, "#%s\n", DEFAULT_NORM_INFO);

	 	for (line_num = 0; line_num < tt->numTaxels; line_num++)
			fprintf(norm_stream, "\t%d\t%f\n", DEFAULT_NORM_MIN, DEFAULT_NORM_SCALE);
		}

	/* Start at the beginning of file. */
	fseek(norm_stream, 0L, SEEK_SET);

	/* Retrieve data from the normalization file. */
	for (line_num = 0; line_num < tt->numTaxels; line_num++) {
		/* Once a comment has been found, remove the rest of the line. */
		while (fgetc(norm_stream) == '#')
			fgets(buffer, 200 , norm_stream);

		/* Restore the last character taken by fgetc().*/
		fseek(norm_stream, -1L, SEEK_CUR);

		results = fscanf(norm_stream, "%d%f", &tempMin, &tt->taxel[line_num].scale);
		tt->taxel[line_num].rawMin = (WORD)tempMin;

		/* Throw away extra data to the end of line char. */
		fgets(buffer, 200 , norm_stream);

		#ifdef  CFG_DEBUG
    	printf("%d\tmin %d\t\tscale %f\n", line_num, tt->taxel[line_num].rawMin, tt->taxel[line_num].scale);
		#endif
  		}

	/* Close the normalization file. */
	if (fclose(norm_stream)) {
		#ifdef	PRINT_TO_CONSOLE
		printf(" Error closing normalization file...\n");
		#endif
		}

	return TRUE;
	}








/********************************************************************
 *                                                                  *
 *  Function:			s_write_norm                                      *
 *                                                                  *
 *  Description:                                                    *
 *                                                                  *
 *  Effects:                                                        *
 *                                                                  *
 ********************************************************************/
BOOL cfg_s_write_norm_data(hCFG hConfig, char * file)
	{
  	FILE  * norm_stream;
  	int   line_num;
	cfg_MTC *tt;
	
	if (!hConfig)
		return FALSE;
	
	tt = (cfg_MTC *)hConfig;

	/* Open and erase the normalization file.*/
	norm_stream = fopen(file, "w");
	if(norm_stream == NULL)
  		return(FALSE);

	/* Create default normalization file header info.*/
	fprintf(norm_stream, "#%s\n", DEFAULT_TACTEX_NAME);
	fprintf(norm_stream, "#%s\n", DEFAULT_COPYRIGHT_INFO);
	fprintf(norm_stream, "#%s\n", DEFAULT_CONTACT_INFO);
	fprintf(norm_stream, "#\n");
	fprintf(norm_stream, "#%s\n", DEFAULT_NORM_HEADER);
	fprintf(norm_stream, "#\n");
	fprintf(norm_stream, "#\t%s %s\n", __DATE__, __TIME__);
	fprintf(norm_stream, "#\n");
	fprintf(norm_stream, "#%s\n", DEFAULT_NORM_INFO);

	for(line_num = 0; line_num < tt->numTaxels; line_num++)
		fprintf(norm_stream, "\t%d\t%f\n",
			(int)tt->taxel[line_num].rawMin, tt->taxel[line_num].scale);

	/* Close the normalization file.*/
	fclose(norm_stream);

	return(TRUE);
	}



int	cfg_g_index_data(hCFG hConfig, int taxelNum)
	{
	cfg_MTC *tt;
			
	if (!hConfig)
		return 0;

	tt = (cfg_MTC *)hConfig;
	return(tt->taxel[taxelNum].xIndex + (tt->taxel[taxelNum].yIndex * tt->numTaxels));
	}

/********************************************************************
 *                                                                  *
 *  Function:			s_norm_min_taxel                            *
 *                                                                  *
 *  Description:                                                    *
 *		This function sets the minimum value of the desired taxel.  *
 *                                                                  *
 *  Effects:                                                        *
 *		None.                                                       *
 *                                                                  *
 ********************************************************************/
void cfg_s_norm_min_taxel(hCFG hConfig, int txl, WORD newMin)
	{
	cfg_MTC *tt;
		
	if (!hConfig)
		return;

	tt = (cfg_MTC *)hConfig;
	tt->taxel[txl].rawMin = newMin;
	return;
	}

/********************************************************************
 *                                                                  *
 *  Function:			s_norm_max_taxel                            *
 *                                                                  *
 *  Description:                                                    *
 *		This function sets the maximum value of the desired taxel.  *
 *                                                                  *
 *  Effects:                                                        *
 *		None.                                                       *
 *                                                                  *
 ********************************************************************/
void	cfg_s_norm_max_taxel(hCFG hConfig, int taxel_num, WORD max)
	{
	cfg_MTC *tt;
		
	if (!hConfig)
		return;

	tt = (cfg_MTC *)hConfig;
	tt->taxel[taxel_num].rawMax = max;
	return;
	}


/********************************************************************
 *                                                                  *
 *  Function:			s_norm_scale_taxel                                *
 *                                                                  *
 *  Description:                                                    *
 *		This function sets the scaling factor for the desired	taxel.  *
 *                                                                  *
 *  Effects:                                                        *
 *		None.                                                         *
 *                                                                  *
 ********************************************************************/
void	cfg_s_norm_scale_taxel(hCFG hConfig, int taxel_num, float scale)
	{
	cfg_MTC *tt;
		
	if (!hConfig)
		return;

	tt = (cfg_MTC *)hConfig;
	tt->taxel[taxel_num].scale = scale;
	return;
	}

/********************************************************************
 *                                                                  *
 *  Function:			g_norm_min_taxel                                  *
 *                                                                  *
 *  Description:                                                    *
 *		This function returns the minimum value of the desired taxel. *
 *                                                                  *
 *  Effects:                                                        *
 *		None.                                                         *
 *                                                                  *
 ********************************************************************/
WORD		cfg_g_norm_min_taxel(hCFG hConfig, int taxel_num)
	{
	cfg_MTC *tt;
	
	if (!hConfig)
		return 0;

	tt = (cfg_MTC *)hConfig;
	return(tt->taxel[taxel_num].rawMin);
	}


/********************************************************************
 *                                                                  *
 *  Function:			g_norm_max_taxel                                  *
 *                                                                  *
 *  Description:                                                    *
 *		This function returns the maximum value of the desired taxel. *
 *                                                                  *
 *  Effects:                                                        *
 *		None.                                                         *
 *                                                                  *
 ********************************************************************/
WORD		cfg_g_norm_max_taxel(hCFG hConfig, int taxel_num)
	{
	cfg_MTC *tt;
	
	if (!hConfig)
		return 0;

	tt = (cfg_MTC *)hConfig;
	return(tt->taxel[taxel_num].rawMax);
	}



/********************************************************************
 *                                                                  *
 *  Function:			g_x_index_taxel                                  *
 *                                                                  *
 *  Description:                                                    *
 *		This function returns the x-index of the specified taxel. *
 *                                                                  *
 *  Effects:                                                        *
 *		None.                                                         *
 *                                                                  *
 ********************************************************************/
int		cfg_g_x_index_taxel(hCFG hConfig, int taxel_num)
	{
	cfg_MTC *tt;
	
	if (!hConfig)
		return 0;

	tt = (cfg_MTC *)hConfig;
	return tt->taxel[taxel_num].xIndex;
	}

/********************************************************************
 *                                                                  *
 *  Function:			g_y_index_taxel                                  *
 *                                                                  *
 *  Description:                                                    *
 *		This function returns the y-index of the specified taxel. *
 *                                                                  *
 *  Effects:                                                        *
 *		None.                                                         *
 *                                                                  *
 ********************************************************************/
int		cfg_g_y_index_taxel(hCFG hConfig, int taxel_num)
	{
	cfg_MTC *tt;
		
	if (!hConfig)
		return 0;

	tt = (cfg_MTC *)hConfig;
	return tt->taxel[taxel_num].yIndex;
	}

/********************************************************************
 Functions:
	cfg_g_x_coord_taxel()
	cfg_g_y_coord_taxel()
 
 Description:
 	These functions return the coords of the specified taxel.
 ********************************************************************/
float	cfg_g_x_coord_taxel(hCFG hConfig, int taxel_num)
	{
	cfg_MTC *tt;
		
	if (!hConfig)
		return 0;

	tt = (cfg_MTC *)hConfig;
	return tt->taxel[taxel_num].xCoord;
	}
	
float	cfg_g_y_coord_taxel(hCFG hConfig, int taxel_num)
	{
	cfg_MTC *tt;
		
	if (!hConfig)
		return 0;

	tt = (cfg_MTC *)hConfig;
	return tt->taxel[taxel_num].yCoord;
	}


/********************************************************************
 Functions:
	cfg_g_x_index_max()
	cfg_g_y_index_max()
	cfg_g_x_coord_max()
	cfg_g_y_coord_max()
 
 Description:
 	These functions return the maximum index/coords of the MTC.
 ********************************************************************/
int		cfg_g_x_index_max(hCFG hConfig)
	{
	if (hConfig)
		return ((cfg_MTC *)hConfig)->xIndexMax;
		
	return 0;
	}

int		cfg_g_y_index_max(hCFG hConfig)
	{
	if (hConfig)
		return ((cfg_MTC *)hConfig)->yIndexMax;
		
	return 0;
	}

float	cfg_g_x_coord_max(hCFG hConfig)
	{
	if (hConfig)
		return ((cfg_MTC *)hConfig)->xCoordMax;
		
	return 0.0;
	}

float	cfg_g_y_coord_max(hCFG hConfig)
	{
	if (hConfig)
		return ((cfg_MTC *)hConfig)->yCoordMax;
		
	return 0.0;
	}

/********************************************************************
 *                                                                  *
 *  Function:			g_normalized_press                          *
 *                                                                  *
 *  Description:                                                    *
 *		This computes the taxel pressure normalization.
 *                                                                  *
 *  Effects:                                                        *
 *		None.                                                         *
 *                                                                  *
 ********************************************************************/
WORD	cfg_g_normalized_press(hCFG hConfig, int taxel_num, WORD raw)
	{
	float temp;
	cfg_Taxel *t;
	
	//// FIRMWARE PATCH //////////////
	const Boolean k_AutoMinimum = true;
	const int     k_AutoFreq = 5;
	static int count = 0;
	///////////////////////////////////
	
	if (!hConfig)
		return 0;

	t = &((cfg_MTC *)hConfig)->taxel[taxel_num];
	
	//// FIRMWARE PATCH //////////////
	// automatically increase the minimum to account for 
	// spurious noise upsetting the firmware offset.
	if (k_AutoMinimum) {
		if (++count > k_AutoFreq) {
			count = 0;
			++t->rawMin;
			}
		
		if (raw < t->rawMin)
			t->rawMin = raw;
		}
	///////////////////////////////////	
	
	temp = (float)raw - (float)t->rawMin;
	if (temp < 0.0)
		temp = 0.0;
		
	return (WORD)(temp * t->scale);
	}


  	
/********************************************************************
Function:
	s_reset_norm_param
	
Description:
 ********************************************************************/
BOOL cfg_s_reset_norm_param(hCFG hTT)
	{  	
	int   taxel_num;
	
	if (!hTT)
		return FALSE;

  	/*Initialize the min, max and scale variables.*/
  	for(taxel_num = 0; taxel_num < ((cfg_MTC *)hTT)->numTaxels; taxel_num++) {
		cfg_s_norm_min_taxel(hTT, taxel_num, CFG_MAX_PRESSURE);
    	cfg_s_norm_max_taxel(hTT, taxel_num, CFG_MIN_PRESSURE);
		cfg_s_norm_scale_taxel(hTT, taxel_num, 0.0);
    	}
    return TRUE;
    }
    	


/********************************************************************
Function:
	s_incr_normalize_taxel
	
Description:
	Incremental normalization - the new data is compared to the 
	stored scaling parameters and the maximum, minimum, and scale 
	values are updated as required.
 ********************************************************************/
void  cfg_s_incr_normalize_taxel(hCFG hConfig, int taxel_num, WORD raw)
	{
	float 		den;
	cfg_Taxel		*t;
	BOOL 		update;
	
	if (!hConfig)
		return;

	t = &((cfg_MTC *)hConfig)->taxel[taxel_num];

  	update = FALSE;
  		
	/* Update the min and max variables */
	if (raw < t->rawMin) {
		t->rawMin = raw;
		update = TRUE;
		}

	if (raw > t->rawMax) {
		t->rawMax = raw;
		update = TRUE;
		}

	/* Determine the scaling factor */
	if (update) {
		den = (float)(t->rawMax - t->rawMin);
		if (den > 0.0)
			t->scale = CFG_FULL_SCALE / den;
		}

	return;
	}
