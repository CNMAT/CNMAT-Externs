/* Lgrid.c --
revised for OSx
march 2003
 */

/* the required include file(s) */
#include "ext.h"
#include "ext_user.h"
#include "ext_anim.h"
#include "ext_strings.h"
#include "ext_path.h"

#define MAXSIZE 256
#define WIDTH 1024
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

/* structure definition of your object */
typedef struct lgrid
{
	t_box lgrid_box;			// required header
	void *m_out[16];				//
	void *statusout;
	
	long theFont;
	//unsigned char theLines[16][WIDTH];
	unsigned char *theLines;
	unsigned char undoLine[WIDTH];
	unsigned char clipboard[WIDTH];
	unsigned char valuetopixel[128];
	unsigned char pixelvalues[128];
	short lastOut[16];
	short clipend;
	short undochannel;
	RGBColor colors[16];
	RGBColor RGBCursor;
	RGBColor lightblue;
	RGBColor accent;
	short show[16];
	short solo;
	long cursor;		// cursor position
	short front;	// active line
	long width;		// width is the right edge of drawing space cursor < width
	long height;  // height is the highest value data can take
	short edge;
	short zero;
	short vStep;
	short hStep;
	short numSteps;
	short numRows;
	short marksize;
	Point lastclick;
	Point initialclick;
	short isInitialClick;
	//short gestureleft;
	//short gestureright;
	short lastindex;
	short highres;
	short initialvalue;
	short editbaseline;
	short boxmode;
	short plotmode;
	short quantize;
	short editmode;
	short editstart;
	short editend;
	short drawvalue;
	short enteredvalue;
	//float pixelsperpoint;
	//short pointsperpixel;
	short fixpoint[WIDTH];
	short fixrow[16];
	short muteEdits;
	short oldstart;
	short oldstop;
	Point plotpt;
	void *redrawQelem;
	void *cursorQelem;
	void *plotQelem;
	void *setBoxQelem;
	void *pointQelem;
	void *rowQelem;
	Rect cursorpath;
	Rect drawingrect;
	Rect paletterect;
	Rect activerect;
	Atom theList[MAXSIZE];
	
	
	Symbol *fn;
	short pathID;
	long ArraySize;         //number of entries in array
	long realArraySize;
} Lgrid;

/* globalthat holds the class definition */
void *lgrid_class;

/* prototypes for your functions */

void lgrid_int(Lgrid *x, long n);
void lgrid_scan(Lgrid *x, long n);
void lgrid_enter(Lgrid *x, long n);
void lgrid_bang(Lgrid *x);
void lgrid_peek(Lgrid* x, long i, long n);
void lgrid_dump(Lgrid * x, long n);
void lgrid_list(Lgrid *x,Symbol *s,int argc,Atom *argv);
void lgrid_set(Lgrid *x,Symbol *s,int argc,Atom *argv);
void lgrid_clear(Lgrid *x,Symbol *s,int argc,Atom *argv);
void lgrid_clearbox(Lgrid *x,Symbol *s,int argc,Atom *argv);
void lgrid_clearall(Lgrid *x,Symbol *s,int argc,Atom *argv);
void lgrid_put(Lgrid *x,long channel, long place, long value) ;
void lgrid_plot(Lgrid *x, long n);
void lgrid_setBox(Lgrid *x, long n, long m);
void lgrid_goto(Lgrid *x, long n);
void lgrid_undo(Lgrid *x, long n, long m);
void lgrid_setundo(Lgrid *x);
void lgrid_copy(Lgrid *x,Symbol *s,int argc,Atom *argv);
void lgrid_paste(Lgrid *x,Symbol *s,int argc,Atom *argv);
void lgrid_move(Lgrid *x, long n, long wrap);
void lgrid_front(Lgrid *x, long n);
void lgrid_show(Lgrid *x, long n);
void lgrid_solo(Lgrid *x, long n);
void lgrid_hide(Lgrid *x, long n);
void lgrid_scale(Lgrid *x, float n);
void lgrid_quantsize(Lgrid *x, long n);
void lgrid_marksize(Lgrid *x, long n);
void lgrid_quantize(Lgrid *x, long n);
void lgrid_setNumRows(Lgrid *x, long n);
void lgrid_muteEdits(Lgrid *x, long n);
void lgrid_click(Lgrid *x, Point pt, short modifiers);
void lgrid_plotfun(Lgrid * x);
void lgrid_setBoxFun(Lgrid * x);
void lgrid_dragfun(Lgrid *x, Point pt, short button);
void lgrid_bidle(Lgrid *x);
void reset(Lgrid * x);
void setEditRange(Lgrid * x, int where);
void fillpointsperpixel(Lgrid * x, short vStep);
unsigned char pixeltovalue(Lgrid * x,short whichPixel);
void lgrid_update(Lgrid *x);
void lgrid_size(Lgrid *x, long n, long m);
void lgrid_psave(Lgrid * x, void *dest);
void lgrid_assist(Lgrid *x,void *b,long msg,long arg,char *dst);
void *lgrid_new(t_symbol *sym, short argc, t_atom *argv);
void *newFromMenu(t_patcher *p, long left, long top, long font);
void lgrid_redraw(Lgrid * x);
void lgrid_drawcursor(Lgrid * x);
void lgrid_drawpoint(Lgrid * x);
void lgrid_drawrow(Lgrid * x);
void lgrid_free(Lgrid * x);
void lgrid_setcolors(Lgrid * x);
void lgrid_read(Lgrid *x,Symbol *s);
void lgrid_write(Lgrid *x,Symbol *s,int argc,Atom *argv);
void lgrid_readfile(Lgrid *x,Symbol *s,int argc,Atom *argv);
void lgrid_writefile(Lgrid *x,Symbol *dummy,int argc,Atom *argv);
/* initialization routine */

int main(void)
{
Symbol * s;
Symbol * l;	
	/* tell Max about your class. */
	setup((Messlist **)&lgrid_class, (method)lgrid_new, (method)lgrid_free, (short)sizeof(Lgrid), (method)newFromMenu,A_GIMME, 0);
	
	/* bind your methods to symbols */
	addbang((method)lgrid_bang);
	addint((method)lgrid_int);
	addmess((method)lgrid_int, "cursor",A_DEFLONG,0);
	addmess((method)lgrid_list, "list",A_GIMME,0);
	addmess((method)lgrid_set, "set",A_GIMME,0);
	addmess((method)lgrid_clear, "clear",A_GIMME,0);
	addmess((method)lgrid_clearbox, "clearbox",A_GIMME,0);
	addmess((method)lgrid_copy, "copy",A_GIMME,0);
	addmess((method)reset, "reset",0);
	addmess((method)lgrid_paste, "paste",A_GIMME,0);
	addmess((method)lgrid_move, "move", A_DEFLONG,A_DEFLONG, 0);
	addmess((method)lgrid_clearall, "clearall",A_GIMME,0);
	addmess((method)lgrid_put, "put", A_DEFLONG,A_DEFLONG,A_DEFLONG, 0);
	addmess((method)lgrid_plot, "plot", A_DEFLONG, 0);
	addmess((method)lgrid_setBox, "setbox", A_DEFLONG,A_DEFLONG, 0);
	addmess((method)lgrid_scan, "scan", A_DEFLONG, 0);
	addmess((method)lgrid_goto, "goto", A_DEFLONG, 0);
	//addmess((method)lgrid_enter, "enter", A_DEFLONG, 0);
	addmess((method)lgrid_quantsize, "quantsize", A_DEFLONG, 0);
	addmess((method)lgrid_quantsize, "quant_size", A_DEFLONG, 0);
	addmess((method)lgrid_marksize, "mark_size", A_DEFLONG, 0);
	addmess((method)lgrid_setNumRows, "numrows", A_DEFLONG, 0);
	addmess((method)lgrid_quantize, "quantize", A_DEFLONG, 0);
	addmess((method)lgrid_front, "front", A_DEFLONG, 0);
	addmess((method)lgrid_show, "show", A_DEFLONG, 0);
	addmess((method)lgrid_solo, "solo", A_DEFLONG, 0);
	addmess((method)lgrid_hide, "hide", A_DEFLONG, 0);
	addmess((method)lgrid_scale, "scale", A_DEFFLOAT, 0);
	addmess((method)lgrid_muteEdits, "muteEdits", A_DEFLONG, 0);
	addmess((method)lgrid_undo, "undo", A_DEFLONG,A_DEFLONG, 0);
	addmess((method)lgrid_setundo, "setundo", 0);
	addmess((method)lgrid_size, "size", A_DEFLONG,A_DEFLONG, 0);
	addmess((method)lgrid_peek, "peek", A_DEFLONG,A_DEFLONG, 0);
	addmess((method)lgrid_dump, "dump", A_DEFLONG, 0);
	addmess((method)lgrid_click,"click",A_CANT,0);
	addmess((method)lgrid_update,"update",A_CANT,0);
	addmess((method)lgrid_psave,"psave",A_CANT,0);
	addmess((method)lgrid_bidle,"bidle",A_CANT,0);
	addmess((method)lgrid_assist, "assist",A_CANT,0);
	addmess((method)lgrid_read, "read",A_DEFSYM,0);
	addmess((method)lgrid_write, "write",A_GIMME,0);
	/* list object in the new object list */
	//finder_addclass("Lists","lgrid");



// new copyright poster


s = gensym("goslugsgo");
l = gensym("lobjectsbypqe");
if(s->s_thing != (struct object *)l){
post("Lobjects for Max © Regents of the University of California.");
s->s_thing = (struct object *)l;}
return 0;
}

