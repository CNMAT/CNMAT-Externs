/*
Wavemaker (intended for constructing wavetables for oscillators~)

Written by Adrian Freed and Michael Zbyszynski, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2005-7, The Regents of 
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
NAME: wavemaker
DESCRIPTION: Additive synthesis of one cycle of a harmonic waveform with specified amplitudes and phases.  (Intended for constructing wavetables in buffer~ objects to use with oscillators~)
AUTHORS: Adrian Freed, Michael Zbyszynski
COPYRIGHT_YEARS: 2005-7
SVN_REVISION: $LastChangedRevision: 864 $
VERSION 1.0: First release
VERSION 1.1: Updates to Peak and RMS output, assist strings
VERSION 1.1.1: line breaks.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/


//




inlets=2;
outlets=3;
setinletassist(1,"list of phases");
setinletassist(0,"list of amplitudes, bang calculates");
setoutletassist(2,"Peak and RMS values");
setoutletassist(1,"amplitude value (float)");
setoutletassist(0,"index (int)");


var phases = new Array();
var amplitudes = new Array();
const defblen = 64*1024;
var blen = defblen;
if (jsarguments.length>1) {
	blen = jsarguments[1];
	if(blen<0 || blen >10000000)
		blen = defblen;

	if (jsarguments.length > 2) {
		// copy amplitude phase duples
		for(var j=0;j<(jsarguments.length-1)/2;j+=2)
		{
			amplitudes[j/2] = jsarguments[j+2];
			phases[j/2] = jsarguments[j+3];
		}
	}
}

post("wavemaker: ready to synthesize a buffer of length ", blen, "\n");

var rphases =0;

var out = new Array(blen);
function list()
{
	var l = arrayfromargs(arguments);

	if(this.inlet==1)
	{
		rphases=0;
		phases =l;
	}
	else
		amplitudes = l;
//post(l);
}
function randomphases()
{
	rphases=1;
}

function bang()
{
	// post("Amp.l = ", amplitudes.length);
	// post("Phases.l = ", phases.length);
	if (amplitudes.length == 0) {
		post("wavemaker: No amplitudes have been specified!  Doing nothing.\n");
		return;
	}
		var max = 0;
		var min = 0;
		var rms = 0;
		var v;
//post(phases,"\na ",amplitudes,"\n","\n");
		if(rphases || (phases.length!=amplitudes.length))
		{
			// randomize the phases
			for(var j=0;j<amplitudes.length;++j)
			{

				phases[j] = Math.random()*2*3.14159265358979323;
				// post("Random phase ", j, ": ", phases[j],"\n");
			}
		}
		for(var i=0;i<blen;++i)
		{
			v = 0;
			for(var j=0;j<amplitudes.length;++j)
				v += amplitudes[j]*Math.sin((j+1)*i*2*3.14159265358979323/blen+phases[j]);

//post(v,"\n");
			if(v>max)
				max = v;
			if(v<min)
				min=v;
			out[i] = v;

			rms += v*v;
		}
	if(-min>max)
			max = -min;
	
	// post("sum of squares: ", rms, "\n");
	// post("mean of squares: ", rms/blen, "\n");
	realrms = Math.sqrt(rms/blen);
	outlet(2, "RMS", realrms);
	outlet(2, "Peak", max);
	//post("RMS to peak ratio: ",realrms/max,"\n");
		if(max>0)
		for(var i=0;i<blen;++i)
		{
			outlet(1,out[i]/max);
			outlet(0,i);
		}
	
}