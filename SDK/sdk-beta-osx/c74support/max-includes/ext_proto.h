/* ext_proto.h -- prototypes for MAX external methods */
/* copyright 1996 Opcode/IRCAM */

#ifdef MAC_VERSION
#ifdef __MOTO__
#include <Types.h>
#include <Quickdraw.h>
#endif __MOTO__
#endif MAC_VERSION

void setup(t_messlist **ident, method makefun, method freefun, short size,
	method menufun, short type, ...);
void addmess(method f, char *s, short type, ...);
void addbang(method f);
void addint(method f);
void addfloat(method f);
void addinx(method f, short n);
void addftx(method f, short n);
void *newobject(void *maxclass);
char *getbytes(short size);
void freebytes(void *b, short size);
void intin(void *x, short n);
void floatin(void *x, short n);
void *outlet_new(void *x, char *s);
void *bangout(void *x);
void *intout(void *x);
void *floatout(void *x);
void *listout(void *x);
void *outlet_bang(void *o);
void *outlet_int(void *o, long n);
void *outlet_float(void *o, double f);
void *outlet_list(void *o, struct symbol *s, short ac, struct atom *av);
void *outlet_anything(void *o, struct symbol *s, short ac, struct atom *av);
struct symbol *gensym(char *s);
void *clock_new(void *obj, method fn);
void clock_set(void *obj,long when);
void clock_delay(struct clock *x, long n);
void clock_unset(void *x);
void freeobject(t_object *op);
long gettime(void);
void *qelem_new(void *obj, method fn);
void qelem_set(void *q);
void qelem_unset(void *q);
void qelem_free(struct qelem *x);
char **newhandle(long size);
short growhandle(void *h, long size);
void disposhandle(char **h);
void post(char *fmt, ...);
struct wind *wind_new(void *assoc, short x1, short y1, short x2, short y2, short flags);
void wind_vis(struct wind *x);
void wind_invis(struct wind *x);
GrafPtr wind_setport(struct wind *x);
void wind_settitle(struct wind *x, char *s, short brackets);
void wind_dirty(struct wind *x);
void wind_setsmax(struct wind *x, short a, short b);
void wind_setsval(struct wind *x, short a, short b);
void wind_drag(method fun, void *arg, Point pt);
short wind_defaultscroll(struct wind *x, Point pt, short pageSize);
short defvolume(void);
void *binbuf_new(void);
void binbuf_vinsert(void *x, char *fmt, ...);
void binbuf_insert(struct binbuf *x, t_symbol *s, short argc, t_atom *argv);
void *binbuf_eval(struct binbuf *x, short ac, t_atom *av, void *to);
short binbuf_getatom(struct binbuf *x, long *p1, long *p2, t_atom *ap);
short binbuf_text(struct binbuf *x, char **shit, long n);
short binbuf_totext(struct binbuf *x, char **shit, long *sizep);

#ifndef WIN_VERSION
int sprintf(char *, const char *, ...);
int sscanf(const char *, const char *, ...);
#endif WIN_VERSION

void t_cpytext(void);
void *typedmess(struct object *op, struct symbol *msg, short argc, struct atom *argp);
void binbuf_set(struct binbuf *x, t_symbol *s, short argc, t_atom *argv);
void binbuf_append(struct binbuf *x, t_symbol *s, short argc, t_atom *argv);
GrafPtr patcher_setport(struct patcher *p);
void box_new(struct box *b, struct patcher *patcher, short flags, short x1, short y1, short x2, short y2);
void box_free(struct box *b);
void box_ready(struct box *b);
void *patchbox(void *p, method fn, short ac, t_atom *av, short defwidth);
short lockout_set(short);
void patcher_dirty(struct patcher *p);
void patcher_selectbox(struct patcher *p, struct box *b);
void patcher_deselectbox(struct patcher *p, struct box *b);
void drawstr(char *s);
void binbuf_savebox(struct binbuf *x, void *w, t_symbol *what, short d1, short d2,
	 short d3, long d4, short hidden, short user);
