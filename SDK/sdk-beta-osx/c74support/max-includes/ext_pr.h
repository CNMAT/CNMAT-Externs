typedef struct {
	struct object p_ob;
	void *p_self;
	int p_kind;
	int p_misc;
	void *p_etc;
} Pr;

#define TypePrClass		1
#define TypePrArg		2
#define TypePrProp		3
#define TypePrSuper		4
#define TypePrSelf		5
#define TypePrOut		6

enum {
	i_getprop  = 0,
	i_putprop,
	i_meth,
	i_supermeth,
	i_output,
	i_outprop,
	i_addmeth,
	i_prversion
};

#define getprop(p,s,a,v) ((void *)((*(PNS[i_getprop]))(p,gensym(s),a,v)))
#define putprop(p,s,a,v) ((void *)((*(PNS[i_putprop]))(p,gensym(s),a,v)))
#define meth(p,s,a,v,ra,rv) ((void *)((*(PNS[i_meth]))(p,gensym(s),a,v,ra,rv)))
#define supermeth(p,s,a,v,ra,rv) ((void *)((*(PNS[i_supermeth]))(p,gensym(s),a,v,ra,rv)))
#define output(p,s,a,v,t) ((void *)((*(PNS[i_output]))(p,gensym(s),a,v,t)))
#define outprop(p,s) ((void *)((*(PNS[i_outprop]))(p,gensym(s))))
#define addmeth(p,f,s) ((void *)((*(PNS[i_addmeth]))(p,f,s)))
#define prversion() ((long)((*(PNS[i_prversion]))()))

/* future services: get property names, method names, class name, etc. */