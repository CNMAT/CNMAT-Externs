typedef struct _sndfileinfo
{
	long sr;
	long length;
	long sampsize; // in bytes
	unsigned long filetype;
	unsigned long comptype;
	unsigned long format;
	char name[128];
	long byteswap;
	long nchans;
	long frames;
	void *inst;
	void *markers;
	long framesize;
	long offset;
} t_sndfileinfo;

