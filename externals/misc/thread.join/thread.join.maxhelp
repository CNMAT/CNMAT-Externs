{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 25.0, 69.0, 961.0, 569.0 ],
		"bglocked" : 0,
		"defrect" : [ 25.0, 69.0, 961.0, 569.0 ],
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
					"maxclass" : "comment",
					"text" : "written by Andy Schmeder 2009\n<andy@cnmat.berkeley.edu>",
					"linecount" : 2,
					"presentation_rect" : [ 632.0, 447.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 650.0, 480.0, 220.0, 34.0 ],
					"numoutlets" : 0,
					"id" : "obj-15",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<--- the source of these events is the main thread",
					"linecount" : 2,
					"presentation_rect" : [ 279.0, 129.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 250.0, 130.0, 214.0, 34.0 ],
					"numoutlets" : 0,
					"id" : "obj-14",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<--- at this point execution is back in the \"main\" thread",
					"linecount" : 2,
					"presentation_rect" : [ 364.0, 467.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 330.0, 470.0, 214.0, 34.0 ],
					"numoutlets" : 0,
					"id" : "obj-13",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "thread.join\n\nany event received by thread.join is passed back into the main thread",
					"linecount" : 4,
					"presentation_rect" : [ 523.0, 345.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 490.0, 350.0, 214.0, 62.0 ],
					"numoutlets" : 0,
					"id" : "obj-12",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<--- all event domain processing that happens in here executes in the \"other\" thread",
					"linecount" : 3,
					"presentation_rect" : [ 318.0, 279.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 230.0, 290.0, 214.0, 48.0 ],
					"numoutlets" : 0,
					"id" : "obj-11",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "thread.fork\n\ncreates a thread; events received on its inlet are moved to the other thread for output",
					"linecount" : 5,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 490.0, 210.0, 214.0, 75.0 ],
					"numoutlets" : 0,
					"id" : "obj-10",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "thread.fork / thread.join\n\nthread-level parallelism for event domain computation\n\nWARNING: this object will crash MaxMSP if used improperly or with objects that are not \"overdrive safe\"",
					"linecount" : 9,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 490.0, 40.0, 156.0, 131.0 ],
					"numoutlets" : 0,
					"id" : "obj-9",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print in-main-thread",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 190.0, 470.0, 115.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-8",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "thread.join",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 150.0, 400.0, 67.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-7",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 110.0, 40.0, 79.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-6",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 60.0, 100.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-5",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "aoeu 1212013 a b c 1 2 3 4 5 6 7 5 54 4 3",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 210.0, 100.0, 233.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-4",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print in-other-thread",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 190.0, 250.0, 116.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-3",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"patching_rect" : [ 150.0, 80.0, 34.0, 34.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-2"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "thread.fork",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 150.0, 190.0, 68.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-1",
					"fontname" : "Arial"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-3", 0 ],
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
					"source" : [ "obj-2", 0 ],
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
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
