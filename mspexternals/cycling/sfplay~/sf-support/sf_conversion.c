#include "ext.h"
#include "sf2.h"

static unsigned char g711NumBits[] = {
   0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
   6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
};

double sf_mulaw_decode(unsigned char ulaw) 
{
	unsigned char exponent,mantissa;
	long adjusted;
	
	ulaw = ~ulaw;
	exponent = (ulaw>>4)&0x07;
	mantissa = (ulaw&0x0f)+16;
	adjusted = ((long)mantissa<<(long)(exponent+3))-128-4;
	if (adjusted<0) adjusted=0;
	//adjusted = ((long)mantissa<<(long)(exponent+3)); //well maybe we should be subtracting 128 and 4 from this amount, but we have wrap around problems
	return (ulaw&0x80) ? ((double)adjusted*(1./32768.)) : ((double)adjusted*(-1./32768.));	
}

double sf_alaw_decode(unsigned char alaw) 
{
	unsigned char exponent,mantissa;
	unsigned long adjusted;
	
	alaw ^= 0x55;
	exponent = (alaw>>4)&0x07;
	mantissa = (alaw&0x0f)+(exponent?16:0);
	adjusted = ((long)mantissa<<(long)(exponent+3));  //was +4 but it seems too loud! 
	return (alaw&0x80) ? ((double)adjusted*(1./32768.)) : ((double)adjusted*(-1./32768.));	
}

unsigned char sf_mulaw_encode(double f) 
{
	unsigned char sign,exponent,mantissa;
	long adjusted;
	
	f = (f>1.)?1.:((f<-1.)?-1.:f); //clip to -1/1
	adjusted = f*(32767.);
	sign = (adjusted<0)?0:0x80;
	if (adjusted<0) adjusted=-adjusted;
	adjusted += 128L+4L;
	if (adjusted > 32767) adjusted = 32767;
	exponent = g711NumBits[(adjusted>>7)&0xFF] - 1;
	mantissa = (adjusted >> (exponent + 3L)) & 0xFL;
	return ~(sign | (exponent << 4) | mantissa);
}

unsigned char sf_alaw_encode(double f) 
{
	unsigned char sign,exponent,mantissa;
	long adjusted;
	
	//something about this algorithm seems wrong
	f = (f>1.)?1.:((f<-1.)?-1.:f); //clip to -1/1
	adjusted = f*(32767.);
	sign = (adjusted<0)?0:0x80;
	if (adjusted<0) adjusted=-adjusted;
	adjusted += 8L; 
	if (adjusted > 32767) adjusted = 32767;
	exponent = g711NumBits[(adjusted>>8L)&0x7F];
	mantissa = (adjusted >> (exponent + 4L)) & 0xFL;
	return sign | (((exponent << 4) | mantissa) ^ 0x55);
}


long sf_float2samptype(float *in, void *out, long n, long skip, long bytes, 
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
					p = (char *)out;
					while (--m) {
						c += inc;
						idx = (long) c;
						f = in[idx];
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						*p = 127. * f;
						p += skip;
					}
					break;					
				case SF_SAMPTYPE_UINT: 			// Only needed for WAVE				
					bp = (unsigned char *)out;
					while (--m) {
						c += inc;
						idx = (long) c;
						f = in[idx];
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						*bp = 127. * (f + 1.);
						bp += skip;
					}
					break;
				case SF_SAMPTYPE_ALAW:		
					bp = (unsigned char *)out;
					while (--m) {
						c += inc;
						idx = (long) c;
						f = in[idx];
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						*bp = sf_alaw_encode(f);
						bp += skip;
					}
					break;
				case SF_SAMPTYPE_MULAW:			
					bp = (unsigned char *)out;
					while (--m) {
						c += inc;
						idx = (long) c;
						f = in[idx];
						f = (f>1.)?1.:((f<-1.)?-1.:f);
						*bp = sf_mulaw_encode(f);
						bp += skip;
					}
					break;
				default:
					goto err;
			}			
			break;
		case 2:
			sp = (short *)out;
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
						}
					} else {
						while (--m) {
							c += inc;
							idx = (long) c;
							f = in[idx];
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							*sp = 32767. * f;
							sp += skip;
							
						}
					}
					break;
				default:
					goto err;
			}	
			break;			
		case 3:
			bp = (unsigned char *)out;
			skip--;
			switch (samptype) {
				case SF_SAMPTYPE_INT:
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
					lp = (long *)out;
					if (byteswap) {
						while (--m) {
							c += inc;
							idx = (long) c;
							f = in[idx];
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							*lp = 65535. * 32767. * f;
							*lp = BYTEORDER_SWAPW32(*lp);
							lp += skip;
						}
					} else {
						while (--m) {
							c += inc;
							idx = (long) c;
							f = in[idx];
							f = (f>1.)?1.:((f<-1.)?-1.:f);
							*lp = 65535. * 32767. * f;
							lp += skip;
						}
					}
					break;
				case SF_SAMPTYPE_FL32:
					fp = (float *)out;
					if (byteswap) {
						while (--m) {
							c += inc;
							idx = (long) c;
							*fp = BYTEORDER_SWAPF32(in[idx]); //no clip
							fp += skip;
						}
					} else {
						while (--m) {
							c += inc;
							idx = (long) c;
							*fp = in[idx]; //no clip
							fp += skip;
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
					dp = (double *)out;
					if (byteswap) {
						while (--m) {
							c += inc;
							idx = (long) c;
							*dp = BYTEORDER_SWAPF64((double)(in[idx])); //no clip
							dp += skip;
						}
					} else {
						while (--m) {
							c += inc;
							idx = (long) c;
							*dp = in[idx]; //no clip
							dp += skip;
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

long sf_float2samptype_interp(float *in, void *out, long n, long offset, long skip, long bytes, 
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
	double frak;
	
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
					p = (char *)out;
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
					}
					break;					
				case SF_SAMPTYPE_UINT: 			// Only needed for WAVE				
					bp = (unsigned char *)out;
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
					}
					break;
				case SF_SAMPTYPE_ALAW:		
					bp = (unsigned char *)out;
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
					}
					break;
				case SF_SAMPTYPE_MULAW:			
					bp = (unsigned char *)out;
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
					}
					break;
				default:
					goto err;
			}			
			break;
		case 2:
			sp = (short *)out;
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
						}
					}
					break;
				default:
					goto err;
			}	
			break;			
		case 3:
			bp = (unsigned char *)out;
			skip--;
			switch (samptype) {
				case SF_SAMPTYPE_INT:
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
					lp = (long *)out;
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
						}
					}
					break;
				case SF_SAMPTYPE_FL32:
					fp = (float *)out;
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
					dp = (double *)out;
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

