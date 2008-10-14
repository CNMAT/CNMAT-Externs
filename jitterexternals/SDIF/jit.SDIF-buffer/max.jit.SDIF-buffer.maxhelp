{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 386.0, 44.0, 879.0, 746.0 ],
		"bglocked" : 0,
		"defrect" : [ 386.0, 44.0, 879.0, 746.0 ],
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
					"maxclass" : "jit.cellblock",
					"id" : "obj-20",
					"numinlets" : 2,
					"numoutlets" : 4,
					"fontname" : "Arial",
					"patching_rect" : [ 33.0, 289.0, 422.0, 261.0 ],
					"outlettype" : [ "list", "", "", "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "131 5",
					"id" : "obj-21",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 434.0, 234.0, 61.0, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "float64",
					"id" : "obj-22",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 365.0, 234.0, 60.0, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"id" : "obj-18",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 293.0, 234.0, 65.0, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route planecount type dim",
					"id" : "obj-19",
					"numinlets" : 1,
					"numoutlets" : 4,
					"fontname" : "Arial",
					"patching_rect" : [ 293.0, 203.0, 227.0, 20.0 ],
					"outlettype" : [ "", "", "", "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.matrixinfo",
					"id" : "obj-14",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 293.0, 169.0, 75.0, 20.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-17",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 49.0, 83.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"id" : "obj-16",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 205.0, 136.0, 34.0, 20.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "tuples time 0.3 columns 1 2 3",
					"id" : "obj-15",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 205.0, 39.0, 167.0, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SDIF-tuples foo",
					"id" : "obj-12",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 205.0, 79.0, 94.0, 20.0 ],
					"outlettype" : [ "list", "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"id" : "obj-13",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 600.0, 549.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Total number of frames",
					"linecount" : 2,
					"id" : "obj-9",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 744.0, 677.0, 63.0, 27.0 ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Minimum and maximum frame times (sec)",
					"linecount" : 2,
					"id" : "obj-10",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 632.0, 677.0, 110.0, 27.0 ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Frame Type",
					"id" : "obj-3",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 566.0, 677.0, 61.0, 17.0 ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "stream ID",
					"id" : "obj-11",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 509.0, 677.0, 52.0, 17.0 ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Name of most recently loaded file",
					"linecount" : 2,
					"id" : "obj-5",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 380.0, 677.0, 95.0, 27.0 ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Name of SDIF-buffer",
					"linecount" : 2,
					"id" : "obj-6",
					"numinlets" : 1,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 295.0, 677.0, 71.0, 27.0 ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"id" : "obj-23",
					"numinlets" : 1,
					"triscale" : 0.9,
					"numoutlets" : 2,
					"fontname" : "Arial",
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"patching_rect" : [ 745.0, 659.0, 42.0, 17.0 ],
					"outlettype" : [ "int", "bang" ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-24",
					"numinlets" : 1,
					"triscale" : 0.9,
					"numoutlets" : 2,
					"fontname" : "Arial",
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"patching_rect" : [ 687.0, 659.0, 48.0, 17.0 ],
					"outlettype" : [ "float", "bang" ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-25",
					"numinlets" : 1,
					"triscale" : 0.9,
					"numoutlets" : 2,
					"fontname" : "Arial",
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"patching_rect" : [ 636.0, 659.0, 48.0, 17.0 ],
					"outlettype" : [ "float", "bang" ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1TRC",
					"id" : "obj-26",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 565.0, 659.0, 34.0, 15.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend set",
					"id" : "obj-27",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 544.0, 636.0, 60.0, 17.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"id" : "obj-28",
					"numinlets" : 1,
					"triscale" : 0.9,
					"numoutlets" : 2,
					"fontname" : "Arial",
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"patching_rect" : [ 510.0, 659.0, 41.0, 17.0 ],
					"outlettype" : [ "int", "bang" ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "bone.sdif",
					"id" : "obj-29",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 377.0, 659.0, 129.0, 15.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend set",
					"id" : "obj-30",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 377.0, 636.0, 60.0, 17.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "foo",
					"id" : "obj-31",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 296.0, 659.0, 75.0, 15.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend set",
					"id" : "obj-32",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 296.0, 636.0, 60.0, 17.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "OSC-route /name /filename /streamID /frameType /minTime /maxTime /numFrames",
					"id" : "obj-33",
					"numinlets" : 1,
					"numoutlets" : 8,
					"fontname" : "Arial",
					"patching_rect" : [ 296.0, 611.0, 505.0, 17.0 ],
					"outlettype" : [ "list", "list", "list", "list", "list", "list", "list", "list" ],
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SDIF-info foo",
					"id" : "obj-8",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 601.0, 576.0, 81.0, 20.0 ],
					"outlettype" : [ "list" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"id" : "obj-7",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 395.0, 26.0, 60.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read-stream-number bone.sdif 0",
					"id" : "obj-4",
					"numinlets" : 2,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 395.0, 63.0, 182.0, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SDIF-buffer foo",
					"id" : "obj-2",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 395.0, 115.0, 92.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.SDIF-buffer foo",
					"id" : "obj-1",
					"numinlets" : 1,
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 40.0, 127.0, 104.0, 20.0 ],
					"outlettype" : [ "jit_matrix", "" ],
					"fontsize" : 12.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-12", 0 ],
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
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 6 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 5 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 4 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 3 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 2 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 1 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 2 ],
					"destination" : [ "obj-21", 1 ],
					"hidden" : 0,
					"midpoints" : [ 441.166656, 231.0, 485.5, 231.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 1 ],
					"destination" : [ "obj-22", 1 ],
					"hidden" : 0,
					"midpoints" : [ 371.833344, 228.0, 415.5, 228.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-18", 1 ],
					"hidden" : 0,
					"midpoints" : [ 302.5, 228.0, 348.5, 228.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
