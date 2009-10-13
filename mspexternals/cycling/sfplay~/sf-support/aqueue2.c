#include "ext.h"
#include "aqueue2.h"
#include "sf2.h"

// queue routines

#ifndef ABS 
#define ABS(x) ((x)<0?-(x):(x))
#endif

t_aqueue *aq_new(long size, long atom)
{
	t_aqueue *a = (t_aqueue *)sysmem_newptr(sizeof(t_aqueue));
	if (!a)
		return 0;
	a->a_buf = sysmem_newptr(size);
	if (!a->a_buf) {
		sysmem_freeptr(a);
		return 0;
	}
	a->a_size = size;
	a->a_atom = atom;
	aq_init(a);
	return a;
}

void aq_free(t_aqueue *a)
{
	sysmem_freeptr(a->a_buf);
	sysmem_freeptr(a);
}

void aq_init(t_aqueue *a)
{
	a->a_head = 0;
	a->a_tail = 0;
	a->a_tailrev = 0;
}

long aq_spacetowrite(t_aqueue *a)	// "write count"--how much isn't in queue
{
	return ((a->a_head > a->a_tail) ? (a->a_head - a->a_tail - a->a_atom) : 
		(a->a_head + a->a_size - a->a_tail - a->a_atom));
}

long aq_spacetoread(t_aqueue *a)	// "read count"--how much is in queue
{
	return ((a->a_tail >= a->a_head) ? (a->a_tail - a->a_head) : 
		(a->a_tail + a->a_size - a->a_head));
}

long aq_spacetowrite_rev(t_aqueue *a)	// "write count"--how much isn't in queue
{
	return ((a->a_tailrev >= a->a_head) ? (a->a_tailrev - a->a_head - a->a_atom) : 
		(a->a_tailrev + a->a_size - a->a_head - a->a_atom));
}

long aq_spacetoread_rev(t_aqueue *a)	// "read count"--how much is in queue
{
	return ((a->a_head > a->a_tailrev) ? (a->a_head - a->a_tailrev) : 
		(a->a_head + a->a_size - a->a_tailrev));
}

//gethead nolonger makes any sense...
long aq_gethead(t_aqueue *a, Ptr *outptr)	// where to read from, return 
{
	long count;
	
	*outptr = a->a_buf + (long)a->a_head;
	count = aq_spacetoread(a);
	return ((count + a->a_head > a->a_size) ? a->a_size - a->a_head : count);
}

long aq_gettail(t_aqueue *a, Ptr *outptr)
{
	long count;
	
	*outptr = a->a_buf + a->a_tail;
	count = aq_spacetowrite(a);
    return ((count + a->a_tail > a->a_size) ? a->a_size - a->a_tail : count);
}

//gethead nolonger makes any sense
/*long aq_gethead_rev(t_aqueue *a, Ptr *outptr)	// where to read from, return 
{
	long count;
	
	*outptr = a->a_buf + a->a_head;
	count = aq_spacetoread_rev(a);
	return ((a->a_head - count < 0) ? a->a_head : count);
}
*/

long aq_gettail_rev(t_aqueue *a, Ptr *outptr)
{
	long count;
	
	*outptr = a->a_buf + a->a_tailrev;
	count = aq_spacetowrite_rev(a);
    return ((a->a_tailrev - count < 0) ? a->a_tailrev : count);
}

void aq_incrtail(t_aqueue *a, long count) // update write pointer
{
	a->a_tail += count;
	a->a_tail = (a->a_tail >= a->a_size) ? a->a_tail - a->a_size : a->a_tail;
	//if our tailrev is inbetween tail and head set to tail
	if (a->a_tail < a->a_head){
		if ((a->a_tail > a->a_tailrev) || (a->a_tailrev > a->a_head))
			a->a_tailrev = a->a_tail;
	} else if (a->a_tail > a->a_head) {
		if ((a->a_tailrev > a->a_head) && (a->a_tailrev < a->a_tail))
			a->a_tailrev = a->a_tail;
	}
}

void aq_incrtail_rev(t_aqueue *a, long count) // update write pointer
{
	a->a_tailrev -= count;
	a->a_tailrev = (a->a_tailrev <= 0) ? a->a_tailrev + a->a_size : a->a_tailrev;
	//if our tail is inbetween tailrev and head set to tailrev
	if (a->a_tailrev > a->a_head){
		if ((a->a_tail > a->a_tailrev) || (a->a_tail < a->a_head))
			a->a_tail = a->a_tailrev;
	} else if (a->a_tailrev < a->a_head) {
		if ((a->a_tail < a->a_head) && (a->a_tail > a->a_tailrev))
			a->a_tail = a->a_tailrev;
	}
}

