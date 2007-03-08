/*
running-average (intended for constructing wavetables for oscillators~)

Written by John MacCallum, The Center for New Music and Audio Technologies,
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
NAME: running-average
DESCRIPTION: Computes a running average of the input
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006-7
SVN_REVISION: $LastChangedRevision: 618 $
VERSION 1.0: First release
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

inlets = 1;
outlets = 1;

var avg_length = jsarguments[1];
var avg_array = new Array();
var avg_sum = 0;

function msg_float(x){
	addNewValue(x);
}

function msg_int(x){
	addNewValue(x);
}

function addNewValue(x){
	var popped;
	if(avg_array.length == avg_length){
		popped = avg_array.pop();
		avg_sum = avg_sum - popped;
	}
		
	avg_array.unshift(x);
	avg_sum = avg_sum + x;
}

function length(i){
	avg_length = i;
}

function output_average(){
	outlet(0, avg_sum / avg_array.length);
}

function bang(){
	output_average();
}

function clear(){
	avg_array = null;
	avg_array = new Array();
	avg_sum = 0;
}

function length(l){
	clear();
	avg_length = l;
}

