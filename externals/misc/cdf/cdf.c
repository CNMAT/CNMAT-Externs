#include "version.h"
#include "ext.h"
#include "version.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cdf.h"

struct t_epochNode
{
	double epoch;
	int record;
	struct t_epochNode *next;
	struct t_epochNode *prev;
};
typedef struct t_epochNode t_epochNode;

typedef struct _cdf
{
	t_object c_ob;
	void *c_out0; // data
	void *c_bangout; // bang when done outputting data
	void *c_OSCout; // osc messages
	long c_maxrRec, c_maxzRec;
	CDFid c_id;
	char c_varAttrName[CDF_ATTR_NAME_LEN256];
	t_epochNode *c_epochListHead;
	t_epochNode *c_epochListTail;
} t_cdf;

void *cdf_class;

void cdf_anything(t_cdf *x, t_symbol *msg, short argc, t_atom *argv);
void cdf_bang(t_cdf *x);
void cdf_int(t_cdf *x, long n);
void *cdf_new();
void cdf_free(t_cdf *x);
void cdf_assist(t_cdf *x, void *b, long m, long a, char *s);
void cdf_list(t_cdf *x, t_symbol *msg, short argc, t_atom *argv);
void cdf_float(t_cdf *x, double xx);
void cdf_read(t_cdf *x, t_symbol *fn);
CDFstatus cdf_printGlobalInfo(t_cdf *x);
CDFstatus cdf_outputzVarNames(t_cdf *x);
void cdf_getzVarData(t_cdf *x, t_symbol *msg, short argc, t_atom *argv);
void cdf_getzVarInfo(t_cdf *x, t_symbol *var);
int cdf_getEpochs(t_cdf *x);
long cdf_getRecForEpoch(t_cdf *x, double epoch);
void cdf_searchEpochsFromHead(t_cdf *x, double epoch, t_epochNode **epochNode);
void cdf_searchEpochsFromTail(t_cdf *x, double epoch, t_epochNode **epochNode);
void cdf_outputDone(t_cdf *x, char *OSCstring);

void StatusHandler (CDFstatus status);


//--------------------------------------------------------------------------

int main(void)
{
	setup((t_messlist **)&cdf_class, (method)cdf_new, (method)cdf_free, (short)sizeof(t_cdf), 0L, 0); 
	
	version(0);

	addmess((method) version, "version", 0);
	addbang((method)cdf_bang);
	addint((method)cdf_int);
	addfloat((method)cdf_float);
	addmess((method)cdf_list, "list", A_GIMME, 0);
	addmess((method)cdf_anything, "anything", A_GIMME, 0);
	addmess((method)cdf_assist, "assist", A_CANT, 0);
	addmess((method)cdf_read, "read", A_SYM, 0);
	addmess((method)cdf_getzVarData, "getzVarData", A_GIMME, 0);
	addmess((method)cdf_getzVarInfo, "getzVarInfo", A_DEFSYM, 0);
	
	return 0;
}

void cdf_anything(t_cdf *x, t_symbol *msg, short argc, t_atom *argv){
}

void cdf_bang(t_cdf *x){
}

void cdf_int(t_cdf *x, long n){
}

void cdf_assist(t_cdf *x, void *b, long m, long a, char *s){
	/*
	if (m == ASSIST_OUTLET)
		sprintf(s,"Probability distribution and arguments");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"Random variate");
			break;
		}
	}
	*/
}

void *cdf_new(){
	t_cdf *x;

	x = (t_cdf *)newobject(cdf_class); // create a new instance of this object
	
	x->c_OSCout = outlet_new(x, 0);
	x->c_bangout = bangout(x);
	x->c_out0 = outlet_new(x, 0);
	
	CDFstatus status;    /* CDF completion status. */

	char  subincrement, copyright[CDF_COPYRIGHT_LEN+1];
	long  version, release, increment;

	/******************************************************************************
	* Get the current CDF library version number.
	******************************************************************************/
	status = CDFgetLibraryVersion (&version, &release, &increment, &subincrement);
	if (status != CDF_OK) StatusHandler (status);
	post ("Current CDF library version: CDF %d.%d.%d%c", version, release, increment, subincrement);

	status = CDFgetLibraryCopyright (copyright);
	if (status != CDF_OK) StatusHandler (status);
	post ("CDF library copyright:%s\n", copyright);
	
	x->c_maxzRec = 0;
		   	
	return(x);
}

