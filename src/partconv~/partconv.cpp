
#include <errno.h>
#include <xmmintrin.h>
#include <pmmintrin.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "partconv.h"


#define CACHE_SIZE (3*1024*1024) //bytes



//#define _DEBUG_
#define MEASURE_COMPUTE_TIME
//#define _TIME_LEVELS_


#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))



void AtomicSet(unsigned *ptr, unsigned new_value)
{
        while (true)
        {
                unsigned old_value = *ptr;
                if (__sync_bool_compare_and_swap(ptr, old_value, new_value)) 
                    return;
        }
}

int checkState(int *state, int start_level, int sync_depth, int check_for)
{
    for (int L = start_level; L <= sync_depth; L++)
    {
        if (state[L] != check_for)
            return 0;
    }
    return 1;
}

#ifdef __APPLE__
int get_bus_speed()
{
    int mib[2];
    unsigned int miblen;
    int busspeed;
    int retval;
    size_t len;

    mib[0]=CTL_HW;
    mib[1]=HW_BUS_FREQ;
    miblen=2;
    len=4;
    retval = sysctl(mib, miblen, &busspeed, &len, NULL, 0);

    return busspeed;
}
#endif


/* PartConvFilter methods */

int PartConvFilter::setup(
        int M_in, int* N_in, int* num_parts_in, 
        Vector *inImpulseResponse, int fftwOptLevel, const char* wisdom)
{
    // for setup benching
    M = M_in;
    

    N = new int[M];
    num_parts = new int[M];
    nfft = new int[M];
    cfft = new int[M];

    memcpy(N,N_in,sizeof(int)*M);
    memcpy(num_parts,num_parts_in,sizeof(int)*M);

    // block size at each level
    for (int i = 0; i < M; i++)
    {
        nfft[i] = 2*N[i];
        cfft[i] = nfft[i]/2 + 1;

        if (N[i] != nextpow2(N[i]))
        {
            fprintf(stderr,"PartConvFilter ERROR: block size of level %u must be a power of 2\n",i);
            fprintf(stderr,"N[%u] = %u\n",i,N[i]);
            delete[] N;
            delete[] num_parts;
            delete[] nfft;
            delete[] cfft;
            return 1;
        }
    }
    for (int i = 1; i < M; i++)
    {
        if (N[i] <= N[i-1])
        {
            fprintf(stderr,"PartConvFilter ERROR: N[%u]=%u <= N[%u]=%u\n",i, N[i], i-1, N[i-1]);
            delete[] N;
            delete[] num_parts;
            delete[] nfft;
            delete[] cfft;
            return 1;
        }
    }


    bcurr = new unsigned[M];
    ind_N0_in_level = new int[M];
    num_N0_in_level = new int[M];

    for (int i = 0; i < M; i++)
    {
        num_N0_in_level[i] = N[i]/N[0];
    }
    for (int i = 0; i < M; i++)
    {
        bcurr[i] = 0;                               //incremented at end of ::runLevel
        ind_N0_in_level[i] = num_N0_in_level[i]-1;  //incremented at beg of ::run
    }

    Vector impulseResponse;
    if (inImpulseResponse == NULL)
    { //if we're benching, create impulse response of appropriate length
        int length = 0;
        for (int L = 0; L < M; L++)
        {
            length += N[L]*num_parts[L];
        }
        impulseResponse.create(length);
        for (int i = 0; i < impulseResponse.getSize(); i++)
            impulseResponse[i] = 1;
    }
    else
    {   
        impulseResponse = *inImpulseResponse;
    }

    int position = 2*N[0];
    int delay[M];
    for (int i = 0; i < M-1; i++)
    {
        delay[i] = position/N[0];
        position += N[i]*num_parts[i];
        if (position < 2*N[i+1])
        {
            fprintf(stderr,"PartConvFilter ERROR: sufficient delay not allowed for block size of level %u\n",i+2);
            delete[] bcurr;
            delete[] ind_N0_in_level;
            delete[] num_N0_in_level;
            delete[] N;
            delete[] num_parts;
            delete[] nfft;
            delete[] cfft;
            return 1;
        }
    }
    delay[M-1] = position/N[0];

    num_parts[M-1] = ceil((float)(impulseResponse.getSize() - (position - 2*N[0]))/N[M-1]);  
    if (impulseResponse.getSize() - (position - 2*N[0]) < 0)
    {
        fprintf(stderr,"PartConvFilter ERROR: last partitioning level (%u) not required\n",M);
        delete[] bcurr;
        delete[] ind_N0_in_level;
        delete[] num_N0_in_level;
        delete[] N;
        delete[] num_parts;
        delete[] nfft;
        delete[] cfft;
        return 1;
    }
    position += N[M-1]*num_parts[M-1];

    int impulse_response_size = impulseResponse.getSize();
    impulseResponse.zeroPad(position);

    // partition impulse response into levels
    Vector *h_level = new Vector[M];
    position = 0;
    h_level[0].create(N[0]*num_parts[0]);
    h_level[0].copyFrom(&impulseResponse[position]);
    for (int i = 1; i < M; i++)
    {
        position += N[i-1]*num_parts[i-1];
        h_level[i].create(N[i]*num_parts[i]);
        h_level[i].copyFrom(&impulseResponse[position]);
    }

    // partion levels into blocks
    VectorArray *h = new VectorArray[M];
    for (int i = 0; i < M; i++)
    {
        h[i].splitVector(h_level[i],N[i],nfft[i],N[i]);
    }

    H = new ComplexVectorArray[M];

    for (int i = 0; i < M; i++)
        H[i].create(num_parts[i],cfft[i]);


    // do the fft on the impluse partitions...
    for (int L = 0; L < M; L++)
    {
        //float scale = nfft[L]*h_norm*2;
        float scale = nfft[L];
        for (int i = 0; i < num_parts[L]; i++)
        {
            fftwf_plan h_fwd = 
                fftwf_plan_dft_r2c_1d(nfft[L], h[L][i] ,H[L][i] ,FFTW_ESTIMATE);
            fftwf_execute(h_fwd);
            fftwf_destroy_plan(h_fwd);
            // scaling by nfft*(norm of original h)
            for (int j = 0; j < cfft[L]; j++) 
            {
                H[L][i][j][0] /= scale;
                H[L][i][j][1] /= scale;
            }
        }
    }

    Xbuf = new DelayBuffer*[M];
    for (int i = 0; i < M; i++)
        Xbuf[i] = new DelayBuffer(N[0],N[i]/N[0],delay[i]);

    Ybuf = new DoubleBuffer*[M];
    for (int i = 0; i < M; i++)
        Ybuf[i] = new DoubleBuffer(N[i]);

    Xfft = new ComplexVectorArray[M];      //holds ffts of input
    inbuffer = new Vector[M];      //holds input block
    outbuffer = new Vector[M];     //holds output block
    fftbuffer = new ComplexVector[M];    //holds fft of input block
    Yfft = new ComplexVector[M];         //holds sum of an FDL
    for (int i = 0; i < M; i++)
    {
        Xfft[i].create(num_parts[i],cfft[i]);
        inbuffer[i].create(nfft[i]);
        fftbuffer[i].create(cfft[i]);
        outbuffer[i].create(nfft[i]);
        Yfft[i].create(cfft[i]);
    }


    if (wisdom != NULL)
    {
        if (FILE* wisdomfile = fopen(wisdom,"r"))
        {
            fftwf_import_wisdom_from_file(wisdomfile);
            fclose(wisdomfile);
        }
    }
    bck = new fftwf_plan[M];
    fwd = new fftwf_plan[M];

    if (fftwOptLevel == 1)
        fftwOptLevel = FFTW_MEASURE;
    else if (fftwOptLevel == 2)
        fftwOptLevel = FFTW_PATIENT;
    else
        fftwOptLevel = FFTW_ESTIMATE;

    for (int i = 0; i < M; i++)
    {
        bck[i] = fftwf_plan_dft_c2r_1d(nfft[i],Yfft[i](),outbuffer[i](),fftwOptLevel);
        fwd[i] = fftwf_plan_dft_r2c_1d(nfft[i],inbuffer[i](),fftbuffer[i](),fftwOptLevel);
    }
    if (wisdom != NULL)
    {
        if (FILE* wisdomfile = fopen(wisdom,"w"))
        {
            fftwf_export_wisdom_to_file(wisdomfile);
            fclose(wisdomfile);
        }
    }


    output_mix = new float[N[0]]();
    input_mix = new float[N[0]]();
    

    // print specs of filter
    if (verbosity > 0)
    {
        printf("\nPartitioning Specs:\n");
        printf("impulse response: %.2f sec\n",(double)impulse_response_size/FS);
        printf("partitioning: ");
        for (int i = 0; i < M-1; i++)
            printf("%ux%u, ", N[i],num_parts[i]);
        printf("%ux%u\n",N[M-1],num_parts[M-1]);
        printf("filter latency: %g ms\n",(float)1000*N[0]/FS);

    }
    delete [] h_level;
    delete [] h ;
    
    return 0;
}

