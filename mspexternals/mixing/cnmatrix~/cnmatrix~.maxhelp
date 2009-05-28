{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 76.0, 52.0, 900.0, 731.0 ],
		"bglocked" : 0,
		"defrect" : [ 76.0, 52.0, 900.0, 731.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "frame does linear interpolation over one signal vector.  use for equal power panning.",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 180.0, 675.0, 496.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-45",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cnmatrix~ 3 5 frame",
					"fontsize" : 12.0,
					"numinlets" : 3,
					"patching_rect" : [ 45.0, 675.0, 117.0, 20.0 ],
					"numoutlets" : 5,
					"color" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal" ],
					"id" : "obj-46",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "fast is no interpolation at all.  audible clicks may result.  use for static routing only.",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 180.0, 645.0, 496.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-43",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cnmatrix~ 3 5 fast",
					"fontsize" : 12.0,
					"numinlets" : 3,
					"patching_rect" : [ 45.0, 645.0, 106.0, 20.0 ],
					"numoutlets" : 5,
					"color" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal" ],
					"id" : "obj-44",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "smooth is default, uses the same formula as slide~.  use this for an audio mixer.",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 180.0, 615.0, 496.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-42",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Available interpolation modes:",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 45.0, 585.0, 175.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-39",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cnmatrix~ 3 5 smooth",
					"fontsize" : 12.0,
					"numinlets" : 3,
					"patching_rect" : [ 45.0, 615.0, 126.0, 20.0 ],
					"numoutlets" : 5,
					"color" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal" ],
					"id" : "obj-37",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "this obj is just like matrix~ except it takes jitter matrices as input to set the gains.  The rows of the matrix represent the outputs, and the columns are the inputs (same as matrixctl).  ",
					"linecount" : 7,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 662.0, 386.0, 175.0, 103.0 ],
					"numoutlets" : 0,
					"id" : "obj-8",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 535.0, 37.0, 37.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-36",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "2 1 1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 533.0, 73.0, 37.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-30",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 536.0, 202.0, 34.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-27",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setcell 2 1 val 1., bang",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 198.0, 163.0, 130.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-5",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setcell 1 2 val 1., bang",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 187.0, 134.0, 130.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-4",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setcell 0 0 val 1., bang",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 170.0, 101.0, 130.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-2",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"patching_rect" : [ 20.0, 230.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "matrixctrl",
					"numinlets" : 1,
					"patching_rect" : [ 417.0, 48.0, 70.0, 134.0 ],
					"numoutlets" : 2,
					"rows" : 5,
					"outlettype" : [ "list", "list" ],
					"id" : "obj-1",
					"columns" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "128",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 320.0, 323.0, 32.5, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-35",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 92.0, 110.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-41",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 41.0, 110.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-40",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak 0 0",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 41.0, 154.0, 50.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-33",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numinlets" : 2,
					"patching_rect" : [ 612.0, 477.0, 15.0, 74.0 ],
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"id" : "obj-38"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "slide 10000",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 285.0, 254.0, 72.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-34",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 354.0, 55.0, 34.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-32",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "**********",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 354.0, 32.0, 57.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-31",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "meter~",
					"numinlets" : 1,
					"patching_rect" : [ 573.0, 446.0, 12.0, 78.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"id" : "obj-26"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "meter~",
					"numinlets" : 1,
					"patching_rect" : [ 556.0, 446.0, 12.0, 78.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"id" : "obj-25"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "meter~",
					"numinlets" : 1,
					"patching_rect" : [ 539.0, 446.0, 12.0, 78.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"id" : "obj-24"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "meter~",
					"numinlets" : 1,
					"patching_rect" : [ 522.0, 446.0, 12.0, 78.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"id" : "obj-23"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "meter~",
					"numinlets" : 1,
					"patching_rect" : [ 505.0, 446.0, 12.0, 78.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"id" : "obj-20"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setcell $1 $2 val 1., bang",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 41.0, 205.0, 143.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-29",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setall 0., bang",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 144.0, 244.0, 85.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-28",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.cellblock",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 41.0, 350.0, 200.0, 200.0 ],
					"numoutlets" : 4,
					"rows" : 5,
					"outlettype" : [ "list", "", "", "" ],
					"cols" : 3,
					"id" : "obj-22",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.matrix 1 float32 3 5",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 41.0, 273.0, 125.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "jit_matrix", "" ],
					"id" : "obj-21",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"patching_rect" : [ 295.0, 408.0, 40.0, 40.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dac~ 1 2 3 4 5",
					"fontsize" : 12.0,
					"numinlets" : 5,
					"patching_rect" : [ 365.0, 487.0, 95.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-7",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cycle~ 240",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 431.0, 222.0, 75.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-10",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cycle~ 640",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 496.0, 268.0, 75.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-11",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numinlets" : 2,
					"patching_rect" : [ 476.0, 358.0, 15.0, 74.0 ],
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"id" : "obj-12"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numinlets" : 2,
					"patching_rect" : [ 457.0, 358.0, 15.0, 74.0 ],
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"id" : "obj-13"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numinlets" : 2,
					"patching_rect" : [ 438.0, 358.0, 15.0, 74.0 ],
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"id" : "obj-14"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numinlets" : 2,
					"patching_rect" : [ 419.0, 358.0, 15.0, 74.0 ],
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"id" : "obj-15"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numinlets" : 2,
					"patching_rect" : [ 397.0, 358.0, 15.0, 74.0 ],
					"numoutlets" : 2,
					"orientation" : 2,
					"outlettype" : [ "signal", "int" ],
					"id" : "obj-16"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cycle~ 440",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 463.0, 245.0, 75.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-17",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cnmatrix~ 3 5",
					"fontsize" : 12.0,
					"numinlets" : 3,
					"patching_rect" : [ 397.0, 306.0, 83.0, 20.0 ],
					"numoutlets" : 5,
					"color" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal" ],
					"id" : "obj-18",
					"fontname" : "MS Reference Sans Serif"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "What this test example does is adding the inlet signals and outputing the result to each outlet",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 573.0, 305.0, 175.0, 62.0 ],
					"numoutlets" : 0,
					"id" : "obj-19",
					"fontname" : "MS Reference Sans Serif"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-18", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 1 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 2 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 3 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-18", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 4 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-33", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-7", 4 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-7", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-7", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-7", 0 ],
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
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