void *textbox(void *p, method fn, short ac, struct atom *av);
short box_ownerlocked(struct box *b);
void box_size(void *b, short x, short y);
short rescopy(long type,short id);
void inlet_new(void *x, char *s);
void wind_noworrymove(short way);
void wind_setundo(char *str, short enable);
long getschedtime(void);
long getexttime(void);
void clock_xdelay(void *x, long n);
void clock_xset(void *x, long n);
void clock_xunset(void *x);
short clock_getextfmt(void);
long isr(void);
short maxversion(void);
short table_get(t_symbol *s, long ***hp, long *sp);
short table_dirty(t_symbol *s);
short binbuf_read(struct binbuf *x, char *name, short volume, short binary);
short binbuf_write(struct binbuf *x, char *fn, short vol, short binary);
void ouchstring(char *s, ...);
char **palette(void);
short ispatcher(t_object *x);
short isnewex(struct object *x);
short newex_knows(struct box *x, Symbol *selector);
void *expr_new(short argc, t_atom *argv, t_atom *result);
short expr_eval(struct expr *x, short argc, t_atom *argv, t_atom *result);
void error(char *, ...);
void preset_store(char *fmt, ... /*struct b100 arg1 */);
void preset_set(Object *obj, long val);
void preset_int(void *x,long n);
void colorinfo(struct colorinfo *r);
void finder_addclass(char *category, char *classString);
void *newinstance(t_symbol *s, short argc, t_atom *argv);
method getfn(struct object *op, struct symbol *msg);
method egetfn(struct object *op, struct symbol *msg);
method zgetfn(struct object *op, struct symbol *msg);
void *ed_new(t_object *assoc);
void ed_vis(struct ed *x);
void ed_settext(struct ed *x, Handle text, long len);
short readatom(char *outstr, char **shit, long *n, long e, t_atom *ap);
short open_dialog(char *name, short *volptr, long *typeptr, long *types, short ntypes);
short saveas_dialog(char *name, short *volptr, short *binptr);
short locatefile(char *name, short *outvol, short *binflag);
void *inlet4(void *x, void *w, char *s, char *s1);
void inlet_to(struct inlet *x, void *w);
void patcher_okclose(struct patcher *p, void *x);
short advise(char *s, ...);
void qelem_front(struct qelem *x);
void evnum_incr(void);
long evnum_get(void);
void patcher_eachdo(eachdomethod fun, void *arg);
short box_nodraw(struct box *b);
void wind_setcursor(short which);
struct num *num_new(short top, short left, short bottom, short right, method draw,
	longmethod inc, long flags, long min, long max, long val, short font, short fsize);
void num_draw(struct num *x);
void num_hilite(struct num *x, short way);
long num_test(struct num *x, Point pt);
void num_track(struct num *x, Point pt, method track, void *arg);
void num_setvalue(struct num *x, long newval, short redraw);
void *proxy_new(void *x,long id,long *stuffloc);
void *gwind_new(t_object *assoc, Symbol *s, short style, 
	short left, short top, short bottom, short right);
void gwind_offscreen(struct gwind *x);
void *gwind_setport(struct gwind *x);
struct gwind *gwind_get(Symbol *s);
struct offscreen *off_new(GrafPtr port);
void off_free(struct offscreen *o);
struct sprite *sprite_new(void *assoc, struct offscreen *owner, long number, Rect *rect, method proc);
void sprite_moveto(struct sprite *x, short h, short v);
void off_maxrect(struct offscreen *o,Rect *a,Rect *b,Rect *maxRect);
void sprite_move(struct sprite *x, short dh, short dv);
void sprite_redraw(struct sprite *x, short dh, short dv, short frame, short next);
void off_copy(struct offscreen *o);
void sprite_draw(struct sprite *x);
void sprite_erase(struct sprite *x);
void sprite_rect(struct sprite *x, Rect *newRect, short frame, short next);
void off_tooff(struct offscreen *o,PixMapHandle pm,Rect *src,Rect *dst);
void off_size(struct offscreen *o);
void off_copyrect(struct offscreen *o,Rect *r);
void alias(char *name);
short locatefiletype(char *name, short *outvol, long filetype, long creator);
void midiinfo(struct midiinforec *mr);
long serialno(void);
void postatom(t_atom *ap);
void sprite_newpriority(struct sprite *x, long newpri);
void assist_string(short id,long msg,long arg, short firstin, short firstout,char *dst,...);
void box_erase(struct box *b);
void *defer(void *ob,method fn,struct symbol *sym,short argc,struct atom *argv);
void *defer_low(void *ob,method fn,struct symbol *sym,short argc,struct atom *argv);
void *callback_new(void *assoc, ProcPtr proc, long refCon, short offset, ProcPtr *callfun);
short locatefilelist(char *name, short *outvol, long *outtype,
	long *filetypelist, short numtypes);