void aq_incrhead(t_aqueue *a, double count) // update read pointer
{
	a->a_head += count;
	if (a->a_head >= a->a_size)
		a->a_head = a->a_head - (double)a->a_size;
}

void aq_incrhead_ex(t_aqueue *a, int samples, int skip, int sampsize, double speed) // update read pointer
{
	double jump,jumpfrak,head,headfrak,direction;
	long jumplong,headlong,c,framesize; 
	
	if (speed >= 0)
		direction = 1;
	else
		direction = -1;
		
	//post("aq_incrhead_ex: samples=%d,skip=%d,sampsize=%d,speed=%f",samples,skip,sampsize,speed);
	//account for fp round off error
	framesize = skip*sampsize;
	headlong = (long)a->a_head;
	if ((c=(headlong%framesize))>=sampsize) {
		//post("aq_incrhead_ex: OUT OF FRAMESYNC: headlong=%d,framesize=%d,sampsize=%d,c=%d",headlong,framesize,sampsize,c);
		//out of frame sync. nudge to nearest frame
		headlong = headlong/framesize;
		if (c>(framesize/2))
			headlong++; 
		a->a_head = (double)(headlong*framesize);
	}
	
	//treat head as samples
	head = a->a_head/(double)sampsize;
	headlong = (long)head;
	headfrak = head - (double)headlong;
	//calculate jump in samples
	jump = (double)samples * speed;
	jumplong = (long)jump;
	jumpfrak = jump - (double)jumplong;
	//calculate jump in bytes preserving sample position
	jump = jumplong*(double)framesize;
	jumpfrak += headfrak;
	if (jumpfrak > 1.) {
		jump += framesize;
		jumpfrak -= 1;
	} else if (jumpfrak < 0.) { 
		jump -= framesize;
		jumpfrak += 1;
	} 
	jump += jumpfrak*(double)sampsize;
	
	a->a_head = (double)headlong*sampsize + (double)jump;
	if (a->a_head >= a->a_size)
		a->a_head = a->a_head - (double)a->a_size;
	else if  (a->a_head < 0.)
		a->a_head = (double)a->a_size + a->a_head;

	if (ABS(a->a_head-a->a_tail)<framesize)
	{
		//post("---------------------------NOT A FULL FRAME LEFT-----------------------");
		a->a_head = a->a_tail;
	}
}

/*
void aq_decrtail(t_aqueue *a, long count) // update write pointer
{
	a->a_tail -= count;
	a->a_tail = (a->a_tail <= 0) ? a->a_size + a->a_tail : a->a_tail;
}

void aq_decrhead(t_aqueue *a, double count) // update read pointer
{
	a->a_head -= count;
	if (a->a_head < 0)
		a->a_head = a->a_size + a->a_head;
}
*/
// routine that copies data, wrapping around if necessary
 
/*
void aq_read(t_aqueue *a, Ptr dst, long count)
{
    long copy_count = (count + a->a_head > a->a_size) ? a->a_size - a->a_head : count;
	BlockMoveData(a->a_buf + a->a_head, dst, copy_count);
	count -= copy_count;
	if (count == 0) {
		a->a_head += copy_count;
		if (a->a_head == a->a_size)
			a->a_head = 0;
	} else {
		BlockMoveData(a->a_buf, dst + copy_count, count);
		a->a_head = count;
	}
}
*/

//in if statement, i used comma below instead of block, which was causing problems
#define	CONVERT_LOOP_MACRO   				\
		f = (double)i++ * speed + headfrak; \
		dex = (long)f; 						\
		frak = f - (double) dex;			\
		if (frak < 0.)  (--dex, ++frak);  	\
		c = dex*skip + headlong;			\
		d = c+skip;							\
		if (c >= size) c -= size;			\
		else if (c < 0.) c += size;			\
		if (d >= size) d -= size;			\
		else if (d < 0.) d += size;			

#define	CONVERT_LOOP_MACRO_NOINTERP   		\
		dex = i++ * speed_int;   			\
		c = dex*skip + headlong;			\
		if (c >= size) c -= size;			\
		else if (c < 0.) c += size;			