void cdf_free(t_cdf *x){
}

void cdf_list(t_cdf *x, t_symbol *msg, short argc, t_atom *argv){
}

void cdf_float(t_cdf *x, double xx){
} 

void StatusHandler (CDFstatus status){
	char message[CDF_STATUSTEXT_LEN+1];

	if (status < CDF_WARN) {
		post ("An error has occurred, halting...\n");
	} else if (status < CDF_OK) {
		post ("Warning, function may not have completed as expected...\n");
	} else if (status > CDF_OK) {
		post ("Function completed successfully, but be advised that...\n");
    }       
	CDFgetStatusText (status, message);
	post ("%s\n", message);
	return;
}

void cdf_read(t_cdf *x, t_symbol *fn){
	char *filename = fn->s_name;
	
	CDFstatus status;    // CDF completion status. 

	FILE *fp;            // File pointer - used to read input data file. 

	char  title[80], *buffer, units[50], varName[CDF_VAR_NAME_LEN256+1],
		  attrName[CDF_ATTR_NAME_LEN256+1], subincrement,
		  copyright[CDF_COPYRIGHT_LEN+1];
	short lat;
	int   i, j, time, *image, pixelValue;
	long  TimeVarNum, ImageVarNum, LatVarNum, attrNum, recNum, numDims, recVary,
		  dimIndices[2], dimCounts[2], dimInterval[2], dimSizes[2], dimVarys[2];
	long  datatype, numElements, numRecs, arraySize, attrScope, maxgEntry,
		  maxrEntry, maxzEntry;
	
	
	/******************************************************************************
	* Open the CDF.
	******************************************************************************/

	status = CDFopenCDF (filename, &x->c_id);
	if (status != CDF_OK) StatusHandler (status);
	
	post ("CDF file name: %s", filename);
	status = cdf_printGlobalInfo(x);
	if(status != CDF_OK) StatusHandler(status);
	
	status = cdf_outputzVarNames(x);
	if(status != CDF_OK)StatusHandler(status);
	
	cdf_getEpochs(x);
}

CDFstatus cdf_printGlobalInfo(t_cdf *x){
	CDFstatus status;    // CDF completion status. 
	
	char  title[1024], attrName[CDF_ATTR_NAME_LEN256+1];
	int   i, j;
	long  dimSizes, numDims;
	long  datatype, numElements, attrScope, maxgEntry,
		  maxrEntry, maxzEntry;
	long  encoding, majority, maxrRec, numrVars, maxzRec, numzVars, numAttrs;
	long  version, release, increment;
	
	//status = CDFgetCopyright (id, copyright);
	//if (status != CDF_OK) StatusHandler (status);
	//post ("CDF copyright:%s\n", copyright);

	// Get the CDF version that was used to create this CDF file 
	status = CDFgetVersion (x->c_id, &version, &release, &increment);
	if (status != CDF_OK) return status;

	status = CDFinquireCDF (x->c_id, 
							&numDims, dimSizes,        // only good for rVars 
							&encoding, &majority, 
							&maxrRec, &numrVars,       // only good for rVars 
							&maxzRec, &numzVars, &numAttrs);
	if (status != CDF_OK) return status;
	
	x->c_maxrRec = maxrRec;
	x->c_maxzRec = maxzRec;
	
	post ("\tVersion: CDF %d.%d.%d", version, release, increment);
	post ("\tNumber of variables:  %d rVars, %d zVars", numrVars, numzVars);
	post ("\tNumber of attributes (global & variable): %d", numAttrs);
	post ("\tMax record number for zVariables: %d", maxzRec);
	
	long numGlobalAttrs;
	status = CDFgetNumgAttributes (x->c_id, &numGlobalAttrs); 
	if (status !=  CDF_OK) return status;
	post("\tnumber of global attributes: %d\n", numGlobalAttrs);
	
	// this is a workaround for a wierd bug I was having.  in the for loop
	// the numAttrs var would become very large after a few iterations.
	// seems like a bogus pointer...
	int numberOfAttributes = (int)numAttrs;
	for(i = 0; i < numberOfAttributes; i++){
		status = CDFinquireAttr (x->c_id, i, attrName, &attrScope, 
							 &maxgEntry, 
							 &maxrEntry,
							 &maxzEntry);
		if (status != CDF_OK) return status;
		
		if (attrScope == GLOBAL_SCOPE) {
			for (j = 0; j <= maxgEntry; j++) {
				status = CDFinquireAttrgEntry (x->c_id, i, (long) j, &datatype, 
											   &numElements);
				if (status != CDF_OK) return status;
				
				status = CDFgetAttrgEntry (x->c_id, i, (long) j, title);
				if (status != CDF_OK) return status;
				title[numElements] = '\0';
			}
		} else {
			for(j = 0; j <= maxzEntry; j++){
				status = CDFconfirmzEntryExistence(x->c_id, i, j);
				if(status != NO_SUCH_ENTRY){
					
					status = CDFinquireAttrzEntry (x->c_id, i, (long) j, &datatype, 
												   &numElements);
					if (status != CDF_OK) return status;
					
					switch(datatype){
						case CDF_CHAR:
							status = CDFgetAttrzEntry (x->c_id, i, (long) j, title);
							if (status != CDF_OK) return status;
							title[numElements] = '\0';
							break;
					}
				}
			}
		}
	}
	
	return CDF_OK;
}

