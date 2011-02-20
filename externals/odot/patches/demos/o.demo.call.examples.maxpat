{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 24.0, 44.0, 1429.0, 940.0 ],
		"bglocked" : 0,
		"defrect" : [ 24.0, 44.0, 1429.0, 940.0 ],
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
					"maxclass" : "number",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 202.0, 576.0, 50.0, 20.0 ],
					"id" : "obj-61",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"setminmax" : [ 0.0, 127.0 ],
					"contdata" : 1,
					"numinlets" : 1,
					"patching_rect" : [ 96.0, 548.0, 197.0, 16.0 ],
					"id" : "obj-59",
					"numoutlets" : 2,
					"orientation" : 0,
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/number 0.256410 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 96.0, 627.0, 138.0, 20.0 ],
					"id" : "obj-56",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /number scale 0. 127. 0. 1.",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 94.0, 599.0, 184.0, 20.0 ],
					"id" : "obj-57",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/number $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 94.0, 576.0, 80.0, 20.0 ],
					"id" : "obj-58",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/numbers 0.300000 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 95.0, 509.0, 138.0, 20.0 ],
					"id" : "obj-53",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /numbers zl median",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 92.0, 482.0, 147.0, 20.0 ],
					"id" : "obj-54",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/numbers 0.2 0.4 0.4  -6.",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 94.0, 453.0, 179.0, 20.0 ],
					"id" : "obj-55",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Javascript !",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 490.0, 79.0, 150.0, 20.0 ],
					"id" : "obj-44",
					"fontname" : "Arial",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/pitches c b a ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 87.0, 222.0, 106.0, 20.0 ],
					"id" : "obj-49",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /pitches jstrigger (a.reverse())",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 87.0, 195.0, 199.0, 20.0 ],
					"id" : "obj-50",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/pitches a b c",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 87.0, 164.0, 91.0, 20.0 ],
					"id" : "obj-52",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Javascript String operations!",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1135.0, 695.0, 150.0, 34.0 ],
					"id" : "obj-51",
					"fontname" : "Arial",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/symbols jd ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 1159.0, 770.0, 138.0, 20.0 ],
					"id" : "obj-47",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /symbols jstrigger (a[0].charAt(0)+a[1].charAt(1))",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1065.0, 734.0, 299.0, 20.0 ],
					"id" : "obj-48",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Javascript !",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1055.0, 92.0, 150.0, 20.0 ],
					"id" : "obj-45",
					"fontname" : "Arial",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/numbers -6.000000 0.400000 0.400000 0.200000 ",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 905.0, 159.0, 197.0, 34.0 ],
					"id" : "obj-42",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /numbers jstrigger (a.reverse())",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 975.0, 119.0, 207.0, 20.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/number 0.579710 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 147.0, 838.0, 246.0, 20.0 ],
					"id" : "obj-28",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/number 1.932367 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 529.0, 803.0, 138.0, 20.0 ],
					"id" : "obj-30",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /number scale 0. 0.3 0. 1.",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 439.0, 755.0, 177.0, 20.0 ],
					"id" : "obj-31",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/numbers 0.2  03. -1.1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 732.0, 727.0, 246.0, 20.0 ],
					"id" : "obj-32",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/numbers -1.100000 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 874.0, 956.0, 138.0, 20.0 ],
					"id" : "obj-33",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /numbers minimum -1.",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 843.0, 885.0, 161.0, 20.0 ],
					"id" : "obj-34",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/number 0.761387 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 580.0, 967.0, 138.0, 20.0 ],
					"id" : "obj-35",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /number sqrt",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 651.0, 889.0, 109.0, 20.0 ],
					"id" : "obj-36",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/number -0.579710 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 412.0, 962.0, 138.0, 20.0 ],
					"id" : "obj-37",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /number * -1.",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 480.0, 883.0, 111.0, 20.0 ],
					"id" : "obj-38",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/number 1.579710 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 145.0, 969.0, 138.0, 20.0 ],
					"id" : "obj-39",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /number + 1.",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 295.0, 882.0, 110.0, 20.0 ],
					"id" : "obj-40",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/number $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 361.0, 663.0, 246.0, 20.0 ],
					"id" : "obj-41",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/symbols john adrian Matt sir ",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 1224.0, 496.0, 138.0, 34.0 ],
					"id" : "obj-26",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /symbols zl join sir",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1166.0, 462.0, 139.0, 20.0 ],
					"id" : "obj-27",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/e 0.031416 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 91.0, 316.0, 86.0, 20.0 ],
					"id" : "obj-23",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/e 0.1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 87.0, 262.0, 58.0, 20.0 ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /e expr $f1 * $f1 * 3.141593",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 86.0, 289.0, 188.0, 20.0 ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/symbols adrian ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 1194.0, 598.0, 138.0, 20.0 ],
					"id" : "obj-21",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /symbols zl nth 2",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1164.0, 548.0, 131.0, 20.0 ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/symbols Matt adrian john ",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 972.0, 602.0, 138.0, 34.0 ],
					"id" : "obj-19",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /symbols zl sort",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 942.0, 551.0, 125.0, 20.0 ],
					"id" : "obj-20",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/symbols Matt john adrian ",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 758.0, 601.0, 138.0, 34.0 ],
					"id" : "obj-15",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /symbols zl scramble",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 728.0, 551.0, 153.0, 20.0 ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/symbols 3 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 453.0, 605.0, 138.0, 20.0 ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /symbols zl len",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 521.0, 526.0, 121.0, 20.0 ],
					"id" : "obj-18",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/numbers 0.400000 0.400000 0.200000 -6.000000 ",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 1216.0, 329.0, 138.0, 48.0 ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /numbers zl scramble",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1177.0, 266.0, 156.0, 20.0 ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/numbers 4 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 898.0, 366.0, 138.0, 20.0 ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /numbers zl len",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 964.0, 286.0, 123.0, 20.0 ],
					"id" : "obj-12",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/numbers -5.000000 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 730.0, 361.0, 138.0, 20.0 ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /numbers zl sum",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 798.0, 282.0, 130.0, 20.0 ],
					"id" : "obj-9",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/numbers 0.300000 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 463.0, 368.0, 138.0, 20.0 ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /numbers zl median",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 616.0, 284.0, 147.0, 20.0 ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/symbols  john adrian Matt",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 759.0, 418.0, 246.0, 20.0 ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/numbers 0.2 0.4 0.4  -6.",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 603.0, 98.0, 246.0, 20.0 ],
					"id" : "obj-7",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/vector 0.040000 1.000000 4.000000 ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 93.0, 417.0, 215.0, 20.0 ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/vector 0.2 1.0 2.0",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 91.0, 357.0, 121.0, 20.0 ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /vector vexpr $f1 * $f1",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 91.0, 386.0, 159.0, 20.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-15", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-56", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-58", 0 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-49", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-54", 0 ],
					"destination" : [ "obj-53", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-54", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-4", 1 ],
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
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-23", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 0 ],
					"destination" : [ "obj-58", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-28", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-8", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-30", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-36", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-35", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-33", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-47", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-21", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-26", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-19", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-17", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-39", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-37", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-13", 1 ],
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
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-42", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-10", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-11", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