//specialized function for audio queue for wrapping + speed		
void aq_samptype2float(t_aqueue *a, float *fout, long count, long offset, long skip, long sampsize, unsigned long comptype, long swap, double speed)
{
	double head;
	char *buf=a->a_buf;
	double f,frak,headfrak;
	long dex,c,d,headlong,size,speed_int,lval,lval2;
	int i=0;
	Byte *bp;
	short *sp;
	long *lp;
	float *fp;
	double *dp;
	
	bp = (Byte *)buf;
	sp = (short *)buf;
	lp = (long *)buf;
	fp = (float *)buf;
	dp = (double *)buf;
		
	//head in terms of samples
	head = a->a_head/(double)sampsize+(double)offset;
	size = a->a_size/(double)sampsize;
	headlong = (long)head;
	headfrak = head - (double)headlong;
	
	speed_int = (long)speed;
		
	++count;
	--fout; //less one for the more efficient preinc

	if ((speed-speed_int)==0) { //use faster no-interp
		if (swap) {
			if (comptype == SF_SAMPTYPE_INT) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (float)(buf[c]) * (1./128.);
						} 
					} else if (sampsize==2) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = ((float)(BYTEORDER_SWAPW16(sp[c])) * (1./32768.)); 
						}
					} else if (sampsize==3) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							c *= 3;
#if C74_LITTLE_ENDIAN
							*++fout = (((float)(((bp[c])<<24L)+((bp[c+1])<<16L)+((bp[c+2])<<8L)))
									* (1. / (65535. * 32768.))); 
#else
							*++fout = (((float)(((bp[c])<<8L)+((bp[c+1])<<16L)+((bp[c+2])<<24L)))
									* (1. / (65535. * 32768.))); 