int PartConvFilter::cleanup(void)
{
    // free stuff
    for (int i = 0; i < M; i++)
    {
        fftwf_destroy_plan(bck[i]);
        fftwf_destroy_plan(fwd[i]);

        delete Xbuf[i];
        delete Ybuf[i];
    }
    delete[] N;
    delete[] num_parts;
    delete[] nfft;
    delete[] cfft;

    delete[] Yfft;
    delete[] Xfft;
    delete[] inbuffer;
    delete[] outbuffer;
    delete[] fftbuffer;
    delete[] H;
    delete[] fwd;
    delete[] bck;

    delete[] Xbuf;
    delete[] Ybuf;

    //delete[] output_mix;
    //delete[] input_mix;


    
    
    delete[] bcurr;
    delete[] ind_N0_in_level;
    delete[] num_N0_in_level;



    if (verbosity > 1)
        printf("\ncleaned up PartConvFilter instance\n");

    return 0;
}

int PartConvFilter::runLevel(int L)
{
    int slot;

    memcpy(&inbuffer[L][0], &inbuffer[L][N[L]], sizeof(float)*N[L]);
    Xbuf[L]->read(&inbuffer[L][N[L]]);

    //fft slot
    slot = bcurr[L] % num_parts[L];
    fftwf_execute(fwd[L]); // take fft of inbuffer, save in fftbuffer
    memcpy(Xfft[L][slot], fftbuffer[L](), sizeof(fftwf_complex)*cfft[L]);

    // reset Yfft to zeros
    memset(Yfft[L](), 0, sizeof(fftwf_complex)*cfft[L]);

    // do filtering
    for (int p = 0; p < num_parts[L]; p++) 
    {
        slot = (bcurr[L]-p + num_parts[L]) % num_parts[L];
        const float *Aptr = (const float *)Xfft[L][slot];
        const float *Bptr = (const float *)H[L][p];
        float *Cptr = (float *)Yfft[L]();

        
        __m128 A, B, C, D;
#ifndef _DONT_UNROLL_CMULT_LOOP
        for (int i = 0; i < cfft[L]-1; i+=8)
        {
            A = _mm_load_ps(Aptr);
            B = _mm_load_ps(Bptr);
            C = _mm_load_ps(Cptr);
            
            D = _mm_moveldup_ps(A);
            D = _mm_mul_ps(D, B);
            
            A = _mm_movehdup_ps(A);
            B = _mm_shuffle_ps(B, B, 0xB1);
            A = _mm_mul_ps(A, B);
            
            D = _mm_addsub_ps(D, A);
            C = _mm_add_ps(C, D);
            _mm_store_ps(Cptr, C);
            
            // unroll
            
            A = _mm_load_ps(Aptr+4);
            B = _mm_load_ps(Bptr+4);
            C = _mm_load_ps(Cptr+4);
            
            D = _mm_moveldup_ps(A);
            D = _mm_mul_ps(D, B);
            
            A = _mm_movehdup_ps(A);
            B = _mm_shuffle_ps(B, B, 0xB1);
            A = _mm_mul_ps(A, B);
            
            D = _mm_addsub_ps(D, A);
            C = _mm_add_ps(C, D);
            _mm_store_ps(Cptr+4, C);
            
            // unroll
            
            A = _mm_load_ps(Aptr+8);
            B = _mm_load_ps(Bptr+8);
            C = _mm_load_ps(Cptr+8);
            
            D = _mm_moveldup_ps(A);
            D = _mm_mul_ps(D, B);
            
            A = _mm_movehdup_ps(A);
            B = _mm_shuffle_ps(B, B, 0xB1);
            A = _mm_mul_ps(A, B);
            
            D = _mm_addsub_ps(D, A);
            C = _mm_add_ps(C, D);
            _mm_store_ps(Cptr+8, C);
            
            // unroll
            
            A = _mm_load_ps(Aptr+12);
            B = _mm_load_ps(Bptr+12);
            C = _mm_load_ps(Cptr+12);
            
            D = _mm_moveldup_ps(A);
            D = _mm_mul_ps(D, B);
            
            A = _mm_movehdup_ps(A);
            B = _mm_shuffle_ps(B, B, 0xB1);
            A = _mm_mul_ps(A, B);
            
            D = _mm_addsub_ps(D, A);
            C = _mm_add_ps(C, D);
            _mm_store_ps(Cptr+12, C);
            
            Aptr += 16;
            Bptr += 16;
            Cptr += 16;
        }
#else
        for (int i = 0; i < cfft[L]-1; i+=2)
        {
            A = _mm_load_ps(Aptr);
            B = _mm_load_ps(Bptr);
            C = _mm_load_ps(Cptr);
            
            D = _mm_moveldup_ps(A);
            D = _mm_mul_ps(D, B);
            
            A = _mm_movehdup_ps(A);
            B = _mm_shuffle_ps(B, B, 0xB1);
            A = _mm_mul_ps(A, B);
            
            D = _mm_addsub_ps(D, A);
            C = _mm_add_ps(C, D);
            _mm_store_ps(Cptr, C);
            
            Aptr += 4;
            Bptr += 4;
            Cptr += 4;
        }
#endif
        Cptr[0]  += (Aptr[0] * Bptr[0]) - (Aptr[1] * Bptr[1]);
        Cptr[1]  += (Aptr[0] * Bptr[1]) + (Aptr[1] * Bptr[0]); 

        
    }

    // take ifft of FDL
    fftwf_execute(bck[L]); //take ifft of Yfft, save in outbuffer
    
    //copy output into double buffer
    memcpy(Ybuf[L]->getWriteBuffer(),outbuffer[L](), N[L]*sizeof(float));

    bcurr[L]++;

    return 0;
}

