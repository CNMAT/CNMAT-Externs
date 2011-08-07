{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 751.0, 117.0, 1195.0, 535.0 ],
		"bglocked" : 0,
		"defrect" : [ 751.0, 117.0, 1195.0, 535.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 20.0,
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
					"maxclass" : "newobj",
					"text" : "o.print",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 34.0, 441.0, 66.0, 29.0 ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 13.0, 365.0, 32.5, 27.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gswitch",
					"int" : 1,
					"numinlets" : 3,
					"numoutlets" : 1,
					"patching_rect" : [ 8.0, 397.0, 41.0, 32.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-12"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /verbose",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 40.0, 118.0, 155.0, 29.0 ],
					"outlettype" : [ "", "FullPacket" ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "supports lists by descent to the bottom of the largest argument",
					"linecount" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 439.0, 297.0, 296.0, 52.0 ],
					"id" : "obj-9",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route #1/temp",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 184.0, 433.0, 151.0, 29.0 ],
					"outlettype" : [ "", "FullPacket" ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.if #1 < 0",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 71.973137, 181.0, 90.0, 27.0 ],
					"outlettype" : [ "FullPacket", "FullPacket" ],
					"id" : "obj-7",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Factorial\n\n\nAdrian Freed 2011",
					"linecount" : 4,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 413.0, 89.0, 241.0, 98.0 ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 101.0, 477.0, 25.0, 25.0 ],
					"id" : "obj-4",
					"comment" : ""
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "inlet",
					"numinlets" : 0,
					"numoutlets" : 1,
					"patching_rect" : [ 41.0, 76.0, 25.0, 25.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-1",
					"comment" : ""
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr #1/factorial = 1 #1/temp = #1",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 103.0, 265.0, 295.0, 27.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "OSC messages without /n or outside numerical bounds are passed through.\n\n",
					"linecount" : 3,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 151.0, 58.0, 331.0, 69.0 ],
					"id" : "obj-15",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr #1/factorial *= #1/temp-- ( #1/temp += ( #1/temp <= 1))",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 102.973145, 357.0, 499.0, 27.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.if product( #1/temp) > 1",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 101.973137, 314.0, 212.0, 27.0 ],
					"outlettype" : [ "FullPacket", "FullPacket" ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"fontsize" : 18.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 1 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-12", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 1 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 112.473145, 395.0, 393.986572, 395.0, 393.986572, 305.0, 111.473137, 305.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 1 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 1 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