#endif
						}
					} else if (sampsize==4) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (double)(BYTEORDER_SWAPW32(lp[c])) * (1./(65535. * 32768.)); 
						} 
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_FL32) {
					if (sampsize==4) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (float)(BYTEORDER_SWAPF32(fp[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_FL64) {
					if (sampsize==8) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (double)(BYTEORDER_SWAPF64(dp[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_MULAW) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (float)(sf_mulaw_decode(buf[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_ALAW) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (float)(sf_alaw_decode(buf[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_UINT) {
					//only wave less than 8bits are in unsigned format. perhaps support others later
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = ((float)(bp[c]) * (1./128.)) - 1.;
						} 
					} else goto zero;
			} else goto zero;

		} else {
			if (comptype == SF_SAMPTYPE_INT) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (float)(buf[c]) * (1./128.);
						} 
					} else if (sampsize==2) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = ((float)(sp[c]) * (1./32768.)); 
						}
					} else if (sampsize==3) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							c *= 3;
#if C74_LITTLE_ENDIAN
							lval = (bp[c] << 8L) + (bp[c+1] << 16L) + (bp[c+2] << 24L);
#else
							lval = (bp[c] << 24L) + (bp[c+1] << 16L) + (bp[c+2] << 8L);
#endif
							*++fout = (((float)lval) * (1. / (65535. * 32768.))); 
						}
					} else if (sampsize==4) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (double)(lp[c]) * (1./(65535. * 32768.));
						} 
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_FL32) {
					if (sampsize==4) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (float)(fp[c]); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_FL64) {
					if (sampsize==8) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (double)(dp[c]); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_MULAW) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (float)(sf_mulaw_decode(buf[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_ALAW) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = (float)(sf_alaw_decode(buf[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_UINT) {
					//only wave less than 8bits are in unsigned format. perhaps support others later
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO_NOINTERP;
							*++fout = ((float)(bp[c]) * (1./128.)) - 1.;
						} 
					} else goto zero;
			} else goto zero;
		}	
	} else { //interpolate
		if (swap) {
			if (comptype == SF_SAMPTYPE_INT) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.) 
								*++fout = ((double)(buf[c]) * (1./128.) * (1. - frak)) + ((double)(buf[d]) * (1./128.) * frak);
							else
								*++fout = (double)(buf[c]) * (1./128.);
						} 
					} else if (sampsize==2) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.) 
								*++fout = ((double)(BYTEORDER_SWAPW16(sp[c])) * (1./32768.) * (1. - frak)) + ((double)(BYTEORDER_SWAPW16(sp[d])) * (1./32768.) * frak); 
							else	
								*++fout = ((double)(BYTEORDER_SWAPW16(sp[c])) * (1./32768.)); 
						}
					} else if (sampsize==3) {
						while (--count){
							CONVERT_LOOP_MACRO;
							c *= 3;
							d *= 3;
							if (frak > 0.) {
#if C74_LITTLE_ENDIAN
								lval = ((bp[c])<<24L)+((bp[c+1])<<16L)+((bp[c+2])<<8L);
								lval2 = ((bp[d])<<24L)+((bp[d+1])<<16L)+((bp[d+2])<<8L);
#else
								lval = ((bp[c])<<8L)+((bp[c+1])<<16L)+((bp[c+2])<<24L);
								lval2 = ((bp[d])<<8L)+((bp[d+1])<<16L)+((bp[d+2])<<24L);
#endif
								*++fout = (((double)lval * (1. / (65535. * 32768.)) * (1. - frak))) + 
								(((double)lval2 * (1. / (65535. * 32768.)) * (frak))); 
							}
							else {
#if C74_LITTLE_ENDIAN
								lval = ((bp[c])<<24L)+((bp[c+1])<<16L)+((bp[c+2])<<8L);
#else
								lval = ((bp[c])<<8L)+((bp[c+1])<<16L)+((bp[c+2])<<24L);
#endif
								*++fout = ((double)lval) * (1. / (65535. * 32768.)); 
							}
						}
					} else if (sampsize==4) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.)
								*++fout = ((double)(BYTEORDER_SWAPW32(lp[c])) * (1./(65535. * 32768.)) * (1. - frak)) + ((double)(BYTEORDER_SWAPW32(lp[d])) * (1./(65535. * 32768.)) * frak);
							else	
								*++fout = (double)(BYTEORDER_SWAPW32(lp[c])) * (1./(65535. * 32768.)); 
						} 
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_FL32) {
					if (sampsize==4) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.)
								*++fout = ((double)(BYTEORDER_SWAPF32(fp[c])) * (1. - frak)) + ((double)(BYTEORDER_SWAPF32(fp[d])) * frak); 
							else
								*++fout = (double)(BYTEORDER_SWAPF32(fp[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_FL64) {
					if (sampsize==8) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.)
								*++fout = ((double)(BYTEORDER_SWAPF64(dp[c])) * (1. - frak)) + ((double)(BYTEORDER_SWAPF64(dp[d])) * frak); 
							else 
								*++fout = (double)(BYTEORDER_SWAPF64(dp[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_MULAW) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.)
								*++fout = ((double)(sf_mulaw_decode(buf[c])) * (1. - frak)) + ((double)(sf_mulaw_decode(buf[d])) * frak); 
							else
								*++fout = (double)(sf_mulaw_decode(buf[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_ALAW) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.)
								*++fout = ((double)(sf_alaw_decode(buf[c])) * (1. - frak)) + ((double)(sf_alaw_decode(buf[d])) * frak); 
							else 
								*++fout = (double)(sf_alaw_decode(buf[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_UINT) {
					//only wave less than 8bits are in unsigned format. perhaps support others later
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.) 
								*++fout = (((double)(bp[c]) * (1./128.) * (1. - frak)) + ((double)(bp[d]) * (1./128.) * frak)) - 1.;
							else
								*++fout = ((double)(bp[c]) * (1./128.)) - 1.;
						} 
					} else goto zero;
			} else goto zero;
		} else {
			if (comptype == SF_SAMPTYPE_INT) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.) 
								*++fout = ((double)(buf[c]) * (1./128.) * (1. - frak)) + ((double)(buf[d]) * (1./128.) * frak);
							else
								*++fout = (double)(buf[c]) * (1./128.);
						} 
					} else if (sampsize==2) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.) 
								*++fout = ((double)(sp[c]) * (1./32768.) * (1. - frak)) + ((double)(sp[d]) * (1./32768.) * frak); 
							else	
								*++fout = ((double)(sp[c]) * (1./32768.)); 
						}
					} else if (sampsize==3) {
						while (--count){
							CONVERT_LOOP_MACRO;
							c *= 3;
							d *= 3;
							if (frak > 0.) { 
#if C74_LITTLE_ENDIAN
								lval = ((bp[c])<<8L)+((bp[c+1])<<16L)+((bp[c+2])<<24L);
								lval2 = ((bp[d])<<8L)+((bp[d+1])<<16L)+((bp[d+2])<<24L);
#else
								lval = ((bp[c])<<24L)+((bp[c+1])<<16L)+((bp[c+2])<<8L);
								lval2 = ((bp[d])<<24L)+((bp[d+1])<<16L)+((bp[d+2])<<8L);
#endif
								*++fout = (((double)lval) * (1. / (65535. * 32768.)) * (1. - frak)) + 
									(((double)lval2) * (1. / (65535. * 32768.)) * (frak)); 
							}
							else {
#if C74_LITTLE_ENDIAN
								lval = ((bp[c])<<8L)+((bp[c+1])<<16L)+((bp[c+2])<<24L);
#else
								lval = ((bp[c])<<24L)+((bp[c+1])<<16L)+((bp[c+2])<<8L);
#endif
								*++fout = (((double)lval) * (1. / (65535. * 32768.))); 
							}
						}
					} else if (sampsize==4) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.)
								*++fout = ((double)(lp[c]) * (1./(65535. * 32768.)) * (1. - frak)) + ((double)(lp[d]) * (1./(65535. * 32768.)) * frak);
							else	
								*++fout = (double)(lp[c]) * (1./(65535. * 32768.));
						} 
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_FL32) {
					if (sampsize==4) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.)
								*++fout = ((double)(fp[c]) * (1. - frak)) + ((double)(fp[d]) * frak); 
							else
								*++fout = (double)(fp[c]); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_FL64) {
					if (sampsize==8) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.)
								*++fout = ((double)(dp[c]) * (1. - frak)) + ((double)(dp[d]) * frak); 
							else 
								*++fout = (double)(dp[c]); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_MULAW) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.)
								*++fout = ((double)(sf_mulaw_decode(buf[c])) * (1. - frak)) + ((double)(sf_mulaw_decode(buf[d])) * frak); 
							else
								*++fout = (double)(sf_mulaw_decode(buf[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_ALAW) {
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.)
								*++fout = ((double)(sf_alaw_decode(buf[c])) * (1. - frak)) + ((double)(sf_alaw_decode(buf[d])) * frak); 
							else 
								*++fout = (double)(sf_alaw_decode(buf[c])); 
						}
					} else goto zero;
			} else if (comptype == SF_SAMPTYPE_UINT) {
					//only wave less than 8bits are in unsigned format. perhaps support others later
					if (sampsize==1) {
						while (--count){
							CONVERT_LOOP_MACRO;
							if (frak > 0.) 
								*++fout = (((double)(bp[c]) * (1./128.) * (1. - frak)) + ((double)(bp[d]) * (1./128.) * frak)) - 1.;
							else
								*++fout = ((double)(bp[c]) * (1./128.)) - 1.;
						} 
					} else goto zero;
			} else goto zero;
		}
	}	    
    return;

