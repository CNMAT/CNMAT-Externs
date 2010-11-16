{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 559.0, 44.0, 1017.0, 645.0 ],
		"bglocked" : 0,
		"defrect" : [ 559.0, 44.0, 1017.0, 645.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 10.0, 10.0 ],
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
					"text" : "/crapstack/foo 34 \n/bloo 54.799999 \n/crapstack/foo/fou 83 evil ",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 90.0, 130.0, 143.0, 48.0 ],
					"id" : "obj-16",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/bar/foo 34 \n/bar/bloo 54.799999 ",
					"linecount" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 490.0, 440.0, 143.0, 34.0 ],
					"id" : "obj-15",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/foo 34, /bloo 54.8, bang",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 490.0, 350.0, 140.0, 18.0 ],
					"id" : "obj-10",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "OpenSoundControl",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 490.0, 380.0, 113.0, 20.0 ],
					"id" : "obj-11",
					"numoutlets" : 3,
					"fontname" : "Arial",
					"outlettype" : [ "", "", "OSCTimeTag" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.prepend /bar",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 490.0, 410.0, 89.0, 20.0 ],
					"id" : "obj-12",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "prepend /bar to any incoming address string that starts with /foo",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 230.0, 90.0, 370.0, 20.0 ],
					"id" : "obj-9",
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "prepend /bar to every address",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 610.0, 410.0, 349.0, 20.0 ],
					"id" : "obj-7",
					"numoutlets" : 0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/foo 34, /bloo 54.8, /foo/fou 83 evil, bang",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 90.0, 30.0, 225.0, 18.0 ],
					"id" : "obj-4",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "OpenSoundControl",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 90.0, 60.0, 113.0, 20.0 ],
					"id" : "obj-2",
					"numoutlets" : 3,
					"fontname" : "Arial",
					"outlettype" : [ "", "", "OSCTimeTag" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.prepend /foo /bar",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 90.0, 90.0, 112.0, 20.0 ],
					"id" : "obj-1",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"outlettype" : [ "FullPacket" ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-16", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-15", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
