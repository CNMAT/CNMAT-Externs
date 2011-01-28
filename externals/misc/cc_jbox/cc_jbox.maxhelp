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
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 132.0, 96.0, 52.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-5",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "open_editor",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 108.0, 68.0, 74.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-4",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "cfile /Users/john/Development/cnmat/trunk/max/externals/misc/cc_jbox/gui.c",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 64.0, 32.0, 415.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-3",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "cc_jbox",
					"outlets" : 2,
					"inlets" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"load" : "/private/var/tmp/gui.dylib",
					"cfile" : "/Users/john/Development/cnmat/trunk/max/externals/misc/cc_jbox/gui.c",
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numoutlets" : 2,
					"maxsdk" : "/Users/john/Development/cnmat/trunk/max/SDK/MaxSDK-5",
					"patching_rect" : [ 108.0, 124.0, 300.0, 100.0 ],
					"outlettype" : [ "", "" ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"build_path" : "/private/var/tmp",
					"textcolor" : [  ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
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
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