short resnamecopy(long type, char *name);
void binbuf_delete(struct binbuf *x, long fromType, long toType, long fromData, long toData);
short force_install(char *classname);
void sched_suspend(void);
void sched_resume(void);
short readtohandle(char *name, short volume, char ***h, long *sizep);
void *fileload(char *name,short vol);
void *intload(char *name, short volume, t_symbol *s, short ac, t_atom *av, short couldedit);
short outlet_add(struct outlet *x, struct inlet *ip);
void outlet_rm(struct outlet *x, struct inlet *ip);
void box_color(struct box *b);
void box_getcolor(struct box *b,short index, RGBColor *rgb);
void assist_drawstr(struct patcher *p, char *s);
void schedule(void *ob, method fun, long when, Symbol *sym, short argc, Atom *argv);
long usecolor(struct box *b);
void palette_getcolor(struct box *b, short index, RGBColor *rgb);
void message_register(struct symbol *name, struct symbol *superClass,
	struct object *theObject, struct symbol *objectName, struct tlTrack *x);
void editor_register(struct symbol *dataType, struct symbol *name, method newFN,
	method menuFN, method updateFN);
void *atombuf_new(long argc, t_atom *argv);
void atombuf_free(struct atombuf *x);
void atombuf_text(struct atombuf **x, char **shit, long size);
void event_new(struct event *e, struct tlTrack *track, struct symbol *dataType, 
	struct symbol *message, struct symbol *editor, struct symbol *label,
	long start, long duration, long flags, struct box *box);
void event_box(struct event *e, short top, short bottom);
void message_patcherRegister(struct symbol *name, struct symbol *superClass,
	struct object *theObject, struct symbol *objectName, struct patcher *p);
void open_promptset(char *s);
void saveas_promptset(char *s);
void *stringload(char *name);
void *resource_install(char *name, long rsrc);
void *toolfile_new(char *name, short vol, long type);
long toolfile_fread(struct toolfile *t, char *buf, long n);
long toolfile_fwrite(struct toolfile *t, char *buf, long n);
void wind_filename(struct wind *x, char *fn, short vol, short bin);
short wind_close(struct wind *x);
void wind_nocancel(void);
short wind_inhscroll(struct wind *x, Point pt);
void wind_setbin(struct wind *x, short way);
void class_noinlet(t_messlist *m);
void expr_install(exprmethod fun, const char *buf, short argc);
void dialog_poll(short dosched, short doevent, unsigned short evtMask);
void message_unregister(struct symbol *name, struct symbol *superClass,
	struct object *theObject, struct symbol *objectName, struct tlTrack *x);
void message_patcherUnregister(struct symbol *name, struct symbol *superClass,
	struct object *theObject, struct symbol *objectName, struct patcher *p);
void *connection_client(t_object *cli, t_symbol *name, t_symbol *classname, method traverse);
void connection_server(t_object *obj, t_symbol *name);
void connection_send(t_object *server, t_symbol *name, t_symbol *mess, void *arg);
void connection_delete(t_object *ob, t_symbol *name);
short toolfile_getc(struct toolfile *t);
void debug_printf(char *,...);
void event_spool(struct event *e);
void event_schedule(struct event *e, method fn, t_object *dest, void *arg,
	long delay, long flags);
