/*************************/

/* Copyright 1988 IRCAM. */

/*************************/



#ifndef _EDIT_H_

#define _EDIT_H_



#ifdef MAC_VERSION

typedef struct ed

{

	struct object	e_ob;

	TEHandle		e_teh;

	int				e_nlines;

	Point			e_origin;

	struct wind		*e_wind;

	ProcPtr 		e_docClik;

	struct object	*e_assoc;	/* <z1> associated object */

} Ed, t_ed;

#endif MAC_VERSION



#ifdef WIN_VERSION

typedef struct ed

{

	struct object	e_ob;

	void			*e_teh;		// will be 0

	short			e_nlines;

	Point			e_origin;	// not used

	struct wind		*e_wind;

	HWND			e_docClick;

	struct object	*e_assoc;	/* <z1> associated object */

	WNDPROC			e_wproc;

	HWND			e_sbhorz;

	HWND			e_sbvert;



} t_ed;

#endif WIN_VERSION



#endif _EDIT_H_



