#define	SCOPE	extern
#include "NNInclude.h"

#define skip_whitespace(x)\
{\
	while (*x == ' ' || *x == '\n' || *x == '\t' || *x == '\r') {\
		x = (char*)x + 1;\
	}\
	while (!(*x == ' ' || *x == '\n' || *x == '\t' || *x == '\r')) {\
		x = (char*)x + 1;\
	}\
	while (*x == ' ' || *x == '\n' || *x == '\t' || *x == '\r') {\
		x = (char*)x + 1;\
	}\
}

	
/* ------------------------- nn_doread --------------------- */

extern double random();

void	*nn_doread(NeuralNetPtr x,Symbol *fileName)
{
    OSErr	err;
	double	*weights,slope;
	long	i,j,k,in,hid,hid_layers,out;
	short	file;
	long	old_size,index,size=0;
	char	*old_ptr;
	char	*file_ptr;
	int		savelock;
	char	filename[256];
	short	vol;
	SFTypeList	typelist;
	OSType	dstType;

	strcpy((char*)filename,fileName->s_name);
	if (fileName==gensym("")||locatefiletype(filename,&vol,0L,0L)){
#ifdef OPEN_DIALOG_WORKS  // MW couldn't make this compile 021211
		/* do open dialog if no file given or not found */
		if (open_dialog(filename,
		(&vol),
		(&dstType),
		typelist,
		 0))
#endif
			goto CloseIt;		/* didn't want to open */
	}	
	if (err = FSOpen(CtoPstr(filename),vol,&file))
		goto CloseIt;
#ifdef debug
post("before file_size is: %ld",size);
#endif
	GetEOF(file,&size);
#ifdef debug
post("file_size is: %ld",size);
#endif
	old_ptr = file_ptr = (char*)getbytes((short)size);
#ifdef debug
post("old_ptr: %ld", (long)old_ptr);
#endif
	if (!old_ptr) {
		FSClose(file);
		goto CloseIt;
	}
#ifdef debug
post("file_size is: %ld",size);
post("old_ptr: %ld", (long)old_ptr);
#endif
	FSRead(file,&size,file_ptr);
	FSClose(file);

	old_size = size;
	if (!sscanf(file_ptr,"%ld",&in)) goto bail;
	skip_whitespace(file_ptr);
	if (!sscanf(file_ptr,"%ld",&hid)) goto bail;
	skip_whitespace(file_ptr);
	if (!sscanf(file_ptr,"%ld",&hid_layers)) goto bail;
	skip_whitespace(file_ptr);
	if (!sscanf(file_ptr,"%ld",&out)) goto bail;
	skip_whitespace(file_ptr);	
		
#ifdef debug
post("IN: %ld HID: %ld ",in,hid);
post("LAYERS: %ld OUT: %ld",hid_layers,out);
#endif	
#ifdef never
	if ((in != x->nn_net->numIN) || (out != x->nn_net->numOUT) ||
		(hid < x->nn_net->numHID)) {
		post("RNNet: This file is for a network with %ld input, %ld hidden,",in,hid);
		post("		 and %ld output units.  Please reconfigure network with Get Info.",out);
		freebytes(old_ptr,(short)old_size);	
		goto CloseIt;
	}
#endif

	if (!(weights = (double*)getbytes((short)(sizeof(double)*WEIGHTS)))) {
		post("MaxNet: out of memory! Cannot get %ld bytes",(long)sizeof(double)*WEIGHTS);
		freebytes(old_ptr,(short)old_size);	
		goto CloseIt;
	}
	
	index = 0;
	
	if (hid_layers>0){
		for (i=0;i<x->nn_net->numIN;i++) {				/* input layer to 1st hidden */
			for (j=0;j<hid;j++,index++) {
				if (!sscanf(file_ptr,"%lf",(weights+index))) goto bail;
				skip_whitespace(file_ptr);
			}
			for (j=hid;j<x->nn_net->numHID;j++,index++) {	/* extra units for growing */
				*(weights+index) = random();
			}				
		}
	
		if(x->nn_debug){
			post("finished reading in input weights");
		}
			
		for (k=0;k<hid_layers-1;k++) {		/* loop over all hidden layers */
			for (i=0;i<hid;i++) {				/* inner hidden layer to next hidden layer */
				for (j=0;j<hid;j++,index++) {
					if (!sscanf(file_ptr,"%lf",(weights+index))) goto bail;
					skip_whitespace(file_ptr);
				}
				for (j=hid;j<x->nn_net->numHID;j++,index++)	/* extra units for growing */
					*(weights+index) = random();
			}
			for (i=hid;i<x->nn_net->numHID;i++) {
				for (j=0;j<hid;j++,index++)
					*(weights+index) = random();
				for (j=hid;j<x->nn_net->numHID;j++,index++)	/* extra units for growing */
					*(weights+index) = random();
			}		
		}
		
		if(x->nn_debug){
			post("finished reading in hidden layer weights");
		}
		
		for (i=0;i<hid;i++) {				/* last hidden layer to output layer */
			for (j=0;j<out;j++,index++) {
				if (!sscanf(file_ptr,"%lf",(weights+index))) goto bail;
				skip_whitespace(file_ptr);
			}
		}
		for (i=hid;i<x->nn_net->numHID;i++) {		/* extra units for growing */
			for (j=0;j<out;j++,index++)
				*(weights+index) = random();
		}				
	
		for (k=0;k<hid_layers;k++) {
			for (i=0;i<hid;i++,index++) {				/* bias weights */
				if (!sscanf(file_ptr,"%lf",(weights+index))) goto bail;
				skip_whitespace(file_ptr);	
			}
			for (i=hid;i<x->nn_net->numHID;i++,index++) {		/* extra units for growing */
				*(weights+index) = random();
			}
		}
		
	}else{
		for (i=0;i<x->nn_net->numIN;i++) {				/* input layer to output */
			for (j=0;j<out;j++,index++) {
				if (!sscanf(file_ptr,"%lf",(weights+index))) goto bail;
				skip_whitespace(file_ptr);
			}
		}
	
	}
	
	if(x->nn_debug){
		post("finished reading in output weights");
	}
		
	for (i=0;i<out;i++,index++) {				/* bias weights */
		if (!sscanf(file_ptr,"%lf",(weights+index))) goto bail;
		skip_whitespace(file_ptr);	
	}
	for (i=out;i<x->nn_net->numOUT;i++,index++) {		/* extra units for growing */
		*(weights+index) = random();
	}
	
	if(x->nn_debug){
		post("finished reading in bias weights");
	}
		
	setWeights(x->nn_net,weights,false);

	if(x->nn_debug){
		post("finished setting in weights");
	}
		
	for (i=0;i<x->nn_net->numOUT;i++) {
		if (!sscanf(file_ptr,"%lf",&slope)) goto bail;
		skip_whitespace(file_ptr);	
		setSlopeFactorOut(x->nn_net,i,slope);
	}		
	
	if(x->nn_debug){
		post("finished reading in output slopes");
	}
		
bail:
	freebytes(weights,(short)(sizeof(double)*WEIGHTS));
	freebytes(old_ptr,(short)old_size);	
CloseIt:
	;
}