void PartConvFilter::reset()
{ //reset all internal buffers
    memset(bcurr,0,M*sizeof(unsigned));
    //memset(output_mix,0,N[0]*sizeof(float));
    //memset(input_mix,0,N[0]*sizeof(float));

    for (int L = 0; L < M; L++)
    {
        ind_N0_in_level[L] = num_N0_in_level[L] - 1;
        inbuffer[L].reset();
        Xfft[L].reset();
        Xbuf[L]->reset();
        Ybuf[L]->reset();
    }
}

double PartConvFilter::bench(const int L, const int TRIALS, const int POLLUTE_EVERY, const float MAX_TIME, double *benchData)
{
    //find WCET for each level
    int *pollute_cache = NULL;
    const int pollute_size = CACHE_SIZE/sizeof(int);

    if (POLLUTE_EVERY > 0)
    {
        pollute_cache = new int[pollute_size];
        for (int i = 0; i < pollute_size; i++)
            pollute_cache[i] = i;
    }


    // measure mean performance
    double mean = 0;
    int trial;

    int cnt = 0;
    for (trial = 0; trial < TRIALS; trial++)
    {
        if (POLLUTE_EVERY > 0)
        {
            if (cnt++ == POLLUTE_EVERY)
            {
                cnt = 0;
                //pollute the cache
                for (int i = 0; i < pollute_size; i++)
                    pollute_cache[i]++;
            }
        }

        mean -= get_time();
        runLevel(L);
        mean += get_time();

        bcurr[L]++;

        if(mean > MAX_TIME)
        {
            trial++;
            break;
        }
    }

    mean /= trial;

    if (benchData != NULL)
    {
        int value = 100*mean/((double)N[L]/FS);
        benchData[2*L] = value;
        benchData[2*L+1] = value;
    }

    if (POLLUTE_EVERY > 0)
        delete[] pollute_cache;
    reset();
    return mean;
}

void PartConvFilter::sync_levels()
{
    // which levels need to sync?  -------------------
    start_sync_depth = 0;
    end_sync_depth = 0;

    for (int L = 0; L < M; L++)
    {
        ind_N0_in_level[L]++;
        if (ind_N0_in_level[L] == num_N0_in_level[L])
        {
            ind_N0_in_level[L] = 0;
            start_sync_depth = L;
        }
        else if (ind_N0_in_level[L] == num_N0_in_level[L]-1)
            end_sync_depth = L;
    }
}

