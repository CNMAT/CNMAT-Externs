#include <stdlib.h>
#include <string.h>

typedef char t_cmmjl_osc_type;

typedef struct _cmmjl_osc_obj{
	int size;
	struct _cmmjl_osc_obj *next;
} t_cmmjl_osc_obj;

union _cmmjl_osc_word{
	int w_int;
	float w_float;
	char *w_string;
	char *w_blob;
	long long w_timetag;
};

typedef struct _cmmjl_osc_atom{
	union _cmmjl_osc_word a_w;
	t_cmmjl_osc_type a_type;
} t_cmmjl_osc_atom;

typedef struct _cmmjl_osc_message{
	t_cmmjl_osc_obj obj;
	char *address;
	t_cmmjl_osc_type *typetags;
	int argc;
	char *argv;
} t_cmmjl_osc_message;

typedef struct _cmmjl_osc_bundle{
	t_cmmjl_osc_obj obj;
	char bundle[8];
	t_cmmjl_osc_obj *element;
	char *timetag;
} t_cmmjl_osc_bundle;

typedef struct _cmmjl_osc_packet{
	t_cmmjl_osc_obj *contents;
	int size;
} t_cmmjl_osc_packet;

t_cmmjl_osc_packet *cmmjl_osc_make_packet(long len, char *ptr);
void cmmjl_osc_make_bundle(long len, char *ptr, t_cmmjl_osc_bundle *b);
t_cmmjl_osc_bundle *cmmjl_osc_bundle_new(void);
t_cmmjl_osc_message *cmmjl_osc_message_new();
t_cmmjl_osc_message *cmmjl_osc_message_new_args(char *address, char *typetags, long argc, char *argv);
t_cmmjl_osc_message *cmmjl_osc_message_new_raw(long len, char *ptr);
void cmmjl_osc_message_set_address(char *address, t_cmmjl_osc_message *msg);
void cmmjl_osc_message_set_typetags(char *tt, t_cmmjl_osc_message *msg);
void cmmjl_osc_message_set_data(long argc, char *argv, t_cmmjl_osc_message *msg);
void cmmjl_osc_message_get_address(t_cmmjl_osc_message *msg, char **address);
void cmmjl_osc_message_get_arguments(t_cmmjl_osc_message *msg, int size, t_cmmjl_osc_atom *args);
void cmmjl_osc_free_packet(t_cmmjl_osc_packet *p);
void cmmjl_osc_free_bundle(t_cmmjl_osc_bundle *b);
void cmmjl_osc_free_message(t_cmmjl_osc_message *m);
void cmmjl_osc_set_bundle_id(t_cmmjl_osc_bundle *b);
int cmmjl_osc_is_bundle(t_cmmjl_osc_obj *obj);
