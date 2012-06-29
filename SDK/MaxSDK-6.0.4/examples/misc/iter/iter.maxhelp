{
	"patcher" : 	{
		"rect" : [ 55.0, 72.0, 481.0, 297.0 ],
		"bglocked" : 0,
		"defrect" : [ 55.0, 72.0, 481.0, 297.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 10.0,
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
					"maxclass" : "newobj",
					"text" : "autohelp",
					"numinlets" : 1,
					"id" : "obj-6",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 332.0, 236.0, 55.0, 20.0 ],
					"hidden" : 1,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Can be used to play chords from lists",
					"linecount" : 2,
					"numinlets" : 1,
					"id" : "obj-11",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 231.0, 116.0, 112.0, 33.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "60 63 67",
					"numinlets" : 2,
					"id" : "obj-12",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 10.0,
					"patching_rect" : [ 232.0, 148.0, 56.0, 18.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Watch the Max window",
					"numinlets" : 1,
					"id" : "obj-14",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 85.0, 200.0, 128.0, 20.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "99 98 97 96 95",
					"numinlets" : 2,
					"id" : "obj-15",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 10.0,
					"patching_rect" : [ 48.0, 124.0, 88.0, 18.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"numinlets" : 1,
					"id" : "obj-16",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 48.0, 200.0, 33.0, 20.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "iter",
					"numinlets" : 1,
					"id" : "obj-17",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 10.0,
					"patching_rect" : [ 48.0, 157.0, 27.0, 20.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "iter",
					"numinlets" : 1,
					"id" : "obj-18",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 10.0,
					"patching_rect" : [ 232.0, 173.0, 27.0, 20.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "makenote 64 500",
					"numinlets" : 3,
					"id" : "obj-19",
					"numoutlets" : 2,
					"outlettype" : [ "int", "int" ],
					"fontsize" : 10.0,
					"patching_rect" : [ 232.0, 199.0, 100.0, 20.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "noteout 1",
					"numinlets" : 3,
					"id" : "obj-20",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 232.0, 228.0, 59.0, 20.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "iter",
					"numinlets" : 1,
					"fontface" : 3,
					"id" : "obj-1",
					"numoutlets" : 0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 18.0,
					"patching_rect" : [ 10.0, 8.0, 461.0, 30.0 ],
					"fontname" : "Arial",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "Break a list up into a series of numbers",
					"numinlets" : 1,
					"id" : "obj-2",
					"numoutlets" : 0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 11.0,
					"patching_rect" : [ 10.0, 36.0, 461.0, 21.0 ],
					"fontname" : "Arial",
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_description",
					"text" : "iter unpacks lists one element at a time through its outlet.",
					"numinlets" : 1,
					"id" : "obj-3",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 10.0, 57.0, 462.0, 20.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textbutton",
					"varname" : "autohelp_top_ref",
					"numinlets" : 1,
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"fontface" : 3,
					"id" : "obj-4",
					"numoutlets" : 3,
					"text" : "open iter reference",
					"fontlink" : 1,
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"outlettype" : [ "int", "", "int" ],
					"textovercolor" : [ 0.33, 0.45, 0.6, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 122.884438, 14.66663 ],
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 11.0,
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"spacing_x" : 0.0,
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"patching_rect" : [ 353.11557, 22.0, 122.884438, 14.66663 ],
					"underline" : 1,
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"spacing_y" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_see_title",
					"text" : "See Also:",
					"numinlets" : 1,
					"fontface" : 1,
					"id" : "obj-7",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 337.0, 240.0, 100.0, 20.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"varname" : "autohelp_see_menu",
					"numinlets" : 1,
					"id" : "obj-8",
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"fontsize" : 10.0,
					"types" : [  ],
					"items" : [ "(Objects:)", ",", "Cycle", ",", "Unpack", ",", "Thresh", ",", "Zl", ",", "<separator>", ",", "(Tutorials:)", ",", "Tutorial_30" ],
					"patching_rect" : [ 337.0, 260.0, 130.0, 20.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel",
					"mode" : 1,
					"numinlets" : 1,
					"grad1" : [ 0.27, 0.35, 0.47, 1.0 ],
					"id" : "obj-5",
					"numoutlets" : 0,
					"background" : 1,
					"patching_rect" : [ 5.0, 5.0, 471.0, 52.0 ],
					"grad2" : [ 0.85, 0.85, 0.85, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_see_panel",
					"numinlets" : 1,
					"border" : 2,
					"id" : "obj-9",
					"numoutlets" : 0,
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ],
					"background" : 1,
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ],
					"patching_rect" : [ 332.0, 236.0, 140.0, 50.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 1 ],
					"destination" : [ "obj-20", 1 ],
					"hidden" : 0,
					"midpoints" : [ 322.5, 224.0, 261.5, 224.0 ]
				}

			}
 ]
	}

}