void PartConvFilter::sync_levels_relaxed()
{
    // which levels need to sync?  -------------------
    start_sync_depth = 0;
    end_sync_depth = 0;

    for (int L = 0; L < M; L++)
    {
        ind_N0_in_level[L]++;
        if (ind_N0_in_level[L] == num_N0_in_level[L])
        {
            ind_N0_in_level[L] = 0;
            start_sync_depth = L;
            end_sync_depth = L;
        }
    }
}

void PartConvFilter::input(const float* const input)
{
    for (int L = 0; L < M; L++)
    {
        Xbuf[L]->write(input);
        if (L <= start_sync_depth)
            Xbuf[L]->prepNextRead();
    }
}


void PartConvFilter::output(float* const output)
{
    memcpy(output, outbuffer[0](), N[0]*sizeof(float));
    for (int L = 1; L < M; L++)
    {
        if (L <= end_sync_depth)
            Ybuf[L]->swap(); //swap read/write buffers

        int readBlock = ind_N0_in_level[L] + 1;
        if (readBlock == num_N0_in_level[L])
            readBlock = 0;
        const float* const ptr = Ybuf[L]->getReadBuffer() + N[0]*readBlock;
        for (int i = 0; i < N[0]; i++)
            output[i] += ptr[i];
    }
}

void PartConvFilter::output_relaxed(float* const output)
{
    memcpy(output, outbuffer[0](), N[0]*sizeof(float));
    for (int L = 1; L < M; L++)
    {
        if (L <= end_sync_depth)
            Ybuf[L]->swap(); //swap read/write buffers

        int readBlock = ind_N0_in_level[L];
        if (readBlock == num_N0_in_level[L])
            readBlock = 0;
        const float* const ptr = Ybuf[L]->getReadBuffer() + N[0]*readBlock;
        for (int i = 0; i < N[0]; i++)
            output[i] += ptr[i];
    }
}


/* PartConvMax methods */

