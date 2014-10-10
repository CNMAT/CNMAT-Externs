#ifndef __buffers__
#define __buffers__

#include "fftw3.h"




template <class T> class VectorTemplate;
template <class T> class MatrixTemplate;

typedef VectorTemplate<float> Vector;
typedef MatrixTemplate<float> VectorArray;
typedef VectorTemplate<fftwf_complex> ComplexVector;
typedef MatrixTemplate<fftwf_complex> ComplexVectorArray;


template <class T>
class VectorTemplate
{
    public:
        VectorTemplate(int inSize = 0) : size(0), data(NULL) { create(inSize); }
        VectorTemplate(const VectorTemplate<T> &vec); //copy constructor
        ~VectorTemplate() { destroy(); }
        VectorTemplate<T> &operator=(const VectorTemplate<T> &vec); //assignment operator
        T& operator[](int ind) { return data[ind]; }
        T* operator()(int ind = 0) { return &data[ind]; }

        int create(int inSize);
        void destroy();
        void reset()
        {
            if (size > 0) 
                memset(data,0,sizeof(T)*size);
        }

        int getSize() const { return size; }

        int zeroPad(int length, int offset = 0);
        void copyFrom(T* src)
        {
            if (size > 0)
                memcpy(data, src, sizeof(T)*size); 
        }

        
    private:
        int size;
        T* data;
};

template <class T>
class MatrixTemplate
{
    public: 
        MatrixTemplate(int inDim1 = 0, int inDim2 = 0) : 
            dim1(0), dim2(0), data(NULL) { create(inDim1,inDim2); }
        MatrixTemplate(const MatrixTemplate<T> &mat);  //copy constructor
        ~MatrixTemplate() { destroy(); } 
        MatrixTemplate<T> &operator=(const MatrixTemplate<T> &mat); //assignment operator
        //T* operator[](int ind) {return dataRows[ind];}
        T* operator[](int ind) {return data[ind];}

        int create(int inDim1, int inDim2);
        void destroy();
        void reset() 
        { 
            for (int i = 0; i < dim1; i++)
                memset(data[i],0,sizeof(T)*dim2);
        }

        //int getSize() const { return size; }
        int getDim1() const { return dim1; }
        int getDim2() const { return dim2; }

        int splitVector(VectorTemplate<T> vec, int length, int paddedLength = 0, int offset = 0);


    private:
        //int size;
        int dim1;
        int dim2;
        T** data;
        //T** dataRows;
};

typedef struct vec{
    float* data;
    int size;
} vec;

typedef struct Cvec{
    fftwf_complex* Cdata;
    int size;
} Cvec;

typedef struct matrix{
    float** data;
    int rows;
    int cols;
} matrix;

typedef struct Cmatrix{
    fftwf_complex** Cdata;
    int rows;
    int cols;
} Cmatrix;


double get_time();
int start_timer(double* timers, int ind);
int stop_timer(double* timers, int ind);


Cvec zero_pad_complex(Cvec vector, int padded_length, int offset);
Cmatrix splitArrayComplex(Cvec x, int rowlength,int paddedRowLength,int offset);

matrix splitArray(vec x, int rowLength, int paddedRowLength, int offset); 
vec zero_pad(vec vector, int padded_length, int offset);
int nextpow2(int x);
int log2i(int x);

Cmatrix create_Cmatrix(int rows, int cols);
matrix create_matrix(int rows, int cols);
Cvec create_Cvec(int size);
vec create_vec(int size);

void reset_Cmatrix(Cmatrix A);
void reset_matrix(matrix A);
void reset_Cvec(Cvec A);
void reset_vec(vec A);

void free_vec(vec* x);
void free_Cvec(Cvec* x);
void free_matrix(matrix* A);
void free_Cmatrix(Cmatrix* A);

float error_norm(vec x, vec y, float scale);

void write_matrix(matrix A, const char* file);
void write_vec(vec A, const char* file);

void naive_convolution(float *y, float *x, float *h, int x_len, int h_len);


#endif //__sigproc_util__