/* example methods */

void lgrid_bang(Lgrid *x)  // output then step
{
	
	int i, value;
	outlet_bang(x->statusout);
	for( i = x->numRows-1; i >= 0;--i){
		value = x->theLines[i*WIDTH +x->cursor];
		if((x->show[i]) && (x->lastOut[i] != value))
			outlet_int(x->m_out[i],(long)value);	// send stored value out an outlet
		x->lastOut[i] = value;
	}
	++x->cursor;
	if(x->cursor >= x->width) x->cursor = 0;
	

	qelem_set(x->cursorQelem);
}

void lgrid_int(Lgrid *x, long n)
{
	
	int i,value;
	x->cursor = n;		
	x->theList[0].a_type = A_LONG;
	x->theList[0].a_w.w_long = x->cursor;
	
	outlet_anything(x->statusout,gensym("cursor"),1,x->theList);
				
	if((x->cursor > x->width) || (x->cursor < 0)) x->cursor = 0;
	for( i = x->numRows-1; i >= 0;--i){
		value = x->theLines[i*WIDTH+x->cursor];
		if((x->show[i]) && (x->lastOut[i] != value))
			outlet_int(x->m_out[i],(long)value);	// send stored value out an outlet
		x->lastOut[i] = value;
	}
	
	
	qelem_set(x->cursorQelem);
	
}

void lgrid_scan(Lgrid *x, long n)
{
	
	int i,value;
	x->cursor = n;					
	if((x->cursor > x->width) || (x->cursor < 0)) x->cursor = 0;
	for( i = x->numRows-1; i >= 0;--i){
		value = x->theLines[i*WIDTH+x->cursor];
		if(x->show[i]) 
			outlet_int(x->m_out[i],(long)value);	// send stored value out an outlet
		x->lastOut[i] = value;
	}
	++x->cursor;
	if(x->cursor >= x->width) x->cursor = 0;
	qelem_set(x->cursorQelem);
	
}

void lgrid_enter(Lgrid *x, long n)
{
	x->enteredvalue = n;
	if(x->enteredvalue < 1) x->enteredvalue = 1;
	if(x->enteredvalue > 127) x->enteredvalue = 127;
}

void lgrid_peek(Lgrid* x, long i, long n)
{
	Atom theAtom;
	theAtom.a_type = A_LONG;
	if((n >= 0) && ( n < x->width))
		if((i >= 0) && ( i< x->numRows))
		theAtom.a_w.w_long =(long)x->theLines[i*WIDTH+n];
		outlet_anything(x->statusout,gensym("data"),1,&theAtom);
}

void lgrid_dump(Lgrid * x, long n)
{
	int i, end;
	
	if(x->width > MAXSIZE)
		end = MAXSIZE;
	else
		end = x->width;
	if((n >= 0) && (n < x->numRows)){
	for(i = 0 ; i< end; ++i)
		x->theList[i].a_w.w_long = x->theLines[n *WIDTH+ i];
	}
	outlet_list(x->statusout, 0, end, x->theList);
}

// set value in each line at cursor

void lgrid_set(Lgrid *x,Symbol *s,int argc,Atom *argv)  // no output, does not move cursor
{
	int i;
	long value;
	if(argc > x->numRows) argc = x->numRows;
		for(i = 0; i<  argc; ++i){
		if(argv[i].a_type != A_LONG) continue;
		value =argv[i].a_w.w_long;
		if(value < 0 ) value = 0;
		if(value > 127) value = 127;
		x->theLines[i*WIDTH+x->cursor] = value;
		if(!x->muteEdits)
			outlet_int(x->m_out[i],value);
		}
		++x->cursor;
		if(x->cursor >= x->width) x->cursor = 0;
		qelem_set(x->redrawQelem);
}

// set values in current line starting at cursor

void lgrid_list(Lgrid *x,Symbol *s,int argc,Atom *argv) // outputs, does not move cursor
{
	int i,place;
	long value;
	
		
		for(i = 0; i<  argc; ++i){
		value = argv[i].a_w.w_long;
		if(value < 0 ) value = 0;
		if(value > 127) value = 127;
		place = x->cursor + i;
		if( place >= x->width) break;
		
		x->theLines[x->front *WIDTH+place] = value;
		
		}
		x->cursor = place;
		if (x->cursor >= x->width) x->cursor = 0;
		qelem_set(x->redrawQelem);
}
void lgrid_clear(Lgrid *x,Symbol *s,int argc,Atom *argv)  // clears front with undo or changes front and clears

{
	int i;
	long line,start, end;
	
	switch(argc){
		case 0:
		lgrid_setundo(x);
		for(i = 0; i < WIDTH; ++i)
			x->theLines[x->front *WIDTH+i] = 0;
		
		break;
		
		case 1:
		line = argv[0].a_w.w_long;
		if((line < 0) || (line > x->numRows-1)) return;
		x->front = line;
		lgrid_setundo(x);
		for(i = 0; i < WIDTH; ++i)
			x->theLines[line *WIDTH+i] = 0;
		break;
		
		case 2:
		line = argv[0].a_w.w_long;
		start = x->cursor;
		end = start + argv[1].a_w.w_long;
		if((line < 0) || (line > x->numRows-1)) return;
		x->front = line;
		lgrid_setundo(x);
		for(i = start; i < WIDTH; ++i){
			x->theLines[line *WIDTH+i] = 0;
			if(i >= end) break;
		}
		x->cursor = i;
		if(x->cursor >= x->width) x->cursor = 0;
		break;
		
		default:
		line = argv[0].a_w.w_long;
		start =argv[1].a_w.w_long;
		end = start + argv[2].a_w.w_long;
		if((line < 0) || (line > x->numRows-1)) return;
		x->front = line;
		lgrid_setundo(x);
		for(i = start; i < WIDTH; ++i){
			x->theLines[line *WIDTH+i] = 0;
			if(i >= end) break;
		}
		
		x->cursor = i;
		if(x->cursor >= x->width) x->cursor = 0;
		break;
	}
		qelem_set(x->redrawQelem);
} 


void lgrid_clearbox(Lgrid *x,Symbol *s,int argc,Atom *argv)  // clears front with undo or changes front and clears

