#ifndef _MAX_LUA_API_H_
#define _MAX_LUA_API_H_

/*
	Include this header to use Lua with access to the raw Lua API
	
	If raw access to the Lua API is not needed, use ext_lua.h instead.
*/

#include "ext_lua.h"

#ifdef __cplusplus
	extern "C" {
#endif // __cplusplus

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

#define DEBUG_STACKTRACE "DEBUG_STACKTRACE"


/*
	NOBOX object wrapper for lua_State
*/
typedef struct {
	t_object		m_ob;
	lua_State		* L; /* note that access to G must be single-threaded! */
} t_lua;

t_lua * lua_create();
void lua_free(t_lua * x);
void lua_doscript(t_lua * x, t_symbol * fullpath);
void lua_dolisting(t_lua *x, const char **listing);
void lua_dostring(t_lua *x, const char *code);
t_max_err lua_docall(t_lua * x, t_symbol * funcname, long ac, t_atom * av);
void lua_trycall(t_lua * x, t_symbol * funcname, long ac, t_atom * av);
lua_State * lua_getstate(t_lua * x);

/*
	Push and get the NOBOX Lua object into/from the lua_State
*/
void lua_push_udata(lua_State *L, t_lua *x);
t_lua * lua_to_udata(lua_State *L);

/*
	Convert a string listing (array of strings) to a buffer (flat C string)
*/
char * lua_get_buffer(const char **listing);

/*
	Free a string buffer
*/
void lua_free_buffer(char *buffer);


/*
	Push a t_atom onto the stack
		atoms of type A_SYM will push a string
		atoms of type A_FLOAT or A_LONG will push a lua_Number (double)
		atoms of type A_OBJ will use lua_push_max_object() (with garbage collection disabled)
		atoms of other types will push nil
*/
int lua_max_push_atom(lua_State * L, t_atom a);
int lua_max_push_atom_array(lua_State * L, t_atomarray * atomarray);

/* 
	pushes a string based on the atom's a_type
		e.g. a float atom will push the string 'A_FLOAT'
*/
void lua_push_atom_type(lua_State * L, short type);


/*
	Attempt to retrieve a t_atom from the stack
		booleans and whole numbers will return atoms of type A_LONG
		other numbers will return atoms of type A_FLOAT
		strings will return atoms of type A_SYM
			(watch out for creating lots of symbols...)
		full userdata will try to use lua_to_max_object()
		lightuserdata and coroutines will return atoms of type A_OBJ
		other values will return atoms of type A_NOTHING
*/
int lua_max_to_atom(lua_State * L, int idx, t_atom * atom);
int lua_max_to_atom_array(lua_State * L, int idx, t_atomarray * atomarray);

/*
	Push a t_object onto the stack
		(attempts to lazily define metatable based on the associated t_class)
		garbagecollect == 1 enables garbage collection of the object in Lua
			(Lua will call object_free() on it when no longer in scope)
		garbagecollect == 0 disables garbage collection from Lua
*/
void lua_push_max_object(lua_State * L, t_object * x, int garbagecollect);

/*
	Attempt to retrieve a Max object from the value at stack index idx
*/
t_object * lua_to_max_object(lua_State * L, int idx);
t_object * lua_to_max_object_typed(lua_State * L, int ud, t_class * cls);


/*
	Run a string of code in a Lua state
		args is the number of arguments on the stack to pass into the code
*/
int lua_max_dostring(lua_State * L, const char * code, int args);


/*
	Call a function in protected mode so errors don't trigger a panic
*/
t_max_err lua_max_pcall(lua_State *L, int nargs, int nres, t_symbol **errmsg);

/*
	Let Lua gracefully handle Max t_max_err errors as an error in Lua
*/
void lua_max_error(lua_State * L, t_max_err err);

/*
	Lua function equivalent of post()
*/
int lua_max_print(lua_State * L);

/* 
	useful while debugging: 
		post() the values of each element on the stack 
*/
void lua_max_stackdump(lua_State * L, const char * label);



/*
	Load, or preload, a module using a given name
*/
void lua_max_openlib(lua_State * L, lua_CFunction lib, const char * name);
void lua_max_preloadlib(lua_State * L, lua_CFunction lib, const char * name);

/*
	Append a function to Lua, to use when searching for modules with require()
*/
void lua_max_add_loader(lua_State *L, lua_CFunction findmodule);

/*
	loader for t_lua_module, to use when searching for modules with require()
*/	
int lua_module_find(lua_State *L);

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef __cplusplus
}
#endif

#endif // _MAX_LUA_API_H_

