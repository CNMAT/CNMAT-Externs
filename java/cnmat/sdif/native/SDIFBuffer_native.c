#include "ext.h"
#include "SDIFBuffer_native.h"
#include "CNMAT_MMJ_SDIF.h"

int SDIFJava_test_init();
void error_report(SDIFresult r, const char *st);

JNIEXPORT jint JNICALL Java_cnmat_sdif_SDIF_1buffer_n_1init(JNIEnv *env, jobject obj){
	return SDIFJava_test_init();
}

JNIEXPORT jobject JNICALL Java_cnmat_sdif_SDIF_1buffer_n_1getMatrixHeader(JNIEnv *env, jobject obj, jstring s){
	char *s_char = (*env)->GetStringUTFChars(env, s, NULL);

	CNMAT_MMJ_SDIF_buffer b;
	b.t_bufferSym = gensym(s_char);
	LookupMyBuffer(&b);

        if (b.t_buffer == 0) {
                error(": no buffer!");
                return;
	}

	SDIFmem_Matrix m;
	if(!(m = GetMatrix(&b, "1TRC", 1.0, 1)))
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
	return 1;
}

void error_report(SDIFresult r, const char *st){
	if(r) error("SDIF native lib: %s: %s", st, SDIF_GetErrorString(r));
	else error("SDIF native lib: %s", st);
}
