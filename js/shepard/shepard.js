/* 
Shepard tone synthesis 1.0

Written by Adrian Freed, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2005-6, The Regents of 
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
NAME: Shepard
DESCRIPTION: Shepard tone (infinite glissando) synthesis
AUTHORS: Adrian Freed
COPYRIGHT_YEARS: 2005-6
SVN_REVISION: $LastChangedRevision: 618 $
VERSION 1.0: First release
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/


inlets=1;
outlets=1;

var n=8; // number of partials
var f0=80; // frequency f0 in Hz
var f=1; // normalized frequency
var offset=0;
function bang()
{
	msg_float(1.0594633);
}
function msg_float(ff)
{
	f *=ff;
	var pi = Math.pow(1.0594633,12);
//	post(pi);
	var l=new Array();

	while(f<1)
	{
		f = f-1+pi;
		offset = (offset+n-1)%n;
	}
	while(f>pi )
	{
		f = f-pi+1;
		offset = (offset+1)%n;
	
	}


	var freq;
	for(var j=0;j<n;++j)
	{
		freq = f* Math.pow(pi,j) ;
		l[((j+n-offset)%n)*2] = f0*freq;
	//l[((j+n-offset)%n)*2+1] = Math.exp(-4/n*(f+j-n/2)*(f+j -n/2)); // gaussian works, many hat functions do in fact
		l[((j+n-offset)%n)*2+1] = Math.pow(10,0.075*(80*0.5*(1+Math.cos(3.14159265358979*(f+j-n/2)/((n+pi)/2)))-80)); // Shepard used a raised cosine
	}
	outlet(0,l);
}


function pitch(f)
{
	if(f>0 && f<1000000)
		f0 = f;
}
function partials(p)
{
	if(p>0 && p<10000000)
		n = p;
}