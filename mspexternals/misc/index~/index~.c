#include "ext.h"
#include "z_dsp.h"
#include "buffer.h"

void *index_class;

typedef struct _index
{
    t_pxobject l_obj;
    t_symbol *l_sym;
    t_buffer *l_buf;
    long l_chan;
} t_index;

long Constrain(long v, long lo, long hi);
t_int *index_perform(t_int *w);
void index_dsp(t_index *x, t_signal **sp);
void index_set(t_index *x, t_symbol *s);
void *index_new(t_symbol *s, long chan);
void index_in1(t_index *x, long n);
void index_assist(t_index *x, void *b, long m, long a, char *s);
void index_dblclick(t_index *x);

t_symbol *ps_buffer;

void main(void)
{
	setup(&index_class, index_new, (method)dsp_free, (short)sizeof(t_index), 0L, 
		A_SYM, A_DEFLONG, 0);
	addmess((method)index_dsp, "dsp", A_CANT, 0);
	addmess((method)index_set, "set", A_SYM, 0);
	addinx((method)index_in1,1);
	addmess((method)index_assist, "assist", A_CANT, 0);
	addmess((method)index_dblclick, "dblclick", A_CANT, 0);
	dsp_initclass();
	ps_buffer = gensym("buffer~");
	rescopy('STR#',3294);
}

long Constrain(long v, long lo, long hi)
{
	if (v < lo)
		return lo;
	else if (v > hi)
		return hi;
	else
		return v;
}

t_int *index_perform(t_int *w)
{
    t_index *x = (t_index *)(w[1]);
    t_float *in = (t_float *)(w[2]);
    t_float *out = (t_float *)(w[3]);
    int n = (int)(w[4]);
	t_buffer *b = x->l_buf;
	float *tab;
	float f;
	long index,chan,frames,nc;
	
	if (x->l_obj.z_disabled)
		goto out;
	if (!b)
		goto zero;
	if (!b->b_valid)
		goto zero;
		
	tab = b->b_samples;
	chan = x->l_chan;
	frames = b->b_frames;
	nc = b->b_nchans;
	while (--n) {
		f = *++in + 0.5;
		index = f;
		if (index < 0)
			index = 0;
		else if (index >= frames)
			index = frames - 1;
		if (nc > 1)
			index = index * nc + chan;
		*++out = tab[index];
	}
	return (w+5);
zero:
	while (--n) *++out = 0.;
out:
	return (w+5);
}

void index_set(t_index *x, t_symbol *s)
{
	t_buffer *b;
	
	x->l_sym = s;
	if ((b = (t_buffer *)(s->s_thing)) && ob_sym(b) == ps_buffer) {
		x->l_buf = b;
	} else {
		error("index~: no buffer~ %s", s->s_name);
		x->l_buf = 0;
	}
}

void index_in1(t_index *x, long n)
{
	if (n)
		x->l_chan = Constrain(n,1,4) - 1;
	else
		x->l_chan = 0;
}

void index_dsp(t_index *x, t_signal **sp)
{
    index_set(x,x->l_sym);
    dsp_add(index_perform, 4, x, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[0]->s_n+1);
}

void index_dblclick(t_index *x)
{
	t_buffer *b;
	
	if ((b = (t_buffer *)(x->l_sym->s_thing)) && ob_sym(b) == ps_buffer)
		mess0(b,gensym("dblclick"));
}

void index_assist(t_index *x, void *b, long m, long a, char *s)
{
	assist_string(3294,m,a,1,3,s);
}

void *index_new(t_symbol *s, long chan)
{
	t_index *x = (t_index *)newobject(index_class);
	dsp_setup((t_pxobject *)x, 1);
	intin((t_object *)x,1);
	outlet_new((t_object *)x, "signal");
	x->l_sym = s;
	index_in1(x,chan);
	return (x);
}