/* ------------------------- nn_dowrite --------------------- */

void	*nn_dowrite(NeuralNetPtr x,Symbol *fileName)
{
    OSErr	err;
	double	*weights;
	short	i,file;
	long	count;
	char	buf[BUFSIZ];
	short	vol,binary;
	char	filename[256];
	
	strcpy(filename,fileName->s_name);
	if (fileName!=gensym(""))
		vol = defvolume();
	else{
		if (saveas_dialog(filename,&vol,&binary))
			goto CloseIt;
	}
	
	Create(CtoPstr(filename),vol,'max2','TEXT');
	if (err = FSOpen((ConstStr255Param)filename,vol,&file))
		goto CloseIt;

	sprintf(buf,"%d\r",x->nn_net->numIN);
	count = strlen(buf);
	FSWrite(file,&count,buf);
	sprintf(buf,"%d\r",x->nn_net->numHID);
	count = strlen(buf);
	FSWrite(file,&count,buf);
	sprintf(buf,"%d\r",x->nn_net->numHIDLayers);
	count = strlen(buf);
	FSWrite(file,&count,buf);
	sprintf(buf,"%d\r",x->nn_net->numOUT);
	count = strlen(buf);
	FSWrite(file,&count,buf);
	
	weights = (double*)getbytes((short)(sizeof(double)*WEIGHTS));
	getWeights(x->nn_net,weights);
	
	for (i=0;i<WEIGHTS;i++) {
		sprintf(buf,"%lf\r",*(weights+i));
		count = strlen(buf);
		FSWrite(file,&count,buf);
	}		
	
	for (i=0;i<x->nn_net->numOUT;i++) {
		sprintf(buf,"%lf\r",getSlopeFactorOut(x->nn_net,i));
		count = strlen(buf);
		FSWrite(file,&count,buf);
	}		
	
	sprintf(buf,"Epochs: %d\rPSS: %lf",x->epochs,x->prevPSS);
	count = strlen(buf);
	FSWrite(file,&count,buf);

	freebytes(weights,(short)(sizeof(double)*WEIGHTS));
	FSClose(file);
	FlushVol(0,vol);
CloseIt:;
}

void	*nn_free_pattern_memory(x,set)
NeuralNetPtr	x;
patternSetPtr	set;
{
	if (set->input) freebytes(set->input,(short)(set->patterns*sizeof(double)*x->nn_net->numIN));
	if (set->target) freebytes(set->target,(short)(set->patterns*sizeof(double)*x->nn_net->numOUT));
	if (set->outz) freebytes(set->outz,(short)(set->patterns*sizeof(double)*x->nn_net->numOUT));
	if (set->errz) freebytes(set->errz,(short)(set->patterns*sizeof(double)));
#ifdef MAXObject
	if (x->nn_errorAtoms) freebytes(x->nn_errorAtoms,(short)(set->patterns*sizeof(struct atom)));
#endif
}