int PartConvMax::setup(int inSampleRate, int n, int m, int k, float* impulses, int filterLength, int stride, int* blockSizes, int levels, int fftwOptLevel, const char* wisdom, int max_threads_per_level, int max_level0_threads)
{
    int ret;
    
    FS = inSampleRate;

    numInputChannels = n;
    numOutputChannels = m;
    if ( (n == m) && (n == k) )  //parallel filtering with distinct impulse responses
    {
        numPCs = k;
        numRepeats = 1;
        inputChannel = new int[numPCs];
        outputChannel = new int[numPCs];

        for (int i = 0; i < numPCs; i++)
        {
            inputChannel[i] = i;
            outputChannel[i] = i;
        }
    }
    else if ( (n == m) && (k == 1) ) //parallel with identical impulse reponse
    {
        
        numPCs = 1;
        numRepeats = n;
        inputChannel = new int[numRepeats];
        outputChannel = new int[numRepeats];

        for (int i = 0; i < numRepeats; i++)
        {
            inputChannel[i] = i;
            outputChannel[i] = i;
        }
    }
    else if ( m == n*k ) //filterbank
    {
        numPCs = k;
        numRepeats = n;
        inputChannel = new int[numPCs*numRepeats];
        outputChannel = new int[numPCs*numRepeats];

        for (int i = 0; i < numPCs; i++)
        {
            for (int j = 0; j < numRepeats; j++)
            {
                inputChannel[i*numRepeats+j] = j;
                outputChannel[i*numRepeats+j] = i*numRepeats+j;
            }
        }
    }
    else
    {
        fprintf(stderr, "PartConvMax::setup() does not support requested channel configuration\n");
        return 1; 
    }



    buffer_size = blockSizes[0];
    num_levels = levels;

    int *num_parts = new int[num_levels]();

    int total_length = 0;
    for (int i = 0; i < num_levels-1; i++)
    {
        num_parts[i] = 2*blockSizes[i+1]/blockSizes[i] - 2;
        total_length += blockSizes[i]*num_parts[i];
        // check if partitioning is already long enough for filter
        if (total_length >= filterLength)
        {
            num_levels = i+1;
            break;
        }
    }
    
    //output_mix = new float[buffer_size*numOutputChannels];
    //input_mix = new float[buffer_size*numInputChannels];
    outbuffers = new float*[numOutputChannels];
    inbuffers = new float*[numInputChannels];
    outbuffers[0] = new float[buffer_size*numOutputChannels];
    inbuffers[0] = new float[buffer_size*numInputChannels];
    for (int i = 1; i < numOutputChannels; i++)
        outbuffers[i] = outbuffers[0] + i*buffer_size;
    for (int i = 1; i < numInputChannels; i++)
        inbuffers[i] = inbuffers[0] + i*buffer_size;


    pc = new PartConvFilter[numPCs*numRepeats];
    
    //numPCs *= numRepeats;

    
    level_counter = new unsigned[num_levels]; 
    level_size = new unsigned[num_levels];
    level_map = NULL;

    for (int i = 0; i < num_levels; i++)
    {
        level_size[i] = blockSizes[i]/blockSizes[0];
        level_counter[i] = level_size[i] - 1;
    }



    if (wisdom != NULL)
    {
        if (FILE* wisdomfile = fopen(wisdom,"r"))
        {
            fftwf_import_wisdom_from_file(wisdomfile);
            fclose(wisdomfile);
        }
    }

    Vector impulseResponse(filterLength);
    for (int i = 0; i < numPCs; i++)
    {
        if (stride == 1)
            impulseResponse.copyFrom(&impulses[filterLength*i]);
        else
        {
            for (int l = 0; l < filterLength; l++)
                impulseResponse[l] = impulses[stride*(filterLength*i + l)];
        }

        for (int j = 0; j < numRepeats; j++)
        {
            const int ind = i*numRepeats+j;
            if (verbosity > 2)
            {
                pc[ind].verbosity = 1;
            }
            else
            {
                pc[ind].verbosity = 0;
            }

            ret = pc[ind].setup(num_levels,blockSizes,num_parts,&impulseResponse,fftwOptLevel);

            if (ret)
            {
                printf("setup PartConvFilter %u.%u (%u): failed\n",i,j,ind);
                return 1;
            }
        }
    }
    if (wisdom != NULL)
    {
        if (FILE* wisdomfile = fopen(wisdom,"w"))
        {
            fftwf_export_wisdom_to_file(wisdomfile);
            fclose(wisdomfile);
        }
    }

    if (verbosity > 0)
    {
        printf("\nPartitioning scheme:\n");
        for(int i = 0; i < num_levels-1; i++)
        {
            printf("%ux%u, ",pc[0].N[i],pc[0].num_parts[i]);
        }
            printf("%ux%u",pc[0].N[num_levels-1],pc[0].num_parts[num_levels-1]);
        printf("\n\n");
    }

    // now treat all PCs the same whether they are repeats or not.
    numPCs *= numRepeats;
    numRepeats = 1;

    delete[] num_parts;

    // divide up work amongst threads
    threads_in_level = new int[num_levels](); //init 0's
    workList = new WorkList**[num_levels];
    workList[0] = new WorkList*[num_levels*max_threads_per_level](); //init 0's (NULL's)
    for (int i = 1; i < num_levels; i++)
        workList[i] = workList[0] + i*max_threads_per_level;





    const float callback_time = (float)buffer_size/FS;
    double work_sum[num_levels][max_threads_per_level];
    double work_level_sum[num_levels];

#ifdef MEASURE_COMPUTE_TIME
    const bool measure_flag = true;
#else
    const bool measure_flag = false;
#endif

    //bool time_constraint_policy[num_levels];
    for (int l = 0; l < num_levels; l++)
    {  
        /* only benchmark level zero FDLs for providing timing info to real-time policy */

        double t;
        if (measure_flag && l == 0)
        {
            const int trials = 1000;
            const int pollute_every = 3;
            const float max_time = 0.1;
            t = pc[0].bench(l,trials,pollute_every,max_time);
        }
        else
            t = 1.0;

        double work_target = 0;
        for(int i = 0; i < numPCs; i++)
        {
            work_target += t;
        }
        work_target /= max_threads_per_level;

        work_level_sum[l] = 0;
        int thr = 0;
        work_sum[l][thr] = 0;
        for (int indPC = 0; indPC < numPCs; indPC++)
        {

            if (work_sum[l][thr] >= work_target)
            {
                thr++;
                work_sum[l][thr] = 0;
                if (thr == max_threads_per_level)
                {
                    fprintf(stderr,"work partitioning error\n");
                    exit(-1);
                }
            }

            // add work unit to linked list
            workList[l][thr] = new WorkList(workList[l][thr]);

            workList[l][thr]->arg = l;
            workList[l][thr]->ind = indPC;
            workList[l][thr]->obj = &pc[indPC];
            workList[l][thr]->time = t;
            work_sum[l][thr] += t;
            work_level_sum[l] += t;
            if (verbosity > 1)
                printf("level: %u, thr: %u, indPC: %u, new_work: %f, work_sum: %f\n",
                        l,thr,indPC,t,work_sum[l][thr]);
        }
        threads_in_level[l] = thr+1;
        
        if (verbosity > 1)
            printf("threads total: %u\n",threads_in_level[l]);
    }
    //
    // Level 0 stuff
    total_level0_work_units = threads_in_level[0];
    threads_in_level[0] = min( total_level0_work_units - 1, max_level0_threads );
    first_thread_level = (threads_in_level[0] == 0); // 0 if no L0 threads
    // -----




    // setup threading stuff ----------------------------------------------

    terminate_flag = 0;

    // worker_thread[level][thread]
    worker_thread = new pthread_t*[num_levels];
    worker_thread[0] = new pthread_t[num_levels*max_threads_per_level];
    for (int i = 1; i < num_levels; i++)
        worker_thread[i] = worker_thread[0] + i*max_threads_per_level;

    workerThreadData = new WorkerThreadData*[num_levels];
    workerThreadData[0] = new WorkerThreadData[num_levels*max_threads_per_level];
    for (int i = 1; i < num_levels; i++)
        workerThreadData[i] = workerThreadData[0] + i*max_threads_per_level;

    worker_state = new int*[num_levels]; 
    worker_state[0] = new int[num_levels*max_threads_per_level](); //init 0's
    for (int i = 1; i < num_levels; i++)
        worker_state[i] = worker_state[0] + i*max_threads_per_level;
    
    thread_counter = new int[num_levels](); //init 0
    sync_target = new int[num_levels](); //init 0
    worker_cond = new pthread_cond_t[num_levels];
    worker_mutex = new pthread_mutex_t[num_levels];
    main_cond = new pthread_cond_t[num_levels];
    main_mutex = new pthread_mutex_t[num_levels];
    pthread_attr_t thread_attr[num_levels];


    for (int i = first_thread_level; i < num_levels; i++)
    {
        for (int j = first_thread_level; j <= i; j++)
            sync_target[i] += threads_in_level[j];
    }

    for (int i = first_thread_level; i < num_levels; i++)
    {
        pthread_cond_init(&worker_cond[i],NULL);
        pthread_mutex_init(&worker_mutex[i],NULL);
        pthread_cond_init(&main_cond[i],NULL);
        pthread_mutex_init(&main_mutex[i],NULL);
    }


    for (int i = first_thread_level; i < num_levels; i++)
    {
        struct sched_param thread_param;
        pthread_attr_init(&thread_attr[i]);
        pthread_attr_setdetachstate(&thread_attr[i], PTHREAD_CREATE_JOINABLE);
        pthread_attr_setschedpolicy(&thread_attr[i], SCHED_FIFO);

        pthread_attr_getschedparam(&thread_attr[i], &thread_param);

        //thread_param.sched_priority = sched_get_priority_max(SCHED_FIFO) - i + 15;
#ifdef _PORTAUDIO_
        // for some reason, we have to set it explicitly here
        const int actual_max_priority = 63; 
#else
        // when running in Max/MSP, 48 actually maps to 63
        const int actual_max_priority = 48; //apple low balls the call to sched_get_priority_max(SCHED_FIFO)
#endif
        thread_param.sched_priority = actual_max_priority - log2i(level_size[i]) + 1;
        // level-0 always runs in callback thread.  

        pthread_attr_setschedparam(&thread_attr[i], &thread_param);
        pthread_attr_setinheritsched(&thread_attr[i], PTHREAD_EXPLICIT_SCHED);



        for (int j = 0; j < threads_in_level[i]; j++)
        {
            workerThreadData[i][j].This = this;
            workerThreadData[i][j].levelNum = i;
            workerThreadData[i][j].threadNum = j;

            worker_state[i][j] = 1;

            int err = pthread_create(&worker_thread[i][j], &thread_attr[i], workerThreadEntry, (void*)&workerThreadData[i][j]);
            if ( err != 0) 
            {
                printf("pthread_create error: %d\n",err); 
                if (err == EPERM)
                    printf("EPERM\n");
                exit(-1);
            }


#ifdef __APPLE__ 
            if (verbosity > 1)
                printf("level: %u.%u, worksum: %g, callback_time: %g\n",
                        i,j,work_sum[i][j],callback_time);

            if (i == 0 && measure_flag)
            {
                thread_time_constraint_policy_data_t time_policy;
                const int bus_speed = get_bus_speed();
                const float level_time = (float)level_size[i]*buffer_size/FS;


                time_policy.period = (uint32_t)bus_speed * .93985 * level_time;
                const uint32_t comp_max = 5E7; // 47ms @ bus_speed = 1064000000
                const uint32_t comp_min = 5E4; // 47us
                time_policy.computation = (uint32_t)(work_sum[i][j] * bus_speed);
                time_policy.computation = max( time_policy.computation, comp_min); //5E4/1064E6=0.05ms
                time_policy.computation = min( time_policy.computation, comp_max); //5E4/1064E6=0.05ms
                time_policy.constraint = (uint32_t)time_policy.period * 0.7;  // would like to leave cushion
                time_policy.preemptible = 1;
                err = thread_policy_set(
                        pthread_mach_thread_np(worker_thread[i][j]),
                        THREAD_TIME_CONSTRAINT_POLICY,
                        (thread_policy_t)&time_policy,
                        THREAD_TIME_CONSTRAINT_POLICY_COUNT);

                if (err != KERN_SUCCESS)
                {
                    printf("mach set policy error: %d\n",err); 
                    printf("not able to set time constraint policy for level: %u.%u\n",i,j);
                    printf("%u.%u mach cycles: %u %u %u %u %u\n",
                            i,j,
                            time_policy.period,
                            time_policy.computation,
                            time_policy.constraint,
                            time_policy.preemptible,
                            get_bus_speed()
                          );
                    printf("%u.%u mach time: %g %g %g %u %u\n",
                            i,j,
                            time_policy.period/(float)bus_speed,
                            time_policy.computation/(float)bus_speed,
                            time_policy.constraint/(float)bus_speed,
                            time_policy.preemptible,
                            get_bus_speed()
                          );
                }
            }


#ifdef _DEBUG_ 
            // check policy params

            printf("\nSETUP PRIORITIES OF %u.%u\n",i,j);
            struct sched_param thread_param;
            int policy;
            pthread_getschedparam(worker_thread[i][j], &policy, &thread_param);
            if (policy == SCHED_FIFO)
                printf("pthread policy: SCHED_FIFO\n");
            else
                printf("pthread policy: not SCHED_FIFO\n");
            printf("priority: %u\n",thread_param.sched_priority);


            thread_time_constraint_policy_data_t time_policy;
            mach_msg_type_number_t count = THREAD_TIME_CONSTRAINT_POLICY_COUNT;
            boolean_t get_default = 0;
            err = thread_policy_get(
                    pthread_mach_thread_np(worker_thread[i][j]),
                    THREAD_TIME_CONSTRAINT_POLICY,
                    (thread_policy_t)&time_policy,
                    &count,
                    &get_default);
            if ( err != KERN_SUCCESS) 
            {
                printf("mach get policy error: %d (L=%u)\n",err,i); 
                //exit(-1);
            }
            else
            {
                printf("%u mach time: %u %u %u %u %u %u\n",
                        i,
                        time_policy.period,
                        time_policy.computation,
                        time_policy.constraint,
                        time_policy.preemptible,
                        get_bus_speed(),
                        get_default
                      );
            }
            get_default = 0;
            count = THREAD_PRECEDENCE_POLICY_COUNT;
            thread_precedence_policy_data_t prec_policy;
            err = thread_policy_get(
                    pthread_mach_thread_np(worker_thread[i][j]),
                    THREAD_PRECEDENCE_POLICY,
                    (thread_policy_t)&prec_policy,
                    &count,
                    &get_default);
            if ( err != KERN_SUCCESS) 
            {
                printf("mach get precedence error: %d (L=%u)\n",err,i); 
                //exit(-1);
            }
            else
            {
                printf("%u mach prec: %d %d\n", i, prec_policy.importance, get_default);
            }
#endif //_DEBUG_
#endif //__APPLE__
        }
        pthread_attr_destroy(&thread_attr[i]);

    }


    int end_sync_depth = num_levels-1;
    pthread_mutex_lock(&main_mutex[end_sync_depth]);
    while( thread_counter[end_sync_depth] != sync_target[end_sync_depth])
        pthread_cond_wait(&main_cond[end_sync_depth],&main_mutex[end_sync_depth]);
    pthread_mutex_unlock(&main_mutex[end_sync_depth]);


    if(verbosity > 1)
        printf("Total PartConvFilter instances   : %d\n\n", numPCs);
    
    frameNum = 0;

    return 0;
}

