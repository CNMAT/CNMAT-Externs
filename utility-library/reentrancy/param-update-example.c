typedef struct myparamstruct {
	int n;
	float *values;
} myparams;

typedef struct _myobjecct {
	t_pxobject b_obj;
	...
	paramPointers pp;
} myobject;


new() {
	myobject *x;
	myparams *one, *two, *three;
	
	x=newobject(...);
	
	one = getbytes(...);
	two = getbytes(...);
	three = getbytes(...);
	
	one->values = initial_values;
	
	InitParamPointers(x->pp, one, two, three);
}


Update(myobject *x, ...) {
	myparams *nextParams;
	
	nextParams = (myparams *) CheckOutParamBlock(x->pp);
	
	/* Lots of computation */
	nextParams->values = new_values;
	
	CheckInNewParams(x->pp);
}

Perform(myobject *x, ...) {
	myparams *fromParams, *nextParams;
	
	fromParams = (myparams *) GetPreviousParams(x->pp);
	toParams = (myparams *) GetCurrentParams(x->pp);
	
	/* Process using those "snapshot"s */
	if (fromParams == toParams) {
		/* Parameters haven't changed since the last perform(); no need to interpolate */
	
		...
	} else {
		/* Interpolate from fromParams to toParams */
	}
	
	DoneWithPreviousParams(x->pp);
}
