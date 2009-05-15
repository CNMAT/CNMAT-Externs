#include "ext.h"
#include "SDIFBuffer_native.h"
#include "CNMAT_MMJ_SDIF.h"

void error_report(SDIFresult r, const char *st);
jclass sdif_matrix_header_class;
jclass sdif_matrix_class;
jmethodID sdif_matrix_header_ctor;
jmethodID sdif_matrix_ctor;

JNIEXPORT jint JNICALL Java_cnmat_sdif_SDIFBuffer_n_1init(JNIEnv *env, jobject obj){
	sdif_matrix_class = (*env)->FindClass(env, "cnmat/sdif/SDIFMatrix");
	sdif_matrix_header_class = (*env)->FindClass(env, "cnmat/sdif/SDIFMatrixHeader");
	sdif_matrix_header_ctor = (*env)->GetMethodID(env, sdif_matrix_header_class, "<init>", "([CIII)V");
	sdif_matrix_ctor = (*env)->GetMethodID(env, sdif_matrix_class, "<init>", "(Lcnmat/sdif/SDIFMatrixHeader;)V");
	CNMAT_MMJ_SDIF_init(error_report);
	return 1;
}

JNIEXPORT jobject JNICALL Java_cnmat_sdif_SDIFBuffer_n_1getMatrix
  (JNIEnv *env, jobject obj, jstring bufferName, jcharArray type, jdouble time, jint direction){
	char *bufferName_char = (*env)->GetStringUTFChars(env, bufferName, NULL);
	jchar *type_char = (*env)->GetCharArrayElements(env, type, 0);
	int col, row, counter;

	CNMAT_MMJ_SDIF_buffer b;
	b.t_bufferSym = gensym(bufferName_char);
	LookupMyBuffer(&b);

        if (b.t_buffer == 0) {
                error(": no buffer!");
                return NULL;
	}

	SDIFmem_Matrix m;
	if(!(m = GetMatrix(&b, "1TRC", 1.0, 1)))
                return NULL;

	jstring matrixType_jstring = (*env)->NewStringUTF(env, m->header.matrixType);
	jobject matrix_header_obj = (*env)->NewObject(env, 
						     sdif_matrix_header_class, 
						     sdif_matrix_header_ctor, 
						     matrixType_jstring, 
						     m->header.matrixDataType, 
						     m->header.rowCount, 
						     m->header.columnCount);

	jobject sdif_matrix_obj = (*env)->NewObject(env, 
						    sdif_matrix_obj, 
						    sdif_matrix_ctor, 
						    matrix_header_obj);

	jmethodID setData;
	if(m->header.matrixDataType == SDIF_INT32){
		setData = (*env)->GetMethodID(env, sdif_matrix_class, "setData", "([I)V");
		(*env)->CallVoidMethod(env, obj, setData, (int *)m->data);
	}else if(m->header.matrixDataType == SDIF_FLOAT32){
		setData = (*env)->GetMethodID(env, sdif_matrix_class, "setData", "([F)V");
		(*env)->CallVoidMethod(env, obj, setData, (float *)m->data);
	}else{
		//post error
	}

	//cid = (*env)->GetMethodID(env, sdif_matrix_class, "<init>", "(Lcnmat/sdif/SDIFMatrixHeader;[I)V");
	//sdif_matrix_obj = (*env)->NewObject(env, sdif_matrix_obj, cid, matrixHeader_obj, d);


	return sdif_matrix_obj;
}

JNIEXPORT jobject JNICALL Java_cnmat_sdif_SDIFBuffer_n_1getMatrixHeader(JNIEnv *env, 
									jobject obj, 
									jstring s)
{
	char *s_char = (*env)->GetStringUTFChars(env, s, NULL);

	CNMAT_MMJ_SDIF_buffer b;
	b.t_bufferSym = gensym(s_char);
	LookupMyBuffer(&b);

        if (b.t_buffer == 0) {
                error_report(0, "no buffer!");
                return NULL;
	}

	SDIFmem_Matrix m;
	char dt[4] = {'1', 'T', 'R', 'C'};
	if(!(m = GetMatrix(&b, dt, 1.0, 1))){
                return NULL;
	}
	post("made it here");
	return;

	jstring matrixType_jstring = (*env)->NewStringUTF(env, m->header.matrixType);
	jobject o = (*env)->NewObject(env, 
				      sdif_matrix_header_class, 
				      sdif_matrix_header_ctor, 
				      matrixType_jstring, 
				      m->header.matrixDataType, 
				      m->header.rowCount, 
				      m->header.columnCount);
	return o;
}

void error_report(SDIFresult r, const char *st){
	if(r) error("SDIF_native_lib: %s: %s", st, SDIF_GetErrorString(r));
	else error("SDIF_native_lib: %s", st);
}