int PartConvMax::cleanup(void)
{
    // join worker threads
    __sync_add_and_fetch(&terminate_flag, 1);
    for (int i = first_thread_level; i < num_levels; i++)
    {
        for (int j = 0; j < threads_in_level[i]; j++)
            worker_state[i][j] = -1;

    }
    for (int i = first_thread_level; i < num_levels; i++)
        pthread_cond_broadcast(&worker_cond[i]);

    for (int i = first_thread_level; i < num_levels; i++)
    {
        for (int j = 0; j < threads_in_level[i]; j++)
            pthread_join(worker_thread[i][j],NULL);
    }
        
    for (int i = 0; i < numPCs; i++)
    {
        pc[i].cleanup();
    }




    delete[] inputChannel;
    delete[] outputChannel;
    delete[] pc;

    //delete[] input_mix;
    //delete[] output_mix;
    delete[] outbuffers[0];
    delete[] inbuffers[0];
    delete[] outbuffers;
    delete[] inbuffers;

    delete[] level_counter; 
    delete[] level_size;


    for (int l = 0; l < num_levels; l++)
    {
        for (int thr = 0; thr < threads_in_level[l]; thr++)
        {
            WorkList *curr = workList[l][thr];
            WorkList *next;
            while(curr != NULL)
            {
                next = curr->next;
                delete curr;
                curr = next;
            }
        }

    }

    delete[] threads_in_level;
    delete[] sync_target;
    delete[] thread_counter;
    delete[] workList[0];
    delete[] workList;

    for (int i = first_thread_level; i < num_levels; i++)
    {
        pthread_cond_destroy(&worker_cond[i]);
        pthread_mutex_destroy(&worker_mutex[i]);
        pthread_cond_destroy(&main_cond[i]);
        pthread_mutex_destroy(&main_mutex[i]);
    }
    delete[] worker_thread[0];
    delete[] worker_thread;
    delete[] workerThreadData[0];
    delete[] workerThreadData;
    delete[] worker_state[0];
    delete[] worker_state;
    delete[] worker_cond;
    delete[] worker_mutex;
    delete[] main_cond;
    delete[] main_mutex;


    return 0;
}

