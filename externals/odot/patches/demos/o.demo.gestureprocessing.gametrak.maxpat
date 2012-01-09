{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 263.0, 126.0, 1017.0, 782.0 ],
		"bglocked" : 0,
		"defrect" : [ 263.0, 126.0, 1017.0, 782.0 ],
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
					"maxclass" : "o.message",
					"text" : "/right/z 0.00952381 \n/left/y 0.390965 \n/gametrak/footswitch 0 \n/right/z 0.00952381 \n/right/y -0.497924 \n/right/x 0.671306 \n/left/z 0.00854701 \n/left/y 0.390965 \n/left/x 0.40757 \n",
					"linecount" : 9,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"presentation_rect" : [ 505.0, 227.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-13",
					"patching_rect" : [ 505.0, 227.0, 150.0, 131.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.io.gametrak",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-12",
					"patching_rect" : [ 29.0, 105.0, 83.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-11",
					"patching_rect" : [ 675.0, 315.0, 150.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Debug",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"id" : "obj-45",
					"patching_rect" : [ 488.0, 1099.0, 150.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gswitch",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-43",
					"patching_rect" : [ 179.0, 1078.0, 41.0, 32.0 ],
					"numinlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/left/y -0.1875 \n/left/x 0.09375 \n/left/radius \n/left/orientation \n/left/delta/orientation \n/left/anticlockwise 0 \n/left/clockwise 0 \n/left/radius/delta -inf \n/left/was/y -0.1875 \n/left/was/x 0.09375 \n/left/was/radius \n/left/was/orientation \n/left/z 0.528 \n/left/was/z 0.28 \n/left/pluck 0 \n",
					"linecount" : 15,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-42",
					"patching_rect" : [ 188.0, 1123.0, 242.0, 213.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-40",
					"patching_rect" : [ 40.0, 1047.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : ""
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.prepend /left",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-39",
					"patching_rect" : [ 29.0, 972.0, 87.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Aggregate",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"id" : "obj-37",
					"patching_rect" : [ 149.0, 1020.0, 150.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b l",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "bang", "" ],
					"id" : "obj-35",
					"patching_rect" : [ 30.0, 996.0, 32.5, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.accumulate",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-33",
					"patching_rect" : [ 40.0, 1026.0, 82.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Threshhold",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"id" : "obj-32",
					"patching_rect" : [ 601.0, 897.0, 150.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Flag detected features\n\n\n",
					"linecount" : 3,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"id" : "obj-30",
					"patching_rect" : [ 403.0, 935.0, 150.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-23",
					"patching_rect" : [ 321.0, 983.0, 20.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "select 1",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "bang", "" ],
					"id" : "obj-25",
					"patching_rect" : [ 319.0, 947.0, 52.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-19",
					"patching_rect" : [ 240.0, 984.0, 20.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "select 1",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "bang", "" ],
					"id" : "obj-22",
					"patching_rect" : [ 242.0, 947.0, 52.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-18",
					"patching_rect" : [ 155.0, 978.0, 20.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "select 1",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "bang", "" ],
					"id" : "obj-15",
					"patching_rect" : [ 164.0, 946.0, 52.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /pluck /anticlockwise /clockwise",
					"numoutlets" : 4,
					"fontname" : "Arial",
					"outlettype" : [ "", "", "", "FullPacket" ],
					"id" : "obj-14",
					"patching_rect" : [ 164.0, 921.0, 218.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 4
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /radius/delta = /radius - median(/window/radius)",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-7",
					"patching_rect" : [ 27.0, 810.0, 300.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /radius [zl stream 5] @as /window/radius",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "" ],
					"id" : "obj-3",
					"patching_rect" : [ 26.0, 782.0, 257.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "bang" ],
					"id" : "obj-10",
					"patching_rect" : [ 339.0, 781.0, 60.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/pluck/upper/threshhold 0.05 \n/pluck/lower/threshhold -0.05",
					"linecount" : 2,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-9",
					"patching_rect" : [ 339.0, 806.0, 321.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.multisliders /pluck/upper/threshhold 0. 0.2 /pluck/lower/threshhold 0. -0.2",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-80",
					"patching_rect" : [ 339.0, 845.0, 405.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Include Video of how it might be used",
					"linecount" : 2,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"id" : "obj-79",
					"patching_rect" : [ 704.0, 239.0, 150.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Novelty Detection\n\n\n\n\n",
					"linecount" : 5,
					"numoutlets" : 0,
					"bgcolor" : [ 0.192157, 0.67451, 0.360784, 0.0 ],
					"fontname" : "Arial",
					"id" : "obj-75",
					"patching_rect" : [ 767.0, 795.0, 150.0, 75.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.union",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-68",
					"patching_rect" : [ 29.0, 863.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /pluck = (/radius/delta > /pluck/upper/threshhold) || (/radius/delta < /pluck/lower/threshhold)",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-70",
					"patching_rect" : [ 31.0, 899.0, 527.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Feature\nDetection\n\n\n\n\n",
					"linecount" : 6,
					"numoutlets" : 0,
					"bgcolor" : [ 0.192157, 0.67451, 0.360784, 0.0 ],
					"fontname" : "Arial",
					"id" : "obj-67",
					"patching_rect" : [ 477.0, 674.0, 64.0, 89.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Gestural Input Device Workflow Illustration. Adrian Freed 2011 CNMAT",
					"linecount" : 4,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"id" : "obj-65",
					"patching_rect" : [ 220.0, 15.0, 150.0, 62.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Identify",
					"numoutlets" : 0,
					"bgcolor" : [ 0.192157, 0.67451, 0.360784, 0.0 ],
					"fontname" : "Arial",
					"id" : "obj-63",
					"patching_rect" : [ 404.0, 59.0, 150.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr (/anticlockwise = /orientation > 0.001)\\; (/clockwise = /orientation < -0.001)",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-20",
					"patching_rect" : [ 27.0, 733.0, 437.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Select",
					"numoutlets" : 0,
					"bgcolor" : [ 0.192157, 0.67451, 0.360784, 0.0 ],
					"fontname" : "Arial",
					"id" : "obj-59",
					"patching_rect" : [ 435.0, 576.0, 150.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /left",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "", "FullPacket" ],
					"id" : "obj-57",
					"patching_rect" : [ 28.0, 589.0, 71.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "unit delay",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"id" : "obj-2",
					"patching_rect" : [ 196.0, 677.0, 76.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /delta/orientation = /orientation - /was/orientation",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-55",
					"patching_rect" : [ 28.0, 707.0, 304.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.remember /orientation",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"id" : "obj-56",
					"patching_rect" : [ 28.0, 678.0, 138.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Fuse by conversion to polar coordinates",
					"linecount" : 2,
					"numoutlets" : 0,
					"bgcolor" : [ 0.192157, 0.67451, 0.360784, 0.0 ],
					"fontname" : "Arial",
					"id" : "obj-54",
					"patching_rect" : [ 438.0, 613.0, 150.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /orientation = atan2(/x\\,/y)",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-50",
					"patching_rect" : [ 28.0, 647.0, 184.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr (/radius = sqrt(/x * /x + /y * //y) )",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-41",
					"patching_rect" : [ 28.0, 621.0, 213.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /*/[xy] * 30. @prepending /degrees",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "" ],
					"id" : "obj-29",
					"patching_rect" : [ 28.0, 533.0, 225.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /*/z * 2. @prepending /meters",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "" ],
					"id" : "obj-5",
					"patching_rect" : [ 28.0, 559.0, 199.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Model\nDisplay and simulate interaction using direct manipulation substitution\nof the original",
					"linecount" : 5,
					"numoutlets" : 0,
					"bgcolor" : [ 0.192157, 0.67451, 0.360784, 0.0 ],
					"fontname" : "Arial",
					"id" : "obj-34",
					"patching_rect" : [ 426.0, 323.0, 147.0, 75.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Neutralize\n with respect to vendor numerical encoding",
					"linecount" : 3,
					"numoutlets" : 0,
					"bgcolor" : [ 0.192157, 0.67451, 0.360784, 0.0 ],
					"fontname" : "Arial",
					"id" : "obj-26",
					"patching_rect" : [ 418.0, 158.0, 150.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /*/z scale 4095 0 0. 1.",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "" ],
					"id" : "obj-24",
					"patching_rect" : [ 30.0, 148.0, 157.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /*/[xy] scale 0 4095 -1. 1.",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "" ],
					"id" : "obj-6",
					"patching_rect" : [ 30.0, 175.0, 174.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Situate\n with respect to the performer (note Mac HID numbers, adjust for PC)",
					"linecount" : 3,
					"numoutlets" : 0,
					"bgcolor" : [ 0.192157, 0.67451, 0.360784, 0.0 ],
					"fontname" : "Arial",
					"id" : "obj-21",
					"patching_rect" : [ 418.0, 101.0, 263.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-4",
					"patching_rect" : [ 31.0, 214.0, 322.0, 298.0 ],
					"name" : "o.io.gametrak.display.maxpat",
					"args" : [  ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Calibrate to a \nfamiliar norm",
					"linecount" : 2,
					"numoutlets" : 0,
					"bgcolor" : [ 0.172549, 0.556863, 0.305882, 0.0 ],
					"fontname" : "Arial",
					"id" : "obj-38",
					"patching_rect" : [ 435.0, 533.0, 150.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-13", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-11", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-80", 0 ],
					"destination" : [ "obj-68", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-70", 0 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-70", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-68", 0 ],
					"destination" : [ "obj-70", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 1 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [ 89.5, 615.0, 17.0, 615.0, 10.0, 1012.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-56", 0 ],
					"destination" : [ "obj-55", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-56", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-42", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 1 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-43", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 2 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 1 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
