{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 34.0, 96.0, 584.0, 663.0 ],
		"bglocked" : 0,
		"defrect" : [ 34.0, 96.0, 584.0, 663.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 11.595187,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"metadata" : [  ],
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "draw in the multislider",
					"patching_rect" : [ 29.0, 146.0, 135.0, 20.0 ],
					"id" : "obj-37",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontface" : 3,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "autohelp",
					"patching_rect" : [ 418.0, 572.0, 55.0, 20.0 ],
					"id" : "obj-4",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "supports: any type/planecount.",
					"patching_rect" : [ 274.0, 115.0, 166.0, 20.0 ],
					"id" : "obj-15",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"patching_rect" : [ 20.0, 486.0, 160.0, 160.0 ],
					"id" : "obj-16",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"onscreen" : 0,
					"depthbuffer" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"patching_rect" : [ 506.0, 422.0, 43.0, 20.0 ],
					"id" : "obj-17",
					"numinlets" : 1,
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 11.595187,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"patching_rect" : [ 462.0, 422.0, 43.0, 20.0 ],
					"id" : "obj-18",
					"numinlets" : 1,
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 11.595187,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak offset 0 0",
					"patching_rect" : [ 418.0, 446.0, 107.0, 20.0 ],
					"id" : "obj-19",
					"numinlets" : 3,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.fill bar 2",
					"patching_rect" : [ 177.0, 413.0, 62.0, 20.0 ],
					"id" : "obj-20",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"outlettype" : [ "", "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.fill bar 3",
					"patching_rect" : [ 334.0, 413.0, 62.0, 20.0 ],
					"id" : "obj-21",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"outlettype" : [ "", "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.fill bar 1",
					"patching_rect" : [ 20.0, 413.0, 62.0, 20.0 ],
					"id" : "obj-22",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"outlettype" : [ "", "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"bgcolor" : [ 0.588235, 0.588235, 0.980392, 1.0 ],
					"patching_rect" : [ 334.0, 298.0, 154.0, 103.0 ],
					"setstyle" : 1,
					"contdata" : 1,
					"setminmax" : [ 0.0, 255.0 ],
					"id" : "obj-23",
					"numinlets" : 1,
					"settype" : 0,
					"numoutlets" : 2,
					"size" : 256,
					"outlettype" : [ "", "" ],
					"slidercolor" : [ 0.196078, 0.196078, 0.588235, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"bgcolor" : [ 0.588235, 0.980392, 0.588235, 1.0 ],
					"patching_rect" : [ 177.0, 298.0, 154.0, 103.0 ],
					"setstyle" : 1,
					"contdata" : 1,
					"setminmax" : [ 0.0, 255.0 ],
					"id" : "obj-24",
					"numinlets" : 1,
					"settype" : 0,
					"numoutlets" : 2,
					"size" : 256,
					"outlettype" : [ "", "" ],
					"slidercolor" : [ 0.196078, 0.588235, 0.196078, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"bgcolor" : [ 0.980392, 0.588235, 0.588235, 1.0 ],
					"patching_rect" : [ 20.0, 298.0, 154.0, 103.0 ],
					"setstyle" : 1,
					"contdata" : 1,
					"setminmax" : [ 0.0, 255.0 ],
					"id" : "obj-25",
					"numinlets" : 1,
					"settype" : 0,
					"numoutlets" : 2,
					"size" : 256,
					"outlettype" : [ "", "" ],
					"slidercolor" : [ 0.588235, 0.196078, 0.196078, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.matrix bar 4 char 16 16",
					"patching_rect" : [ 20.0, 461.0, 139.0, 20.0 ],
					"id" : "obj-26",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"outlettype" : [ "jit_matrix", "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "can only reference one plane per list, and it will only fill upto 2 dimensions, bangs when done.",
					"linecount" : 2,
					"patching_rect" : [ 274.0, 176.0, 253.0, 33.0 ],
					"id" : "obj-27",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "offset message specifies where to start filling.",
					"linecount" : 2,
					"patching_rect" : [ 428.0, 467.0, 139.0, 33.0 ],
					"id" : "obj-28",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"patching_rect" : [ 202.0, 172.0, 50.0, 20.0 ],
					"id" : "obj-29",
					"numinlets" : 1,
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 11.595187,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "offset $1",
					"patching_rect" : [ 202.0, 194.0, 53.0, 18.0 ],
					"id" : "obj-30",
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "onscreen 0",
					"patching_rect" : [ 450.0, 232.0, 66.0, 18.0 ],
					"id" : "obj-31",
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 11.595187,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"patching_rect" : [ 450.0, 205.0, 56.0, 20.0 ],
					"id" : "obj-32",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"fontsize" : 11.595187,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.fill foo 0",
					"patching_rect" : [ 17.0, 220.0, 61.0, 20.0 ],
					"id" : "obj-33",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"outlettype" : [ "", "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.pwindow",
					"patching_rect" : [ 167.0, 249.0, 256.0, 32.0 ],
					"id" : "obj-34",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"onscreen" : 0,
					"depthbuffer" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.matrix foo 1 char 256",
					"patching_rect" : [ 17.0, 248.0, 129.0, 20.0 ],
					"id" : "obj-35",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"bgcolor" : [ 0.760784, 0.760784, 0.760784, 1.0 ],
					"patching_rect" : [ 17.0, 105.0, 154.0, 103.0 ],
					"setstyle" : 1,
					"contdata" : 1,
					"setminmax" : [ 0.0, 255.0 ],
					"id" : "obj-36",
					"numinlets" : 1,
					"settype" : 0,
					"numoutlets" : 2,
					"size" : 256,
					"outlettype" : [ "", "" ],
					"slidercolor" : [ 0.364706, 0.364706, 0.364706, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "fills a matrix with a list (only one plane per list)",
					"patching_rect" : [ 274.0, 138.0, 244.0, 20.0 ],
					"id" : "obj-38",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "arguments <matrix_name> <plane>",
					"patching_rect" : [ 274.0, 156.0, 192.0, 20.0 ],
					"id" : "obj-39",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "16x16 matrix, so we see the lists wrap across the second dimension",
					"linecount" : 4,
					"patching_rect" : [ 186.0, 487.0, 106.0, 60.0 ],
					"id" : "obj-40",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "jit.fill",
					"frgb" : [ 0.27, 0.35, 0.47, 1.0 ],
					"patching_rect" : [ 10.0, 8.0, 485.0, 30.0 ],
					"id" : "obj-5",
					"numinlets" : 1,
					"fontname" : "Arial",
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"numoutlets" : 0,
					"fontface" : 3,
					"fontsize" : 20.871338
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "Fill a matrix with a list",
					"frgb" : [ 0.27, 0.35, 0.47, 1.0 ],
					"patching_rect" : [ 10.0, 36.0, 485.0, 21.0 ],
					"id" : "obj-7",
					"numinlets" : 1,
					"fontname" : "Arial",
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 12.754705
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_description",
					"text" : "The jit.fill object fills one plane of a named matrix with the values of a list received in its left inlet. The offset and plane attributes determine the cells to be filled. Upon completion, the jit.fill object sends a bang message out its left outlet.",
					"linecount" : 3,
					"patching_rect" : [ 10.0, 57.0, 502.0, 46.0 ],
					"id" : "obj-8",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textbutton",
					"varname" : "autohelp_top_ref",
					"spacing_y" : 0.0,
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"patching_rect" : [ 367.255493, 22.0, 132.744507, 14.666666 ],
					"id" : "obj-9",
					"numinlets" : 1,
					"fontname" : "Arial",
					"fontlink" : 1,
					"textovercolor" : [ 0.4, 0.5, 0.65, 1.0 ],
					"underline" : 1,
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"text" : "open jit.fill reference",
					"numoutlets" : 3,
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontface" : 3,
					"outlettype" : [ "int", "", "int" ],
					"fontsize" : 12.754705,
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 132.744507, 14.666666 ],
					"spacing_x" : 0.0,
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_see_title",
					"text" : "See Also:",
					"patching_rect" : [ 423.0, 576.0, 100.0, 20.0 ],
					"id" : "obj-11",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontface" : 1,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"varname" : "autohelp_see_menu",
					"types" : [  ],
					"patching_rect" : [ 423.0, 596.0, 130.0, 20.0 ],
					"id" : "obj-12",
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"fontsize" : 11.595187,
					"items" : [ "(Objects:)", ",", "jit.iter", ",", "jit.matrix", ",", "jit.spill", ",", "zl", ",", "<separator>", ",", "(Tutorials:)", ",", "jitterchapter13", ",", "jitterchapter14" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel",
					"patching_rect" : [ 5.0, 5.0, 495.0, 52.0 ],
					"mode" : 1,
					"id" : "obj-10",
					"numinlets" : 1,
					"numoutlets" : 0,
					"grad1" : [ 0.86, 0.86, 0.75, 1.0 ],
					"background" : 1,
					"grad2" : [ 0.78, 0.84, 0.86, 0.7 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_see_panel",
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ],
					"patching_rect" : [ 418.0, 572.0, 140.0, 50.0 ],
					"id" : "obj-13",
					"numinlets" : 1,
					"border" : 2,
					"numoutlets" : 0,
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ],
					"background" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [ 26.5, 214.0, 26.5, 214.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [ 211.5, 214.0, 26.5, 214.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 427.5, 471.0, 413.0, 471.0, 413.0, 408.0, 29.5, 408.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 29.5, 407.0, 29.5, 407.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [ 343.5, 439.0, 29.5, 439.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [ 186.5, 439.0, 29.5, 439.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 1
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 1
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [ 26.5, 285.0, 155.0, 285.0, 155.0, 244.0, 176.5, 244.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 1
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [ 427.5, 471.0, 413.0, 471.0, 413.0, 408.0, 186.5, 408.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [ 186.5, 407.0, 186.5, 407.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [ 343.5, 407.0, 343.5, 407.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [ 427.5, 471.0, 413.0, 471.0, 413.0, 408.0, 343.5, 408.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-19", 1 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-19", 2 ],
					"hidden" : 0,
					"midpoints" : [ 515.5, 441.0, 515.5, 441.0 ]
				}

			}
 ]
	}

}