void track_clipBegin(struct tlTrack *x, Rect *r);
void track_clipEnd(struct tlTrack *x);
short event_offsetRect(struct event *e);
void track_eraseDragTime(struct tlTrack *x);
void track_drawDragParam(struct tlTrack *x, char *buf);
void track_drawDragTime(struct tlTrack *x, long top, long bottom);
void track_drawTime(struct tlTrack *x, long time);
long track_pixToMS(struct tlTrack *x, short pix);
short track_MSToPix(struct tlTrack *x, long time);
long track_posToMS(struct tlTrack *x, short pos);
short track_MSToPos(struct tlTrack *x, long time);
GrafPtr	track_setport(struct tlTrack *x);
void *event_clock(struct event *e);
struct patcher *event_avoidRect(struct event *e, short id);
void event_save(struct event *e, Atom *a);
void *tabfromhandle(long **handle, long size);
void stdlist(t_object *x, t_symbol *s, short ac, t_atom *av);
void setclock_delay(struct object *x, struct clock *c, long time);
void setclock_unset(struct object *x, struct clock *c);
long setclock_gettime(struct object *x);
void patcher_avoidrect(struct patcher *p, Rect *r2, short id);
short collective_load(char *name, short vol, short argc, t_atom *argv);
void assist_queue(struct object *x, method fun);
void *wind_syswind(struct wind *x);
void clock_fdelay(struct clock *c, double time);
void setclock_fdelay(struct object *s, struct clock *c, double time);
void clock_getftime(double *time);
void setclock_getftime(struct object *s, double *time);
short nameinpath(char *name, short *ref);
void *scheduler_new(void);
void *scheduler_set(void *x);
void scheduler_run(void *x, double until);
void scheduler_settime(void *x, double time);
void scheduler_gettime(void *x, double *time);
void quittask_install(method m, void *a);
void quittask_remove(method m);
void *notify_new(t_object *owner);
void notify_enlist(t_object *dependent, t_object *owner);
void notify_update(void *xx);
void notify_free(t_object *owner);
void inspector_open(t_object *x, void *p, void *b);
void *object_subpatcher(t_object *x, long *index, void *arg);
void event_process(void *e, struct wind *w);
void event_run(void);
void error_subscribe(t_object *x);
void error_unsubscribe(t_object *x);
void wind_setgrowbounds(struct wind *x, short minx, short miny, short maxx, short maxy);
short saveasdialog_extended(char *name, short *vol, long *type, long *typelist, short numtypes);

// temporary typedef, need to fix this...
#ifndef PATH_SPEC
#include <Files.h>
typedef FSSpec PATH_SPEC;
typedef Boolean (*pathmethod)(void *arg, PATH_SPEC *par, short patharg);
#define __PATHMETHOD__
typedef short FILE_REF;
#endif

short genpath(PATH_SPEC *fs);
short path_lookup(PATH_SPEC *fs);
short path_new(PATH_SPEC *fs);
short path_tospec(short path, char *name, PATH_SPEC *fs);
void path_namefromspec(PATH_SPEC *fs, char *name);
void path_setpermanent(struct _pathlink *list, short parent);
void path_addnamed(long pathtype, char *name, short recursive, short permanent);
void path_build(struct _pathlink **head, short startpath, short recursive);
void path_eachdo(short startpath, pathmethod fun, void *arg, short patharg);
short locatefile_extended(char *name, short *outvol, long *outtype, long *filetypelist,
	short numtypes);
short locatefile_pathlist(struct _pathlink *list, char *name, short *outvol, long *outtype,
	long *filetypelist, short numtypes);
