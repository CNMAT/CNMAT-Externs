{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 7,
			"minor" : 3,
			"revision" : 5,
			"architecture" : "x86",
			"modernui" : 1
		}
,
		"rect" : [ 23.0, 61.0, 1080.0, 683.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 15,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"showrootpatcherontab" : 0,
		"showontab" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"id" : "obj-4",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 5,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 1080.0, 657.0 ],
						"bglocked" : 1,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 2,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 2,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "gridded",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-4",
									"linecount" : 5,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 440.0, 60.100006, 160.0, 68.0 ],
									"presentation_rect" : [ 450.0, 94.0, 0.0, 0.0 ],
									"style" : "",
									"text" : "when you click on a control point, you get diagnostic information, including x and y, and spacing of nearby control points"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 440.0, 32.0, 150.0, 20.0 ],
									"style" : "",
									"text" : "Clicking on a control point:"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-43",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 753.0, 61.100006, 306.0, 31.0 ],
									"style" : "",
									"text" : "shift-drag in timewarp~'s viewport to make a selection first, then you can use a number of options to edit..."
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 12.0,
									"id" : "obj-42",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 40.0, 32.0, 66.0, 20.0 ],
									"style" : "",
									"text" : "Selections"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-41",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 753.0, 198.928284, 321.0, 31.0 ],
									"style" : "",
									"text" : "to clear the range selector, just click outside of it in timewarp~'s view (except on a control point)"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-40",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 470.0, 222.956573, 232.0, 43.0 ],
									"style" : "",
									"text" : "this msg will select points in all functions (including inactive functions), if the points are present within the selection bounds"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-38",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 261.5, 222.956573, 194.0, 21.0 ],
									"style" : "",
									"text" : "apply_selection_box_to_all_functions"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-37",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 158.0, 90.100006, 173.0, 19.0 ],
									"style" : "",
									"text" : "snap the moving of points to grid "
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-36",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 40.0, 90.100006, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-34",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 71.0, 90.100006, 77.0, 21.0 ],
									"style" : "",
									"text" : "snaptogrid $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-33",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 158.0, 60.100006, 255.0, 19.0 ],
									"style" : "",
									"text" : "snap all currently selected controlpoints to the grid"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-29",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 60.100006, 105.0, 21.0 ],
									"style" : "",
									"text" : "snapselectiontogrid"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-27",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 261.5, 196.928284, 276.0, 21.0 ],
									"style" : "",
									"text" : "add_points_at_selection_boundaries_for_all_functions"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-26",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 145.0, 233.456573, 31.0, 22.0 ],
									"style" : "",
									"text" : "thru"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-25",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 261.5, 170.899994, 190.0, 21.0 ],
									"style" : "",
									"text" : "add_points_at_selection_boundaries"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-24",
									"maxclass" : "number",
									"maximum" : 2,
									"minimum" : 0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 43.5, 263.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-19",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 753.0, 160.691818, 301.0, 19.0 ],
									"style" : "",
									"text" : "copy/paste, but start at end of the last selected control point"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-18",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 753.0, 132.845917, 151.0, 19.0 ],
									"style" : "",
									"text" : "flip selection about the x axis"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-16",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 753.0, 105.0, 270.0, 19.0 ],
									"style" : "",
									"text" : "reverse the points w/r/t frequency, keeping time intact"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-14",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 3,
											"revision" : 5,
											"architecture" : "x86",
											"modernui" : 1
										}
,
										"rect" : [ 59.0, 104.0, 485.0, 253.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 15.0, 15.0 ],
										"gridsnaponopen" : 1,
										"objectsnaponopen" : 1,
										"statusbarvisible" : 2,
										"toolbarvisible" : 1,
										"lefttoolbarpinned" : 0,
										"toptoolbarpinned" : 0,
										"righttoolbarpinned" : 0,
										"bottomtoolbarpinned" : 0,
										"toolbars_unpinned_last_save" : 0,
										"tallnewobj" : 0,
										"boxanimatetime" : 200,
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"description" : "",
										"digest" : "",
										"tags" : "",
										"style" : "",
										"subpatcher_template" : "gridded",
										"boxes" : [ 											{
												"box" : 												{
													"id" : "obj-19",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 153.0, 128.192307, 233.0, 22.0 ],
													"style" : "",
													"text" : "selectregion functions 2 timerange 4. 2."
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-11",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 153.0, 157.384613, 233.0, 22.0 ],
													"style" : "",
													"text" : "selectregion functions 2 controlpoints 3 4"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 12.0,
													"id" : "obj-33",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 153.0, 99.0, 233.0, 22.0 ],
													"style" : "",
													"text" : "selectregion timerange 2 3 freqrange 2 13"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-15",
													"maxclass" : "comment",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 96.5, 61.0, 178.0, 20.0 ],
													"style" : "",
													"text" : "timerange:  length, startposition"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-3",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 34.0, 16.0, 365.0, 22.0 ],
													"style" : "",
													"text" : "selectregion functions 2 timerange 8. 6. subdivs 4 controlpoints 3 4"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-10",
													"index" : 1,
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 34.0, 204.0, 30.0, 30.0 ],
													"style" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"source" : [ "obj-11", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"source" : [ "obj-19", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"source" : [ "obj-3", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"source" : [ "obj-33", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 851.0, 263.0, 185.0, 22.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"style" : "",
										"tags" : ""
									}
,
									"style" : "",
									"text" : "p selecting_regions (in progress)"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-9",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 127.5, 120.600006, 252.0, 19.0 ],
									"style" : "",
									"text" : "clear a previously selected group of control points"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-12",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 120.600006, 78.0, 21.0 ],
									"style" : "",
									"text" : "clearselection"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-20",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 660.0, 160.691818, 88.0, 21.0 ],
									"style" : "",
									"text" : "repeatselection"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-21",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 660.0, 105.0, 88.0, 21.0 ],
									"style" : "",
									"text" : "reverseselected"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-8",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 660.0, 132.845917, 87.0, 21.0 ],
									"style" : "",
									"text" : "invertselected"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 101.5, 263.0, 85.0, 22.0 ],
									"style" : "",
									"text" : "setfunction $1"
								}

							}
