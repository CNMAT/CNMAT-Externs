{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 589.0, 69.0, 1047.0, 818.0 ],
		"bglocked" : 0,
		"defrect" : [ 589.0, 69.0, 1047.0, 818.0 ],
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
					"maxclass" : "o.message",
					"text" : "/foo 10 15 \n/result 15 15 15 15 15 15 15 15 15 15 \n",
					"linecount" : 3,
					"numinlets" : 2,
					"patching_rect" : [ 240.0, 760.0, 148.0, 48.0 ],
					"id" : "obj-73",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15",
					"numinlets" : 2,
					"patching_rect" : [ 240.0, 704.0, 75.0, 20.0 ],
					"id" : "obj-72",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr make_list(10\\, /foo[[1]])",
					"numinlets" : 1,
					"patching_rect" : [ 240.0, 736.0, 169.0, 20.0 ],
					"id" : "obj-71",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15 20 \n/result 20 15 10 \n",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 40.0, 496.0, 98.0, 34.0 ],
					"id" : "obj-70",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15 20",
					"numinlets" : 2,
					"patching_rect" : [ 40.0, 448.0, 79.0, 20.0 ],
					"id" : "obj-69",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr reverse(/foo)",
					"numinlets" : 1,
					"patching_rect" : [ 40.0, 472.0, 115.0, 20.0 ],
					"id" : "obj-68",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 1 2 3 \n/bar 5 6 7 \n/bloo 10 11 12 \n/result 1 2 3 \n",
					"linecount" : 4,
					"numinlets" : 2,
					"patching_rect" : [ 720.0, 440.0, 90.0, 62.0 ],
					"id" : "obj-67",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 1 2 3\n/bar 5 6 7\n/bloo 10 11 12",
					"linecount" : 3,
					"numinlets" : 2,
					"patching_rect" : [ 720.0, 352.0, 84.0, 48.0 ],
					"id" : "obj-66",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo[[0\\,1\\,2]]",
					"numinlets" : 1,
					"patching_rect" : [ 720.0, 408.0, 114.0, 20.0 ],
					"id" : "obj-65",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 1 2 3 \n/bar 5 6 7 \n/bloo 10 11 12 \n/result 1 2 3 5 6 7 10 11 12 \n",
					"linecount" : 4,
					"numinlets" : 2,
					"patching_rect" : [ 424.0, 128.0, 157.0, 62.0 ],
					"id" : "obj-64",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 1 2 3\n/bar 5 6 7\n/bloo 10 11 12",
					"linecount" : 3,
					"numinlets" : 2,
					"patching_rect" : [ 424.0, 32.0, 84.0, 48.0 ],
					"id" : "obj-63",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr concat(/foo\\, /bar\\, /bloo)",
					"numinlets" : 1,
					"patching_rect" : [ 424.0, 88.0, 177.0, 20.0 ],
					"id" : "obj-62",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /list[[/oddi]] @as /list/odd",
					"numinlets" : 1,
					"patching_rect" : [ 544.0, 648.0, 177.0, 20.0 ],
					"id" : "obj-61",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 1 2 3 4 5 6 7 8 9 10 \n/bar 0 2 4 6 8 \n/result 1 3 5 7 9 \n",
					"linecount" : 3,
					"numinlets" : 2,
					"patching_rect" : [ 544.0, 704.0, 142.0, 48.0 ],
					"id" : "obj-58",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/list 1 2 3 4 5 6 7 8 9 10\n/eveni 0 2 4 6 8 \n/oddi 1 3 5 7 9",
					"linecount" : 3,
					"numinlets" : 2,
					"patching_rect" : [ 544.0, 560.0, 139.0, 48.0 ],
					"id" : "obj-59",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /list[[/eveni]] @as /list/even",
					"numinlets" : 1,
					"patching_rect" : [ 544.0, 616.0, 189.0, 20.0 ],
					"id" : "obj-60",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-functiongraph",
					"numinlets" : 2,
					"patching_rect" : [ 480.0, 480.0, 110.0, 18.0 ],
					"id" : "obj-57",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-functiongraph",
					"numinlets" : 2,
					"patching_rect" : [ 32.0, 408.0, 110.0, 18.0 ],
					"id" : "obj-56",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-argvlex",
					"numinlets" : 2,
					"patching_rect" : [ 32.0, 376.0, 76.0, 18.0 ],
					"id" : "obj-53",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /y",
					"numinlets" : 1,
					"patching_rect" : [ 728.0, 600.0, 115.0, 20.0 ],
					"id" : "obj-55",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"outlettype" : [ "", "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"patching_rect" : [ 872.0, 560.0, 50.0, 20.0 ],
					"id" : "obj-54",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0., 50. 10000.",
					"numinlets" : 2,
					"patching_rect" : [ 832.0, 520.0, 84.0, 18.0 ],
					"id" : "obj-52",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "line 0.",
					"numinlets" : 3,
					"patching_rect" : [ 728.0, 512.0, 46.0, 20.0 ],
					"id" : "obj-51",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"numinlets" : 1,
					"patching_rect" : [ 728.0, 656.0, 255.0, 134.0 ],
					"id" : "obj-50",
					"numoutlets" : 2,
					"setstyle" : 2,
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/x $1",
					"numinlets" : 2,
					"patching_rect" : [ 728.0, 544.0, 66.0, 20.0 ],
					"id" : "obj-49",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr j0(/x) @as /y",
					"numinlets" : 1,
					"patching_rect" : [ 728.0, 568.0, 115.0, 20.0 ],
					"id" : "obj-48",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-constants",
					"numinlets" : 2,
					"patching_rect" : [ 192.0, 528.0, 89.0, 18.0 ],
					"id" : "obj-47",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15 \n/bar 1 2 3 \n/result 10.6366 15.6366 \n",
					"linecount" : 5,
					"numinlets" : 2,
					"patching_rect" : [ 112.0, 680.0, 69.0, 75.0 ],
					"id" : "obj-46",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15\n/bar 1 2 3",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 112.0, 592.0, 66.0, 34.0 ],
					"id" : "obj-45",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo + TWO_OVER_PI",
					"numinlets" : 1,
					"patching_rect" : [ 112.0, 648.0, 167.0, 20.0 ],
					"id" : "obj-44",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15 \n/result 0 1 \n",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 328.0, 680.0, 75.0, 34.0 ],
					"id" : "obj-40",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15\n/bar 1 2 3",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 328.0, 592.0, 66.0, 34.0 ],
					"id" : "obj-42",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr !(/foo > /bar)",
					"numinlets" : 1,
					"patching_rect" : [ 328.0, 640.0, 113.0, 20.0 ],
					"id" : "obj-43",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-functions",
					"numinlets" : 2,
					"patching_rect" : [ 160.0, 504.0, 85.0, 18.0 ],
					"id" : "obj-41",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr",
					"numinlets" : 1,
					"patching_rect" : [ 160.0, 560.0, 44.0, 20.0 ],
					"id" : "obj-39",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15 \n/bar 1 2 3 \n/average 12.5 \n",
					"linecount" : 3,
					"numinlets" : 2,
					"patching_rect" : [ 408.0, 536.0, 89.0, 48.0 ],
					"id" : "obj-36",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15\n/bar 1 2 3",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 408.0, 464.0, 66.0, 34.0 ],
					"id" : "obj-37",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr sum(/foo) / length(/foo) @as /average",
					"numinlets" : 1,
					"patching_rect" : [ 408.0, 504.0, 247.0, 20.0 ],
					"id" : "obj-38",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15\n/bar 1 2 3",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 528.0, 416.0, 66.0, 34.0 ],
					"id" : "obj-35",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15\n/bar 1 2 3",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 528.0, 344.0, 66.0, 34.0 ],
					"id" : "obj-34",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr sum(/foo)",
					"numinlets" : 1,
					"patching_rect" : [ 528.0, 384.0, 98.0, 20.0 ],
					"id" : "obj-33",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15 \n/result 0 1 \n",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 824.0, 320.0, 75.0, 34.0 ],
					"id" : "obj-30",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15\n/bar 1 2 3",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 824.0, 232.0, 66.0, 34.0 ],
					"id" : "obj-31",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo > /bar",
					"numinlets" : 1,
					"patching_rect" : [ 824.0, 280.0, 102.0, 20.0 ],
					"id" : "obj-32",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15 \n/result 0 1 \n",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 824.0, 120.0, 75.0, 34.0 ],
					"id" : "obj-27",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15",
					"numinlets" : 2,
					"patching_rect" : [ 824.0, 56.0, 62.0, 20.0 ],
					"id" : "obj-28",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo > 10",
					"numinlets" : 1,
					"patching_rect" : [ 824.0, 80.0, 94.0, 20.0 ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "if all arguments are lists, the operations will be applied to n elements of each list where n is the length of the shortest list",
					"linecount" : 5,
					"numinlets" : 1,
					"patching_rect" : [ 624.0, 256.0, 154.0, 75.0 ],
					"id" : "obj-26",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "binary operations where one arg is a list and the other is a scalar will result in the scalar being applied to all elements of the list.",
					"linecount" : 5,
					"numinlets" : 1,
					"patching_rect" : [ 624.0, 56.0, 154.0, 75.0 ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 3.14159 \n/result 10 \n",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 288.0, 416.0, 99.0, 34.0 ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 3.14159",
					"numinlets" : 2,
					"patching_rect" : [ 288.0, 360.0, 92.0, 20.0 ],
					"id" : "obj-23",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr round(/foo[[0]])",
					"numinlets" : 1,
					"patching_rect" : [ 288.0, 384.0, 126.0, 20.0 ],
					"id" : "obj-21",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"patching_rect" : [ 32.0, 312.0, 20.0, 20.0 ],
					"id" : "obj-22",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15 \n/result 100 15 \n",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 80.0, 312.0, 88.0, 34.0 ],
					"id" : "obj-20",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15",
					"numinlets" : 2,
					"patching_rect" : [ 80.0, 256.0, 62.0, 20.0 ],
					"id" : "obj-19",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr pow(/foo\\, 2.)",
					"numinlets" : 1,
					"patching_rect" : [ 80.0, 288.0, 117.0, 20.0 ],
					"id" : "obj-18",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15 \n",
					"numinlets" : 2,
					"patching_rect" : [ 384.0, 312.0, 75.0, 20.0 ],
					"id" : "obj-15",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15",
					"numinlets" : 2,
					"patching_rect" : [ 384.0, 248.0, 62.0, 20.0 ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo[[2]] > 10",
					"numinlets" : 1,
					"patching_rect" : [ 384.0, 272.0, 114.0, 20.0 ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15 \n/result 1 0 \n",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 216.0, 312.0, 75.0, 34.0 ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15",
					"numinlets" : 2,
					"patching_rect" : [ 216.0, 248.0, 62.0, 20.0 ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo <= 10",
					"numinlets" : 1,
					"patching_rect" : [ 216.0, 272.0, 101.0, 20.0 ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"patching_rect" : [ 128.0, 24.0, 20.0, 20.0 ],
					"id" : "obj-12",
					"numoutlets" : 1,
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "random 1000",
					"numinlets" : 2,
					"patching_rect" : [ 128.0, 88.0, 81.0, 20.0 ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "metro 1",
					"numinlets" : 2,
					"patching_rect" : [ 128.0, 64.0, 51.0, 20.0 ],
					"id" : "obj-9",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/result 22\n/foo 10 15\n/bar 88",
					"linecount" : 3,
					"numinlets" : 2,
					"patching_rect" : [ 24.0, 184.0, 66.0, 48.0 ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15\n/bar 88\n/result 22",
					"linecount" : 3,
					"numinlets" : 2,
					"patching_rect" : [ 24.0, 120.0, 66.0, 48.0 ],
					"id" : "obj-7",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo $1 15\n/result 22\n/bar 88",
					"linecount" : 3,
					"numinlets" : 2,
					"patching_rect" : [ 128.0, 120.0, 66.0, 48.0 ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.print",
					"numinlets" : 1,
					"patching_rect" : [ 312.0, 200.0, 44.0, 20.0 ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.printbytes",
					"numinlets" : 1,
					"patching_rect" : [ 216.0, 200.0, 73.0, 20.0 ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/bar 10 15",
					"numinlets" : 2,
					"patching_rect" : [ 288.0, 136.0, 63.0, 20.0 ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 15",
					"numinlets" : 2,
					"patching_rect" : [ 216.0, 136.0, 62.0, 20.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo + (4 + 3) @as /foo",
					"numinlets" : 1,
					"patching_rect" : [ 216.0, 168.0, 167.0, 20.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-71", 0 ],
					"destination" : [ "obj-73", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-72", 0 ],
					"destination" : [ "obj-71", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-65", 0 ],
					"destination" : [ "obj-67", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-65", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-24", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-60", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-61", 0 ],
					"destination" : [ "obj-58", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 0 ],
					"destination" : [ "obj-60", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-56", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-55", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-54", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-51", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-49", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-1", 0 ],
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
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-15", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-20", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-9", 0 ],
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
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
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
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-14", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-27", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-30", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-40", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 0 ],
					"destination" : [ "obj-46", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-44", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-53", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-63", 0 ],
					"destination" : [ "obj-62", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 0 ],
					"destination" : [ "obj-64", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-35", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-38", 0 ],
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
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-36", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-69", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-68", 0 ],
					"destination" : [ "obj-70", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
