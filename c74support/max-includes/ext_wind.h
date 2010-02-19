/*************************/
/* Copyright 1988 IRCAM. */
/*************************/
#ifndef _EXT_WIND_H_
#define _EXT_WIND_H_

#ifdef __cplusplus
extern "C" {
#endif


/** Returned values from wind_advise()
	@ingroup misc
*/
typedef enum {
	aaYes =	1,		///< Yes button was choosen
	aaNo,			///< No button was choosen
	aaCancel		///< Cancel button was choosen
} e_max_wind_advise_result;


/**	Throw a dialog which may have text and up to three buttons.  
	For example, this can be used to ask "Save changes before..." 
	@ingroup	misc
	@param	w	The window with which this dialog is associated.
	@param	s	A string with any sprintf()-like formatting to be displayed.
	@param	...	Any variables that should be substituted in the string defined by s.
	@return		One of the values defined in #e_max_wind_advise_result, depending on what the user selected.
*/
short wind_advise(t_object *w, char *s, ...);


/**	Change the cursor.
	@ingroup		misc

	@param	which	One of the following predefined cursors:
	@code
	#define C_ARROW		1
	#define C_WATCH		2
	#define C_IBEAM		3
	#define C_HAND		4
	#define	C_CROSS		5
	#define C_PENCIL	6
	#define	C_GROW		8
	@endcode

	@remark		wind_setcursor() keeps track of what the cursor was previously set 
				to, so if something else has changed the cursor, you may not see a new 
				cursor if you set it to the previous argument to wind_setcursor().
				
				The solution is to call wind_setcursor(0) before calling it with the 
				desired cursor constant. Use wind_setcursor(-1) to tell Max you’ll set 
				the cursor to your own cursor directly.
*/
void wind_setcursor(short which);


#ifdef __cplusplus
}
#endif

#endif // _EXT_WIND_H_
