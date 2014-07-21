{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 44.0, 44.0, 671.0, 876.0 ],
		"bglocked" : 0,
		"defrect" : [ 44.0, 44.0, 671.0, 876.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-12",
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"presentation_rect" : [ 297.0, 691.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 300.0, 690.0, 50.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "filter.rate",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-13",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"presentation_rect" : [ 297.0, 661.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 300.0, 660.0, 57.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "realtime.sample~ 1 @rate 700.",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-14",
					"numoutlets" : 2,
					"outlettype" : [ "list", "OSCTimeTag" ],
					"fontname" : "Arial",
					"patching_rect" : [ 300.0, 630.0, 175.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-8",
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 90.0, 690.0, 50.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "filter.rate",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-7",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"fontname" : "Arial",
					"patching_rect" : [ 90.0, 660.0, 57.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "realtime.sample~ 1 @rate 600.",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-3",
					"numoutlets" : 2,
					"outlettype" : [ "list", "OSCTimeTag" ],
					"fontname" : "Arial",
					"patching_rect" : [ 90.0, 630.0, 175.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "many signals in, \"what\" is a list...",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-5",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 240.0, 540.0, 225.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "realtime.sample~ 6",
					"numinlets" : 6,
					"fontsize" : 12.0,
					"id" : "obj-4",
					"numoutlets" : 2,
					"outlettype" : [ "list", "OSCTimeTag" ],
					"fontname" : "Arial",
					"patching_rect" : [ 90.0, 540.0, 112.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"id" : "obj-2",
					"numoutlets" : 0,
					"patching_rect" : [ 350.0, 100.0, 45.0, 45.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "realtime.onehz~",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-1",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 90.0, 130.0, 96.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print when",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-11",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 190.0, 390.0, 66.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print what",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-9",
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 90.0, 390.0, 63.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "realtime.sample~",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"id" : "obj-6",
					"numoutlets" : 2,
					"outlettype" : [ "list", "OSCTimeTag" ],
					"fontname" : "Arial",
					"patching_rect" : [ 90.0, 330.0, 102.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"numinlets" : 2,
					"id" : "obj-10",
					"numoutlets" : 0,
					"patching_rect" : [ 300.0, 260.0, 212.0, 140.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-7", 0 ],
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
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 1 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