, 							{
								"box" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"colors" : [ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 ],
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"freqmax" : 20.0,
									"functions" : [ 11, 0.0, 10.347001, 10.347001, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 3.722891, 5.110409, 5.110409, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5.855422, 4.738172, 4.738172, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 6.975923, 4.542586, 4.542586, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 7.481942, 10.347002, 10.347002, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 8.475905, 5.741324, 4.542586, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 8.981924, 10.347002, 10.347002, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 9.975889, 5.741324, 5.741324, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 11.403611, 6.813879, 6.813879, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 12.090361, 6.577415, 6.577415, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 18.0, 4.542585, 4.542585, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 2.053873, 12.009177, 12.009177, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 4.377105, 0.623853, 0.623853, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5.855422, 4.797853, 4.797853, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 9.999999, 16.499998, 16.499998, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 12.090361, 12.580568, 12.580568, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 17.999998, 1.499999, 1.499999, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 11, 0.0, 8.328074, 8.328074, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 4.5, 8.999999, 8.999999, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5.855422, 9.369659, 9.369659, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 9.999999, 10.499999, 10.499999, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 11.499999, 12.499999, 12.499999, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 11.999999, 8.999999, 8.999999, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 12.090361, 7.915656, 7.915656, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 12.499999, 2.999999, 2.999999, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 15.487951, 5.236592, 5.236592, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 18.0, 11.293373, 11.293373, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
									"highlight_every_n" : 67,
									"id" : "obj-1",
									"major_grid_line_color" : [ 0.65098, 0.666667, 0.662745, 0.68 ],
									"maxclass" : "timewarp~",
									"name" : "twhelp",
									"numinlets" : 2,
									"numoutlets" : 7,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "", "" ],
									"patching_rect" : [ 40.0, 300.0, 996.0, 317.0 ],
									"pointColor" : [ 0.317647, 0.654902, 0.976471, 1.0 ],
									"selectionColor" : [ 0.784314, 0.145098, 0.023529, 1.0 ],
									"show_xaxis" : 1,
									"show_yaxis" : 1,
									"snaptogrid" : 1,
									"timemax" : 18.0
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 12.0,
									"id" : "obj-28",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 753.0, 32.0, 147.0, 20.0 ],
									"style" : "",
									"text" : "Selecting with the mouse:"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"hidden" : 1,
									"source" : [ "obj-20", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"hidden" : 1,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"hidden" : 1,
									"source" : [ "obj-24", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"hidden" : 1,
									"source" : [ "obj-25", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-26", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"hidden" : 1,
									"source" : [ "obj-27", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-29", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-34", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-34", 0 ],
									"hidden" : 1,
									"source" : [ "obj-36", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"hidden" : 1,
									"source" : [ "obj-38", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"hidden" : 1,
									"source" : [ "obj-8", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 731.428589, 191.0, 70.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p Selecting"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 5,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 1080.0, 657.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 2,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 2,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "gridded",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 907.0, 77.5, 118.0, 20.0 ],
									"style" : "",
									"text" : "cellblock legend"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 10.0,
									"id" : "obj-4",
									"linecount" : 10,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 907.0, 103.5, 105.0, 118.0 ],
									"style" : "",
									"text" : "time\nduration\narrival tempo\ndeparture tempo\narrival phase\ndeparture phase\nalpha\nbeta\nerror corrected alpha\nerror corrected beta"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-1",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 187.0, 77.5, 590.0, 19.0 ],
									"style" : "",
									"text" : "typical values for the Alpha and Beta parameters are 0.25 to 4., although they can be effective outside of these bounds."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 24.0, 16.5, 121.0, 20.0 ],
									"style" : "",
									"text" : "Cellblock connection"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-2",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 187.0, 35.5, 698.0, 31.0 ],
									"style" : "",
									"text" : "Using a jit.cellblock in combination with timewarp~ exposes various extra parameters that are not intuitive to edit with max-based messaging.\nClick inside of the jit cellblock object below and chang the values to experiment with this"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-33",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 40.0, 89.0, 87.0, 22.0 ],
									"style" : "",
									"text" : "route cellblock"
								}

							}
, 							{
								"box" : 								{
									"cols" : 11,
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 10.0,
									"hsync" : 0,
									"id" : "obj-11",
									"maxclass" : "jit.cellblock",
									"numinlets" : 2,
									"numoutlets" : 4,
									"outlettype" : [ "list", "", "", "" ],
									"patching_rect" : [ 40.0, 120.0, 764.0, 162.5 ],
									"precision" : 6,
									"rows" : 100,
									"selmode" : 5,
									"selsync" : 0,
									"vsync" : 0
								}

							}
, 							{
								"box" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"colors" : [ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 ],
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"freqmax" : 15.0,
									"functions" : [ 3, 0.0, 2.555205, 2.555205, 0.0, 0.0, 0.1, 0.9, 3.0, 2.0, 2.0, 2.0, 0, 5.01004, 11.687697, 11.687697, 0.0, 1.2, 0.1, 0.9, 0.3, 1.0, 2.0, 2.0, 0, 10.0, 6.09442, 6.09442, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
									"id" : "obj-6",
									"major_grid_line_color" : [ 0.65098, 0.666667, 0.662745, 0.68 ],
									"maxclass" : "timewarp~",
									"name" : "twhelp",
									"numinlets" : 2,
									"numoutlets" : 7,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "", "" ],
									"patching_rect" : [ 40.0, 300.0, 996.0, 317.0 ],
									"pointColor" : [ 0.317647, 0.654902, 0.976471, 1.0 ],
									"selectionColor" : [ 0.784314, 0.145098, 0.023529, 1.0 ],
									"show_xaxis" : 1,
									"show_yaxis" : 1
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 1 ],
									"midpoints" : [ 49.5, 289.0, 1026.5, 289.0 ],
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"source" : [ "obj-33", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-33", 0 ],
									"midpoints" : [ 1026.5, 620.0, 31.0, 620.0, 31.0, 83.5, 49.5, 83.5 ],
									"source" : [ "obj-6", 6 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 912.714294, 191.0, 140.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p \"Editing with cellblock\""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 5,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 1080.0, 657.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 2,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 2,
						"objectsnaponopen" : 0,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "gridded",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"attr" : "num_minor_y_grid_divisions",
									"fontsize" : 11.0,
									"id" : "obj-19",
									"lock" : 1,
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 170.0, 250.0, 190.0, 21.0 ],
									"style" : "",
									"text_width" : 151.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "num_minor_x_grid_divisions",
									"fontsize" : 11.0,
									"id" : "obj-21",
									"lock" : 1,
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 170.0, 160.0, 190.0, 21.0 ],
									"style" : "",
									"text_width" : 151.0
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-11",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 810.0, 240.0, 140.0, 31.0 ],
									"style" : "",
									"text" : "start the highlighting beat counter at control points"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-36",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 810.0, 210.0, 22.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-7",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 838.5, 210.0, 181.0, 21.0 ],
									"style" : "",
									"text" : "reset_highlight_at_controlpoints $1"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 810.0, 90.0, 91.0, 22.0 ],
									"style" : "",
									"text" : "loadmess set 4"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-50",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 810.0, 170.0, 158.0, 31.0 ],
									"style" : "",
									"text" : "highlight every N beats for the current function",
									"textjustification" : 0
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-49",
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 810.0, 140.0, 45.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-3",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 860.0, 140.0, 112.0, 21.0 ],
									"style" : "",
									"text" : "highlight_every_n $1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-40",
									"maxclass" : "live.line",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 121.0, 33.5, 170.0, 5.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-39",
									"maxclass" : "live.line",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 430.0, 180.0, 117.5, 5.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-38",
									"maxclass" : "live.line",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 490.0, 150.0, 5.0, 30.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-37",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 140.0, 70.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-35",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 610.0, 170.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-34",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 610.0, 200.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-31",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 610.0, 140.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-30",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 610.0, 230.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-20",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 400.0, 108.0, 180.0, 43.0 ],
									"style" : "",
									"text" : "display numbers for graph bins\nY: frequency in Hz\nX: time in seconds",
									"textjustification" : 1
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-18",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 540.0, 50.0, 72.0, 22.0 ],
									"style" : "",
									"text" : "loadmess 1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-16",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 140.0, 220.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-12",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 140.0, 190.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 140.0, 130.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 140.0, 100.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 293.0, 26.0, 287.0, 33.0 ],
									"style" : "",
									"text" : "view a different function.  This also operates as an alternative to the \"addfunction\" message."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "number",
									"maximum" : 3,
									"minimum" : 0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 40.0, 30.0, 79.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-2",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 60.0, 79.0, 21.0 ],
									"style" : "",
									"text" : "setfunction $1"
								}

							}
