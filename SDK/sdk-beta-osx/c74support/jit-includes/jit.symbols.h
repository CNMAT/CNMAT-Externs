/* 
	jit.symbols.h

	Copyright 2001-2002 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com
		
*/

#ifndef __JIT_SYMBOLS_H__
#define __JIT_SYMBOLS_H__

extern t_symbol *_jit_sym_nothing;
extern t_symbol *_jit_sym_new;
extern t_symbol *_jit_sym_free;
extern t_symbol *_jit_sym_classname;
extern t_symbol *_jit_sym_getname;
extern t_symbol *_jit_sym_getmethod;
extern t_symbol *_jit_sym_get;
extern t_symbol *_jit_sym_set;
extern t_symbol *_jit_sym_register;
extern t_symbol *_jit_sym_char;
extern t_symbol *_jit_sym_long;
extern t_symbol *_jit_sym_float32;
extern t_symbol *_jit_sym_float64;
extern t_symbol *_jit_sym_symbol;
extern t_symbol *_jit_sym_pointer;
extern t_symbol *_jit_sym_object;
extern t_symbol *_jit_sym_atom;
extern t_symbol *_jit_sym_list;
extern t_symbol *_jit_sym_type;
extern t_symbol *_jit_sym_dim;
extern t_symbol *_jit_sym_planecount;
extern t_symbol *_jit_sym_val;
extern t_symbol *_jit_sym_plane;
extern t_symbol *_jit_sym_cell;
extern t_symbol *_jit_sym_jit_matrix;
extern t_symbol *_jit_sym_class_jit_matrix;
extern t_symbol *_jit_sym_togworld;
extern t_symbol *_jit_sym_fromgworld;
extern t_symbol *_jit_sym_frommatrix;
extern t_symbol *_jit_sym_jit_attr_offset;
extern t_symbol *_jit_sym_jit_attr_offset_array;
extern t_symbol *_jit_sym_modified;
extern t_symbol *_jit_sym_lock;
extern t_symbol *_jit_sym_getinfo;
extern t_symbol *_jit_sym_getdata;
extern t_symbol *_jit_sym_outputmatrix;
extern t_symbol *_jit_sym_clear;
extern t_symbol *_jit_sym_err_calculate;
extern t_symbol *_jit_sym_max_jit_classex;
extern t_symbol *_jit_sym_setall;
extern t_symbol *_jit_sym_chuck;
extern t_symbol *_jit_sym_getsize;
extern t_symbol *_jit_sym_getindex;
extern t_symbol *_jit_sym_objptr2index;
extern t_symbol *_jit_sym_append;
extern t_symbol *_jit_sym_insertindex;
extern t_symbol *_jit_sym_deleteindex;
extern t_symbol *_jit_sym_makearray;
extern t_symbol *_jit_sym_reverse;
extern t_symbol *_jit_sym_rotate;
extern t_symbol *_jit_sym_shuffle;
extern t_symbol *_jit_sym_swap;
extern t_symbol *_jit_sym_findfirst;
extern t_symbol *_jit_sym_findall;
extern t_symbol *_jit_sym_methodall;
extern t_symbol *_jit_sym_methodindex;
extern t_symbol *_jit_sym_sort;
extern t_symbol *_jit_sym_matrix_calc;
extern t_symbol *_jit_sym_genframe;
extern t_symbol *_jit_sym_filter;
extern t_symbol *_jit_sym_jit_mop;
extern t_symbol *_jit_sym_newcopy;
extern t_symbol *_jit_sym_setinfo;
extern t_symbol *_jit_sym_jit_linklist;
extern t_symbol *_jit_sym_inputcount;
extern t_symbol *_jit_sym_outputcount;
extern t_symbol *_jit_sym_getinput;
extern t_symbol *_jit_sym_getoutput;
extern t_symbol *_jit_sym_getinputlist;
extern t_symbol *_jit_sym_getoutputlist;
extern t_symbol *_jit_sym_ioname;
extern t_symbol *_jit_sym_matrixname;
extern t_symbol *_jit_sym_outputmode;
extern t_symbol *_jit_sym_matrix;
extern t_symbol *_jit_sym_getmatrix;
extern t_symbol *_jit_sym_typelink;
extern t_symbol *_jit_sym_dimlink;
extern t_symbol *_jit_sym_planelink;
extern t_symbol *_jit_sym_restrict_type;
extern t_symbol *_jit_sym_restrict_planecount;
extern t_symbol *_jit_sym_restrict_dim;
extern t_symbol *_jit_sym_special;
extern t_symbol *_jit_sym_getspecial;
extern t_symbol *_jit_sym_adapt;
extern t_symbol *_jit_sym_decorator;
extern t_symbol *_jit_sym_frommatrix_trunc;
extern t_symbol *_jit_sym_ioproc;
extern t_symbol *_jit_sym_getioproc;
extern t_symbol *_jit_sym_name;
extern t_symbol *_jit_sym_types;
extern t_symbol *_jit_sym_minplanecount;
extern t_symbol *_jit_sym_maxplanecount;
extern t_symbol *_jit_sym_mindim;
extern t_symbol *_jit_sym_maxdim;
extern t_symbol *_jit_sym_gl_points;
extern t_symbol *_jit_sym_gl_lines;
extern t_symbol *_jit_sym_gl_line_strip;
extern t_symbol *_jit_sym_gl_line_loop;
extern t_symbol *_jit_sym_gl_triangles;
extern t_symbol *_jit_sym_gl_tri_strip;
extern t_symbol *_jit_sym_gl_tri_fan;
extern t_symbol *_jit_sym_gl_quads;
extern t_symbol *_jit_sym_gl_quad_strip;
extern t_symbol *_jit_sym_gl_polygon;
extern t_symbol *_jit_sym_gl_tri_grid;
extern t_symbol *_jit_sym_gl_quad_grid;


#endif // __JIT_SYMBOLS_H__
