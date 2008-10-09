{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 106.0, 44.0, 1119.0, 744.0 ],
		"bglocked" : 0,
		"defrect" : [ 106.0, 44.0, 1119.0, 744.0 ],
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
		"metadata" : [  ],
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "flonum",
					"patching_rect" : [ 835.0, 34.0, 84.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-39",
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"outlettype" : [ "float", "bang" ],
					"presentation_rect" : [ 835.0, 34.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 792.0, 45.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-40",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"presentation_rect" : [ 792.0, 45.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "uzi 1000",
					"patching_rect" : [ 792.0, 76.0, 56.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"id" : "obj-41",
					"numoutlets" : 3,
					"fontsize" : 12.0,
					"outlettype" : [ "bang", "bang", "int" ],
					"presentation_rect" : [ 792.0, 76.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "list-accum 10000",
					"patching_rect" : [ 792.0, 253.0, 101.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-42",
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"outlettype" : [ "", "int" ],
					"presentation_rect" : [ 792.0, 253.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "randdist uniform 0.5 35",
					"patching_rect" : [ 822.0, 131.0, 133.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-43",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 822.0, 131.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "randdist uniform 0.1 0.9",
					"patching_rect" : [ 807.0, 157.0, 137.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-44",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 807.0, 157.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "randdist uniform 100 15000",
					"patching_rect" : [ 792.0, 186.0, 157.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-45",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 792.0, 186.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pack 0. 0. 0.",
					"patching_rect" : [ 792.0, 223.0, 76.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 3,
					"id" : "obj-46",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 792.0, 223.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "b 3",
					"patching_rect" : [ 792.0, 106.0, 46.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-47",
					"numoutlets" : 3,
					"fontsize" : 12.0,
					"outlettype" : [ "bang", "bang", "bang" ],
					"presentation_rect" : [ 792.0, 106.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "range 0 $1",
					"patching_rect" : [ 439.0, 357.0, 68.0, 18.0 ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"id" : "obj-38",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"patching_rect" : [ 439.0, 311.0, 128.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-37",
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"range" : [ 0.0, 100000000.0 ],
					"patching_rect" : [ 298.0, 437.0, 130.0, 130.0 ],
					"numinlets" : 2,
					"id" : "obj-35",
					"numoutlets" : 0,
					"presentation_rect" : [ 346.0, 407.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"range" : [ 0.0, 100000000.0 ],
					"patching_rect" : [ 157.0, 436.0, 130.0, 130.0 ],
					"numinlets" : 2,
					"id" : "obj-34",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "noise~",
					"patching_rect" : [ 283.0, 32.0, 46.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-16",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "signal" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"patching_rect" : [ 283.0, 195.0, 190.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"id" : "obj-17",
					"mode" : 2,
					"numoutlets" : 2,
					"sig" : 0.0,
					"fontsize" : 12.0,
					"outlettype" : [ "signal", "float" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "resonators~",
					"patching_rect" : [ 253.0, 102.0, 74.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-18",
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"outlettype" : [ "signal", "list" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "bench~ out thru 2",
					"patching_rect" : [ 283.0, 144.0, 104.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"id" : "obj-19",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "signal" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "bench~ in thru 2",
					"patching_rect" : [ 283.0, 63.0, 97.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-20",
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"outlettype" : [ "signal", "signal" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "noise~",
					"patching_rect" : [ 86.0, 32.0, 46.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-13",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "signal" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"patching_rect" : [ 622.0, 33.0, 84.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-14",
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"patching_rect" : [ 653.0, 407.0, 45.0, 45.0 ],
					"numinlets" : 2,
					"id" : "obj-11",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"patching_rect" : [ 86.0, 195.0, 190.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"id" : "obj-3",
					"mode" : 2,
					"numoutlets" : 2,
					"sig" : 0.0,
					"fontsize" : 12.0,
					"outlettype" : [ "signal", "float" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 579.0, 44.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"id" : "obj-12",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "uzi 1000",
					"patching_rect" : [ 579.0, 75.0, 56.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"id" : "obj-10",
					"numoutlets" : 3,
					"fontsize" : 12.0,
					"outlettype" : [ "bang", "bang", "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "list-accum 10000",
					"patching_rect" : [ 579.0, 252.0, 101.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-9",
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"outlettype" : [ "", "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "randdist uniform 0.5 35",
					"patching_rect" : [ 609.0, 130.0, 133.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-8",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "randdist uniform 0.1 0.9",
					"patching_rect" : [ 594.0, 156.0, 137.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-1",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "randdist uniform 100 15000",
					"patching_rect" : [ 579.0, 185.0, 157.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-2",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pack 0. 0. 0.",
					"patching_rect" : [ 579.0, 222.0, 76.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 3,
					"id" : "obj-5",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "b 3",
					"patching_rect" : [ 579.0, 105.0, 46.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-4",
					"numoutlets" : 3,
					"fontsize" : 12.0,
					"outlettype" : [ "bang", "bang", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "resonators~",
					"patching_rect" : [ 56.0, 102.0, 74.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-7",
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"outlettype" : [ "signal", "list" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "bench~ out thru 1",
					"patching_rect" : [ 86.0, 144.0, 104.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 2,
					"id" : "obj-6",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "signal" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "bench~ in thru 1",
					"patching_rect" : [ 86.0, 63.0, 97.0, 20.0 ],
					"fontname" : "Arial",
					"numinlets" : 1,
					"id" : "obj-15",
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"outlettype" : [ "signal", "signal" ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-41", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 1 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-46", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 0 ],
					"destination" : [ "obj-46", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-46", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 2 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 1 ],
					"destination" : [ "obj-44", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-45", 0 ],
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
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 1 ],
					"destination" : [ "obj-19", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-17", 0 ],
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
					"source" : [ "obj-4", 1 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 2 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-5", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-5", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 1 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-10", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 1 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
