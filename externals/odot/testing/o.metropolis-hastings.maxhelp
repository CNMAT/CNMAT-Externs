{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 0.0, 44.0, 1328.0, 910.0 ],
		"bglocked" : 0,
		"defrect" : [ 0.0, 44.0, 1328.0, 910.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 8.0, 8.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b f",
					"numinlets" : 1,
					"id" : "obj-53",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 488.0, 136.0, 32.5, 20.0 ],
					"outlettype" : [ "bang", "float" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p draw_interp",
					"numinlets" : 2,
					"id" : "obj-52",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 632.0, 416.0, 83.0, 20.0 ],
					"outlettype" : [ "float", "clear" ],
					"fontsize" : 12.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 640.0, 480.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 8.0, 8.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"id" : "obj-25",
									"numoutlets" : 2,
									"fontname" : "Arial",
									"patching_rect" : [ 458.0, 484.0, 50.0, 20.0 ],
									"outlettype" : [ "float", "bang" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t f b clear",
									"numinlets" : 1,
									"id" : "obj-34",
									"numoutlets" : 3,
									"fontname" : "Arial",
									"patching_rect" : [ 50.0, 156.0, 60.0, 20.0 ],
									"outlettype" : [ "float", "bang", "clear" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"id" : "obj-36",
									"numoutlets" : 1,
									"patching_rect" : [ 50.0, 100.0, 20.0, 20.0 ],
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t f f",
									"numinlets" : 1,
									"id" : "obj-37",
									"numoutlets" : 2,
									"fontname" : "Arial",
									"patching_rect" : [ 50.0, 308.0, 32.5, 20.0 ],
									"outlettype" : [ "float", "float" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl 4096 iter 1",
									"numinlets" : 2,
									"id" : "obj-38",
									"numoutlets" : 2,
									"fontname" : "Arial",
									"patching_rect" : [ 50.0, 284.0, 79.0, 20.0 ],
									"outlettype" : [ "", "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.route /range",
									"numinlets" : 1,
									"id" : "obj-16",
									"numoutlets" : 2,
									"fontname" : "Arial",
									"patching_rect" : [ 50.0, 252.0, 85.0, 20.0 ],
									"outlettype" : [ "", "FullPacket" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"id" : "obj-14",
									"numoutlets" : 2,
									"fontname" : "Arial",
									"patching_rect" : [ 50.0, 132.0, 50.0, 20.0 ],
									"outlettype" : [ "float", "bang" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/step $1",
									"numinlets" : 2,
									"id" : "obj-39",
									"numoutlets" : 1,
									"fontname" : "Arial",
									"patching_rect" : [ 50.0, 188.0, 51.0, 20.0 ],
									"outlettype" : [ "" ],
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.range @min 0 @max 1000 @step 1",
									"numinlets" : 1,
									"id" : "obj-9",
									"numoutlets" : 2,
									"fontname" : "Arial",
									"patching_rect" : [ 50.0, 220.0, 212.0, 20.0 ],
									"outlettype" : [ "FullPacket", "FullPacket" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "moveto 0 126",
									"numinlets" : 2,
									"id" : "obj-58",
									"numoutlets" : 1,
									"fontname" : "Arial",
									"patching_rect" : [ 163.0, 541.0, 83.0, 18.0 ],
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "prepend lineto",
									"numinlets" : 1,
									"id" : "obj-40",
									"numoutlets" : 1,
									"fontname" : "Arial",
									"patching_rect" : [ 298.0, 548.0, 87.0, 20.0 ],
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 1. 126. 0.",
									"numinlets" : 6,
									"id" : "obj-41",
									"numoutlets" : 1,
									"fontname" : "Arial",
									"patching_rect" : [ 339.0, 485.0, 105.0, 20.0 ],
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 1000. 0. 512.",
									"numinlets" : 6,
									"id" : "obj-42",
									"numoutlets" : 1,
									"fontname" : "Arial",
									"patching_rect" : [ 163.0, 485.0, 125.0, 20.0 ],
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0. 0. 0. 0.",
									"numinlets" : 5,
									"id" : "obj-15",
									"numoutlets" : 1,
									"fontname" : "Arial",
									"patching_rect" : [ 298.0, 524.0, 103.0, 20.0 ],
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.route /y",
									"numinlets" : 1,
									"id" : "obj-13",
									"numoutlets" : 2,
									"fontname" : "Arial",
									"patching_rect" : [ 299.0, 453.0, 61.0, 20.0 ],
									"outlettype" : [ "", "FullPacket" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"id" : "obj-45",
									"numoutlets" : 1,
									"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
									"outlettype" : [ "float" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"id" : "obj-47",
									"numoutlets" : 1,
									"patching_rect" : [ 299.0, 40.0, 25.0, 25.0 ],
									"outlettype" : [ "FullPacket" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"id" : "obj-48",
									"numoutlets" : 0,
									"patching_rect" : [ 63.0, 628.0, 25.0, 25.0 ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"id" : "obj-49",
									"numoutlets" : 0,
									"patching_rect" : [ 165.333313, 628.0, 25.0, 25.0 ],
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-40", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-41", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-41", 0 ],
									"destination" : [ "obj-15", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-42", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-36", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 1 ],
									"destination" : [ "obj-58", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-39", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-38", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-38", 0 ],
									"destination" : [ "obj-37", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-37", 0 ],
									"destination" : [ "obj-42", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-39", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-25", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-45", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-47", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-37", 1 ],
									"destination" : [ "obj-48", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 2 ],
									"destination" : [ "obj-49", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-58", 0 ],
									"destination" : [ "obj-49", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-40", 0 ],
									"destination" : [ "obj-49", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"globalpatchername" : "",
						"fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontface" : 0,
						"fontsize" : 12.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t 1. b dump",
					"numinlets" : 1,
					"id" : "obj-27",
					"numoutlets" : 3,
					"fontname" : "Arial",
					"patching_rect" : [ 608.0, 120.0, 71.0, 20.0 ],
					"outlettype" : [ "float", "bang", "dump" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"id" : "obj-26",
					"numoutlets" : 1,
					"patching_rect" : [ 608.0, 96.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "zl group",
					"numinlets" : 2,
					"id" : "obj-17",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 1264.0, 288.0, 53.0, 20.0 ],
					"outlettype" : [ "", "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "zl group",
					"numinlets" : 2,
					"id" : "obj-18",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 1200.0, 288.0, 53.0, 20.0 ],
					"outlettype" : [ "", "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "unpack 0. 0.",
					"numinlets" : 1,
					"id" : "obj-19",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 1200.0, 264.0, 76.0, 20.0 ],
					"outlettype" : [ "float", "float" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "function",
					"numinlets" : 1,
					"id" : "obj-20",
					"numoutlets" : 4,
					"patching_rect" : [ 768.0, 24.0, 512.0, 126.0 ],
					"outlettype" : [ "float", "", "", "bang" ],
					"addpoints" : [ 43.243244, 0.908163, 0, 160.360367, 0.479592, 0, 349.549561, 0.377551, 0, 466.0, 0.871287, 2, 567.567566, 0.183673, 0, 702.702698, 0.938776, 0, 780.180176, 0.102041, 0, 902.702698, 0.27551, 0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.raised-cosine-interp",
					"numinlets" : 1,
					"id" : "obj-24",
					"numoutlets" : 3,
					"fontname" : "Arial",
					"patching_rect" : [ 960.0, 480.0, 126.0, 20.0 ],
					"outlettype" : [ "FullPacket", "", "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "lcd",
					"numinlets" : 1,
					"id" : "obj-43",
					"numoutlets" : 4,
					"patching_rect" : [ 408.0, 624.0, 512.0, 128.0 ],
					"outlettype" : [ "list", "list", "int", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /x 574. /xx /yy /w 100.",
					"numinlets" : 4,
					"id" : "obj-12",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 960.0, 440.0, 165.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "uniformrandom 0 1000",
					"numinlets" : 1,
					"id" : "obj-54",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 144.0, 128.0, 131.0, 20.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"id" : "obj-51",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 488.0, 104.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/interp/xx 43.2432 160.36 349.55 466 567.568 702.703 780.18 902.703 \n/interp/yy 0.908163 0.479592 0.377551 0.871287 0.183673 0.938776 0.102041 0.27551 \n",
					"linecount" : 3,
					"numinlets" : 2,
					"id" : "obj-28",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 720.0, 368.0, 421.0, 48.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /interp/xx 0. 0.25 0.75 1. /interp/yy 0. 0.5 0.5 0.",
					"numinlets" : 2,
					"id" : "obj-50",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 720.0, 336.0, 297.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.var @op union",
					"numinlets" : 2,
					"id" : "obj-3",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 472.0, 224.0, 99.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.var @op union",
					"numinlets" : 2,
					"id" : "obj-23",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 256.0, 464.0, 99.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /interp",
					"numinlets" : 1,
					"id" : "obj-22",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 256.0, 440.0, 85.0, 20.0 ],
					"outlettype" : [ "", "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.raised-cosine-interp",
					"numinlets" : 1,
					"id" : "obj-21",
					"numoutlets" : 3,
					"fontname" : "Arial",
					"patching_rect" : [ 256.0, 488.0, 126.0, 20.0 ],
					"outlettype" : [ "FullPacket", "", "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"id" : "obj-11",
					"numoutlets" : 1,
					"patching_rect" : [ 48.0, 64.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"id" : "obj-6",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 192.0, 544.0, 90.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"id" : "obj-29",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 80.0, 232.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"id" : "obj-32",
					"numoutlets" : 1,
					"patching_rect" : [ 24.0, 232.0, 20.0, 20.0 ],
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "qmetro 1",
					"numinlets" : 2,
					"id" : "obj-35",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 24.0, 256.0, 58.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t l l",
					"numinlets" : 1,
					"id" : "obj-10",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 144.0, 448.0, 32.5, 20.0 ],
					"outlettype" : [ "", "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"setminmax" : [ 0.0, 1000.0 ],
					"numinlets" : 1,
					"id" : "obj-7",
					"contdata" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 408.0, 480.0, 512.0, 136.0 ],
					"setstyle" : 2,
					"outlettype" : [ "", "" ],
					"orientation" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /x",
					"numinlets" : 1,
					"id" : "obj-5",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 168.0, 520.0, 61.0, 20.0 ],
					"outlettype" : [ "", "FullPacket" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/proposalwidth 50 \n/maxnumtries 50 \n/interp/xx 43.2432 160.36 349.55 466 567.568 702.703 780.18 902.703 \n/interp/yy 0.908163 0.479592 0.377551 0.871287 0.183673 0.938776 0.102041 0.27551 \n/interp/w 10 \n/x 904.342 \n/x-1 906.222 \n",
					"linecount" : 9,
					"numinlets" : 2,
					"id" : "obj-2",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 144.0, 264.0, 356.0, 131.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.metropolis-hastings",
					"numinlets" : 2,
					"id" : "obj-1",
					"numoutlets" : 4,
					"fontname" : "Arial",
					"patching_rect" : [ 144.0, 416.0, 125.0, 20.0 ],
					"outlettype" : [ "", "", "FullPacket", "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"id" : "obj-137",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 376.0, 128.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"id" : "obj-138",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 320.0, 128.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t f f",
					"numinlets" : 1,
					"id" : "obj-98",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 48.0, 160.0, 32.5, 20.0 ],
					"outlettype" : [ "float", "float" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b b",
					"numinlets" : 1,
					"id" : "obj-94",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"patching_rect" : [ 48.0, 96.0, 34.0, 20.0 ],
					"outlettype" : [ "bang", "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "uniformrandom",
					"numinlets" : 1,
					"id" : "obj-31",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 48.0, 128.0, 91.0, 20.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.build /u /x 500. /proposalwidth 1. /maxnumtries 50 /interp/w 100.",
					"numinlets" : 5,
					"id" : "obj-30",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"patching_rect" : [ 48.0, 184.0, 360.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-2", 1 ],
					"hidden" : 0,
					"midpoints" : [ 153.5, 476.0, 140.0, 476.0, 140.0, 251.0, 490.5, 251.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-53", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-53", 1 ],
					"destination" : [ "obj-12", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-53", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-12", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-12", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-50", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-52", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-52", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 1 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [ 705.5, 466.0, 403.0, 466.0, 403.0, 619.0, 417.5, 619.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 2 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 1 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 2 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 1 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 1 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 1 ],
					"destination" : [ "obj-5", 0 ],
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
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-54", 0 ],
					"destination" : [ "obj-30", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-137", 0 ],
					"destination" : [ "obj-30", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-138", 0 ],
					"destination" : [ "obj-30", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-98", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-30", 4 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-1", 1 ],
					"hidden" : 0,
					"midpoints" : [ 265.5, 531.0, 382.0, 531.0, 382.0, 404.0, 259.5, 404.0 ]
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
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 1 ],
					"destination" : [ "obj-23", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-28", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-3", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-98", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-94", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-94", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-94", 1 ],
					"destination" : [ "obj-54", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 3 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-35", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