{
	int i;
	long line,start, end;
	lgrid_setundo(x);
	line = x->front;
	switch(argc){
		case 0:
		start =(x->cursor/x->hStep) * x->hStep;
		end = start + x->hStep;
		
		for(i = start; i < x->width; ++i){
			x->theLines[line *WIDTH+i] = 0;
			if(i >= end) break;
		}
		x->cursor = i;
		if(x->cursor >= x->width) x->cursor = 0;
		
		break;
		
		
		
		case 1:
		start = argv[0].a_w.w_long * x->hStep;
		end = start +  x->hStep;
		
		for(i = start; i < x->width; ++i){
			x->theLines[line *WIDTH+i] = 0;
			if(i >= end) break;
		}
		x->cursor = i;
		if(x->cursor >= x->width) x->cursor = 0;
		break;
		
		
		case 2:
		start =argv[0].a_w.w_long * x->hStep;
		end = start + argv[1].a_w.w_long * x->hStep;
		
		for(i = start; i < x->width; ++i){
			x->theLines[line *WIDTH+i] = 0;
			if(i >= end) break;
		}
		x->cursor = i;
		if(x->cursor >= x->width) x->cursor = 0;
		break;
		
	}
		qelem_set(x->redrawQelem);
}


void lgrid_clearall(Lgrid *x,Symbol *s,int argc,Atom *argv) // clears a bunch with no undo

{
	int i,j;
	long line;
	
	if(argc){
		for(j = 0; j<  argc; ++j){
			line = argv[j].a_w.w_long;
			if((line < 0) || (line > x->numRows-1)) continue;
			for(i = 0; i < WIDTH; ++i)
				x->theLines[line *WIDTH+i] = 0;
		}
	} else {
		for(j = 0; j<  x->numRows; ++j){
			for(i = 0; i < WIDTH; ++i)
				x->theLines[j *WIDTH+i] = 0;
		}
	}
	x->undochannel = -1;
	x->cursor = 0;
	qelem_set(x->redrawQelem);
}


void lgrid_put(Lgrid *x,long channel, long place, long value)  // no output, does not move cursor

{
	if((channel > -1) && (channel < x->numRows))
		if((place > -1) && (place < x->width)){
			if(value < 0) value = 0;
			if(value > 127) value = 127;
			x->theLines[channel *WIDTH+place] = value;
			x->fixpoint[place] = 1;
			qelem_set(x->pointQelem);
		}
		
		
}


void lgrid_goto(Lgrid *x, long n)
{
	
	short savelock;
	x->cursor = n;					
	if(x->cursor > x->width) x->cursor = x->width-1;
	if (x->cursor < 0) x->cursor = 0;
	reset(x);
	qelem_set(x->cursorQelem);
	x->theList[0].a_type = A_LONG;
			x->theList[0].a_w.w_long = x->cursor;
			savelock = lockout_set(1);
			outlet_anything(x->statusout,gensym("goto"),1,x->theList);
			lockout_set(savelock);
	
}

void reset(Lgrid * x)
{
	int i;
	for(i = 0; i < x->numRows; ++i)
		x->lastOut[i] = -1;
}

void lgrid_quantsize(Lgrid *x, long n)
{
	
	if (n > 0)
	x->hStep = n;
	
	qelem_set(x->redrawQelem);
	
}

void lgrid_marksize(Lgrid *x, long n)
{
	
	if (n >= 0)
	x->marksize = n;
	
	qelem_set(x->redrawQelem);
	
}

void lgrid_setNumRows(Lgrid *x, long n)
{
	
	if (n > 0 && n <= 16)
	x->numRows = n;
	qelem_set(x->redrawQelem);
	
}

void lgrid_front(Lgrid *x, long n)
{
	
	if((n >= 0) && (n < x->numRows)){
		if(x->solo)
			x->show[x->front] = 0;
		x->front = n;	
		x->show[n] = 1;
		x->activerect.left = x->drawingrect.left;
		x->activerect.right = x->drawingrect.right;
		x->activerect.top = x->drawingrect.top + n * x->vStep;
		x->activerect.bottom = x->activerect.top + x->vStep;
	}	 
	qelem_set(x->redrawQelem);
	x->theList[0].a_type = A_LONG;
	x->theList[0].a_w.w_long = x->front;
	
	outlet_anything(x->statusout,gensym("front"),1,x->theList);
	
}

void lgrid_quantize(Lgrid *x, long n)
{
	x->quantize = n;
	
}

void lgrid_show(Lgrid *x, long n)
{
	int i;
	if(n > 15)
		for(i=0; i < x->numRows;++i )
			x->show[i] = 1;
	if((n >= 0) && (n < x->numRows)){
		x->show[n] = 1;	
		}
	qelem_set(x->redrawQelem);
}

void lgrid_solo(Lgrid *x, long n)
{
	int i;
	x->solo = n;
	if(n){
		for(i=0; i < x->numRows;++i )
			x->show[i] = 0;
		x->show[x->front] = 1;
		}
	else
	for(i=0; i < x->numRows;++i )
			x->show[i] = 1;
	qelem_set(x->redrawQelem);
}

void lgrid_hide(Lgrid *x, long n)
{
	if((n >= 0) && (n < x->numRows)){
		x->show[n] = 0;	
		}
	qelem_set(x->redrawQelem);
}

void lgrid_scale(Lgrid *x, float n)
{
	int i;
	short temp;
	lgrid_setundo(x);
	if(n > 0){
		for(i = 0; i < x->width; ++i){
			temp = n * x->theLines[x->front *WIDTH+i] ;
			if(temp > 127) temp = 127;
			x->theLines[x->front *WIDTH+i] = temp;
			}
		}
	qelem_set(x->redrawQelem);
}

void lgrid_muteEdits(Lgrid *x, long n)
{
	x->muteEdits = n;
}

void lgrid_undo(Lgrid *x, long n, long m)
{
	int i, start, end;
	unsigned char temp;
	start = n;
	if(m == 0) m= x->width;
	end = n + m;
	if( end > x->width) end = x->width;
	if(start < 0) start = 0;
	if((x->undochannel > -1) && (x->undochannel < x->numRows))
		for(i = start; i < end; ++i){
			temp = x->theLines[x->undochannel *WIDTH+i];
			x->theLines[x->undochannel *WIDTH+i]= x->undoLine[i] ;
			 x->undoLine[i] = temp;
		}
	 else post("Lgrid: can't undo");
	qelem_set(x->redrawQelem);
}

void lgrid_setundo(Lgrid *x)
{
	int i;
	for(i = 0; i < WIDTH; ++i)
		x->undoLine[i] = x->theLines[x->front *WIDTH+i];
	x->undochannel = x->front;
}

void lgrid_copy(Lgrid *x,Symbol *s,int argc,Atom *argv)
{
	int i,j, end;
	
	switch(argc) {
		case 0:
		for(i = 0; i < x->width; ++ i)
			x->clipboard[i] = x->theLines[x->front *WIDTH+i];
		x->clipend = x->width;
		break;
		
		case 1:
		
		j = x->cursor;
		end = argv[0].a_w.w_long;
			for( i = 0; i < end; ++i){
				if( j >= x->width) break;
				x->clipboard[i] = x->theLines[x->front *WIDTH+j++];
			}
		x->clipend = i;
		break;
		
		
		default:
		j = argv[0].a_w.w_long;
		if( j < 0) j = 0;
		end = argv[1].a_w.w_long;
		
			for( i = 0; i < end; ++i){
				if( j >= x->width) break;
				x->clipboard[i] = x->theLines[x->front *WIDTH+j++];
			}
		x->clipend = i;
		break;
	}
}

void lgrid_paste(Lgrid *x,Symbol *s,int argc,Atom *argv) // args are start point, length
{
	int i,j, end;
	lgrid_setundo(x);
	switch(argc) {
		case 0:
		j  = x->cursor;
		for(i = 0; i < x->clipend; ++ i){
			x->theLines[x->front *WIDTH+j++] = x->clipboard[i];
			if(j >= x->width) break;
			}
		break;
		
		case 1:
		j = argv[0].a_w.w_long;
		if(j < 0) j  = 0;
		for(i = 0; i < x->clipend; ++ i){
			if(j >= x->width) break;
			x->theLines[x->front *WIDTH+j++] = x->clipboard[i];
			}
		break;
		
		
		default:
		j = argv[0].a_w.w_long;
		if( j < 0) j = 0;
		end  = argv[1].a_w.w_long;
		if( end > x->clipend) end = x->clipend;
		if(end > 0)
			for( i = 0; i < end; ++i){
				if(j >= x->width) break;
				x->theLines[x->front *WIDTH+j++] = x->clipboard[i];
				}
		break;
	}
	x->cursor = j;
	if(x->cursor >= x->width)
		x->cursor = 0;
	qelem_set(x->redrawQelem);
}