CDFstatus cdf_outputzVarNames(t_cdf *x){
	long numzVars;
	char varName[CDF_VAR_NAME_LEN256+1];
	CDFstatus status;
	int i;
	t_atom outArray[2];
	SETSYM(outArray, gensym("/varname"));
	
	status = CDFgetNumzVars(x->c_id, &numzVars);
	if (status != CDF_OK) return status;
	
	for(i = 0; i < numzVars; i++){
		status = CDFgetzVarName(x->c_id, i, varName);
		if(status != CDF_OK) return status;
		SETSYM(outArray + 1, gensym(varName));
		outlet_anything(x->c_OSCout, gensym("list"), 2, outArray);
	}
	
	cdf_outputDone(x, "/varname");
	return CDF_OK;
}

void cdf_getzVarData(t_cdf *x, t_symbol *msg, short argc, t_atom *argv){
	if(argc < 3){
		error("cdf: getzVarData takes 3 or more arguments (see help file).");
		return;
	}
	int i, j;
	CDFstatus status;
	
	char varName[CDF_VAR_NAME_LEN256+1];
	long  numElems, dataType, numDims, recVary, dimSizes[CDF_MAX_DIMS], dimVarys[2];
	t_atom *outArray;
	
	long record_start = 0L;
	long record_end = 0L;
	long record = 0L;
	if(argv[1].a_type != A_SYM){
		error("cdf: the second argument to getzVarData should be a string (see help file).");
		return;
	}
	if(!strcmp(argv[1].a_w.w_sym->s_name, "record")){
		record_start = argv[2].a_w.w_long;
		if(argc > 3) record_end = argv[3].a_w.w_long;
		else record_end = record_start;
	}
	else if(!strcmp(argv[1].a_w.w_sym->s_name, "epoch")){
		// this should be test for reasonableness
		record_start = cdf_getRecForEpoch(x, computeEPOCH(argv[2].a_w.w_long,
														argv[3].a_w.w_long,
														argv[4].a_w.w_long,
														argv[5].a_w.w_long,
														argv[6].a_w.w_long,
														argv[7].a_w.w_long,
														argv[8].a_w.w_long));
		if(argc > 9){
			record_end = cdf_getRecForEpoch(x, computeEPOCH(argv[9].a_w.w_long,
														argv[10].a_w.w_long,
														argv[11].a_w.w_long,
														argv[12].a_w.w_long,
														argv[13].a_w.w_long,
														argv[14].a_w.w_long,
														argv[15].a_w.w_long));
		}
		else record_end = record_start;
	}
	else if(!strcmp(argv[1].a_w.w_sym->s_name, "epochstring")){
		// this string should be tested for valid format
		record_start = cdf_getRecForEpoch(x, parseEPOCH(argv[2].a_w.w_sym->s_name));
		if(argc > 3)
			record_end = cdf_getRecForEpoch(x, parseEPOCH(argv[3].a_w.w_sym->s_name));
		else record_end = record_start;
	}
	
	// we could use CDFhyperGetzVarData, but i'm too lazy to rewrite all this, so for now it goes in a big-ass for loop.
	for(record = record_start; record <= record_end; record++){
		status = CDFinquirezVar (x->c_id, CDFgetVarNum(x->c_id, argv[0].a_w.w_sym->s_name), varName, &dataType,
							 &numElems, &numDims, &dimSizes, &recVary, dimVarys);
		if (status != CDF_OK) StatusHandler (status);
		
		if(dataType == CDF_REAL4 || dataType == CDF_FLOAT || dataType == CDF_REAL8 || dataType == CDF_DOUBLE){
			if(numDims == 0){
				outArray = (t_atom *)calloc(2, sizeof(t_atom));
				if(dataType == CDF_REAL4 || dataType == CDF_FLOAT){
					float data;
					status = CDFgetzVarRecordData (x->c_id, CDFgetVarNum(x->c_id, varName), record, &data);
					if (status != CDF_OK) StatusHandler (status);
					SETSYM(outArray, gensym(varName));
					SETFLOAT(outArray + 1, data);
					outlet_list(x->c_out0, 0L, 2, outArray);
				} else {
					double data;
					status = CDFgetzVarRecordData (x->c_id, CDFgetVarNum(x->c_id, varName), record, &data);
					if (status != CDF_OK) StatusHandler (status);
					SETSYM(outArray, gensym(varName));
					SETFLOAT(outArray + 1, (float)data);
					outlet_list(x->c_out0, 0L, 2, outArray);
				}
				free(outArray);
			} else {
				long totalNumData = 1;
				// if there are 3 dimensions of sizes 2 6 2, the total number of data contained in this 
				// variable record is 2x6x2 = 24;
				for(i = 0; i < numDims; i++)
					totalNumData *= dimSizes[i];
				
				outArray = (t_atom *)calloc(totalNumData + 1, sizeof(t_atom));	
				
				void *data;
				if(dataType == CDF_REAL4 || dataType == CDF_FLOAT){
					data = (float *)calloc(totalNumData, sizeof(float));	
					status = CDFgetzVarRecordData (x->c_id, CDFgetVarNum(x->c_id, varName), record, data);
				} else {
					data = (double *)calloc(totalNumData, sizeof(double));	
					status = CDFgetzVarRecordData (x->c_id, CDFgetVarNum(x->c_id, varName), record, data);
				}
				
				if (status != CDF_OK) StatusHandler (status);
				
				SETSYM(outArray, gensym(varName));
				for(j = 0; j < totalNumData; j++)
					SETFLOAT(outArray + j + 1, *((float *)data + j));
				
				outlet_list(x->c_out0, 0L, (short)totalNumData + 1, outArray);
				free(data);
				free(outArray);
			}
			
		} else if (dataType == CDF_CHAR){		
			
			int arSize = 0;
			int numStrings = 0;
			for(i = 0; i < numDims; i++){
				arSize += dimSizes[i] * numElems;
				numStrings += dimSizes[i];
			}
			arSize += numDims;
			char *data = (char *)calloc(arSize, sizeof(char));
			outArray = (t_atom *)calloc(numStrings + 1, sizeof(t_atom));	
			
			status = CDFgetzVarRecordData (x->c_id, CDFgetVarNum(x->c_id, varName), record, data);
			if (status != CDF_OK) StatusHandler (status);
			
			int k;
			char *tmp = (char *)calloc(256, sizeof(char));
			for(i = 0; i < numDims; i++){
				for(j = 0; j < dimSizes[i]; j++){
					for(k = 0; k < numElems; k++){
						tmp[k] = data[k + (numElems * j)];
						tmp[numElems] = '\0';
					}
					SETSYM(outArray + j + 1, gensym(tmp));
				}
			}
			SETSYM(outArray, gensym(varName));
			outlet_list(x->c_out0, 0L, numStrings + 1, outArray);
			free(data);
			free(tmp);
			free(outArray);
			
		} else if(dataType == CDF_EPOCH){
		
			double data;
			t_atom outArray[2];
			status = CDFgetzVarRecordData (x->c_id, CDFgetVarNum(x->c_id, varName), record, &data);
			if (status != CDF_OK) StatusHandler (status);
			char epstring[EPOCH_STRING_LEN + 1];
			encodeEPOCH(data, epstring);
			SETSYM(outArray, gensym(varName));
			SETSYM(outArray + 1, gensym(epstring));
			outlet_list(x->c_out0, 0L, 2, outArray);
			
		} else {
			error("cdf:  No routine to handle datatype %d.  Contact: johnmac@berkeley.edu", dataType);
		}
	}
	outlet_bang(x->c_bangout);
}

