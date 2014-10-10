#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/time.h>


#include "fftw3.h"

#include "buffers.h"


double get_time();
void* safe_fftwf_malloc(size_t size);

double get_time()
{
    //output time in seconds
    struct timeval t;
    gettimeofday(&t,NULL);
    return (double)(t.tv_sec+t.tv_usec/1E6);
}

void* safe_fftwf_malloc(size_t size)
{
    void* ptr = fftwf_malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr,"safe_fftwf_malloc: NULL returned for chunk of size %u\n", (unsigned)size);
        exit(-1);
    }
    else 
        return ptr;
}


/* VectorTemplate ------------------------- */

template <class T>
VectorTemplate<T>::VectorTemplate(const VectorTemplate<T> &vec)
{
    size = 0;
    data = NULL;
    create(vec.size);
    memcpy(data, vec.data, sizeof(T)*vec.size);
}

template <class T>
VectorTemplate<T> &VectorTemplate<T>::operator=(const VectorTemplate<T> &vec)
{
    if (this != &vec)  //check for self-assignment
    {
        create(vec.size);
        memcpy(data, vec.data, sizeof(T)*vec.size);
    }
    return *this;
}

template <class T>
int VectorTemplate<T>::create(int inSize)
{
    if (inSize != size)
    {
        destroy();
        if (inSize > 0)
        {
            size = inSize;
            data = (T*)safe_fftwf_malloc(size*sizeof(T));
            reset();
            return 0;
        }
        else // invalid size
            return 1;
    }
    else
    {
        // no size change. just reset
        reset();  
        return 0;
    }
}


template <class T>
void VectorTemplate<T>::destroy()
{
    if (size > 0)
    {
        fftwf_free(data);
        size = 0;
        data = NULL;
    }
}

template <class T>
int VectorTemplate<T>::zeroPad(int length, int offset)
{
    if (length < size + offset)
    {
        fprintf(stderr, "VectorTemplate<T>::zeroPad() padded size too small!\n");
        return 1;
    }
    if (size > 0)
    {
        T* newData = (T*)safe_fftwf_malloc(sizeof(T)*length);
        memset(newData, 0, sizeof(T)*length);
        memcpy(newData + offset, data, sizeof(T)*size);
        fftwf_free(data);
        data = newData;
        size = length;
        return 0;
    }
    else
        return 1; //nothing to pad
}



/* MatrixTemplate<T> ------------------------ */

template <class T> //copy constructor
MatrixTemplate<T>::MatrixTemplate(const MatrixTemplate<T> &mat)
{
    //size = 0;
    dim1 = 0;
    dim2 = 0;
    data = NULL;
    //dataRows = NULL;
    create(mat.dim1,mat.dim2);
    for (int i = 0; i < mat.dim1; i++)
        memcpy(data[i], mat.data[i], sizeof(T)*mat.dim2);
}

template <class T> //assignment
MatrixTemplate<T> &MatrixTemplate<T>::operator=(const MatrixTemplate<T> &mat)
{
    if (this != &mat)  //check for self-assignment
    {
        create(mat.dim1,mat.dim2);
        for (int i = 0; i < mat.dim1; i++)
            memcpy(data[i], mat.data[i], sizeof(T)*mat.dim2);
    }
    return *this;
}

template <class T>
int MatrixTemplate<T>::create(int inDim1, int inDim2)
{
    const int newSize = inDim1*inDim2;
    if (inDim1 != dim1 || inDim2 != dim2)
    {
        destroy();

        if (newSize > 0)
        {
            dim1 = inDim1;
            dim2 = inDim2;

            data = new T*[dim1];
            for (int i = 0; i < dim1; i++)
                data[i] = (T*)safe_fftwf_malloc(dim2*sizeof(T));
            reset();
            return 0;
        }
        else //invalid size
            return 1;
    }
    else
    {
        // no size change, just reset
        reset();
        return 0;
    }
}


template <class T>
void MatrixTemplate<T>::destroy()
{
    if (dim1 > 0)
    {
        for (int i = 0; i < dim1; i++)
            fftwf_free(data[i]);
        delete[] data;
            
        data = NULL;
        dim1 = 0;
        dim2 = 0;
    }
}

template <class T>
int MatrixTemplate<T>::splitVector(VectorTemplate<T> vec, int length, int paddedLength, int offset)
{
    if (paddedLength == 0)
        paddedLength = length;

    if (paddedLength < length + offset)
    {
        fprintf(stderr, "MatrixTemplate<T>::split() paddedLength < length + offset\n");
        return 1;
    }

    int fullRows = vec.getSize()/length;
    int leftover = vec.getSize() % length;

    create(fullRows + (leftover > 0), paddedLength);

    for (int i = 0; i < fullRows; i++) 
        memcpy(&data[i][offset], &vec[i*length],sizeof(T)*length);

    if (leftover > 0)
        memcpy(&data[fullRows][offset], &vec[fullRows*length], sizeof(T)*leftover);

    return 0;
}


// instantiate templates
template class VectorTemplate<float>;
template class MatrixTemplate<float>;
template class VectorTemplate<fftwf_complex>;
template class MatrixTemplate<fftwf_complex>;



int nextpow2(int y) 
{
    unsigned x;
    if (y >= 0)
        x = (unsigned)y;
    else 
        return -1;
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return (int)(x + 1);
}

int log2i(int x)
{
    if (x <= 0)
        return -1;

    int isPow2 = x && !( (x-1) & x);
    if (!isPow2)
    {
        fprintf(stderr, "log2i: %d is not a positive power of 2\n",x);
        return -1;
    }

    int pow = 0;
    unsigned y = (unsigned) x;
    while (y > 1)
    {
        y >>= 1;
        pow++;
    }
    return pow;
}
            

