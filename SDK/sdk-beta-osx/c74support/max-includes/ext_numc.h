/* ext_numc.h -- Numerical include file */

/* flag constants */

#define N_HILITE 1
#define N_RIGHT 2
#define N_LEFT  4
#define N_FRAME 8
#define N_CLIP 16
#define N_ENDTRACK 32
#define N_BOLD 64
#define N_FREE 128

/* data structure */

struct num {
	struct tinyobject n_ob;
	long n_max,n_min,n_val;
	void *(*n_draw)();
	void *(*n_track)();
	void *(*n_inc)();
	Rect n_outline;
	Point n_start;
	int n_hilite;
	int n_font,n_fsize;
	long n_flags;
	void *n_arg;
	GrafPtr n_port;
};

typedef struct num Numerical;

