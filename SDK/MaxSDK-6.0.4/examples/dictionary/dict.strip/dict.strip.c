#include "ext.h"
#include "ext_obex.h"
#include "ext_strings.h"
#include "ext_dictobj.h"

#define MAX_NUM_ITEMS 256


typedef struct _dict_strip{
    t_object	obj;
	void		*outlet_keyvals;
	void		*outlet_dict;
	t_symbol	*keys[MAX_NUM_ITEMS];
	long		numkeys;
	t_symbol	*name;
} t_dict_strip;


// prototypes
void *dict_strip_new(t_symbol *s, long argc, t_atom *argv);
void dict_strip_free(t_dict_strip *x);
void dict_strip_assist(t_dict_strip *x, void *b, long m, long a, char *s);
void dict_strip_bang(t_dict_strip *x);
void dict_strip_dictionary(t_dict_strip *x, t_symbol *s);


// class statics/globals
static t_class	*s_dict_strip_class;


/************************************************************************************/
int main(void)
{
	t_class	*c;
	
	common_symbols_init();
	c = class_new("dict.strip", (method)dict_strip_new, (method)dict_strip_free, sizeof(t_dict_strip), (method)NULL, A_GIMME, 0);

	class_addmethod(c, (method)dict_strip_dictionary, 	"dictionary", 	A_SYM, 0);
	// class_addmethod(c, (method)dict_strip_bang, 		"bang", 		0);
    class_addmethod(c, (method)dict_strip_assist,		"assist",		A_CANT, 0);
	
	CLASS_ATTR_SYM_VARSIZE(c,	"keys",		0, t_dict_strip, keys, numkeys, MAX_NUM_ITEMS);
	CLASS_ATTR_CATEGORY(c,		"keys",		0, "Dictionary");
	CLASS_ATTR_LABEL(c,			"keys",		0, "Keys");
	CLASS_ATTR_BASIC(c,			"keys",		0);
	
	class_register(_sym_box, c);
	s_dict_strip_class = c;
	return 0;
}


/************************************************************************************/
void *dict_strip_new(t_symbol *s, long argc, t_atom *argv)
{
	t_dict_strip 	*x;
	long			attrstart = attr_args_offset(argc, argv);

	x = (t_dict_strip *)object_alloc(s_dict_strip_class);
	if (x) {
		x->outlet_keyvals = outlet_new(x, 0L);
		x->outlet_dict = outlet_new(x, "dictionary");
		
		if (attrstart)
			object_attr_setvalueof(x, gensym("keys"), attrstart, argv);
		
		attr_args_process(x, argc, argv);
	}
	return x;
}


void dict_strip_free(t_dict_strip *x)
{
	;
}


/************************************************************************************/
void dict_strip_assist(t_dict_strip *x, void *b, long m, long a, char *s)
{
	if (m==ASSIST_INLET) {
		strcpy(s, "dictionary input");
	}
	else if (m==ASSIST_OUTLET) {
		switch (a) {
			case 0: strcpy(s, "dictionary output stripped of specified keys"); break;
			case 1: strcpy(s, "series of key/value pairs stripped from the dictionary"); break;
		}
	}
}


void dict_strip_dictionary(t_dict_strip *x, t_symbol *s)
{
	t_dictionary	*d = dictobj_findregistered_retain(s);
	long			i;
	t_atom			a;
	t_max_err		err;
	
	if (!d) {
		object_error((t_object*)x, "unable reference dictionary named %s", s);
		return;
	}
	
	for (i=0; i < x->numkeys; i++) {
		err = dictionary_getatom(d, x->keys[i], &a);
		if (!err) {
			if (atomisatomarray(&a)) {
				t_atom	*av = NULL;
				long	ac = 0;
				
				atomarray_copyatoms((t_atomarray*)a.a_w.w_obj, &ac, &av);
				outlet_anything(x->outlet_keyvals, x->keys[i], ac, av);
				sysmem_freeptr(av);
			}
			else
				outlet_anything(x->outlet_keyvals, x->keys[i], 1, &a);
			dictionary_deleteentry(d, x->keys[i]);
		}
	}
	
	x->name = s;	
	dict_strip_bang(x);
	object_notify(d, _sym_modified, NULL);
	dictobj_release(d);
}


void dict_strip_bang(t_dict_strip *x)
{
	if (x->name) {
		t_atom a[1];
		
		atom_setsym(a, x->name);
		outlet_anything(x->outlet_dict, _sym_dictionary, 1, a);
	}
}