int PartConvMax::run(float** const output, float** const input) 
{
    // which levels need to sync?  -------------------
    int start_sync_depth = 0;
    int end_sync_depth = 0;

    for (int l = 0; l < num_levels; l++)
    {
        level_counter[l]++;
        if (level_counter[l] == level_size[l])
        {
            level_counter[l] = 0;
            start_sync_depth = l;
        }
        else if (level_counter[l] == level_size[l]-1)
            end_sync_depth = l;
    }



    // update buffering indexing
    for (int i=0; i < numPCs; i++)
        pc[i].sync_levels();

    // buffer input
    for (int i=0; i < numPCs; i++)
    {
        pc[i].input(input[inputChannel[i]]);
    }

    // start computation threads -------------------------
    if (start_sync_depth >= first_thread_level)
    {
        pthread_mutex_lock(&worker_mutex[start_sync_depth]);

        thread_counter[start_sync_depth] = 0;
        for (int l = first_thread_level; l <= start_sync_depth; l++)
            for (int j = 0; j < threads_in_level[l]; j++)
                worker_state[l][j] = 1;


        pthread_cond_broadcast(&worker_cond[start_sync_depth]);
        pthread_mutex_unlock(&worker_mutex[start_sync_depth]);
    }

    // execute a subset of L0 work
    for (int j = threads_in_level[0]; j < total_level0_work_units; j++)
    {
        WorkList *curr = workList[0][j];
        while(curr != NULL)
        {
            curr->obj->runLevel(curr->arg);
            curr = curr->next;
        }
    }


    // end computation threads ------------------------------------
    if (end_sync_depth >= first_thread_level)
    {
        if( thread_counter[end_sync_depth] != sync_target[end_sync_depth]) //check if all 0's to sync depth
        {
            pthread_mutex_lock(&main_mutex[end_sync_depth]);
            while( thread_counter[end_sync_depth] != sync_target[end_sync_depth])
            {
                pthread_cond_wait(&main_cond[end_sync_depth],&main_mutex[end_sync_depth]);
            }
            pthread_mutex_unlock(&main_mutex[end_sync_depth]);
        }
        //reset thread_counter to zero
        int ret = __sync_sub_and_fetch(&thread_counter[end_sync_depth],sync_target[end_sync_depth]);
        if (ret != 0)
            fprintf(stderr,"atomic reset error!\n");

    }


    // copy output
    for (int i=0; i < numPCs; i++)
        pc[i].output(output[outputChannel[i]]);

    frameNum++;

    return 0;
}

void PartConvMax::reset()
{ 
    //reset all internal buffers
    // do we need to reset any other state?

    for (int i = 0; i < numPCs; i++)
        pc[i].reset();
}

