/*

Written by Takahiko Suzuki, The Center for New Music and Audio
Technologies, University of California, Berkeley.  Copyright (c)
2000,01,02,03,04,05,06, The Regents of the University of California
(Regents).

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
NAME: divcount~
DESCRIPTION: "Divided count~," outputting the continuously-increasing signal out each active outlet in turn.  Used for "sequential looping".
AUTHORS: Takahiko Suzuki
COPYRIGHT_YEARS: 2000,01,02,03,04,05,06
DRUPAL_NODE: /patch/4070
SVN_REVISION: $LastChangedRevision$
VERSION 000308: Takahiko's final version.
VERSION 309: Built for UB by Matt Wright
VERSION 309.1: Force Package Info Generation
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/



#pragma	mark	HEADER
#include <string.h>
#include "ext.h"
#include "z_dsp.h"

/*
	ctrl  stat1	len1  stat2	len2  stat3	len3  stat4	len4  stat5	len5
	==----==----==----==----==----==----==----==----==----==----==
	| divcount~ 5												 |
	==----------==----------==----------==----------==----------==
	master		out1		out2		out3		out4		out5
	
	args:	integer of num of output.
	inlet:
		ctrl:	bang: n/a for now.... maybe just for start button.....
				 int: start from <int> if it is running, or set current position
			 "start": start from current position
			  "stop": stop running
			  "info": post parameters. just for checking purpose only.
		lenx: positive integer of sample length
	   statx: on/off status. 0 or non-zero.
	
	outlet:
		master: index out. always out.
		  outx: index
*/


#define VERSION		"000308"
#define	MAX_CHAN	16
#define	BETWEEN(x, y, z)	(((x>=y)&&(x<=z))?true:false)

void *z_divcount_class;

typedef struct{
	void	*len;
	void	*stat;
	int		status;
	long	length;
	long	offset;
} channel;

typedef struct{
	t_pxobject	x_obj;
	Boolean		run;
	int			num;
	long		pos;
	long		length;
	channel		*ch;
	long		inlet_number;
	t_sample	dis_out;
} t_z_divcount;

#pragma	mark	PROTOTYPES
////////////////////////////////////////////////////////////////////////////////////////////////
//	Prototypes																				  //
////////////////////////////////////////////////////////////////////////////////////////////////
void *z_divcount_new(long n);
void z_divcount_free(t_z_divcount *x);
void z_divcount_bang(t_z_divcount *x);
void z_divcount_int(t_z_divcount *x, long n);
void z_divcount_dis(t_z_divcount *x, double n);
void z_divcount_start(t_z_divcount *x);
void z_divcount_stop(t_z_divcount *x);
void z_divcount_info(t_z_divcount *x);
t_int *z_divcount_perform(t_int *w);
void z_divcount_dsp(t_z_divcount *x, t_signal **sp, short *count);
void z_divcount_assist(t_z_divcount *x, void *b, long m, long a, char *s);
char *strcpy(char *s1, const char *s2);

#pragma	mark	-
////////////////////////////////////////////////////////////////////////////////////////////////
//	main																					  //
////////////////////////////////////////////////////////////////////////////////////////////////
void main(void){
	setup((t_messlist **)&z_divcount_class, (method)z_divcount_new, (method)z_divcount_free, 
		  (short)sizeof(t_z_divcount), 0L, A_DEFLONG, 0);
	
	addmess((method)z_divcount_dsp, "dsp", A_CANT, 0);
	addmess((method)z_divcount_assist,"assist",A_CANT,0);
	
	addbang((method)z_divcount_bang);
	addint((method)z_divcount_int);
	addmess((method)z_divcount_start,"start",A_NOTHING,0);
	addmess((method)z_divcount_stop,"stop",A_NOTHING,0);
	addmess((method)z_divcount_info,"info",A_NOTHING,0);
	addmess((method)z_divcount_dis,"dis",A_DEFFLOAT,0);

	dsp_initclass();
	
	finder_addclass("Z-DSP", "divcount~");
	post("divcount~ v.%s Copyright©2000 Takahiko Suzuki", VERSION);	//	should be "Regents of University of California, etc..." ??
}

