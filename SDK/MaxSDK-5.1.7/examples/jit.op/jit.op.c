/* 
	Copyright 2001 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com	
*/

#include "jit.common.h"
#include "jit.op.h"

typedef struct _jit_op 
{
	t_object	ob;
	long		opsymcount;
	t_symbol	*opsym[JIT_MATRIX_MAX_PLANECOUNT];
} t_jit_op;

typedef struct _jit_op_vecdata
{
	t_jit_op_fn		opfn[JIT_MATRIX_MAX_PLANECOUNT];
} t_jit_op_vecdata;

void *_jit_op_class;

t_jit_err jit_op_init(void); 
t_jit_op *jit_op_new(void);
void jit_op_free(t_jit_op *x);
t_jit_err jit_op_getvecdata(t_jit_op *x, t_jit_op_vecdata *vd);
t_jit_err jit_op_matrix_calc(t_jit_op *x, void *inputs, void *outputs);

void jit_op_calculate_ndim(t_jit_op_vecdata *vecdata, long dim, long *dimsize, long planecount, t_jit_matrix_info *in1_minfo, char *bip1, 
	t_jit_matrix_info *in2_minfo, char *bip2, t_jit_matrix_info *out_minfo, char *bop);

void jit_op_vector_ignore	(long n, void *vecdata, t_jit_op_info *in1, t_jit_op_info *in2, t_jit_op_info *out); 


//arith
t_symbol *ps_pass,*ps_mult,*ps_div,*ps_add,*ps_sub,*ps_addm,*ps_subm,*ps_mod,*ps_min,*ps_max,*ps_abs,*ps_avg,*ps_absdiff;
//fliparith
t_symbol *ps_flippass,*ps_flipdiv,*ps_flipsub,*ps_flipmod;
//bitwise
t_symbol *ps_bitand,*ps_bitor,*ps_bitxor,*ps_bitnot,*ps_rshift,*ps_lshift;
//logical
t_symbol *ps_and,*ps_or,*ps_xor,*ps_not,*ps_gt,*ps_lt,*ps_gte,*ps_lte,*ps_eq,*ps_neq;
//logical pass
t_symbol *ps_gtp,*ps_ltp,*ps_gtep,*ps_ltep,*ps_eqp,*ps_neqp;
//trancendental
//trig
t_symbol *ps_sin,*ps_cos,*ps_tan,*ps_asin,*ps_acos,*ps_atan,*ps_atan2;
//hyperbolic trig
t_symbol *ps_sinh,*ps_cosh,*ps_tanh,*ps_asinh,*ps_acosh,*ps_atanh;
//exponential/logarithmic
t_symbol *ps_exp,*ps_exp2,*ps_ln,*ps_log2,*ps_log10,*ps_hypot,*ps_pow,*ps_sqrt;
//rounding
t_symbol *ps_ceil,*ps_floor,*ps_round,*ps_trunc;

