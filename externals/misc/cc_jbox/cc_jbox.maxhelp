{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 296.0, 209.0, 640.0, 480.0 ],
		"bglocked" : 0,
		"defrect" : [ 296.0, 209.0, 640.0, 480.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 4.0, 4.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "compile",
					"presentation_rect" : [ 131.0, 94.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 132.0, 96.0, 52.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "open_editor",
					"presentation_rect" : [ 108.0, 69.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 108.0, 68.0, 74.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "cfile /Users/johnmac/Development/cnmat/trunk/max/externals/misc/cc_jbox/gui.c",
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 64.0, 32.0, 437.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "cc_jbox",
					"outlets" : 2,
					"load" : "/private/var/tmp/gui.dylib",
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"inlets" : 2,
					"patching_rect" : [ 108.0, 124.0, 300.0, 100.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [  ],
					"cfile" : "/Users/johnmac/Development/cnmat/trunk/max/externals/misc/cc_jbox/gui.c",
					"id" : "obj-1",
					"fontname" : "Arial",
					"maxsdk" : "/Users/johnmac/Development/cnmat/trunk/max/SDK/MaxSDK-5",
					"build_path" : "/private/var/tmp",
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
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
 ]
	}

}
