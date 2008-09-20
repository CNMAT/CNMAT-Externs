#include "ext.h"
#include "SDIF_buffer_native.h"
#include "CNMAT_MMJ_SDIF.h"

int SDIFJava_test_init();
void error_report(SDIFresult r, const char *st);

JNIEXPORT jint JNICALL Java_cnmat_sdif_SDIF_1buffer_n_1init(JNIEnv *env, jobject obj){
	return SDIFJava_test_init();
}

JNIEXPORT jobject JNICALL Java_cnmat_sdif_SDIF_1buffer_n_1getMatrixHeader(JNIEnv *env, jobject obj, jstring s){
	char *s_char = (*env)->GetStringUTFChars(env, s, NULL);
	/*
	SDIFBufferLookupFunction f;
	SDIFBuffer *buffer;
	Symbol *buffer_sym = gensym(s_char);
	SDIFbuf_Buffer buf;

        f = (SDIFBufferLookupFunction) ps_SDIF_buffer_lookup->s_thing;
        if (f == 0) {
                // No SDIF buffer has ever been created yet. 
                //x->t_buffer = 0;
		buffer = 0;

        } else {
                //x->t_buffer = (*f)(x->t_bufferSym);
		buffer = (*f)(buffer_sym);
        }

	//  get access to the SDIFbuf_Buffer instance                                                                                          
	if (buffer){
		//x->t_buf = (x->t_buffer->BufferAccessor)(x->t_buffer);
		buf = (buffer->BufferAccessor)(buffer);
		post("got buffer");
		post("buffer = %p");
	}else{
		//x->t_buf = NULL;
		buf = NULL;
	}

	SDIFmem_Matrix m =  GetMatrixWithoutInterpolation(buffer, "1TRC", 1.0, 1, buffer_sym);
	*/

	CNMAT_MMJ_SDIF_buffer b;
	b.t_bufferSym = gensym(s_char);
	LookupMyBuffer(&b);

        if (b.t_buffer == 0) {
                error("¥ SDIF-tuples: no buffer!");
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
	//jobject o = (*env)->NewObject(env, c, cid);
	post("%s", m->header.matrixType);
	jstring tmpString = (*env)->NewStringUTF(env, m->header.matrixType);
	jobject o = (*env)->NewObject(env, c, cid, tmpString, m->header.matrixDataType, m->header.rowCount, m->header.columnCount);
	post("o = %p", o);
	/*
	jclass tmpclass = (*env)->GetObjectClass(env, o);

	jmethodID mid = (*env)->GetMethodID(env, tmpclass, "setRowCount", "(I)V");
	if(mid = 0){
		error("couldn't find method setMatrixType");
		return NULL;
	}
	jint foo = m->header.rowCount;
	(*env)->CallVoidMethod(env, o, mid, 15);
	*/

	return o;
	//return NULL;
}

int SDIFJava_test_init(){
	CNMAT_MMJ_SDIF_init(error_report);
	return 1;
}

void error_report(SDIFresult r, const char *st){
	if(r) error("SDIF native lib: %s: %s", st, SDIF_GetErrorString(r));
	else error("SDIF native lib: %s", st);
}