, 							{
								"box" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"colors" : [ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 ],
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"freqmax" : 23.0,
									"functions" : [ 5, 0.0, 5.562264, 5.562264, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 0.870616, 1.716326, 1.716326, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 8.277108, 3.555205, 3.555205, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 12.058043, 0.0, 0.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 18.0, 4.933753, 4.933753, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 2.053873, 12.009177, 12.009177, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 4.377105, 0.623853, 0.623853, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 9.180696, 17.000002, 17.000002, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 10.0, 1.87156, 1.87156, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 6, 0.0, 7.444794, 7.444794, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 6.524096, 3.280756, 3.280756, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 11.349397, 6.119873, 6.119873, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 11.746987, 3.596214, 3.596214, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 13.03012, 9.211355, 9.211355, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 18.0, 1.388012, 1.388012, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 4, 0.0, 16.403784, 16.403784, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 7.355422, 12.996844, 12.996844, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 12.86747, 2.902207, 2.902207, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 18.0, 1.640378, 1.640378, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
									"id" : "obj-1",
									"major_grid_line_color" : [ 0.65098, 0.666667, 0.662745, 0.68 ],
									"maxclass" : "timewarp~",
									"name" : "twhelp",
									"numinlets" : 2,
									"numoutlets" : 7,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "", "" ],
									"patching_rect" : [ 40.0, 300.0, 996.0, 317.0 ],
									"pointColor" : [ 0.317647, 0.654902, 0.976471, 1.0 ],
									"selectionColor" : [ 0.784314, 0.145098, 0.023529, 1.0 ],
									"show_beat_numbers" : 0,
									"show_major_xgrid" : 0,
									"show_major_ygrid" : 0,
									"show_minor_xgrid" : 0,
									"show_xaxis" : 0,
									"show_yaxis" : 0,
									"timemax" : 18.0
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-14",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 430.0, 230.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-15",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 460.0, 230.0, 84.0, 21.0 ],
									"style" : "",
									"text" : "show_yaxis $1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-13",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 430.0, 200.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-10",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 460.0, 200.0, 84.0, 21.0 ],
									"style" : "",
									"text" : "show_xaxis $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-26",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 170.0, 190.0, 115.0, 21.0 ],
									"style" : "",
									"text" : "show_minor_ygrid $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-27",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 170.0, 220.0, 115.0, 21.0 ],
									"style" : "",
									"text" : "show_major_ygrid $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-25",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 170.0, 130.0, 115.0, 21.0 ],
									"style" : "",
									"text" : "show_minor_xgrid $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-24",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 170.0, 100.0, 115.0, 21.0 ],
									"style" : "",
									"text" : "show_major_xgrid $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-23",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 640.0, 140.0, 85.0, 21.0 ],
									"style" : "",
									"text" : "show_beats $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"hidden" : 1,
									"id" : "obj-22",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 640.0, 230.0, 129.0, 21.0 ],
									"style" : "",
									"text" : "show_beat_numbers $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-29",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 640.0, 170.0, 130.0, 21.0 ],
									"style" : "",
									"text" : "showtempocorrection $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"hidden" : 1,
									"id" : "obj-28",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 640.0, 200.0, 121.0, 21.0 ],
									"style" : "",
									"text" : "showbeatcorrection $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-17",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 170.0, 70.0, 70.0, 21.0 ],
									"style" : "",
									"text" : "showgrid $1"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-10", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"hidden" : 1,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"hidden" : 1,
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"hidden" : 1,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-27", 0 ],
									"hidden" : 1,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-17", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
									"hidden" : 1,
									"order" : 1,
									"source" : [ "obj-18", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-31", 0 ],
									"hidden" : 1,
									"order" : 0,
									"source" : [ "obj-18", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-19", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-22", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-23", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-24", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-25", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-26", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-27", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-28", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-29", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 0 ],
									"hidden" : 1,
									"source" : [ "obj-30", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-23", 0 ],
									"hidden" : 1,
									"source" : [ "obj-31", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-28", 0 ],
									"hidden" : 1,
									"source" : [ "obj-34", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-29", 0 ],
									"hidden" : 1,
									"source" : [ "obj-35", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"hidden" : 1,
									"source" : [ "obj-36", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-37", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-3", 0 ],
									"hidden" : 1,
									"source" : [ "obj-49", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-49", 0 ],
									"hidden" : 1,
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-25", 0 ],
									"hidden" : 1,
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 470.142853, 191.0, 105.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p \"View: Visibility\""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 5,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 1080.0, 657.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 2,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 2,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "gridded",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-18",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 230.0, 74.5, 100.0, 33.0 ],
									"style" : "",
									"text" : "common viewing routines"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-24",
									"maxclass" : "number",
									"maximum" : 2,
									"minimum" : 0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 40.0, 30.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-8",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 98.0, 30.0, 79.0, 21.0 ],
									"style" : "",
									"text" : "setfunction $1"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-60",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 3,
											"revision" : 5,
											"architecture" : "x86",
											"modernui" : 1
										}
,
										"rect" : [ 59.0, 104.0, 153.0, 249.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 15.0, 15.0 ],
										"gridsnaponopen" : 1,
										"objectsnaponopen" : 1,
										"statusbarvisible" : 2,
										"toolbarvisible" : 1,
										"lefttoolbarpinned" : 0,
										"toptoolbarpinned" : 0,
										"righttoolbarpinned" : 0,
										"bottomtoolbarpinned" : 0,
										"toolbars_unpinned_last_save" : 0,
										"tallnewobj" : 0,
										"boxanimatetime" : 200,
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"description" : "",
										"digest" : "",
										"tags" : "",
										"style" : "",
										"subpatcher_template" : "gridded",
										"boxes" : [ 											{
												"box" : 												{
													"id" : "obj-43",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 67.0, 132.0, 47.0, 22.0 ],
													"style" : "",
													"text" : "set 18."
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-42",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 32.0, 75.0, 47.0, 22.0 ],
													"style" : "",
													"text" : "set 23."
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-46",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"patching_rect" : [ 32.0, 25.0, 60.0, 22.0 ],
													"style" : "",
													"text" : "loadbang"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-58",
													"index" : 1,
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 32.0, 187.0, 30.0, 30.0 ],
													"style" : ""
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-59",
													"index" : 2,
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 187.0, 30.0, 30.0 ],
													"style" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-58", 0 ],
													"source" : [ "obj-42", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-59", 0 ],
													"source" : [ "obj-43", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-42", 0 ],
													"order" : 1,
													"source" : [ "obj-46", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-43", 0 ],
													"order" : 0,
													"source" : [ "obj-46", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 479.0, 84.0, 36.0, 22.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"style" : "",
										"tags" : ""
									}
,
									"style" : "",
									"text" : "p init"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"hidden" : 1,
									"id" : "obj-54",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 789.0, 144.0, 59.0, 21.0 ],
									"style" : "",
									"text" : "pak 0. 18."
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"format" : 6,
									"id" : "obj-2",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 559.0, 144.0, 54.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"format" : 6,
									"id" : "obj-45",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 619.0, 144.0, 54.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-53",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 679.0, 144.0, 100.0, 21.0 ],
									"style" : "",
									"text" : "timeminmax $1 $2"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-33",
									"linecount" : 5,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 455.0, 170.0, 158.0, 68.0 ],
									"style" : "",
									"text" : "careful with these parameters as they force the object to redraw its divisions and can cause the CPU to get bogged down in rendering...",
									"textcolor" : [ 0.65098, 0.666667, 0.662745, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"format" : 6,
									"id" : "obj-19",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 619.0, 204.0, 54.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"format" : 6,
									"id" : "obj-21",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 619.0, 174.0, 54.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-7",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 679.0, 204.0, 67.0, 21.0 ],
									"style" : "",
									"text" : "timemax $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-11",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 679.0, 174.0, 64.0, 21.0 ],
									"style" : "",
									"text" : "timemin $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"format" : 6,
									"id" : "obj-5",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 619.0, 114.0, 54.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"format" : 6,
									"id" : "obj-3",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 619.0, 84.0, 54.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-52",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 679.0, 114.0, 66.0, 21.0 ],
									"style" : "",
									"text" : "freqmax $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-51",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 679.0, 84.0, 62.0, 21.0 ],
									"style" : "",
									"text" : "freqmin $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"format" : 6,
									"id" : "obj-4",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 559.0, 54.0, 54.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"format" : 6,
									"id" : "obj-48",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 619.0, 54.0, 54.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"hidden" : 1,
									"id" : "obj-10",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 789.0, 54.0, 76.0, 21.0 ],
									"style" : "",
									"text" : "pak 0. 23."
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-12",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 679.0, 54.0, 100.0, 21.0 ],
									"style" : "",
									"text" : "freqminmax $1 $2"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-15",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 139.0, 150.0, 143.0, 19.0 ],
									"style" : "",
									"text" : "query timewarp~'s bounds "
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 150.0, 92.0, 22.0 ],
									"style" : "",
									"text" : "gettimeminmax"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-47",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 679.0, 240.0, 311.0, 31.0 ],
									"style" : "",
									"text" : "scale the function to fit the currently defined time bounds\n(try changing the time min or max and then clicking on this)"
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.384314, 0.384314, 0.384314, 1.0 ],
									"bgfillcolor_angle" : 270.0,
									"bgfillcolor_autogradient" : 0.79,
									"bgfillcolor_color" : [ 0.290196, 0.309804, 0.301961, 1.0 ],
									"bgfillcolor_color1" : [ 0.384314, 0.384314, 0.384314, 1.0 ],
									"bgfillcolor_color2" : [ 0.376471, 0.376471, 0.376471, 1.0 ],
									"bgfillcolor_proportion" : 0.39,
									"bgfillcolor_type" : "gradient",
									"fontname" : "Arial",
									"fontsize" : 11.0,
									"gradient" : 0,
									"id" : "obj-43",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 99.0, 176.0, 21.0 ],
									"style" : "",
									"text" : "scaletofit y, getfreqminmax, dump",
									"textcolor" : [ 0.878431, 0.933333, 0.933333, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-16",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 615.0, 240.0, 54.0, 21.0 ],
									"style" : "",
									"text" : "scaletofit"
								}

							}
, 							{
								"box" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"colors" : [ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 ],
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"freqmax" : 20.0,
									"functions" : [ 5, 0.0, 5.056604, 5.056604, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 0.919548, 1.560296, 1.560296, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 4.551765, 20.0, 20.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 12.735748, 0.0, 0.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 18.0, 8.706623, 8.706623, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 3.696972, 14.128442, 14.128442, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 7.87879, 0.733945, 0.733945, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 16.525253, 19.999998, 19.999998, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 18.0, 2.201835, 2.201835, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
									"highlight_every_n" : 67,
									"id" : "obj-1",
									"major_grid_line_color" : [ 0.65098, 0.666667, 0.662745, 0.68 ],
									"maxclass" : "timewarp~",
									"name" : "twhelp",
									"numinlets" : 2,
									"numoutlets" : 7,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "", "" ],
									"patching_rect" : [ 40.0, 300.0, 996.0, 317.0 ],
									"pointColor" : [ 0.317647, 0.654902, 0.976471, 1.0 ],
									"selectionColor" : [ 0.784314, 0.145098, 0.023529, 1.0 ],
									"show_xaxis" : 1,
									"show_yaxis" : 1,
									"timemax" : 18.0
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.384314, 0.384314, 0.384314, 1.0 ],
									"bgfillcolor_angle" : 270.0,
									"bgfillcolor_autogradient" : 0.79,
									"bgfillcolor_color" : [ 0.290196, 0.309804, 0.301961, 1.0 ],
									"bgfillcolor_color1" : [ 0.384314, 0.384314, 0.384314, 1.0 ],
									"bgfillcolor_color2" : [ 0.376471, 0.376471, 0.376471, 1.0 ],
									"bgfillcolor_proportion" : 0.39,
									"bgfillcolor_type" : "gradient",
									"fontname" : "Arial",
									"fontsize" : 11.0,
									"gradient" : 0,
									"id" : "obj-44",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 74.5, 175.0, 21.0 ],
									"style" : "",
									"text" : "scaletofit x, gettimeminmax, dump",
									"textcolor" : [ 0.878431, 0.933333, 0.933333, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-6",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 108.0, 226.0, 64.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-41",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 40.0, 226.0, 64.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-13",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 108.0, 250.0, 67.0, 21.0 ],
									"style" : "",
									"text" : "timemax $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-14",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 250.0, 64.0, 21.0 ],
									"style" : "",
									"text" : "timemin $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-20",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "", "", "FullPacket" ],
									"patching_rect" : [ 40.0, 200.0, 155.0, 21.0 ],
									"style" : "",
									"text" : "o.route /time/min /time/max"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-20", 0 ],
									"midpoints" : [ 863.666687, 627.0, 26.583344, 627.0, 26.583344, 189.0, 49.5, 189.0 ],
									"source" : [ "obj-1", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
									"hidden" : 1,
									"source" : [ "obj-10", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"hidden" : 1,
									"source" : [ "obj-19", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-54", 0 ],
									"hidden" : 1,
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-41", 0 ],
									"source" : [ "obj-20", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"source" : [ "obj-20", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"hidden" : 1,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"hidden" : 1,
									"source" : [ "obj-24", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-51", 0 ],
									"hidden" : 1,
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"hidden" : 1,
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"source" : [ "obj-41", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-43", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-44", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-54", 1 ],
									"hidden" : 1,
									"source" : [ "obj-45", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 1 ],
									"hidden" : 1,
									"source" : [ "obj-48", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-52", 0 ],
									"hidden" : 1,
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-51", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-52", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-53", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-53", 0 ],
									"hidden" : 1,
									"source" : [ "obj-54", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-19", 0 ],
									"hidden" : 1,
									"order" : 0,
									"source" : [ "obj-60", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"hidden" : 1,
									"order" : 1,
									"source" : [ "obj-60", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-48", 0 ],
									"hidden" : 1,
									"order" : 1,
									"source" : [ "obj-60", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"hidden" : 1,
									"order" : 0,
									"source" : [ "obj-60", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 602.285706, 191.0, 102.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p \"View: Bounds\""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 5,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 1080.0, 657.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 2,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 2,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 0,
						"lefttoolbarpinned" : 2,
						"toptoolbarpinned" : 2,
						"righttoolbarpinned" : 2,
						"bottomtoolbarpinned" : 2,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 0,
						"enablevscroll" : 0,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "gridded",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"attr" : "drawposition",
									"id" : "obj-13",
									"lock" : 1,
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 256.5, 240.0, 120.0, 22.0 ],
									"style" : "",
									"text_width" : 95.0
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-12",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 230.0, 153.5, 315.5, 19.0 ],
									"style" : "",
									"text" : "note:  timewarp~ is stateful and does not have an undo history.",
									"textcolor" : [ 0.784314, 0.145098, 0.023529, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 10.0,
									"id" : "obj-8",
									"linecount" : 5,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 659.0, 60.0, 120.0, 62.0 ],
									"style" : "",
									"text" : "te_breakout (tempo editor's breakout) allows you to access the output signals of a specified function."
								}

							}
, 							{
								"box" : 								{
									"data" : [ 593, "png", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ],
									"embed" : 1,
									"id" : "obj-78",
									"maxclass" : "fpic",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "jit_matrix" ],
									"patching_rect" : [ 256.5, 21.0, 109.0, 48.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 828.0, 221.0, 74.0, 22.0 ],
									"style" : "",
									"text" : "startwindow"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 9.172735,
									"id" : "obj-28",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 793.0, 25.0, 156.0, 27.0 ],
									"style" : "",
									"text" : "specify the tempo editor's breakout parameters with attributes",
									"textjustification" : 1
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 9.172735,
									"id" : "obj-27",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 628.0, 188.0, 151.0, 37.0 ],
									"style" : "",
									"text" : "timwarp output\n(output the wrapped phase of time map's beats)",
									"textjustification" : 1
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 9.172735,
									"id" : "obj-26",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 100.0, 264.0, 108.818184, 27.0 ],
									"style" : "",
									"text" : "phase input \n(index into tempo map)",
									"textjustification" : 1
								}

							}
, 							{
								"box" : 								{
									"bubble" : 1,
									"id" : "obj-24",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 822.0, 254.0, 98.0, 24.0 ],
									"style" : "",
									"text" : "turn audio on"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-22",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 3,
											"revision" : 5,
											"architecture" : "x86",
											"modernui" : 1
										}
,
										"rect" : [ 59.0, 104.0, 157.0, 281.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 15.0, 15.0 ],
										"gridsnaponopen" : 1,
										"objectsnaponopen" : 1,
										"statusbarvisible" : 2,
										"toolbarvisible" : 1,
										"lefttoolbarpinned" : 0,
										"toptoolbarpinned" : 0,
										"righttoolbarpinned" : 0,
										"bottomtoolbarpinned" : 0,
										"toolbars_unpinned_last_save" : 0,
										"tallnewobj" : 0,
										"boxanimatetime" : 200,
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"description" : "",
										"digest" : "",
										"tags" : "",
										"style" : "",
										"subpatcher_template" : "gridded",
										"boxes" : [ 											{
												"box" : 												{
													"id" : "obj-9",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 50.0, 128.0, 38.0, 22.0 ],
													"style" : "",
													"text" : "<~ 0."
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-8",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 50.0, 100.0, 44.0, 22.0 ],
													"style" : "",
													"text" : "delta~"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-20",
													"index" : 1,
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 50.0, 40.0, 30.0, 30.0 ],
													"style" : ""
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-21",
													"index" : 1,
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 50.0, 210.0, 30.0, 30.0 ],
													"style" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-8", 0 ],
													"source" : [ "obj-20", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-9", 0 ],
													"source" : [ "obj-8", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-21", 0 ],
													"source" : [ "obj-9", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 786.0, 105.0, 141.0, 22.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"style" : "",
										"tags" : ""
									}
,
									"style" : "",
									"text" : "p detect_zero_crossings"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-19",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 3,
											"revision" : 5,
											"architecture" : "x86",
											"modernui" : 1
										}
,
										"rect" : [ 59.0, 104.0, 342.0, 221.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 15.0, 15.0 ],
										"gridsnaponopen" : 1,
										"objectsnaponopen" : 1,
										"statusbarvisible" : 2,
										"toolbarvisible" : 1,
										"lefttoolbarpinned" : 0,
										"toptoolbarpinned" : 0,
										"righttoolbarpinned" : 0,
										"bottomtoolbarpinned" : 0,
										"toolbars_unpinned_last_save" : 0,
										"tallnewobj" : 0,
										"boxanimatetime" : 200,
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"description" : "",
										"digest" : "",
										"tags" : "",
										"style" : "",
										"subpatcher_template" : "gridded",
										"boxes" : [ 											{
												"box" : 												{
													"id" : "obj-2",
													"maxclass" : "comment",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 105.0, 32.0, 154.0, 20.0 ],
													"style" : "",
													"text" : "< zero crossings in (signal)"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-16",
													"maxclass" : "newobj",
													"numinlets" : 4,
													"numoutlets" : 1,
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 192.0, 100.0, 118.0, 22.0 ],
													"style" : "",
													"text" : "reson~ 300 900 300"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-12",
													"maxclass" : "newobj",
													"numinlets" : 4,
													"numoutlets" : 1,
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 50.0, 100.0, 118.0, 22.0 ],
													"style" : "",
													"text" : "reson~ 300 700 300"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-17",
													"index" : 1,
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 50.0, 28.0, 30.0, 30.0 ],
													"style" : ""
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-18",
													"index" : 1,
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 50.0, 174.0, 30.0, 30.0 ],
													"style" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-18", 0 ],
													"source" : [ "obj-12", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-18", 0 ],
													"source" : [ "obj-16", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-12", 0 ],
													"order" : 1,
													"source" : [ "obj-17", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-16", 0 ],
													"order" : 0,
													"source" : [ "obj-17", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 786.0, 137.0, 52.0, 22.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"style" : "",
										"tags" : ""
									}
,
									"style" : "",
									"text" : "p sonify"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-15",
									"maxclass" : "gain~",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "signal", "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 786.0, 171.0, 20.0, 61.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-14",
									"maxclass" : "ezdac~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 784.5, 252.0, 28.0, 28.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 649.5, 142.0, 108.0, 42.0 ],
									"range" : [ 0.0, 1.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"linecount" : 2,
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 786.0, 59.0, 166.0, 35.0 ],
									"style" : "",
									"text" : "te_breakout~ @name twhelp @function 0 @numoutlets 1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-11",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 40.0, 217.5, 43.0, 22.0 ],
									"style" : "",
									"text" : "*~ 10."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-10",
									"maxclass" : "scope~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 100.0, 195.0, 108.818184, 67.0 ],
									"range" : [ 0.0, 1.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 40.0, 158.0, 88.0, 22.0 ],
									"style" : "",
									"text" : "phasor~ 0.125"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Lato Regular",
									"fontsize" : 12.0,
									"id" : "obj-6",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 15.5, 94.0, 530.0, 50.0 ],
									"style" : "",
									"text" : "timewarp~ is a tool for editing tempo-based trajectories that either align to or deviate from an input phase ramp (signal).  It's particularly useful for sculpting multiple time maps.  The editing of alpha and beta parameters for arrival and departure points of a segment allows for warping.",
									"textcolor" : [ 0.32549, 0.345098, 0.372549, 0.84 ]
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Lato Regular",
									"fontsize" : 12.0,
									"id" : "obj-5",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 15.5, 71.0, 447.0, 21.0 ],
									"style" : "",
									"text" : "construct tempo-based beat and subdivision trajectories from an input phase signal"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 46.454056,
									"id" : "obj-4",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 15.5, 11.0, 224.0, 58.0 ],
									"style" : "",
									"text" : "timewarp~"
								}

							}
, 							{
								"box" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"colors" : [ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 ],
									"drawposition" : 0,
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"freqmax" : 15.0,
									"functions" : [ 5, 0.0, 8.832807, 8.832807, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 0.833333, 1.987381, 1.987381, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 2.5, 14.195583, 14.195583, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5.993976, 1.135646, 1.135646, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 10.0, 6.09442, 6.09442, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
									"id" : "obj-1",
									"major_grid_line_color" : [ 0.65098, 0.666667, 0.662745, 0.68 ],
									"maxclass" : "timewarp~",
									"name" : "twhelp",
									"numinlets" : 2,
									"numoutlets" : 7,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "", "" ],
									"patching_rect" : [ 40.0, 300.0, 996.0, 317.0 ],
									"pointColor" : [ 0.317647, 0.654902, 0.976471, 1.0 ],
									"selectionColor" : [ 0.784314, 0.145098, 0.023529, 1.0 ],
									"show_xaxis" : 1,
									"show_yaxis" : 1
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"midpoints" : [ 266.0, 295.0, 49.5, 295.0 ],
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 1 ],
									"order" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"order" : 1,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"source" : [ "obj-19", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 0 ],
									"order" : 0,
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-3", 0 ],
									"order" : 1,
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-19", 0 ],
									"source" : [ "obj-22", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"order" : 0,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"order" : 1,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 360.0, 191.0, 83.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p Introduction"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-5",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 5,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 23.0, 87.0, 1080.0, 657.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 2,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 2,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "gridded",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-10",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 830.0, 554.0, 117.0, 22.0 ],
									"style" : "",
									"text" : "print dumped_beats"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "FullPacket" ],
									"patching_rect" : [ 830.0, 496.0, 53.0, 22.0 ],
									"style" : "",
									"text" : "o.flatten"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 830.0, 526.0, 63.0, 22.0 ],
									"style" : "",
									"text" : "o.atomize"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 490.0, 71.0, 287.0, 22.0 ],
									"presentation_rect" : [ 493.72464, 71.0, 0.0, 0.0 ],
									"style" : "",
									"text" : "dumpbeats functions 0 subdivs 1 2 controlpoints 3 4"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-27",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 655.0, 537.5, 139.0, 43.0 ],
									"style" : "",
									"text" : "filter out a specific subdivision with listenumerate..."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-25",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 598.0, 537.5, 44.0, 22.0 ],
									"style" : "",
									"text" : "o.print"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-20",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 598.0, 496.0, 199.0, 22.0 ],
									"style" : "",
									"text" : "o.listenumerate /function/0/subdiv/2"
								}

							}