#pragma	mark	-
////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_new																			  //
////////////////////////////////////////////////////////////////////////////////////////////////
void *z_divcount_new(long n){	
	t_z_divcount *x = (t_z_divcount *)newobject(z_divcount_class); 
	
	if(n<2){
		n = 2;
	//	error("divcount~: num of channels can't be less than 2. Bound to 2.");	
	}
	if(n>MAX_CHAN){
		n = MAX_CHAN;
	//	error("divcount~: num of channels can't be greater than 16. Bound to 16.");	
	}
	x->num = n;
	x->run = false;
	x->pos = 0;
	x->length = 0;
	x->dis_out = -1.;
	
	dsp_setup((t_pxobject *)x, 0);			//	no signal inlet
	outlet_new((t_pxobject *)x, "signal");	//	outlet for master index
	
	x->ch = (channel*)getbytes(sizeof(channel) * n);
	if(!(x->ch)){
		error("divcount~: ouch!! memory allocation failed!!");
		return 0;
	}
	
	while(n--){
		x->ch[n].stat = proxy_new(x, n*2+2, &x->inlet_number);	//	status inlet for channel x
		x->ch[n].len = proxy_new(x, n*2+1, &x->inlet_number);	//	length inlet for channel x
		outlet_new((t_pxobject *)x, "signal");					//	outlet for channel x
		x->ch[n].status = x->ch[n].length = x->ch[n].offset = 0;
	}
	return (x);
}
	
