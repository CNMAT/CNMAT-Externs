	/* linkThing.o.h */
	
typedef struct linkThing {
	struct	tinyobject l_ob;
	void	*l_thing;
	void	*l_clock;
	void	*l_who;
	void	*l_arg;
#ifdef __MOTO__
	void	*(*l_fn)();
#else
	void	*(*l_fn)(...);
#endif __MOTO__
	struct	linkThing *l_prev;
	struct	linkThing *l_next;
	long	l_logical;				/* this is used as a tip-off that */
	short	l_flags;
} linkThing, *LinkThing;

extern	Symbol s_linkThing;

/* flags for linkthing */

#define L_DIEONSTOP 1
#define L_MUSTHAPPEN 2
