/* 
	jit.gworld.h

	Copyright 2001-2002 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com
	
*/

#ifndef __JIT_GWORLD_H__
#define __JIT_GWORLD_H__

//utils for 32 bit gworlds
void jit_gworld_clear(GWorldPtr gp, long c);

long jit_gworld_can_coerce_matrix(struct _gworld_conv_info *gc, void *m);
long jit_gworld_matrix_equal_dim(GWorldPtr gp, void *m);
t_jit_err jit_coerce_matrix_pixmap(void *m, PixMap *pm);


#endif //__JIT_GWORLD_H__
