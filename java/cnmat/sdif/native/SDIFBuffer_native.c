#include "ext.h"
#include "SDIFBuffer_native.h"
#include "CNMAT_MMJ_SDIF.h"

int SDIFJava_test_init();
void error_report(SDIFresult r, const char *st);

JNIEXPORT jint JNICALL Java_cnmat_sdif_SDIFBuffer_n_1init(JNIEnv *env, jobject obj){
	return SDIFJava_test_init();
}

JNIEXPORT jobject JNICALL Java_cnmat_sdif_SDIFBuffer_n_1getMatrix
  (JNIEnv *env, jobject obj, jstring bufferName, jcharArray type, jdouble time, jint direction){
	post("hi from the native lib");
	char *bufferName_char = (*env)->GetStringUTFChars(env, bufferName, NULL);
	jchar *type_char = (*env)->GetCharArrayElements(env, type, 0);

	CNMAT_MMJ_SDIF_buffer b;
	b.t_bufferSym = gensym(bufferName_char);
	LookupMyBuffer(&b);

        if (b.t_buffer == 0) {
                error(": no buffer!");
                return;
	}

	SDIFmem_Matrix m;
	if(!(m = GetMatrix(&b, "1TRC", 1.0, 1)))
                return;

	jclass matrixHeader_class = (*env)->FindClass(env, "cnmat/sdif/SDIFMatrixHeader");
	jmethodID cid = (*env)->GetMethodID(env, matrixHeader_class, "<init>", "(Ljava/lang/String;III)V");
	jstring tmpString = (*env)->NewStringUTF(env, m->header.matrixType);
	jobject matrixHeader_obj = (*env)->NewObject(env, matrixHeader_class, cid, tmpString, m->header.matrixDataType, m->header.rowCount, m->header.columnCount);

	jclass sdif_matrix_class = (*env)->FindClass(env, "cnmat/sdif/SDIFMatrix");
	jobject sdif_matrix_obj = NULL;
	if(m->header.matrixDataType == SDIF_INT32){
		int counter = 0;
		int d[m->header.rowCount * m->header.columnCount];
		int c, r;
		for(r = 0; r < m->header.rowCount; r++){
			for(c = 0; c < m->header.columnCount; c++){
				d[counter++] = SDIFutil_GetMatrixCell_int32(m, c, r);
			}
		}
		cid = (*env)->GetMethodID(env, sdif_matrix_class, "<init>", "(Lcnmat/sdif/SDIFMatrixHeader;[I)V");
		sdif_matrix_obj = (*env)->NewObject(env, sdif_matrix_obj, cid, matrixHeader_obj, d);
	}else{
		int counter = 0;
		float d[m->header.rowCount * m->header.columnCount];
		int c, r;
		for(r = 0; r < m->header.rowCount; r++){
			for(c = 0; c < m->header.columnCount; c++){
				d[counter++] = SDIFutil_GetMatrixCell(m, c, r);
			}
		}
		cid = (*env)->GetMethodID(env, sdif_matrix_class, "<init>", "(Lcnmat/sdif/SDIFMatrixHeader;[F)V");
		sdif_matrix_obj = (*env)->NewObject(env, sdif_matrix_obj, cid, matrixHeader_obj, d);
	}
	return sdif_matrix_obj;
}

JNIEXPORT jobject JNICALL Java_cnmat_sdif_SDIFBuffer_n_1getMatrixHeader(JNIEnv *env, jobject obj, jstring s){
	SDIFJava_test_init();
	char *s_char = (*env)->GetStringUTFChars(env, s, NULL);

	CNMAT_MMJ_SDIF_buffer b;
	post("b = %p\n", b.t_buffer);
	b.t_bufferSym = gensym(s_char);
	LookupMyBuffer(&b);

        if (b.t_buffer == 0) {
                error(": no buffer!");
                return;
	}

	post("b = %p\n", b.t_buffer);
	SDIFmem_Matrix m;

	GetMatrix(&b, "1TRC", 1.0, 1);
	/*
	if(!(m = GetMatrix(&b, "1TRC", 1.0, 1)))
                return NULL;
	*/

	return;
	post("matrixDataType = %d, rows = %d, cols = %d\n", m->header.matrixDataType, m->header.rowCount, m->header.columnCount);
	jclass c = (*env)->FindClass(env, "cnmat/sdif/MatrixHeader");
	post("class = %p", c);
	jmethodID cid = (*env)->GetMethodID(env, c, "<init>", "(Ljava/lang/String;III)V");
	post("cid = %p", cid);
	post("%s", m->header.matrixType);
	jstring tmpString = (*env)->NewStringUTF(env, m->header.matrixType);
	jobject o = (*env)->NewObject(env, c, cid, tmpString, m->header.matrixDataType, m->header.rowCount, m->header.columnCount);
	post("o = %p", o);

	return o;
}

int SDIFJava_test_init(){
	CNMAT_MMJ_SDIF_init(error_report);
	post("INIT WAS SUCCESSFUL");
	return 1;
}

void error_report(SDIFresult r, const char *st){
	if(r) error("SDIF native lib: %s: %s", st, SDIF_GetErrorString(r));
	else error("SDIF native lib: %s", st);
}
