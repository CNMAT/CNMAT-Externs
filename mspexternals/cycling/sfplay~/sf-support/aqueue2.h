//#include <MacTypes.h> //temp while adding navservices-jkc

typedef struct aq
{
	char *a_buf;
	long a_size;
	long a_atom;
	double a_head;	// read ptr
	long a_tail;	// write ptr
	long a_tailrev;	// reverse write ptr
} t_aqueue;

t_aqueue *aq_new(long size, long atom);
void aq_free(t_aqueue *a);
void aq_init(t_aqueue *a);
long aq_spacetowrite(t_aqueue *a);
long aq_spacetoread(t_aqueue *a);
long aq_spacetowrite_rev(t_aqueue *a);
long aq_spacetoread_rev(t_aqueue *a);
long aq_gethead(t_aqueue *a, Ptr *outptr);
long aq_gettail(t_aqueue *a, Ptr *outptr);
long aq_gethead_rev(t_aqueue *a, Ptr *outptr);
long aq_gettail_rev(t_aqueue *a, Ptr *outptr);
void aq_incrtail(t_aqueue *a, long count);
void aq_incrtail_rev(t_aqueue *a, long count);
void aq_incrhead(t_aqueue *a, double count);
void aq_decrtail(t_aqueue *a, long count);
void aq_decrhead(t_aqueue *a, double count);
void aq_read(t_aqueue *a, Ptr dst, long count);
void aq_copyfrom(t_aqueue *a, Ptr src, long count, long pos);
void aq_copyfrom_rev(t_aqueue *a, Ptr src, long count, long pos);
void aq_samptype2float(t_aqueue *a, float *fout, long count, long offset, long skip, long sampsize, unsigned long comptype, long swap, double speed);
long aq_float2samptype(float *in, t_aqueue *a, long n, long offset, long skip, long bytes, unsigned long samptype, short byteswap, double resamp, double *current);
long aq_float2samptype_interp(float *in, t_aqueue *a, long n, long offset, long skip, long bytes, unsigned long samptype, short byteswap, double resamp, double *current, double last);

void aq_incrhead_ex(t_aqueue *a, int samples, int skip, int samplesize, double speed);

