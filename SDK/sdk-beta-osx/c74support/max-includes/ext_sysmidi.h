// sysmidi.h

typedef struct _iq {
	long q_head;
	long q_tail;
	long q_nitems;
	long q_size;
	long q_busy;
	long q_busy2;
	Byte *q_data;
	Byte q_psysex;
	Byte q_gsysex;
} t_iq;

typedef void (*sendbytesmethod)(long port, short number, long b1, long b2, long b3);

typedef struct _midiportinfo {
	t_symbol *p_name;
	long p_refnum;
	short p_id;
	short p_abbrev;
	short p_chanoffset;
	char p_enabled;
} t_midiportinfo;

typedef struct _sysmididriver {
	t_object d_ob;
	t_symbol *d_name;
	char d_available;	// positive response on informational instantiation
	char d_hastimer;
	t_midiportinfo *d_inports;
	t_midiportinfo *d_outports;
	t_iq d_q;
	
	// driver-specific info follows
} t_sysmididriver;

typedef struct _mididriverinfo {
	t_symbol *i_name;
	char i_doestiming;
	struct _mididriverinfo *i_next;
} t_mididriverinfo;

void sysmidi_enqpacket(t_iq *qp, long port, Byte *data, double ts, short len, short contFlags);
void sysmidi_changed(t_sysmididriver *x, t_midiportinfo *in_new, t_midiportinfo *out_new,
	short *inmap, short *outmap, short newincount, short newoutcount);