void lgrid_move(Lgrid *x, long n, long wrap)  // move line left by n pixels
{
	int i;
	int buffer[WIDTH];
	if(n == 0) return;
	if(n > x->width) return;
	if(n < - x->width) return; 
	lgrid_setundo(x);
	for (i = 0; i < x->width; ++i)
	 buffer[i] = x->theLines[x->front *WIDTH+i];
	
	if(wrap){
		if(n < 0){
			for(i = 0 ; i < x->width + n; ++i)
				x->theLines[x->front *WIDTH+i] = buffer[i - n] ;
			for(; i < x->width; ++i)
				x->theLines[x->front *WIDTH+i] = buffer[i -n -x->width ] ;
		}
		else{
			for(i = 0 ; i < n; ++i)
				x->theLines[x->front *WIDTH+i] = buffer[x->width - n + i] ;
			for(; i < x->width; ++i)
				x->theLines[x->front *WIDTH+i] = buffer[(i- n)];
		}
		
	}
	else
	{
		if(n < 0){
			for(i = 0 ; i < x->width + n; ++i)
				x->theLines[x->front *WIDTH+i] = buffer[(i- n)] ;
			for(; i < x->width; ++i)
				x->theLines[x->front *WIDTH+i] = 0 ;
		}
		else{
			for(i = 0 ; i < n; ++i)
				x->theLines[x->front *WIDTH+i] = 0 ;
			for(; i < x->width; ++i)
				x->theLines[x->front *WIDTH+i] = buffer[(i- n)];
		}
		
	
	}
	
	qelem_set(x->redrawQelem);
}

void lgrid_size(Lgrid *x, long n, long m)
{
	
	if((m <= 0) || m > 512) m = 512;
	if(n <= 0) n = 128;
	if(n > WIDTH) n = WIDTH;
	
	box_size((t_box *) x, (short) n + 8, (short) m + 10);
	
}

void lgrid_redraw(Lgrid * x)  // this is in a qelem and calls update
{
	GrafPtr gp;
	gp = patcher_setport(x->lgrid_box.b_patcher);
	if(gp) {
		if( !box_nodraw((t_box *) x)){
			lgrid_update(x);
			box_enddraw((t_box *) x);
		}
		SetPort(gp);
	}
}

void lgrid_drawcursor(Lgrid * x)  // this is in a qelem 
{
	long oldcursor;
	GrafPtr gp;
	RGBColor saveColor;
	Rect cursorrect;
	RgnHandle old,boxclip;
	
	if(x->cursor == 0) 
		oldcursor = x->width - 1;
	else
		oldcursor = x->cursor - 1;
		
	cursorrect.top = x->zero - x->height - 4;
	cursorrect.bottom = x->zero - x->height - 1;
	cursorrect.left = x->cursor + x->edge;
	cursorrect.right = cursorrect.left + 1;
	
	gp = patcher_setport(x->lgrid_box.b_patcher);
	if(gp) {
		if( !box_nodraw((t_box *) x)){
			GetClip(old = NewRgn());
			RectRgn(boxclip = NewRgn(), &x->lgrid_box.b_rect);
			SectRgn(old,boxclip,boxclip);
			SetClip(boxclip);
			
			GetForeColor(&saveColor);
			RGBForeColor(&x->RGBCursor);
			EraseRect(&x->cursorpath);
			PaintRect(&cursorrect);
			SetClip(old);
			RGBForeColor(&saveColor);
			box_enddraw((t_box *) x);
		}
		SetPort(gp);
	}
}

void fillpointsperpixel(Lgrid * x, short numSteps)
{
	int i;
	float increment;
	increment = (float)numSteps / 128.0;
	x->valuetopixel[0] = 0;
	x->pixelvalues[0] = 0;
	for(i = 1;i< 128;++i)
	{
	x->valuetopixel[i] = 1 + increment * i;  // contains the pixel to draw for each value
	x->pixelvalues[i] = 127;   // initialize this
	}
	
	for(i = 1;i< numSteps;++i)
	{
	
	x->pixelvalues[i] = i / increment;// contains the value a pixel represents
	}
}

unsigned char pixeltovalue(Lgrid * x,short whichPixel)
{
	if(whichPixel < 1) return( 0);
	if(whichPixel > 127) return( 127);
	return(x->pixelvalues[whichPixel]);

}

void lgrid_update(Lgrid * x)  // this can be called from redraw or by an update request
{
	
	int i,j;
	short theTop;
	CGrafPtr thePort;
	GrafPtr p;
	Rect p_bounds,gw_bounds;
	GWorldPtr myGWorld;
	GDHandle theGDHandle;
	PixMapHandle thePixMap;
	BitMapPtr thePixMapPtr;
	BitMapPtr p_bits,gw_bits;
	Rect r,cursorrect;
	RgnHandle old,boxclip;
	RGBColor saveColor;
	GetForeColor(&saveColor);
	/************************* To use indexed colors
	for(i =0; i < x->numRows; ++i)
		box_getcolor((t_box *)x, (short)i,&x->colors[i]);
	*/
	x->edge = x->lgrid_box.b_rect.left + 6;  // these are only set here
	
	x->height = (x->lgrid_box.b_rect.bottom - x->lgrid_box.b_rect.top) - 8;
	x->vStep = x->height /  x->numRows;
	if(x->vStep < 1) x->vStep = 1;
	if(x->vStep > 131) x->vStep = 131;
	fillpointsperpixel(x,x->vStep-3);
	
	x->height = x->numRows * x->vStep;
	x->zero = 8 + x->lgrid_box.b_rect.top + x->height;
	
	x->width = x->lgrid_box.b_rect.right - x->lgrid_box.b_rect.left - 8;
	x->width &= 0x3FF;
	x->numSteps = x->width / x->hStep;
	x->width = x->numSteps * x->hStep;
	x->drawingrect.left = x->edge;
	x->drawingrect.right = x->edge + x->width + 1;
	x->drawingrect.bottom = x->zero + 1;
	x->drawingrect.top = x->zero - x->height;
	theTop = x->drawingrect.top;
	
	x->activerect.left = x->drawingrect.left;
	x->activerect.right = x->drawingrect.right - 1;
	x->activerect.top = x->drawingrect.top + x->front * x->vStep;
	x->activerect.bottom = x->activerect.top + x->vStep;
		
	x->cursorpath.left = x->edge;
	x->cursorpath.right = x->edge + x->width;
	x->cursorpath.top = x->lgrid_box.b_rect.top + 1;
	x->cursorpath.bottom = x->cursorpath.top + 6;
	
	x->paletterect.left = x->lgrid_box.b_rect.left;
	x->paletterect.right = x->edge - 1;
	x->paletterect.top = x->drawingrect.top;
	x->paletterect.bottom = x->paletterect.top + x->height;
	
	cursorrect.top = x->zero - x->height - 4;
	cursorrect.bottom = x->zero - x->height - 1;
	cursorrect.left = x->cursor + x->edge;
	cursorrect.right = cursorrect.left + 1;
	
	r = x->lgrid_box.b_rect;
	InsetRect(&r,1,1);
	
	
	GetPort(&p);  // wherever it thinks it's about to draw
	GetPortBounds(p, &p_bounds);
	NewGWorld(&myGWorld,0,&p_bounds,NIL,NIL,0);
	GetGWorld(&thePort, &theGDHandle);
	SetGWorld(myGWorld,NIL);
	
	thePixMap = GetGWorldPixMap(myGWorld);
	LockPixels(thePixMap);
	
	
	// draw
	GetPortBounds(myGWorld,&gw_bounds);
	EraseRect(&gw_bounds);
	RGBForeColor(&x->lightblue);
	FrameRect(&x->drawingrect);
	  // draw gridlines
	for(i = 0; i < x->numSteps; ++i){  // vertical
		j = i * x->hStep;
		if(x->marksize && ((j % x->marksize) == 0))
			RGBForeColor(&x->accent);
		else
			RGBForeColor(&x->lightblue);
		MoveTo(x->edge + j,theTop);
		LineTo(x->edge + j,x->zero);
	}
	for(j=0; j < x->numRows;++j) {  // horizontal
		RGBForeColor(&x->lightblue);
		MoveTo(x->edge,theTop + x->vStep * j);
		LineTo(x->drawingrect.right - 1,theTop + x->vStep * j);
		
		RGBForeColor(&x->colors[j]);  // draw palette
		r.left = x->edge - 5;
		r.right = x->edge - 1;
		r.top = x->zero - x->height + x->vStep * j;
		r.bottom = r.top + 6;
		if(j == x->front)
		FrameRect(&r);
		else
		PaintRect(&r);
		 // draw squares
		if(x->show[j]){
			r.bottom = (theTop + x->vStep * j) + x->vStep - 1;
			for(i=0; i < x->width;++i){
				if(x->theLines[j *WIDTH+i]){
					 r.top = r.bottom  -x->valuetopixel[x->theLines[j *WIDTH+i]] ;
					 r.left = x->edge + i;
					 r.right = r.left + 1;
					 PaintRect(&r);
				}
			}
		}
	}
	
		RGBForeColor(&x->RGBCursor);
		//EraseRect(&x->cursorpath);
		PaintRect(&cursorrect);
		
	// return to original Gworld
	SetGWorld(thePort, theGDHandle); 
	
	// set clipregion
	GetClip(old = NewRgn());
	r = x->lgrid_box.b_rect;
	InsetRect(&r,1,1);
	RectRgn(boxclip = NewRgn(), &r);
	SectRgn(old,boxclip,boxclip);
	SetClip(boxclip);
	
	// CopyBits
	
	thePixMapPtr = (BitMap *) GetPixBaseAddr(thePixMap);
	gw_bits = (BitMap *)GetPortBitMapForCopyBits(myGWorld);
	p_bits = (BitMap *)GetPortBitMapForCopyBits(p);
	CopyBits(gw_bits,p_bits,&x->lgrid_box.b_rect,&x->lgrid_box.b_rect,srcCopy  ,boxclip);
	UnlockPixels(thePixMap);
	
	
	SetClip(old);
	RGBForeColor(&saveColor);
	DisposeRgn(old);
	DisposeRgn(boxclip);
	DisposeGWorld(myGWorld);
	
}