zero:
	while (--count) *++fout = 0.;
	return;
}


long aq_float2samptype(float *in, t_aqueue *a, long n, long offset, long skip, long bytes, 
		unsigned long samptype, short byteswap, double resamp, double *current)
{
	char *p;
	unsigned char *bp;
	short *sp;
	long dbe;
	long *lp;
	float *fp;
	double *dp;
	double f;
	long samps=0;
	double fm,c,inc=0.;
	long m,idx;
	void *end,*start;
	
	start = (void *)(a->a_buf+offset);	
	end = (void *)(a->a_buf+(a->a_size-1));	
	fm=((double)n+*current)*resamp;
	samps=m=(long)fm;
	if (resamp){
		inc=1./resamp;
		c=(*current==0.)?-inc:-*current;
		*current=(fm-(double)m)*inc;
	} else {
		c=0;
	}
	++m;
		
	switch (bytes) {
		case 1:
			switch (samptype) {
				case SF_SAMPTYPE_INT:
					p = (char *)(a->a_buf+a->a_tail+offset);
					while (--m) {
						c += inc;
						idx = (long) c;
						f = in[idx];
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						*p = 127. * f;
						p += skip;
						if (p>(char*)end) p=start;
					}
					break;					
				case SF_SAMPTYPE_UINT: 			// Only needed for WAVE				
					bp = (unsigned char *)(a->a_buf+a->a_tail+offset);
					while (--m) {
						c += inc;
						idx = (long) c;
						f = in[idx];
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						*bp = 127. * (f + 1.);
						bp += skip;
						if (bp>(unsigned char *)end) bp=start;
					}
					break;
				case SF_SAMPTYPE_ALAW:		
					bp = (unsigned char *)(a->a_buf+a->a_tail+offset);
					while (--m) {
						c += inc;
						idx = (long) c;
						f = in[idx];
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						*bp = sf_alaw_encode(f);
						bp += skip;
						if (bp>(unsigned char*)end) bp=start;
					}
					break;
				case SF_SAMPTYPE_MULAW:			
					bp = (unsigned char *)(a->a_buf+a->a_tail+offset);
					while (--m) {
						c += inc;
						idx = (long) c;
						f = in[idx];
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						*bp = sf_mulaw_encode(f);
						bp += skip;
						if (bp>(unsigned char*)end) bp=start;
					}
					break;
				default:
					goto err;
			}			
			break;
		case 2:
			sp = (short *)(a->a_buf+a->a_tail+offset);
			switch (samptype) {
				case SF_SAMPTYPE_INT:
					if (byteswap) {
						while (--m) {
							c += inc;
							idx = (long) c;
							f = in[idx];
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							*sp = 32767. * f;
							*sp = BYTEORDER_SWAPW16(*sp);
							sp += skip;
							if (sp>(short*)end) sp=start;
						}
					} else {
						while (--m) {
							c += inc;
							idx = (long) c;
							f = in[idx];
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							*sp = 32767. * f;
							sp += skip;
							if (sp>(short*)end) sp=start;
						}
					}
					break;
				default:
					goto err;
			}	
			break;			
		case 3:
			bp = (unsigned char *)(a->a_buf+a->a_tail+offset);
			skip--;
			switch (samptype) {
				case SF_SAMPTYPE_INT:
#if C74_LITTLE_ENDIAN
					// rbs -- since we write out below ina platform independent way
					// we actually want the reverse logic. 
					byteswap = !byteswap;
#endif
					if (byteswap){
						while (--m) {
							c += inc;
							idx = (long) c;
							f = in[idx];
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							dbe = 65535. * 32767. * f;
							*bp++ = (dbe>>8);
							*bp++ = (dbe>>16);
							*bp++ = (dbe>>24);
							bp += skip * 3;	// actually nchans - 1
							if (bp>(unsigned char*)end) bp=start;
						}
					} else {
						while (--m) {
							c += inc;
							idx = (long) c;
							f = in[idx];
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							dbe = 65535. * 32767. * f;
							*bp++ = (dbe>>24);
							*bp++ = (dbe>>16);
							*bp++ = (dbe>>8);
							bp += skip * 3;	// actually nchans - 1
							if (bp>(unsigned char*)end) bp=start;
						}
					}
					break;
				default:
					goto err;
			}	
			break;
		case 4:
			switch (samptype) {
				case SF_SAMPTYPE_INT:
					lp = (long *)(a->a_buf+a->a_tail+offset);
					if (byteswap) {
						while (--m) {
							c += inc;
							idx = (long) c;
							f = in[idx];
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							*lp = 65535. * 32767. * f;
							*lp = BYTEORDER_SWAPW32(*lp);
							lp += skip;
							if (lp>(long*)end) lp=start;
						}
					} else {
						while (--m) {
							c += inc;
							idx = (long) c;
							f = in[idx];
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							*lp = 65535. * 32767. * f;
							lp += skip;
							if (lp>(long*)end) lp=start;
						}
					}
					break;
				case SF_SAMPTYPE_FL32:
					fp = (float *)(a->a_buf+a->a_tail+offset);
					if (byteswap) {
						while (--m) {
							c += inc;
							idx = (long) c;
							*fp = BYTEORDER_SWAPF32(in[idx]); //no clip
							fp += skip;
							if (fp>(float*)end) fp=start;
						}
					} else {
						while (--m) {
							c += inc;
							idx = (long) c;
							*fp = in[idx]; //no clip
							fp += skip;
							if (fp>(float*)end) fp=start;
						}
					}
					break;
				default:
					goto err;
			}		
			break;
		case 8:
			switch (samptype) {
				case SF_SAMPTYPE_FL64:
					dp = (double *)(a->a_buf+a->a_tail+offset);
					if (byteswap) {
						while (--m) {
							c += inc;
							idx = (long) c;
							*dp = BYTEORDER_SWAPF64((double)(in[idx])); //no clip
							dp += skip;
							if (dp>(double *)end) dp=start;	
						}
					} else {
						while (--m) {
							c += inc;
							idx = (long) c;
							*dp = in[idx]; //no clip
							dp += skip;
							if (dp>(double *)end) dp=start;
						}
					}
				default:
					goto err;
			}
			break;
		default:
			goto err;
	}
err:
	return samps;
}

