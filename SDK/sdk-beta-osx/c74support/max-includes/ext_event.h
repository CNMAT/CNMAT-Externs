typedef struct event {
	Object e_obj;
	struct smallbox *e_box;	/* Pointer to Smallbox (or Box) that holds the rectangle */
	struct event *e_upd;	/* Update list link (used internally for spooling events) */
	Rect *e_rect;			/* Pointer to rectangle within e_box */
	struct event *e_next;	/* Linked list of Events in the Timeline Track */
	struct tlTrack *e_track;	/* Pointer to owning Track */
	struct oList *e_assoc;	/* List of associated objects (internal use) */
	Object *e_o;			/* Pointer to object in unitary receiver case */
	Symbol *e_label;		/* Descriptive text for Event locate pop-up menu */
	long e_start;			/* Event start time (in milliseconds) */
	long e_duration;		/* Event duration (in milliseconds) */
	Symbol *e_dataType;		/* Data type of the message */
	Symbol *e_message;		/* Message selector that is sent */
	Symbol *e_editor;		/* Editor name */
	struct editor *e_edit;	/* Internal information about the Editor */
	void *e_saveThing;		/* Internal temporary variable */
	void *e_thing;			/* Internal unused variable */
	short e_wantOffset;		/* If the track containing the editor has been collapsed */ 
	Boolean e_active;		/* Currently unused */
	Boolean e_preview;		/* Should be non-zero, not currently used */
	Boolean e_constantWidth;	/* If this flag is non-zero, the Events duration is */
	Boolean e_editable;		/* Flag is non-zero if clicking on the Event modifies its */
	Boolean e_smallbox;		/* Flag is non-zero if the Event uses the Smallbox data */
} event, *Event;

#ifndef F_GROWY
#define F_GROWY			2		/* can grow in y direction by dragging */
#define F_GROWBOTH		32		/* can grow independently in both x and y */
#endif
#define F_CONSTANTWIDTH 16		/* constant width */