int	nn_get_pattern_memory(x,set)
NeuralNetPtr	x;
patternSetPtr	set;
{
	if (!(set->input = (double*) getbytes((short)(set->patterns*sizeof(double)*x->nn_net->numIN))))
		return(0);
	if (!(set->target = (double*) getbytes((short)(set->patterns*sizeof(double)*x->nn_net->numOUT))))
		return(0);
	if (!(set->outz = (double*) getbytes((short)(set->patterns*sizeof(double)*x->nn_net->numOUT))))
		return(0);
	if (!(set->errz = (double*) getbytes((short)(set->patterns*sizeof(double)))))
		return(0);
#ifdef MAXObject
	if (!(x->nn_errorAtoms = (Atom *) getbytes((short)(set->patterns*sizeof(struct atom)))))
		return(0);
#endif
	return(1);
}

/* ------------------------- nn_doget_pattern --------------------- */

void	*nn_doget_pattern(NeuralNetPtr x,patternSetPtr set,Symbol *fileName)
{
    OSErr	err;
    short	file;
 	int		i,j,in,hid,hid_layers,out,patterns;		// MW changed from "short" 020604
	long	old_size,size;
	char	*file_ptr,*old_ptr;
	int		savelock,k;
	char	filename[256];
	short	vol;
	SFTypeList	typelist;
	OSType	dstType;
	
	savelock = lockout_set(1);

	strcpy(filename,fileName->s_name);
	if (fileName==gensym("")||locatefiletype(filename,&vol,0L,0L)){
#ifdef OPEN_DIALOG_WORKS
		/* do open dialog if no file given or not found */
		if (open_dialog(filename,&vol,&dstType,typelist,0))
#endif
			goto CloseIt;		/* didn't want to open */
	}
	
	if (err = FSOpen(CtoPstr(filename),vol,&file))
		goto CloseIt;
	strcpy(set->fileName,PtoCstr((unsigned char*)filename));
	GetEOF(file,&size);
	old_ptr = file_ptr = (char*) getbytes((short)size);
	if (!old_ptr) {
		FSClose(file);
		goto CloseIt;
	}
	FSRead(file,&size,file_ptr);
	FSClose(file);

	old_size = size;


	if (!sscanf(file_ptr,"%d",&in)) goto bail;
	skip_whitespace(file_ptr);
	
	if (!sscanf(file_ptr,"%d",&hid)) goto bail;
	skip_whitespace(file_ptr);
	
	if (!sscanf(file_ptr,"%d",&hid_layers)) goto bail;
	skip_whitespace(file_ptr);
		
	if (!sscanf(file_ptr,"%d",&out)) goto bail;
	skip_whitespace(file_ptr);
	
	if (!sscanf(file_ptr,"%d",&patterns)) goto bail;
	skip_whitespace(file_ptr);

				
#ifdef debug
post("IN: %d HID: %d",in,hid);
post("OUT: %d Patterns: %d",out,patterns);
post("set->patterns %d",set->patterns);
#endif  	
	if ((in != x->nn_net->numIN) || (out != x->nn_net->numOUT)) {
		post("MAXNet: This training data is for a network with %d input, %d hidden,",in,hid);
		post("		 and %d output units.  Please reconfigure network with Get Info.",out);
		freebytes(old_ptr,(short)old_size);	
		goto CloseIt;
	}
	if (set->patterns > 0)
		nn_free_pattern_memory(x,set);
	set->patterns = patterns;
	if (!nn_get_pattern_memory(x,set)) {
		post("MAXNet - Not enough pattern memory");
		freebytes(old_ptr,(short)old_size);
		goto CloseIt;
	}
	for (i=0;i<patterns;i++) {
		for (j=0;j<in;j++) {			
			if (!sscanf(file_ptr,"%lf",(set->input+(i*in)+j))) goto bail;
#ifdef debug
			post("pattern %d input unit %d value %lf",i,j,*(set->input+(i*in)+j));
#endif
			skip_whitespace(file_ptr);
		}
		for (j=0;j<out;j++) {			
			if (!sscanf(file_ptr,"%lf",(set->target+(i*out)+j))) goto bail;
#ifdef debug
			post("pattern %d output unit %d value %lf",i,j,*(set->target+(i*out)+j));
#endif
			skip_whitespace(file_ptr);
		}
	}

CloseIt:;
	lockout_set(savelock);
	return;
bail:
	post("MAXNet - pattern file format not correct");
	nn_free_pattern_memory(x,set);
	set->patterns = 0;
	lockout_set(savelock);
}

