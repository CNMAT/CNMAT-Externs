	/* timelineAction.h */

typedef	struct action {
	struct	action *ac_next;
	void	*(*ac_icon)(Rect*);		/* icon draw procedure */
	void	*ac_class;				/* the actionClass object */
	Symbol	*ac_name;				/* name of class */
	short	ac_vol;					/* volume where patch or code resource is located */
	short	ac_type;				/* code resource or max patch? */
	Boolean	ac_auto;				/* automatically loaded at beginning? */
} action, *Action;

