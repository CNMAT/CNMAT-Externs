/*
 *  ext_time.h
 *
 *  Copyright 2008 Cycling '74. All rights reserved.
 *
 */

#ifndef __EXT_TIME_H__
#define __EXT_TIME_H__

BEGIN_USING_C_LINKAGE

/**
 Flags that determine attribute and time object behavior
 @ingroup time
*/
enum {
	TIME_FLAGS_LOCATION = 1,		///< 1 1 0 location-based bar/beat/unit values (as opposed to interval values, which are 0 0 0 relative)
	TIME_FLAGS_TICKSONLY = 2,		///< only ticks-based values (not ms) are acceptable
	TIME_FLAGS_FIXEDONLY = 4,		///< only fixed values (ms, hz, samples) are acceptable
	TIME_FLAGS_LOOKAHEAD = 8,		///< add lookahead attribute (unsupported)
	TIME_FLAGS_USECLOCK = 16,		///< this time object will schedule events, not just hold a value
	TIME_FLAGS_USEQELEM = 32,		///< this time object will defer execution of scheduled events to low priority thread
	TIME_FLAGS_FIXED = 64,			///< will only use normal clock (i.e., will never execute out of ITM)
	TIME_FLAGS_PERMANENT = 128,		///< event will be scheduled in the permanent list (tied to a specific time)
	TIME_FLAGS_TRANSPORT = 256,		///< add a transport attribute
	TIME_FLAGS_EVENTLIST = 512,		///< add an eventlist attribute (unsupported)
	TIME_FLAGS_CHECKSCHEDULE = 1024,	///< internal use only
	TIME_FLAGS_LISTENTICKS = 2048,		///< flag for time_listen: only get notifications if the time object holds tempo-relative values
	TIME_FLAGS_NOUNITS = 4096,			///< internal use only
	TIME_FLAGS_BBUSOURCE = 8192,		///< source time was in bar/beat/unit values, need to recalculate when time sig changes
	TIME_FLAGS_POSITIVE = 16384			///< constrain any values < 0 to 0
};



/**
 Stop a currently scheduled time object.
 
 @ingroup	time
 @param		x				The time object.

*/
void time_stop(t_object *x);


/**
 Execute a time object's task, then if it was already set to execute, reschedule for the current interval value of the object.
 
 @ingroup	time
 @param		x				The time object.
 
*/
void time_tick(t_object *x);


/**
 Convert the value of a time object to milliseconds.
 
 @ingroup	time
 @param		x				The time object.
 @return					The time object's value, converted to milliseconds.
*/
double time_getms(t_object *x);


/**
 Convert the value of a time object to ticks.
 
 @ingroup	time
 @param		x				The time object.
 @return					The time object's value, converted to ticks.
*/
double time_getticks(t_object *x);


/**
 Return the phase of the ITM object (transport) associated with a time object.
 
 @ingroup	time
 @param		tx				The time object.
 @param		phase			Pointer to a double to receive the progress within the specified time value of the associated ITM object.
 @param		slope			Pointer to a double to receive the slope (phase difference) within the specified time value of the associated ITM object.
*/
void time_getphase(t_object *tx, double *phase, double *slope);


/**
 Specify that a millisecond-based attribute to be updated automatically when the converted milliseconds of the time object's value changes.
 
 @ingroup	time
 @param		x				The time object.
 @param		attr			Name of the millisecond based attribute in the owning object that will be updated
 @param		flags			If TIME_FLAGS_LISTENTICKS is passed here, updating will not happen if the time value is fixed (ms) based
 */
void time_listen(t_object *x, t_symbol *attr, long flags);


/**
 Set the current value of a time object (either an interval or a position) using a Max message.
 
 @ingroup	time
 @param		tx				The time object.
 @param		s				Message selector.
 @param		argc			Count of arguments.
 @param		argv			Message arguments.
 */
void time_setvalue(t_object *tx, t_symbol *s, long argc, t_atom *argv);

/**
 Create an attribute permitting a time object to be changed in a user-friendly way.
 
 @ingroup	time
 @param		c				Class being initialized.
 @param		attrname		Name of the attribute associated with the time object.
 @param		attrlabel		Descriptive label for the attribute (appears in the inspector)
 @param		flags			Options, see "Flags that determine time object behavior" above
*/
void class_time_addattr(t_class *c, char *attrname, char *attrlabel, long flags);

/**
 Create a new time object.
 
 @ingroup	time
 @param		owner			Object that will own this time object (task routine, if any, will pass owner as argument).
 @param		attrname		Name of the attribute associated with the time object.
 @param		tick			Task routine that will be executed (can be NULL)
 @param		flags			Options, see "Flags that determine time object behavior" above
 @return					The newly created time object.
*/
void *time_new(t_object *owner, t_symbol *attrname, method tick, long flags);

/**
 Return a time object associated with an attribute of an owning object.
 
 @ingroup	time
 @param		owner			Object that owns this time object (task routine, if any, will pass owner as argument).
 @param		attrname		Name of the attribute associated with the time object.
 @return					Time object associated with the named attribute.
*/
t_object *time_getnamed(t_object *owner, t_symbol *attrname);


void time_enable_attributes(t_object *x);

/**
 Return whether this time object currently holds a fixed (millisecond-based) value.
 
 @ingroup	time
 @param		x				Time object.
 @return					True if time object's current value is fixed, false if it is tempo-relative.
*/
long time_isfixedunit(t_object *x);


/**
 Schedule a task, with optional quantization.
 
 @ingroup	time
 @param		x				The time object that schedules temporary events (must have been created with TIME_FLAGS_USECLOCK but not TIME_FLAGS_PERMANENT)
 @param		quantize		A time object that holds a quantization interval, can be NULL for no quantization.
*/
void time_schedule(t_object *x, t_object *quantize);


/**
 Schedule a task, with optional minimum interval, 
 
 @ingroup	time
 @param		x				The time object that schedules temporary events (must have been created with TIME_FLAGS_USECLOCK but not TIME_FLAGS_PERMANENT)
 @param		quantize		The minimum interval into the future when the event can occur, can be NULL if there is no minimum interval.
*/
void time_schedule_limit(t_object *x, t_object *quantize);

/**
 Schedule a task for right now, with optional quantization.
 
 @ingroup	time
 @param		x				The time object that schedules temporary events. The time interval is ignored and 0 ticks is used instead.
 @param		quantize		A time object that holds a quantization interval, can be NULL for no quantization.
*/
void time_now(t_object *x, t_object *quantize);


/**
 Return the ITM object associated with this time object.
 
 @ingroup	time
 @param		ox				Time object.
 @return					The associated ITM object.
*/
void *time_getitm(t_object *ox);


/**
 Calculate the quantized interval (in ticks) if this time object were to be scheduled at the current time.
 
 @ingroup	time
 @param		ox				Time object.
 @param		vitm			The associated ITM object (use time_getitm() to determine it).
 @param		oq				A time object that holds a quantization interval, can be NULL.
 @return					Interval (in ticks) for scheduling this object.
*/
double time_calcquantize(t_object *ox, void *vitm, t_object *oq);


/**
 Associate a named setclock object with a time object (unsupported).
 
 @ingroup	time
 @param		tx				Time object.
 @param		sc				Name of an associated setclock object.
*/
void time_setclock(t_object *tx, t_symbol *sc);

END_USING_C_LINKAGE

#endif // __EXT_TIME_H__

