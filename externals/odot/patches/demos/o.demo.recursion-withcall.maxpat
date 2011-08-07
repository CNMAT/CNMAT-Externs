{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 711.0, 81.0, 739.0, 625.0 ],
		"bglocked" : 0,
		"defrect" : [ 711.0, 81.0, 739.0, 625.0 ],
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
					"maxclass" : "comment",
					"text" : "o.route can help you avoid triggers",
					"linecount" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 198.0, 398.0, 156.0, 34.0 ],
					"id" : "obj-36",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "GCD",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 27.0, 254.0, 150.0, 20.0 ],
					"id" : "obj-34",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Factorial",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 23.0, 6.0, 150.0, 20.0 ],
					"id" : "obj-31",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "240 54",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 206.0, 289.0, 47.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "12 9",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 123.0, 288.0, 34.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-28",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "% 0",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 37.0, 439.0, 55.5, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-26",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /b /a",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 37.0, 468.0, 93.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-21",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /a /b /b",
					"numinlets" : 1,
					"numoutlets" : 4,
					"patching_rect" : [ 36.0, 408.0, 132.0, 20.0 ],
					"outlettype" : [ "", "", "", "FullPacket" ],
					"id" : "obj-19",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /a",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 165.0, 472.0, 61.0, 20.0 ],
					"outlettype" : [ "", "FullPacket" ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 165.0, 498.0, 82.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-9",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "17 19",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 43.0, 288.0, 41.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/b $1 \n/a $2",
					"linecount" : 2,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 37.0, 318.0, 131.0, 34.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-12",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.if /b != 0",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 36.0, 374.0, 64.0, 20.0 ],
					"outlettype" : [ "FullPacket", "FullPacket" ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /result",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 195.0, 158.0, 84.0, 20.0 ],
					"outlettype" : [ "", "FullPacket" ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 195.0, 184.0, 82.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "4",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 36.0, 34.0, 32.5, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /result /factorial * 0",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 36.0, 128.0, 141.0, 20.0 ],
					"outlettype" : [ "FullPacket", "" ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /factorial 1 /result 1",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 36.0, 63.0, 151.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /factorial - 1",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 36.0, 155.0, 105.0, 20.0 ],
					"outlettype" : [ "FullPacket", "" ],
					"id" : "obj-7",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.if /factorial > 1",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 36.0, 97.0, 96.0, 20.0 ],
					"outlettype" : [ "FullPacket", "FullPacket" ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 45.5, 188.0, 21.0, 188.0, 21.0, 88.0, 45.5, 88.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 1 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 122.5, 122.0, 204.5, 122.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [ 46.5, 521.0, 13.0, 521.0, 13.0, 361.0, 45.5, 361.0 ]
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
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 1 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 90.5, 397.0, 174.5, 397.0 ]
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
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 1 ],
					"destination" : [ "obj-26", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 2 ],
					"destination" : [ "obj-21", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
