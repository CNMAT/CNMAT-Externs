/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
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
NAME: cc
DESCRIPTION: write and compile c code in max
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_critical.h"
#include "ext_hashtab.h"
#include <dlfcn.h>

#define BUFSIZE 4096
#define OBJSIZE 65536
#define CFLAGS "-Wall -O3 -isysroot /Developer/SDKs/MacOSX10.5.sdk -I%s/c74support/max-includes -include %s/c74support/max-includes/macho-prefix.pch -F%s/c74support/max-includes -mmacosx-version-min=10.5 -fPIC"

#define LDFLAGS "-isysroot /Developer/SDKs/MacOSX10.5.sdk -F%s/c74support/max-includes -lmx -framework Carbon -framework MaxAPI"

typedef int (*ccmethod)(t_object *, char *, int, int, t_atom *, int, void **);

typedef struct _cc{
	t_object ob;
	void **outlets;
	long ninlets, noutlets;
	long inlet;
	void **proxies;
	t_object *ed;
	char *code_buf;
	int code_buf_len;
	int code_len;
	char *cfile, *ofile, *dfile, *stfile, *logfile;
	char *cfile_fullpath, *ofile_fullpath, *dfile_fullpath, *stfile_fullpath, *logfile_fullpath;
	short cfile_path, build_path;
	void *handle;
	t_hashtab *ht;
	t_symbol **function_names;
	int num_functions;
	t_symbol *path_to_maxsdk;
	char *def_cflags;
	char *def_ldflags;
	t_symbol *user_cflags;
	t_symbol *user_ldflags;
	char *user_obj;
	int verbose;
	long ret;
	int cfile_is_tmp;
} t_cc;

static t_class *cc_class;

