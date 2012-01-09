{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 377.0, 87.0, 1024.0, 667.0 ],
		"bglocked" : 0,
		"defrect" : [ 377.0, 87.0, 1024.0, 667.0 ],
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
					"text" : "/right/z 0.00537241 \n/right/y -0.497436 \n/right/x 0.637118 \n/gametrak/footswitch 0 \n/right/z 0.00537241 \n/right/y -0.497436 \n/right/x 0.637118 \n/left/z 0.003663 \n/left/y 0.0002442 \n/left/x 0.982418 \n",
					"linecount" : 10,
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 550.0, 427.0, 184.0, 144.0 ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-3",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.io.gametrak.display.maxhelp",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 14.0, 7.0, 214.0, 20.0 ],
					"id" : "obj-38",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gswitch",
					"numinlets" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 58.0, 59.0, 41.0, 32.0 ],
					"id" : "obj-5"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"args" : [  ],
					"patching_rect" : [ 57.0, 411.0, 324.0, 298.0 ],
					"id" : "obj-4",
					"name" : "o.io.gametrak.display.maxpat"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.io.gametrak",
					"numinlets" : 0,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 65.0, 29.0, 83.0, 20.0 ],
					"id" : "obj-2",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"args" : [  ],
					"patching_rect" : [ 55.0, 99.0, 324.0, 298.0 ],
					"id" : "obj-1",
					"name" : "o.io.gametrak.display.maxpat"
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
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-3", 1 ],
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
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-5", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
