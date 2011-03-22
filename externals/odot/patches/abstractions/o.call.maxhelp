{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 419.0, 44.0, 1109.0, 773.0 ],
		"bglocked" : 0,
		"defrect" : [ 419.0, 44.0, 1109.0, 773.0 ],
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
					"text" : "/black 19\n/flag 84\n/foo 98\n/bar 34",
					"linecount" : 4,
					"fontsize" : 12.0,
					"presentation_rect" : [ 656.0, 495.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 656.0, 496.0, 120.0, 62.0 ],
					"numoutlets" : 1,
					"id" : "obj-11",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/black 19 \n/flag 84 \n/foo 3.01238 \n/bar 34 \n",
					"linecount" : 4,
					"fontsize" : 12.0,
					"presentation_rect" : [ 656.0, 615.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 656.0, 616.0, 121.0, 62.0 ],
					"numoutlets" : 1,
					"id" : "obj-12",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /foo /bar /black /flag vexpr ($f1 + $f4 / $f3) / $f2 @scalarmode 1 @appending /jeff",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 656.0, 568.0, 473.0, 20.0 ],
					"numoutlets" : 3,
					"id" : "obj-19",
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "", "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/left/xyz 0.5 0.3 0.2 \n",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 392.0, 16.0, 145.0, 20.0 ],
					"numoutlets" : 1,
					"id" : "obj-24",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/left/xyz 0.5 0.3 0.2 \n/left/z 0.2 \n/left/y 0.3 \n/left/x 0.5 \n",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 392.0, 136.0, 145.0, 62.0 ],
					"numoutlets" : 1,
					"id" : "obj-10",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /left/xyz unpack 0. 0. 0. @multiple-value-set /x /y /z @prepending /left",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 392.0, 88.0, 410.0, 20.0 ],
					"numoutlets" : 3,
					"id" : "obj-23",
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "", "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/black 19\n/flag 84\n/foo 98\n/bar 34",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 656.0, 272.0, 120.0, 62.0 ],
					"numoutlets" : 1,
					"id" : "obj-5",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo/bar 89 \n/foo/bloo 88 \n/jeff/foo/bar 44.5 \n/jeff/foo/bloo 44 \n",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 392.0, 680.0, 121.0, 62.0 ],
					"numoutlets" : 1,
					"id" : "obj-7",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo/bar 89\n/foo/bloo 88",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 392.0, 568.0, 141.0, 34.0 ],
					"numoutlets" : 1,
					"id" : "obj-8",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /foo/* / 2. @prepending /jeff",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 392.0, 616.0, 188.0, 20.0 ],
					"numoutlets" : 3,
					"id" : "obj-9",
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "", "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/black 19 \n/flag 84 \n/foo 3.01238 \n/bar 34 \n",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 656.0, 392.0, 121.0, 62.0 ],
					"numoutlets" : 1,
					"id" : "obj-4",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /foo /bar /black /flag vexpr ($f1 + $f4 / $f3) / $f2 @scalarmode 1",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 656.0, 344.0, 379.0, 20.0 ],
					"numoutlets" : 3,
					"id" : "obj-6",
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "", "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/black 19 \n/flag 84 \n/foo 98 \n/bar 34 \n/jeff/foo 3.01238 \n",
					"linecount" : 5,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 120.0, 592.0, 121.0, 75.0 ],
					"numoutlets" : 1,
					"id" : "obj-1",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/black 19\n/flag 84\n/foo 98\n/bar 34",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 120.0, 456.0, 120.0, 62.0 ],
					"numoutlets" : 1,
					"id" : "obj-2",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /foo /bar /black /flag vexpr ($f1 + $f4 / $f3) / $f2 @scalarmode 1 @prepending /jeff",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 120.0, 528.0, 477.0, 20.0 ],
					"numoutlets" : 3,
					"id" : "obj-3",
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "", "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/black 19 \n/flag 84 \n/foo 98 \n/bar 34 \n/jeff 3.01238 \n",
					"linecount" : 5,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 120.0, 336.0, 121.0, 75.0 ],
					"numoutlets" : 1,
					"id" : "obj-18",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/black 19\n/flag 84\n/foo 98\n/bar 34",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 120.0, 232.0, 120.0, 62.0 ],
					"numoutlets" : 1,
					"id" : "obj-17",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /foo /bar /black /flag vexpr ($f1 + $f4 / $f3) / $f2 @scalarmode 1 @as /jeff",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 120.0, 304.0, 429.0, 20.0 ],
					"numoutlets" : 3,
					"id" : "obj-16",
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "", "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/left/x 0.5 \n/left/y 0.3 \n/left/z 0.2 \n/left/xyz 0.5 0.3 0.2 \n",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 120.0, 136.0, 145.0, 62.0 ],
					"numoutlets" : 1,
					"id" : "obj-15",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/left/x .5\n/left/y .3\n/left/z .2",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 120.0, 16.0, 120.0, 48.0 ],
					"numoutlets" : 1,
					"id" : "obj-14",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.call /*/x /*/y /*/z pack 0. 0. 0. @as /left/xyz",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 120.0, 88.0, 241.0, 20.0 ],
					"numoutlets" : 3,
					"id" : "obj-13",
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket", "", "FullPacket" ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-12", 1 ],
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
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-10", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-15", 1 ],
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
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-18", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 1 ],
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
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-4", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