void cc_compile(t_cc *x);
void cc_docompile(t_cc *x, t_symbol *msg, short argc, t_atom *argv);
void cc_bang(t_cc *x);
void cc_read_symbols(t_cc *x);
void cc_edclose(t_cc *x, char **text, long size);
long cc_edsave(t_cc *x, char **text, long size);
void cc_okclose(t_cc *x, char *prompt, short *result);
void cc_dblclick(t_cc *x);
/*
void cc_read(t_cc *x, t_symbol *s, short argc, t_atom *argv);
void cc_doread(t_cc *x, t_symbol *s, short argc, t_atom *argv);
void cc_save(t_cc *x, t_symbol *s, short argc, t_atom *argv);
void cc_saveas(t_cc *x, t_symbol *s, short argc, t_atom *argv);
void cc_dosave(t_cc *x, t_symbol *s, short argc, t_atom *argv);
*/
void cc_make_file_paths(t_cc *x);
void cc_free(t_cc *x);
void cc_assist(t_cc *x, void *b, long m, long a, char *s);
void cc_post_log(t_cc *x, void (*p)(char *, ...));
void cc_write_template(t_cc *x, FILE *fp);
void *cc_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err cc_notify(t_cc *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
t_max_err cc_cfile_set(t_cc *x, t_object *attr, long argc, t_atom *argv);
t_max_err cc_cfile_get(t_cc *x, t_object *attr, long *argc, t_atom **argv);
t_max_err cc_build_path_set(t_cc *x, t_object *attr, long argc, t_atom *argv);
t_max_err cc_build_path_get(t_cc *x, t_object *attr, long *argc, t_atom **argv);

t_symbol *ps_cc_instance_count;

void cc_compile(t_cc *x){
	defer_low(x, (method)cc_docompile, NULL, 0, NULL);
}

void cc_docompile(t_cc *x, t_symbol *msg, short argc, t_atom *argv){
	if(x->handle){
		dlclose(x->handle);
	}
	FILE *fp = fopen(x->cfile_fullpath, "w");
	fprintf(fp, "%s", x->code_buf);
	fclose(fp);

	/*
	long err;
	t_filehandle fh;
	err = path_createsysfile(x->cfile, x->cfile_path, 'TEXT', &fh);
	if(err){
		error("cc: problem creating file");
		return;
	}
	post("codebuf = %s", x->code_buf);
	err = sysfile_writetextfile(fh, &(x->code_buf), TEXT_LB_NATIVE);
	sysfile_close(fh);
	*/

	int ret;
	char compbuf[1024];
	char flags[1024];
	sprintf(flags, "%s %s", x->def_cflags, x->user_cflags->s_name);
	sprintf(compbuf, "gcc %s -o %s -c %s > %s 2>&1", flags, x->ofile_fullpath, x->cfile_fullpath, x->logfile_fullpath);
	if(x->verbose){
		post("%s", compbuf);
	}
	if(ret = system(compbuf)){
		error("cc: error compiling file:");
		cc_post_log(x, error);
		return;
	}

	sprintf(flags, "%s %s", x->def_ldflags, x->user_ldflags->s_name);
	sprintf(compbuf, "gcc %s -o %s -dynamiclib %s >> %s 2>&1", flags, x->dfile_fullpath, x->ofile_fullpath, x->logfile_fullpath);
	if(x->verbose){
		post("%s", compbuf);
	}
	if(ret = system(compbuf)){
		error("cc: error linking file:");
		cc_post_log(x, error);
		return;
	}

	sprintf(compbuf, "nm %s | awk '$2 ~ /T/ {print $3}' > %s", x->ofile_fullpath, x->stfile_fullpath);
	if(x->verbose){
		post("%s", compbuf);
	}
	system(compbuf);

	x->handle = dlopen(x->dfile_fullpath, RTLD_NOW);
	if(!x->handle){
		error("cc: %s", dlerror());
		return;
	}

	cc_read_symbols(x);

	/*
	x->f = dlsym(x->handle, "f");
	if((err = dlerror()) != NULL){
		error("o.tt_callback: %s", err);
		return;
	}

	post("f(%f, %f) = %f\n", 3., 4., x->f(3., 4.));
	*/
}

void cc_bang(t_cc *x){

}

void cc_anything(t_cc *x, t_symbol *msg, int argc, t_atom *argv){
	long f = 0;
	hashtab_lookup(x->ht, msg, (t_object **)(&f));
	if(!f){
		error("cc: couldn't look up function %s", msg->s_name);
		return;
	}
	int inlet = proxy_getinlet((t_object *)x);
	((ccmethod)(f))((t_object *)x, x->user_obj, inlet, argc, argv, x->noutlets, x->outlets);
	/*
	t_atombuf *ab[x->noutlets];
	int i;
	for(i = 0; i < x->noutlets; i++){
		ab[i] = NULL;
	}
	if(!(((ccmethod)(f))(x->user_obj, inlet, argc, argv, ab))){
		for(i = x->noutlets - 1; i >= 0; i--){
			if(ab[i]){
				if(ab[i]->a_argc == 1){
					switch(ab[i]->a_argv->a_type){
					case A_FLOAT:
						outlet_float(x->outlets[i], atom_getfloat(ab[i]->a_argv));
						break;
					case A_LONG:
						outlet_int(x->outlets[i], atom_getlong(ab[i]->a_argv));
						break;
					case A_SYM:
						outlet_anything(x->outlets[i], atom_getsym(ab[i]->a_argv), 0, NULL);
						break;
					}
				}else{
					if(ab[i]->a_argv->a_type == A_SYM){
						outlet_anything(x->outlets[i], atom_getsym(ab[i]->a_argv), ab[i]->a_argc - 1, ab[i]->a_argv + 1);
					}else{
						outlet_list(x->outlets[i], NULL, ab[i]->a_argc, ab[i]->a_argv);
					}
				}
			}
		}
	}
	*/
}

void cc_read_symbols(t_cc *x){
	hashtab_clear(x->ht);
	char *err;
	FILE *fp = fopen(x->stfile_fullpath, "r");
	char buf[LINE_MAX];
	int i = 0;
	while(fgets(buf, LINE_MAX, fp)){
		buf[strlen(buf) - 1] = '\0'; // get rid of the newline char
		ccmethod f = (ccmethod)dlsym(x->handle, buf + 1);
		if((err = dlerror()) == NULL){
			x->function_names[i] = gensym(buf + 1); // skip over the leading underscore
			hashtab_store(x->ht, x->function_names[i], (t_object *)f);
			post("function %d = %s", i, buf + 1);
		}
	}
	fclose(fp);
}

/****************************************************
 * editor stuff taken from chuck~
 ***************************************************/
void cc_edclose(t_cc *x, char **text, long size){
	if(size + 1 > x->code_buf_len){
		char *tmp = (char *)realloc(x->code_buf, (size * 2) * sizeof(char));
		if(tmp){
			x->code_buf = tmp;
			x->code_buf_len = size * 2;
		}else{
			error("cc: out of memory!");
			return;
		}
	}
	strncpy(x->code_buf, *text, size);
	x->code_buf[size] = '\0';
	x->code_len = size + 1;

	x->ed = NULL;
}

long cc_edsave(t_cc *x, char **text, long size){
	post("edsave %s", *text);
	if(x->ed){
		t_class *c = class_findbyname(CLASS_NOBOX, gensym("jed"));
		post("messcount = %d", c->c_messcount);
		int i;
		for(i = 0; i < c->c_messcount; i++){
			if(c->c_messlist[i].m_sym){
				post("%s", c->c_messlist[i].m_sym->s_name);
			}
		}
	}
	/*
	FILE *fp = fopen(x->cfile_fullpath, "w");
	fwrite(x->code_buf, sizeof(char), x->code_len, fp);
	fclose(fp);
	*/
	return 1;
}

void cc_okclose(t_cc *x, char *prompt, short *result){
	if(x->cfile_is_tmp){
		sprintf(prompt, "Save changes before closing?");
		*result = 1;
	}else{
		*result = 0;
	}
	return;
}

void cc_dblclick(t_cc *x){
	// this should be done only if the file was actually modified--otherwise, just use the buffer
	FILE *fp = fopen(x->cfile_fullpath, "r");
	if(fp){
		x->code_len = fread(x->code_buf, sizeof(char), x->code_buf_len, fp);
	}else if(x->cfile_is_tmp){
		fclose(fp);
		fp = fopen(x->cfile_fullpath, "w");
		cc_write_template(x, fp);
		fclose(fp);
		fp = fopen(x->cfile_fullpath, "r");
		x->code_len = fread(x->code_buf, sizeof(char), x->code_buf_len, fp);
	}
	fclose(fp);
	if(x->ed){
		object_method(x->ed, gensym("settext"), x->code_buf, gensym("utf-8"));
	}else{
		x->ed = (t_object *)object_new(CLASS_NOBOX, gensym("jed"), (t_object *)x, 0);
		if(x->cfile_is_tmp){
			object_method(x->ed, gensym("filename"), x->cfile, x->cfile_path);
			object_attr_setsym(x->ed, gensym("title"), gensym("editor"));
		}else{
			object_method(x->ed, gensym("filename"), x->cfile, x->cfile_path);
			object_attr_setsym(x->ed, gensym("title"), gensym(x->cfile_fullpath));
		}
		object_method(x->ed, gensym("settext"), x->code_buf, gensym("utf-8"));
	}

	object_attr_setchar(x->ed, gensym("visible"), 1);
}

/**************************************************
 * File IO stuff from chuck~
 **************************************************/
/*
void cc_read(t_cc *x, t_symbol *s, short argc, t_atom *argv){
	defer_low(x, (method)cc_doread, s, argc, argv);
}

void cc_doread(t_cc *x, t_symbol *s, short argc, t_atom *argv){
	char filename[256];
	short err;
	long type = 'TEXT'; 
	long size;
	short volptr;
	long outtype;
	t_filehandle fh;
	t_handle script_handle;
	
	if(argc == 0){
		if(open_dialog(filename, &volptr, &outtype, 0L, 0)){ // allow all types of files
			return; //user cancelled
		}
	}else{
		if(locatefile_extended(filename, &volptr, &outtype, &type, 1)){
			error("cc~: error opening file: can't find file"); 
			return; //not found
		}
	}
	
	//we should have a valid filename at this point
	err = path_opensysfile(filename, volptr, &fh, READ_PERM);  
	if(err){       
		fh = 0;      
		error("error %d opening file", err); 
		return;
	}
	
	strcpy(x->cfile, filename);
	x->cfile_path = volptr;
	cc_make_file_paths(x);
	
	sysfile_geteof(fh, &size);

	if(!(script_handle = sysmem_newhandle(size + 1))){
		error("cc: couldn't get a filehandle");
	}

	sysfile_readtextfile(fh, script_handle, size, TEXT_LB_NATIVE);
	strcpy(x->code_buf, *script_handle);

	x->code_buf[size] = '\0'; // the max5 text editor apparently needs this
	// BGG for some reason mach-o doesn't like this one... the memory hit should be small
//	sysmem_freehandle(script_handle);
	sysfile_close(fh);

	return;	
}
*/


void cc_save(t_cc *x, t_symbol *s, short argc, t_atom *argv){	
	//defer(x, (method)cc_dosave, s, argc, argv);
	post("save!!");
}

void cc_saveas(t_cc *x, t_symbol *s, short argc, t_atom *argv){
	//defer(x, (method)cc_dosave, s, argc, argv);
	post("saveas!!");
}
/*
void cc_dosave(t_cc *x, t_symbol *s, short argc, t_atom *argv){
	char filename[256];
	t_handle script_handle;
	short err;
	long type_chosen, thistype = 'TEXT';
	t_filehandle fh;
	
	if(argc == 0) {
		if(saveasdialog_extended(x->cfile, &(x->cfile_path), &type_chosen, &thistype, 1)){
			return; //user cancelled
		}
	}
	
	err = path_createsysfile(x->cfile, x->cfile_path, thistype, &fh);  
	if (err) {       
		error("cc: error %d creating file", err); 
		return;
	}
	
	script_handle = sysmem_newhandle(0);
	sysmem_ptrandhand(x->code_buf, script_handle, x->code_len); 

	err = sysfile_writetextfile(fh, script_handle, TEXT_LB_UNIX); 
	if (err) {       
		error("cc: error %d writing file", err); 
		return;
	}

	// BGG for some reason mach-o doesn't like this one... the memory hit should be small
//	sysmem_freehandle(script_handle);
	sysfile_seteof(fh, x->code_len); 
	sysfile_close(fh);

	return;	

}
*/

void cc_make_file_paths(t_cc *x){
	char path[512], *p;
	int path_len;

	path_topathname(x->cfile_path, x->cfile, path);
	p = path;
	path_len = strlen(path);
	while(*(p++) != ':' && (p - path) < path_len){}
	strcpy(x->cfile_fullpath, p);

	//sprintf(x->cfile_fullpath, "%s/%s", p, x->cfile);

	path_topathname(x->build_path, NULL, path);
	p = path;
	path_len = strlen(path);
	while(*(p++) != ':' && (p - path) < path_len){}

	sprintf(x->ofile_fullpath, "%s/%s", p, x->ofile);
	sprintf(x->dfile_fullpath, "%s/%s", p, x->dfile);
	sprintf(x->stfile_fullpath, "%s/%s", p, x->stfile);
	sprintf(x->logfile_fullpath, "%s/%s", p, x->logfile);
}

void cc_free(t_cc *x){
	if(x->cfile_is_tmp){
		// save prompt here would be nice...
		remove(x->cfile_fullpath);
	}
	free(x->code_buf);
	free(x->cfile);
	free(x->ofile);
	free(x->dfile);
	free(x->stfile);
	free(x->logfile);
	free(x->function_names);
	if(x->handle){
		dlclose(x->handle);
	}
	free(x->def_cflags);
	free(x->def_ldflags);
	free(x->user_obj);
	int i;
	for(i = 0; i < x->ninlets - 1; i++){
		object_free(x->proxies[i]);
	}
	free(x->proxies);
	free(x->outlets);
}

void cc_assist(t_cc *x, void *b, long io, long index, char *s){
	switch(io){
	case 1:
		switch(index){
		case 0:
			sprintf(s, "");
			break;
		}
		break;
	case 2:
		switch(index){
		case 0:
			sprintf(s, "");
			break;
		}
		break;
	}
}

void cc_post_log(t_cc *x, void (*p)(char *, ...)){
	FILE *fp = fopen(x->logfile_fullpath, "r");
	char buf[LINE_MAX];
	while(fgets(buf, LINE_MAX, fp)){
		p("%s", buf);
	}
	fclose(fp);
}

void cc_write_template(t_cc *x, FILE *fp){
	if(!fp){
		return;
	}
	fprintf(fp, "#include \"ext.h\"\n");
	fprintf(fp, "#include \"ext_obex.h\"\n");
	fprintf(fp, "#include \"ext_obex_util.h\"\n");

	fprintf(fp, "int foo(t_object *o, char *x, int inlet, int argc, t_atom *argv, int numoutlets, void **outlets){\n");
	fprintf(fp, "\n\treturn 0; // non-zero indicates an error\n");
	fprintf(fp, "}\n");

	fprintf(fp, "\n\n");

	fprintf(fp, "void free(t_object *o, char *x){\n");
	fprintf(fp, "\t// free any instance data here\n");
	fprintf(fp, "}\n");

	fprintf(fp, "void new(t_object *o, char *x){\n");
	fprintf(fp, "\t// allocate and initialize any instance data/variables here\n");
	fprintf(fp, "\t// this function will be called each time you compile\n");
	fprintf(fp, "}\n");
}

void *cc_new(t_symbol *msg, short argc, t_atom *argv){
	t_cc *x;

	if(x = (t_cc *)object_alloc(cc_class)){
		x->ht = (t_hashtab *)hashtab_new(0);
		hashtab_flags(x->ht, OBJ_FLAG_DATA);

		x->ed = NULL;
		x->code_buf_len = BUFSIZE;
		x->code_buf = (char *)calloc(BUFSIZE, sizeof(char));
		int len = 512;

		x->cfile = (char *)calloc(len, sizeof(char));
		x->ofile = (char *)calloc(len, sizeof(char));
		x->dfile = (char *)calloc(len, sizeof(char));
		x->stfile = (char *)calloc(len, sizeof(char));
		x->logfile = (char *)calloc(len, sizeof(char));

		x->cfile_fullpath = (char *)calloc(len, sizeof(char));
		x->ofile_fullpath = (char *)calloc(len, sizeof(char));
		x->dfile_fullpath = (char *)calloc(len, sizeof(char));
		x->stfile_fullpath = (char *)calloc(len, sizeof(char));
		x->logfile_fullpath = (char *)calloc(len, sizeof(char));

		x->verbose = 0;

		// private?!?
		//short tmpdir = path_tempfolder();
		short tmpdir;
		char *tmp = "/private/var/tmp";
		char fn[512];
		path_frompathname(tmp, &tmpdir, fn);
		x->cfile_path = x->build_path = tmpdir;

		sprintf(x->cfile, "cc_%ld.c", (long)ps_cc_instance_count->s_thing);
		sprintf(x->ofile, "cc_%ld.o", (long)ps_cc_instance_count->s_thing);
		sprintf(x->dfile, "cc_%ld.dylib", (long)ps_cc_instance_count->s_thing);
		sprintf(x->stfile, "cc_%ld.st", (long)ps_cc_instance_count->s_thing);
		sprintf(x->logfile, "cc_%ld.log", (long)ps_cc_instance_count->s_thing);

		sprintf(x->cfile_fullpath, "%s/%s", tmp, x->cfile);
		sprintf(x->ofile_fullpath, "%s/%s", tmp, x->ofile);
		sprintf(x->dfile_fullpath, "%s/%s", tmp, x->dfile);
		sprintf(x->stfile_fullpath, "%s/%s", tmp, x->stfile);
		sprintf(x->logfile_fullpath, "%s/%s", tmp, x->logfile);

		x->function_names = (t_symbol **)calloc(128, sizeof(t_symbol *));

		x->path_to_maxsdk = gensym("");
		x->user_cflags = gensym("");
		x->user_ldflags = gensym("");

		x->ninlets = 1;
		x->noutlets = 1;
		attr_args_process(x, argc, argv);

		x->proxies = (void **)calloc(x->ninlets - 1, sizeof(void *));
		int i;
		for(i = 0; i < x->ninlets - 1; i++){
			x->proxies[0] = proxy_new((t_object *)x, x->ninlets - (i + 1), &(x->inlet));
		}
		x->outlets = (void **)calloc(x->noutlets, sizeof(void *));
		for(i = x->noutlets - 1; i >= 0; i--){
			x->outlets[i] = outlet_new((t_object *)x, NULL);
		}

		char *sdk = x->path_to_maxsdk->s_name;

		x->def_cflags = (char *)calloc(512, sizeof(char));
		sprintf(x->def_cflags, CFLAGS, sdk, sdk, sdk);
		x->def_ldflags = (char *)calloc(512, sizeof(char));
		sprintf(x->def_ldflags, LDFLAGS, sdk);

		x->user_obj = (char *)calloc(OBJSIZE, sizeof(char));
		long ic = (long)(ps_cc_instance_count->s_thing);
		ic += 1;
		ps_cc_instance_count->s_thing = (void *)ic;
		x->handle = NULL;

		x->cfile_is_tmp = 1;

		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("cc", (method)cc_new, (method)cc_free, sizeof(t_cc), 0L, A_GIMME, 0);

	class_addmethod(c, (method)cc_bang, "bang", 0);
	class_addmethod(c, (method)cc_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)cc_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)cc_notify, "notify", A_CANT, 0);

	//for the text editor and code
	class_addmethod(c, (method)cc_edclose, "edclose", A_CANT, 0); 
	class_addmethod(c, (method)cc_edsave, "edsave", A_CANT, 0); 
	class_addmethod(c, (method)cc_dblclick, "dblclick", A_CANT, 0);
	class_addmethod(c, (method)cc_okclose, "okclose", A_CANT, 0);  
	class_addmethod(c, (method)cc_compile, "compile", 0);
	//class_addmethod(c, (method)cc_read, "read", A_GIMME, 0);
	class_addmethod(c, (method)cc_save, "save", A_GIMME, 0);
	class_addmethod(c, (method)cc_saveas, "saveas", A_GIMME, 0);

	CLASS_ATTR_SYM(c, "maxsdk", 0, t_cc, path_to_maxsdk);
	CLASS_ATTR_LONG(c, "outlets", 0, t_cc, noutlets);
	CLASS_ATTR_LONG(c, "inlets", 0, t_cc, ninlets);
	CLASS_ATTR_LONG(c, "verbose", 0, t_cc, verbose);

	CLASS_ATTR_CHAR(c, "cfile", 0, t_cc, cfile);
	CLASS_ATTR_ACCESSORS(c, "cfile", cc_cfile_get, cc_cfile_set);

	CLASS_ATTR_CHAR(c, "build_path", 0, t_cc, build_path);
	CLASS_ATTR_ACCESSORS(c, "build_path", cc_build_path_get, cc_build_path_set);

	CLASS_ATTR_LONG(c, "ret", 0, t_cc, ret);
	class_register(CLASS_BOX, c);
	cc_class = c;

	ps_cc_instance_count = gensym("cc_instance_count");
	ps_cc_instance_count->s_thing = 0;

	common_symbols_init();
    
	return 0;
}

