{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 175.0, 110.0, 1265.0, 730.0 ],
		"bglocked" : 0,
		"defrect" : [ 175.0, 110.0, 1265.0, 730.0 ],
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
					"maxclass" : "message",
					"text" : "/sine rgb 0.9 0.9 0.9",
					"numinlets" : 2,
					"patching_rect" : [ 554.0, 116.0, 117.0, 18.0 ],
					"id" : "obj-42",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/sawtooth rgb 0.1 0.7. 0.",
					"numinlets" : 2,
					"patching_rect" : [ 1094.0, 305.0, 139.0, 18.0 ],
					"id" : "obj-41",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/triangle rgb 0.7 . 0. 0.",
					"numinlets" : 2,
					"patching_rect" : [ 970.0, 268.0, 127.0, 18.0 ],
					"id" : "obj-40",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"patching_rect" : [ 390.0, 3.0, 20.0, 20.0 ],
					"id" : "obj-34",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 55",
					"numinlets" : 1,
					"patching_rect" : [ 389.0, 28.0, 79.0, 20.0 ],
					"id" : "obj-32",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess poll",
					"numinlets" : 1,
					"patching_rect" : [ 696.0, 593.0, 84.0, 20.0 ],
					"id" : "obj-7",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "* 0.4",
					"numinlets" : 2,
					"patching_rect" : [ 695.0, 646.0, 35.0, 20.0 ],
					"id" : "obj-4",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "float" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mousestate",
					"numinlets" : 1,
					"patching_rect" : [ 695.0, 621.0, 73.0, 20.0 ],
					"id" : "obj-2",
					"numoutlets" : 5,
					"fontname" : "Arial",
					"outlettype" : [ "int", "int", "int", "int", "int" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"patching_rect" : [ 554.0, 711.0, 45.0, 45.0 ],
					"id" : "obj-25",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.5",
					"numinlets" : 2,
					"patching_rect" : [ 552.0, 684.0, 42.0, 20.0 ],
					"id" : "obj-24",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "kslider",
					"numinlets" : 2,
					"patching_rect" : [ 388.0, 52.0, 336.0, 53.0 ],
					"id" : "obj-23",
					"numoutlets" : 2,
					"outlettype" : [ "int", "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mtof",
					"numinlets" : 1,
					"patching_rect" : [ 347.0, 123.0, 34.0, 20.0 ],
					"id" : "obj-22",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"patching_rect" : [ 339.0, 543.0, 50.0, 20.0 ],
					"id" : "obj-20",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.",
					"numinlets" : 2,
					"patching_rect" : [ 375.0, 647.0, 36.0, 20.0 ],
					"id" : "obj-15",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cycle~",
					"numinlets" : 2,
					"patching_rect" : [ 348.0, 613.0, 45.0, 20.0 ],
					"id" : "obj-16",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.",
					"numinlets" : 2,
					"patching_rect" : [ 644.0, 644.0, 36.0, 20.0 ],
					"id" : "obj-17",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "saw~",
					"numinlets" : 2,
					"patching_rect" : [ 623.0, 615.0, 39.0, 20.0 ],
					"id" : "obj-18",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.",
					"numinlets" : 2,
					"patching_rect" : [ 467.0, 645.0, 36.0, 20.0 ],
					"id" : "obj-13",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "rect~",
					"numinlets" : 3,
					"patching_rect" : [ 441.0, 614.0, 46.0, 20.0 ],
					"id" : "obj-14",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.",
					"numinlets" : 2,
					"patching_rect" : [ 555.0, 645.0, 36.0, 20.0 ],
					"id" : "obj-10",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tri~",
					"numinlets" : 3,
					"patching_rect" : [ 530.0, 612.0, 46.0, 20.0 ],
					"id" : "obj-9",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /sine/weight /square/weight /triangle/weight /sawtooth/weight",
					"numinlets" : 1,
					"patching_rect" : [ 394.0, 543.0, 373.0, 20.0 ],
					"id" : "obj-8",
					"numoutlets" : 5,
					"fontname" : "Arial",
					"outlettype" : [ "", "", "", "", "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "rbfi",
					"pointdiameter" : 10.01,
					"numinlets" : 1,
					"patching_rect" : [ 400.0, 154.0, 372.0, 372.0 ],
					"id" : "obj-1",
					"textcolor" : [  ],
					"numoutlets" : 5,
					"fontname" : "Arial",
					"always_draw_labels" : 1,
					"outlettype" : [ "", "", "", "", "" ],
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"rbf" : 4,
					"line_width" : 0.0,
					"fontsize" : 16.0,
					"spaces" : [ "space", 4, 0.432796, 0.612903, "/sine", 0.9, 0.9, 0.9, 1.0, -6.942374, 0.0, 0.595568, 0.306793, 0, 0.322581, 0.766129, "/square", 0.1875, 0.1875, 0.75, 0.0, 2.73178, 5172.633789, 0.026475, 0.142878, 0, 0.350333, 0.268293, "/triangle", 0.7, 0.0, 0.0, 1.0, 4.089089, 2958900.25, 0.058538, 0.180529, 0, 0.733925, 0.427938, "/sawtooth", 0.1, 0.7, 0.0, 1.0, 3.590613, 357227.4375, 0.049828, 0.179671, 0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-24", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-15", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 1 ],
					"destination" : [ "obj-13", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 2 ],
					"destination" : [ "obj-10", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 3 ],
					"destination" : [ "obj-17", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