t_jit_err jit_op_init(void) 
{
	long attrflags=0;
	t_jit_object *attr,*mop;
	t_symbol *atsym;
	
	atsym = gensym("jit_attr_offset");
	
	_jit_op_class = jit_class_new("jit_op",(method)jit_op_new,(method)jit_op_free,
		sizeof(t_jit_op),0L);
	
	//add mop
	mop = jit_object_new(_jit_sym_jit_mop,2,1);
	jit_class_addadornment(_jit_op_class,mop);
	//add methods
	jit_class_addmethod(_jit_op_class, (method)jit_op_matrix_calc, 		"matrix_calc", 		A_CANT, 0L);
	//add attributes	
	attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;
	attr = jit_object_new(_jit_sym_jit_attr_offset_array,"op",_jit_sym_symbol,JIT_MATRIX_MAX_PLANECOUNT,attrflags,
		(method)0L,(method)0L,calcoffset(t_jit_op,opsymcount),calcoffset(t_jit_op,opsym));
	jit_class_addattr(_jit_op_class,attr);

	jit_class_register(_jit_op_class);

    //arith
	ps_pass			= gensym("pass");
	ps_mult	 		= gensym("*");
	ps_div 			= gensym("/");
	ps_add	 		= gensym("+");
	ps_sub		 	= gensym("-");
	ps_addm	 		= gensym("+m");
	ps_subm		 	= gensym("-m");
	ps_mod		 	= gensym("%");
	ps_min			= gensym("min");
	ps_max			= gensym("max");
	ps_abs			= gensym("abs");
	ps_avg			= gensym("avg");
	ps_absdiff		= gensym("absdiff");
	
	//fliparith
	ps_flippass		= gensym("!pass");
	ps_flipdiv 		= gensym("!/");
	ps_flipsub		= gensym("!-");
	ps_flipmod		= gensym("!%");

	//bitwise
	ps_bitand		= gensym("&");
	ps_bitor		= gensym("|");
	ps_bitxor		= gensym("^");
	ps_bitnot		= gensym("~");
	ps_rshift		= gensym(">>");
	ps_lshift		= gensym("<<");
	
	//logical
	ps_and			= gensym("&&");
	ps_or			= gensym("||");
	ps_not			= gensym("!");
	ps_gt			= gensym(">");
	ps_lt			= gensym("<");
	ps_gte			= gensym(">=");
	ps_lte			= gensym("<=");
	ps_eq			= gensym("==");
	ps_neq			= gensym("!=");
	
	//logical pass
	ps_gtp			= gensym(">p");
	ps_ltp			= gensym("<p");
	ps_gtep			= gensym(">=p");
	ps_ltep			= gensym("<=p");
	ps_eqp			= gensym("==p");
	ps_neqp			= gensym("!=p");
	
	
	//trancendental
	//trig
	ps_sin			= gensym("sin");
	ps_cos			= gensym("cos");
	ps_tan			= gensym("tan");
	ps_asin			= gensym("asin");
	ps_acos			= gensym("acos");
	ps_atan			= gensym("atan");
	ps_atan2		= gensym("atan2");
	
	//hyperbolic trig
	ps_sinh			= gensym("sinh");
	ps_cosh			= gensym("cosh");
	ps_tanh			= gensym("tanh");
	ps_asinh		= gensym("asinh");
	ps_acosh		= gensym("acosh");
	ps_atanh		= gensym("atanh");

	//exponential/logarithmic
	ps_exp			= gensym("exp");
	ps_exp2			= gensym("exp2");
	ps_ln			= gensym("ln");
	ps_log2			= gensym("log2");
	ps_log10		= gensym("log10");
	ps_hypot		= gensym("hypot");
	ps_pow			= gensym("pow");
	ps_sqrt			= gensym("sqrt");

	//rounding
	ps_ceil			= gensym("ceil");
	ps_floor		= gensym("floor");
	ps_round		= gensym("round");
	ps_trunc		= gensym("trunc");

	return JIT_ERR_NONE;
}

void jit_op_vector_ignore	(long n, void *vecdata, t_jit_op_info *in1, t_jit_op_info *in2, t_jit_op_info *out)
{
	//nada
}

