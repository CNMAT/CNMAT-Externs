// ext_itm.h copyright 2008 cycling '74 all rights reserved

#ifndef __EXT_ITM_H__
#define __EXT_ITM_H__

BEGIN_USING_C_LINKAGE

typedef struct _clocksource
{
	t_object c_ob;
	method c_getticks;		// returns the current tick count
	method c_getstate;		// returns 0 if transport not going, 1 if going
	t_symbol *c_name;		// name
	long c_usedcount;		// number of transports using this clock source
} t_clocksource;
	
typedef t_object t_itm;

void *itm_getglobal(void);
void *itm_getnamed(t_symbol *s, long create);
void *itm_getfromarg(t_object *o, t_symbol *s);
void itm_reference(t_itm *x);
void itm_dereference(t_itm *x);

void itm_deleteeventlist(t_itm *x, t_symbol *eventlist);
void itm_eventlistseek(t_itm *x);
void itm_geteventlistnames(t_itm *x, long *count, t_symbol ***names);
void itm_switcheventlist(t_itm *x, t_symbol *eventlist, double offset);

double itm_gettime(t_itm *x);
double itm_getticks(t_itm *x);
void itm_dump(t_itm *x);

void itm_sync(t_itm *x);
void itm_settimesignature(t_itm *x, long num, long denom, long flags);
void itm_gettimesignature(t_itm *x, long *num, long *denom);

void itm_seek(t_itm *x, double oldticks, double newticks, long chase);
void itm_pause(t_itm *x);
void itm_resume(t_itm *x);
long itm_getstate(t_itm *x);
void itm_setresolution(double res);
double itm_getresolution(void);
t_symbol *itm_getname(t_itm *x);

t_max_err itm_parse(t_itm *x, long argc, t_atom *argv, long flags, double *ticks, double *fixed, t_symbol **unit, long *bbu, char *bbusource);
double itm_tickstoms(t_itm *x, double ticks);
double itm_mstoticks(t_itm *x, double ms);
double itm_mstosamps(t_itm *x, double ms);
double itm_sampstoms(t_itm *x, double samps);
void itm_barbeatunitstoticks(t_itm *x, long bars, long beats, double units, double *ticks, char position);
void itm_tickstobarbeatunits(t_itm *x, double ticks, long *bars, long *beats, double *units, char position);
void itm_format(t_itm *x, double ms, double ticks, long flags, t_symbol *unit, long *argc, t_atom **argv);
long itm_isunitfixed(t_symbol *u);

void itmclock_delay(t_object *x, t_itm *m, t_symbol *eventlist, double delay, long quantization);
void *itmclock_new(t_object *owner, t_object *timeobj, method task, method killer, long permanent);
void itmclock_set(t_object *x, t_itm *m, t_symbol *eventlist, double time);
void itmclock_unset(t_object *x);

void *itm_clocksource_getnamed(t_symbol *name, long create);
void itm_getclocksources(long *count, t_symbol ***sources);

END_USING_C_LINKAGE

#endif // __EXT_ITM_H__
