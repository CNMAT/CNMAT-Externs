#include "ext.h"
#include "SDIF_buffer_native.h"
//#include "sdif.h"
#include "SDIF-buffer.h"
#include "sdif-util.h"

static Symbol *ps_SDIF_buffer_lookup;
static SDIFmem_Matrix GetMatrixWithoutInterpolation(SDIFBuffer *buffer,
                                                    const char *desiredType,
                                                    sdif_float64 time,
                                                    int direction,
						    Symbol *bufferSym);
static void *my_getbytes(int numBytes);
static void my_freebytes(void *bytes, int size);
int SDIFJava_test_init();

JNIEXPORT jint JNICALL Java_SDIFJava_1test_n_1init(JNIEnv *env, jobject obj){
	return SDIFJava_test_init();
}

JNIEXPORT jobject JNICALL Java_SDIFJava_1test_n_1getMatrixHeader(JNIEnv *env, jobject obj, jstring s){
	char *s_char = (*env)->GetStringUTFChars(env, s, NULL);
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

static SDIFmem_Matrix GetMatrixWithoutInterpolation(SDIFBuffer *buffer,
                                                    const char *desiredType,
                                                    sdif_float64 time,
                                                    int direction,
						    Symbol *bufferSym)
{
	SDIFmem_Frame f;
	SDIFmem_Matrix m, matrixOut;
	SDIFresult r;

	// could pass to function: buffer
        //  get the frame
        if(!(f = (*(buffer->FrameLookup))(buffer, time, direction))) {
		/*
                if ((*(buffer->FrameLookup))(buffer, (sdif_float64) VERY_SMALL, 1) == 0) {
                        if (!x->t_complainedAboutEmptyBufferAlready) {
                                post("¥ SDIF-tuples: SDIF-buffer %s is empty", x->t_bufferSym->s_name);
                                x->t_complainedAboutEmptyBufferAlready = TRUE;
                        }
                } else {
                        if (x->t_errorreporting) {
                                post("¥ SDIF-tuples: SDIF-buffer %s has no frame at time %f",
				     x->t_bufferSym->s_name, (float) time);
                        }
                }
		*/
		post("error using FrameLookup");
                return NULL;
        }

        //  find the matrix
        for (m = f->matrices; m!= 0; m = m->next) {
                if (SDIF_Char4Eq(m->header.matrixType, desiredType)) {
                        break;
                }
        }

        //  couldn't find the matrix
        if (m == 0) {
                post("¥ SDIF-tuples: no matrix of type %c%c%c%c in frame at time %f of SDIF-buffer %s",
		     desiredType[0], desiredType[1], desiredType[2], desiredType[3],
		     (float) f->header.time, bufferSym->s_name);
                return NULL;
        }

	//  we don't support matrix data types other than float and int32
	if (((m->header.matrixDataType >> 8) != SDIF_FLOAT) && (m->header.matrixDataType != SDIF_INT32))  {
                post("¥ SDIF-tuples: Unsupported matrix data type (0x%x) in %c%c%c%c matrix ",
		     m->header.matrixDataType,
		     desiredType[0], desiredType[1], desiredType[2], desiredType[3]);
                post("  in frame at time %f of SDIF-buffer %s (currently only float and int32 are supported)",
		     (float) f->header.time, bufferSym->s_name);
                return NULL;
        }

        //post("*** Cloning matrix at time %f, type 0x%x", time, m->header.matrixDataType);

        //  copy result to output matrix
	r = SDIFutil_CloneMatrix(m, &matrixOut);

        if (r == ESDIF_OUT_OF_MEMORY) {
                error("¥ SDIF-tuples: out of memory to clone matrix");
		return NULL;
        } else if (r!=ESDIF_SUCCESS) {
                error("¥ SDIF-tuples: couldn't clone matrix: %s", SDIF_GetErrorString(r));
                return NULL;
        }

	//  return result                                                                                                                      
	//  NOTE: caller is responsible for calling SDIFmem_FreeMatrix()                                                                       
	return matrixOut;
}

static void *my_getbytes(int numBytes) {
        if (numBytes > SHRT_MAX) {
		return 0;
        }
        return (void *) getbytes((short) numBytes);
}

static void my_freebytes(void *bytes, int size) {
        freebytes(bytes, (short) size);
}

int SDIFJava_test_init(){
	SDIFresult r;
	char *NAME = "SDIFJava_test";
	ps_SDIF_buffer_lookup = gensym("##SDIF-buffer-lookup");
	if (r = SDIF_Init()) {
                ouchstring("%s: Couldn't initialize SDIF library! %s",
                           NAME,
                           SDIF_GetErrorString(r));
                return 0;
        }

        if (r = SDIFmem_Init(my_getbytes, my_freebytes)) {
                post("¥ %s: Couldn't initialize SDIF memory utilities! %s",
                     NAME,
                     SDIF_GetErrorString(r));
                return 0;
        }

        if (r = SDIFbuf_Init()) {
                post("¥ %s: Couldn't initialize SDIF buffer utilities! %s",
                     NAME,
                     SDIF_GetErrorString(r));
                return 0;
        }
	post("SDIFJava_test: initialized sucessfully");
	return 1;
}