////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_free																			  //
////////////////////////////////////////////////////////////////////////////////////////////////
void z_divcount_free(t_z_divcount *x){
	long	i;
	
	for(i=0;i<x->num;i++){
		freeobject(x->ch[i].len);
		freeobject(x->ch[i].stat);
	}
	freebytes(x->ch, sizeof(channel) * x->num);
	dsp_free((t_pxobject *)x);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_assist																		  //
////////////////////////////////////////////////////////////////////////////////////////////////
void z_divcount_assist(t_z_divcount *x, void *b, long m, long a, char *s){
	if(!a){
		switch(m){
			case 1:
				strcpy(s, "bang, int, start, stop, info");
				break;
			case 2:
				strcpy(s, "master index out");
				break;
			default:
				strcpy(s, "Call support!! 1-800-324-5511");;
		}
	}
	else{
		switch(m){
			case 1:
				if(a%2)	sprintf(s, "status of ch.%ld", (a+1)/2);
				else	sprintf(s, "length of ch.%ld", a/2);
				break;
			case 2:
				sprintf(s, "index out of ch.%ld", a);
				break;
			default:
				strcpy(s, "Call support!! 1-800-324-5512");;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_bang																			  //
////////////////////////////////////////////////////////////////////////////////////////////////
void z_divcount_bang(t_z_divcount *x){
	if(x->inlet_number)	error("divcount~: bang should be sent to leftmost inlet.");
	else				x->run = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_int																			  //
////////////////////////////////////////////////////////////////////////////////////////////////
void z_divcount_int(t_z_divcount *x, long n){
	if(!x->inlet_number){
		x->pos = n % x->length;
	}
	else{
		int	i, flag = -1;
		
		if(x->inlet_number%2 == 0){	//	length
			x->ch[x->inlet_number/2-1].length = n;
		}
		else{						//	status
			int	chan = (x->inlet_number-1)/2;
			
			if(x->ch[chan].status && !n){	//	1 -> 0
				if(x->pos >= x->ch[chan].offset){
					
					if(x->pos - x->ch[chan].offset < x->ch[chan].length)
						flag = chan;
					else
						x->pos -= x->ch[chan].length;
				}
			}
			else if(!x->ch[chan].status && n){	//	0 -> 1
				if(x->pos > x->ch[chan].offset){
					x->pos += x->ch[chan].length;
				}
			}
			
			x->ch[chan].status = n;
		}
		
		x->length = 0;
		for(i=0;i<x->num;i++){
			x->ch[i].offset = x->length;
			x->length += x->ch[i].length * x->ch[i].status;
		}
		if(flag>=0){
			x->pos = x->ch[(flag+1)%x->num].offset;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_start																		  //
////////////////////////////////////////////////////////////////////////////////////////////////
void z_divcount_start(t_z_divcount *x){
	if(x->inlet_number)	error("divcount~: message should be sent to leftmost inlet.");
	else				x->run = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_stop																			  //
////////////////////////////////////////////////////////////////////////////////////////////////
void z_divcount_stop(t_z_divcount *x){
	if(x->inlet_number)	error("divcount~: message should be sent to leftmost inlet.");
	else				x->run = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_dis																			  //
////////////////////////////////////////////////////////////////////////////////////////////////
void z_divcount_dis(t_z_divcount *x, double n){
	if(x->inlet_number)	error("divcount~: message should be sent to leftmost inlet.");
	else				x->dis_out = (t_sample) n;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_info																			  //
////////////////////////////////////////////////////////////////////////////////////////////////
void z_divcount_info(t_z_divcount *x){
	long	i;
	
	post("");
	post("divcount~ v.%s", VERSION);
	post("  total length = %ld", x->length);
	for(i=0;i<x->num;i++){
		post("  %2d: %c    l = %8ld", i+1, x->ch[i].status?'x':'-', x->ch[i].length);
	//	post("  %2d: s = %d    l = %ld    offset = %ld", i+1, x->ch[i].status, x->ch[i].length, x->ch[i].offset);
	}
	post("");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_dsp																			  //
////////////////////////////////////////////////////////////////////////////////////////////////
void z_divcount_dsp(t_z_divcount *x, t_signal **sp, short *count){
	switch(x->num){
		case 2:
			dsp_add(z_divcount_perform, 5, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec);
			break;
		case 3:
			dsp_add(z_divcount_perform, 6, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec);
			break;
		case 4:
			dsp_add(z_divcount_perform, 7, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec);
			break;
		case 5:
			dsp_add(z_divcount_perform, 8, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec);
			break;
		case 6:
			dsp_add(z_divcount_perform, 9, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec);
			break;
		case 7:
			dsp_add(z_divcount_perform, 10, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec);
			break;
		case 8:
			dsp_add(z_divcount_perform, 11, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec,
															sp[8]->s_vec);
			break;
		case 9:
			dsp_add(z_divcount_perform, 12, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec,
															sp[8]->s_vec, sp[9]->s_vec);
			break;
		case 10:
			dsp_add(z_divcount_perform, 13, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec,
															sp[8]->s_vec, sp[9]->s_vec, sp[10]->s_vec);
			break;
		case 11:
			dsp_add(z_divcount_perform, 14, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec,
															sp[8]->s_vec, sp[9]->s_vec, sp[10]->s_vec, sp[11]->s_vec);
			break;
		case 12:
			dsp_add(z_divcount_perform, 15, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec,
															sp[8]->s_vec, sp[9]->s_vec, sp[10]->s_vec, sp[11]->s_vec,
															sp[12]->s_vec);
			break;
		case 13:
			dsp_add(z_divcount_perform, 16, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec,
															sp[8]->s_vec, sp[9]->s_vec, sp[10]->s_vec, sp[11]->s_vec,
															sp[12]->s_vec, sp[13]->s_vec);
			break;
		case 14:
			dsp_add(z_divcount_perform, 17, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec,
															sp[8]->s_vec, sp[9]->s_vec, sp[10]->s_vec, sp[11]->s_vec,
															sp[12]->s_vec, sp[13]->s_vec, sp[14]->s_vec);
			break;
		case 15:
			dsp_add(z_divcount_perform, 18, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec,
															sp[8]->s_vec, sp[9]->s_vec, sp[10]->s_vec, sp[11]->s_vec,
															sp[12]->s_vec, sp[13]->s_vec, sp[14]->s_vec, sp[15]->s_vec);
			break;
		case 16:
			dsp_add(z_divcount_perform, 19, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
															sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec,
															sp[8]->s_vec, sp[9]->s_vec, sp[10]->s_vec, sp[11]->s_vec,
															sp[12]->s_vec, sp[13]->s_vec, sp[14]->s_vec, sp[15]->s_vec,
															sp[16]->s_vec);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	z_divcount_perform																		  //
////////////////////////////////////////////////////////////////////////////////////////////////
t_int *z_divcount_perform(t_int *w){
	t_z_divcount	*x = (t_z_divcount *)(w[1]);
	int				n = (int)(w[2]);
	t_sample		*master_out = (t_sample*)(w[3]);
	t_sample		*out;
	int				i, j;
	
	if(!x->run){
		for(i=0;i<=x->num;i++){
			out = (t_sample*)(w[i+3]);
			for(j=0;j<n;j++)
				out[j] = x->dis_out;
		}
		goto exit;
	}
	
	for(i=0;i<n;i++){
		x->pos %= x->length;
		for(j=0;j<x->num;j++){
			out = (t_sample*)(w[j+4]);
			out[i] = x->dis_out;
			if(x->length){
				if(j == x->num-1 && x->pos >= x->ch[x->num-1].offset){	//	last buffer
					out[i] = x->pos - x->ch[j].offset;
				//	x->cur_pos = j;
				}
				else if(x->pos >= x->ch[j].offset && x->pos < x->ch[j+1].offset){
					out[i] = x->pos - x->ch[j].offset;
				//	x->cur_pos = j;
				}
			}
		}
	
		if(x->length){
			master_out[i] = x->pos;
			x->pos++;
		}
		else{
			master_out[i] = x->dis_out;
		}
	}

exit:
	return (w+x->num+4);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	strcpy																					  //
////////////////////////////////////////////////////////////////////////////////////////////////
char *strcpy(char *s1, const char *s2){
	char *ret = s1;
	
	while((*s1++ = *s2++) != 0)
		;
	return ret;
}