short path_resolvefile(char *name, short path, short *outpath);
short path_fileinfo(char *name, short path, void *info);
short path_openfile(char *name, short path, FILE_REF *ref, short perm);
short path_openresfile(char *name, short path, FILE_REF *ref, short perm);
short path_createfile(char *name, short path, long type, FILE_REF *ref);
short path_createresfile(char *name, short path, long type, FILE_REF *ref);
short path_translate(PATH_SPEC *fs, char *name, short *vol, short resolvealias);
short path_topathname(short path, char *file, char *name);
short path_frompathname(char *name, short *path, char *filename);
short path_getnext(struct _pathlink *list, short *val);
void path_setdefault(short path, short recursive);
short path_getdefault(void);
void path_setdefaultlist(struct _pathlink *list);
short path_getmoddate(short path, unsigned long *date);
short path_getfilemoddate(char *filename, short path, unsigned long *date);
long path_listcount(struct _pathlink *list);
void *path_openfolder(short path);
short path_foldernextfile(void *xx, long *filetype, char *name, short descend);
void path_closefolder(void *x);
Boolean syswindow_inlist(WindowPtr w);
void syswindow_show(WindowPtr w);
void syswindow_hide(WindowPtr w);
short getfolder(short *vol);
void path_makefromsymbol(long pathtype, t_symbol *sp, short recursive);
short path_getprefstring(short type, short index, t_symbol **s);
void path_setprefstring(short type, short index, t_symbol *s, short update);
void path_removefromlist(struct _pathlink **list, short parent);
short path_getapppath(void);
short sndfile_info(char *filename, short vol, unsigned long type, struct _sndfileinfo *info);
short path_foldergetspec(void *xx, PATH_SPEC *spec, short resolve);
void forecolor(short index, short way);
void backcolor(short index, short way);
short box_invalnow(struct box *b);
void *box_clip(struct box *b);
void box_enddraw(struct box *b);
short box_visible(struct box *b);
short patcher_boxname(struct patcher *p, struct box *b, struct symbol **s);
void patcher_setboxname(struct patcher *p, struct box *b, struct symbol *s);
short isr_set(short way);
void box_hilite(struct box *b, short way);
void patcher_nohilite(struct patcher *p);
void outlet_atoms(void *out, short argc, t_atom *argv);
void *wind_validwind(void);
void hilite_settarget(struct patcher *p, struct box *b);
char *getbytes16(short size);
void freebytes16(char *mem, short size);
long qtimage_open(char *name, short path, CGrafPtr *gp, void *extra);
long qtimage_getrect(char *name, short path, Rect *r, void *extra);
void *qti_extra_new           (void);
void qti_extra_free           (void *p);
long qti_extra_matrix_get     (void *p, void *m);
long qti_extra_matrix_set     (void *p, void *m);
long qti_extra_rect_get       (void *p, Rect *r);
long qti_extra_rect_set       (void *p, Rect *r);
long qti_extra_scalemode_get  (void *p, long *scalemode);
long qti_extra_scalemode_set  (void *p, long scalemode);
long qti_extra_time_get       (void *p, double *time);
long qti_extra_time_set       (void *p, double time);
void schedule_delay(void *ob, method fun, long delay, t_symbol *sym, short argc, t_atom *argv);
void schedule_defer(void *ob, method fun, long delay, t_symbol *sym, short argc, t_atom *argv);
void *defer_medium(void *ob, method fn, t_symbol *sym, short argc, t_atom *argv);
void *defer_front(void *ob, method fn, t_symbol *sym, short argc, t_atom *argv);
long boxcolor_rgb2index(RGBColor *n);
#ifdef WIN_VERSION
#define SYSINFO_MINOR_VERSION_UNDEFINED			0
#define SYSINFO_MAJOR_VERSION_UNDEFINED			0
#define SYSINFO_MAJOR_VERSION_XP				1
#define SYSINFO_MAJOR_VERSION_2K				2
#define SYSINFO_MAJOR_VERSION_NT				3
#define SYSINFO_MAJOR_VERSION_ME				4
#define SYSINFO_MAJOR_VERSION_SE				5
#define SYSINFO_MAJOR_VERSION_98				6
#define SYSINFO_MAJOR_VERSION_OSR2				7
#define SYSINFO_MAJOR_VERSION_95				8
#define SYSINFO_MAJOR_VERSION_OTHER				10
Boolean sysinfo_iswinxp(void);
Boolean sysinfo_iswin2k(void);
Boolean sysinfo_is_supported_os(void);
void sysinfo_get_osversion( short* pMajorVersion, short* pMinorVersion );
OSErr sysinfo_query(long selector,long* response);
void file_getmaxfiletypes( long* rgTypeBuffer, long lTypeBufferSize, long* lTypeCount );
void file_getdisplayname_from_filename( char* szFileName, char* szDisplayName, long lBufSize );
void file_getfsspec_from_elaboratedname( char* szElaboratedName, FSSpec* pSpec );
void file_getelaboratedname_from_fsspec( FSSpec* pSpec, char* szElaboratedName );
void file_getclassname_from_filename( char* szName, char* szClassName );
long file_gettype_from_filename( char *szName );
void file_getfullname_from_filetype( char* szPrefix, long lType, char* szBuf, long lBufSize );
void file_getextension_from_filetype( long lType, char* szBuf, long lBufSize );
void file_getprefixname_from_elaboratedname( char* szElaboratedName, char* szPrefix );
void file_getfilename_from_elaboratedname( char* szElaboratedName, char* szFile );
#endif WIN_VERSION
