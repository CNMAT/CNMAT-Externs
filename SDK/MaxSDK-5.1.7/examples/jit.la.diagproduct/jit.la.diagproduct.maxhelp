{
	"patcher" : 	{
		"rect" : [ 26.0, 49.0, 567.0, 396.0 ],
		"bglocked" : 0,
		"defrect" : [ 26.0, 49.0, 567.0, 396.0 ],
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
					"maxclass" : "newobj",
					"text" : "loadbang",
					"numoutlets" : 1,
					"fontsize" : 10.0,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 57.0, 137.0, 56.0, 20.0 ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.cellblock",
					"numoutlets" : 4,
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"gridlinecolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"selmode" : 5,
					"fontsize" : 10.0,
					"outlettype" : [ "list", "", "", "" ],
					"colwidth" : 45,
					"cols" : 3,
					"vscroll" : 0,
					"rowheight" : 17,
					"rows" : 3,
					"precision" : 4,
					"bgcolor" : [ 0.992218, 0.992218, 0.992218, 1.0 ],
					"patching_rect" : [ 86.0, 173.0, 136.0, 52.0 ],
					"hscroll" : 0,
					"id" : "obj-5",
					"fontname" : "Arial",
					"numinlets" : 2,
					"hcellcolor" : [ 0.836576, 0.836576, 0.933852, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 57.0, 161.0, 20.0, 20.0 ],
					"id" : "obj-6",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b",
					"numoutlets" : 1,
					"fontsize" : 10.0,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 34.0, 232.0, 21.0, 20.0 ],
					"id" : "obj-7",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.matrix 1 float32 3 3",
					"numoutlets" : 2,
					"fontsize" : 10.0,
					"outlettype" : [ "jit_matrix", "" ],
					"patching_rect" : [ 57.0, 258.0, 121.0, 20.0 ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "autohelp",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"hidden" : 1,
					"patching_rect" : [ 413.0, 336.0, 55.0, 20.0 ],
					"id" : "obj-12",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"fontsize" : 10.0,
					"outlettype" : [ "float", "bang" ],
					"triscale" : 0.9,
					"patching_rect" : [ 57.0, 343.0, 50.0, 20.0 ],
					"id" : "obj-21",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p complex+float64",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 239.0, 308.0, 105.0, 20.0 ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"numinlets" : 0,
					"patcher" : 					{
						"rect" : [ 65.0, 81.0, 356.0, 309.0 ],
						"bglocked" : 0,
						"defrect" : [ 65.0, 81.0, 356.0, 309.0 ],
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
									"maxclass" : "message",
									"text" : "plane -1",
									"numoutlets" : 1,
									"fontsize" : 10.0,
									"outlettype" : [ "" ],
									"patching_rect" : [ 7.0, 38.0, 50.0, 18.0 ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadbang",
									"numoutlets" : 1,
									"fontsize" : 10.0,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 58.0, 7.0, 56.0, 20.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "jit.cellblock",
									"numoutlets" : 4,
									"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"gridlinecolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"selmode" : 5,
									"fontsize" : 10.0,
									"outlettype" : [ "list", "", "", "" ],
									"colwidth" : 78,
									"cols" : 2,
									"vscroll" : 0,
									"rowheight" : 17,
									"rows" : 2,
									"precision" : 4,
									"bgcolor" : [ 0.992218, 0.992218, 0.992218, 1.0 ],
									"patching_rect" : [ 87.0, 64.0, 158.0, 36.0 ],
									"hscroll" : 0,
									"id" : "obj-3",
									"fontname" : "Arial",
									"numinlets" : 2,
									"hcellcolor" : [ 0.836576, 0.836576, 0.933852, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 58.0, 37.0, 20.0, 20.0 ],
									"id" : "obj-4",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b",
									"numoutlets" : 1,
									"fontsize" : 10.0,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 35.0, 109.0, 21.0, 20.0 ],
									"id" : "obj-5",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "jit.matrix 2 float64 2 2",
									"numoutlets" : 2,
									"fontsize" : 10.0,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 58.0, 136.0, 119.0, 20.0 ],
									"id" : "obj-6",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numoutlets" : 2,
									"fontsize" : 10.0,
									"outlettype" : [ "float", "bang" ],
									"triscale" : 0.9,
									"patching_rect" : [ 120.0, 231.0, 50.0, 20.0 ],
									"id" : "obj-8",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numoutlets" : 2,
									"fontsize" : 10.0,
									"outlettype" : [ "float", "bang" ],
									"triscale" : 0.9,
									"patching_rect" : [ 58.0, 231.0, 50.0, 20.0 ],
									"id" : "obj-9",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "unpack 0. 0.",
									"numoutlets" : 2,
									"fontsize" : 10.0,
									"outlettype" : [ "float", "float" ],
									"patching_rect" : [ 58.0, 198.0, 81.0, 20.0 ],
									"id" : "obj-10",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "jit.la.diagproduct",
									"numoutlets" : 2,
									"fontsize" : 10.0,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 58.0, 167.0, 95.0, 20.0 ],
									"id" : "obj-11",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "when using two planes, the entries are assumed to be complex numbers. plane 0 is real. plane 1 is imaginary.",
									"linecount" : 2,
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"patching_rect" : [ 58.0, 254.0, 292.0, 33.0 ],
									"id" : "obj-12",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 1 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [ 16.5, 61.0, 96.5, 61.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [ 67.5, 162.0, 248.0, 162.0, 248.0, 34.0, 96.5, 34.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [ 44.5, 131.0, 67.5, 131.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [ 96.5, 102.0, 44.5, 102.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [ 67.5, 31.0, 16.5, 31.0 ]
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
						"default_fontsize" : 10.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "supports: float32, float64, 1 or 2 planes (1=real, 2=complex)",
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 239.0, 279.0, 315.0, 20.0 ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.la.diagproduct",
					"numoutlets" : 2,
					"fontsize" : 10.0,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 57.0, 296.0, 95.0, 20.0 ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "jit.la.determinant",
					"numoutlets" : 0,
					"fontface" : 3,
					"fontsize" : 18.0,
					"frgb" : [ 0.27, 0.35, 0.47, 1.0 ],
					"patching_rect" : [ 10.0, 8.0, 485.0, 30.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"numinlets" : 1,
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "Calculate the determinant of a matrix ",
					"numoutlets" : 0,
					"fontsize" : 11.0,
					"frgb" : [ 0.27, 0.35, 0.47, 1.0 ],
					"patching_rect" : [ 10.0, 36.0, 485.0, 21.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"numinlets" : 1,
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_description",
					"text" : "The jit.la.determinant object calculates the determinant of a given input matrix, and send the result out the leftmost outlet. The input matrix must have type float32 or float64, and may have planecount 1 or 2. If the input matrix has a planecount of 2, it is assumed that the data is from the set of complex numbers, and the output determinant is a list of 2 floating point values. The first element is the real component and the second element is the imaginary component.",
					"linecount" : 5,
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 10.0, 57.0, 524.0, 73.0 ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textbutton",
					"varname" : "autohelp_top_ref",
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numoutlets" : 3,
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontface" : 3,
					"fontsize" : 11.0,
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"outlettype" : [ "int", "", "int" ],
					"spacing_x" : 0.0,
					"presentation_rect" : [ 0.0, 0.0, 205.127213, 14.66663 ],
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"fontlink" : 1,
					"textovercolor" : [ 0.4, 0.5, 0.65, 1.0 ],
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"patching_rect" : [ 294.872803, 22.0, 205.127213, 14.66663 ],
					"text" : "open jit.la.determinant reference",
					"id" : "obj-9",
					"fontname" : "Arial",
					"spacing_y" : 0.0,
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"underline" : 1,
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_see_title",
					"text" : "See Also:",
					"numoutlets" : 0,
					"fontface" : 1,
					"fontsize" : 10.0,
					"patching_rect" : [ 418.0, 340.0, 100.0, 20.0 ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"varname" : "autohelp_see_menu",
					"numoutlets" : 3,
					"fontsize" : 10.0,
					"outlettype" : [ "int", "", "" ],
					"items" : [ "(Objects:)", ",", "jit.la.diagproduct", ",", "jit.la.inverse", ",", "jit.la.mult", ",", "jit.la.trace", ",", "jit.la.uppertri" ],
					"types" : [  ],
					"patching_rect" : [ 418.0, 360.0, 130.0, 20.0 ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel",
					"numoutlets" : 0,
					"mode" : 1,
					"patching_rect" : [ 5.0, 5.0, 495.0, 52.0 ],
					"grad2" : [ 0.78, 0.84, 0.86, 0.7 ],
					"id" : "obj-10",
					"numinlets" : 1,
					"grad1" : [ 0.86, 0.86, 0.75, 1.0 ],
					"background" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_see_panel",
					"numoutlets" : 0,
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ],
					"border" : 2,
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ],
					"patching_rect" : [ 413.0, 336.0, 140.0, 50.0 ],
					"id" : "obj-14",
					"numinlets" : 1,
					"background" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [ 66.5, 283.0, 226.0, 283.0, 226.0, 166.0, 95.5, 166.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 43.5, 255.0, 66.5, 255.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 95.5, 228.0, 43.5, 228.0 ]
				}

			}
 ]
	}

}
