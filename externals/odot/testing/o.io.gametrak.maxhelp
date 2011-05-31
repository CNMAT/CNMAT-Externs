{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 24.0, 44.0, 1024.0, 657.0 ],
		"bglocked" : 0,
		"defrect" : [ 24.0, 44.0, 1024.0, 657.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 8.0, 8.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "multislider",
					"setstyle" : 2,
					"numinlets" : 1,
					"size" : 3,
					"numoutlets" : 2,
					"patching_rect" : [ 616.0, 472.0, 160.0, 138.0 ],
					"outlettype" : [ "", "" ],
					"id" : "obj-41"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /gathered",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 616.0, 416.0, 102.0, 20.0 ],
					"outlettype" : [ "", "FullPacket" ],
					"id" : "obj-40",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/scaled/right/z 0.000977 \n/right/y 35 \n/scaled/left/x 0.980464 \n/scaled/left/z 0.002686 \n/scaled/right/y -0.982906 \n/scaled/right/x -0.056410 \n/right/z 4091 \n/gathered 0.980464 0.540415 0.002686 \n/left/y 3154 \n/right/x 1932 \n/left/z 4084 \n/left/x 4055 \n/scaled/left/y 0.540415 ",
					"linecount" : 14,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 272.0, 400.0, 202.0, 200.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-39",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /scaled/left/x /scaled/left/y /scaled/left/z pack 0. 0. 0. @as /gathered",
					"linecount" : 2,
					"fontsize" : 24.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 272.0, 320.0, 604.0, 62.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-38",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/right/y 35 \n/right/z 4091 \n/left/y 3154 \n/right/x 1932 \n/left/z 4084 \n/left/x 4055 ",
					"linecount" : 6,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 272.0, 64.0, 147.0, 89.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-37",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/scaled/right/z 0.000977 \n/right/y 35 \n/scaled/left/x 0.980464 \n/scaled/left/z 0.002686 \n/scaled/right/y -0.982906 \n/scaled/right/x -0.056410 \n/right/z 4091 \n/left/y 3154 \n/right/x 1932 \n/left/z 4084 \n/left/x 4055 \n/scaled/left/y 0.540415 ",
					"linecount" : 12,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 16.0, 416.0, 202.0, 172.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-36",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /*/z scale 0. 4095. 1. 0. @prepending /scaled",
					"fontsize" : 24.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 272.0, 256.0, 551.0, 34.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-35",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/right/y 35 \n/scaled/left/x 0.980464 \n/scaled/right/y -0.982906 \n/scaled/right/x -0.056410 \n/right/z 4091 \n/left/y 3154 \n/right/x 1932 \n/left/z 4084 \n/left/x 4055 \n/scaled/left/y 0.540415 ",
					"linecount" : 10,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 16.0, 240.0, 147.0, 144.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-34",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /*/[xy] scale 0. 4095. -1. 1. @prepending /scaled",
					"fontsize" : 24.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 272.0, 192.0, 586.0, 34.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-22",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/pointer/horizontal 461 \n/pointer/vertical 95 \n/pointer/delta/horizontal 0 \n/pointer/delta/vertical 0 ",
					"linecount" : 4,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 432.0, 48.0, 155.0, 62.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-18",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "osc-pointer",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 1,
					"patching_rect" : [ 536.0, 16.0, 70.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-17",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "osc-gametrak",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 1,
					"patching_rect" : [ 208.0, 16.0, 83.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-1",
					"fontname" : "Arial"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-39", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-36", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-18", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-34", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-37", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
