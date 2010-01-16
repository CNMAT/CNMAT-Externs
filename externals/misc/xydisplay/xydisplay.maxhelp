{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 296.0, 44.0, 968.0, 750.0 ],
		"bglocked" : 0,
		"defrect" : [ 296.0, 44.0, 968.0, 750.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 4.0, 4.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"id" : "obj-19",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 684.0, 332.0, 50.0, 20.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "line_width $1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-16",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 684.0, 360.0, 81.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"id" : "obj-11",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 704.0, 252.0, 50.0, 20.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "weight $1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-15",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 704.0, 284.0, 62.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"id" : "obj-14",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 4.0, 28.0, 50.0, 20.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "exponent $1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-10",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 4.0, 60.0, 77.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "\"0\" 0.583997 \"1\" 0.427382 \"2\" 0.514844 \"3\" 0.884308 \"4\" 0.",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-8",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 188.0, 648.0, 331.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-13",
					"outlettype" : [ "int" ],
					"patching_rect" : [ 664.0, 56.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.304478 0.311377 0.050746 0.778443",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-7",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 148.0, 8.0, 221.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-9",
					"outlettype" : [ "int" ],
					"patching_rect" : [ 664.0, 100.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "locked $1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-2",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 664.0, 124.0, 62.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<--rename a point by name",
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-21",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 752.0, 168.0, 164.0, 20.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<--rename the selected point",
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-20",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 752.0, 192.0, 164.0, 20.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "rename foo",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-17",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 660.0, 192.0, 71.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "rename foo bar",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-12",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 660.0, 168.0, 92.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "outputmode $1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-6",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 664.0, 80.0, 91.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-18",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 436.0, 40.0, 37.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.662687 0.203593 0.835821 0.652695 0.474627 0.568862",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-5",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 76.0, 32.0, 327.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.753906 0.830078",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-4",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 388.0, 600.0, 114.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.169922 0.833984 0.697266 0.318359 0.195312 0.144531",
					"numinlets" : 2,
					"numoutlets" : 1,
					"id" : "obj-3",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 104.0, 708.0, 331.0, 18.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "xydisplay",
					"varname" : "xydisplay",
					"numinlets" : 1,
					"textcolor" : [  ],
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numoutlets" : 3,
					"id" : "obj-1",
					"outlettype" : [ "", "", "" ],
					"fontname" : "Arial",
					"rbf" : 0,
					"patching_rect" : [ 76.0, 84.0, 512.0, 512.0 ],
					"fontsize" : 12.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-8", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 2 ],
					"destination" : [ "obj-4", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-3", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