t_jit_op_fn jit_op_sym2fn(t_symbol *opsym, t_symbol *type)
{
	if (type==_jit_sym_char) {
		if (opsym==ps_pass) {						//arith(8bit fixed)
			return jit_op_vector_pass_char;
		} else if (opsym==ps_mult) {
			return jit_op_vector_mult_char;
		} else if (opsym==ps_div) {
			return jit_op_vector_div_char;
		} else if (opsym==ps_add) { 
			return jit_op_vector_adds_char;
		} else if (opsym==ps_sub) { 
			return jit_op_vector_subs_char;
		} else if (opsym==ps_addm) { 
			return jit_op_vector_add_char;
		} else if (opsym==ps_subm) { 
			return jit_op_vector_sub_char;
		} else if (opsym==ps_mod) {
			return jit_op_vector_mod_char;
		} else if (opsym==ps_min) {
			return jit_op_vector_min_char;
		} else if (opsym==ps_max) {
			return jit_op_vector_max_char;
		} else if (opsym==ps_avg) {
			return jit_op_vector_avg_char;
		} else if (opsym==ps_absdiff) {
			return jit_op_vector_absdiff_char;
		} else if (opsym==ps_flippass) {			//fliparith(8bit fixed)
			return jit_op_vector_flippass_char;
		} else if (opsym==ps_flipdiv) {
			return jit_op_vector_flipdiv_char;
		} else if (opsym==ps_flipsub) {
			return jit_op_vector_flipsub_char;
		} else if (opsym==ps_flipmod) {
			return jit_op_vector_flipmod_char;
		} else if (opsym==ps_bitand) {				//bitwise
			return jit_op_vector_bitand_char;		
		} else if (opsym==ps_bitor) {
			return jit_op_vector_bitor_char;
		} else if (opsym==ps_bitxor) {
			return jit_op_vector_bitxor_char;
		} else if (opsym==ps_bitnot) {
			return jit_op_vector_bitnot_char;
		} else if (opsym==ps_rshift) {
			return jit_op_vector_rshift_char;
		} else if (opsym==ps_lshift) {
			return jit_op_vector_lshift_char;		
		} else if (opsym==ps_and) {					//logical
			return jit_op_vector_and_char;		
		} else if (opsym==ps_or) {
			return jit_op_vector_or_char;
		} else if (opsym==ps_not) {
			return jit_op_vector_not_char;
		} else if (opsym==ps_gt) {
			return jit_op_vector_gt_char;
		} else if (opsym==ps_lt) {
			return jit_op_vector_lt_char;
		} else if (opsym==ps_gte) {
			return jit_op_vector_gte_char;
		} else if (opsym==ps_lte) {
			return jit_op_vector_lte_char;
		} else if (opsym==ps_eq) {
			return jit_op_vector_eq_char;
		} else if (opsym==ps_neq) {
			return jit_op_vector_neq_char; 
		} else if (opsym==ps_gtp) {					//logical pass
			return jit_op_vector_gtp_char;
		} else if (opsym==ps_ltp) {
			return jit_op_vector_ltp_char;
		} else if (opsym==ps_gtep) {
			return jit_op_vector_gtep_char;
		} else if (opsym==ps_ltep) {
			return jit_op_vector_ltep_char;
		} else if (opsym==ps_eqp) {
			return jit_op_vector_eqp_char;
		} else if (opsym==ps_neqp) {
			return jit_op_vector_neqp_char;
		} else {									//otherwise, ignore
			return jit_op_vector_ignore; 
		}
	} else if (type==_jit_sym_long) {
		if (opsym==ps_pass) {						//arith
			return jit_op_vector_pass_long;
		} else if (opsym==ps_mult) {
			return jit_op_vector_mult_long;
		} else if (opsym==ps_div) {
			return jit_op_vector_div_long;
		} else if (opsym==ps_add) {
			return jit_op_vector_add_long;
		} else if (opsym==ps_sub) {
			return jit_op_vector_sub_long;
		} else if (opsym==ps_mod) {
			return jit_op_vector_mod_long;
		} else if (opsym==ps_min) {
			return jit_op_vector_min_long;
		} else if (opsym==ps_max) {
			return jit_op_vector_max_long;
		} else if (opsym==ps_abs) {
			return jit_op_vector_abs_long;
		} else if (opsym==ps_avg) {
			return jit_op_vector_avg_long;
		} else if (opsym==ps_absdiff) {
			return jit_op_vector_absdiff_long;
		} else if (opsym==ps_flippass) {			//fliparith
			return jit_op_vector_flippass_long;
		} else if (opsym==ps_flipdiv) {
			return jit_op_vector_flipdiv_long;
		} else if (opsym==ps_flipsub) {
			return jit_op_vector_flipsub_long;
		} else if (opsym==ps_flipmod) {
			return jit_op_vector_flipmod_long;
		} else if (opsym==ps_bitand) {				//bitwise
			return jit_op_vector_bitand_long;		
		} else if (opsym==ps_bitor) {
			return jit_op_vector_bitor_long;
		} else if (opsym==ps_bitxor) {
			return jit_op_vector_bitxor_long;
		} else if (opsym==ps_bitnot) {
			return jit_op_vector_bitnot_long;
		} else if (opsym==ps_rshift) {
			return jit_op_vector_rshift_long;
		} else if (opsym==ps_lshift) {
			return jit_op_vector_lshift_long;		
		} else if (opsym==ps_and) {					//logical
			return jit_op_vector_and_long;		
		} else if (opsym==ps_or) {
			return jit_op_vector_or_long;
		} else if (opsym==ps_not) {
			return jit_op_vector_not_long;
		} else if (opsym==ps_gt) {
			return jit_op_vector_gt_long;
		} else if (opsym==ps_lt) {
			return jit_op_vector_lt_long;
		} else if (opsym==ps_gte) {
			return jit_op_vector_gte_long;
		} else if (opsym==ps_lte) {
			return jit_op_vector_lte_long;
		} else if (opsym==ps_eq) {
			return jit_op_vector_eq_long;
		} else if (opsym==ps_neq) {
			return jit_op_vector_neq_long;
		} else if (opsym==ps_gtp) {					//logical pass
			return jit_op_vector_gtp_long;
		} else if (opsym==ps_ltp) {
			return jit_op_vector_ltp_long;
		} else if (opsym==ps_gtep) {
			return jit_op_vector_gtep_long;
		} else if (opsym==ps_ltep) {
			return jit_op_vector_ltep_long;
		} else if (opsym==ps_eqp) {
			return jit_op_vector_eqp_long;
		} else if (opsym==ps_neqp) {
			return jit_op_vector_neqp_long;
		} else {									//otherwise, ignore
			return jit_op_vector_ignore; 
		}
		
	} else if (type==_jit_sym_float32) {
		if (opsym==ps_pass) {						//arith
			return jit_op_vector_pass_float32;
		} else if (opsym==ps_mult) {
			return jit_op_vector_mult_float32;
		} else if (opsym==ps_div) {
			return jit_op_vector_div_float32;
		} else if (opsym==ps_add) {
			return jit_op_vector_add_float32;
		} else if (opsym==ps_sub) {
			return jit_op_vector_sub_float32;
		} else if (opsym==ps_mod) { //no fmod yet
			return jit_op_vector_pass_float32;
		} else if (opsym==ps_min) {
			return jit_op_vector_min_float32;
		} else if (opsym==ps_max) {
			return jit_op_vector_max_float32;
		} else if (opsym==ps_abs) {
			return jit_op_vector_abs_float32;
		} else if (opsym==ps_avg) {
			return jit_op_vector_avg_float32;
		} else if (opsym==ps_absdiff) {
			return jit_op_vector_absdiff_float32;
		} else if (opsym==ps_flippass) {			//fliparith
			return jit_op_vector_flippass_float32;
		} else if (opsym==ps_flipdiv) {
			return jit_op_vector_flipdiv_float32;
		} else if (opsym==ps_flipsub) {
			return jit_op_vector_flipsub_float32;
		} else if (opsym==ps_flipmod) { //no fmod yet
			return jit_op_vector_flippass_float32;
		} else if (opsym==ps_bitand) {				//bitwise(pass?)
			return jit_op_vector_pass_float32;		
		} else if (opsym==ps_bitor) {
			return jit_op_vector_pass_float32;
		} else if (opsym==ps_bitxor) {
			return jit_op_vector_pass_float32;
		} else if (opsym==ps_bitnot) {
			return jit_op_vector_pass_float32;
		} else if (opsym==ps_rshift) {
			return jit_op_vector_pass_float32;
		} else if (opsym==ps_lshift) {
			return jit_op_vector_pass_float32;		
		} else if (opsym==ps_and) {					//logical
			return jit_op_vector_and_float32;		
		} else if (opsym==ps_or) {
			return jit_op_vector_or_float32;
		} else if (opsym==ps_not) {
			return jit_op_vector_not_float32;
		} else if (opsym==ps_gt) {
			return jit_op_vector_gt_float32;
		} else if (opsym==ps_lt) {
			return jit_op_vector_lt_float32;
		} else if (opsym==ps_gte) {
			return jit_op_vector_gte_float32;
		} else if (opsym==ps_lte) {
			return jit_op_vector_lte_float32;
		} else if (opsym==ps_eq) {
			return jit_op_vector_eq_float32;
		} else if (opsym==ps_neq) {
			return jit_op_vector_neq_float32;		
		} else if (opsym==ps_gtp) {					//logical pass
			return jit_op_vector_gtp_float32;
		} else if (opsym==ps_ltp) {
			return jit_op_vector_ltp_float32;
		} else if (opsym==ps_gtep) {
			return jit_op_vector_gtep_float32;
		} else if (opsym==ps_ltep) {
			return jit_op_vector_ltep_float32;
		} else if (opsym==ps_eqp) {
			return jit_op_vector_eqp_float32;
		} else if (opsym==ps_neqp) {
			return jit_op_vector_neqp_float32;
		}											//transcendental
		  else if (opsym==ps_sin) {					//trig
			return jit_op_vector_sin_float32;		
		} else if (opsym==ps_cos) {					
			return jit_op_vector_cos_float32;		
		} else if (opsym==ps_tan) {					
			return jit_op_vector_tan_float32;		
		} else if (opsym==ps_asin) {				
			return jit_op_vector_asin_float32;		
		} else if (opsym==ps_acos) {					
			return jit_op_vector_acos_float32;		
		} else if (opsym==ps_atan) {					
			return jit_op_vector_atan_float32;		
		} else if (opsym==ps_atan2) {					
			return jit_op_vector_atan2_float32;		
		} else if (opsym==ps_sinh) {				//hyperbolic trig
			return jit_op_vector_sinh_float32;		
		} else if (opsym==ps_cosh) {					
			return jit_op_vector_cosh_float32;		
		} else if (opsym==ps_tanh) {					
			return jit_op_vector_tanh_float32;		
		} else if (opsym==ps_asinh) {				
			return jit_op_vector_asinh_float32;		
		} else if (opsym==ps_acosh) {					
			return jit_op_vector_acosh_float32;		
		} else if (opsym==ps_atanh) {					
			return jit_op_vector_atanh_float32;		
		} else if (opsym==ps_exp) {					//exponential/logarithmic
			return jit_op_vector_exp_float32;		
		} else if (opsym==ps_exp2) {				
			return jit_op_vector_exp2_float32;		
		} else if (opsym==ps_ln) {				
			return jit_op_vector_log_float32;		
		} else if (opsym==ps_log2) {				
			return jit_op_vector_log2_float32;		
		} else if (opsym==ps_log10) {				
			return jit_op_vector_log10_float32;		
		} else if (opsym==ps_hypot) {				
			return jit_op_vector_hypot_float32;		
		} else if (opsym==ps_pow) {				
			return jit_op_vector_pow_float32;		
		} else if (opsym==ps_sqrt) {				
			return jit_op_vector_sqrt_float32;		
		} else if (opsym==ps_ceil) {				//rounding
			return jit_op_vector_ceil_float32;		
		} else if (opsym==ps_floor) {				
			return jit_op_vector_floor_float32;		
		} else if (opsym==ps_round) {				
			return jit_op_vector_round_float32;		
		} else if (opsym==ps_trunc) {				
			return jit_op_vector_trunc_float32;		
		} else {									//otherwise, ignore
			return jit_op_vector_ignore; 
		}

	} else if (type==_jit_sym_float64) {
		if (opsym==ps_pass) {						//arith
			return jit_op_vector_pass_float64;
		} else if (opsym==ps_mult) {
			return jit_op_vector_mult_float64;
		} else if (opsym==ps_div) {
			return jit_op_vector_div_float64;
		} else if (opsym==ps_add) {
			return jit_op_vector_add_float64;
		} else if (opsym==ps_sub) {
			return jit_op_vector_sub_float64;
		} else if (opsym==ps_mod) {	//no fmod yet
			return jit_op_vector_pass_float64;
		}  else if (opsym==ps_min) {
			return jit_op_vector_min_float64;
		} else if (opsym==ps_max) {
			return jit_op_vector_max_float64;
		} else if (opsym==ps_abs) {
			return jit_op_vector_abs_float64;
		} else if (opsym==ps_avg) {
			return jit_op_vector_avg_float64;
		} else if (opsym==ps_absdiff) {
			return jit_op_vector_absdiff_float64;
		} else if (opsym==ps_flippass) {			//fliparith
			return jit_op_vector_flippass_float64;
		} else if (opsym==ps_flipdiv) {
			return jit_op_vector_flipdiv_float64;
		} else if (opsym==ps_flipsub) {
			return jit_op_vector_flipsub_float64;
		} else if (opsym==ps_flipmod) { //no fmod yet
			return jit_op_vector_flippass_float64;
		} else if (opsym==ps_bitand) {				//bitwise(pass?)
			return jit_op_vector_pass_float64;		
		} else if (opsym==ps_bitor) {
			return jit_op_vector_pass_float64;
		} else if (opsym==ps_bitxor) {
			return jit_op_vector_pass_float64;
		} else if (opsym==ps_bitnot) {
			return jit_op_vector_pass_float64;
		} else if (opsym==ps_rshift) {
			return jit_op_vector_pass_float64;
		} else if (opsym==ps_lshift) {
			return jit_op_vector_pass_float64;		
		} else if (opsym==ps_and) {					//logical
			return jit_op_vector_and_float64;		
		} else if (opsym==ps_or) {
			return jit_op_vector_or_float64;
		} else if (opsym==ps_not) {
			return jit_op_vector_not_float64;
		} else if (opsym==ps_gt) {
			return jit_op_vector_gt_float64;
		} else if (opsym==ps_lt) {
			return jit_op_vector_lt_float64;
		} else if (opsym==ps_gte) {
			return jit_op_vector_gte_float64;
		} else if (opsym==ps_lte) {
			return jit_op_vector_lte_float64;
		} else if (opsym==ps_eq) {
			return jit_op_vector_eq_float64;
		} else if (opsym==ps_neq) {
			return jit_op_vector_neq_float64;
		}  else if (opsym==ps_gtp) {					//logical pass
			return jit_op_vector_gtp_float64;
		} else if (opsym==ps_ltp) {
			return jit_op_vector_ltp_float64;
		} else if (opsym==ps_gtep) {
			return jit_op_vector_gtep_float64;
		} else if (opsym==ps_ltep) {
			return jit_op_vector_ltep_float64;
		} else if (opsym==ps_eqp) {
			return jit_op_vector_eqp_float64;
		} else if (opsym==ps_neqp) {
			return jit_op_vector_neqp_float64;
		}											//transcendental
		  else if (opsym==ps_sin) {					//trig
			return jit_op_vector_sin_float64;		
		} else if (opsym==ps_cos) {					
			return jit_op_vector_cos_float64;		
		} else if (opsym==ps_tan) {					
			return jit_op_vector_tan_float64;		
		} else if (opsym==ps_asin) {				
			return jit_op_vector_asin_float64;		
		} else if (opsym==ps_acos) {					
			return jit_op_vector_acos_float64;		
		} else if (opsym==ps_atan) {					
			return jit_op_vector_atan_float64;		
		} else if (opsym==ps_atan2) {					
			return jit_op_vector_atan2_float64;		
		} else if (opsym==ps_sinh) {				//hyperbolic trig
			return jit_op_vector_sinh_float64;		
		} else if (opsym==ps_cosh) {					
			return jit_op_vector_cosh_float64;		
		} else if (opsym==ps_tanh) {					
			return jit_op_vector_tanh_float64;		
		} else if (opsym==ps_asinh) {				
			return jit_op_vector_asinh_float64;		
		} else if (opsym==ps_acosh) {					
			return jit_op_vector_acosh_float64;		
		} else if (opsym==ps_atanh) {					
			return jit_op_vector_atanh_float64;		
		} else if (opsym==ps_exp) {					//exponential/logarithmic
			return jit_op_vector_exp_float64;		
		} else if (opsym==ps_exp2) {				
			return jit_op_vector_exp2_float64;		
		} else if (opsym==ps_ln) {				
			return jit_op_vector_log_float64;		
		} else if (opsym==ps_log2) {				
			return jit_op_vector_log2_float64;		
		} else if (opsym==ps_log10) {				
			return jit_op_vector_log10_float64;		
		} else if (opsym==ps_hypot) {				
			return jit_op_vector_hypot_float64;		
		} else if (opsym==ps_pow) {				
			return jit_op_vector_pow_float64;		
		} else if (opsym==ps_sqrt) {				
			return jit_op_vector_sqrt_float64;		
		} else if (opsym==ps_ceil) {				//rounding
			return jit_op_vector_ceil_float64;		
		} else if (opsym==ps_floor) {				
			return jit_op_vector_floor_float64;		
		} else if (opsym==ps_round) {				
			return jit_op_vector_round_float64;		
		} else if (opsym==ps_trunc) {				
			return jit_op_vector_trunc_float64;		
		} else {									//otherwise, ignore
			return jit_op_vector_ignore; 
		}
	}  
	return jit_op_vector_ignore;  // $rbs$ -- warning fix
}