int cdf_getEpochs(t_cdf *x){
	
	char varName[CDF_VAR_NAME_LEN256+1];
	
	CDFid id = x->c_id;
	CDFstatus status;
	long  varNum;
	long  dataType;
	long  numElems;
	char  *buffer;
	
	long  numRecs;
	
	varNum  =  CDFgetVarNum  (id,  "Epoch"); 
	if (varNum  <  CDF_OK)  StatusHandler  (varNum); 
		
	status = CDFgetzVarNumRecsWritten (id, varNum, &numRecs);
	if (status != CDF_OK) StatusHandler (status);
	
	double epoch;
	char epString[EPOCH_STRING_LEN+1];
	
	// construct a doubly-linked list of all the epochs.
	int i;
	t_epochNode *epochNode = (t_epochNode *)malloc(sizeof(t_epochNode));
	epochNode->next = NULL;
	epochNode->prev = NULL;
	x->c_epochListHead = epochNode;
	
	t_atom ar_out[1];
	SETSYM(ar_out, gensym("/epoch"));
	
	for(i = 0; i < numRecs - 1; i++){
		status = CDFgetzVarRecordData(id, varNum, i, &epoch);
		if (status != CDF_OK) StatusHandler (status);
		epochNode->epoch = epoch;
		epochNode->record = i;
		
		encodeEPOCH(epochNode->epoch, epString);
		SETSYM(ar_out + 1, gensym(epString));
		outlet_anything(x->c_OSCout, gensym("list"), 2, ar_out);
		
		epochNode->next = (t_epochNode *)malloc(sizeof(t_epochNode));
		epochNode->next->prev = epochNode;
		epochNode = epochNode->next;
	}
	
	status = CDFgetzVarRecordData(id, varNum, numRecs - 1, &epoch);
	if (status != CDF_OK) StatusHandler (status);
	epochNode->epoch = epoch;
	epochNode->record = numRecs - 1;
	epochNode->next = NULL;
	x->c_epochListTail = epochNode;
	
	/*
	epochNode = x->c_epochListHead;
	while(epochNode){
		encodeEPOCH(epochNode->epoch, epString);
		post("%f, %s", epochNode->epoch, epString);
		epochNode = epochNode->next;
	}
	post("\n**********************************************************\n");
	epochNode = x->c_epochListTail;
	while(epochNode){
		encodeEPOCH(epochNode->epoch, epString);
		post("%f, %s", epochNode->epoch, epString);
		epochNode = epochNode->prev;
	}
	*/
	
	cdf_outputDone(x, "/epoch");
	return 0;
}

