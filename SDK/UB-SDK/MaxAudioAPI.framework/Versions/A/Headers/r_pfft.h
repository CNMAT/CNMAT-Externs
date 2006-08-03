// public fftpatcher struct

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif


typedef struct _pfftpub
{
	t_pxobject x_obj;
	struct t_patcher *x_parent;	// parent patcher
	struct t_patcher *x_patcher;// patcher loaded
	struct _dspchain *x_chain;	// dsp chain within pfft
	
	long x_fftsize;				// fft frame size
	long x_ffthop;				// hop between  fft frames
	long x_fftoffset;			// n samples offset before fft is started
	long x_fftindex;			// current index into fft frame 
	int x_fullspect;			// process half-spectrum (0) or full mirrored spectrum (1)?
	
} t_pfftpub;

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