t_jit_err jit_op_matrix_calc(t_jit_op *x, void *inputs, void *outputs)
{
	t_jit_err err=JIT_ERR_NONE;
	long in1_savelock,in2_savelock,out_savelock;
	t_jit_matrix_info in1_minfo,in2_minfo,out_minfo;
	char *in1_bp,*in2_bp,*out_bp;
	long i,dimcount,planecount,dim[JIT_MATRIX_MAX_DIMCOUNT];
	t_jit_op_vecdata	vecdata;
	void *in1_matrix,*in2_matrix,*out_matrix;
	
	in1_matrix 	= jit_object_method(inputs,_jit_sym_getindex,0);
	in2_matrix 	= jit_object_method(inputs,_jit_sym_getindex,1);
	out_matrix 	= jit_object_method(outputs,_jit_sym_getindex,0);

	if (x&&in1_matrix&&in2_matrix&&out_matrix) {
		in1_savelock = (long) jit_object_method(in1_matrix,_jit_sym_lock,1);
		in2_savelock = (long) jit_object_method(in2_matrix,_jit_sym_lock,1);
		out_savelock = (long) jit_object_method(out_matrix,_jit_sym_lock,1);
		jit_object_method(in1_matrix,_jit_sym_getinfo,&in1_minfo);
		jit_object_method(in2_matrix,_jit_sym_getinfo,&in2_minfo);
		jit_object_method(out_matrix,_jit_sym_getinfo,&out_minfo);
		jit_object_method(in1_matrix,_jit_sym_getdata,&in1_bp);
		jit_object_method(in2_matrix,_jit_sym_getdata,&in2_bp);
		jit_object_method(out_matrix,_jit_sym_getdata,&out_bp);
		if (!in1_bp) { err=JIT_ERR_INVALID_INPUT; goto out;}
		if (!in2_bp) { err=JIT_ERR_INVALID_INPUT; goto out;}
		if (!out_bp) { err=JIT_ERR_INVALID_OUTPUT; goto out;}
		//compatible types? later support implicit type conversion
		if ((in1_minfo.type!=in2_minfo.type)||(in1_minfo.type!=out_minfo.type)) { err=JIT_ERR_MISMATCH_TYPE; goto out;}		
		//get dimensions/planecount
		dimcount   = out_minfo.dimcount;
		planecount = out_minfo.planecount;			
		for (i=0;i<dimcount;i++) {
			//if dimsize is 1, treat as infinite domain across that dimension.
			//otherwise truncate if less than the output dimsize
			dim[i] = out_minfo.dim[i];
			if ((in1_minfo.dim[i]<dim[i])&&in1_minfo.dim[i]>1) {
				dim[i] = in1_minfo.dim[i];
			}
			if ((in2_minfo.dim[i]<dim[i])&&in2_minfo.dim[i]>1) {
				dim[i] = in2_minfo.dim[i];
			}
		}
		
		for (i=0;i<planecount;i++) {		
			vecdata.opfn[i] = jit_op_sym2fn(x->opsym[i%x->opsymcount],in1_minfo.type);
		}
		jit_parallel_ndim_simplecalc3((method)jit_op_calculate_ndim,
			&vecdata, dimcount, dim, planecount, &in1_minfo, in1_bp, &in2_minfo, in2_bp, &out_minfo, out_bp,
			0 /* flags1 */, 0 /* flags2 */, 0 /* flags3 */);

	} else {
		return JIT_ERR_INVALID_PTR;
	}
	
out:
	jit_object_method(out_matrix,_jit_sym_lock,out_savelock);
	jit_object_method(in2_matrix,_jit_sym_lock,in2_savelock);
	jit_object_method(in1_matrix,_jit_sym_lock,in1_savelock);
	return err;
}


