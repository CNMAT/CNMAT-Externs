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
					"maxclass" : "message",
					"text" : "\"preset 0\" inner_radius 200",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-36",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 628.0, 204.0, 155.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-39",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 672.0, 128.0, 50.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-38",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 616.0, 128.0, 50.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak 0. 0.",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-20",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 616.0, 152.0, 57.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "\"preset 0\" coords $1 $2",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-15",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 616.0, 180.0, 135.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontname" : "Arial",
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-19",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 636.0, 620.0, 50.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "zl 1024 len",
					"fontname" : "Arial",
					"outlettype" : [ "", "" ],
					"id" : "obj-14",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 628.0, 584.0, 69.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"fontname" : "Arial",
					"id" : "obj-11",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 580.0, 600.0, 34.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"outlettype" : [ "int" ],
					"id" : "obj-13",
					"numinlets" : 1,
					"patching_rect" : [ 652.0, 12.0, 20.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"outlettype" : [ "int" ],
					"id" : "obj-12",
					"numinlets" : 1,
					"patching_rect" : [ 496.0, 12.0, 20.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "always_draw_labels $1",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-9",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 652.0, 40.0, 135.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "always_draw_circles $1",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-3",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 496.0, 40.0, 137.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"outlettype" : [ "bang" ],
					"id" : "obj-10",
					"numinlets" : 1,
					"patching_rect" : [ 740.0, 368.0, 20.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "uzi 50",
					"fontname" : "Arial",
					"outlettype" : [ "bang", "bang", "int" ],
					"id" : "obj-6",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 760.0, 380.0, 46.0, 20.0 ],
					"numoutlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "add_point",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-2",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 760.0, 408.0, 63.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.791016 0.833984",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-37",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 464.0, 612.0, 114.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "add_point name bar coords 0.5 0.5 rgb 0X0000ff",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-35",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 680.0, 336.0, 268.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "add_point name \"preset 0\" coords 0.3 0.3 rgb 0. 1. 0.",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-34",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 668.0, 308.0, 292.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "add_point name foo coords 0.1 0.2 rgb 1. 0. 0. inner_radius 50 outer_radius 100 locked 1",
					"linecount" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-33",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 640.0, 260.0, 260.0, 32.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend add_point",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-32",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 616.0, 400.0, 111.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-31",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 616.0, 308.0, 41.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-27",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 428.0, 12.0, 41.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll",
					"fontname" : "Arial",
					"outlettype" : [ "", "", "", "" ],
					"id" : "obj-21",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 616.0, 344.0, 59.5, 20.0 ],
					"numoutlets" : 4,
					"saved_object_attributes" : 					{
						"embed" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend move",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-30",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 828.0, 84.0, 87.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-29",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 884.0, 24.0, 50.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-28",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 828.0, 24.0, 50.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak 0. 0.",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-26",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 828.0, 52.0, 57.0, 20.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "command-shift click to delete a point",
					"fontname" : "Arial",
					"id" : "obj-17",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 600.0, 104.0, 207.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "shift click to edit or create new points",
					"fontname" : "Arial",
					"id" : "obj-16",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 600.0, 84.0, 207.0, 20.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"outlettype" : [ "", "" ],
					"id" : "obj-25",
					"setminmax" : [ 0.0, 1.0 ],
					"size" : 275,
					"numinlets" : 1,
					"patching_rect" : [ 696.0, 480.0, 232.0, 136.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route done",
					"fontname" : "Arial",
					"outlettype" : [ "", "" ],
					"id" : "obj-24",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 604.0, 496.0, 68.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "zl 1024 group",
					"fontname" : "Arial",
					"outlettype" : [ "", "" ],
					"id" : "obj-23",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 628.0, 552.0, 83.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "zl slice 1",
					"fontname" : "Arial",
					"outlettype" : [ "", "" ],
					"id" : "obj-22",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 628.0, 524.0, 57.0, 20.0 ],
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "\"preset 0\" 0.67333 \"preset 1\" 0.545817 \"preset 2\" 0.",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-8",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 188.0, 648.0, 338.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "foo 0.304478 0.311377",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-7",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 148.0, 8.0, 133.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-18",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 436.0, 40.0, 37.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.662687 0.203593, 0.835821 0.652695, 0.474627 0.568862",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-5",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 76.0, 32.0, 334.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.566406 0.603516",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-4",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 344.0, 612.0, 114.0, 18.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "rbfi",
					"textcolor" : [  ],
					"fontname" : "Arial",
					"outlettype" : [ "", "", "", "", "" ],
					"id" : "obj-1",
					"rbf" : 0,
					"fontsize" : 12.0,
					"line_width" : 0.0,
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 76.0, 84.0, 512.0, 512.0 ],
					"numoutlets" : 5
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-1", 0 ],
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
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
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
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 4 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 3 ],
					"destination" : [ "obj-37", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 4 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 1 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-26", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 1 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-20", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
