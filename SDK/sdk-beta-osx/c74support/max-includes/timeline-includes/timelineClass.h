typedef	struct oList {
	Object	*obj;
	messfn	start;
	messfn	end;
	struct	oList *next;
} oList, *OList;

typedef struct editor {
	Symbol	*ed_name;							/* editor name */
	void	*(*ed_newFN)(Symbol*,int,Atom*);		/* new function for editor */
	void	*(*ed_menuFN)(Symbol*,Symbol*,void*,void*,long,Point,Symbol*);/* menu function for editor */
	void	*(*ed_updateFN)(struct event *,Rect*,Boolean);		/* update function for editor */
	struct	dataType *ed_dataType;				/* associated data type */
	struct	editor *ed_next;					/* list of editors */
} editor, *Editor;

typedef struct messageList {				/* objects associated with the track */
	OList	ml_obj;
	Symbol	*ml_objName;
	Symbol	*ml_name;					/* the message to send */
	struct	dataType *ml_type;			/* kind of datatype (determines what editors can be used) */
	struct	messageList *ml_next;
} messageList, *MessageList;

typedef	struct dataType {
	Symbol	*dt_name;			/* data type name */
	long	dt_editCount;		/* number of editors for this data type */
	Editor	dt_edit;			/* list of available editors */
	struct	dataType *dt_next;	/* linked list of available data types for a track */
} dataType, *DataType;