void cdf_getzVarInfo(t_cdf *x, t_symbol *var){
	CDFstatus status;
	char  attrName[CDF_ATTR_NAME_LEN256+1];
	int   i, j;
	long  dimSizes, numDims;
	long  datatype, numElements, attrScope, maxgEntry,
		  maxrEntry, maxzEntry;
	long  encoding, majority, maxrRec, numrVars, maxzRec, numzVars, numAttrs, varNum;
	void *entry;
	
	status = CDFinquireCDF (x->c_id, 
							&numDims, dimSizes,        // only good for rVars 
							&encoding, &majority, 
							&maxrRec, &numrVars,       // only good for rVars 
							&maxzRec, &numzVars, &numAttrs);
	if (status != CDF_OK) StatusHandler(status);
	
	varNum = CDFvarNum(x->c_id, var->s_name);
	for(i = 0; i < numAttrs; i++){
		status = CDFinquireAttr (x->c_id, i, attrName, &attrScope, 
							 &maxgEntry, 
							 &maxrEntry,
							 &maxzEntry);
		if (status != CDF_OK) StatusHandler(status);
		if(attrScope == VARIABLE_SCOPE){
			status = CDFconfirmzEntryExistence(x->c_id, i, varNum);
			if(status != NO_SUCH_ENTRY){
				
				status = CDFinquireAttrzEntry (x->c_id, i, varNum, &datatype, 
											   &numElements);
				if (status != CDF_OK) StatusHandler(status);
				
				if(datatype == CDF_REAL4 || datatype == CDF_FLOAT){
					entry = (float *)calloc(numElements, sizeof(float));
					t_atom ar_out[numElements + 2];
					SETSYM(ar_out, gensym("/varinfo"));
					SETSYM(ar_out + 1, gensym(strcat(attrName, ":")));
					
					status = CDFgetAttrzEntry (x->c_id, i, varNum, entry);
					if (status != CDF_OK) StatusHandler(status);
					
					for(j = 0; j < numElements; j++)
						SETFLOAT(ar_out + (j + 2), *((float *)entry + j));
					
					outlet_anything(x->c_OSCout, gensym("list"), numElements + 2, ar_out);
					
				} else if(datatype == CDF_CHAR){
					entry = (char *)calloc(numElements + 1, sizeof(char));
					t_atom ar_out[3];
					SETSYM(ar_out, gensym("/varinfo"));
					SETSYM(ar_out + 1, gensym(strcat(attrName, ":")));
					
					status = CDFgetAttrzEntry (x->c_id, i, varNum, entry);
					if (status != CDF_OK) StatusHandler(status);
					*((char *)entry + numElements) = '\0';
					
					SETSYM(ar_out + 2, gensym(entry));
					outlet_anything(x->c_OSCout, gensym("list"), 3, ar_out);
				} else if(datatype == CDF_EPOCH){
					
				} else {
					error("cdf: no routine to handle data of type %d.  Contact johnmac@berkeley.edu.", datatype);
				}
			}
		}
	}
	free(entry);
	cdf_outputDone(x, "/varinfo");
}

