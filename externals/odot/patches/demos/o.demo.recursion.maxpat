{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 25.0, 69.0, 744.0, 643.0 ],
		"bglocked" : 0,
		"defrect" : [ 25.0, 69.0, 744.0, 643.0 ],
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
					"maxclass" : "comment",
					"text" : "GCD",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 38.0, 250.0, 150.0, 20.0 ],
					"id" : "obj-34",
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
					"patching_rect" : [ 237.0, 305.0, 47.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-23",
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
					"patching_rect" : [ 154.0, 304.0, 34.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /t = /b /b = /a % /b /a = /t",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 114.0, 431.0, 176.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-27",
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
					"patching_rect" : [ 69.0, 472.0, 61.0, 20.0 ],
					"outlettype" : [ "", "FullPacket" ],
					"id" : "obj-32",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 69.0, 501.0, 82.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-35",
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
					"patching_rect" : [ 74.0, 304.0, 41.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-37",
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
					"patching_rect" : [ 68.0, 334.0, 131.0, 34.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-38",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.if /b== 0",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 68.0, 403.0, 64.0, 20.0 ],
					"outlettype" : [ "FullPacket", "FullPacket" ],
					"id" : "obj-39",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Adrian Freed 2011",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 363.0, 68.0, 150.0, 20.0 ],
					"id" : "obj-33",
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
					"patching_rect" : [ 45.0, 42.0, 150.0, 20.0 ],
					"id" : "obj-11",
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
					"patching_rect" : [ 217.0, 194.0, 84.0, 20.0 ],
					"outlettype" : [ "", "FullPacket" ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 217.0, 220.0, 82.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-15",
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
					"patching_rect" : [ 58.0, 70.0, 32.5, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-16",
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
					"patching_rect" : [ 58.0, 99.0, 151.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-18",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /result *= /factorial--",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 58.0, 182.0, 152.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-20",
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
					"patching_rect" : [ 59.0, 140.0, 96.0, 20.0 ],
					"outlettype" : [ "FullPacket", "FullPacket" ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"fontsize" : 12.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-35", 0 ],
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
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"midpoints" : [ 123.5, 466.0, 291.0, 466.0, 291.0, 382.0, 77.5, 382.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 1 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 1 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 145.5, 169.0, 226.5, 169.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 67.5, 224.0, 43.0, 224.0, 43.0, 124.0, 68.5, 124.0 ]
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
 ]
	}

}