void lgrid_drawpoint(Lgrid * x)  // this is in a qelem 
{
	GrafPtr gp;
	int i,j;
	short theTop;
	Rect r;
	RgnHandle old,boxclip;
	RGBColor saveColor;
	GetForeColor(&saveColor);
	gp = patcher_setport(x->lgrid_box.b_patcher);
	if(gp) {
		if( !box_nodraw((t_box *) x)){
			GetClip(old = NewRgn());
	RectRgn(boxclip = NewRgn(), &x->lgrid_box.b_rect);
	SectRgn(old,boxclip,boxclip);
	SetClip(boxclip);
	theTop = x->drawingrect.top;
	
	for(i = 0; i < x->width;++i)
		if(x->fixpoint[i]){
		x->fixpoint[i] = 0;
		r.left = x->edge + i;
		r.right = r.left + 1;
		
		for(j=0; j < x->numRows;++j) {
			if(x->show[j]){
				r.top = (theTop + x->vStep * j) + 1;
				r.bottom = (theTop + x->vStep * j) + x->vStep - 1;
				EraseRect(&r);
				if(i % x->hStep == 0){
					if(x->marksize && ((i % x->marksize) == 0))
						RGBForeColor(&x->accent);
					else
						RGBForeColor(&x->lightblue);  // draw gridlines
					MoveTo(x->edge + i, r.top);
					LineTo(x->edge + i, r.bottom);
				}
				if(x->theLines[j *WIDTH+i]){
					RGBForeColor(&x->colors[j]);
					r.top = r.bottom  - x->valuetopixel[x->theLines[j *WIDTH+i]];
					PaintRect(&r);
				}
			}
		}
	}
	SetClip(old);
	RGBForeColor(&saveColor);
	DisposeRgn(old);
	DisposeRgn(boxclip);
			box_enddraw((t_box *) x);
		}
		SetPort(gp);
	}
}

void lgrid_drawrow(Lgrid * x)  // this is in a qelem 
{
	GrafPtr gp;
	int i,j;
	short theTop;
	Rect r;
	RgnHandle old,boxclip;
	RGBColor saveColor;
	GetForeColor(&saveColor);
	gp = patcher_setport(x->lgrid_box.b_patcher);
	if(gp) {
		if( !box_nodraw((t_box *) x)){
			GetClip(old = NewRgn());
	RectRgn(boxclip = NewRgn(), &x->lgrid_box.b_rect);
	SectRgn(old,boxclip,boxclip);
	SetClip(boxclip);
	theTop = x->drawingrect.top;
	
	for(j = 0; j < x->numRows; ++j)
		if(x->fixrow[j] && x->show[j]){
			x->fixrow[j] = 0;
			for(i = 0; i < x->width;++i)
			{
				r.left = x->edge + i;
				r.right = r.left + 1;
				{
					r.top = (theTop + x->vStep * j) + 1;
					r.bottom = (theTop + x->vStep * j) + x->vStep - 1;
					EraseRect(&r);   // clear old data
					if(i % x->hStep == 0){
						if(x->marksize && ((i % x->marksize) == 0))
						RGBForeColor(&x->accent);
					else
						RGBForeColor(&x->lightblue);  // draw gridlines
						MoveTo(x->edge + i, r.top);
						LineTo(x->edge + i, r.bottom);
					}
					if(x->theLines[j *WIDTH+i]){
						RGBForeColor(&x->colors[j]);
						r.top = r.bottom  - x->valuetopixel[x->theLines[j *WIDTH+i]];
						
						PaintRect(&r);
					}
				}
			}
		}
	SetClip(old);
	RGBForeColor(&saveColor);
	DisposeRgn(old);
	DisposeRgn(boxclip);
			box_enddraw((t_box *) x);
		}
		SetPort(gp);
	}
}


void setEditRange(Lgrid * x, int where)
{
	int i;
	i = where;
	while(i){
		if(x->theLines[x->front *WIDTH+i - 1] != x->theLines[x->front *WIDTH+i ])
			break;
		--i;
	}
	x->editstart = i;
	i = where + 1;
	
	while(i < x->width){
		if(x->theLines[x->front *WIDTH+i - 1] != x->theLines[x->front *WIDTH+i ])
			break;
		++i;
	}
	x->editend = i;

}

