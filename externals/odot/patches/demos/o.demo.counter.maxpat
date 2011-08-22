{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 25.0, 69.0, 1046.0, 631.0 ],
		"bglocked" : 0,
		"defrect" : [ 25.0, 69.0, 1046.0, 631.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 20.0,
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
					"text" : "/count 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 173.0, 331.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 173.0, 331.0, 200.0, 29.0 ],
					"numinlets" : 2,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-2",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"presentation_rect" : [ 91.0, 396.0, 0.0, 0.0 ],
					"patching_rect" : [ 91.0, 396.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/count 6 \n",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 91.0, 468.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 91.0, 468.0, 211.0, 29.0 ],
					"numinlets" : 2,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-4",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /count++ %= 8",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"presentation_rect" : [ 284.0, 422.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 284.0, 422.0, 201.0, 29.0 ],
					"numinlets" : 1,
					"id" : "obj-5",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/count 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 152.0, 50.0, 200.0, 29.0 ],
					"numinlets" : 2,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-21",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 70.0, 115.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-18"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/count 13 \n",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 70.0, 187.0, 211.0, 29.0 ],
					"numinlets" : 2,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-6",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /count++",
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"fontname" : "Arial",
					"patching_rect" : [ 263.0, 141.0, 149.0, 29.0 ],
					"numinlets" : 1,
					"id" : "obj-1",
					"fontsize" : 20.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 79.5, 248.0, 481.0, 248.0, 481.0, 116.0, 272.5, 116.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-4", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [ 100.5, 529.0, 502.0, 529.0, 502.0, 397.0, 293.5, 397.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-4", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
