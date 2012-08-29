/*
 Written by Jeff Lubow, The Center for New Music and Audio Technologies,
 University of California, Berkeley.  Copyright (c) 2009, The Regents of
 the University of California (Regents). 
 Permission to use, copy, modify, distribute, and distribute modified versions
 of this software and its documentation without fee and without a signed
 licensing agreement, is hereby granted, provided that the above copyright
 notice, this paragraph and the following two paragraphs appear in all copies,
 modifications, and distributions.
 
 IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
 BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
 HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
 MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 
 
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 NAME: gridpanel
 DESCRIPTION: A 2-D grid that tracks mouse location.
 AUTHORS: Jeff Lubow
 COPYRIGHT_YEARS: 2011
 SVN_REVISION: $LastChangedRevision: 1 $
 VERSION 0.0: First try
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "jpatcher_api.h"   
#include "jgraphics.h"

typedef struct _gpan {
    t_jbox ob; 
    void *outlet;
    double linewid;
    int xdivisions, ydivisions;
	char idlemousing;
	int xitem, yitem;
    bool drawbox;
    t_symbol msg;
    t_jrgba bgc, fgc, sqc;
} t_gpan;

static t_class *gpan_class;

void gpan_paint(t_gpan *x, t_object *patcherview);
void gpan_bang(t_gpan *x);
void gpan_int(t_gpan *x, long n);
void gpan_float(t_gpan *x, double xx);

void gpan_list(t_gpan *x, t_symbol *msg, short argc, t_atom *argv);
void gpan_anything(t_gpan *x, t_symbol *msg, short argc, t_atom *argv);
void gpan_free(t_gpan *x);
void gpan_assist(t_gpan *x, void *b, long m, long a, char *s);
void gpan_inletinfo(t_gpan *x, void *b, long index, char *t);

void gpan_mousedown(t_gpan *x, t_object *patcherview, t_pt pt, long modifiers);
void gpan_mousedrag(t_gpan *x, t_object *patcherview, t_pt pt, long modifiers);
void gpan_mousemove(t_gpan *x, t_object *patcherview, t_pt pt, long modifiers);
void gpan_mouseleave(t_gpan *x, t_object *patcherview, t_pt pt, long modifiers);
void gpan_mouseup(t_gpan *x, t_object *patcherview, t_pt pt, long modifiers);

//------------------------custom methods--------------------------------------
//----------------------------------------------------------------------------
void gpan_lines(t_gpan *x, long n);
void gpan_xdivisions(t_gpan *x, long n);
void gpan_ydivisions(t_gpan *x, long n);
void gpan_linewidth(t_gpan *x, double n);
void process_mouse(t_gpan *x, t_object *patcherview, t_pt pt);
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void *gpan_new(t_symbol *msg, short argc, t_atom *argv);

void gpan_paint(t_gpan *x, t_object *patcherview) {
	
    t_rect bgrect;                                                                  
    t_jrgba bgcolor = x->bgc; 
	t_jrgba sqcolor = x->sqc;
    t_jrgba lincolor = x->fgc;													// built-in way to access rgba values for obj	
	
    t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview);     // Get the graphics context.    

    //paint bg
    jbox_get_rect_for_view((t_object *)x, patcherview, &bgrect); 
    jgraphics_set_source_jrgba(g, &bgcolor);
    jgraphics_rectangle(g, 0., 0., bgrect.width, bgrect.height);
    jgraphics_fill(g);

    int i;
    double availablespace = bgrect.height - x->linewid;
    double lineoffset = x->linewid / 2.0;
	double frac;
	int xdivisions = x->xdivisions;
	int ydivisions = x->ydivisions;
	
	if(x->drawbox)
    {
		double xdiv, ydiv, xdemarcation, ydemarcation;
		
		// zero to n
		xdiv = bgrect.width / xdivisions;
		ydiv = bgrect.height / ydivisions;
		xdemarcation = xdiv * (double)x->xitem;
		ydemarcation = ydiv * (double)x->yitem;
						
		jgraphics_set_source_jrgba(g, &sqcolor);
		jgraphics_move_to(g, xdemarcation, ydemarcation);
		jgraphics_rectangle(g, xdemarcation, ydemarcation, xdiv, ydiv);
		jgraphics_fill(g);
	}

	jgraphics_set_source_jrgba(g, &lincolor);                   // set color
    jgraphics_set_line_width(g, x->linewid);
	
	//draw h lines    
    for(i = 0; i < (ydivisions + 1); i++) {
        frac = (double)i / x->ydivisions;
        double yloc = (frac * availablespace) + lineoffset;
        jgraphics_move_to(g, 0, yloc);
        jgraphics_line_to(g, bgrect.width, yloc);
    }

	//draw v lines
    for(i = 0; i < (xdivisions + 1); i++) {
        frac = (double)i / x->xdivisions;
        double xloc = (frac * availablespace) + lineoffset;
        jgraphics_move_to(g, xloc, 0);
        jgraphics_line_to(g, xloc, bgrect.height);

    }
	
    jgraphics_stroke(g);

}

void gpan_lines(t_gpan *x, long n) {
    long size = n;
    
    if(size > 1) {
        x->xdivisions = size;
        x->ydivisions = size;
    } else {
        x->xdivisions = 2;
        x->ydivisions = 2;
    }
        
    jbox_redraw((t_jbox *)x);
}

void gpan_xdivisions(t_gpan *x, long n) {
    if(n > 1) {
        x->xdivisions = n;
    } else {
        x->xdivisions = 2;
    }
        
    jbox_redraw((t_jbox *)x);
}

void gpan_ydivisions(t_gpan *x, long n) {
    if(n > 1) {
        x->ydivisions = n;
    } else {
        x->ydivisions = 2;
    }
        
    jbox_redraw((t_jbox *)x);
}

void gpan_linewidth(t_gpan *x, double n) {
    double lw = n;
    
    if((lw > 0.01) && (lw < 40.0)) {
        x->linewid = lw;
    }
    else if(lw < 0.01) {
        x->linewid = 0.01;
    }
    else if(lw > 40.0) {
        x->linewid = 40.0;
    }
    
    jbox_redraw((t_jbox *)x);
}

void gpan_mousedown(t_gpan *x, t_object *patcherview, t_pt pt, long modifiers){
	x->drawbox = TRUE;
	process_mouse(x, patcherview, pt);
}

void gpan_mouseup(t_gpan *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->drawbox = FALSE;
	jbox_redraw((t_jbox *)x);
}

void gpan_mousedrag(t_gpan *x, t_object *patcherview, t_pt pt, long modifiers){
	process_mouse(x, patcherview, pt);
}

void gpan_mousemove(t_gpan *x, t_object *patcherview, t_pt pt, long modifiers){
	if(x->idlemousing)
	{
		process_mouse(x, patcherview, pt);
		x->drawbox = TRUE;	
		jbox_redraw((t_jbox *)x);			
	}
}

void gpan_mouseleave(t_gpan *x, t_object *patcherview, t_pt pt, long modifiers){
	if(x->idlemousing)
	{
		x->drawbox = FALSE;
		jbox_redraw((t_jbox *)x);
	}
}

void gpan_bang(t_gpan *x) {
    jbox_redraw((t_jbox *)x);
}

void gpan_int(t_gpan *x, long n){}
void gpan_float(t_gpan *x, double f){}
void gpan_list(t_gpan *x, t_symbol *msg, short argc, t_atom *argv){}

void gpan_anything(t_gpan *x, t_symbol *msg, short argc, t_atom *argv) {
    jbox_redraw((t_jbox *)x);
}

void gpan_free(t_gpan *x) {
    jbox_free(&x->ob);
}

void gpan_assist(t_gpan *x, void *b, long io, long index, char *s) {
    switch(io) {
        case 1:
            switch(index) {
                case 0:
                    sprintf(s, "this outlet does absolutely nothing.");
                break;
            }
        break;

        case 2:
            switch(index) {
                case 0:
					   sprintf(s, "outputs x and y information");

                break;
            }
        break;
    }
}

void process_mouse(t_gpan *x, t_object *patcherview, t_pt pt)
{
	double tx = pt.x;
    double ty = pt.y;
	    
    t_rect bgrect;                                                                  
    jbox_get_rect_for_view((t_object *)x, patcherview, &bgrect);
    
    int nlinesx = x->xdivisions;
    int nlinesy = x->ydivisions;
    
    if( (tx >= 0) && (tx <= bgrect.width) && (ty >= 0) && (ty <= bgrect.height) )
    {
	    double sx = (tx / bgrect.width) * nlinesx;
	    double sy = (ty / bgrect.height) * nlinesy;
	    
	    int rx = floor(sx);
	    int ry = floor(sy);
		
		x->xitem = rx;
		x->yitem = ry;
		
	    //make a new array av that is the length of xy point
	    t_atom av[2];
		
	    atom_setlong(&(av[0]), rx);
	    atom_setlong(&(av[1]), ry);
	    outlet_list(x->outlet, NULL, 2, av);		
    }
	
	jbox_redraw((t_jbox *)x);	
}

void *gpan_new(t_symbol *msg, short argc, t_atom *argv) {
    t_gpan *x;
    t_dictionary *d = NULL; 
    long boxflags; 
    
    if(!(d = object_dictionaryarg(argc, argv))) {
        return NULL; 
    } 
    
    boxflags = 0 
    | JBOX_DRAWFIRSTIN 
    //| JBOX_NODRAWBOX
    | JBOX_DRAWINLAST
    //| JBOX_TRANSPARENT  
    //| JBOX_NOGROW
    //| JBOX_GROWY
    | JBOX_GROWBOTH
    //| JBOX_HILITE
    | JBOX_BACKGROUND
    | JBOX_DRAWBACKGROUND
    //| JBOX_NOFLOATINSPECTOR
    //| JBOX_MOUSEDRAGDELTA
    //| JBOX_TEXTFIELD
    ;

    if(x = (t_gpan *)object_alloc(gpan_class)) {
        jbox_new((t_jbox *)x, boxflags, argc, argv); 
        x->ob.b_firstin = (void *)x; 
        x->outlet = outlet_new(x, NULL);

        //setup default parameters for members of struct
		x->drawbox = FALSE;
        x->xdivisions = 3;
        x->ydivisions = 3;
        x->linewid = 5.0;
		x->idlemousing = 0;
        
        attr_dictionary_process(x, d); 
        jbox_ready((t_jbox *)x);
        
        return x;
    }
    return NULL;
}

int main(void) {
    t_class *c = class_new("gridpanel", (method)gpan_new, (method)gpan_free, sizeof(t_gpan), (method)NULL, A_GIMME, 0L);
    c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
    jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR); 
    
    class_addmethod(c, (method)gpan_paint, "paint", A_CANT, 0); 
    class_addmethod(c, (method)gpan_bang, "bang", 0);
    class_addmethod(c, (method)gpan_int, "int", A_LONG, 0);
    class_addmethod(c, (method)gpan_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)gpan_list, "list", A_GIMME, 0);
    class_addmethod(c, (method)gpan_anything, "anything", A_GIMME, 0);
    class_addmethod(c, (method)gpan_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)stdinletinfo, "inletinfo", A_CANT, 0);
    
	class_addmethod(c, (method)gpan_mouseup, "mouseup", A_CANT, 0);
    class_addmethod(c, (method)gpan_mousedown, "mousedown", A_CANT, 0);
    class_addmethod(c, (method)gpan_mousedrag, "mousedrag", A_CANT, 0);
    class_addmethod(c, (method)gpan_mousemove, "mousemove", A_CANT, 0);
	class_addmethod(c, (method)gpan_mouseleave, "mouseleave", A_CANT, 0);
	
    class_addmethod(c, (method)gpan_lines, "lines", A_LONG, 0);
    class_addmethod(c, (method)gpan_xdivisions, "xdivisions", A_LONG, 0);
    class_addmethod(c, (method)gpan_ydivisions, "ydivisions", A_LONG, 0);
    class_addmethod(c, (method)gpan_linewidth, "linewidth", A_FLOAT, 0);    

    CLASS_STICKY_ATTR(c, "category", 0, "Color");

    CLASS_ATTR_LONG(c, "xdivisions", 0, t_gpan, xdivisions);
    CLASS_ATTR_SAVE(c, "xdivisions", 0);    
    CLASS_ATTR_LONG(c, "ydivisions", 0, t_gpan, ydivisions);
    CLASS_ATTR_SAVE(c, "ydivisions", 0);
	
	CLASS_ATTR_CHAR(c, "idlemousing", 0, t_gpan, idlemousing);
	CLASS_ATTR_SAVE(c, "idlemousing", 0);
    CLASS_ATTR_STYLE_LABEL(c, "idlemousing", 0, "onoff", "Idle-mousing On");
	
    CLASS_ATTR_DOUBLE(c, "linewidth", 0, t_gpan, linewid);
    CLASS_ATTR_SAVE(c, "linewidth", 0);
    
	CLASS_ATTR_RGBA(c, "sqc", 0, t_gpan, sqc);
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "sqc", 0, "0. 0. 0. 0.");  
	CLASS_ATTR_STYLE_LABEL(c, "sqc", 0, "rgba", "Square Color");

	CLASS_ATTR_RGBA(c, "bgc", 0, t_gpan, bgc);
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgc", 0, "0. 0. 0. 0.");  
    CLASS_ATTR_STYLE_LABEL(c, "bgc", 0, "rgba", "Background Color");

    CLASS_ATTR_RGBA(c, "fgc", 0, t_gpan, fgc);
    CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "fgc", 0, "0. 0. 0. 1.");
    CLASS_ATTR_STYLE_LABEL(c, "fgc", 0, "rgba", "Foreground Color");

    CLASS_STICKY_ATTR_CLEAR(c, "category");

    CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100.");   //default size of box

    class_register(CLASS_BOX, c);
    gpan_class = c;
    common_symbols_init();

    return 0;
}

