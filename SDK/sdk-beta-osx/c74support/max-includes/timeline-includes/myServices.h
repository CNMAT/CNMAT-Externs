void binbuf_append(struct binbuf *x, t_symbol *s, short argc, t_atom *argv);
void binbuf_clear(struct binbuf *x);
void *binbuf_eval(struct binbuf *x, short ac, t_atom *av, void *to);
void binbuf_free(struct binbuf *x);
void binbuf_insert(struct binbuf *x, t_symbol *s, short argc, t_atom *argv);
short binbuf_write(struct binbuf *x, char *fn, short vol, short binary);
void binbuf_vinsert(void *z,char *s,...);
void c_addbang(method f);
short c_rescopy(OSType type,int id);
void clock_unset(void *x);
void clock_free(void *x);
void qelem_free(void *x);
short OpenWorkingDir(Byte *name,short *vol);
void ptoccpy(char *s,char *t);
void ctopcpy(char *s,char *t);
int	crosshatch(t_atom *);
void *intload(char *name, short volume, t_symbol *s, short ac, t_atom *av, short couldedit);

void wind_setsmax(struct wind *x, short a, short b);
void wind_setsval(struct wind *x, short a, short b);
void wind_settitle(struct wind *x, char *s, short brackets);

