/*

OSC-unroute.js by Matt Wright, 060626

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2006, The Regents of 
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
NAME: OSC-unroute
DESCRIPTION: Opposite of OSC-route: prepends bits of OSC addresses to existing OSC messages (javascript)
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2006
SVN_REVISION: $LastChangedRevision: 618 $
VERSION 0.1: First release
VERSION 0.1.1: Moved name/value block from infosource.txt to js source file
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

const MAXINOUT = 64;

// jsarguments[0] is the name of the javascript file
var NbInlets = Math.min(jsarguments.length-1,MAXINOUT);
inlets = NbInlets;
outlets = 1;

for ( i=0 ; i < NbInlets ; i++) {
	setinletassist(i,jsarguments[i+1]);  
}
setoutletassist(0, "OSC messages");

function anything()  {
	var a = arrayfromargs(messagename,arguments);
	a.unshift(jsarguments[inlet+1]);  // Add one new element to front
	outlet(0, a);
}
