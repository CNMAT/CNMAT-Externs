/*
Copyright (c) 1998,99,2000,01,02,03,04,05,06,07
The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: lcm (least common multiple)
DESCRIPTION: Least common multiple
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 1998,99,2000,01,02,03,04,05,06,07
SVN_REVISION: $LastChangedRevision$
VERSION 0.1: Matt's initial version, 3/29/98.
VERSION 0.2: First release; fixed bug with typed-in arguments
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/

   
#include "ext.h"
#include "version.h"
#include "version.c"

#define MAXSIZE 32

typedef struct lcm
{
	struct object l_ob;
	long l_args[MAXSIZE];
	long l_count;
	long l_theLCM;
	void *l_out;
} LCM;

fptr *FNS;
void *class;

void lcm_bang(LCM *x);
void lcm_int(LCM *x, long n);
void lcm_in1(LCM *x, long n);
void lcm_in2(LCM *x, long n);
void lcm_in3(LCM *x, long n);
void lcm_in4(LCM *x, long n);
void lcm_in5(LCM *x, long n);
void lcm_in6(LCM *x, long n);
void lcm_in7(LCM *x, long n);
void lcm_in8(LCM *x, long n);
void lcm_in9(LCM *x, long n);
void lcm_tellmeeverything(LCM *x);

void lcm_list(LCM *x, Symbol *s, short ac, Atom *av);
void *lcm_new(Symbol *s, short ac, Atom *av);
long gcd (long a, long b);
long lcm (long a, long b);

void lcm_findLCM(LCM *x);

void main(fptr *f)
{
	version(0);
	FNS = f;	
		
	setup((t_messlist **)&class, (method)lcm_new,0L, (short)sizeof(LCM), 0L, A_GIMME, 0);
	addbang((method)lcm_bang);
	addint((method)lcm_int);
	addinx((method)lcm_in1,1);
	addinx((method)lcm_in2,2);
	addinx((method)lcm_in3,3);
	addinx((method)lcm_in4,4);
	addinx((method)lcm_in5,5);
	addinx((method)lcm_in6,6);
	addinx((method)lcm_in7,7);
	addinx((method)lcm_in8,8);
	addinx((method)lcm_in9,9);
	addmess((method)lcm_list,"list",A_GIMME,0);
	
	addmess((method)lcm_tellmeeverything,"tellmeeverything",0);
	addmess((method)version,"version",0);

	finder_addclass("Arith/Logic/Bitwise","lcm");
	
	version(0);
}

void lcm_bang(LCM *x) {	
	lcm_findLCM(x);
	outlet_int(x->l_out,x->l_theLCM);
}

void lcm_int(LCM *x, long n)
{
	x->l_args[0] = n;
	lcm_bang(x);
}

void lcm_in1(LCM *x, long n) {
	x->l_args[1] = n;
}

void lcm_in2(LCM *x, long n) {
	x->l_args[2] = n;
}

void lcm_in3(LCM *x, long n) {
	x->l_args[3] = n;
}

void lcm_in4(LCM *x, long n) {
	x->l_args[4] = n;
}

void lcm_in5(LCM *x, long n) {
	x->l_args[5] = n;
}

void lcm_in6(LCM *x, long n) {
	x->l_args[6] = n;
}

void lcm_in7(LCM *x, long n) {
	x->l_args[7] = n;
}

void lcm_in8(LCM *x, long n) {
	x->l_args[8] = n;
}

void lcm_in9(LCM *x, long n) {
	x->l_args[9] = n;
}

void lcm_tellmeeverything(LCM *x) {
	int i;
	post("LCM object has %ld numbers:", x->l_count);
	for (i = 0; i < x->l_count; ++i) {
		post("  %ld", x->l_args[i]);
	}
	lcm_findLCM(x);
	post("LCM of those numbers is %ld", x->l_theLCM);
}


void lcm_list(LCM *x, Symbol *s, short ac, Atom *av)
{
	register short i;
	if (ac > 10) {
		ac = 10;
		post("lcm: warning: truncating list to 10 elements.");
	}
	
	for (i=0; i < ac; i++) {
		if (av[i].a_type!=A_LONG) {
			post("lcm: list must contain nothing but ints.");
			return;
		}
	}
	for (i=0; i < ac; i++) {
		x->l_args[i] = av[i].a_w.w_long;
	}
	
	x->l_count = ac;

	lcm_bang(x);
}


void *lcm_new(Symbol *s, short ac, Atom *av)
{
	LCM *x;
	x = (LCM *)newobject(class);
	
	x->l_args[0] = 1;

	if (ac == 0) {
		x->l_count = 2;
		x->l_args[1] = 1;
		intin(x, 1);
	} else if (ac >= 10) {
		ouchstring("Too many arguments to lcm (max is 9)");
		return 0;
	} else {
		int i;
		for (i = ac-1; i >= 0; --i) {
			if (av->a_type != A_LONG) {
				ouchstring("Arguments to lcm must be ints");
				return 0;
			}
			if (i > 0) {
				intin(x, i);
			}
			x->l_args[i] = av[i].a_w.w_long;
		}
		x->l_count = ac;
	}
	
	x->l_out = intout(x);
	return (x);
}

long gcd (long a, long b) {
	long remainder;
	while (b != 0) {
		remainder = a % b;
		a = b;
		b = remainder;
	}
	return a;
}

long lcm (long a, long b) {
	return a * b / gcd(a, b);
}

void lcm_findLCM(LCM *x) {
	long result, i;
	
	result = x->l_args[0];
	for (i = 1; i < x->l_count; ++i) {
		result = lcm(result, x->l_args[i]);
	}
	
	x->l_theLCM = result;
}