void lgrid_click(Lgrid *x, Point pt, short modifiers)
{
				 //modifiers cmd 256  shift 512  lock 1024  option 2048  ctl 4096
	int i;
	short newfront;
	short savelock;
	x->lastclick = pt;
	x->initialclick = pt;
	x->lastindex=pt.h-x->edge;
	x->isInitialClick = 1;
	x->highres = 0;
	x->boxmode = 0;
	x->plotmode = 0;
	
	if(x->quantize){
		if ((modifiers & 512) || (modifiers & 1024)) // shift or caps lock
		x->boxmode = 0;
		else
		x->boxmode = 1;
	}
	else
		if ((modifiers & 512) || (modifiers & 1024)) // shift or caps lock
			x->boxmode = 1;
			
	if(PtInRect(pt,&x->drawingrect))  // select line
	{
	
	newfront = ((  pt.v - x->paletterect.top) / x->vStep);
		if(newfront >=x->numRows)
			newfront = 0;
	if(newfront != x->front){
			if(x->solo)
				x->show[x->front] = 0;
			x->front = newfront;
			x->show[x->front] = 1;
			x->theList[0].a_type = A_LONG;
			x->theList[0].a_w.w_long = x->front;
			savelock = lockout_set(1);
			outlet_anything(x->statusout,gensym("front"),1,x->theList);
			lockout_set(savelock);
	}
	x->activerect.top = x->drawingrect.top + ( x->front * x->vStep);
	x->activerect.bottom = x->activerect.top + x->vStep;
	
	if(modifiers & 256) {	// command click
		x->editmode = 1;
		if( x->theLines[x->front *WIDTH+pt.h - x->edge] )
			setEditRange(x, pt.h - x->edge);
		else{    // starting from 0
		
			if(x->boxmode){
				x->editstart = ((pt.h - x->edge) / x->hStep) * x->hStep;
				x->editend = ((pt.h - x->edge) / x->hStep + 1) * x->hStep -1; //(1 && x->drawvalue);
			}
			else {
				x->editstart = pt.h - x->edge;
				x->editend = pt.h - x->edge+1;
			}
		}
		if(modifiers & 2048 ){  // + option
			x->highres = 1;
			x->initialvalue = x->theLines[x->front *WIDTH+pt.h - x->edge];
			x->editbaseline = x->activerect.bottom  - x->valuetopixel[x->initialvalue];
		}
	}
	else
		x->editmode = 0;
	
	if(modifiers & 4096){  // control
		if(pt.v == x->activerect.bottom )
			x->drawvalue = 0;
		else
			x->drawvalue = pixeltovalue(x,x->activerect.bottom - pt.v - 1);
		if(modifiers & 2048 ) // + option
			x->plotmode = 1;
	}
	else {
		if(modifiers & 2048 ) //option only
			x->drawvalue = 0;
		else					// plain drawing
			if( x->theLines[x->front *WIDTH+pt.h - x->edge])  // over data, erase
				x->drawvalue = 0;
			else{
				if(pt.v == x->activerect.bottom + 1)
					x->drawvalue = 0;
				else
					x->drawvalue = pixeltovalue(x,x->activerect.bottom - pt.v - 1); // over 0, something is wanted
				
			}
		}
	
	if(x->drawvalue > 127 ) x->drawvalue = 127;
		
	
	
	
	for(i = 0; i < WIDTH; ++i)
		x->undoLine[i] = x->theLines[x->front *WIDTH+i];
	x->undochannel = x->front;
	
	
	wind_drag((method) lgrid_dragfun, x, pt);
	lgrid_dragfun(x, pt, 1);
	}
	else
	{
		if(PtInRect(pt,&x->paletterect)){  // select front
			if(x->solo)
				x->show[x->front] = 0;
			x->front = ((  pt.v - x->paletterect.top) / x->vStep);
			if(x->front >= x->numRows)
				x->front = 0;
			if(x->solo)
				x->show[x->front] = 0;
			x->show[x->front] = 1;
			qelem_set(x->redrawQelem);
			x->theList[0].a_type = A_LONG;
			x->theList[0].a_w.w_long = x->front;
			savelock = lockout_set(1);
			outlet_anything(x->statusout,gensym("front"),1,x->theList);
			lockout_set(savelock);
		}	 
		
		if(PtInRect(pt,&x->cursorpath)){  // move cursor
			x->cursor = pt.h - x->edge;
			qelem_set(x->cursorQelem);
			x->theList[0].a_type = A_LONG;
			x->theList[0].a_w.w_long = x->cursor;
			savelock = lockout_set(1);
			outlet_anything(x->statusout,gensym("goto"),1,x->theList);
			lockout_set(savelock);
		}
	
	}
	

}


void lgrid_dragfun(Lgrid *x, Point pt, short button)
{
	short savelock;
	short index;
	short start, stop;
	
	int i;
	GrafPtr gp;
	Rect r;
	RGBColor saveColor;
	
	if(x->plotmode){
		x->drawvalue = pixeltovalue(x,x->activerect.bottom - pt.v - 1);
		if(x->drawvalue > 127) x->drawvalue = 127;
		if(x->drawvalue < 1) x->drawvalue = 0;
	}
	
	if(x->editmode){
		if(x->highres){
			x->drawvalue = x->initialvalue + x->editbaseline - pt.v -1;
		}else{
			x->drawvalue = pixeltovalue(x,x->activerect.bottom - pt.v - 1);
		}
		if(x->drawvalue < 1) x->drawvalue = 0;
		if(x->drawvalue > 127) x->drawvalue = 127;
		start = x->editstart;
		stop = x->editend;
	}
	else
	{
	
		index = pt.h - x->edge;
		if(index < 0) index = 0;
		if(index > x->width) index = x->width;
		
		if(x->lastindex > index) {  // left of initial click
			
			if(x->boxmode){
				start = (index / x->hStep) * x->hStep;
				stop = (x->lastindex / x->hStep + 1) * x->hStep -(1 && x->drawvalue);
			}
			else {
				start = index;
				stop = x->lastindex;
			}
			//if(index < x->gestureleft) x->gestureleft = index;
		}else{					// left to right
			
			if(x->boxmode){
				start = (x->lastindex / x->hStep) * x->hStep;
				stop = (index / x->hStep + 1) * x->hStep - (1 && x->drawvalue); // if clearing, clear to edge
				
			}
			else {
				start = x->lastindex;
				stop = index + 1;
			}
			//if(index > x->gestureright) x->gestureright = index;
		}
		if(stop >= x->width) stop = x->width;
		x->lastindex = index;
	}
	
	
	
	gp = patcher_setport(x->lgrid_box.b_patcher);
	if(gp) {
		if( !box_nodraw((t_box *) x)){
			
			GetForeColor(&saveColor);
			RGBForeColor(&x->colors[x->front]);
			r.bottom = x->activerect.bottom - 1; 
			for(i = start; i < stop; ++i){
				r.top = x->activerect.top + 1;
				r.left = x->edge + i;
				r.right = r.left + 1;
				EraseRect(&r);
				if( i % x->hStep == 0){
					if(x->marksize && ((i % x->marksize) == 0))
						RGBForeColor(&x->accent);
					else
						RGBForeColor(&x->lightblue);  // draw gridlines
					PaintRect(&r);
					RGBForeColor(&x->colors[x->front]);
				
				}
				x->theLines[x->front *WIDTH+i] = x->drawvalue;
				
				if(x->drawvalue){
		 			r.top = r.bottom - x->valuetopixel[x->drawvalue];
		 			PaintRect(&r);
		 		}
		 	}
			if(x->boxmode && x->drawvalue) {   // clear end of box
				x->theLines[x->front *WIDTH+i] = 0;
				r.top = x->activerect.top + 1;
				r.left = x->edge + i;
				r.right = r.left + 1;
				EraseRect(&r);
				
				
			}
			RGBForeColor(&saveColor);
			box_enddraw((t_box *) x);
		}
		SetPort(gp);
		if(button == 0){
			 qelem_set(x->redrawQelem);
			 reset(x);
		}
	}
	x->oldstart = start;
	x->oldstop = stop;
	
	x->lastclick = pt;
	
	
	if((!x->muteEdits && x->isInitialClick) || (!x->muteEdits && x->editmode)){
	savelock = lockout_set(1);
	outlet_int(x->m_out[x->front], (long) x->drawvalue);
	lockout_set(savelock);
	}
	x->isInitialClick = 0;
}

void lgrid_plot(Lgrid *x, long n)
{
	if(x->quantize){
	lgrid_setBox(x, x->cursor / x->hStep,n);
	x->cursor = (x->cursor / x->hStep) * x->hStep + x->hStep;
	if(x->cursor >= x->width) x->cursor = 0;
	}
	else{
	lgrid_put(x, x->front,x->cursor,n);
	++ x->cursor;
	if(x->cursor >= x->width) x->cursor = 0;
	}
	
	/*x->drawvalue = n;
	if( x->drawvalue < 0) x->drawvalue = 0;
	if(x->drawvalue > 127) x->drawvalue = 127;
	
	x->plotpt.h = x->edge + x->cursor;
	x->plotpt.v = x->valuetopixel[x->drawvalue];  // dosen't matter;
	qelem_set(x->plotQelem);  // calls drawrow*/
	
}