void *PartConvMax::workerThreadEntry(void *arg)
{
    WorkerThreadData *workerThreadData = (WorkerThreadData*)arg;
    PartConvMax *pc = (PartConvMax*)workerThreadData->This;
    const int L = workerThreadData->levelNum;
    const int thr = workerThreadData->threadNum;
    WorkList* const localWorkList = pc->workList[L][thr];
    int* const local_state = &pc->worker_state[L][thr];
    int* const thread_counter = pc->thread_counter;
    int* const sync_target = pc->sync_target;

    const int num_levels = pc->num_levels;

    unsigned thread_level_counter[num_levels];
    unsigned thread_level_size[num_levels];
    for (int l = L; l < num_levels; l++)
    {
        thread_level_size[l] = pc->level_size[l]/pc->level_size[L];
        thread_level_counter[l] = thread_level_size[l] - 1;
    }

    unsigned sync_depth;

    while(!pc->terminate_flag) 
    {
        //figure out number of levels that need to sync
        //assuming that whenever a larger block needs to sync, all smaller blocks do too
        sync_depth = L;
        for (int l = L+1; l < num_levels; l++)
        {
            thread_level_counter[l]++;
            if (thread_level_counter[l] == thread_level_size[l])
            {
                thread_level_counter[l] = 0;
                sync_depth = l;
            }
        }

        __sync_sub_and_fetch (local_state, 1);  //*local_state = 0
        int ret = __sync_add_and_fetch (&thread_counter[sync_depth], 1);  //thread_counter[L] += 1

        if (ret == sync_target[sync_depth])
        {
            // DO NEED MUTEX:
            // if no mutex, sometimes main thread will miss the signal below and never wake up.
            pthread_mutex_lock(&pc->main_mutex[sync_depth]);
            pthread_cond_signal(&pc->main_cond[sync_depth]);
            pthread_mutex_unlock(&pc->main_mutex[sync_depth]);
        }

        
        pthread_mutex_lock(&pc->worker_mutex[sync_depth]);
        while (*local_state == 0)
        {
            pthread_cond_wait(&pc->worker_cond[sync_depth],&pc->worker_mutex[sync_depth]);
        }
        pthread_mutex_unlock(&pc->worker_mutex[sync_depth]);

        if (*local_state < 0) //terminate signal
        {
            break;
        }



        // do work son! ---------------
        WorkList *curr = localWorkList;
        while (curr != NULL)
        {
            curr->obj->runLevel(curr->arg);
            curr = curr->next;
        }
        // --------------------------



    }




    pthread_exit(NULL);
}


/* DelayBuffer methods */

DelayBuffer::DelayBuffer(int inFrameSize, int inBlockSize, int delay)
{
    frameSize = inFrameSize;
    framesPerBlock = inBlockSize;
    // already a delay from compute time and initial N[0] buffering
    readFrameDelay = delay - framesPerBlock - 1;

    // round buffer size up to next multiple of block size
    int rem = (readFrameDelay+1) % framesPerBlock;
    bufferSize = readFrameDelay + 1 + (framesPerBlock - rem);
    numBlocks = bufferSize/framesPerBlock + 1; //extra block to prevent read/write overlap 
    bufferSize = numBlocks*framesPerBlock;
    bufferMem = new float[bufferSize*frameSize]();
    
    buffer = new float*[bufferSize];
    for (int i = 0; i < bufferSize; i++)
        buffer[i] = bufferMem + i*frameSize;

    readBlock = numBlocks-1;
    writeBlock = readFrameDelay/framesPerBlock;
    writeFrame = readFrameDelay % framesPerBlock;
}

DelayBuffer::~DelayBuffer()
{
    delete[] bufferMem;
    delete[] buffer;
}

void DelayBuffer::write(const float* const x)
{
    // write mem at x to current frame
    int frame = writeBlock*framesPerBlock + writeFrame;
    memcpy(buffer[frame],x,frameSize*sizeof(float));
    writeFrame++;
    if (writeFrame == framesPerBlock)
    {
        writeFrame = 0;
        writeBlock++;
        if (writeBlock == numBlocks)
            writeBlock = 0;
    }
}

void DelayBuffer::read(float* y)
{
    // copy current block to mem at y
    int frame = readBlock * framesPerBlock;
    if (readBlock == writeBlock)
    {
        printf("ERROR: readBlock (%u) == writeBlock (%u)\n",readBlock,writeBlock);
        printf("frameSize = %u, framesPerBlock = %u, numBlocks = %u\n",frameSize,framesPerBlock,numBlocks);
        exit(-1);
        //return;
    }
    memcpy(y, buffer[frame], frameSize*framesPerBlock*sizeof(float));
}

void DelayBuffer::reset()
{
    memset(bufferMem,0,bufferSize*sizeof(float));
    readBlock = numBlocks-1;
    writeBlock = readFrameDelay/framesPerBlock;
    writeFrame = readFrameDelay % framesPerBlock;
}

void DelayBuffer::prepNextRead()
{
    readBlock += 1;
    if (readBlock == numBlocks)
        readBlock = 0;
}


/* DoubleBuffer methods */

DoubleBuffer::DoubleBuffer(int inSize)
{
    size = inSize;
    readBuffer = new float[size]();
    writeBuffer = new float[size]();
}

DoubleBuffer::~DoubleBuffer()
{
    delete[] readBuffer;
    delete[] writeBuffer;
}

void DoubleBuffer::reset()
{
    memset(readBuffer,0,size*sizeof(float));
    memset(writeBuffer,0,size*sizeof(float));
}

float* DoubleBuffer::getReadBuffer()
{
    return readBuffer;
}

float* DoubleBuffer::getWriteBuffer()
{
    return writeBuffer;
}

void DoubleBuffer::swap()
{
    float* temp = readBuffer;
    readBuffer = writeBuffer;
    writeBuffer = temp;
}