t_max_err cc_notify(t_cc *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

        if (msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		if(attrname == gensym("maxsdk")){
			char *sdk = x->path_to_maxsdk->s_name;
			sprintf(x->def_cflags, CFLAGS, sdk, sdk, sdk);
			sprintf(x->def_ldflags, LDFLAGS, sdk);
		}
        }
        return 0;
}

t_max_err cc_cfile_set(t_cc *x, t_object *attr, long argc, t_atom *argv){
	if(argc == 0){
		return 1;
	}
	char *fullpath = atom_getsym(argv)->s_name;
	int len = strlen(fullpath);
	path_frompathname(fullpath, &(x->cfile_path), x->cfile);
	cc_make_file_paths(x);

	FILE *fp = fopen(fullpath, "r");
	if(fp){
		x->code_len = fread(x->code_buf, sizeof(char), x->code_buf_len, fp);
	}
	fclose(fp);

	len = strlen(x->cfile);
	char buf[len - 1];
	memset(buf, '\0', len - 1);
	strncpy(buf, x->cfile, len - 2);
	sprintf(x->ofile, "%s.o", buf);
	sprintf(x->dfile, "%s.dylib", buf);
	sprintf(x->stfile, "%s.st", buf);
	sprintf(x->logfile, "%s.log", buf);

	x->cfile_is_tmp = 0;

	return 0;
}

t_max_err cc_cfile_get(t_cc *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
 
        atom_alloc(argc, argv, &alloc);     // allocate return atom
 
        atom_setsym(*argv, gensym(x->cfile_fullpath));

        return 0;
}

t_max_err cc_build_path_set(t_cc *x, t_object *attr, long argc, t_atom *argv){
	if(argc == 0){
		return 1;
	}
	char *fullpath = atom_getsym(argv)->s_name;
	int len = strlen(fullpath);
	char buf[512];
	path_frompathname(fullpath, &(x->build_path), buf);
	sprintf(buf, "mkdir -p %s", fullpath);
	system(buf);

	sprintf(x->ofile_fullpath, "%s/%s", fullpath, x->ofile);
	sprintf(x->dfile_fullpath, "%s/%s", fullpath, x->dfile);
	sprintf(x->stfile_fullpath, "%s/%s", fullpath, x->stfile);
	sprintf(x->logfile_fullpath, "%s/%s", fullpath, x->logfile);

	//cc_make_file_paths(x);

	return 0;
}

t_max_err cc_build_path_get(t_cc *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
 
        atom_alloc(argc, argv, &alloc);     // allocate return atom
 
        atom_setsym(*argv, gensym(x->cfile_fullpath));

        return 0;
}
