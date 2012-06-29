{
	"patcher" : 	{
		"rect" : [ 48.0, 63.0, 517.0, 420.0 ],
		"bglocked" : 0,
		"defrect" : [ 48.0, 63.0, 517.0, 420.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 10.0,
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
					"maxclass" : "panel",
					"varname" : "autohelp_top_picture_panel",
					"numinlets" : 1,
					"id" : "obj-12",
					"patching_rect" : [ 10.0, 7.0, 31.0, 31.0 ],
					"numoutlets" : 0,
					"bordercolor" : [ 1.0, 1.0, 1.0, 0.75 ],
					"border" : 2,
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p mousing",
					"numinlets" : 0,
					"id" : "obj-1",
					"patching_rect" : [ 386.0, 214.0, 79.0, 20.0 ],
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"patcher" : 					{
						"rect" : [ 356.0, 405.0, 450.0, 248.0 ],
						"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
						"bglocked" : 0,
						"defrect" : [ 356.0, 405.0, 450.0, 248.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
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
									"maxclass" : "number",
									"numinlets" : 1,
									"id" : "obj-7",
									"patching_rect" : [ 260.0, 202.0, 50.0, 20.0 ],
									"numoutlets" : 2,
									"outlettype" : [ "int", "bang" ],
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numinlets" : 1,
									"id" : "obj-6",
									"patching_rect" : [ 69.0, 202.0, 50.0, 20.0 ],
									"numoutlets" : 2,
									"outlettype" : [ "int", "bang" ],
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "click jumps directly to the value",
									"linecount" : 2,
									"numinlets" : 1,
									"id" : "obj-5",
									"patching_rect" : [ 91.0, 99.0, 122.0, 33.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Relative:",
									"numinlets" : 1,
									"id" : "obj-3",
									"patching_rect" : [ 243.0, 25.0, 59.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "slider",
									"numinlets" : 1,
									"id" : "obj-4",
									"patching_rect" : [ 260.0, 51.0, 20.0, 140.0 ],
									"numoutlets" : 1,
									"relative" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Absolute:",
									"numinlets" : 1,
									"id" : "obj-2",
									"patching_rect" : [ 52.0, 25.0, 59.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "slider",
									"numinlets" : 1,
									"id" : "obj-14",
									"patching_rect" : [ 69.0, 51.0, 20.0, 140.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "…or drag the horizontal line up and down inside slider",
									"linecount" : 2,
									"numinlets" : 1,
									"id" : "obj-23",
									"patching_rect" : [ 281.0, 99.0, 154.0, 33.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontface" : 0,
						"globalpatchername" : "",
						"default_fontface" : 0,
						"default_fontsize" : 10.0,
						"fontsize" : 10.0,
						"fontname" : "Arial",
						"default_fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Float output:",
					"numinlets" : 1,
					"id" : "obj-36",
					"patching_rect" : [ 377.0, 251.0, 119.0, 20.0 ],
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"id" : "obj-35",
					"patching_rect" : [ 359.0, 305.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "slider",
					"numinlets" : 1,
					"id" : "obj-34",
					"patching_rect" : [ 359.0, 272.333344, 140.0, 20.0 ],
					"size" : 1.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"floatoutput" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "slider can be vertical:",
					"numinlets" : 1,
					"id" : "obj-15",
					"patching_rect" : [ 223.0, 251.0, 119.0, 20.0 ],
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "slider",
					"numinlets" : 1,
					"id" : "obj-14",
					"patching_rect" : [ 269.0, 271.333344, 20.0, 140.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "autohelp",
					"numinlets" : 1,
					"id" : "obj-3",
					"patching_rect" : [ 369.0, 358.0, 55.0, 20.0 ],
					"numoutlets" : 0,
					"hidden" : 1,
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p appearance",
					"numinlets" : 0,
					"id" : "obj-39",
					"patching_rect" : [ 386.0, 187.0, 79.0, 20.0 ],
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"patcher" : 					{
						"rect" : [ 128.0, 315.0, 487.0, 256.0 ],
						"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
						"bglocked" : 0,
						"defrect" : [ 128.0, 315.0, 487.0, 256.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
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
									"maxclass" : "flonum",
									"numinlets" : 1,
									"id" : "obj-32",
									"patching_rect" : [ 214.0, 56.0, 43.0, 20.0 ],
									"numoutlets" : 2,
									"maximum" : 1.0,
									"outlettype" : [ "float", "bang" ],
									"minimum" : 0.0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"id" : "obj-33",
									"patching_rect" : [ 169.0, 56.0, 43.0, 20.0 ],
									"numoutlets" : 2,
									"maximum" : 1.0,
									"outlettype" : [ "float", "bang" ],
									"minimum" : 0.0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"id" : "obj-31",
									"patching_rect" : [ 124.0, 56.0, 43.0, 20.0 ],
									"numoutlets" : 2,
									"maximum" : 1.0,
									"outlettype" : [ "float", "bang" ],
									"minimum" : 0.0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Alpha",
									"numinlets" : 1,
									"id" : "obj-30",
									"patching_rect" : [ 216.0, 38.0, 45.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"id" : "obj-29",
									"patching_rect" : [ 79.0, 56.0, 43.0, 20.0 ],
									"numoutlets" : 2,
									"maximum" : 1.0,
									"outlettype" : [ "float", "bang" ],
									"minimum" : 0.0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Destination",
									"numinlets" : 1,
									"id" : "obj-3",
									"patching_rect" : [ 278.0, 38.0, 67.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Blue",
									"numinlets" : 1,
									"id" : "obj-8",
									"patching_rect" : [ 172.0, 38.0, 33.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Green",
									"numinlets" : 1,
									"id" : "obj-9",
									"patching_rect" : [ 127.0, 38.0, 41.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pak 0. 0. 0. 1.",
									"numinlets" : 4,
									"id" : "obj-10",
									"patching_rect" : [ 79.0, 84.0, 154.0, 20.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "prepend set",
									"numinlets" : 1,
									"id" : "obj-14",
									"patching_rect" : [ 314.0, 82.0, 70.0, 20.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "prepend bgcolor",
									"numinlets" : 1,
									"id" : "obj-15",
									"patching_rect" : [ 79.0, 116.0, 92.0, 20.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "umenu",
									"numinlets" : 1,
									"id" : "obj-16",
									"items" : [ "bgcolor", ",", "bordercolor", ",", "knobcolor" ],
									"patching_rect" : [ 259.0, 56.0, 129.0, 20.0 ],
									"numoutlets" : 3,
									"outlettype" : [ "int", "", "" ],
									"types" : [  ],
									"fontsize" : 10.0,
									"labelclick" : 1,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Red",
									"numinlets" : 1,
									"id" : "obj-17",
									"patching_rect" : [ 81.0, 38.0, 30.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Color Map:",
									"numinlets" : 1,
									"id" : "obj-19",
									"patching_rect" : [ 234.0, 129.0, 100.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "bgcolor  = Background color",
									"numinlets" : 1,
									"id" : "obj-21",
									"patching_rect" : [ 239.0, 150.0, 222.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "bordercolor = Border  color",
									"numinlets" : 1,
									"id" : "obj-22",
									"patching_rect" : [ 239.0, 165.0, 224.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "knobcolor = Knob color",
									"numinlets" : 1,
									"id" : "obj-23",
									"patching_rect" : [ 239.0, 180.0, 223.0, 20.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "slider",
									"numinlets" : 1,
									"id" : "obj-1",
									"patching_rect" : [ 79.0, 172.0, 112.0, 55.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"knobcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"bgcolor" : [ 0.0, 1.0, 1.0, 1.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [ 323.5, 112.0, 88.5, 112.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-31", 0 ],
									"destination" : [ "obj-10", 1 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-33", 0 ],
									"destination" : [ "obj-10", 2 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-32", 0 ],
									"destination" : [ "obj-10", 3 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 1 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontface" : 0,
						"globalpatchername" : "",
						"default_fontface" : 0,
						"default_fontsize" : 10.0,
						"fontsize" : 10.0,
						"fontname" : "Arial",
						"default_fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "slider",
					"numinlets" : 1,
					"id" : "obj-17",
					"patching_rect" : [ 28.0, 138.0, 140.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "set 50",
					"numinlets" : 2,
					"id" : "obj-18",
					"patching_rect" : [ 28.0, 103.0, 43.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "50",
					"numinlets" : 2,
					"id" : "obj-19",
					"patching_rect" : [ 76.0, 103.0, 26.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "20",
					"numinlets" : 2,
					"id" : "obj-20",
					"patching_rect" : [ 108.0, 103.0, 26.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "100",
					"numinlets" : 2,
					"id" : "obj-21",
					"patching_rect" : [ 139.0, 103.0, 32.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"id" : "obj-22",
					"patching_rect" : [ 28.0, 169.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"id" : "obj-23",
					"patching_rect" : [ 79.0, 169.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "the slider can be sized to any shape without changing its numerical range. You can change the range with a size message or using the  object's Inspector.",
					"linecount" : 5,
					"numinlets" : 1,
					"id" : "obj-24",
					"patching_rect" : [ 223.0, 94.0, 194.0, 73.0 ],
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "slider",
					"numinlets" : 1,
					"id" : "obj-25",
					"patching_rect" : [ 40.0, 349.333344, 140.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"triscale" : 0.9,
					"numinlets" : 1,
					"id" : "obj-26",
					"patching_rect" : [ 40.0, 293.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"maximum" : 90,
					"outlettype" : [ "int", "bang" ],
					"minimum" : 30,
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "drag the Number Box to change the range",
					"linecount" : 2,
					"numinlets" : 1,
					"id" : "obj-27",
					"patching_rect" : [ 89.0, 286.0, 122.0, 33.0 ],
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "size $1",
					"numinlets" : 2,
					"id" : "obj-28",
					"patching_rect" : [ 40.0, 321.333313, 47.0, 18.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"id" : "obj-29",
					"patching_rect" : [ 40.0, 377.333344, 50.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "the slider displays and repeats any number sent to its inlet. The set message only changes the slider display.",
					"linecount" : 4,
					"numinlets" : 1,
					"id" : "obj-30",
					"patching_rect" : [ 27.0, 188.0, 177.0, 60.0 ],
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"id" : "obj-31",
					"patching_rect" : [ 284.0, 207.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "slider",
					"numinlets" : 1,
					"id" : "obj-32",
					"patching_rect" : [ 284.0, 173.0, 45.0, 30.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "slider",
					"numinlets" : 1,
					"fontface" : 3,
					"id" : "obj-4",
					"patching_rect" : [ 40.0, 8.0, 455.0, 30.0 ],
					"numoutlets" : 0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 10.0,
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "Output numbers by moving a slider onscreen",
					"numinlets" : 1,
					"id" : "obj-5",
					"patching_rect" : [ 10.0, 36.0, 485.0, 21.0 ],
					"numoutlets" : 0,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 10.0,
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_description",
					"text" : "slider is a user interface resembling a sliding-potentiometer which outputs numbers restricted within a specified range, offset by a specified number, and multiplied by a specified number.",
					"linecount" : 2,
					"numinlets" : 1,
					"id" : "obj-6",
					"patching_rect" : [ 10.0, 57.0, 488.0, 33.0 ],
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textbutton",
					"varname" : "autohelp_top_ref",
					"presentation_rect" : [ 0.0, 0.0, 137.071304, 14.66663 ],
					"text" : "open slider reference",
					"numinlets" : 1,
					"fontface" : 3,
					"id" : "obj-7",
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"spacing_y" : 0.0,
					"patching_rect" : [ 362.928711, 22.0, 137.071304, 14.66663 ],
					"numoutlets" : 3,
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"fontlink" : 1,
					"outlettype" : [ "int", "", "int" ],
					"textovercolor" : [ 0.33, 0.45, 0.6, 1.0 ],
					"fontsize" : 10.0,
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"underline" : 1,
					"spacing_x" : 0.0,
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_see_title",
					"text" : "See Also:",
					"numinlets" : 1,
					"fontface" : 1,
					"id" : "obj-9",
					"patching_rect" : [ 374.0, 362.0, 100.0, 20.0 ],
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"varname" : "autohelp_see_menu",
					"numinlets" : 1,
					"id" : "obj-10",
					"items" : [ "(Objects:)", ",", "Dial", ",", "Hslider", ",", "Kslider", ",", "Nslider", ",", "Multislider", ",", "Pictctrl", ",", "Pictslider", ",", "Rslider", ",", "Uslider", ",", "<separator>", ",", "(Tutorials:)", ",", "Tutorial_7" ],
					"patching_rect" : [ 374.0, 382.0, 130.0, 20.0 ],
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"types" : [  ],
					"fontsize" : 10.0,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "fpic",
					"varname" : "autohelp_top_picture",
					"numinlets" : 1,
					"id" : "obj-13",
					"pic" : "ubudog:\/Applications\/Max5\/Cycling '74\/object-palettes\/slider.svg",
					"patching_rect" : [ 14.0, 11.0, 23.0, 23.0 ],
					"numoutlets" : 0,
					"autofit" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel",
					"numinlets" : 1,
					"id" : "obj-8",
					"patching_rect" : [ 5.0, 5.0, 495.0, 52.0 ],
					"background" : 1,
					"numoutlets" : 0,
					"grad1" : [ 0.27, 0.35, 0.47, 1.0 ],
					"mode" : 1,
					"grad2" : [ 0.85, 0.85, 0.85, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_see_panel",
					"numinlets" : 1,
					"id" : "obj-11",
					"patching_rect" : [ 369.0, 358.0, 140.0, 50.0 ],
					"background" : 1,
					"numoutlets" : 0,
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ],
					"border" : 2,
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [ 37.5, 165.0, 88.5, 165.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [ 85.5, 130.0, 37.5, 130.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [ 148.5, 130.0, 37.5, 130.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [ 117.5, 130.0, 37.5, 130.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0
				}

			}
 ]
	}

}
