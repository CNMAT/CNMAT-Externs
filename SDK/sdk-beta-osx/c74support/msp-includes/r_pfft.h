// public fftpatcher struct

typedef struct _pfftpub
{
	t_pxobject x_obj;
	struct t_patcher *x_parent;	// parent patcher
	struct t_patcher *x_patcher;// patcher loaded
	struct _dspchain *x_chain;	// dsp chain within pfft
	
	long x_fftsize;				// fft frame size
	long x_ffthop;				// hop between  fft frames
	long x_fftoffset;			// n samples offset before fft is started
	int x_fullspect;			// process half-spectrum (0) or full mirrored spectrum (1)?
	
} t_pfftpub;