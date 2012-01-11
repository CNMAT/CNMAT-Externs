{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 0.0, 44.0, 1280.0, 667.0 ],
		"bglocked" : 0,
		"defrect" : [ 0.0, 44.0, 1280.0, 667.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/fader/7 0.740157 \n",
					"numoutlets" : 1,
					"id" : "obj-10",
					"fontname" : "Arial",
					"patching_rect" : [ 361.0, 280.0, 165.0, 20.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"id" : "obj-4",
					"patching_rect" : [ 532.0, 211.0, 25.0, 25.0 ],
					"outlettype" : [ "int" ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/button/bottom/? $1",
					"numoutlets" : 1,
					"id" : "obj-5",
					"fontname" : "Arial",
					"patching_rect" : [ 529.0, 236.0, 176.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"id" : "obj-6",
					"patching_rect" : [ 539.0, 153.0, 26.0, 26.0 ],
					"outlettype" : [ "int" ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/button/top/? $1",
					"numoutlets" : 1,
					"id" : "obj-7",
					"fontname" : "Arial",
					"patching_rect" : [ 534.0, 184.0, 145.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"id" : "obj-9",
					"patching_rect" : [ 500.0, 88.0, 22.0, 22.0 ],
					"outlettype" : [ "int" ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"numoutlets" : 1,
					"id" : "obj-11",
					"patching_rect" : [ 22.0, 306.0, 673.0, 620.0 ],
					"border" : 1,
					"outlettype" : [ "" ],
					"args" : [  ],
					"name" : "o.io.bcf2000panel.maxpat",
					"offset" : [ 4.0, -8.0 ],
					"numinlets" : 1,
					"lockeddragscroll" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 127.",
					"numoutlets" : 1,
					"id" : "obj-12",
					"fontname" : "Arial",
					"patching_rect" : [ 126.0, 239.0, 46.0, 17.0 ],
					"outlettype" : [ "float" ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "dial",
					"numoutlets" : 1,
					"id" : "obj-13",
					"fgcolor" : [ 0.666667, 0.666667, 0.666667, 1.0 ],
					"patching_rect" : [ 127.0, 190.0, 40.0, 40.0 ],
					"outlettype" : [ "float" ],
					"needlecolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlinecolor" : [ 0.882353, 0.882353, 0.882353, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1,
					"vtracking" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 127.",
					"numoutlets" : 1,
					"id" : "obj-14",
					"fontname" : "Arial",
					"patching_rect" : [ 8.0, 221.0, 46.0, 17.0 ],
					"outlettype" : [ "float" ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "dial",
					"numoutlets" : 1,
					"id" : "obj-15",
					"fgcolor" : [ 0.666667, 0.666667, 0.666667, 1.0 ],
					"patching_rect" : [ 8.0, 177.0, 40.0, 40.0 ],
					"outlettype" : [ "float" ],
					"needlecolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlinecolor" : [ 0.882353, 0.882353, 0.882353, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1,
					"vtracking" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 127.",
					"numoutlets" : 1,
					"id" : "obj-16",
					"fontname" : "Arial",
					"patching_rect" : [ 10.0, 119.0, 46.0, 17.0 ],
					"outlettype" : [ "float" ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "dial",
					"numoutlets" : 1,
					"id" : "obj-17",
					"fgcolor" : [ 0.666667, 0.666667, 0.666667, 1.0 ],
					"patching_rect" : [ 11.0, 70.0, 40.0, 40.0 ],
					"outlettype" : [ "float" ],
					"needlecolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlinecolor" : [ 0.882353, 0.882353, 0.882353, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1,
					"vtracking" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/encoder_button/?/? $1",
					"numoutlets" : 1,
					"id" : "obj-19",
					"fontname" : "Arial",
					"patching_rect" : [ 499.0, 116.0, 202.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 127.",
					"numoutlets" : 1,
					"id" : "obj-20",
					"fontname" : "Arial",
					"patching_rect" : [ 85.0, 70.0, 46.0, 17.0 ],
					"outlettype" : [ "float" ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/encoder/d/? $1",
					"numoutlets" : 1,
					"id" : "obj-21",
					"fontname" : "Arial",
					"patching_rect" : [ 128.0, 264.0, 141.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/encoder/c/? $1",
					"numoutlets" : 1,
					"id" : "obj-22",
					"fontname" : "Arial",
					"patching_rect" : [ 8.0, 243.0, 140.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/encoder/b/? $1",
					"numoutlets" : 1,
					"id" : "obj-23",
					"fontname" : "Arial",
					"patching_rect" : [ 10.0, 138.0, 141.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/encoder/a/? $1",
					"numoutlets" : 1,
					"id" : "obj-24",
					"fontname" : "Arial",
					"patching_rect" : [ 86.0, 99.0, 140.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"numoutlets" : 2,
					"id" : "obj-25",
					"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
					"patching_rect" : [ 273.0, 61.0, 12.0, 82.0 ],
					"outlettype" : [ "", "" ],
					"setminmax" : [ 0.0, 1.0 ],
					"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
					"contdata" : 1,
					"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
					"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
					"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
					"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
					"numinlets" : 1,
					"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"numoutlets" : 2,
					"id" : "obj-26",
					"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
					"patching_rect" : [ 252.0, 82.0, 12.0, 82.0 ],
					"outlettype" : [ "", "" ],
					"setminmax" : [ 0.0, 1.0 ],
					"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
					"contdata" : 1,
					"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
					"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
					"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
					"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
					"numinlets" : 1,
					"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"numoutlets" : 2,
					"id" : "obj-27",
					"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
					"patching_rect" : [ 228.0, 106.0, 12.0, 82.0 ],
					"outlettype" : [ "", "" ],
					"setminmax" : [ 0.0, 1.0 ],
					"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
					"contdata" : 1,
					"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
					"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
					"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
					"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
					"numinlets" : 1,
					"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"numoutlets" : 2,
					"id" : "obj-28",
					"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
					"patching_rect" : [ 330.0, 23.0, 12.0, 82.0 ],
					"outlettype" : [ "", "" ],
					"setminmax" : [ 0.0, 1.0 ],
					"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
					"contdata" : 1,
					"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
					"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
					"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
					"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
					"numinlets" : 1,
					"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/fader/{1\\,3\\,5\\,7} $1",
					"numoutlets" : 1,
					"id" : "obj-29",
					"fontname" : "Arial",
					"patching_rect" : [ 249.0, 170.0, 190.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/fader/? $1",
					"numoutlets" : 1,
					"id" : "obj-30",
					"fontname" : "Arial",
					"patching_rect" : [ 227.0, 196.0, 104.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/fader/[5-8] $1",
					"numoutlets" : 1,
					"id" : "obj-31",
					"fontname" : "Arial",
					"patching_rect" : [ 329.0, 109.0, 134.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/fader/[1-4] $1",
					"numoutlets" : 1,
					"id" : "obj-32",
					"fontname" : "Arial",
					"patching_rect" : [ 274.0, 147.0, 134.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.io.bcf2000 BCF2000",
					"numoutlets" : 1,
					"id" : "obj-33",
					"fontname" : "Arial",
					"patching_rect" : [ 373.0, 254.0, 100.0, 17.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/general/? $1",
					"numoutlets" : 1,
					"id" : "obj-34",
					"fontname" : "Arial",
					"patching_rect" : [ 540.0, 87.0, 119.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"id" : "obj-35",
					"patching_rect" : [ 541.0, 53.0, 25.0, 25.0 ],
					"outlettype" : [ "int" ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "dial",
					"numoutlets" : 1,
					"id" : "obj-36",
					"fgcolor" : [ 0.666667, 0.666667, 0.666667, 1.0 ],
					"patching_rect" : [ 86.0, 21.0, 40.0, 40.0 ],
					"outlettype" : [ "float" ],
					"needlecolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlinecolor" : [ 0.882353, 0.882353, 0.882353, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1,
					"vtracking" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"numoutlets" : 2,
					"id" : "obj-37",
					"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
					"patching_rect" : [ 351.0, 3.0, 12.0, 82.0 ],
					"outlettype" : [ "", "" ],
					"setminmax" : [ 0.0, 1.0 ],
					"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
					"contdata" : 1,
					"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
					"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
					"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
					"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
					"numinlets" : 1,
					"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/fader/1 $1",
					"numoutlets" : 1,
					"id" : "obj-38",
					"fontname" : "Arial",
					"patching_rect" : [ 352.0, 87.0, 102.0, 21.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.007843, 0.956863, 0.909804, 1.0 ],
					"fontface" : 1,
					"fontsize" : 14.0,
					"bgcolor" : [ 0.6, 0.6, 1.0, 0.027451 ],
					"numinlets" : 2,
					"bgcolor2" : [ 0.866667, 0.866667, 0.866667, 0.145098 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Behringer BCF2000\n\nAdrian Freed 2011,2007\nJohn MacCallum 2007",
					"linecount" : 4,
					"numoutlets" : 0,
					"id" : "obj-40",
					"fontname" : "Arial",
					"patching_rect" : [ 585.0, 6.0, 195.0, 71.0 ],
					"fontsize" : 14.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-10", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
