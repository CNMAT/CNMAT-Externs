{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 91.0, 44.0, 830.0, 619.0 ],
		"bglocked" : 0,
		"defrect" : [ 91.0, 44.0, 830.0, 619.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 11.595187,
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
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 413.0, 528.0, 20.0, 20.0 ],
					"id" : "obj-46",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "floating $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 413.0, 551.0, 62.0, 18.0 ],
					"id" : "obj-47",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p esc-fullscreen",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 316.0, 551.0, 90.0, 20.0 ],
					"id" : "obj-71",
					"fontname" : "Arial",
					"numinlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 86.0, 108.0, 235.0, 205.0 ],
						"bglocked" : 0,
						"defrect" : [ 86.0, 108.0, 235.0, 205.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 11.595187,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 15.0, 15.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"metadata" : [  ],
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Escape ASCII Code",
									"numoutlets" : 0,
									"fontsize" : 11.595187,
									"patching_rect" : [ 86.0, 67.0, 114.0, 20.0 ],
									"id" : "obj-7",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"patching_rect" : [ 27.0, 157.25, 25.0, 25.0 ],
									"id" : "obj-2",
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "fullscreen $1",
									"numoutlets" : 1,
									"fontsize" : 11.595187,
									"outlettype" : [ "" ],
									"patching_rect" : [ 27.0, 127.25, 76.0, 18.0 ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "key",
									"numoutlets" : 3,
									"fontsize" : 11.595187,
									"outlettype" : [ "int", "int", "int" ],
									"patching_rect" : [ 27.0, 37.0, 46.0, 20.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"numinlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "select 27",
									"numoutlets" : 2,
									"fontsize" : 11.595187,
									"outlettype" : [ "bang", "" ],
									"patching_rect" : [ 27.0, 67.25, 57.0, 20.0 ],
									"id" : "obj-5",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 27.0, 97.25, 20.0, 20.0 ],
									"id" : "obj-6",
									"numinlets" : 1
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 11.595187
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.window foo @depthbuffer 1",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "bang", "" ],
					"patching_rect" : [ 316.0, 580.0, 163.0, 20.0 ],
					"id" : "obj-72",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1.333",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 639.0, 311.0, 38.0, 18.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "int", "bang" ],
					"patching_rect" : [ 55.0, 529.0, 50.0, 20.0 ],
					"minimum" : 0,
					"id" : "obj-2",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"maximum" : 2,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "change x scale to match aspect ratio. e.g. typical 4:3 video would have an x scale of 1.333",
					"linecount" : 3,
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 634.187866, 260.2771, 174.0, 46.0 ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"numoutlets" : 3,
					"fontsize" : 11.595187,
					"outlettype" : [ "int", "", "" ],
					"items" : [ "argb", ",", "uyvy" ],
					"types" : [  ],
					"patching_rect" : [ 369.0, 147.0, 59.0, 20.0 ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend colormode",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 389.0, 177.0, 108.0, 20.0 ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "int", "bang" ],
					"patching_rect" : [ 751.0, 114.0, 50.0, 20.0 ],
					"id" : "obj-9",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "int", "bang" ],
					"patching_rect" : [ 700.0, 114.0, 50.0, 20.0 ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak dim 320 240",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 649.0, 139.0, 121.0, 20.0 ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"numinlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 650.0, 438.0, 50.0, 20.0 ],
					"id" : "obj-12",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak rotate 0. 0. 0. 1.",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 627.0, 460.0, 112.0, 20.0 ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"numinlets" : 5
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "(under OS X only)",
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 465.0, 452.0, 103.0, 20.0 ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 446.0, 452.0, 20.0, 20.0 ],
					"id" : "obj-15",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "client_storage $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 446.0, 477.0, 97.0, 18.0 ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set blend amount",
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 102.0, 368.0, 97.0, 20.0 ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r movie",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 507.0, 140.0, 49.0, 20.0 ],
					"id" : "obj-18",
					"fontname" : "Arial",
					"numinlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s movie",
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 74.0, 276.0, 48.0, 20.0 ],
					"id" : "obj-19",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "use orthographic projection, useful for 2D transformations which do not take distance from camera into account. ortho 2 is typically preferable to ortho 1 which takes lens angle into account.",
					"linecount" : 5,
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 108.0, 531.0, 206.0, 73.0 ],
					"id" : "obj-20",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1.",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 410.0, 349.0, 18.0, 18.0 ],
					"hidden" : 1,
					"id" : "obj-21",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "tex_scale_y $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 289.0, 274.0, 86.0, 18.0 ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 289.0, 251.0, 50.0, 20.0 ],
					"id" : "obj-23",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "tex_scale_x $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 202.0, 274.0, 86.0, 18.0 ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 202.0, 251.0, 50.0, 20.0 ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "tex_offset_y $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 289.0, 221.0, 87.0, 18.0 ],
					"id" : "obj-26",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 289.0, 199.0, 50.0, 20.0 ],
					"id" : "obj-27",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "tex_offset_x $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 202.0, 222.0, 87.0, 18.0 ],
					"id" : "obj-28",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 202.0, 200.0, 50.0, 20.0 ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 569.0, 204.0, 20.0, 20.0 ],
					"id" : "obj-30",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "auto_rotate $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 569.0, 227.0, 85.0, 18.0 ],
					"id" : "obj-31",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 240.0, 153.0, 20.0, 20.0 ],
					"id" : "obj-32",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "interp $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 240.0, 175.0, 54.0, 18.0 ],
					"id" : "obj-33",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nudge $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 292.0, 175.0, 57.0, 18.0 ],
					"id" : "obj-34",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 292.0, 153.0, 50.0, 20.0 ],
					"id" : "obj-35",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 405.0, 384.0, 20.0, 20.0 ],
					"id" : "obj-37",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blend_enable $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 405.0, 407.0, 94.0, 18.0 ],
					"id" : "obj-38",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 102.0, 384.0, 50.0, 20.0 ],
					"id" : "obj-39",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "reset",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 530.0, 228.0, 36.0, 18.0 ],
					"id" : "obj-40",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ortho $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 55.0, 553.0, 51.0, 18.0 ],
					"id" : "obj-41",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 595.0, 114.0, 50.0, 20.0 ],
					"id" : "obj-42",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "rate $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 595.0, 139.0, 47.0, 18.0 ],
					"id" : "obj-43",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 560.0, 140.0, 34.0, 18.0 ],
					"id" : "obj-44",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.qt.movie 320 240",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "jit_matrix", "" ],
					"patching_rect" : [ 507.0, 175.0, 109.0, 20.0 ],
					"id" : "obj-45",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "autohelp",
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 668.0, 552.0, 55.0, 20.0 ],
					"hidden" : 1,
					"id" : "obj-56",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 253.0, 348.0, 20.0, 20.0 ],
					"id" : "obj-63",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "draw as mesh",
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 274.0, 348.0, 100.0, 20.0 ],
					"id" : "obj-64",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "camera 0 0 4",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 51.0, 479.0, 76.0, 18.0 ],
					"hidden" : 1,
					"id" : "obj-65",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 741.0, 332.0, 50.0, 20.0 ],
					"id" : "obj-66",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 690.0, 332.0, 50.0, 20.0 ],
					"id" : "obj-67",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 639.0, 332.0, 50.0, 20.0 ],
					"id" : "obj-68",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak scale 1. 1. 1.",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 638.0, 359.0, 96.0, 20.0 ],
					"id" : "obj-69",
					"fontname" : "Arial",
					"numinlets" : 4
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.gl.handle foo @depth_enable 1 @radius 2.",
					"linecount" : 3,
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 530.0, 253.0, 90.0, 46.0 ],
					"id" : "obj-70",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "int", "bang" ],
					"patching_rect" : [ 182.0, 153.0, 50.0, 20.0 ],
					"id" : "obj-76",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dim $1 $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 182.0, 175.0, 59.0, 18.0 ],
					"id" : "obj-77",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r plane",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 139.0, 261.0, 46.0, 20.0 ],
					"id" : "obj-78",
					"fontname" : "Arial",
					"numinlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.fpsgui",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 55.333332, 299.0, 80.0, 34.666664 ],
					"id" : "obj-79",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s plane",
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 191.0, 468.0, 46.0, 20.0 ],
					"id" : "obj-80",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 756.433655, 383.0, 50.0, 20.0 ],
					"id" : "obj-81",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 705.433655, 383.0, 50.0, 20.0 ],
					"id" : "obj-82",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 654.433655, 383.0, 50.0, 20.0 ],
					"id" : "obj-83",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "color 1. 1. 1. $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 102.0, 407.0, 88.0, 18.0 ],
					"id" : "obj-84",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 504.0, 384.0, 20.0, 20.0 ],
					"id" : "obj-85",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "lighting_enable $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 504.0, 407.0, 105.0, 18.0 ],
					"id" : "obj-86",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 47.0, 384.0, 20.0, 20.0 ],
					"id" : "obj-88",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "axes $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 47.0, 407.0, 49.0, 18.0 ],
					"id" : "obj-89",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 279.0, 384.0, 20.0, 20.0 ],
					"id" : "obj-90",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 235.0, 384.0, 20.0, 20.0 ],
					"id" : "obj-91",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak poly_mode 0 0",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 191.0, 407.0, 107.0, 20.0 ],
					"id" : "obj-92",
					"fontname" : "Arial",
					"numinlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 406.0, 323.0, 56.0, 20.0 ],
					"hidden" : 1,
					"id" : "obj-93",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "int", "bang" ],
					"patching_rect" : [ 62.0, 198.0, 50.0, 20.0 ],
					"id" : "obj-94",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 303.0, 384.0, 20.0, 20.0 ],
					"id" : "obj-95",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "depth_enable $1",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 303.0, 407.0, 95.0, 18.0 ],
					"id" : "obj-96",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 18.0, 198.0, 20.0, 20.0 ],
					"id" : "obj-97",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "qmetro 20",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 18.0, 224.0, 63.0, 20.0 ],
					"id" : "obj-98",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b erase b b",
					"numoutlets" : 4,
					"fontsize" : 11.595187,
					"outlettype" : [ "bang", "erase", "bang", "bang" ],
					"patching_rect" : [ 18.0, 251.0, 75.0, 20.0 ],
					"id" : "obj-99",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.gl.videoplane foo",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "jit_matrix", "" ],
					"patching_rect" : [ 182.0, 311.0, 108.0, 20.0 ],
					"id" : "obj-100",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.gl.render foo",
					"numoutlets" : 2,
					"fontsize" : 11.595187,
					"outlettype" : [ "bang", "" ],
					"patching_rect" : [ 18.0, 583.0, 85.0, 20.0 ],
					"id" : "obj-101",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak position 0. 0. 0.",
					"numoutlets" : 1,
					"fontsize" : 11.595187,
					"outlettype" : [ "" ],
					"patching_rect" : [ 654.0, 409.120483, 109.0, 20.0 ],
					"id" : "obj-102",
					"fontname" : "Arial",
					"numinlets" : 4
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "rotate about z axis",
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 703.63855, 437.759033, 103.0, 20.0 ],
					"id" : "obj-103",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set colormode to argb or uyvy",
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 331.0, 126.0, 161.0, 20.0 ],
					"id" : "obj-104",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "jit.gl.videoplane",
					"numoutlets" : 0,
					"fontface" : 3,
					"fontsize" : 20.871338,
					"patching_rect" : [ 10.0, 8.0, 485.0, 30.0 ],
					"id" : "obj-73",
					"frgb" : [ 0.27, 0.35, 0.47, 1.0 ],
					"fontname" : "Arial",
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "GL accelerated video plane",
					"numoutlets" : 0,
					"fontsize" : 12.754705,
					"patching_rect" : [ 10.0, 36.0, 485.0, 21.0 ],
					"id" : "obj-74",
					"frgb" : [ 0.27, 0.35, 0.47, 1.0 ],
					"fontname" : "Arial",
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_description",
					"text" : "The jit.gl.videoplane object maps incoming video to a plane in 3D space. This may be used to exploit hardware accelerated rotation, scaling, interpolation, and blending. The jit.gl.videoplane object requires one argument: the name of a drawing context. A named drawing context is a named instance of a jit.window, jit.pwindow, or jit.matrix object that has an instance of the jit.gl.render object associated with it. This value may also be set via the OB3D drawto attribute.",
					"linecount" : 4,
					"numoutlets" : 0,
					"fontsize" : 11.595187,
					"patching_rect" : [ 10.0, 57.0, 688.0, 60.0 ],
					"id" : "obj-75",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textbutton",
					"varname" : "autohelp_top_ref",
					"text" : "open jit.gl.videoplane reference",
					"spacing_y" : 0.0,
					"numoutlets" : 3,
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontface" : 3,
					"fontsize" : 12.754705,
					"fontlink" : 1,
					"outlettype" : [ "int", "", "int" ],
					"textovercolor" : [ 0.4, 0.5, 0.65, 1.0 ],
					"presentation_rect" : [ 0.0, 0.0, 199.370407, 14.666666 ],
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"patching_rect" : [ 300.629578, 22.0, 199.370407, 14.666666 ],
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"id" : "obj-87",
					"fontname" : "Arial",
					"underline" : 1,
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"spacing_x" : 0.0,
					"numinlets" : 1,
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_see_title",
					"text" : "See Also:",
					"numoutlets" : 0,
					"fontface" : 1,
					"fontsize" : 11.595187,
					"patching_rect" : [ 673.0, 556.0, 100.0, 20.0 ],
					"id" : "obj-106",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"varname" : "autohelp_see_menu",
					"numoutlets" : 3,
					"fontsize" : 11.595187,
					"outlettype" : [ "int", "", "" ],
					"items" : [ "(Objects:)", ",", "jit.gl.graph", ",", "jit.gl.gridshape", ",", "jit.gl.handle", ",", "jit.gl.isosurf", ",", "jit.gl.mesh", ",", "jit.gl.model", ",", "jit.gl.nurbs", ",", "jit.gl.plato", ",", "jit.gl.shader", ",", "jit.gl.slab", ",", "jit.gl.texture", ",", "jit.gl.sketch", ",", "jit.gl.render", ",", "jit.gl.text2d", ",", "jit.gl.text3d", ",", "jit.gl.volume", ",", "<separator>", ",", "(Tutorials:)", ",", "jitterchapter44" ],
					"types" : [  ],
					"patching_rect" : [ 673.0, 576.0, 130.0, 20.0 ],
					"id" : "obj-107",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel",
					"numoutlets" : 0,
					"background" : 1,
					"grad1" : [ 0.86, 0.86, 0.75, 1.0 ],
					"patching_rect" : [ 5.0, 5.0, 495.0, 52.0 ],
					"grad2" : [ 0.78, 0.84, 0.86, 0.7 ],
					"id" : "obj-105",
					"mode" : 1,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_see_panel",
					"numoutlets" : 0,
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ],
					"background" : 1,
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ],
					"patching_rect" : [ 668.0, 552.0, 140.0, 50.0 ],
					"id" : "obj-108",
					"border" : 2,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-98", 0 ],
					"destination" : [ "obj-99", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-94", 0 ],
					"destination" : [ "obj-98", 1 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-97", 0 ],
					"destination" : [ "obj-98", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 0,
					"midpoints" : [ 422.5, 575.0, 325.5, 575.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-71", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 0,
					"midpoints" : [ 325.5, 575.0, 325.5, 575.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-99", 0 ],
					"destination" : [ "obj-101", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-99", 1 ],
					"destination" : [ "obj-101", 0 ],
					"hidden" : 0,
					"midpoints" : [ 46.166668, 275.0, 27.5, 275.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-101", 0 ],
					"hidden" : 0,
					"midpoints" : [ 64.5, 572.0, 27.5, 572.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-65", 0 ],
					"destination" : [ "obj-101", 0 ],
					"hidden" : 1,
					"midpoints" : [ 60.5, 504.0, 27.5, 504.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-93", 0 ],
					"destination" : [ "obj-65", 0 ],
					"hidden" : 1
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-99", 2 ],
					"destination" : [ "obj-79", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-88", 0 ],
					"destination" : [ "obj-89", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-99", 3 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 0 ],
					"destination" : [ "obj-77", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 398.5, 304.0, 191.5, 304.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 516.5, 300.0, 191.5, 300.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 191.5, 228.0, 191.5, 228.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-78", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 148.5, 295.0, 191.5, 295.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-70", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 539.5, 304.0, 191.5, 304.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 298.5, 304.0, 191.5, 304.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 211.5, 304.0, 191.5, 304.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 298.5, 243.0, 191.5, 243.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 211.5, 243.0, 191.5, 243.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 301.5, 196.0, 191.5, 196.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [ 249.5, 196.0, 191.5, 196.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-69", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [ 647.5, 393.0, 621.0, 393.0, 621.0, 441.0, 200.5, 441.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [ 312.5, 431.0, 200.5, 431.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-92", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [ 200.5, 430.0, 200.5, 430.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-84", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [ 111.5, 430.0, 200.5, 430.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-86", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [ 513.5, 431.0, 200.5, 431.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-89", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [ 56.5, 430.0, 200.5, 430.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-102", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [ 663.5, 431.120483, 621.0, 431.120483, 621.0, 441.0, 200.5, 441.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [ 414.5, 431.0, 200.5, 431.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [ 455.5, 502.0, 433.0, 502.0, 433.0, 441.0, 200.5, 441.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [ 636.5, 484.0, 600.0, 484.0, 600.0, 441.0, 200.5, 441.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-63", 0 ],
					"destination" : [ "obj-91", 0 ],
					"hidden" : 0,
					"midpoints" : [ 262.5, 373.0, 244.5, 373.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-91", 0 ],
					"destination" : [ "obj-92", 1 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-63", 0 ],
					"destination" : [ "obj-90", 0 ],
					"hidden" : 0,
					"midpoints" : [ 262.5, 373.0, 288.5, 373.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-90", 0 ],
					"destination" : [ "obj-92", 2 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-95", 0 ],
					"destination" : [ "obj-96", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 1 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-37", 0 ],
					"hidden" : 1
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-93", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 1
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-85", 0 ],
					"destination" : [ "obj-86", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [ 398.5, 201.0, 502.0, 201.0, 502.0, 169.0, 516.5, 169.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 0 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [ 569.5, 166.0, 516.5, 166.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [ 604.5, 166.0, 516.5, 166.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [ 658.5, 170.0, 516.5, 170.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-13", 1 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-68", 0 ],
					"destination" : [ "obj-69", 1 ],
					"hidden" : 0,
					"midpoints" : [ 648.5, 354.0, 673.166687, 354.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-11", 1 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-83", 0 ],
					"destination" : [ "obj-102", 1 ],
					"hidden" : 0,
					"midpoints" : [ 663.933655, 405.0, 693.5, 405.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-67", 0 ],
					"destination" : [ "obj-69", 2 ],
					"hidden" : 0,
					"midpoints" : [ 699.5, 354.0, 698.833313, 354.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-82", 0 ],
					"destination" : [ "obj-102", 2 ],
					"hidden" : 0,
					"midpoints" : [ 714.933655, 405.0, 723.5, 405.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-69", 3 ],
					"hidden" : 0,
					"midpoints" : [ 750.5, 354.0, 724.5, 354.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-11", 2 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-81", 0 ],
					"destination" : [ "obj-102", 3 ],
					"hidden" : 0,
					"midpoints" : [ 765.933655, 405.0, 753.5, 405.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-70", 0 ],
					"hidden" : 0,
					"midpoints" : [ 578.5, 250.0, 539.5, 250.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-70", 0 ],
					"hidden" : 0,
					"midpoints" : [ 539.5, 250.0, 539.5, 250.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0
				}

			}
 ]
	}

}