void lgrid_plotfun(Lgrid * x)
{
	x->initialclick = x->plotpt;
	x->lastclick = x->plotpt;
	x->lastindex = x->plotpt.h-x->edge;
	x->isInitialClick = 1;
	x->editmode = 0;
	x->plotmode = 1;
	if(x->quantize)
		x->boxmode = 1; 
	else
		x->boxmode = 0;
	lgrid_dragfun(x, x->plotpt, 1);
	
	if(x->boxmode)
	x->cursor = (x->cursor / x->hStep +1) * x->hStep;
	else
	++ x->cursor;					
	if(x->cursor >= x->width) x->cursor = 0;
	
	lgrid_drawcursor( x);
}

void lgrid_setBox(Lgrid *x, long n,long value)
{
	int i;
	short start, stop;
	if( n < 0) n = 0;
	if(n >= x->width / x->hStep) n = 0;
	if(value < 0) value = 0;
	if(value > 127) value = 127;
	start = n * x->hStep;
	stop = start + x->hStep-1;
	for(i = start; i  < stop;++i)
		x->theLines[x->front *WIDTH+i] = value;
	x->theLines[x->front *WIDTH+i] = 0; //clear the space
	x->fixrow[x->front] = 1;
	qelem_set(x->rowQelem);  // calls drawrow
	
}

void lgrid_setBoxFun(Lgrid * x)  // nolonger used??
{
	x->initialclick = x->plotpt;
	x->lastclick = x->plotpt;
	x->isInitialClick = 1;
	x->editmode = 0;
	x->boxmode = 1; 
	lgrid_dragfun(x, x->plotpt, 1);
	
}

void lgrid_bidle(Lgrid *x)
{
	
	if(box_ownerlocked((t_box *)x)) wind_setcursor(6);
	
}

void lgrid_psave(Lgrid * x,void * dest)
{
	short hidden;
	Rect *r;
	
	hidden = x->lgrid_box.b_hidden;
	r = &x->lgrid_box.b_rect;
	if(hidden) {
		binbuf_vinsert(dest,"sssslllllll",gensym("#P"),gensym("hidden"),
			gensym("user"),gensym("Lgrid"), (long)(r->left),(long)(r->top),
			(long)(r->right),(long)(r->bottom),(long)x->hStep,(long)x->numRows,(long)x->marksize);
	} else {
		binbuf_vinsert(dest,"ssslllllll",gensym("#P"),
			gensym("user"),gensym("Lgrid"), (long)(r->left),(long)(r->top),
			(long)(r->right),(long)(r->bottom),(long)x->hStep,(long)x->numRows,(long)x->marksize);
	}
	
}

void lgrid_assist(Lgrid *x,void *b,long msg,long arg,char *dst)

{
	char outstring[16];
	outstring[0] = 'O';
	outstring[1] = 'u';
	outstring[2] = 't';
	outstring[3] = 'p';
	outstring[4] = 'u';
	outstring[5] = 't';
	outstring[6] = ' ';
	outstring[7] = '0';
	outstring[8] = 0;
	outstring[9] = 0;
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"int to get mapped values");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET){
			if(arg == 16)
				strcpy(dst,"Status");
			else{
				if(arg < 10)
					outstring[7] = arg + 48;
				else{
					outstring[7] = '1';
					outstring[8] = arg % 10 + 48;
				}
				strcpy(dst,outstring);
			}
		}
	}
}

/* instance creation routine */

void *lgrid_new(t_symbol *sym, short argc, t_atom *argv) // from file-- data saved by psave
{
	Lgrid *x;
	void *p; // the patcher
	short left, top, right, bottom;
	short flags;
	int i,j;
	//GrafPtr gp;
	
	x = newobject(lgrid_class);		// get memory for a new object & initialize
	x->theLines = getbytes(16 * WIDTH * sizeof(unsigned char));
	p = argv[0].a_w.w_obj;
	left = argv[1].a_w.w_long;
	top = argv[2].a_w.w_long;
	right = argv[3].a_w.w_long;
	bottom = argv[4].a_w.w_long;
	x->hStep = argv[5].a_w.w_long;
	if(argc > 6)
		x->numRows = argv[6].a_w.w_long;
	else
		x->numRows = 16;
		if(argc > 7)
		x->marksize = argv[7].a_w.w_long;
	else
		x->marksize = 0;
	x->statusout = outlet_new(x,0L);
	for(i = 15; i >= 0; --i)
		x->m_out[i] = outlet_new(x,0L);
	x->isInitialClick = 0;
	
	x->boxmode = 0;
	x->highres = 0;
	x->quantize = 0;
	x->editmode = 0;
	x->undochannel = -1;
	x->front = 0;
	x->cursor = 0;
	x->muteEdits = 0;
	x->solo = 0;
	x->clipend = 0;
	x->enteredvalue = 100;
	
	 lgrid_setcolors(x);  // I like my colors better than the index set
	
	for(i = 0; i < 16; ++i){
		x->show[i] = 1;
		x->lastOut[i] = -1;
		x->fixrow[i] = 0;
		}
	
	
	for(i = 0; i < WIDTH; ++i){
		x->clipboard[i] = 0;
		x->fixpoint[i] = 0;
		for(j = 0; j < 16; ++j)
			x->theLines[j *WIDTH+i] = 0;  // test pattern
	}
	
	for(i = 0; i < MAXSIZE; ++i)
		x->theList[i].a_type = A_LONG;
	
	flags = F_DRAWFIRSTIN | F_GROWBOTH | F_SAVVY;
	
	box_new((t_box *) x,p, flags, left,top,right,bottom);
	x->redrawQelem = qelem_new(x, (method) lgrid_redraw);
	x->cursorQelem = qelem_new(x, (method) lgrid_drawcursor);
	x->plotQelem = qelem_new(x, (method) lgrid_plotfun);
	x->setBoxQelem = qelem_new(x, (method) lgrid_setBoxFun);
	x->pointQelem = qelem_new(x, (method) lgrid_drawpoint);
	x->rowQelem = qelem_new(x, (method) lgrid_drawrow);
	x->lgrid_box.b_firstin = (void *) x;
	box_ready((t_box *) x);
	
	
	return (x);							// return newly created object to caller
}

void *newFromMenu(t_patcher *p, long left, long top, long font)
{
	int i;
	t_atom argv[20];
	argv[0].a_type = A_OBJ;
	argv[0].a_w.w_obj = (t_object *) p;
	for(i=0; i < 20; ++i)
		argv[i].a_type  = A_LONG;
	argv[1].a_w.w_long = left;
	argv[2].a_w.w_long = top;
	argv[3].a_w.w_long = left + 128;
	argv[4].a_w.w_long = top + 64;
	argv[5].a_w.w_long = 24;  // hStep
	argv[6].a_w.w_long = 16;  // numRows
	argv[7].a_w.w_long = 0;  // marksize
	return lgrid_new(0,8,argv);
}
	
void lgrid_free(Lgrid * x)
{
	  freebytes(x->theLines,16 * WIDTH * sizeof(unsigned char));
	qelem_free(x->redrawQelem);
	qelem_free(x->cursorQelem);
	qelem_free(x->plotQelem);
	qelem_free(x->setBoxQelem);
	qelem_free(x->pointQelem);
	qelem_free(x->rowQelem);
	box_free((t_box *)x);
	
}

