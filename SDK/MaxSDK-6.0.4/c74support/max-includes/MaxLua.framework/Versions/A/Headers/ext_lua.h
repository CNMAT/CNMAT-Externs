#ifndef _MAX_LUA_H_
#define _MAX_LUA_H_

/*
	Include this header to use Lua via the opaque NOBOX wrapper object only
	
	To access the raw Lua API, use the ext_luaapi.h header instead.
*/

#include "ext.h"
#include "ext_obex.h"

#ifdef __cplusplus
	extern "C" {
#endif // __cplusplus

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

/*
	Call this to initialize the NOBOX object wrapper for lua_State
	
	NOBOX object wrapper API:
	
	Example:
		t_object * l = object_new(CLASS_NOBOX, gensym("lua"));
		lua_State * L = object_method(l, gensym("getstate"));
		object_method(l, gensym("doscript"), gensym("test.lua"));
		t_atom args[2];
		atom_setsym(args, gensym("pi"));
		atom_setfloat(&args[1], 3.14);
		object_method(l, gensym("call"), gensym("print"), 2, args);	
		object_free(l);
*/
void max_lua_init();

/*
	data structure for loading Lua scripts / modules
	from arrays of C strings
*/
typedef struct _lua_module {
	t_object ob;
	char name[64];
	const char **script_listing;
	char *script;
} t_lua_module;

t_lua_module * lua_module_get_module(t_symbol *name);
char * lua_module_get_buffer(t_lua_module *x);
int lua_module_init(void);
t_lua_module * lua_module_new(t_symbol *name, const char **script_listing);
void lua_module_free(t_lua_module *x);


#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef __cplusplus
}
#endif

#endif // _MAX_LUA_H_