void jit_op_calculate_ndim(t_jit_op_vecdata *vecdata, long dimcount, long *dim, long planecount, t_jit_matrix_info *in1_minfo, char *bip1, 
	t_jit_matrix_info *in2_minfo, char *bip2, t_jit_matrix_info *out_minfo, char *bop)
{
	long i,j,n;
	char *ip1=bip1,*ip2=bip2,*op=bop;
	t_jit_op_info in1_opinfo,in2_opinfo,out_opinfo;
	double zero=0;
	long onefn;
	method fn;
		
	if (dimcount<1) return; //safety
	
	switch(dimcount) {
	case 1:
		dim[1] = 1;
	case 2:
		//if planecount the same, flatten planes - treat as single plane data for speed
		n = dim[0];
		onefn=1;
		fn = vecdata->opfn[0];
		for (i=0;i<planecount;i++) {
			if (vecdata->opfn[i]!=fn) onefn=0;
		}
		if (onefn&&(in1_minfo->dim[0]>1)&&(in2_minfo->dim[0]>1)&&(out_minfo->dim[0]>1)&&
			(in1_minfo->planecount==in2_minfo->planecount)&&(in1_minfo->planecount==out_minfo->planecount)) {
			in1_opinfo.stride = 1;
			in2_opinfo.stride = 1;
			out_opinfo.stride = 1;
			n *= planecount;
			planecount = 1;
		} else {
			in1_opinfo.stride = in1_minfo->dim[0]>1?in1_minfo->planecount:0;
			in2_opinfo.stride = in2_minfo->dim[0]>1?in2_minfo->planecount:0;
			out_opinfo.stride = out_minfo->dim[0]>1?out_minfo->planecount:0;
		}
		if (in1_minfo->type==_jit_sym_char) {
			for (i=0;i<dim[1];i++){
				for (j=0;j<planecount;j++) {
					in1_opinfo.p = bip1 + i*in1_minfo->dimstride[1] + j%in1_minfo->planecount;
					in2_opinfo.p = bip2 + i*in2_minfo->dimstride[1] + j%in2_minfo->planecount;
					out_opinfo.p = bop  + i*out_minfo->dimstride[1] + j%out_minfo->planecount;
					(*(vecdata->opfn[j]))(n,vecdata,&in1_opinfo,&in2_opinfo,&out_opinfo);
				}
			}
		} else if (in1_minfo->type==_jit_sym_long) {
			for (i=0;i<dim[1];i++){
				for (j=0;j<planecount;j++) {
					in1_opinfo.p = bip1 + i*in1_minfo->dimstride[1] + (j%in1_minfo->planecount)*4;
					in2_opinfo.p = bip2 + i*in2_minfo->dimstride[1] + (j%in2_minfo->planecount)*4;
					out_opinfo.p = bop  + i*out_minfo->dimstride[1] + (j%out_minfo->planecount)*4;
					(*(vecdata->opfn[j]))(n,vecdata,&in1_opinfo,&in2_opinfo,&out_opinfo);
				}
			}
		} else if (in1_minfo->type==_jit_sym_float32) {
			for (i=0;i<dim[1];i++){
				for (j=0;j<planecount;j++) {
					in1_opinfo.p = bip1 + i*in1_minfo->dimstride[1] + (j%in1_minfo->planecount)*4;
					in2_opinfo.p = bip2 + i*in2_minfo->dimstride[1] + (j%in2_minfo->planecount)*4;
					out_opinfo.p = bop  + i*out_minfo->dimstride[1] + (j%out_minfo->planecount)*4;
					(*(vecdata->opfn[j]))(n,vecdata,&in1_opinfo,&in2_opinfo,&out_opinfo);
				}
			}
		} else if (in1_minfo->type==_jit_sym_float64) {
			for (i=0;i<dim[1];i++){
				for (j=0;j<planecount;j++) {
					in1_opinfo.p = bip1 + i*in1_minfo->dimstride[1] + (j%in1_minfo->planecount)*8;
					in2_opinfo.p = bip2 + i*in2_minfo->dimstride[1] + (j%in2_minfo->planecount)*8;
					out_opinfo.p = bop  + i*out_minfo->dimstride[1] + (j%out_minfo->planecount)*8;
					(*(vecdata->opfn[j]))(n,vecdata,&in1_opinfo,&in2_opinfo,&out_opinfo);
				}
			}
		}
		break;
	default:
		for	(i=0;i<dim[dimcount-1];i++) {
			ip1 = bip1 + i*in1_minfo->dimstride[dimcount-1];
			ip2 = bip2 + i*in2_minfo->dimstride[dimcount-1];
			op  = bop  + i*out_minfo->dimstride[dimcount-1];
			jit_op_calculate_ndim(vecdata,dimcount-1,dim,planecount,in1_minfo,ip1,in2_minfo,ip2,out_minfo,op);
		}
	}
}

t_jit_op *jit_op_new(void)
{
	t_jit_op *x;
	long i;
		
	if (x=(t_jit_op *)jit_object_alloc(_jit_op_class)) {
		x->opsymcount = 1;
		for (i=0;i<JIT_MATRIX_MAX_PLANECOUNT;i++)
			x->opsym[i] = NULL;
	} else {
		x = NULL;
	}	
	return x;
}

void jit_op_free(t_jit_op *x)
{
	//nada
}
