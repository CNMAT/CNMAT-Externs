// ext_drag.h copyright 2008 cycling '74

#ifndef __EXT_DRAG_H__

#define __EXT_DRAG_H__

BEGIN_USING_C_LINKAGE

char *jdrag_getitemstring(t_object *dg, long index, long quote, long backslash);
t_object *jdrag_getobject(t_object *dg, long index);
void jdrag_getlocation(t_object *dg, long index, double *xpos, double *ypos);
t_object *jdrag_createobject(t_object *dg, t_object *pv, void *d);
t_object *jdrag_createnewobj(t_object *dg, t_object *pv, char *classname, char *str);
t_object *jdrag_createmessage(t_object *dg, t_object *pv, t_symbol *msg, long ac, t_atom *av);
void jdrag_add(t_object *dg, t_object *rcv, t_symbol *msg, char *desc, void *arg);
long jdrag_matchdragrole(t_object *dx, t_symbol *role, long flags);
void jdrag_setboxlocation(t_object *dg, t_object *box);
void jdrag_object_add(t_object *dg, t_object *obj, t_symbol *msg);
void jdrag_box_add(t_object *dg, t_object *box, t_symbol *msg);

END_USING_C_LINKAGE

#endif	// __EXT_DRAG_H__
