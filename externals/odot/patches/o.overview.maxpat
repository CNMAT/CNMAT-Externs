{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ -1644.0, 88.0, 1503.0, 830.0 ],
		"bglocked" : 0,
		"defrect" : [ -1644.0, 88.0, 1503.0, 830.0 ],
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
					"maxclass" : "comment",
					"text" : "Order",
					"fontface" : 1,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 663.0, 623.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 664.0, 624.0, 162.0, 20.0 ],
					"id" : "obj-127",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"presentation_rect" : [ 652.0, 615.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 656.0, 616.0, 766.0, 34.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-125",
					"border" : 1,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "in general, where possible, the order of the messages in a bundle is preserved whenever operations are performed on the contents of a bundle",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 1128.0, 645.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 1136.0, 656.0, 231.0, 62.0 ],
					"id" : "obj-124",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.mappatch",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "FullPacket", "" ],
					"patching_rect" : [ 720.0, 768.0, 73.0, 20.0 ],
					"id" : "obj-123",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.spew",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 664.0, 768.0, 49.0, 20.0 ],
					"id" : "obj-122",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.spew with no arguments and o.mappatch output the contents in order",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 656.0, 720.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 664.0, 728.0, 231.0, 34.0 ],
					"id" : "obj-121",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.spew /mysynth",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 946.0, 661.0, 0.0, 0.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "FullPacket" ],
					"patching_rect" : [ 873.484558, 696.0, 100.0, 20.0 ],
					"id" : "obj-118",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.select /mysynth",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 794.0, 661.0, 0.0, 0.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "FullPacket" ],
					"patching_rect" : [ 766.70105, 696.0, 103.0, 20.0 ],
					"id" : "obj-119",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /mysynth",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 658.0, 661.0, 0.0, 0.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "FullPacket" ],
					"patching_rect" : [ 664.0, 696.0, 99.0, 20.0 ],
					"id" : "obj-120",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.route, o.select, and o.spew output data according to the order of the arguments--from right to left beginning with any unmatched addresses",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 656.0, 625.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 664.0, 656.0, 393.0, 34.0 ],
					"id" : "obj-117",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.prepend and o.rename can be used to manipulate the address of an OSC message",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 1190.0, 73.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 1176.0, 72.0, 248.0, 34.0 ],
					"id" : "obj-115",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 20 \n",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1215.0, 123.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1216.0, 136.0, 73.0, 20.0 ],
					"id" : "obj-114",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo/bar 20 \n",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1120.0, 116.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1120.0, 136.0, 73.0, 20.0 ],
					"id" : "obj-113",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/bar 20",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1116.0, 74.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1120.0, 88.0, 46.0, 20.0 ],
					"id" : "obj-112",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.prepend /foo",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 1118.0, 95.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 1120.0, 112.0, 88.0, 20.0 ],
					"id" : "obj-111",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Operations on addresses",
					"fontface" : 1,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 1118.0, 41.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 1120.0, 40.0, 162.0, 20.0 ],
					"id" : "obj-108",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"presentation_rect" : [ 1110.0, 33.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 1112.0, 32.0, 310.0, 34.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-109",
					"border" : 1,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Operations on data",
					"fontface" : 1,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 663.0, 191.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 664.0, 192.0, 162.0, 20.0 ],
					"id" : "obj-107",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.mappatch outputs each message in sequence and constructs a new bundle based input received in the right inlet.",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 1179.0, 384.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 1176.0, 424.0, 228.0, 48.0 ],
					"id" : "obj-104",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1074.0, 353.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"hidden" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1040.0, 376.0, 50.0, 18.0 ],
					"id" : "obj-102",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1338.0, 370.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"hidden" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1400.0, 393.0, 32.0, 18.0 ],
					"id" : "obj-103",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend /midi",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1168.0, 560.0, 83.0, 20.0 ],
					"id" : "obj-101",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 10.",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"patching_rect" : [ 1216.0, 536.0, 34.0, 20.0 ],
					"id" : "obj-100",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/midi 69 \n/gain 0.1 \n/nada 0 \n",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1112.0, 474.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1104.0, 512.0, 61.0, 48.0 ],
					"id" : "obj-97",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "ftom 0.",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1168.0, 536.0, 47.0, 20.0 ],
					"id" : "obj-96",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /freq /gain",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "FullPacket" ],
					"patching_rect" : [ 1168.0, 512.0, 105.0, 20.0 ],
					"id" : "obj-84",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/freq 440.\n/gain 1.\n/nada 0.",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1109.0, 400.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1112.0, 416.0, 64.0, 48.0 ],
					"id" : "obj-81",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1033.0, 359.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"hidden" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1032.0, 584.0, 50.0, 18.0 ],
					"id" : "obj-74",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1033.0, 207.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"hidden" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1032.0, 216.0, 50.0, 18.0 ],
					"id" : "obj-78",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.var can perform grouping operations on bundles",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"presentation_rect" : [ 965.0, 201.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 1048.0, 232.0, 156.0, 34.0 ],
					"id" : "obj-71",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 20 \n",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1235.0, 332.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1245.639404, 360.0, 52.0, 20.0 ],
					"id" : "obj-70",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/bloo 128 \n/bar 44 \n",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1139.0, 334.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1174.819702, 344.0, 65.0, 34.0 ],
					"id" : "obj-56",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 66\n/bloo 128",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1222.0, 189.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1296.0, 232.0, 58.0, 34.0 ],
					"id" : "obj-50",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 20\n/bar 44",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1113.0, 191.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1208.0, 232.0, 49.0, 34.0 ],
					"id" : "obj-18",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 20 \n/bloo 128 \n/bar 44 \n",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 1036.0, 314.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1104.0, 328.0, 65.0, 48.0 ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.rename /bar /foo",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 1216.0, 112.0, 109.0, 20.0 ],
					"id" : "obj-3",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"hidden" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 664.0, 336.0, 50.0, 18.0 ],
					"id" : "obj-98",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"hidden" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1016.0, 353.0, 32.0, 18.0 ],
					"id" : "obj-99",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/x 0.5\n/y 0.3\n/z -.4",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 664.0, 384.0, 95.0, 48.0 ],
					"id" : "obj-95",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/x 0.5 \n/y 0.3 \n/z -0.4 \n/xyz 0.5 0.3 -0.4 \n",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 664.0, 464.0, 101.0, 62.0 ],
					"id" : "obj-94",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "equivalent to:",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 864.0, 440.0, 82.0, 20.0 ],
					"id" : "obj-93",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /xyz",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 944.0, 464.0, 71.0, 20.0 ],
					"id" : "obj-92",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pack 0. 0. 0.",
					"fontsize" : 12.0,
					"numinlets" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 944.0, 440.0, 76.0, 20.0 ],
					"id" : "obj-91",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /x /y /z",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "FullPacket" ],
					"patching_rect" : [ 944.0, 416.0, 86.0, 20.0 ],
					"id" : "obj-90",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "call a Max object with a set of OSC messages",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 760.0, 400.0, 172.0, 34.0 ],
					"id" : "obj-89",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /x /y /z pack 0. 0. 0. @as /xyz",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "FullPacket", "", "FullPacket" ],
					"patching_rect" : [ 664.0, 440.0, 198.0, 20.0 ],
					"id" : "obj-88",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "construct expressions that use OSC data as arguments (like expr and vexpr)",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 808.0, 264.0, 166.0, 48.0 ],
					"id" : "obj-87",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-constants",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 720.0, 248.0, 89.0, 18.0 ],
					"id" : "obj-86",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-functions",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 720.0, 224.0, 85.0, 18.0 ],
					"id" : "obj-85",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 2 \n/result 25.1327 \n",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 664.0, 296.0, 95.0, 34.0 ],
					"id" : "obj-83",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 2",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 664.0, 224.0, 50.0, 20.0 ],
					"id" : "obj-82",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 864.0, 72.0, 20.0, 20.0 ],
					"id" : "obj-66"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /foo 10 /bar 50",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 864.0, 96.0, 127.0, 20.0 ],
					"id" : "obj-67",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.message can be used like o.var",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 984.0, 120.0, 110.0, 34.0 ],
					"id" : "obj-68",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 \n/bar 50 \n",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 864.0, 120.0, 117.0, 34.0 ],
					"id" : "obj-69",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "left inlet causes immediate output, right inlet sets the contents (like zl reg)",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 704.0, 96.0, 153.0, 48.0 ],
					"id" : "obj-65",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "stores a bundle",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 704.0, 80.0, 150.0, 20.0 ],
					"id" : "obj-64",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Storage",
					"fontface" : 1,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 664.0, 40.0, 162.0, 20.0 ],
					"id" : "obj-63",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 656.0, 32.0, 441.0, 34.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-58",
					"border" : 1,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 656.0, 32.0, 441.0, 138.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-62",
					"border" : 2,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 32.0, 704.0, 20.0, 20.0 ],
					"id" : "obj-57"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /foo 10 /bar 50",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 32.0, 728.0, 127.0, 20.0 ],
					"id" : "obj-55",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "the right inlet of o.message sets the contents of the message box which can be useful for visualizing OSC data",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 152.0, 752.0, 329.0, 34.0 ],
					"id" : "obj-54",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 10 \n/bar 50 \n",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 32.0, 752.0, 117.0, 34.0 ],
					"id" : "obj-52",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Diagnostics",
					"fontface" : 1,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 32.0, 600.0, 163.0, 20.0 ],
					"id" : "obj-45",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 24.0, 592.0, 608.0, 34.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-37",
					"border" : 1,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "prints each byte to the max window.  useful for diagnosing incorrectly formatted OSC bundles",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 352.0, 672.0, 266.0, 34.0 ],
					"id" : "obj-25",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/1/gain 1.0\n/mysynth/1/freq 440.",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 280.0, 632.0, 117.0, 34.0 ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.printbytes",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 280.0, 680.0, 73.0, 20.0 ],
					"id" : "obj-33",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "prints the contenst of the bundle to the max window",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 80.0, 672.0, 150.0, 34.0 ],
					"id" : "obj-21",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/1/gain 1.0\n/mysynth/1/freq 440.",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 32.0, 632.0, 117.0, 34.0 ],
					"id" : "obj-19",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "supports the same syntax and functions as o.expr",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 184.0, 520.0, 150.0, 34.0 ],
					"id" : "obj-13",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "routes the full bundle out the left outlet if the test test is non-zero, or out the right outlet if the test results in 0",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 184.0, 464.0, 217.0, 48.0 ],
					"id" : "obj-12",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 152.0, 528.0, 20.0, 20.0 ],
					"id" : "obj-80"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 32.0, 528.0, 20.0, 20.0 ],
					"id" : "obj-79"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/1/gain 1.0\n/unmatched 20.",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 32.0, 464.0, 117.0, 34.0 ],
					"id" : "obj-77",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "with no arguments, o.spew dumps the contents of a bundle as a series of Max messages (similar to OpenSoundControl)",
					"linecount" : 5,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 480.0, 368.0, 156.0, 75.0 ],
					"id" : "obj-73",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.spew",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 480.0, 344.0, 49.0, 20.0 ],
					"id" : "obj-75",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/1/gain 1 \n",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 32.0, 376.0, 61.0, 20.0 ],
					"id" : "obj-72",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "leave the address in tact and output the result as a Max message",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 320.0, 400.0, 150.0, 48.0 ],
					"id" : "obj-61",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "leave the address in tact",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 168.0, 400.0, 150.0, 20.0 ],
					"id" : "obj-60",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "strip off the portion of the address that matched ",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 24.0, 400.0, 144.0, 34.0 ],
					"id" : "obj-59",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/mysynth/1/gain 1.",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 320.0, 376.0, 108.0, 18.0 ],
					"id" : "obj-53",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/1/gain 1 \n",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 168.0, 376.0, 109.0, 20.0 ],
					"id" : "obj-51",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/1/gain 1.0\n/unmatched 20.",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 32.0, 296.0, 117.0, 34.0 ],
					"id" : "obj-49",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.spew /mysynth",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "FullPacket" ],
					"patching_rect" : [ 320.0, 344.0, 100.0, 20.0 ],
					"id" : "obj-48",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.select /mysynth",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "FullPacket" ],
					"patching_rect" : [ 168.0, 344.0, 103.0, 20.0 ],
					"id" : "obj-47",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Routing and selecting",
					"fontface" : 1,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 32.0, 264.0, 163.0, 20.0 ],
					"id" : "obj-46",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 24.0, 256.0, 608.0, 34.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-44",
					"border" : 1,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"hidden" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 328.0, 128.0, 50.0, 18.0 ],
					"id" : "obj-43",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"hidden" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 592.0, 145.0, 32.0, 18.0 ],
					"id" : "obj-42",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"hidden" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 312.0, 216.0, 50.0, 18.0 ],
					"id" : "obj-41",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"hidden" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 312.0, 64.0, 50.0, 18.0 ],
					"id" : "obj-40",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "all inlets trigger output (like pak)",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 440.0, 200.0, 192.0, 20.0 ],
					"id" : "obj-35",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "left inlet triggers output (like pack)",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 440.0, 168.0, 192.0, 20.0 ],
					"id" : "obj-34",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "bind data to OSC addresses",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 440.0, 152.0, 192.0, 20.0 ],
					"id" : "obj-32",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "output on bang (similar to OpenSoundControl or zl group)",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 440.0, 104.0, 192.0, 34.0 ],
					"id" : "obj-31",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.bild /foo /bar",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 344.0, 200.0, 87.0, 20.0 ],
					"id" : "obj-30",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "accumulate OSC messages",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 440.0, 88.0, 192.0, 20.0 ],
					"id" : "obj-28",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "$n variable substution like the Max message box",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 160.0, 192.0, 150.0, 34.0 ],
					"id" : "obj-27",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 32.0, 168.0, 50.0, 20.0 ],
					"id" : "obj-26",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/1/gain 1.0\n/mysynth/1/freq $1",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 32.0, 192.0, 117.0, 34.0 ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "multiple messages separated by newlines",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 160.0, 96.0, 150.0, 34.0 ],
					"id" : "obj-23",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.message",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 32.0, 72.0, 150.0, 20.0 ],
					"id" : "obj-22",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Object creation",
					"fontface" : 1,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 32.0, 40.0, 162.0, 20.0 ],
					"id" : "obj-20",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.var @op intersection",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 1279.0, 336.0, 131.0, 20.0 ],
					"id" : "obj-17",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.var @op difference",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 1221.0, 312.0, 122.0, 20.0 ],
					"id" : "obj-16",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.var @op union",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 1150.0, 288.0, 99.0, 20.0 ],
					"id" : "obj-15",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.var",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 664.0, 88.0, 37.0, 20.0 ],
					"id" : "obj-14",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /mysynth",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "FullPacket" ],
					"patching_rect" : [ 32.0, 344.0, 99.0, 20.0 ],
					"id" : "obj-11",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.print",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 32.0, 680.0, 44.0, 20.0 ],
					"id" : "obj-9",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/1/gain 1.0\n/mysynth/1/freq 440.",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 32.0, 96.0, 117.0, 34.0 ],
					"id" : "obj-7",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.mappatch",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "FullPacket", "" ],
					"patching_rect" : [ 1112.0, 488.0, 73.0, 20.0 ],
					"id" : "obj-6",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.if /mysynth/1/gain < 10",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "FullPacket", "FullPacket" ],
					"patching_rect" : [ 32.0, 504.0, 141.0, 20.0 ],
					"id" : "obj-5",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr abs(/foo - 10) * PI",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 664.0, 272.0, 141.0, 20.0 ],
					"id" : "obj-4",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /foo /bar",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 344.0, 160.0, 93.0, 20.0 ],
					"id" : "obj-2",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.accumulate",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 344.0, 96.0, 82.0, 20.0 ],
					"id" : "obj-1",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 24.0, 32.0, 608.0, 34.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-39",
					"border" : 1,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 24.0, 32.0, 608.0, 207.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-36",
					"border" : 2,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 24.0, 256.0, 608.0, 316.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-76",
					"border" : 2,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 24.0, 592.0, 608.0, 207.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-38",
					"border" : 2,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"presentation_rect" : [ 659.0, 181.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 656.0, 184.0, 766.0, 34.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-105",
					"border" : 1,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"presentation_rect" : [ 659.0, 181.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 656.0, 184.0, 767.0, 416.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-106",
					"border" : 2,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"presentation_rect" : [ 1110.0, 33.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 1112.0, 32.0, 310.0, 138.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-110",
					"border" : 2,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"presentation_rect" : [ 652.0, 615.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"patching_rect" : [ 656.0, 616.0, 767.0, 183.0 ],
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"id" : "obj-126",
					"border" : 2,
					"rounded" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-114", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-112", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-111", 0 ],
					"destination" : [ "obj-113", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-112", 0 ],
					"destination" : [ "obj-111", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-102", 0 ],
					"destination" : [ "obj-103", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 0 ],
					"destination" : [ "obj-101", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-84", 2 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [ 1263.5, 546.0, 1274.0, 546.0, 1274.0, 482.0, 1175.5, 482.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-100", 0 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [ 1225.5, 558.0, 1281.0, 558.0, 1281.0, 476.0, 1175.5, 476.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-101", 0 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [ 1177.5, 585.0, 1289.0, 585.0, 1289.0, 471.0, 1175.5, 471.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-84", 1 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-84", 0 ],
					"destination" : [ "obj-96", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 1 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-97", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-81", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-78", 0 ],
					"destination" : [ "obj-74", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-10", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-56", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-70", 1 ],
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
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-17", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-16", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-15", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 1 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-79", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-24", 0 ],
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
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-51", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-53", 1 ],
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
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-72", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-55", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-52", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-67", 0 ],
					"destination" : [ "obj-69", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-67", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-86", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-85", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-83", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-82", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-90", 0 ],
					"destination" : [ "obj-91", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-90", 1 ],
					"destination" : [ "obj-91", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-90", 2 ],
					"destination" : [ "obj-91", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-91", 0 ],
					"destination" : [ "obj-92", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-95", 0 ],
					"destination" : [ "obj-88", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-88", 0 ],
					"destination" : [ "obj-94", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-98", 0 ],
					"destination" : [ "obj-99", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
