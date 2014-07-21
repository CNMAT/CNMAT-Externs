/*

SDIF Menu 2
by Michael Zbyszynski

Copyright (c) 2005-11.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby
granted, provided that the above copyright notice, this paragraph and
the following two paragraphs appear in all copies, modifications, and
distributions.

Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Michael Zbyszynski, The Center for New Music and Audio Technologies,
University of California, Berkeley. 

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: SDIF-menu2
DESCRIPTION: Attractive tabular display of information from SDIF-fileinfo about the streams in an SDIF file.  (jsui)
AUTHORS: Michael Zbyszynski
COPYRIGHT_YEARS: 2005-11
SVN_REVISION: $LastChangedRevision$
VERSION 0.5: This one can draw a little nicer.
VERSION 1.0: Fixed alpha values for newer versions of Max.
VERSION 1.1: Added save methods for recall of colors
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

outlets = 2;
sketch.default2d();
sketch.fsaa = 0;
var title = ["no file", 0];
var streams = new Array();
var h = 0;
var c = 0.;
var hilite = 999;
var j = 999;
var full = 0;

var mybrgb = [0.1, 0.1, 0.2, 1.];
var myfrgb = [0.2, 0.7, 1., 1.];
var myrgb2 = [0.2, 0.1, 0.3, 1.];
var hilitergb = [0.2, 1., 1., 1.];

var myfont = "Sans Serif";  // Geneva on Mac, Arial on PC
var myfontsize = 12;
var offset = 20;

draw();

function anything(murph,b,c)
{
	var murph = arrayfromargs(messagename,arguments);
	if (murph[0] == "/file")	
	{
	clear();
	title[0] = murph[1];
	title[1] = murph[2];
	full = 0;
	draw();
	refresh();
	}
	if (murph[0] == "/stream")
	{
	streams[h] = new astream(murph[1], murph[1], murph[1], murph[1], murph[1]);
	streams[h].number = murph[1];
	streams[h].type = murph[2];
	streams[h].starttime = murph[3];
	streams[h].endtime = murph[4];
	streams[h].dat3 = murph[5];
	h++;
	full = 1;
	if (h > title[1]) h = title[1];
	draw();
	refresh();
	}


}

function msg_int(a)
{
	for (i=0;i<h;i++)
	{
	if (streams[i].number == a) 
	{
	j = i;
	joe = title[0] + streams[0].number;
 	outlet(0, title[0], streams[j].number);
	}
	}
	bang();
	
}

//setter functions for colors -------------------------------------------------

function frgb(r,g,b)
{
	myfrgb[0] = r;
	myfrgb[1] = g;
	myfrgb[2] = b;
	myfrgb[3] = 1.;
	draw();
	refresh();
}

function brgb(r,g,b)
{
	mybrgb[0] = r;
	mybrgb[1] = g;
	mybrgb[2] = b;
	mybrgb[3] = 1.;
	draw();
	refresh();
}

function rgb2(r,g,b)
{
	myrgb2[0] = r;
	myrgb2[1] = g;
	myrgb2[2] = b;
	myrgb2[3] = 1.;
	draw();
	refresh();
}

function highlight(r,g,b)
{
	hilitergb[0] = r;
	hilitergb[1] = g;
	hilitergb[2] = b;
	hilitergb[3] = 1.;
	draw();
	refresh();
}

//recall saved settings with unique functions--------------------------------------------------

function save()
{
    embedmessage("r_brgb", mybrgb[0], mybrgb[1], mybrgb[2], mybrgb[3]);
    embedmessage("r_frgb", myfrgb[0], myfrgb[1], myfrgb[2], myfrgb[3]);
    embedmessage("r_rgb2", myrgb2[0], myrgb2[1], myrgb2[2], myrgb2[3]);
    embedmessage("r_hlit", hilitergb[0], hilitergb[1], hilitergb[2], hilitergb[3]);
}

//getter functions for colors-----------------------------------------------------------------

function r_hlit(r, g, b)
{
    highlight(r, g, b);
}

function r_frgb(r, g, b)
{
    frgb(r, g, b);
}

function r_brgb(r, g, b)
{
    brgb(r, g, b);
}

function r_rgb2(r, g, b)
{
    rgb2(r, g, b);
}

//---------------------------------------------------------------------------------------------

function fsaa(v)
{
	sketch.fsaa = v;
	draw();
	refresh();
}

function clear(p,q)
{
	h = 0;
	full = 0;
	title = ["no file", 0];
	streams = new Array();
	draw();
	refresh();
}

// the astream class constructor
function astream(a, b, c, d, e) 
{
	this.number = a;
	this.name = b;
	this.starttime = 0.;
	this.endtime= d;
	this.numframes = e;
}

function howmany()
{
	r = title[1];
	outlet(1, r);
}

function font(v)
{
	myfont = v.toString();
	draw();
	refresh();
}

function fontsize(v)
{
	myfontsize = v;
	draw();
	refresh();
}

function draw()
{
	var str,width,height,aspect;

	height = box.rect[3] - box.rect[1]; 
	width = box.rect[2] - box.rect[0]; 
	aspect = width/height;

	with (sketch) {
		//erase background
		glclearcolor(mybrgb);
		glclear();
	
		glcolor(myfrgb);
		font(myfont);
		fontsize(myfontsize*1.2);
		offset = (myfontsize/12)*20;
		moveto(screentoworld (10, offset))
		joe = "name: " + title[0] +  "   streams: " + title[1];
		text(joe);
		moveto(screentoworld (0, offset+3))
		lineto(screentoworld (1000, offset+3, 0));
		fontsize(myfontsize);
	

		maxColWidths = [0, 0, 0, 0, 0];
		for (i=0;i<h;i++) {
			width_height = gettextinfo(streams[i].number+" ");
			if (width_height[0] > maxColWidths[0]) maxColWidths[0] = width_height[0];

			width_height = gettextinfo(streams[i].type+" ");
			if (width_height[0] > maxColWidths[1]) maxColWidths[1] = width_height[0];

			width_height = gettextinfo(streams[i].starttime.toFixed(3)+" ");
			if (width_height[0] > maxColWidths[2]) maxColWidths[2] = width_height[0];

			width_height = gettextinfo(streams[i].endtime.toFixed(3)+" ");
			if (width_height[0] > maxColWidths[3]) maxColWidths[3] = width_height[0];

			width_height = gettextinfo(streams[i].numframes+" ");
			if (width_height[0] > maxColWidths[4]) maxColWidths[4] = width_height[0];
		}
	

		for (i=0;i<h;i++)
		{
		jim = (offset * (i+1) + offset + 3);
		if (j == i) glcolor(hilitergb)
			else glcolor(myfrgb);
		moveto(screentoworld (10, jim))
		joe = streams[i].number + " ";
		text(joe);
		jill = maxColWidths[0] + 15;
		moveto(screentoworld (jill, jim))
		joe = streams[i].type + " ";
		//textalign("center", "bottom");
		text(joe);
		jill = maxColWidths[0] + maxColWidths[1] + 20;
		moveto(screentoworld (jill, jim))
		joe = streams[i].starttime.toFixed(3) + " ";
		text(joe);
		jill = maxColWidths[0] + maxColWidths[1] + maxColWidths[2] + 20;
		moveto(screentoworld (jill, jim))
		joe = streams[i].endtime.toFixed(3) + " ";
		text(joe);
		jill = maxColWidths[0] + maxColWidths[1] + maxColWidths[2] + maxColWidths[3] + 20;
		moveto(screentoworld (jill, jim))
		joe = streams[i].numframes + " ";
		text(joe);
		textalign("left", "bottom");
		}
	}
}

function bang()
{
	draw();
	refresh();
}

function onresize(w,h)
{
	draw();
	refresh();
}
onresize.local = 1; //private

function onclick(x,y)
{
	if (full == 0)
	{
	err = "¥ error: No SDIF streams available";
	post (err);
	post ();
	}
	else
	 {
	j = 999;
	for (i=0;i<h;i++)
	{
	k = (i * 20) + 20;
	if (k< y) j = i;
	}
	if (j > title[1]) j = title[1]-1;
	hilite = j;
	joe = title[0] + streams[0].number;
 	outlet(0, title[0], streams[j].number);
	bang();
	}
}
onclick.local = 1; //private