long aq_float2samptype_interp(float *in, t_aqueue *a, long n, long offset, long skip, long bytes, 
		unsigned long samptype, short byteswap, double resamp, double *current, double last)
{
	char *p;
	unsigned char *bp;
	short *sp;
	long dbe;
	long *lp;
	float *fp;
	double *dp;
	double f,g;
	long samps=0;
	double fm,c,inc=0.;
	long m,idx;
	void *end,*start;
	double frak;
	
	start = (void *)(a->a_buf+offset);	
	end = (void *)(a->a_buf+(a->a_size-1));	
	fm=((double)n+*current)*resamp;
	samps=m=(long)fm;
	if (resamp){
		inc=1./resamp;
		c=(*current==0.)?-inc:-*current; 
		*current=(fm-(double)m)*inc;
	} else {
		c=0;
	}
	++m;
	c -= 1.;
	
	switch (bytes) {
		case 1:
			switch (samptype) {
				case SF_SAMPTYPE_INT:
					p = (char *)(a->a_buf+a->a_tail+offset);
					while (--m) {
						c += inc;
						if (c<0.) {
							frak = (c-(long)c);
							frak = (frak==0.)?0.:1.+frak;
							f = last;
							g = in[0];
						} else { 
							idx = (long) c;
							frak = c-idx;
							f = in[idx];
							g = in[idx+1];
						}
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						g = (g>1.)?1.:((g<-1.)?-1.:g);
						*p = 127.*(f+((g-f)*frak));
						p += skip;
						if (p>(char*)end) p=start;
					}
					break;					
				case SF_SAMPTYPE_UINT: 			// Only needed for WAVE				
					bp = (unsigned char *)(a->a_buf+a->a_tail+offset);
					while (--m) {
						c += inc;						
						if (c<0.) {
							frak = (c-(long)c);
							frak = (frak==0.)?0.:1.+frak;
							f = last;
							g = in[0];
						} else { 
							idx = (long) c;
							frak = c-idx;
							f = in[idx];
							g = in[idx+1];
						}
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						g = (g>1.)?1.:((g<-1.)?-1.:g);
						*bp = 127.*((f+((g-f)*frak))+1.);
						bp += skip;
						if (bp>(unsigned char*)end) bp=start;
					}
					break;
				case SF_SAMPTYPE_ALAW:		
					bp = (unsigned char *)(a->a_buf+a->a_tail+offset);
					while (--m) {
						c += inc;
						if (c<0.) {
							frak = (c-(long)c);
							frak = (frak==0.)?0.:1.+frak;
							f = last;
							g = in[0];
						} else { 
							idx = (long) c;
							frak = c-idx;
							f = in[idx];
							g = in[idx+1];
						}
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						g = (g>1.)?1.:((g<-1.)?-1.:g);
						*bp = sf_alaw_encode(f+((g-f)*frak));
						bp += skip;
						if (bp>(unsigned char*)end) bp=start;
					}
					break;
				case SF_SAMPTYPE_MULAW:			
					bp = (unsigned char *)(a->a_buf+a->a_tail+offset);
					while (--m) {
						c += inc;
						if (c<0.) {
							frak = (c-(long)c);
							frak = (frak==0.)?0.:1.+frak;
							f = last;
							g = in[0];
						} else { 
							idx = (long) c;
							frak = c-idx;
							f = in[idx];
							g = in[idx+1];
						}
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						g = (g>1.)?1.:((g<-1.)?-1.:g);
						*bp = sf_mulaw_encode(f+((g-f)*frak));
						bp += skip;
						if (bp>(unsigned char*)end) bp=start;
					}
					break;
				default:
					goto err;
			}			
			break;
		case 2:
			sp = (short *)(a->a_buf+a->a_tail+offset);
			switch (samptype) {
				case SF_SAMPTYPE_INT:
					if (byteswap) {
						while (--m) {
							c += inc;
							if (c<0.) {
								frak = (c-(long)c);
								frak = (frak==0.)?0.:1.+frak;
								f = last;
								g = in[0];
							} else { 
								idx = (long) c;
								frak = c-idx;
								f = in[idx];
								g = in[idx+1];
							}
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							g = (g>1.)?1.:((g<-1.)?-1.:g);
							*sp = 32767.*(f+((g-f)*frak));
							*sp = BYTEORDER_SWAPW16(*sp);
							sp += skip;
							if (sp>(short*)end) sp=start;
						}
					} else {
						while (--m) {
							c += inc;
							if (c<0.) {
								frak = (c-(long)c);
								frak = (frak==0.)?0.:1.+frak;
								f = last;
								g = in[0];
							} else { 
								idx = (long) c;
								frak = c-idx;
								f = in[idx];
								g = in[idx+1];
							}
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							g = (g>1.)?1.:((g<-1.)?-1.:g);
							*sp = 32767.*(f+((g-f)*frak));
							sp += skip;
							if (sp>(short*)end) sp=start;
							
						}
					}
					break;
				default:
					goto err;
			}	
			break;			
		case 3:
			bp = (unsigned char *)(a->a_buf+a->a_tail+offset);
			skip--;
			switch (samptype) {
				case SF_SAMPTYPE_INT:
#if C74_LITTLE_ENDIAN
					// rbs -- since we write out below ina platform independent way
					// we actually want the reverse logic. 
					byteswap = !byteswap;
#endif
					if (byteswap){
						while (--m) {
							c += inc;
							if (c<0.) {
								frak = (c-(long)c);
								frak = (frak==0.)?0.:1.+frak;
								f = last;
								g = in[0];
							} else { 
								idx = (long) c;
								frak = c-idx;
								f = in[idx];
								g = in[idx+1];
							}
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							g = (g>1.)?1.:((g<-1.)?-1.:g);
							dbe = 65535.*32767.*(f+((g-f)*frak));
							*bp++ = (dbe>>8);
							*bp++ = (dbe>>16);
							*bp++ = (dbe>>24);
							bp += skip * 3;	// actually nchans - 1
							if (bp>(unsigned char*)end) bp=start;
						}
					} else {
						while (--m) {
							c += inc;
							if (c<0.) {
								frak = (c-(long)c);
								frak = (frak==0.)?0.:1.+frak;
								f = last;
								g = in[0];
							} else { 
								idx = (long) c;
								frak = c-idx;
								f = in[idx];
								g = in[idx+1];
							}
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							g = (g>1.)?1.:((g<-1.)?-1.:g);
							dbe = 65535.*32767.*(f+((g-f)*frak));
							*bp++ = (dbe>>24);
							*bp++ = (dbe>>16);
							*bp++ = (dbe>>8);
							bp += skip * 3;	// actually nchans - 1
							if (bp>(unsigned char*)end) bp=start;
						}
					}
					break;
				default:
					goto err;
			}	
			break;
		case 4:
			switch (samptype) {
				case SF_SAMPTYPE_INT:
					lp = (long *)(a->a_buf+a->a_tail+offset);
					if (byteswap) {
						while (--m) {
							c += inc;
							if (c<0.) {
								frak = (c-(long)c);
								frak = (frak==0.)?0.:1.+frak;
								f = last;
								g = in[0];
							} else { 
								idx = (long) c;
								frak = c-idx;
								f = in[idx];
								g = in[idx+1];
							}
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							g = (g>1.)?1.:((g<-1.)?-1.:g);
							*lp = 65535.*32767.*(f+((g-f)*frak));
							*lp = BYTEORDER_SWAPW32(*lp);
							lp += skip;
							if (lp>(long*)end) lp=start;
						}
					} else {
						while (--m) {
							c += inc;
							if (c<0.) {
								frak = (c-(long)c);
								frak = (frak==0.)?0.:1.+frak;
								f = last;
								g = in[0];
							} else { 
								idx = (long) c;
								frak = c-idx;
								f = in[idx];
								g = in[idx+1];
							}
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							g = (g>1.)?1.:((g<-1.)?-1.:g);
							*lp = 65535.*32767.*(f+((g-f)*frak));
							lp += skip;
							if (lp>(long *)end) lp=start;
						}
					}
					break;
				case SF_SAMPTYPE_FL32:
					fp = (float *)(a->a_buf+a->a_tail+offset);
					if (byteswap) {
						while (--m) {
							c += inc;
							if (c<0.) {
								frak = (c-(long)c);
								frak = (frak==0.)?0.:1.+frak;
								f = last;
								g = in[0];
							} else { 
								idx = (long) c;
								frak = c-idx;
								f = in[idx];
								g = in[idx+1];
							}
							*fp = BYTEORDER_SWAPF32(f+((g-f)*frak)); //no clip
							fp += skip;
							if (fp>(float *)end) fp=start;
						}
					} else {
						while (--m) {
							c += inc;
							if (c<0.) {
								frak = (c-(long)c);
								frak = (frak==0.)?0.:1.+frak;
								f = last;
								g = in[0];
							} else { 
								idx = (long) c;
								frak = c-idx;
								f = in[idx];
								g = in[idx+1];
							}
							*fp = f+((g-f)*frak); //no clip
							fp += skip;
							if (fp>(float *)end) fp=start;
						}
					}
					break;
				default:
					goto err;
			}		
			break;
		case 8:
			switch (samptype) {
				case SF_SAMPTYPE_FL64:
					dp = (double *)(a->a_buf+a->a_tail+offset);
					if (byteswap) {
						while (--m) {
							c += inc;
							if (c<0.) {
								frak = (c-(long)c);
								frak = (frak==0.)?0.:1.+frak;
								f = last;
								g = in[0];
							} else { 
								idx = (long) c;
								frak = c-idx;
								f = in[idx];
								g = in[idx+1];
							}
							*dp = BYTEORDER_SWAPF64((double)(f+((g-f)*frak))); //no clip
							dp += skip;
							if (dp>(double *)end) dp=start;	
						}
					} else {
						while (--m) {
							c += inc;
							if (c<0.) {
								frak = (c-(long)c);
								frak = (frak==0.)?0.:1.+frak;
								f = last;
								g = in[0];
							} else { 
								idx = (long) c;
								frak = c-idx;
								f = in[idx];
								g = in[idx+1];
							}
							*dp = (double)(f+((g-f)*frak)); //no clip
							dp += skip;
							if (dp>(double *)end) dp=start;
						}
					}
				default:
					goto err;
			}
			break;
		default:
			goto err;
	}
err:
	return samps;
}

void aq_copyfrom(t_aqueue *a, Ptr src, long count, long pos)
{
	a->a_head = a->a_tail = pos;
	sysmem_copyptr(src,a->a_buf + a->a_tail, count); 
	aq_incrtail(a,count); //this won't work in reverse...
}

void aq_copyfrom_rev(t_aqueue *a, Ptr src, long count, long pos)
{
	a->a_head = a->a_tailrev = pos+count;
	aq_incrtail_rev(a,count);
	sysmem_copyptr(src,a->a_buf + a->a_tailrev, count);
}