void lgrid_setcolors(Lgrid * x)
{
		x->colors[0].red = 0;  // black
		x->colors[0].green = 0;
		x->colors[0].blue = 0;
		
		x->colors[1].red = 134 << 8;  // brown
		x->colors[1].green = 5 << 8;
		x->colors[1].blue = 0;
		
		x->colors[2].red = 255 << 8;  // red
		x->colors[2].green = 0;
		x->colors[2].blue = 0;
		
		x->colors[3].red = 255<< 8; // orange
		x->colors[3].green = 145<< 8;
		x->colors[3].blue = 64 << 8;
		
		x->colors[4].red = 255 << 8;  //yellow
		x->colors[4].green = 255 << 8;
		x->colors[4].blue = 0;
		
		x->colors[5].red = 0;  // green
		x->colors[5].green = 255 << 8;
		x->colors[5].blue = 0;
		
		x->colors[6].red = 0;  // blue
		x->colors[6].green = 0;
		x->colors[6].blue = 255 << 8;
		
		x->colors[7].red = 255 << 8; // violet
		x->colors[7].green = 0;
		x->colors[7].blue = 255<< 8;
		
		x->colors[8].red = 132 << 8;  // gray
		x->colors[8].green = 116 << 8;
		x->colors[8].blue = 116 << 8;
		
		x->colors[9].red = 191 << 8; // lt Brn
		x->colors[9].green = 109<< 8;
		x->colors[9].blue =  23 << 8;
		
		x->colors[10].red = 200<< 8; //pink
		x->colors[10].green = 64 << 8;
		x->colors[10].blue = 40 << 8;
		
		
		x->colors[11].red = 239 << 8;  // lt orange
		x->colors[11].green = 163<< 8 << 8;
		x->colors[11].blue = 113<< 8;
		
		
		x->colors[12].red = 204 << 8; //lt yellow
		x->colors[12].green = 205<< 8;
		x->colors[12].blue = 25 << 8;
		
		
		x->colors[13].red = 113 << 8; //lt green
		x->colors[13].green = 209 << 8;
		x->colors[13].blue = 25 << 8;
		
		
		x->colors[14].red = 29 << 8;  // lt blue
		x->colors[14].green = 95 << 8;
		x->colors[14].blue = 214 << 8;
		
		
		x->colors[15].red = 180 << 8;  // l violet
		x->colors[15].green = 22 << 8;
		x->colors[15].blue = 180 << 8;
		
		x->RGBCursor.red = 255 << 8 ;
		x->RGBCursor.green = 0;
		x->RGBCursor.blue = 0;
		
		x->lightblue.red = 200 << 8 ;
		x->lightblue.green = 200 << 8;
		x->lightblue.blue = 220 << 8;
		
		x->accent.red = 100 << 8 ;
		x->accent.green = 100 << 8;
		x->accent.blue = 100 << 8;

}

void lgrid_read(Lgrid *x,Symbol *s)  // file operations must be deferred
{

	
	defer_low(x,(method)lgrid_readfile,s,0,NULL);
	
}

void lgrid_write(Lgrid *x,Symbol *s,int argc,Atom *argv)  // file operations must be deferred
{
	
	
	defer_low(x,(method)lgrid_writefile,s,argc, argv);
	
}

void lgrid_readfile(Lgrid *x,Symbol *s,int argc,Atom *argv)  // User Interface lifted from coll
{

	long types[2];
	short refNum;
	int prevLock;
	int j;
	long i;
	long type;
	long typefound;
	char name[256];
	//Str255 filename;
	//float *bigArray;
	//void *voidPtr;
	
	long incount;
	
	
	name[0] = 0;
	types[0]='maxb';

	if (s != gensym(""))   // "" is an empty symbol
	{
		if (locatefile_extended (s->s_name, &x->pathID, &typefound,types,1))
			{
			ouchstring("Couldn't find %s. Please locate the file, or an alternate.",
				s->s_name);
			x->fn=gensym("");
			s = gensym("");
			}
		else
			{
			x->fn = s;
			i = 0;
			while(s->s_name[i])
				{
				name[i] = s->s_name[i];
				++i;
				}
			name[i] = 0;
			}
	}
	
	if(s == gensym(""))
	{
		if (open_dialog(name, &x->pathID, &type, types, 1)) 
		{
		
		return;
		}
		else
			x->fn = gensym(name);
			
			
	}
	post("Lgrid: reading %s",x->fn->s_name);

	if(path_openfile(name,x->pathID,&refNum,READ_PERM))
		{
		ouchstring("Error Opening %s",x->fn->s_name);
		
		return;
		}
	
	prevLock = lockout_set(1);  //Stop interrupts
	//HLock(x->aHandle);          // and lock memory
	//voidPtr = *x->aHandle;		// fool the compiler
	//bigArray = voidPtr;
	incount = 16 * WIDTH * sizeof(unsigned char);  // read no more than incount bytes
	FSRead(refNum,&incount,x->theLines);
	FSClose(refNum);
	//HUnlock(x->aHandle);
	lockout_set(prevLock);
	
	post("Larray: %ld bytes read",incount); // incount is now number of bytes read
	for(i = 0; i > WIDTH; ++i){
		for(j=0;j<x->numRows;++j){
			if(x->theLines[j *WIDTH+i] > x->height) x->theLines[j *WIDTH+i] = x->height;
		}
	}
	qelem_set(x->redrawQelem);
	
}


void lgrid_writefile(Lgrid *x,Symbol *dummy,int argc,Atom *argv)
{

	OSType type,creator;
	short refNum;
	int prevLock;
	long i;
	Symbol *s;
	char name[256];
	//Str255 filename;
	//float *bigArray;
	//void *voidPtr;
	long outcount;
	long result;
	long writesize;
	
	name[0] = 0;
	type ='maxb';
	creator = '????';
	writesize = x->ArraySize;
	s = gensym("");
	if(argc)  // user may enter length, name or name, length
		{
			if(argv[0].a_type == A_LONG)   // just a length specifier
				writesize = argv[0].a_w.w_long;
			if(argv[0].a_type == A_SYM)		// a file name
				s = argv[0].a_w.w_sym;
		}
	if(argc > 1)
		{
			if(argv[1].a_type == A_LONG)  // a length specifier
				writesize = argv[1].a_w.w_long;
			if(argv[1].a_type == A_SYM)  // again file name
				s = argv[1].a_w.w_sym;
		}
	
	if(s == gensym(""))  // no name specified
		{
		if (saveas_dialog(name, &x->pathID, 0L)) // user bailed
			{
		
			return;
			}
		else
			x->fn = gensym(name);
			
		}
	else   // name was specified
		{
		x->fn = s;
		i = 0;
		while(s->s_name[i])
			{
			name[i] = s->s_name[i];
			++i;
			}
		name[i] = 0;
		}
		
	prevLock = lockout_set(1);  // turn off interrupts
/*	HLock(x->aHandle);			// and lock memory
	voidPtr = *x->aHandle;
	bigArray = voidPtr;
	
		if(writesize % 2 == 0)
		outcount = (writesize / 2) * sizeof(short);
		else
		outcount = (writesize / 2 +1)*sizeof(short);*/
	
	outcount = 16 * WIDTH * sizeof(unsigned char);
	result = path_createfile(name,x->pathID,type,&refNum);
	if(result & (result != dupFNErr))  // does not warn if specified file exists
		{
		post("Could not create %s! error#  %ld",x->fn->s_name,result);
		goto exitHere;  				// these error handlers are not tested
		}
	post("Lgrid: writing %s",x->fn->s_name);
	
	result = SetEOF(refNum,outcount);  	// Not absolutely required, but a good idea
										// when file is used over with differing sizes
	if(result)  
		{
		post("Could not set length of %s! error#  %ld",x->fn->s_name,result);
		goto CloseIt;
		}
	result = FSWrite(refNum,&outcount,x->theLines);
	if(result)  
		{
		post("Could not write to %s! error#  %ld",x->fn->s_name,result);
		}
CloseIt:
	result = FSClose(refNum);
	if(result)  
		{
		post("Could not close %s! error#  %ld",x->fn->s_name,result);
		goto exitHere;
		}
	
	post("Larray: %ld bytes saved",outcount);
	
exitHere:	
	//HUnlock(x->aHandle);
	lockout_set(prevLock);
	
	
}