long cdf_getRecForEpoch(t_cdf *x, double epoch){
	t_epochNode *epochNode = (t_epochNode *)malloc(sizeof(t_epochNode));
	if(fabs(x->c_epochListHead->epoch - epoch) <= fabs(x->c_epochListTail->epoch - epoch))
		cdf_searchEpochsFromHead(x, epoch, &epochNode);
	else cdf_searchEpochsFromTail(x, epoch, &epochNode);
	
	return (long)epochNode->record;
}

void cdf_searchEpochsFromHead(t_cdf *x, double epoch, t_epochNode **epochNode){
	*epochNode = x->c_epochListHead;
	double diff = fabs(epoch - (*epochNode)->epoch);
	while(*epochNode){
		if(fabs(epoch - (*epochNode)->epoch) <= diff){
			diff = fabs(epoch - (*epochNode)->epoch);
			*epochNode = (*epochNode)->next;
		} else {
			*epochNode = (*epochNode)->prev;
			return;
		}
	}
	// if we made it this far, it means the epochNode is now NULL, so set it to the tail.
	*epochNode = x->c_epochListTail;
}

void cdf_searchEpochsFromTail(t_cdf *x, double epoch, t_epochNode **epochNode){
	*epochNode = x->c_epochListTail;
	double diff = fabs(epoch - (*epochNode)->epoch);
	while(*epochNode){
		if(fabs(epoch - (*epochNode)->epoch) <= diff){
			diff = fabs(epoch - (*epochNode)->epoch);
			*epochNode = (*epochNode)->prev;
		} else {
			*epochNode = (*epochNode)->next;
			return;
		}
	}
	// if we made it this far, it means the epochNode is now NULL, so set it to the head.
	*epochNode = x->c_epochListHead;
}

void cdf_outputDone(t_cdf *x, char *OSCstring){
	t_atom out[2];
	SETSYM(out, gensym(OSCstring));
	SETSYM(out + 1, gensym("done"));
	outlet_anything(x->c_OSCout, gensym("list"), 2, out);
}