, 							{
								"box" : 								{
									"fontface" : 0,
									"fontsize" : 12.0,
									"id" : "obj-18",
									"maxclass" : "o.display",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 181.0, 496.0, 128.0, 34.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontface" : 0,
									"fontsize" : 12.0,
									"id" : "obj-17",
									"maxclass" : "o.display",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 32.0, 496.0, 128.0, 34.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-14",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 3,
											"revision" : 5,
											"architecture" : "x86",
											"modernui" : 1
										}
,
										"rect" : [ 84.0, 129.0, 618.0, 634.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 15.0, 15.0 ],
										"gridsnaponopen" : 1,
										"objectsnaponopen" : 1,
										"statusbarvisible" : 2,
										"toolbarvisible" : 1,
										"lefttoolbarpinned" : 0,
										"toptoolbarpinned" : 0,
										"righttoolbarpinned" : 0,
										"bottomtoolbarpinned" : 0,
										"toolbars_unpinned_last_save" : 0,
										"tallnewobj" : 0,
										"boxanimatetime" : 200,
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"description" : "",
										"digest" : "",
										"tags" : "",
										"style" : "",
										"subpatcher_template" : "gridded",
										"boxes" : [ 											{
												"box" : 												{
													"fontface" : 0,
													"fontsize" : 12.0,
													"id" : "obj-12",
													"linecount" : 136,
													"maxclass" : "o.display",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 50.0, 86.0, 236.0, 1868.0 ],
													"text" : "/dump/function/0 : {\n\t/function : 0,\n\t/point/0 : {\n\t\t/function : 0,\n\t\t/point : 0,\n\t\t/time : 0.431727,\n\t\t/tempo/arrival : 3.59306,\n\t\t/tempo/departure : 3.59306,\n\t\t/phase/arrival : 0.,\n\t\t/phase/departure : 0.,\n\t\t/alpha : 1.,\n\t\t/beta : 1.,\n\t\t/error/alpha : 2.,\n\t\t/error/beta : 2.\n\t},\n\t/point/1 : {\n\t\t/function : 0,\n\t\t/point : 1,\n\t\t/time : 0.833333,\n\t\t/tempo/arrival : 1.1087,\n\t\t/tempo/departure : 1.1087,\n\t\t/phase/arrival : 0.,\n\t\t/phase/departure : 0.,\n\t\t/alpha : 1.,\n\t\t/beta : 1.,\n\t\t/error/alpha : 2.,\n\t\t/error/beta : 2.\n\t},\n\t/point/2 : {\n\t\t/function : 0,\n\t\t/point : 2,\n\t\t/time : 3.63454,\n\t\t/tempo/arrival : 17.,\n\t\t/tempo/departure : 17.,\n\t\t/phase/arrival : 0.,\n\t\t/phase/departure : 0.,\n\t\t/alpha : 1.,\n\t\t/beta : 1.,\n\t\t/error/alpha : 2.,\n\t\t/error/beta : 2.\n\t},\n\t/point/3 : {\n\t\t/function : 0,\n\t\t/point : 3,\n\t\t/time : 5.99398,\n\t\t/tempo/arrival : 0.,\n\t\t/tempo/departure : 0.,\n\t\t/phase/arrival : 0.,\n\t\t/phase/departure : 0.,\n\t\t/alpha : 1.,\n\t\t/beta : 1.,\n\t\t/error/alpha : 2.,\n\t\t/error/beta : 2.\n\t},\n\t/point/4 : {\n\t\t/function : 0,\n\t\t/point : 4,\n\t\t/time : 10.,\n\t\t/tempo/arrival : 7.18612,\n\t\t/tempo/departure : 7.18612,\n\t\t/phase/arrival : 0.,\n\t\t/phase/departure : 0.,\n\t\t/alpha : 1.,\n\t\t/beta : 1.,\n\t\t/error/alpha : 2.,\n\t\t/error/beta : 2.\n\t}\n},\n/dump/function/1 : {\n\t/function : 1,\n\t/point/0 : {\n\t\t/function : 1,\n\t\t/point : 0,\n\t\t/time : 0.,\n\t\t/tempo/arrival : 0.,\n\t\t/tempo/departure : 0.,\n\t\t/phase/arrival : 0.,\n\t\t/phase/departure : 0.,\n\t\t/alpha : 1.,\n\t\t/beta : 1.,\n\t\t/error/alpha : 2.,\n\t\t/error/beta : 2.\n\t},\n\t/point/1 : {\n\t\t/function : 1,\n\t\t/point : 1,\n\t\t/time : 2.05387,\n\t\t/tempo/arrival : 12.0092,\n\t\t/tempo/departure : 12.0092,\n\t\t/phase/arrival : 0.,\n\t\t/phase/departure : 0.,\n\t\t/alpha : 1.,\n\t\t/beta : 1.,\n\t\t/error/alpha : 2.,\n\t\t/error/beta : 2.\n\t},\n\t/point/2 : {\n\t\t/function : 1,\n\t\t/point : 2,\n\t\t/time : 4.37711,\n\t\t/tempo/arrival : 0.623853,\n\t\t/tempo/departure : 0.623853,\n\t\t/phase/arrival : 0.,\n\t\t/phase/departure : 0.,\n\t\t/alpha : 1.,\n\t\t/beta : 1.,\n\t\t/error/alpha : 2.,\n\t\t/error/beta : 2.\n\t},\n\t/point/3 : {\n\t\t/function : 1,\n\t\t/point : 3,\n\t\t/time : 9.1807,\n\t\t/tempo/arrival : 17,\n\t\t/tempo/departure : 17,\n\t\t/phase/arrival : 0.,\n\t\t/phase/departure : 0.,\n\t\t/alpha : 1.,\n\t\t/beta : 1.,\n\t\t/error/alpha : 2.,\n\t\t/error/beta : 2.\n\t},\n\t/point/4 : {\n\t\t/function : 1,\n\t\t/point : 4,\n\t\t/time : 10.,\n\t\t/tempo/arrival : 1.87156,\n\t\t/tempo/departure : 1.87156,\n\t\t/phase/arrival : 0.,\n\t\t/phase/departure : 0.,\n\t\t/alpha : 1.,\n\t\t/beta : 1.,\n\t\t/error/alpha : 2.,\n\t\t/error/beta : 2.\n\t}\n}"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-13",
													"index" : 1,
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 50.0, 40.0, 30.0, 30.0 ],
													"style" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-12", 0 ],
													"source" : [ "obj-13", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 330.0, 496.0, 141.0, 22.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"style" : "",
										"tags" : ""
									}
,
									"style" : "",
									"text" : "p view_dumped_content"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "", "", "", "FullPacket" ],
									"patching_rect" : [ 32.0, 451.0, 466.0, 22.0 ],
									"style" : "",
									"text" : "o.select /time/* /freq/* /dump/*"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 132.193237, 71.0, 90.0, 22.0 ],
									"style" : "",
									"text" : "getfreqminmax"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 32.0, 71.0, 92.0, 22.0 ],
									"style" : "",
									"text" : "gettimeminmax"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 285.72464, 71.0, 196.0, 22.0 ],
									"style" : "",
									"text" : "dumpbeats functions 0 subdivs 1 2"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 230.618347, 71.0, 41.0, 22.0 ],
									"style" : "",
									"text" : "dump"
								}

							}
