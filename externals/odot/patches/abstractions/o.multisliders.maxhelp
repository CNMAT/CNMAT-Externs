{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 440.0, 57.0, 640.0, 480.0 ],
		"bglocked" : 0,
		"defrect" : [ 440.0, 57.0, 640.0, 480.0 ],
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
					"text" : "<--check out the bpatcher args here",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 411.0, 256.0, 204.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-11",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<--double click this ",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 467.0, 191.0, 150.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-10",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<--double click this ",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 476.0, 118.0, 150.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-9",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<--double click this ",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 456.0, 82.0, 150.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-8",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"name" : "o.multisliders.maxpat",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 154.0, 225.0, 239.0, 97.0 ],
					"args" : [ "/joe", -1, 1, 3, "/jim", "/him", 9, 3, "/silly", 2, "@ghostbar", 33 ],
					"numinlets" : 1,
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.multisliders /joe -1 1 3 /jim /him 9 3 /silly 2",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontname" : "Arial",
					"patching_rect" : [ 147.0, 193.0, 242.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-5",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.multisliders /horizontal 0 1024 /vertical 0 1280 @setstyle 2",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontname" : "Arial",
					"patching_rect" : [ 147.0, 117.0, 330.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-3",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.multisliders /horizontal 0 1024 /vertical 0 1280",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontname" : "Arial",
					"patching_rect" : [ 152.0, 80.0, 264.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-2",
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.io.mouse",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 158.0, 33.0, 69.0, 20.0 ],
					"numinlets" : 0,
					"id" : "obj-1",
					"fontsize" : 12.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
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
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
