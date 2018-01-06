

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"


typedef struct _patcherbangs
{
	t_object        ob;
    t_patcher       *base_patch;
    void            **outlets;
} t_patcherbangs;

t_class *patcherbangs_class;

void patcherbangs_bang(t_patcherbangs *x)
{}

t_max_err patcherbangs_notify(t_patcherbangs *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    if( msg == gensym("snapshotlistchanged") ) // << there are a lot of these
    {
        return 0;
    }
    
//    post("%s %s %p %p ", s->s_name, msg->s_name, sender, data);

    if( msg == gensym("loadbang_start"))
    {
        outlet_bang(x->outlets[0]);
    }
    else if( msg == gensym("loadbang_parameter"))
    {
        outlet_bang(x->outlets[1]);
    }
    else if( msg == gensym("loadbang_pattr"))
    {
        outlet_bang(x->outlets[2]);
    }
    else if( msg == gensym("loadbang_internal"))
    {
        outlet_bang(x->outlets[3]);
    }
    else if( msg == gensym("loadbang_loadbang"))
    {
        outlet_bang(x->outlets[4]);
    }
    else if( msg == gensym("loadbang_end"))
    {
        outlet_bang(x->outlets[5]);
    }
    else if( msg == gensym("aftersnapshotrestore") )
    {
        outlet_bang(x->outlets[6]);
    }
    if( msg == gensym("willfree") )
    {
        outlet_bang(x->outlets[7]);
    }
    if( msg == gensym("free") )
    {
        outlet_bang(x->outlets[8]);
    }
    
    return 0;
}

void patcherbangs_loadbang(t_patcherbangs *x)
{
}


void patcherbangs_assist(t_patcherbangs *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET)
        sprintf(s,"inlet (does nothing currently)");
    else {
        switch (a) {
            case 0:
                sprintf(s,"bang free patcher");
                break;
            case 1:
                sprintf(s,"bang willfree patcher");
                break;
            case 2:
                sprintf(s, "bang after snapshot restore");
                break;
            case 3:
                sprintf(s, "loadbang_end");
                break;
            case 4:
                sprintf(s, "loadbang_loadbang");
                break;
            case 5:
                sprintf(s, "loadbang_internal");
                break;
            case 6:
                sprintf(s, "loadbang_pattr");
                break;
            case 7:
                sprintf(s, "loadbang_parameter");
                break;
            case 8:
                sprintf(s, "loadbang_start");
                break;
        }
    }
}

void patcherbangs_free(t_patcherbangs *x)
{
    object_detach_byptr(x, x->base_patch);
    free(x->outlets);
}

void *patcherbangs_new(t_symbol *msg, short argc, t_atom *argv)
{
    t_patcherbangs *x = NULL;
    x = (t_patcherbangs *)object_alloc(patcherbangs_class);
	if(x)
    {
     
        x->outlets = (void **)malloc(sizeof(void *) * 9);
        for( int i = 0; i < 9; i++)
        {
            x->outlets[i] = bangout(x);
        }
        
        t_patcher *patcher;
        t_max_err err = object_obex_lookup(x, gensym("#P"), &patcher);
        if( err )
        {
            object_error((t_object*)x, "error type %d", err );
            return 0;
        }
        
        x->base_patch = patcher;
        object_attach_byptr(x, x->base_patch);
        
    }
	return x;
}

int main(void)
{
	t_class *c = class_new("patcherbangs", (method)patcherbangs_new, (method)patcherbangs_free, sizeof(t_patcherbangs), 0L, A_GIMME, 0);
	class_addmethod(c, (method)patcherbangs_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)patcherbangs_bang, "bang", 0);

    class_addmethod(c, (method)patcherbangs_notify, "notify", A_CANT, 0);
    class_addmethod(c, (method)patcherbangs_loadbang, "loadbang", 0);

    
	class_register(CLASS_BOX, c);
	patcherbangs_class = c;

	common_symbols_init();

	return 0;
}

