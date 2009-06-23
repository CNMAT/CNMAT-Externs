{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 0.0, 113.0, 1245.0, 759.0 ],
		"bglocked" : 0,
		"defrect" : [ 0.0, 113.0, 1245.0, 759.0 ],
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
					"maxclass" : "newobj",
					"text" : "/~ 10.",
					"id" : "obj-72",
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1120.0, 550.0, 41.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/~ 100.",
					"id" : "obj-71",
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1060.0, 550.0, 48.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "open",
					"id" : "obj-70",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1100.0, 590.0, 37.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"presentation_rect" : [ 1099.0, 591.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"id" : "obj-69",
					"outlettype" : [ "int" ],
					"patching_rect" : [ 1070.0, 590.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"presentation_rect" : [ 1068.0, 585.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sfrecord~ 3",
					"id" : "obj-68",
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1070.0, 630.0, 71.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 3,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-67",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 610.0, 230.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak 0. 0.",
					"id" : "obj-66",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 550.0, 260.0, 57.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-62",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 550.0, 230.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"id" : "obj-65",
					"outlettype" : [ "int" ],
					"patching_rect" : [ 980.0, 480.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"id" : "obj-64",
					"outlettype" : [ "int" ],
					"patching_rect" : [ 980.0, 430.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump_beats",
					"id" : "obj-61",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 480.0, 60.0, 77.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"id" : "obj-60",
					"fontname" : "Arial",
					"patching_rect" : [ 650.0, 90.0, 34.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "show_beat_correction $1",
					"id" : "obj-59",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 980.0, 510.0, 145.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "time_minmax 0 10",
					"id" : "obj-58",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 550.0, 330.0, 109.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "time_minmax $1 $2",
					"id" : "obj-57",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 550.0, 300.0, 115.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "show_tempo_correction $1",
					"id" : "obj-55",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 980.0, 460.0, 155.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"id" : "obj-56",
					"outlettype" : [ "int", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 590.0, 160.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-53",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 720.0, 490.0, 98.0, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-54",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 460.0, 490.0, 98.0, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-51",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 860.0, 610.0, 98.0, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-52",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 600.0, 610.0, 98.0, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-50",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 900.0, 570.0, 98.0, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-39",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 640.0, 570.0, 98.0, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"id" : "obj-26",
					"patching_rect" : [ 320.0, 620.0, 130.0, 130.0 ],
					"numinlets" : 2,
					"numoutlets" : 0,
					"range" : [ 0.0, 10.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"id" : "obj-27",
					"patching_rect" : [ 180.0, 620.0, 130.0, 130.0 ],
					"numinlets" : 2,
					"numoutlets" : 0,
					"range" : [ 0.0, 1000.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"id" : "obj-28",
					"patching_rect" : [ 40.0, 620.0, 130.0, 130.0 ],
					"numinlets" : 2,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "te_breakout~ @name foo @function 1",
					"id" : "obj-38",
					"outlettype" : [ "signal", "signal", "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 750.0, 520.0, 214.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "resonators~ 660. 1. 15. 900. 0.5 25.",
					"id" : "obj-40",
					"outlettype" : [ "signal", "list" ],
					"fontname" : "Arial",
					"patching_rect" : [ 750.0, 650.0, 201.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 100.",
					"id" : "obj-41",
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 750.0, 620.0, 49.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "<~",
					"id" : "obj-47",
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 750.0, 590.0, 32.5, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "delay~ 1 1",
					"id" : "obj-49",
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 760.0, 550.0, 66.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "te_breakout~ @name foo @function 0",
					"id" : "obj-37",
					"outlettype" : [ "signal", "signal", "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 530.0, 520.0, 214.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setFunction $1",
					"id" : "obj-36",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 590.0, 190.0, 89.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "addFunction",
					"id" : "obj-35",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 400.0, 10.0, 77.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-63",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 20.0, 140.0, 53.300972, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-45",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 850.0, 10.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "freq_minmax 0. $1",
					"id" : "obj-48",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 850.0, 40.0, 110.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-46",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 730.0, 10.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"id" : "obj-44",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 430.0, 60.0, 41.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"id" : "obj-43",
					"fontname" : "Arial",
					"patching_rect" : [ 600.0, 90.0, 34.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"id" : "obj-42",
					"outlettype" : [ "int", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 340.0, 30.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "mode $1",
					"id" : "obj-34",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 340.0, 60.0, 57.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route cellblock dump",
					"id" : "obj-33",
					"outlettype" : [ "", "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 590.0, 40.0, 121.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textedit",
					"text" : "0.25",
					"id" : "obj-22",
					"outlettype" : [ "", "int", "", "" ],
					"clickmode" : 1,
					"fontname" : "Geneva",
					"lines" : 1,
					"keymode" : 1,
					"patching_rect" : [ 710.0, 90.0, 198.0, 20.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"wordwrap" : 0,
					"fontsize" : 9.998779,
					"numinlets" : 1,
					"numoutlets" : 4
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t select l",
					"id" : "obj-24",
					"outlettype" : [ "select", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 860.0, 400.0, 53.0, 20.0 ],
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.cellblock",
					"id" : "obj-14",
					"outlettype" : [ "list", "", "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 710.0, 110.0, 470.0, 282.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 4
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0. 1. 0., 11.33333 3. 0.",
					"id" : "obj-31",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 180.0, 20.0, 130.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-32",
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 650.0, 420.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "editSelection 10. $1 0.",
					"id" : "obj-30",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 650.0, 460.0, 129.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear",
					"id" : "obj-29",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 190.0, 60.0, 37.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "time_minmax 0. $1",
					"id" : "obj-25",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 730.0, 40.0, 112.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "tempo_editor",
					"id" : "obj-1",
					"outlettype" : [ "signal", "signal", "signal", "" ],
					"show_y_axis" : 1,
					"name" : "foo",
					"fontname" : "Arial",
					"patching_rect" : [ 80.0, 110.0, 458.0, 249.0 ],
					"mode" : 0,
					"textcolor" : [  ],
					"show_beat_correction" : 0,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"show_x_axis" : 1,
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numoutlets" : 4,
					"show_tempo_correction" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "*******************************************************",
					"id" : "obj-23",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 800.0, 700.0, 267.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"id" : "obj-21",
					"fontname" : "Arial",
					"patching_rect" : [ 800.0, 730.0, 34.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-20",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 530.0, 420.0, 98.0, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 60.",
					"id" : "obj-19",
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 530.0, 390.0, 42.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "resonators~ 440. 1. 15. 800. 0.5 25.",
					"id" : "obj-18",
					"outlettype" : [ "signal", "list" ],
					"fontname" : "Arial",
					"patching_rect" : [ 530.0, 650.0, 201.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 100.",
					"id" : "obj-17",
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 530.0, 620.0, 49.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "<~",
					"id" : "obj-16",
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 530.0, 590.0, 32.5, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "delay~ 1 1",
					"id" : "obj-15",
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 540.0, 550.0, 66.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"id" : "obj-9",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 40.0, 20.0, 32.5, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-13",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 390.0, 410.0, 98.0, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-12",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 240.0, 410.0, 98.0, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"id" : "obj-11",
					"outlettype" : [ "signal", "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 80.0, 410.0, 98.0, 20.0 ],
					"mode" : 2,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2,
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"id" : "obj-10",
					"patching_rect" : [ 320.0, 450.0, 130.0, 130.0 ],
					"numinlets" : 2,
					"numoutlets" : 0,
					"range" : [ 0.0, 10.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"id" : "obj-8",
					"patching_rect" : [ 180.0, 450.0, 130.0, 130.0 ],
					"numinlets" : 2,
					"numoutlets" : 0,
					"range" : [ 0.0, 1000.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"id" : "obj-6",
					"patching_rect" : [ 40.0, 450.0, 130.0, 130.0 ],
					"numinlets" : 2,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0., 10 10000.",
					"id" : "obj-7",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 80.0, 20.0, 81.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"id" : "obj-5",
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 80.0, 360.0, 458.0, 14.0 ],
					"setminmax" : [ 0.0, 10.0 ],
					"orientation" : 0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "snapshot~ 1",
					"id" : "obj-4",
					"outlettype" : [ "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 100.0, 80.0, 76.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "line~ 0.",
					"id" : "obj-3",
					"outlettype" : [ "signal", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 80.0, 50.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"id" : "obj-2",
					"patching_rect" : [ 530.0, 680.0, 45.0, 45.0 ],
					"numinlets" : 2,
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-72", 0 ],
					"destination" : [ "obj-68", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-71", 0 ],
					"destination" : [ "obj-68", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 2 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 1 ],
					"destination" : [ "obj-71", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-69", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-70", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-56", 0 ],
					"destination" : [ "obj-36", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-2", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-47", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-63", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
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
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-16", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-2", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 869.5, 426.0, 1185.0, 426.0, 1185.0, 76.0, 719.5, 76.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 1 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 903.5, 423.0, 1191.0, 423.0, 1191.0, 81.0, 719.5, 81.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 1 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 1 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 1 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 2 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-49", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 2 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 2 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 1 ],
					"destination" : [ "obj-52", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 1 ],
					"destination" : [ "obj-51", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-53", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-54", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-1", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
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
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 2 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 2 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 2 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-1", 0 ],
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
					"source" : [ "obj-1", 3 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
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
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-58", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 2 ],
					"destination" : [ "obj-60", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-61", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-64", 0 ],
					"destination" : [ "obj-55", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-65", 0 ],
					"destination" : [ "obj-59", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 0 ],
					"destination" : [ "obj-66", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-67", 0 ],
					"destination" : [ "obj-66", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