, 							{
								"box" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"colors" : [ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 ],
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"freqmax" : 17.0,
									"functions" : [ 5, 0.431727, 3.59306, 3.59306, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 0.833333, 1.108696, 1.108696, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 3.634538, 17.0, 17.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5.993976, 0.0, 0.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 10.0, 7.18612, 7.18612, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 2.053873, 12.009177, 12.009177, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 4.377105, 0.623853, 0.623853, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 9.180696, 17.000002, 17.000002, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 10.0, 1.87156, 1.87156, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
									"id" : "obj-1",
									"major_grid_line_color" : [ 0.65098, 0.666667, 0.662745, 0.68 ],
									"maxclass" : "timewarp~",
									"name" : "twhelp",
									"numinlets" : 2,
									"numoutlets" : 7,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "", "" ],
									"patching_rect" : [ 32.0, 108.0, 996.0, 317.0 ],
									"pointColor" : [ 0.317647, 0.654902, 0.976471, 1.0 ],
									"selectionColor" : [ 0.784314, 0.145098, 0.023529, 1.0 ],
									"show_xaxis" : 1,
									"show_yaxis" : 1
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"source" : [ "obj-1", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"source" : [ "obj-2", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-18", 0 ],
									"source" : [ "obj-2", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-9", 0 ],
									"source" : [ "obj-2", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-25", 0 ],
									"source" : [ "obj-20", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 1079.857178, 191.0, 63.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p Queries"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 5,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 1080.0, 657.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 2,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 2,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "gridded",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"attr" : "minor_grid_line_width",
									"id" : "obj-20",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 308.0, 131.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "major_grid_line_width",
									"id" : "obj-21",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 308.0, 104.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "textcolor",
									"id" : "obj-22",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 576.0, 158.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "selectionColor",
									"id" : "obj-23",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 576.0, 131.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "pointColor",
									"id" : "obj-24",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 576.0, 104.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "phaseColor",
									"id" : "obj-25",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 576.0, 77.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "minor_grid_line_color",
									"id" : "obj-16",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 308.0, 77.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "major_grid_line_color",
									"id" : "obj-15",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 308.0, 50.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "lineColor",
									"id" : "obj-14",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 576.0, 50.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "correctionColor_box",
									"id" : "obj-12",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 158.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "correctionColor",
									"id" : "obj-11",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 131.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "color",
									"id" : "obj-10",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 104.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"attr" : "bgFuncColor",
									"id" : "obj-9",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 77.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
, 							{
								"box" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"colors" : [ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 ],
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"freqmax" : 17.0,
									"functions" : [ 5, 0.0, 10.019323, 10.019323, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 0.833333, 1.108696, 1.108696, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 2.5, 17.0, 17.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5.993976, 0.0, 0.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 10.0, 6.454791, 6.454791, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 2.053873, 12.009177, 12.009177, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 4.377105, 0.623853, 0.623853, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 9.180696, 17.000002, 17.000002, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 10.0, 1.87156, 1.87156, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
									"id" : "obj-1",
									"major_grid_line_color" : [ 0.65098, 0.666667, 0.662745, 0.68 ],
									"maxclass" : "timewarp~",
									"name" : "twhelp",
									"numinlets" : 2,
									"numoutlets" : 7,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "", "" ],
									"patching_rect" : [ 40.0, 300.0, 996.0, 317.0 ],
									"pointColor" : [ 0.317647, 0.654902, 0.976471, 1.0 ],
									"selectionColor" : [ 0.784314, 0.145098, 0.023529, 1.0 ],
									"show_xaxis" : 1,
									"show_yaxis" : 1
								}

							}
, 							{
								"box" : 								{
									"attr" : "bgcolor",
									"id" : "obj-2",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 50.0, 192.0, 22.0 ],
									"style" : "",
									"text_width" : 150.0
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-10", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-20", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-22", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-23", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-24", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-25", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"hidden" : 1,
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 1170.0, 191.0, 85.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p Appearance"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 3,
							"revision" : 5,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 1080.0, 657.0 ],
						"bglocked" : 1,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 2,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 2,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "gridded",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-23",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 950.0, 486.0, 104.0, 19.0 ],
									"presentation_rect" : [ 946.5, 487.0, 0.0, 0.0 ],
									"style" : "",
									"text" : "initialize timewarp~"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-22",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 460.0, 83.0, 167.0, 31.0 ],
									"presentation_rect" : [ 482.5, 92.0, 0.0, 0.0 ],
									"style" : "",
									"text" : "clear the contents of a function \n(not removing the function itself)",
									"textjustification" : 1
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-20",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 167.5, 20.0, 170.0, 31.0 ],
									"presentation_rect" : [ 434.5, 101.0, 0.0, 0.0 ],
									"style" : "",
									"text" : "timewarp~ currently allows for a maximum of 8 functions",
									"textjustification" : 0
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-10",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 20.0, 590.0, 324.0, 19.0 ],
									"presentation_rect" : [ 101.5, 563.0, 0.0, 0.0 ],
									"style" : "",
									"text" : "note:  timewarp~ is stateful and does not have an undo history.",
									"textcolor" : [ 0.784314, 0.145098, 0.023529, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-33",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 735.0, 558.0, 78.0, 19.0 ],
									"style" : "",
									"text" : "time, minFreq"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-28",
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 471.5, 58.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-21",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 526.5, 58.0, 88.0, 21.0 ],
									"style" : "",
									"text" : "clearfunction $1"
								}

							}
, 							{
								"box" : 								{
									"hidden" : 1,
									"id" : "obj-17",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 353.0, 504.5, 31.0, 22.0 ],
									"style" : "",
									"text" : "thru"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 12.0,
									"id" : "obj-16",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 735.5, 19.0, 135.0, 20.0 ],
									"style" : "",
									"text" : "Editing with the mouse:"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-26",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 620.0, 504.5, 100.0, 19.0 ],
									"style" : "",
									"text" : "clipboard contents"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 12.0,
									"id" : "obj-7",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 15.5, 469.0, 64.0, 20.0 ],
									"style" : "",
									"text" : "Selections"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 15.5, 20.0, 118.0, 20.0 ],
									"style" : "",
									"text" : "Editing functions"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-40",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 950.0, 534.0, 113.0, 31.0 ],
									"style" : "",
									"text" : "assumes that function 2 exists"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-37",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 353.0, 469.0, 442.0, 19.0 ],
									"style" : "",
									"text" : "these commands assume that you've made a selection, and/or copied data into memory",
									"textcolor" : [ 0.0, 0.0, 0.0, 0.47 ]
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-34",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 15.5, 493.0, 297.0, 19.0 ],
									"style" : "",
									"text" : "make a selection (hold shift, then click-drag), then click this"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-32",
									"linecount" : 7,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 735.5, 43.0, 328.0, 93.0 ],
									"style" : "",
									"text" : "command-click: add a new point to the active function\ncontrol-click: remove a point\nclick-drag a point (or group of selected points) to move it\ncontrol-click a point to remove it\nshift-click a point to add it to, or remove it from a group of points\nhold shift and drag to select a range of points\nclick-drag a selection boundary once it's been drawn to move it"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-30",
									"linecount" : 5,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 428.0, 542.253174, 139.0, 68.0 ],
									"style" : "",
									"text" : "addfunction adds a new function to the list of functions (currently there is no 'removefunction N' available)"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-29",
									"linecount" : 4,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 167.5, 58.0, 242.0, 56.0 ],
									"style" : "",
									"text" : "setfunction adds a function if it doesn't exist.  If no functions exist between the input function max and the current max, timewarp~ will generate functions for the missing slots."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-27",
									"maxclass" : "number",
									"maximum" : 7,
									"minimum" : 0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 15.5, 58.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-25",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 72.5, 58.0, 79.0, 21.0 ],
									"style" : "",
									"text" : "setfunction $1"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-19",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 620.0, 586.0, 92.0, 21.0 ],
									"style" : "",
									"text" : "pasteattime 5.25"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-18",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 620.0, 558.0, 104.0, 21.0 ],
									"style" : "",
									"text" : "pasteatcoords 4. 7."
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-15",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 855.5, 582.0, 83.0, 21.0 ],
									"style" : "",
									"text" : "clearselection"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-14",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 855.5, 558.0, 83.0, 21.0 ],
									"style" : "",
									"text" : "clearrange"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-13",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 855.5, 534.0, 82.0, 21.0 ],
									"style" : "",
									"text" : "clearfunction 2"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-12",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 855.5, 510.0, 83.0, 21.0 ],
									"style" : "",
									"text" : "clearcurrent"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-11",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 855.5, 486.0, 82.0, 21.0 ],
									"style" : "",
									"text" : "clear"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-9",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 353.0, 542.253174, 67.0, 21.0 ],
									"style" : "",
									"text" : "addfunction"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-8",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 15.5, 542.253174, 276.0, 21.0 ],
									"style" : "",
									"text" : "add_points_at_selection_boundaries_for_all_functions"
								}

							}
