{
	"patcher" : 	{
		"rect" : [ 61.0, 78.0, 521.0, 428.0 ],
		"bglocked" : 0,
		"defrect" : [ 61.0, 78.0, 521.0, 428.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 10.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxfadetime" : 200,
		"boxanimatetime" : 200,
		"scrollanimatetime" : 200,
		"imprint" : 0,
		"metadata" : [  ],
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "autohelp",
					"id" : "obj-1",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"hidden" : 1,
					"patching_rect" : [ 372.0, 360.0, 55.0, 20.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "filein allows access to any file as binary data. When a number arrives at one of its inlets, the data at that byte index is sent out filein's outlet. The particular inlet determines whether 8, 16, or 32 bits of data are output.",
					"linecount" : 3,
					"id" : "obj-8",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 10.0, 93.0, 422.0, 46.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "access data from file on disk",
					"id" : "obj-9",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 121.0, 178.0, 155.0, 20.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "read entire file into object",
					"id" : "obj-10",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 121.0, 154.0, 138.0, 20.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read",
					"id" : "obj-11",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"patching_rect" : [ 84.0, 153.0, 34.0, 18.0 ],
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "spool",
					"id" : "obj-12",
					"fontsize" : 10.0,
					"numinlets" : 2,
					"patching_rect" : [ 84.0, 178.0, 36.0, 18.0 ],
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "long at byte offset",
					"id" : "obj-13",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 222.0, 206.0, 100.0, 20.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "read or spool with argument specifies a file in Max's search path",
					"linecount" : 4,
					"id" : "obj-14",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 327.0, 154.0, 103.0, 60.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "word at byte offset",
					"id" : "obj-15",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 113.0, 206.0, 103.0, 20.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"id" : "obj-16",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 138.0, 224.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"id" : "obj-17",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 239.0, 225.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "by bytes",
					"id" : "obj-18",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 38.0, 206.0, 52.0, 20.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "filein filein_data.bin",
					"id" : "obj-19",
					"fontsize" : 10.0,
					"numinlets" : 3,
					"patching_rect" : [ 37.0, 254.0, 221.0, 20.0 ],
					"numoutlets" : 3,
					"fontname" : "Arial",
					"outlettype" : [ "int", "bang", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"id" : "obj-20",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 37.0, 225.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Data output",
					"id" : "obj-21",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 47.0, 281.0, 68.0, 20.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"id" : "obj-22",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 37.0, 300.0, 95.0, 20.0 ],
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Note: if the file is spooled, access requests at interrupt level are deferred, and cannot be counted on to occur at any particular time.",
					"linecount" : 2,
					"id" : "obj-23",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 22.0, 324.0, 387.0, 33.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "bang on EOF",
					"id" : "obj-24",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 155.0, 281.0, 78.0, 20.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "optional filename as argument (\"spool\" after name spools the file, otherwise it is read in)",
					"linecount" : 2,
					"id" : "obj-25",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 260.0, 246.0, 247.0, 33.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "bang when \"read\" operation finished",
					"linecount" : 2,
					"id" : "obj-26",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 256.0, 282.0, 118.0, 33.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"id" : "obj-27",
					"numinlets" : 1,
					"patching_rect" : [ 138.0, 280.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"id" : "obj-28",
					"numinlets" : 1,
					"patching_rect" : [ 239.0, 280.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "filein",
					"fontface" : 3,
					"id" : "obj-2",
					"fontsize" : 18.0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"numinlets" : 1,
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 10.0, 8.0, 485.0, 30.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "Read in a file of binary data",
					"id" : "obj-3",
					"fontsize" : 11.0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"numinlets" : 1,
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"patching_rect" : [ 10.0, 36.0, 485.0, 21.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_description",
					"text" : "filein reads a file of binary data and outputs the data at various points in the file given the appropriate input.",
					"linecount" : 2,
					"id" : "obj-4",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 10.0, 57.0, 422.0, 33.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textbutton",
					"varname" : "autohelp_top_ref",
					"fontface" : 3,
					"id" : "obj-5",
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 11.0,
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"spacing_y" : 0.0,
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 132.803406, 14.66663 ],
					"numinlets" : 1,
					"fontlink" : 1,
					"patching_rect" : [ 367.196594, 22.0, 132.803406, 14.66663 ],
					"numoutlets" : 3,
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"textovercolor" : [ 0.33, 0.45, 0.6, 1.0 ],
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"text" : "open filein reference",
					"fontname" : "Arial",
					"outlettype" : [ "int", "", "int" ],
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"spacing_x" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_see_title",
					"text" : "See Also:",
					"fontface" : 1,
					"id" : "obj-7",
					"fontsize" : 10.0,
					"numinlets" : 1,
					"patching_rect" : [ 377.0, 364.0, 100.0, 20.0 ],
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"varname" : "autohelp_see_menu",
					"id" : "obj-29",
					"fontsize" : 10.0,
					"types" : [  ],
					"numinlets" : 1,
					"items" : [ "(Objects:)", ",", "Text" ],
					"patching_rect" : [ 377.0, 384.0, 130.0, 20.0 ],
					"numoutlets" : 3,
					"fontname" : "Arial",
					"outlettype" : [ "int", "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel",
					"background" : 1,
					"id" : "obj-6",
					"grad2" : [ 0.85, 0.85, 0.85, 1.0 ],
					"numinlets" : 1,
					"mode" : 1,
					"patching_rect" : [ 5.0, 5.0, 495.0, 52.0 ],
					"numoutlets" : 0,
					"grad1" : [ 0.27, 0.35, 0.47, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_see_panel",
					"background" : 1,
					"id" : "obj-30",
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ],
					"numinlets" : 1,
					"patching_rect" : [ 372.0, 360.0, 140.0, 50.0 ],
					"numoutlets" : 0,
					"border" : 2,
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-19", 2 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-19", 1 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [ 93.5, 174.0, 18.0, 174.0, 18.0, 249.0, 46.5, 249.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [ 93.5, 250.0, 46.5, 250.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 1 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 2 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0
				}

			}
 ]
	}

}
