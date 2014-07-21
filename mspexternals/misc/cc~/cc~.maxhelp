{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 233.0, 44.0, 1024.0, 629.0 ],
		"bglocked" : 0,
		"defrect" : [ 233.0, 44.0, 1024.0, 629.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "number~",
					"outlettype" : [ "signal", "float" ],
					"presentation_rect" : [ 512.0, 26.0, 0.0, 0.0 ],
					"mode" : 1,
					"fontname" : "Arial",
					"sig" : 2.89,
					"patching_rect" : [ 512.0, 28.0, 56.0, 20.0 ],
					"numinlets" : 2,
					"id" : "obj-9",
					"numoutlets" : 2,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"outlettype" : [ "signal", "float" ],
					"presentation_rect" : [ 196.0, 27.0, 0.0, 0.0 ],
					"mode" : 1,
					"fontname" : "Arial",
					"sig" : 3.79,
					"patching_rect" : [ 196.0, 28.0, 56.0, 20.0 ],
					"numinlets" : 2,
					"id" : "obj-8",
					"numoutlets" : 2,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"outlettype" : [ "signal", "float" ],
					"mode" : 2,
					"fontname" : "Arial",
					"sig" : 0.0,
					"patching_rect" : [ 196.0, 152.0, 56.0, 20.0 ],
					"numinlets" : 2,
					"id" : "obj-6",
					"numoutlets" : 2,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"patching_rect" : [ 832.0, 240.0, 45.0, 45.0 ],
					"numinlets" : 2,
					"id" : "obj-5",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "maxsdk /Users/johnmac/Development/cnmat/trunk/max/SDK/MaxSDK-5",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 380.0, 68.0, 394.0, 18.0 ],
					"numinlets" : 2,
					"id" : "obj-4",
					"numoutlets" : 1,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "verbose 1",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 76.0, 36.0, 63.0, 18.0 ],
					"numinlets" : 2,
					"id" : "obj-2",
					"numoutlets" : 1,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "compile_and_load",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 196.0, 68.0, 108.0, 18.0 ],
					"numinlets" : 2,
					"id" : "obj-3",
					"numoutlets" : 1,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cc~ @siginlets 2 @sigoutlets 2 @outlets 0 @maxsdk /Users/johnmac/Development/cnmat/trunk/max/SDK/MaxSDK-5",
					"linecount" : 2,
					"outlettype" : [ "signal", "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 196.0, 100.0, 349.0, 34.0 ],
					"numinlets" : 2,
					"id" : "obj-1",
					"numoutlets" : 2,
					"fontsize" : 12.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-1", 1 ],
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
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-6", 0 ],
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
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