, 							{
								"box" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"colors" : [ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 ],
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"freqmax" : 15.0,
									"functions" : [ 8, 0.0, 5.925925, 5.925925, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 1.80593, 2.824074, 2.824074, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 2.570281, 8.895899, 8.895899, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 3.782943, 9.321767, 9.321767, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5.158446, 4.684543, 4.684543, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 6.942053, 7.859707, 7.859707, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 8.598383, 7.870369, 7.870369, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 10.0, 3.61111, 3.61111, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 4, 0.0, 2.129338, 2.129338, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 1.560721, 3.894231, 3.894231, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 6.959203, 5.961538, 5.961538, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 10.0, 10.646688, 10.646688, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
									"id" : "obj-6",
									"major_grid_line_color" : [ 0.65098, 0.666667, 0.662745, 0.68 ],
									"maxclass" : "timewarp~",
									"name" : "twhelp",
									"numinlets" : 2,
									"numoutlets" : 7,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "", "" ],
									"patching_rect" : [ 15.5, 146.0, 1054.0, 312.0 ],
									"pointColor" : [ 0.317647, 0.654902, 0.976471, 1.0 ],
									"selectionColor" : [ 0.784314, 0.145098, 0.023529, 1.0 ],
									"show_xaxis" : 1,
									"show_yaxis" : 1
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-1",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 15.5, 517.0, 190.0, 21.0 ],
									"style" : "",
									"text" : "add_points_at_selection_boundaries"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-4",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 691.0, 530.0, 37.0, 21.0 ],
									"style" : "",
									"text" : "paste"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-3",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 651.5, 530.0, 34.0, 21.0 ],
									"style" : "",
									"text" : "copy"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 11.0,
									"id" : "obj-2",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 620.0, 530.0, 28.0, 21.0 ],
									"style" : "",
									"text" : "cut"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"hidden" : 1,
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"hidden" : 1,
									"source" : [ "obj-17", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-18", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-19", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"hidden" : 1,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"hidden" : 1,
									"source" : [ "obj-25", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-25", 0 ],
									"hidden" : 1,
									"source" : [ "obj-27", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 0 ],
									"hidden" : 1,
									"source" : [ "obj-28", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"hidden" : 1,
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"hidden" : 1,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"hidden" : 1,
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 828.571411, 191.0, 57.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p Editing"
				}

			}
 ],
		"lines" : [  ],
		"dependency_cache" : [ 			{
				"name" : "thru.maxpat",
				"bootpath" : "C74:/patchers/m4l/Pluggo for Live resources/patches",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "timewarp~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.select.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.display.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.listenumerate.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.print.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.atomize.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.flatten.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "te_breakout~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.route.mxo",
				"type" : "iLaX"
			}
 ],
		"autosave" : 0
	}

}
