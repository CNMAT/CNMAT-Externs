{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 481.0, 44.0, 1017.0, 782.0 ],
		"bglocked" : 0,
		"defrect" : [ 481.0, 44.0, 1017.0, 782.0 ],
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
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/right/y 59 \n/left/z 4064 \n/left/x 2779 \n/right/z 4064 \n/left/y 2551 \n/right/x 2375 ",
					"linecount" : 6,
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 225.0, 150.0, 91.0, 89.0 ],
					"id" : "obj-3",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/right/y -0.971184 \n/left/z 0.00757021 \n/left/x 0.357265 \n/right/z 0.00757021 \n/left/y 0.24591 \n/right/x 0.159951 \n/degrees/right/y -29.1355 \n/degrees/left/x 10.7179 \n/degrees/left/y 7.37729 \n/degrees/right/x 4.79853 \n/meters/left/z 0.0151404 \n/meters/right/z 0.0151404 \n/left/radius 0.433716 \n/left/orientation 0.967958 \n",
					"linecount" : 14,
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 15.0, 495.0, 205.0, 200.0 ],
					"id" : "obj-20",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p Situate",
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 45.0, 90.0, 58.0, 20.0 ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 773.0, 114.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 773.0, 114.0, 640.0, 480.0 ],
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
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.rename /hid/Game-Trak-V1.3/1/20 /right/z",
									"outlettype" : [ "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 10.0, 154.0, 242.0, 20.0 ],
									"id" : "obj-20",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.rename /hid/Game-Trak-V1.3/1/19 /right/y",
									"outlettype" : [ "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 10.0, 130.0, 242.0, 20.0 ],
									"id" : "obj-21",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.rename /hid/Game-Trak-V1.3/1/18 /right/x",
									"outlettype" : [ "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 10.0, 107.0, 242.0, 20.0 ],
									"id" : "obj-22",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.rename /hid/Game-Trak-V1.3/1/17 /left/z",
									"outlettype" : [ "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 9.0, 83.0, 235.0, 20.0 ],
									"id" : "obj-18",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.rename /hid/Game-Trak-V1.3/1/16 /left/y",
									"outlettype" : [ "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 9.0, 59.0, 235.0, 20.0 ],
									"id" : "obj-17",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.rename /hid/Game-Trak-V1.3/1/15 /left/x",
									"outlettype" : [ "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 9.0, 38.0, 235.0, 20.0 ],
									"id" : "obj-2",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"outlettype" : [ "" ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 8.0, 7.0, 25.0, 25.0 ],
									"id" : "obj-16",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 10.0, 181.0, 25.0, 25.0 ],
									"id" : "obj-19",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-20", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-22", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/right/y 59 \n/left/z 4064 \n/left/x 2779 \n/right/z 4064 \n/left/y 2551 \n/right/x 2375 ",
					"linecount" : 6,
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 300.0, 15.0, 91.0, 89.0 ],
					"id" : "obj-29",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p Feature Detection",
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 15.0, 270.0, 116.0, 20.0 ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 862.0, 44.0, 583.0, 247.0 ],
						"bglocked" : 0,
						"defrect" : [ 862.0, 44.0, 583.0, 247.0 ],
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
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.call /left/orientation < -0.001 @as /left/clockwise",
									"outlettype" : [ "FullPacket", "", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 3,
									"patching_rect" : [ 50.0, 152.0, 274.0, 20.0 ],
									"id" : "obj-21",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.call /left/orientation > 0.001 @as /left/anticlockwise",
									"outlettype" : [ "FullPacket", "", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 3,
									"patching_rect" : [ 50.0, 127.0, 289.0, 20.0 ],
									"id" : "obj-20",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p Difference",
									"outlettype" : [ "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 50.0, 100.0, 75.0, 20.0 ],
									"id" : "obj-16",
									"fontname" : "Arial",
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 201.0, 190.0, 434.0, 122.0 ],
										"bglocked" : 0,
										"defrect" : [ 201.0, 190.0, 434.0, 122.0 ],
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
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "unit delay",
													"fontsize" : 12.0,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 86.0, 37.0, 156.0, 20.0 ],
													"id" : "obj-2",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "o.call /left/orientation /was/left/orientation - 0. @as /delta/left/orientation",
													"outlettype" : [ "FullPacket", "", "FullPacket" ],
													"fontsize" : 12.0,
													"numinlets" : 1,
													"numoutlets" : 3,
													"patching_rect" : [ 8.0, 62.0, 388.0, 20.0 ],
													"id" : "obj-5",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "o.remember",
													"outlettype" : [ "FullPacket" ],
													"fontsize" : 12.0,
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 8.0, 38.0, 75.0, 20.0 ],
													"id" : "obj-4",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"outlettype" : [ "FullPacket" ],
													"numinlets" : 0,
													"numoutlets" : 1,
													"patching_rect" : [ 8.0, 7.0, 25.0, 25.0 ],
													"id" : "obj-14",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 7.0, 87.0, 25.0, 25.0 ],
													"id" : "obj-15",
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 0 ],
													"destination" : [ "obj-15", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"globalpatchername" : "",
										"default_fontface" : 0,
										"fontface" : 0,
										"fontsize" : 12.0,
										"default_fontname" : "Arial",
										"default_fontsize" : 12.0,
										"fontname" : "Arial"
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"outlettype" : [ "FullPacket" ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 50.0, 67.0, 25.0, 25.0 ],
									"id" : "obj-22",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 50.0, 183.0, 25.0, 25.0 ],
									"id" : "obj-24",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-20", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p Adjust DetectionThreshholds",
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 1,
					"patching_rect" : [ 135.0, 285.0, 173.0, 20.0 ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 110.0, 562.0, 436.0, 147.0 ],
						"bglocked" : 0,
						"defrect" : [ 110.0, 562.0, 436.0, 147.0 ],
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
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadbang",
									"outlettype" : [ "bang" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 8.0, 2.0, 60.0, 20.0 ],
									"id" : "obj-10",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/pluck/upper/threshhold 0.05 \n/pluck/lower/threshhold -0.05",
									"linecount" : 2,
									"outlettype" : [ "" ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 1,
									"patching_rect" : [ 8.0, 30.0, 321.0, 34.0 ],
									"id" : "obj-9",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.multisliders /pluck/upper/threshhold 0. 0.2 /pluck/lower/threshhold 0. -0.2",
									"outlettype" : [ "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 6.0, 77.0, 405.0, 20.0 ],
									"id" : "obj-4",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 6.0, 105.0, 25.0, 25.0 ],
									"id" : "obj-15",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p Chart",
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 105.0, 360.0, 50.0, 20.0 ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 345.0, 120.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 345.0, 120.0 ],
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
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.multisliders /left/radius /left/pluck -1. 1. @setstyle 2",
									"outlettype" : [ "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 5.0, 39.0, 290.0, 20.0 ],
									"id" : "obj-11",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"outlettype" : [ "FullPacket" ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 5.0, 8.0, 25.0, 25.0 ],
									"id" : "obj-6",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 5.0, 68.0, 25.0, 25.0 ],
									"id" : "obj-13",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "fpic",
					"autofit" : 1,
					"numinlets" : 1,
					"ignoreclick" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 118.5, 8.0, 119.0, 109.0 ],
					"id" : "obj-5",
					"embed" : 1,
					"data" : [ 57039, "", "IBkSG0fBZn....PCIgDQRA..AvB..D.KHX....Pd84Xc....DLmPIQEBHf.B7g.YHB..f.PRDEDU3wI68lGlbTcdu+eNU06KytznQZj.DHVsAK1bvQFCFb.6DuFGv12Df3fIlGmfSLd4wj3b4F6vCdiqyie7cK+t1OAi88ZmahArMFSPXVjjQKHDBoQBDRHjfQZlQy9L8RUcUme+QOum4zk5YzHX.EI5W8zO8np6tpScpy4648866xQo0ZMMjFRCogbbf3brtAzPZHMjFxbUZ.X0PZHMjiajF.VMjFRC43FoAfUCogzPNtQZ.X0PZHMjiajF.VMjFRC43FoAfUCogzPNtQlW.rJWtr4ukv5JHHnl+eXXn46D8ydMIZfP4O8HDMddxwpL+bMZHMjYRzfNHDHj.BoBATIL.MPkJGqabm3IulArBCCIYxjn0Z788w22G.bccwyyCkRQXXHNNSeobccAfJyKOQCAU02UTAEAjHFfBzDLOb9aHMjYWTJEfiYxjqiKJfXtGCaTmfJyalDJfOIRj.OOOBCCIQhD366iiiCgggDFFViFOwiGed3JW87oC8nXwQQQko.v.EgyxuqgzPlGDsFTSMfy.ZEhVGfueoicsqSPkWy.VNNN344Q73wId73TpTIt0O+sRGczAKXAKf68duWy2ywwAkRQPPfQSrW6R.nBwqTAFajCA5xfFpD.BXVCog75kDDFNk17UEsNDEgL9XCwjicniossSDk4ESBES7777HUpTrpe2UMkZxv+5+5+Jdddn0ZC2Uttt333LuvujVAn0DF5SgIGk.+hn0UG3na3SgFxaPR0wb.gZPGfWowwyahi0MqS3j4EMrbccQq0DKVL.XUqZUzYmcRXXHO6y9rrsssMTJkQ6JXZdrdMKZWPoHQbWHL.GMnb.WGGBa.X0PdcVblZXbXnFst56PHD5S5jMzve9VlWzvBpR7nVqQq0rnEsHd6+Nuc7884fG7f78+AeeJWtrAbSz3RYr8+UunUNf1AW2jDywop2ACgpb7+Z+72PZHylHigCC0nTfiiBTZzgdjHdiELmuk4kdTAzJLLDkRgqqKuu266i1ZqMbbb39t26ie6u82ZB+gXwhY.3dsJJATx0gvJUCYB4zpogaZZHu9JgghmnqR3tiRA5PBB7PqaDWCy2x7hIgRHKHZXAvEdgWHW9ke4DDDfmmG21scaLxHifVqMdTrRkYONola.ZdSAPoIQFESVr.JU.9UJiKNl1Ti3wpg75gnzwqRGR7xTITixwETYPEeLftNV27NgSlWLIT3kJd7pO7777XwKdw768686wRW5RQq0rqcsK9TepOE999355RPPfQSKQCMIVtrO2yEQo.BUDOdL78KCNgUUMugzPdcVDVMznmh+J.sFGGGh4F6XWC6DTYdSCKwbP4uiEKFm9oe57G+G+GSpTovwwg0u90ysbK2hwKgEKVzbNjeuDaVZsdNRLuIb8HYhbT1q.fdJfvWq2cMjFxQmn0ZiYhNtM3vZ9VlW5QEhGEytRlLINNNzbKMya6s813i9Q+nl3z5e4e4egO0m5SQkJUHSlLFsnDPK6yocJ+bjDs1kzoyhmWQPUAkJVCy.aHutKUCmgpiyDs5CC0UW.1oAf07s7ZtG0yyq5Ix5gi3AvkszkQlLY3Jthqfq9puZJWtLYylk6+9uet9q+5o+962D3odddlyg.bkLYx4TaPqAzwIV7zDFVFpTZpi2fzyFxquR3T7ipPgiiZJM6CpBhoZ.XMeKul6QSjHg4uEO+UoRESp4bRmzIQkJU3i8w9X7Q9HeDBBBPoT7HOxiv0ccWGacqakXwhYRiG4ymq7WM8EGvMEtwzToRwpChZrBWC4MHQgxvmUPvTofVCdTm2k4kYz1oZiRoHQhDFOysfEr.Cw6W0UcUbsW60R5zoIVrXr4MuY9DehOAe+u+2GsVa3uR3BSHy+HISa4mCwbcvyqHZsZdINuZHMjYSbi.JIbXMeEmgMjZk4E.KWW2ZHKWNlRoHYxjrhUrBRlLI4xki2+6+8yW3K7En4lalDIRvXiMF+8+8+87mci+Y7bO2ygqqqIQoO5iFdGbbgPcvzdrogzPd8TTTCvjVakf+M.rl2E2a+1u8ae97DFcUEsVSxjIIUpTL5nixniNJm5odpb9m+4Se80G6ae6iXwhwt10t3AevGDWWWV4JWoIWCkymDNDvzAnZPP.5vXPLHzAJ4qQG.AdiRtlyfRm4vFyzXUuFx7onUfpRYTtwwGMwTUnvgdAxjpSbx1NNMx1h4UYdGvJpH0BqLYxP1rYYxImjgGdX5pqt3RuzKkLYxvN24NoRkJToREV8pWMqcsqk1aucN0S8TMfSPsAlpDNENppCZznIlqKt3wjSbHxmqcPk4vxNmF.VMj4aQQ.ZkCA.wTgTZ7CQrDoHV51ZjMqyyxq68m1dOr81amku7kSlrYXvAGjwGebtoa5l367c9Nb5m9oyniNJoSmlMsoMwMcS2D2zMcSr8suci4gx60DxCZPQUuz3hhXttL4jSBNpFZj2PdiQpYfV0EUcbZjVXudHutqgkXVmdpn+solZh1ZsM.XzQGk8u+8yxV1x3S7I9DjNcZ1111F999DKVLdgW3E3m+y+4ru8sO5pqtnqtplpCJkxTYHPqIjJTQqPWQiKkY3gNHs2dmTILKQcTXCMrZHy2hRG.JGBUJbIfRicHRkNKpjM2PCq4Y40c.KXZPB4cGGGSfjBvXiMFSN4jbUW0UwUdkWIG7fGjW5kdISIo4oe5mle0u5Ww9129XoKcozd6saN25.ezNJbbhQLWENwfwFrWxlqITp733ppaaogzPlODstpIgJGWBQgKUnzXGhjoyAIx2fCq4Y40c.K6Zfk.V355RpToHSlLlDftToRr+8ueV7hWLW+0e8rrksL16d2Kuxq7JDOdbBBBXSaZS7K9E+B5qu9n6k1Ms0VaUCABET1WgthBW2.Fc3Wl3IxR5rcT21SCogLeIgZvQEBJGz3fCUn3nCPlrMgNdtF.VyyxaHZXIkFYoltKwYUxjIoolZxvKU73wY+6e+ru8sOdaus2F2zMcSr3EuX1+92O6e+6mzoSiVqYcqac7vO7CyHiLBq3TNIxlOONttPH353ie4QQGFR5LK7vbsbC.qFx7ons.rBQAZOJNxgHcSMCwx0vjv4YQoecNg6r2wbBBBLglf72B.1vCOLG3.GfxkKyDSLAkJUhLYxv4cdmGddd7.OvCv+q+W+uX26d2jJUJfpauXKcwKha3F+S4Z+DeRZIaZHrLCOPOT1KjE084WsDJaeC2.vpgLOJgZvAez3PEkCNgSxH6+4nstVFgIVPiJx17r7FhIghX6wP6ZnEfIvRkRNCTMOEe9m+4IHHf2065cw0bMWCszRK7BuvKvPCMDISljwlXX9MOxFYyabSbZm1RYwKYo3pBXxIGl7M0FZhY3BS1AerKdfM.vZHulDcYvIAUvgX5Jn7JxfCOFMuvtHjPbnQIlY9TdCwjvYSDvDwjv74ySlLYv00kXwhQpToX7wGm8t28RXXHW4Udk7Q9HeDRjHA6bm6jRdEIQ7br28ta942+8QEOOt3K77nTwwHalVPEKcM41HTqVeM.rZHulDsFsxAMJbU.AkYhIFilZqcBwEmF5XMuJutaR3bQpm1NdddTnPAFYjQv22mgGdXFd3gIQhDrzksT5n8Nvyyia+1+x7.+xGG+fxjJoKEKTh2wuy4xm4O+SvkekeXHVS0D7o1ZuYm31MjFxqJQGPHtD.DWUgJSNHCzeez0IcJTwIIwnwXr4S4XtFVhTMoQmtBMHZWkMaVzZMIRjflZpIpToB82e+L7vCSPP.2vM7I3zN0ykmZyafAFnexmqE1yteA18t1Am+YcVz9hOESEjHVrXSUy20lpiZCMrZHu1Dmp6rWJvUUA+xiSE+JjIWyD5jvZ+ftgLeHGy0vJ51Xuj9MhYhxwpToBkJUhgFZHSbaUrXQR35yh69bHYZE2423N4+2O49HtiOKr83jKYF9S+K9xbcW20Uy0TN+999yS69zMj2zJZHPCgpPhq7XhAeEBpDPSs0EAwyQrFg0v7pbLWCqnZ3XyokMVpTVkSjHAYylkDIRTMAoC74U5seJGTjq8i+wni16l0+aebJUbTRkLF2+u52fRo3bO2yk3wiimmGwhEiRkJMmKPfMjFxrJppubHfIFcPRkLEwSkkPmjMzuZdVNlCXASqUkv0jMeSBvkr8gkHQBRkJEwhEijISRBWGRjJGiNwvr68ratxq7Cxa4rNM5YqOIdkJQuGZB1912Ns1ZqrxUtRSZ+L2qY7MjFxrHSYNnVANTlQGpexmqIbRjCsJVC.q4Y4XtIgyUdjD.svvPiYbAAATdxQ3PCUlQJzGiNYQlXHEc0dZBKtK9p+82AO41NH.jOed9VequEefOvGfRkJQpToZvgUC40rDVATwf.z3pmfd26yyhWzhPkpCBTIa3iv4Y4X9B.yU.CQCKaNmbccISS4o6ksX5pqSkVx2DszpCElr.dUxvG9O72iNZNKAAUXzIJvs8U9JrmW74HUB.BQawuPi8tvichrPjHQeVD0gLyzmG84Wzem8VH270yZUL.c.tAJTp3Dnlf.mzfNItzXOEX9VNlCX8ZVzU2hvas0lX4m5xYoc2MYxlgzoSyhWzh4F+ytdZto7n0AL3gFj+y+c+8nCfJ990j0N1leNSS.ZHu9HRpZYKReu81Gm82WR0K6OOJumQ2MmhGOtA.a9Ry5Pc3T6EAXtF5PMU7Ac3bqDe2Pl6xw+.VJWpL0BmohGmN5nUZs8EP5r4vMYRdGu8Khq5p98PQ0A+O7C+v7i+w+XhEON990eyZsAP0arR8pRs06yzZMAAAlPSod+N42JK5333PkJUvyyCee+ZN21Zb8pUbTNDKla0MHmfJTInBJGEwhQ0cA5Fx7pb7OfEP73fiFBIjjIRwhW7hoiEtHxjuYJUbB93W6Ggy7LOqp6WbAv+3286xKtmciXcYzIHR0LsA+Vu9K0awgnNdwVqHY+qzNI5sC7X6+urQlDOdbRjHAwiG2j+pyWgzRPX.NNt35B9d9nTNDKdbPAgAMLIb9V9ODdI70hDFF.NtnTfV6iCt355P5zovwsLicngooV6f15naV25VKtNJFcjQPCbYWwUhajPnnAH0a7RTS2rOlbb6uijuoQA6lXhI3Ydlmg68duWt669t4+y+m+Or4MuYxlMKc2c2.UyvAIDYhdcd001cPGVAEtTr3XTp7PzTtNARhxENAQmf+CibL2KguVkJgdfNAwbgfvIPohCgIwwIfwm7EYeaaGDFKOjrc9pes6jG4geHxkMEKXgKje7+x+Bq3TVt4bE0LiFdQ70ewthzVu.FFlFTILLznwDTsh01SO8vy7LOCqeCqmc876hCdvCxXiMVMdSNUpTbAWvEve5e5eJW8Ue0lxscTdydU09ABC7vUkfREGjIK7hzd6mAAUxiahJPijeddUNtu2z0IABMTUKletUWTS4RrDonkVySlVVBub+k32+896yucsqAe+xbnAGj+e++9I74+q97lUbmoINMjW+kn45YXXnIcpjxq83iON6XG6fMsoMw5W+5Ym6XmbnAODkJUxDecoRkh74yWyVEmRoXcqacrgMrAtnK5h3Ftgaf2869caR6q4R6ZlDOeOh4.npBv535.SwckuWYhm339oX+GJ43dMrzHEQMHLbBbTI.RfVAUBGfA201nikbl7x8Wh9Oznb6+m+JrwMrVhkHAceRKkG9W+vjJUJSTuKj01HnReiQlIsX888wyyicricvZW6ZY8qe87BuvKvAO3AoXwh333XpnG17MNSmOg2qJUpfqqKWvEbA7w+3ebtlq4Zl012QBvJPGhqBzANrycrI7C5ky8btLvoIzTFkSiroX9TNtGvBppdUUMir732Tp6evW7wos16lXIamWbuGhG3AWMe665aRXPEBBJym6y844y849b355RgBEHSlLlyWCMrltePLgZlxCynluEk2I68WR6ZdVzraXaaaa7fO3Cxl27lYW6ZWzWe8gmmmwTP6EVjqWTshkiYa1m7tn4k.fcNmy4vMbC2.e3O7GlToRUy8ob9KWtrAXTz1K52azQGkUspUQqs1Jqd0q178gCuvUVuv3ngL2jSn.rp99TfVSMfn2c+DzdGKl3o5fgGoB6+k6muzW9KxVd5MQxjwo6tWJ2+8e+rvEtPfpCnjUrajbzLq.UQ+L6eS877Jf42GDDvfCNHacqakMu4MyNetcxjSLI6XG6fcricPPP.M2by0j2n999FseiV.HqGw8QkneG6.K8rNqyhO4m7SxG9C+gIQhDl14LUFhhduuksrEtjK4R3sbNuEdx0+jFPMatzhJ1.YMj4lbBmA1J0TCjm5+mLYZJVrLoaNEszRLBztb0W8Uy1d1sfVqYe6ae7s9VeKty67NMtA200kxkK2H4noVMSjLMHJQ41lPKZfDU788o+96msu8syS8TOEabiajm+4edS8NKLLzTy9at4lwyyiQGcT788Ia1rjOedhGONUpTwbcBCCMa2aRawF7rddRz9dRoTFSD2912N25sdq7i9Q+H9TepOEW0UcU.L81IGSCDGDDXNtbsJUpDkKWlErvETy00teQ3jydyXogbzImX.XMKqplOeybv9Fg1VD3FCBCqvpdm+t7v+6mGqe8OIYxjke5O8mxkdoWJ+A+A+Ale2a10rxVr2HaEsOEMOjvDH520yyiCbfCvl1zlXqacq7zO8Syd26d4PG5PFyBiGOtYWAW.Cbbbn4la1TgXmXhIX3gGlIlXBxjIioZzZqwGb3wOm.dEUSL4us2qL0ZMISlDkRwl27l4y7Y9LrpUsJ9ytw+Lth28UPXXnQiKg6LnpFRRex3iON.zYmcdXfUx0Tb.f7+avU5Qub7Of0r.Vo0gjHdZ78GD.BB7o81agRkKw0bMWC6ZW6xLP6Nti6fy67NOV5RWJkKW1X1RiATTCmM.0n8oXtTgBE3fG7fFvom9oeZ10t1EiM1X344YJMPR.bFMUnbbbLEXQQSjVasUhGONEKVDeeei2.UJEIRjfzoSa.uDMerayyjFVQyEQWWWyuMUpTToREd7G+wYcqac7tdWuK9LelOCW7Eew0v+UXXHISlzL9nu95CnJfkngtMHusIwhVVMFaczKG+CXMChvkUnVQhDogJUHdhT.wHQxXbwW7EyG8i9Q4e9e9eFeee1yd1CeouzWhevO3GPxjIMS.dy9fJQSJa9ZDvpm64dN1zl1DOyy7Lr0stU1+92OCMzPFsQjHL2lCnnZ8XS9d4xkAvLQub4xjHQBRlLo4+WtbY788oRkJlOOVrX0PJtsog1kGan1bFUtV1DoKscgGqUu5Uyi8XOF+Au++.tk+xagUrhUXFS344YnUXyadyjJUJy0LYxjlwOBXXzj2ugbzKG+S5tdpUUiPnZ3Tjw6TdDN3A6m1WXmjHSSToR.k7pP+Gb.5qu931u8amsrksP73woToR7I+jeR9pe0uJIRj3MEdIbtDGRxjeQypsrksv25a8sXMqYMTtbYCuRRcJSHx100Eee+ZLEBnFfEYRs.XH+NQSK4yiRpeoRknToRFdrRlLIISlzn8lbts8bn88jsHDiGKVroRj4o0TKVrX344Q4xko81am+j+j+Dt7K+x4gdnGhG5gdHNvAN.EKVjhEKxHiLBc1Ymblm4YxkbIWBevO3Gjy+7OeCHkMA9x8ZCuEdzImvAXASwOvT.VtTjd26KQl7MSKs2Ig.JUb12KUUafcu6cye6e6eKSLwDFhQ+Begu.25sdqyHAxGOIyGOds8N392+945ttqi0rl0vBVvBpI+7rCs.au2YyOEPM.SQCOAX5b.zNGBs8nmsIVdddFvKgKpzoSazJ7HceIsCIopkyscbakJUJ5t6tYfAFfW5kdIilX1.a1mGQRlLIqZUqhO2m6yw6889dAl1KzUpTog1VuJji6ykPTNUeMUcpULETM0+HHMEJuGbiOAoS2EJcUxUykKNCORYZok7rrksLVyZViY.7S9jOIISlzvagiiigb4nlZD0yT1+sL3b1Das.rk5klPQOdTW5G85E0adx8gLoTNl3ornWG6.xTZie8u9Wme1O6mQpToLltY+4Q+aaPHyiLqxAyg83zp8FsOH54Uq0lnaW1bcEttpToh4dXlBrT6mkRD0KsMOOOxlMKuqK6cwG7C7Ao+96mst0sxDSLQMd6SZCJkxDqXBfmRoX26d2be228wy+7OOWxkbIjISFSed8xmwFw+2rKG+CXcXRDMJzN3W4PDD3S1bKBzwQiFGWMddgLw3ixBVvBXgKbg7DOwSPhDIPq073O9iSlLY3s+1e6n0Z777LlaHfXyz9an72yEsyjISRISQ98Q0Rw93vzCrsmfaqwh.BYSxqmmmgeN42NxHiX9tEKVjjISRO8zC+5e8ule3O7GxBVvBLjeuqcsKti63NnPgBl5hubcs0xRtmr8p3bMUmNZSIJasjDsqDy3Fe7wqI.RiEKlAX1tuS.PDyY88847NuyiO6m8yxke4WNeuu22ie8u9Wa5mD.NwTXQaL4bKGWhDeeeed1m8Y4gdnGhy5rNKZu81ISlL0zFreF2PlY4De.qPGTpwYhIGglZpKPmfPcHNNJhmHC99U4AYwKdwjNcZdpm5oLdA5Idhmf3wiyEcQWDISlzvGCb3ZTXt50QyfYSrK.c1DaWu5aOTafsZeNr8Jl358BEJPxjIYG6XGzSO8P+82OKdwKFGGG5s2d4y9W8Y4a70+FbO2y8PlLY3htnKBsVyMey2Leuu22im8YeVdfG3AHQhDbgW3ExV1xV3+w+i+GjOedCWLBWShFCR7IEs8KxQhOo5Un9lsW1ZiI+dIjIRkJkI9nJWtrgOLoMJ8kQIceUqZU7k+xeYN4S9j4FuwajMu4Ma34xdLf.bYahqc5BY+rsRkJzWe8wV25VYIKYI333P974qQSsnZO1.75vk2T.X4FqDCMT+zRKcgRkxxM5tnCCYxImjff.tfK3B.fsrksX7RzZW6Zob4xboW5khqqKkJUxT4JqmIgylIMyjHZ+Xqgj86RrFYqYjjbvx+OHHfctycxO9G+iYrwFiku7kShDI3Q9MOB27m9l4G7C9Ab+2+8SPP.WxkbITpTI9ReouDCLv.L93iyi9nOJ8zSObgW3ExS7DOAaYKag74yimmGO5i9nrksrE5s2dYiabiFW1mISFC411EUO4kcpqD89sdf8GMZhYK0yjOAvR7noPbtDjpRXVXWdlUJEqXEqfa8VuUV9xWNe5O8ml0t10hRU0ynRJ5HQ.epTovyyC.il4RXWHfTRElHUpTjHQB1+92O82e+btm64RgBELf+QAsNdm6zWuj2T.X4DymAOzAn4V5.G2zTMZ30fxg3VlKUnPAV0pVEJkhMsoMYBru0rl0P+82OW1kcYlAohIQQ49IpF.GIIpIc1CXk+VVwVttaXCafG6wdLFXfA3TO0SEGGG9+9+8+Ke7O9Gm0rl0vC8PODCO7vrxUtR58U5k+o+o+IiFbO5i9nroMsI5pqt3AevGjQFYDi4I8zSOrl0rF16d2KiM1XlqsRoXu6cur0stUy4QhMJaOyUOMslIdph99LAfcjbZf8DaQyRa.cYWVJYxjlz6oXwhFvDI7KBBBHWtb79deuOtzK8RY0qd0789deOiIchYdxyA4Yt7aEs2RlLoIVyjPaHVrXTtbYBCCIa1r77O+ySqs1JKXAKfDIRPKszxgk+hMj5Kuo.vR4VgwG6PjLUJRjHGfKZMnQS7oVwbhIlfxkKyjSNIum2y6gDIRvl1zlv22mDIRvF1vFnmd5ge2e2eWZokV.l1aU0iKh4pHlXHbiX6J+gGdX5omd3.G3.r3EuXhEKF+jexOg+h+h+Bt+6+94gdnGhQGcTtrK6x3YdlmgG3Ad.C31u829aYaaaajJUJV6ZWqQarDIRv91293we7GmwGeb788MZvkHQBFZngnPgBlv7vyyCkRQrXwHWtbFsRrCxSQiFAnPz1xlOK6R3iHyFXl4o4bLrKpmltB.kbeaCt566SgBELlA1ZqsxIcRmDm8Ye1zUWcwN1wN3W8q9UFMwjyuMHmsmBSkJEM0TSzRKsTiF3Qq67c1YmnTJ1291GW9ke4DFFZ.tr0PEl1r5FxzxaN.rb7ob4wHDHS5V.hCn.UEzgTy.ROOOFYjQ3C9A+fzVaswS7DOgwEz8zSObe228gmmGm4YdljISFiYB1R8LUblDAnRoTTrXQi22dhm3I3Vtkag+6+2+uyu3W7Kv00kK7BuP1912N+jexOgzoSSXXHqe8qmm64dNRlLIqcsq0T2xylMK6cu6kMu4MSPPfw8+B3iHR94I.MJkhToRgiiiI1lBsLa1tTCK75IgffPpsbtrMaNJuSR+zbQlMNrrCEg5E3nBGShFR16qkJUUmN333vYe1mMq3zWAoSmlEsnEgiiCO7C+vTnPAiyK.LjsKZN455Z5uhGONYxjwb9kmkiM1XlbvLWtbzd6sy3iONu025akN6rS5t6tMbBFkOzF.V0Jm3CXocP4Dfu+n346Q5TMCpTnpFEDftplRRJY344QXXHCLv.7g9PeH5pqt3QdjGgxkKS974YjQFgG6wdL9k+heICMzPrhUrBRlL4gYBzb0qOAAA7i9Q+H9VequE+ve3OjN5nCN0S8TYW6ZW7O9O9OZ.xV25VG6ae6iS5jNIdvG7AMZAkNcZ5omd34e9mmBEJXbmu3Vd6PVPLmUlvIsaIMWjnIWLkRz7ShPaeeelbxIM+eoeS.DESns4ywNhxqGA70qOJplRy1qnb8D0TboMI7CJ.WwhEizoSSxjIYYKaYbtm64RKszhgapS4TNEbbb3YdlmolHvWz90000ncoPvdpTo3TO0SkUrhUPhDILo3UgBELzNHeu1ZqMtxq7Joqt5ht6t6CKOCe0vk2aFjS3.rTJmZe4Dh12EpTA+R6i7McJnbbIPoH.vEkYRt3x+wGebBCCYjQFg2y648v4bNmCabiajAFX.hEKF4ymmAFX.d7G+w4m9S+orMTAXVk...H.jDQAQksuMVvBV.c0UWlUisC2gImbRyJn81aurgMtAZq01HUpTL3fCxG+i+wYaaaaLv.CvC8POD4xkiy6scd7C+g+PJUpjg+kd5oGV25VWMd8RHzUlXJooh.zIlqIARoDrhRAxSzVBl1M7RZu.XLgRHaVq0TpTIi1Vx0WzTSLAKZjiamSc1dzyNHMiNIsdwp0bMPXixinsWKkW999jOed9nezOJ4xkihEKRKszB999jJUJdGui2AYxjgW5kdIFe7wMs2ngKhiiCs1ZqjHQBVzhVDcuztoboxL3fCRgBEL7WI.6c1YmbsW60xa4s7V37NuyqlxmiMGbQ0D01CxuYU6qS3.rNbQWM.RCKyjE5i7MsDfXDpbPgFW0zCBs2QUDuJ433v4e9mOW0UcUL1XiwK9huHiM1XFSKJWtLa8Y1J+q+q+q7XO1iwniNJm1ocZlxuqvehmmGe6u82luxe6Wgu+2+6yZVyZ3hu3Klt5pKt669tYrwFyD+PO0S8TzSO8vq7JuhgfXYkc6RrhTUDrC3Q6jKFvLYQ3cQlPHlSUrXwZx2O6JHfbts0nBlN50KUpj42IZpAXHo11jLaSDsiSKf5ZRmHGsZrFUjqUz1tPX9kcYWFKYIKg95qOiIbs1ZqUCBzDw4ctp2Ius21ai7MkmA5e.FczQqIGAk9VaOlVtbYFXfA3.G3.LwDSXLmNc5zbdm64we3e3eHe3O7GlS+zOcxjIioORZqEJTv34P6.LUZ+uV3L83c43+Ty4HJgPnCZugX+8tYV1xNevIGZmDDPHt5Z4XIHHfQGcTN3AOHiLxHFSFVvBV.AAAricrCti63NXsqcsL1XiYBVQIBqCCCoyN6jOxG4ivIexmLZsla7FuQbbb3i8w9X7K+k+RZs0Vw22mt6tatyu9cx+ka++BadyaljISRlLYnRkJjISFii.rKoIBww17pTnPAiG6jHoV.CBBBXhIl.kRYlbHh7Yxjtb4xY75kcBFKZlY6oMwzQsVa.ukTpQ1C.EyQEfHguII0YfCOh5qmVEQk5E0+yjHSrsCzWALsiN5fe+e+eeJTn.iLxHrjkrDxjICKXAKf74ySas0FoSmlVZoEBBB3ke4Wlst0sxZW6ZYaaaaLxHivjSNIkKWllZpIyhdYxjolpLQqs1JmwYbF767676vUbEWAm9oe5zTSMcXAebzhEncEvsd8MuYTdyAfk1ABmjW5E2HKs6kiSxNQqRRHgnBoFMTfpSjOzgNDG3.Gf95qOVxRVBmwYbFFfCsVSO8zC+fevOvj.rxprEKVznQgXVz0bMWC+C+C+CbW20cwsca2FKdwK1z5jhUmTdRDxtk59jjJMgggTrXQC2SxJ5B.hjNJB2L1oMS4xkM7mDKVLxlMqIVhDSVEGKHZaIlLIbdIfZBgxh4nRaR.hDR5EPKIlsj6AQCNw8+xBE1l4HZAFUaKQrArlKwqTT.K4YyocZmFm7IexTnPAxkOGczdGrrksL777XIKYIjKWNRlLI4ymmN5nCZqs1HVrXze+8yvCOLCN3fricrCNvAN.EJTvL1IQhDzQGcP974oqt5hEu3EyIexmbMgufj0A999r28tWNoS5jLfUhibpWkBodAN7aljS3ArzDhR6.3wKu2mlN5nIRk6jAU5pZXgScMAYzQGk95qOFbvAo+96upWjVwJXxImzX5PPP.6YO6gu7W9Kyu427aLdFpXwhTpTIyJyZsl2+6+8Stb43e5e5eBnp2pxjIigCHwE6JkxDbpB3f3NcaxssIJW.PjvPP7bkbbAbZxImz78EfIo8Il3IltjMaV.L.SBnmqqqI8WDuJJelRopAHxFTS.DsC0.wKjQAnhxYi8yE43x6GI.q540sff.xlMKm4YdlUauISPKM2Bc0UWr7kubFbvAo81amVZoEZs0VIa1rzbyMa.urycPax7iV5lqGnhjD4xmq0Z1wN1AM2byrjkrjZzhR5eEORVO93dyl7lGNrHDuhEHTOFoxtPz53fJDESmZFQcAuXxjjWZc1YmjMaVyfRGGGtoa5lnmd5wX1jnpeoRkLgofRoXW6ZWr28tWCvj3pcQqFgze63DRH1UhsJ6XcR.IRmNsYfs.HHZEATSYxQNOEKVrljfVzpQ.gkM8Aatkr6eriLb4ZHdMy92I7DA0uDFam+j1ZTTuEPpmLWlzFMLJjI6MkuIZs0VMUigVZtEdqu02JkJUxnkqD2Y4xkyrPjTcTk9L49MpWPsSgGI55KVrnYAOX5TpJa1rL93iSSM0jYgGXZGdTupgwQpu4DU4D93+2rO5ncHcpbTt7DS8ASUf+lB3PlDJRxjIo4lalVZoExkKGkKWlm4YdFdzG8QY0OxpMfNEKVjW5kdIyproSmlq4ZtF5niNLwzk30vhEKR5zoo4la1vyw3iOtwbBYfoXJhXtlMw5hIcoSmtFy4DSZk.Xz00kIlXBC+V1IGrTezmbxIMSlDP6LYxX3xxdxgLwUZaRMnRlTlHQBZpolHa1rFfAwrzToRY1LIDB7kygD3l10UKaPRnVvtYxLwY74u0yYah9imHtwb1fJAzTSMQ974YrwFyzOKNbnolZhJUpv1291Ye6aeFu5Es5XL4jSxHiLBvz0BdwSrwiGmgGdXd4W9kYrwFyzuCUC2j1auciYqO45eRtq65tXngFx3s1no4zaFAqfSfq3n1hVWMjqRjHM9iUFHfPsFUXHNNSqAf7tLXPhWlvvP1zl1D2y8bOFtl9hewuH2xsbKrrksLJUpjIQi2+92OO3C9fLwDSXV0UL0RpI4hYfkKWFsVyXiMlgzZQSEwrywGebyDqrYyZLoSzFazQGEfCSaorYyRxjIMbpo0ZiYbRnYXmFM1QltzOHZPDkiIIZ2E9ojuu7ajymvolbNDyLsCiD6fZU5Wr4tI5BIS+Lct4gLasgscFATcqsWzpQoT7hu3KViVLR+nXpMPMZLZykTPP.uxq7JL4jSxJW4JM.Z17RIlSKNjns1ZybdxmOOkJUpJ3dlr7c+teWV+5WOe0u5W0Dqe1fiuYsZ3dLWCKILBjTbv16VyzfUawNdUrOlI+tBqfRCZ7Q4Vg3pLPPUMC7mBrxNi6saSEKVjlatYVzhVD999rqcsKiYLeiuw2fuy246P2c2MISljIlXBSHBL3fCRrXwnolZhlZpISPDJqrJAQnnsjXtwHiLhgKHAfos1ZCGGGlbxIY7wG2v+gvikLnWLcULkU.1DhvEPGasWhFvqQcedzxCi74ReU8hve6mI1QwuMmV1lPI7sYGXpB3.Ta.mF0zKoev1junlIJWW61kD49EJTv7YG7fGzrXj7LV52xlMaM4JnDpH1Z6IKDHZKJsWoupXwhjMaVZu81YxImzDuehFv.lD01wwglZpI1vF1.+Q+Q+Q7O+O+Oad1F0bbo8Kd10te4DQ4XNGVxf1nGyd0qnOjjiIlcXysh7aMSzHjPsKNtg33pov3CvAd4gYsqeCr9M7zTpvDrrksrZhoo0t10x27a9M4Nuy6j8rm8v69c+tM4wmvwgqqKabiaj95qOdkW4ULtK2lTaX5fwz1adR7RAS6UIQyJ6HF21iVRXLHqTKDlGcxrc7FIG2FzIZ+3b44iHud4elnfk1wmk7dTvRatchdtrARh941dKUhn+LYxXLuMd73jOedZpolHWtbjOedRkJk4bJ4zYTOKKNeHHHfEtvEZ5ykEikwF4xkiXwhwXiMlgCSQaan53kG9geX9M+leiYLwi9nOJ8rid3rOqylN5nCyX.4YpPjuMmZ170chjbL2jPoNgKSLqWGbzAyxpYBHfnltrBtc0CHdrXru82Kae6afs7Tqic1ySwddgA3kNX+D5jhjwT7ddOuG9q9q9q3rO6yF.tq65tX0qd0zTSMwce22MZsl+j+j+DV3BWH6ZW6xXRjuuO6ae6yPztLnbhIlfToRYHDWhMGkpZPAJqvJl+XCNKQwtVqM+F6voPpuS18IBGI0yrti0CXms3oR9bQaLYQCIncEsLiRFuzeIZWX+Y0CnR9aguRsVSwhEMgnP1rYMl9WnPAlXhIniN5vPNtPBtDLvxuUZuR6ShgN.SZII7b566aJC2B+h4xkigGdX.LdXtPgBr0stUy8tD08+6Oz+NO2NeN9q+q+q4O5O5OplqgLdTddaukjchlbLGvRTCNp4EhHZQXuRlcQhKpIRZslW9keYV+5WOae6ame659sz6AN.G5P8Sf+jzY6MgiJAJfXwbIHv2TBa+5e8uNW3EdgbRmzIUCuO2y8bOTpbIyp6EJTfhEKRqs1pgTV6XdRL2vNfOAHc5z333X.sDdoD08EdrJTnfItpjU2EdqDyWkiYaxks1G1dGClc.i4aI50tdWGay3r4+RdFJbaIOas252kO29bXeNidcreIwHlD2XSLwDzau8hRoXYKaYzZqsZxORwDb.S.jFOdbiIg1ocjvOkzFrArDtB888YvAGDWWWlbxIQoTjOedSpaMwDSvF23FYqacq0TD.CCqVVZ1291Gewu3Wjm7IeR9q+q+qo6t61XJn.3KxIp6Z4GyArfo0DvNReEW5a+PHpFCxpYRUIXCaXC7LOyyvK8RuDiN5nToRERlLEZpN4NQrLjJYVB0U.kFGEnlhWhd5oGtwa7F4a+s+1rhUrB788Y7wGmVZoEzZM+r+seF999jKWNfpZ0LxHiXztRZeBg1SLwDL1XiQ974qA.Q7vzjSNISN4j.XxSO4uEhdsKXbx4P.vjAxx00lrX6++7oYb0CXXl.KpGP3Q52IfuIRjvDhD1UCCorsHKhIl.Ma2iQoJ.vvQl3U0wFaLC.oTFcj1nnMqXFn3jCIi..LNWv1IKBeRdddTrXQfpKlMwDSXZGxXLA76ke4WlG7AePN3AOXMNPQ.kDuUdO2y8vy9rOK21scabYW1kY3JTHs2NEmNVqg87sbLGvxN0DjAlBW.1AXGL8.n8t28x5W+5YaaaarwMtQ5u+9M6GdBODRfU54EPrXZbbbIzymxkqP5TPbWGTNJBClNNhFZng3VtkagUtxUZlzLxHiPas0F999lTawFXPBFSgXVwTuff.JTnfIuvjnOWturMuP.okAoBeDRjxaW9ajWhol1D6FUSKn9lEFkT5iFYtLIXtnMmsoaQ4mRh4LYwJ6fOMZ.Tdjt11jiKZgHStkqqvgTe80mA3xFjR9aQjmW144mM42hVWhyaj3ZCvDibRMXSzjt+96md5oGd1m8YMfjh0DRr0UrXQimK20t1EepO0mhO8m9Sy0e8WuwDVA7SRj+Szji4Q5tDiRQIZ1000DeR6cu6kMsoMw5W+5MIE73iO9TZPM8lpf7vslUWTwHjxnzwPETl7ISSqsEiWt+QwOLNwbml7U.i4YdddL5niZZaxFwfbdkU9kuS1rYMWeasGFbvp65zoSm1jNMxjfff.C4q1dKSVYcl.ZjGY1lGZ+4QAvlIY9Z02YxzynfayjIa12W1ZJJKJH75AX13TEfYAHYlN2vzNnQ3DZrwFyrgaH86RrskMaV5niNXoKcornEsHVwJVAczQGrfEr.S7yIN7PFmFUqVQ788wuhOEKTz.bIoziDibtttL3fCxt28t4EdgWfAGbPS5WIi8jwcB+axBVh4lW8Ue0by27My4e9mOvI1akXGyArrMcw1U5dddbW20cwO+m+yYvAGjQFYDyCOYkW42YGE11C3APiKZJghD3D5QbcLVXmwouAmjJjD4tWHw0dxfrx33iOtgekb4xYhv7xkKiRoXrwFyPRtvIkcTNKttNUpTjKWNCXkbcESRDuUIhXFismejeWz5wTTvriElBTOBui1tNRC2reNJZEIZpHZiHOKDOvF87F85K+d4bIIrr77FlNUoRmNM4xkiEsnEwhW7hMonSSMUMx3aokVn4latlsVLYgJ47IfIBOlCMzPlWiO93L93iaJu180Wezau8Ru81qYblXppnAmsIshGGgoCahhEKxIexmLeguvWfOzG5CcBc9FdDArrI00tF.UOBOORSZhRfNT0CLae6a2Xh2kdoWJWy0bM7c+teW969696HLLjlatYi1D1lIXmfuyjK6cBff3gDfCw0wPUdRNkkzN8OxnLVopbXUSGRjeerXwnPgBlsLJItorIwElthIHArosKksmjHSHDvG618QCePy1wskn.31me6EHlqxqFdwr6mhdNlMMifoivaosJN7PoTFyrsIbdlFWJfHx4oXwhL1XiYR0FITSDOtJd4sqt5hVasU5pqtniN5vjtNh450abmc5MU0TtB344azZerwFigGdXFYjQXvAGzDHo1g6hMXsb9s0fV3xS5ijR8y0ccWGe9O+mmzoSWSes8BAGO68v4jFVyDuEB.1gcRUSGWThVQhL5nixN24NYcqacr8suc1912NCLv.lTLQRKj96ueScmxNcQr0LYtHNZWBcCnBZbCiQ7JkXwKHGiVn.iUBzpYGvRFn344YzDJWtbSwOlmoMUoREiF.xJ01CnjIIBwtoSm9v3nytua1ZSGoiWy8uygW1fs+cBuZ1ZtMSl3DUlK.W0aBmL13Uil.R.8J.BhYzxXL6P.wNvWsCDYsVeXgOgnslPdtsyNRmNM4ykm7Mkm74ySKszBszRKFMhk3Dq50Mz.NUoRElbxIoToRlz1YzQG0TT+DMzk9GaOCJNePHuW9NhXGDrBunRb7coW5kxsca2Fmy4bNlykr2D.SWFaNdjT9iHfUTMmjD5MZddYqtpjvsgggLzPCwN1wNXiabirksrEdtm643.G3.FOmH.d1jY644YhbawTp74yaRqEgim4xDFGsKZ2.70Zb0NjP6Sa4cvKDFsP.gLMXZ8d3YOQShQGwk3RjPKjAWoREFe7wMl2IqxISbpTohg3TohGLWhH4WK.VQACitXhLY0l36nlYOa8Ixy+YRriUJ61isFVyj4bQuVx+OHHfImbRSZGIN0v1ap1+N4cwoH17.YmL51.O1.V1YGf8mY+ts4oBHhrHkjFSRBsauUikOe95F2bQMIdl5OreNZug3t3tVLeta8yw0dsWqghkYJNGOdRlybXY6AGA0VH0ytidfAFfsssswu82tNd5mdKr6cuaCIihJuxCegDznd6R1wVFXfALQ0qv+ivwS8L6rt.NZWzNUHTovIzg3ZOxkMjDoRQ+CVDsJ4L9ak6WaPxxkKaRv0lZpoZzvTVIahIlvDiURIfwtaVzhwtdqeXs65Xp8L8clsiaWdZpWelb8sCSBaw1QAR6IZ6SDaOxE8UzTYQdOZtHF8dHpo018chVuZ8zaY81o2hctPFEHPtWEupYmdXxyZ4kc1BDUjiGcAb6OWt2k4JBGVJkhVZoEyhWRraYWwNhZVn7t8eK0NdQSY4boTJttq653y9Y+rzbyMahMK6xhywa.XGQ.K6jrz9FVd.0e+8yl1zlLw.0t28tMw.kXxnD0sxkx1Uv1kNCavAohMN7vCahzXoZ.XOIz1d75sppBWB093FKNnAckhjOKzbKsPuGbrZzvplNFqA3RaWtV14gVtb4Ha1rFu.IQc7DSLgIvOkHZ2dU7ijIWuZArhdrYBfQ5qDuzJDYayKiVqMZfHZOG0s8R6yVKD6I3Q05Y1.vlsIl1fLx0SoTlPGQh9bYWqQFWEc7AL8huQ42xFDSDau0ZCLIsMa.Q4k88u8BRxucrwFyXZXlLYHe97zbyMabjhsGysArmom2NNNlfYUNlr3ZkJUXUqZU727272va4s7VLiAOdMvRmSZXItOMQhDlc+2m7IeR1wN1A6bm6zTlRjNznQcq7PWFTDc0ynp8ZaFQ4xkMqHIQ6sLnrdS5iZNfiiKZsONNwAsKAUljzoBIat7LxvdDvrOoOJOHRLBM4jSxfCNnQs9jISZ7diDMzRXYXy+VzAvylIgxD74Jf0r88hpggbLwr0ff.SM7RRkD4XGsRz6Qa9gjEvjwHQG9EUCEaGqX+crAODsZk5hu3XD4YU8zbSNlsVUQ6mpW+nLVH5hiyTeucnWX6UxImbRlXhILkgawKj14zps161.uQWzQ3vRzVTlmIUzzhEKxxW9x4u4u4ugq9puZy3viGAsNpHcub4xbsW60xF1vFLwgjvWks26.pQSB6iI.WxCe6Neoi1N0SfoSEFA.Ha1rl5qTM2Lyv.HkSHDpvQkjPsG5vBToRHgAIIYl5eNru2k6e4cInRkJnfRoLbVYugQH2mQSqD4dULwQNd8t9yzwp2wq22yt8K.G1ZMbnCcHJWtbMkPEwjKnVvmijDkuJ6euMo9R6P52D5AhF6Y1ZUIsqn.ux3LAzRFiHbLJ880SSKnVsohF+ZhFhQOVz6YaMqhJ1VAXa8fD7pCO7vFf1latYiGws6Cp200dQeY7n3fKgyL4yfpkRmrYyxMey2L2xsbKGV+4wKxbJrFDMA5s2d4c7NdGlxthL.2lGDaxZsCrNf5pQjcBjJoff89ZmDw2RnEXWdcEWLWyMTDssBCCHdLEAAPL2rDp8XxIGjwFcLRFuUZcAMMi+doMG0qSx8fTOpFYjQPoplWXhZ81ZVXW9bsm3Jm+Y65OSGqdGel9d1QNdPP0pnp3dcw75ndIT.rNZEa.tn.eQAzjuqjYBRM9JJeQQilbave4cYLp8hHBQ7xKYbY85ypG2S1KRUOy9lMswrIM2NqDDdpD.aeeeFZngXjQFwrQfHw4krnV8dNKuDMkjEFkELEMuruej4Wuu226i63NtCZokVNtCv5HVdYr0Z4W8q9Ubu268Vy1Ek8D3nO.q2pyylpyhs61qdJhLny1iKx2wVsV6AYx4KHPCJEtwfPcHnhwviLFJWM4ym+v7RkbuH+9nljXCHKAOn3lZAjRzZXlLYU..e0.PYOf0t8Z2uKfERMVOLLjAGbPNvAN.CMzPS6HgXwlpOQixAT3T6.cEDyMFPHJEDOVbz5p+sRA3BtNtnIANpXDONTIrBggtDRbbcBPqsGSnv0UQX3zSpkcmGwqvxDcocKiOfZIwVd1Xunn.9I7HZqoqM+S1iaixspc+c83latDGSQA2foAVs+8NNNlxtsP8g7Lzdbsb+aGXy064ez6SwjTouNd73ricrCdzG8QYEqXEzc2cWSa1dgY6x7rsDM1GeiTNh.V1lp7i9Q+HV+5WeMQZ9LoV4Qi5lylVF1DOZGzpBo9hlL1tfVdPI7FYGIxxpcRpdHwwic.6Y29iRzq7cjWBuL1OnExqsM4M580Q6wh1mVOyescNhvsFT0ru95qOydpm8uOTK.At3pldwB.h6V0je2oL8Od7jDKQbhGKAtS8YwbEdBCILzivoJJ0IhqQocHHPzvRQrX1w3U0iY+nW3fQH3WlvDsuLpSKhtffsm6DuSaOlUz1I5hIQ4mJ5j+WMuNRh.3JiYDpFDdoDSksshwVa3YZ7kb8iVerDyMO3AOHO1i8XjHQBV4JWYMimjEisKYQQO+06++FgbDS9YoQ466y5W+5M+e6zXY1TydtJyFGNhHw+j7cKVrnY.or5k8JS1aQV1CTEs4jh4Fb3EDPIusrislnfXh4r16lJR6T5ih1mTu9o4hLSeWaM0jIfM0TUybGYjQXfAFvDyaRaNZ6Rq0D3WA4nIRjf3wlNwqiEyEPlDBggBvghPcETJGzZEAAUHLzifv.JW1i.UEhEOM99UHHTSPH35n.bX5Et8qoOGnlnDWlfZWhfsAZp2jHIvbsoVnPgBjJUpZRz7YhWqYah4QyBwyEQFOFOdbiIZRX.ATSXOXa4wz.+ydbqIVkX+ajMFkQGcT9FeiuAu3K9h7U9JeEfoKD.hXa1cTPsiExbtZM7xu7Kyd1ydLwQETaLx7pUrsSO5pE1lkIZU455Z1nC.LoXQXXnwKK1NDvdEI6hZlr0WYGwv1wtizlj.tSV0wl6BQiMQiF6ZVtcZ27ZUlsyiRMcDQq0ZSan+962jHsx8qrhqz1APAnmBoR5yRlHAwSjfXtwPqEuMIIdqvIVL777IQ7zToR.flrYxRnNY08nnrgDp0TrXE78kMV0.7qLMXYL2Zqm7QIaVhFcQqKgf95ogQTMAjmSs1ZqL1XiYHiWLKRVrsdwW0rAdMeKNNNlwKBnkqqKiLxHlM41VZoESYJx99qdyaj6Q46jJUJSAizdQAou1yyi69tua1+92Oeiuw2f1au8ZVPNpUBhbrP6JXNVhjCCCYyady7i9Q+HChsb7nd14nUh5Ennhsmzh5oIGGGCnicoEVbUu8lcf3AO6zhPqqFDedddFOQJNRnolZhksrkYpFDEJTnFB3sU6elV4y1bhnxrsR8QyDHa2SK6tO81auLzPCgVqqouP.bkAfIRjffJAl9rLoSS5zoIUxjDyMFNTkypffP.Mwk5SlViBvQoHnhB2XtDFVsuohulf.PqUnvgjIRZ1+AiGKFNJEg9AlmsAgSWABrW.zVKP6zS4HQ1ssHffx3G6suLaNXmsmA0que9dxpM.jXgfzmXueUJJKD04U0yrXQrC5Y46Y6DIA3dW6ZW73O9iyYcVmkojgKK7ae9qmS.diTlS.VZslW3EdAtu669NrPRv1t+WMxQZPWTtKDstjGfxmIQ9rPBu8tfboRkX7wGmQGcTylEg7ckREhTv7j3PR.0tfK3B3RuzKky7LOSpToBCO7vl7ArdfTQM8bljijVSy0iIZPJQTeu81qw6ewhEy3da6zuwtOW.yyjICoRlh3x.zo97P8zopSX3TatEVC3QoHQh334UjffPbcSPxjoInRfgyJoOQ3JzMtqQiUMGdLVIsO6bqShuHkRYBWAwaryF2QxjRgl.4bYOdpdjsOS+8qWfV12+P0ESjJ1frwbHY7QzvApdhbbaPIwh.YgbAXT5OOvAN.qd0qlEtvExoe5mdcy00nm+2nk4DfkLg7m8y9YlxFqMmIudBXYediNgS.mDSGj+122m1ZqMV7hWLs0VaL3fCxfCNHkJUplvzPdvauoiJo0PoRkXrwFim9oeZFZng3bNmygq+5udtfK3BXvAGjAFXfZhaon7nUO9EhdeMW0vZ197vvPxmOOkKWlC1zFvUE...B.IQTPTcvCx3iOt431lCZWKkriMolZpIRkLIILaRqZTZ.kBkS01Xro3pSLGyyqZ9cpPQrXgTp33zQ64osValwGaBT5pgShqil.cEzgAn0AnTPhDtDOQLpdKDhu+zg8fc+knEss1UR4zwF.Ztv8j.XJwLnsI6xKaNsNR.SGMi2qGXZz4LQ85l88n.LK4jn77T79YTfqnNhvd+NP7bMTKGdxhAoRkBOOOd3G9goXwhrpUsJCP4ro0+ajxbxKgJkh1auct669to+962T9Trc07QS.FF87CysNfntsUzbP1i8JVrHZsly8bOWSg5e0qd0bnCcnZ3ByN0LDvKI9orMEQ7PUu81Kqacqi8u+8y67c9N4C7A9.zbyMyANvALkcF61nslV0SKqizpzGMCFjASR4JQLARtmj6K.iIERVKjOe9pD5ZxDAMNvT.UftZzfPPX.M2bybCW20ya8s9VYmO2yQ4xd335PPXQbbB3q7U9bb8W++IVySrdFd3QHdBGz5JnopIiNNNnCmxrOGkQaKe+J0vshbOYWAIfoKVg17qXGCd1dt0VhZBkMWdhV3x325YsvrABJuej.jNRhLNLJeQhlyx3VoDE433XbBQ87RXTvvn+eo+vdSrHZ7Pt4MuY1912NW4Udk0c9ssBKuQJy43vJHHfMtwMx1291MpJJEqL3v85Rzeu8+OpMwyzmKcJQcqs8mIlEIwazMey2L+k+k+kbO2y8v8du2qYkk5YNDLsJyQCpQQqDaOXsm8rGd1m8Y4zNsSiK4RtDNkS4Tnu95i95qOi1GBHmnwWzIZQiasi1A4QA3SlLICMzPFhUk7hyNfEk+O.gAUHQbY6WOID3O00MFUpDPr3IQghvJ9DOlhJ3QX4R71WYFdWuykvZ23NYOuz.3nRBU7Hcrg4+zGeErmm+EYvA2H9U5m8r6QIQJMPNPGCs1As1Akp5KzfxAbbcHVr3lRwhXddzIh1AeKfIo6kENq23J69K6fzUnRPdtHk3knUKV485MltdbVFcLr7d8.TrOOxBv0CrQ.VSlLIoRkxn4uvWq3fE6qSzX9xdLiMfr8hp17GJGSoT7BuvKvZVyZ3bO2ykN6ry59rQl+EUCOXZs5lOk4rFVNNNrm8rGdzG8QOrUCr6zORpNFEnZldXWuiIWW6T2Qoptcse1m8Yyse62NW0UcUbq25sZ1W2p28yg0IL0pq1oLhs4S1pdOv.Cvt10tn6t6lS6zNMV0pVEiLxH7bO2yYpT.xjqnqtIW+YB7rdRzeazia6Hg5oQhbuL8jqPRlLEoSmh3wSfiCfFB0hmDcnb4R3n.eOOnXazcmd7I9DmJe865myS8zGBM.9SPGYSve70dgLvAmj68deZh4LHW1kdprg0OBSVn5V.er3tnTZTJosGBnfoHkW4nLANpL3WtOscvgsFTB.Szrbndhcel8yC6IqBPuMY71etce9Q54Qz2iZh1L8rc1DwTNosUnPAS9FZuUwId2tddwe1tVy124.G3.7DOwSPmc1ImwYbF0j8J066aaAyqVqtlMYNygETc.8O8m9SMpRG0SEQcQb8V8w97A0u.uUSCLxDe6XpR7hxEdgWH2vMbCrpUsJ95e8uN+a+a+alycTD9Yq8HlDJSZDNwjGPx1LtTtlW4JWI4xkiO5G8iRu81Kae6aul8XQaspp2JPuZToN5jjnd2zteSVgVZCAAADOV7opfDU2xwBMqBVsMVpT05c1UeU+d7gd+e.5sucw65cVgb3wt2pCs0TWr7keZ7e8+5+e7m+YdW7BO6FYsO5KvBapSV1x7XYcmlW34SwUb4eHV1xWJ68k1MfFkho.sp890w0oFugYWivrMmKZefXR3QZE75owtMfjjz5hVKRFUTOmlLSSPidcr+tGMlFdjtODqHBBBLInt8BshLSl1VOkGp27OaMxbbbXjQFgG+webxkKGqbkqzTHBjEYreFXqTgc+v7kbD0WStf999bVm0YcXpFJ2z1Szmql0Hm+50QVuAYPsgRQ4xkYQKZQbS2zMwUcUWEOxi7H7+9+8+aSIrctH1ARora1XqpMfY0sff.imI2vF1.CLv.zau8Rmc1Ieyu42j26668RwhEM7DYqMnbuLSSBms9JasQi96jxqqbuXeb6pCgA3x0kXwigiiZpUjU.JbblNZziEyEsFJ64SwfWjS5jWH8r8QHgiB3f3DeHFZrAYvwKx912fTtrKgg9rzkjgcsq9IYhV3RdGWAmxo0MnpfREBD.0noU0qq.taGI5hDUiQaQtmiNVYldISjh1+GOdbSkCUq00rEcEEDx92UuiWuWhLSsqijHKRKZOkJUJ5niNnkVZAeeeFd3gYzQG034Zax3ipXP8.WiBRUu9rLYxvjSNI29se6709ZesZ1KQmIsyr8J87oLmCbzvvPxjICm4Ydl7hu3KdXAFonJ3qkFY8d.ZeNs8vmzdt1q8ZX4Ke4LzvCw+s+a+2.nFsMloDt09+Kt001qLBHbpToLqnnTUIsUJ3aUpTg74yy.CL.c1Ym709peMFZvgXyadylUvk1fMOZ1ah.GIfK6EErOlzeY6ldo+yFDNZepiqCtNBmNSm9RUCYgpZjNwDiyu5AeP787HqSEbpDiAKWgmt2wId9jrmd1FOym4SRt3dbgq7jnrFFehgXImx4vi8urcd1csO9h+c2JECNHMkNOXeKpn1+OSuuLJAg6QRDmILWW811Txn.VxD874yaBskImbRS0dP981lpVu90neVTtFmIYt74x4RdllNcZZqs1nRkJlxpiVqMaBrx2sdswnJBHe25wAlbLozkWrXQ9e9+7+IG3.Gfu1W6qQqs1poMYCPJ0AOatSmuj4L5h7v6htnKxnAQ8z9YtnJX8TUclzxJ5Ms.Xn0ZNkS4T3c7N9cIVrX7.+xGfm9oeZSGnrENcjjzoSa3APFPKUN.wiLBIs4ymmErfEP5zoYoKcoU+9tNrfEr.788oqt5h67NuSNoS5jnToRl1Rz9k4BuB1R836a19sxftnZk355hhp7XIlKGFTM50iEykPcEbioHSlz35FmjISiWX6ry8uWdau8lIY7rD5uTzAsR5LsSoRwYa67U3pduWL25W3SxV5AdldJQ57IwMdAxmqUPGCvcpWJPKlGqAlNAlitPW81q.ru+lqaE6Qm.FkSIYLFfI.WEMsrCZynzcTuyU8zrZlzzZtJ1b1IKDUoRERmNMs2d6jOed777XvAGjgGd3Caiec1Dasnp2bR66wBEJP73wIe977K9E+B9y+y+y4EewWrFGgXq0p8B1ymxQ7IdzN+K4RtDyl.ZzGZ1lFJR8.hNRRzNunqVoTUyWty4bNGSGyd1ydLdTQhR25UelhJB3GT0s+czQGbZm1owocZmlI2tjbPSzvZAKXAbwW7ESqs1JmxIeJlIPJkht6tatsa61He970jhO18Mx.j4R6qd8K18OxDxnDDKRzxDS0fjsLdd9nTNnYZGZn.BmJbBzgg3fKAoRv+9pGlE2ZabkWRdZN133Vwmwm7UPkRS1rJdmW5am1WzYwZV+H3lXgfxmjIBoRQMhGB0ZGzgQcTS0qcgBEL7hBbXS1rCMCQrKCMGoW1IObTwt75HOqkEmEOxYqQbT9HmIyDEI57jiVIZdhZ6Dnb4xQas0lI2QGe7wYngFxj6nGoqmsVm1VwDsuGvjlaB2tadyala7FuQdpm5o.l1q51luNWo83nQlSlDJo9gVq4LNiyflZpISZNXy8fslDGIIpYN1GulFXroq66Zs1TFh6ryNoqt5xbNjjf1t9VKl2cjt25t6tos1ZyTy3ylMKSLwDru8sOSP0AUqRjISljK3Bt.thq3JXQKZQr7kubyCxQGcTZqs19+m5dWhQNttOezupp986ddzyLbdPRIQQZJQRQQIJQIZaEX+O95XXGeCx0KrMRPPV3jkAYQPRPBPVXDDfaxhrIHAHH.N.4FuvvvKrSPrsbbhhcriiskrkhsrkjsHEImgyLcOcO8ioeT0cQyue8u5Lmp6dFNjR4.LX5GUW0442462yC9fevOH9DehOA9a9a9ajCpBJVgdBxgMmSY1eYF7vr9ZCrJveHXTrXdHYxDv00CC5OP7OJ.FRKwgSfKRgAn81OH9K9+8Uw+2++bbT9D.atUADGcwrEqhLoVF+Y+Y+83MuoK1KnFbi4g3wRiNMagzwigdAwvHY.C..YZRwQ7EqdocFTcc2lntDbaRKJ0V0Ra0QVztKA+Nt4j9T0QCVYSOUQIovgEnhE9boB1SjHg.h544EJmVUudcTqVsPVUzVwjDhMvpnz+UPPfDUEu9q+5329292F+I+I+I3C9A+f2yzaktLUVITaotLYxfuxW4qfexO4mX8T3PqvN9dyu21qiRleZQH8Du74yiG4QdDjKWNL+7yCGmgIBsuzW5KA.H9UkNehq6v0VVw00EyLyLXwEWT9NJtaiFMP850kEPttt3BW3B3i8w9X3QezGEW3BWPpSzGYdi23MPkJUvEu3EwK7Bu.t90udnHCf.Wlot2wUhpOiEdLtyEjrNYd+G94tnW+92QfLf3IFp.de+.v3+aX9sxeXb94.3GaOraWO789AMwstgK5tWFTslCd0WyAe8u65Xql6AemtvyKF7BR.Ge.O2zvwOIB7ah3d.tN9H.CCR5XC+.Df.r0laIlnm.BZvJ8lNr8PVPLMYa1Wo+abKfFm3Yj0LAGz9XmdyGyCgDVOMEWTW+sUrIEg92y9FSWkgLdzgXFOQo0mtU5MKIwBxVyj0n94aCvU6VQ0qWGesu1WC4ymGO1i8Xghf.yh1MKzG1vGjxAJA9w+dkW4Uv+8+8+89ZvlURSJ656WTOqwMABX3Nlm3Dm.qrxJnvcNm3RjHAN9wON94+7eNt10tF50qGxkKWnyMQSe3h6BwAtBEJfd85glMahM2bSr4lahc2cWTsZUzqWOTtbY7deuuW7I+jeR7deuuWbricL4P3jCd7vnfGXpyO+73q809ZnYylg7jZNvMsYzgntFdun9xztx.AtzJ1W+7oNG30j3N9f0.eeff.354gDIhOL3nc8fm2v9tVsaip0pgc1oAZ0pMfWL353BWmQoflgRYFf.m.D33OjWk+v+mHYR354hVsZisqtM50qeHqbZJJAGe3XGOnIzYriIM24tov4HZESqGK0AWbT5HKpxjVaLM+VcH2PFpz3A.PzOK.BU+A1uWvaxXzTI819KQhDXu81CuvK7BnWud3Ye1mU1.mLCAFM2iRKcX0u0TmsFzKz52uO9BegufvDwrimMznB2AcGtIBdTHybmsjIShG4QdDLyLyf74KHA4aoRkvYNyYvVaskHJGsziVGEZk7x5GOIgYtMuQiFX2c2UnUetycN7w+3eb7a7a7afqbkqfJUpHAlJEYk6dQ2dHa1r3zm9z3+5+5+B+re1Oae6zePVbMN.KtPlA4LAnHqTaz+0hLJGuUNL.kiA3Lz+r50qG7bFpfbGLLyh535L7cNdvKVhgN+IbgqiCbbnPe9vG8Qe+9vy0CttdHYxTHQhjnW+AXm50GlcM2qMPvnnYvjcEP3C1UW2g4CsBEJHrqNn.UGzqmyS.fbJzn86IsNJsUeF2yRKNlMvfoQbRsdnnwgnnr5XuTmn.r4321tuSp9CDds427a9Mw0t10vy7LOij2533p1sKHC0Ci3iSMfEEUx2eX944e3e3ePzajYCKrhUCGN.5AI80xhsNH8msxJqfkVZITpTITrXQTpTIg59BKr.tzSbIrzRKgFMZHclTbMsHBZGpjGQTr9kLYRrvBKfye9yiOwm3Sfe8e8ec7Q9HeDbpScpP.UbhrY75wDjFAL95e8udj91yzTlDfEYAnSuvTIn19M5MRnSSNXv.zevHlNd2w8GbgCB7CP.tyBCu3vw0CAX3ew.F5o79Cf+.e3iAvwCHV7gA4bxDIQhjIPuACP0Z0v1U2F0qWenH1ddvUMln2gm0QN9wMAxmOuz+NML.FWe5zNFn0MiNFGIqFJhkd903F2FWc5fVGMmOqSk3zghcbFZLAyn1PanFymyzBXw5.A1+9e+uOd4W9kw69c+tkzcjFfW6hOGF86M0mZNDnghd7A+feP7M+leyPH2l9FCQ80hhn0UgohKGWg584hW7hXgEW.m8ccVQN80VaMYxCU391auMdsW60vq7JuBdi23Mvq8ZuF1d6sEGCj67vAxUWcUr1ZqgSbhSf0VaMb5SeZb5SeZL6ryhrYyJwuFaec5zAu4a9lXlYlAyM2bRcTuykuuOdy27MwG9C+gEkgZaw4DGjlfHgwiGGMZzHTH5PkypYnXFX2..9Xfj.+777P7XwQ7DCiSuDwSf3dtvyKFb8bguOv.eeD.W33B.3BO+dvw0EtdC+ywC.vA2IjAQ6F6gF61.sZ1B852KTcftnhtXq9xw4BEJfLYxDhs+jJGEVpRKtMSAQzgJoEE4y5vv5CX7t2y3LNkM8T555hc1YGb6aeaISqVrXQTtb4PajoAM38iqQOHNANuFZk9Nc5fG7AeP7W7W7Wfyd1yJthAiBDNmUaztosLUVIzFE2KbgKfuw23aXsxyNYJ6Ov9cRR8fKe+jJ7nzZ1YlEUpTAau81nRkJX2c2EyN6rHQhDHa1rHYxjnXwh3jm7j38+9e+X6s2Fat4lRlmjx2GKVLjKWNjNcZL2bygEWbQToRkPGgXZKG466KoDY5pCu5q9pxoRsIEWOOOToRETrXQQ48ZqnZqO4vT788Q5zokMBX9SB.gXCnsxl3Xod.twFxjB.nydcP2tcPycaf3wRf3IhiXwhOz63cbfWrgaJ34NTmWddIFJJXP.50a.1qYGztSGr2dcP+96g98bw.+Qd2drXIDihny9n5hIyPdXipOfTGGnfte7fL+Jp6kdSFtAb2tckyiyrYyFxr9Z8aMIwdzhTZVmipsXqP1SbiUlO+2XiMBkZZlYlYBEP9lLB00kokckVrPN18Zu1qgememeG7o+zeZ7DOwS.fghTyPg5v.VAb.7zc8hsACFfm4YdF729292tuIB1.sLGzzCRQIBnMQEmYlYPhDIvC8POjvVoUqVBCG8tDYxjQngVtbYb5Se5PhuRceQvHccy70zsNzJNLd73X4kWFat4lXqs1B4xkSd1ZfHl1au10tVn1tlk43Z2SpPPHlrB48oSmNgV3q8yIcIlWbzePOL..tPESZCBvf9CPe3Czcno8cBnNcbPf+vbYULu6j9n86C+..3neF2Q+WtNv6N94Uu9CP+A243gy8NG+ZJegRy.iVCjoAG8Xxz.VoKGl9Vfv5ng.PZQ9YdBi.altHwAoXtItFT1Df1T43ZlQbrtXwhne+9nVsZnYylXmc1A.CyS7ZIiLsL3Aon06nl0TPP.d0W8Uwu6u6uK9C9C9CvG3C7A..jiwuCaYpz5EApzAa4S7DOgbDYwEf1Dwicj1.1l5J4c.ZJUpDRkdnHfLQBRkp644gLYxHGQ7j9N0iD0wiNVD4.KWnX5yU7f.fhB666KwoHcahye9yG5PHk+WeezJ5VOo9nxOrz9+FCairYyhXwhsOQVXgVpo+fQhjEbm4p862G8CFLTbwfAv2u+v+ig8Qc2qK760Gd.na+dvGAv0KFhGOIh6ECwb8PLGO3BOw2tz4y8DwigDwiILuLyCY.PrDHEIWOFbPX.X1WcPK5XyS6BKtttHWtbvwwQNLT30..qaTOo5jd8CmyNthdsm1oN4b6986iYlYF4bNrSmNRV2kfuiScCSyebyRfQIXQpypzoSi27MeS7G9G9GhO2m6yIeN+sSqOapKSU5kgcHTYdNNCib7m+4ed4fovLCNpa379XyL6SpRq28Z0UWEm9zmFoRkB0qWWDcKQhg42os2dab8qecL2byEJTaXGKYSs6t6J53gJzUqyD97dgW3EPxjIQoRkj5Bv9iUJd.Av1mFH7Mey2De9O+mW7lac61V3SXNA1zjz7ZzrI05CjE1mP+GSeJ.w9iff.HNjU.jDrmL4HVLLnuenqAXj6e5iQL7FBZN.CcHTeLvuGBv.DyyEtNCcTTWmgmygCCMH+gu2M748GOfbKUpDJUpTnEv5wyCxBs6lhdtJWjIf82QuLL1RMCiGsd1z5DRulvbsRTJ+Np1nY8S+8Djmt.BM9DmymOe9PyC0wpaTtbiMcrMt953wiiNc5fu9W+qi3wiim3IdBgf.snoongiSBrItE.m.atqV73wwYNyY.vH8gvc5MimPycNzUrIUz4eZdRKeqacKA.k5TPqPeSGgSua0FarAdsW60P0pUkNE8IyRud8DGY7XG6X3Mdi2PXZwOm96BASIKONfSvJeee7xu7KKmpObmHMHCKlz+4+MmnaBNMtc+.fbhqjKWNQOKj1NC5acfeqm7n8OJ8e5hIyHSw5r8c5qgKr3FCUpTAUpTQRkOSpLIF.59yCSwrOmi4z8FzGcXsa2VNSA0aNaBjcPYHZi4kVGTQwjSuAU974Q4xkQhDIDW241291VuuZVR2sEBp666i+7+7+b7m9m9mJyizhGZK1as0+LwZDmfp0+C6ntxUtB969696jcczLTL0kEYvnydnSCfE2kfulIk+YlYFQ7J1vn9ELUlMEGLd73nYylX80WG4ymGYylU.ozhN46OL+Ve7iebr81aiFMZH9UhVWP5cwHnEeNCFL.qu953e+e+eWRcy5I772EEiTccwbWYVLUHrtn2kMSlLh6XvCWC9WTi474oEWSOFOohY3BQeARylRbZ0DIBcNBFkgZtaAdz6XePTIglsjtOvltXXn7v9bf8CpeXJ1zuEPzmBy78jIchDIP4xkQ+98wsu8skLTKOskzFMf+VZMcy64AoPqFRoE9LelOC1c2cwm9S+oEiW..ouhgnUTOqCjR2YgC1W5RWBEKVDUqVMTnln2Ewl3VGjFNWXSZ2Drf5VfCXzCn0N4pevnWyEdYylEEJTHTdOxLdxHql4laNr5pqhc1YmPdxNe9rcnsbEEad80WG+nezOBe+u+2WtuDXzFSynlvYaAlswBa2KJF.Y8QVpTLFFJG59GfvKvX+RTLmz6BqGmIfI0yiVTNMaMclx..gXJeXXiLthMQwlFfKS1Zr9FKVLwqx0hVQ1BD3xT+t2MsEy5q4ldluVGUEwhECyM2bHHH.0pUCsZ0Bat4l..nXwh6SWqZ0CDUcdRiOZUI45Nzy2+re1OKpUqF9y9y9yDe0R+bzyELYONQ.K8N7ZODG.Xs0VCqt5pXyM2beUbaCNZ5wSpiv74666ilMaFZmCsr1bRTpToBIF0fACjii8tc6hhEKhie7iis1ZqPl9ejdXF97X30rxJqf23MdCzrYyPsCBBne1NNNnVsZnc613ZW6Z3e7e7eDas0V6SGLldcOum5hFLTqnY8B4nz+mtemKVzgHAcKC1+POdm43K8FDlfTl5FT6XfZm8TKRDAm3bGcbbx9bs6WnYkG07ioEvwF39Ak0.aCZF1ixiXiDuWGlL999Xmc1QTY.USfsM0mT8ebWmozL7y3605rz2e3YevLyLCFLX.pVspD1XNNNg7wM.64y9ooey76z9mUpTovd6sG9m9m9m..j7pEOCDY+JmiYVlpipdaUNtn8bm+b36889d6awiMQ9zxcOs6xvme+98w1ausvxhS34yhCHLXY87FlYMICCG2gKtxjIClYlYj6E02.Q46OnOh4Mbmys1ZKrzRKImJOMa1TNgQnUBoXkdddXyM2Dqu953VqeK7U+JeU7c9NeGQbT.Dxz7ZVEiaBoomLaabXRSdzhCo28h65RcAx6G6O4BTyOSO1VrXwPOGMSJW2Q4sJyMxHHL66zmt1rtp8Day1zgobX9cbwt92Z5HzYylEdddnUqVXvfAhZI3oRdtb4jqIJqlOM06IwlwT2l.iDiWKoAsjLYZUudc4ZYlXvTUD1pOSymywWpKK.bmTzcJ7k9ReIjJUJ7G8G8GgRkJEhgeTt9vTAXoeMmLQ8Vc0m8p3e7+u+wPz2zrFLaDbghoWwGUgLRBBBvVasEt0stExkKGJTnPH1VLDD3DLMCqff.336HN6W974QxjIk7FDox64M7nNZPrAghRes4zYaiWKAGWe80wst0sP61swW8q9UwW9K+kCYJdBLPq1o8x3wU3DPSQV0iGliSl+dSPKd8lNgodgAedT2B72XNIVy9SeMiqXaNktMMt6wQgngGz6mdrRmG4YXCQP4ff.IF9nN651sqLOiLslV1U.i2GohZ9iF3xlKYP1TbsHyJIdddRbZpCUsI0eMtOyLwCpW2O6ryhu3W7Khie7iiO0m5SIIMS8gd6AVjP9CABqDT9PO24NmPyiettgZtfg.c5qcbE5aGwiGGsZ0Bu4a9lhouKVrXHQZn3NrNyAGZFWGGGwReISlTLoJEYhVlh0SeeeTqVM49PeYoc61Hc5zna2tX2c2Equ95xm+Ue9uJ99euuO777P850kcKzTs0ICsn7eMVzhRpYfD03gYQy3IJcRnYcYt.wTTT80oA1z2KayAzskwUeMYhcP.+rUhR2eGjB6637KV233HCd9hEKhc2cWznQCLXv.I3660qG1c2cguuOxkK29DyeZJ1TuhsOW2N43.miSlV53Kb94mGwhECUqVUNUymat4Ptb4hz0Fr8rh5yzGWdoSkFvILHb974we+m4uGqs1Z3i9Q+nxl4zq3228N3fzqYor2d6g2+6+8iW7EewP5EQuSo1zqZ591lryhdgBAj..JTn.Vc0UwoN0ovJqrLN0odXIFyHKFsnD5DkOEuwjMgdGdSJ6TuNttCyT.bW0Z0pgpUqhNc5..fe1O6mguy246fe9O+mKfdLLdhpsw5hoiSZxzQqyKBtpmHZd+OHE8NXSytm11kebe+jteGzxQMCKc62DfkOOM.U1rYQ850Q0pUQ5zowwN1wvlatYnjm3fACjC8TpzcFf4L.t4X23DOz1F5Gz92Is7lFNnVsZXmc1A862GYylEEKVTD2WO+isQ12Yq9o2bSSNwwwANvQNQw401qWOL2bygOym4yf4meOJP1wC..f.PRDEDU98QrIT8crslonjLYRb5SeZ7C9A+..X2BLQsaIYXXqnGrnUF3NUW6ZWStmMZrKVc0UwpqtpjpXotYXt5QCXoU5OqSlK5888Ew8bbcf+fgSB2XiMjTPK0g11auM9w+3eL9o+zepjEBzLxrwpPKdllkh4jWtytIaGcrfcXV.eTun+d888dYwlgKz80bCrb4xI9vDsJXiFMva8VuEpToBpWutXRd5VLTDQZjitc6hVsZA.f74yKy8s4ZJGT8bEUYRiIjIHut50qKF2JHH.kJUJTlVvjs+9.jbFYrGds..NvAvY+.Z.Ck3XiM1.+U+U+U3O9O9OVtFahDNUoWlIU1XiMvW4q7UFKMeahfvFVTEcimgZ.YXzpUKr6t6hToSINAGsJSr3wDS1qAmzNsHAVHPHAynhIoHdarwF30e8WGu5q9pX80WGsa2F6s2d3l27l3Ue0WEe6u82Fu0a8Vnd85Rn.oc5NsHYi6OyAQMHEk6m.TZY708Siaxo4DJaiESCCKa+FaiuS5YcXJuc.HR+XiFhgpnfFTfASe4RkAbfjBuoE4ngc33F02J2TUa0UfvpPwrLMs+CSeTPPf3tK.P7QO87MBDSRFZ.I8qEWVw0y50osnt1iC52uOt90uNN6YOKVYkUhbN8cMCqACFfKe4KK5zQWL8kCcGDv3ygzrQYpfY9c6s2d3F23FX6s2FG6XGCqs1ZnYylXiM1.oRkBkJUBkKWNzQ3kYm.u2c61U7KKtKXiFMjIn850C85OLF51c2cwN6rCpUqF1c2cwt6taH5uZGhbb6PpARMEKYbJZkEJRgMcgXxfLpm8gsLNcKcP+7osbujUno5AXw00EYyjEv4NG5EdtnSmNxhOBN0tcar9FqiYmcV333HozEJdUylMQmNcP5zoQ1rYQ61sEUIjOe9Ppsf0CSV02Ka+TunISlDyN6r..RlMQmC7LkDfV1MzFuNtgD4S2+FEHjuuOJTn.ZznA9re1OKt3EuXjNO5csNr.Fdpm7rO6yh27MeSYvjLMzhzn8VdVnBoGGfEuOLgj44M5T+ktYP73wQgBEvxKuLle94Qtb4DKcvcH77FALLXvHe3xLnZI6LdF00nQCzrYSYhFs7S61sQlLYr5CQlsSaCVlWK0+gVw5l+FVX+rNfsG20eu56Nrrndmh3i54dZcyv4LwhEShTgjIRB3.4TWl+dFpKYxjAyM2bRL6wB2DjheEOdbr2d6gtc6FJ+zqYYMokkZPVae9Ao8qelbMS0pUEeULc5zRn8P23gVBUjf3N5lRKQgY+q4Zb9etNf8G+k+k+k3wdrGyZ64tOXgvPSee5SeZ7Zu1qI9OglcjlQkoHfDPKJeyRqzYFSRZqBRG7D.X6s2Fau81R9.hYkTdNCpmDx5EEUj.Qc5zA6t6tBUeJlW+98EPJfg.sYxjQhGJskXz5VZbJ011jM1Wnc9U82SVUZJ0Dz07dcTnCDcccZVLbTXYu6mESvAJNOYlu6t6JY4.W2gGXIz3RDzhon650qiae6ai4laNQrP5WVdddnQiFhEEodinnW.C8SO87n6ULjMa+Z2Qx2eXFIoToRv00Eas0VBQAF5aoRkB860GC7GrOQ7X8abpZvlwF5zoC777P0pUwW3K7EvEu3EsN+8tFvhc1W8pWE+K+K+KgVPYpDQ9eyc0hROVlVsgN1F80E57llmTHCFLPz+D0S.2MPKWMPXGxTyvgSXz4Ga.HSlIvb5zoEOC2bWEsmrGU6ybmxnT.u9ZXeqVomSqYxmVfmo82ZiI33XG9NIvJcg5pgL2C7CjMw3gSJ.DmrLc5zByKlsRxkKG1c2cwFarAVbwEkLSJY4mMa1PIPRBpQoG.fvzJpMbNp6+3lcjUYPPfjD.mat4fiiC1d6sQqVsPsZ0PrXwjyz.dfRv6ioHf.1CcLSKwqcehff.7u8u8ugM1XCToRk8UeORXX455hqd0qFROV1l7ZahrlMBGjzMXR8jMTJFHuV5xAriSGTuzjs1hKMy5mIpOuFFPtZO9lgYg1oVsQ8cZbFxwUuhB.RakF12LoMHzk6U5E4tAL7dUYZX6QCajLQRjLURILvZ0pkvpfh9uyN6fM2bSL2bygxkKGJ6t544gb4xglMahqe8qiEVXgPounjISBWWWgoEyjp6t6th+ABL5TKeRa3cTz9APHl5lyEWnxBhDMc5zAau81BKKczlD076nr7IWOCbm03dwjnXY80WGe2u62E+h+h+hG8VIjzZWYkUvW6q80vq+5udnEQ7ZzUXMCGa9mkM4e4640wvywrn84KSFS53VSyhR+r0gphNMxvr1HAHoCDRQe0Ths0GY68QAfcPzkAYYoCFb1uMs.Hii99jZK7yFGf7A44ePKlaTXt4TTF+PesTY3T2Ras0VnQiFglmwMAYv5VudcQMCZoJ36YteuXwhgVbpOCAoTA7v1kVTjLNnyapqu.i1HTuNyz0HF2b.83UTVtyyyCtNt.NCyXusZ0RXT1pUKTtTYTnXAwWC4bUsdqMAn3lr53.lRlzs2PPeJ4T+A8wG5C8g1231QBCKZNzm3IdB7M9FeCITTLEoyD3xFq.SwjlzjYF6SZFR5IHbxf94qum1zwj94655J49cl5P3NuLqdp8Vc8.0c6BwnrxpYgSPLEObRVpjkCRetM8pEU8ebe+QUwb9iIqVcnPwwF1+vLvA0SJOd23IQcqVsvryNqHRGWvkMaVzoSGrwFaHosafQ5.hLjZznAt4Mu4vLk6chFDp6UpCqc2cW4rJv0cXJ+lqeXZMh2WJgAGmk3j0x3gs9eyMVzjFBIRmRA5.CAYqToB50qGpWuNRjHAt4stIRkdHyS1GqIjXtlj8O50mbLPK8BI.8ydieF1c2ckTONK20mqz5ILefOvGHTPLZy5U1PdYgCJ19N9605gR63cZ1QISlDoSmFYxjQTxtYhpiJf2TuVlKvnUJKVrnL3Pcl0rYyPLZhRORQ0dLKlfciyG0z+FxTT6+WGT.yoAXaZJQwz59UwTrZ5hI73LifO4ymWLg+d6sG1c2cEFzbNjiyvruAU3LGiotRiEKF1d6sk.6kicjcNOskpWuNZ2tcnXNLd73RzYPi4nE8jtLi97lzT7KS0ZLIlT5+X+R73wQLuXgViDKdrP590ya3goxbyMmX3qc1YGw.W54ZZO9W+moTNZedjeNAt..t10tFtwMtw9FeuqArXEsWud3BW3B3QdjGYhhiXBLALZ2Qa5DZbESJv1DOPOvNoNPar0.FR+ldROvPSUycjsEL2iq9GkHJlu2z.Ai6dqMIu1QSOpKiaL4sCfJsXRreUmuszhhw3Osb4xHe97nWudXmc1A6ryNR5MgfFj0CYZwHlf5sz00UxgTas0Vh0p0ee5zokCBhpUqhVsZIVTlrnn+MQWlIUpTHa1r..h0I4yV6w4DLFHZVTQATIo3GugfQdw7BAPw+30QQTqTohbNNDDDfM2bS4LK.XTjjPWsgVWeP+vNrsVkLTLS5PsTQ9MZz.u5q9p6e79nZhC2w387ddOx4OlICAsHfZlDrC2DHSSyzD.QeOYQS2jcX5qy1BJtSiI3fMprLWiSZr6s2dnQiFglLe2VL0+RTfV1Z6Z8nXKSLX6Oa2qwU2zudb2q6WLszsYJdNWXvSaXBVkOe9Poy3adyahpUqJgUCWvx6IOKD0AuLWHGDLLhKJVrHbbbvlatI5zoivzh0MdOBBBvN6rCZ2tcnMTHvWpToPylMQylMQlLYjrQByvHTTQNe0zsWLAoLmWq8.c40wF85P5txMrGpC.IszL+7yKeWylMwsu8sk1hVI5Z.q9C5KQSB+yTGx5TKiiyP848S9I+j8MdeWqCKcmluuOdrG6wvBKr.t10t191g2FiJa5PRGyRSyyzTWM5AOccvl76Spn2k1yyCkKWFsa2VDcf4HKsR5sc+mFcCYq8p2gSeuMaubP2wwIzBuIUrwdabWyj52teyxRq2P8hXJxOO.Q3m2oSGzuWezsWWIaqRFAzhfLDZ.FcpdSEsSOSm2SfgJktVsZXqs1ByN6rhq2..wuln0.qUqlndApjcpOzff.wgSYlwkVPjKvoCQq0oksMLBIB4chiOSUeXtFS+dMwBt9x22GEKVDoSmVNkfXaFXjAfXaRKwDGaz9lIGSXxhjqeHP2Farw9FuOR7CKsuCs7xKiKdwKhabiarO.IS1QlV0xjoUTKTzu1LhtMYvY66zK3somMymAeOUnp15M.PzOA2scbE8BLMaN8yUW2MYYpqOlff5ID.iRsGSSIp5ss98IApc+rnCQIfgs4zoSib4xIoCnff.ztcawixaGzFtCFFifAAirnGCyJ5nmzkDnHZTM.zhhz5gtttnToRRJZQK9FvH1Itttnd85xygLgoRqY3ovC9zRkJIfe850Sx5sT2W6SY42YLfgGiMcdYxbx17b87MMg.J1LOXfAfbdGlISF3OvG9CFkVmrAXYNG0jcEK999RfhqK20xvP.ChtVpTI73O9iKoa0wouFcGitC2b2RSZu5ho3hlCfQIxG+slx3a9605TKHHPNq2XfsxH3m4waS.mCaIp1y3.VzSR.Bmu0GWYRfUQwL8dsne1Z6l+wMLymKOVbwEwpqtJle94giiC1YmcvVasE1XiMvMtwMvVaskr.LlWLQDwb4xEJuUo8qJ9GuVFxVtttR1YfJKtb4xv00E2912V1DCXT10fLsn+boYCxwadxkSvQd9LxmCCSLtdSDkyUEGqdtg9Na5uJp465wXtNjhww4VruHHHPh6198G546c60UbWAcBEPqpFSU2.LZyUN2kQVhY4HggklEUwhEwwN1wvi9nOJdgW3Ej3qi5VHHXT7JoQv0LpzfCLHH05nQSQcR5MZbIgroQmSjhqFTjm.I5SCj50qi3wiKA.6zVLEuks+P6XZ3dCZEMyqKJQDSlLIZ0pkrvPeHZX1maVzSbMuF1+aFun19856gsmgFblsOBFYxDkumhgmMaVIjVn3c6ryNRJRgVaKVrXXiM1.862GyO+7XutCyrkoRkRbs.NN1nQCTtbYYCMlv6JVrH777DqIRKhmHQhPm0eL9S888EQ8X8lm4.Ma1Dat4lxQGO0KJYZQ8VEDDfBEJf74yKouHdh2TrXQjLQRDf.jNyPCDLn+.jLcRodMt431HNXBrnmSRFVzfBjAT0pUEVn56gl8j4ygEaRYwMDzpXgk6Z.KsqE.LLzBVXgEvYO6Yw2+6+8kE05I2lVUyFKKciT24FEqs6UknTrYxjIQwhEw1ausbsLJ84f2QYc0FvjsmgMPCpSFBdqAFhRbSahBpeMet1zA4jXXZxf0zO1z.U999xlV7jQhYxSBTvT9Cy1mLWnQQwBBBDK9wvKoXwhnYylhkAYp4Me97nUqVnZ0pxg0PxjIkc+oqsvmGAjX5WgA3bmNcDwlnudQi.v4GMa1D0pUCkJURdFbSX5VETLPZcSlEQn04RNWR3.GotA.Ij0FG6WSUmnAnLAtzFPi.y5y1.xnhLIME8ylZZLkJRGCtb9fMIDNRbbTVXmc4xkwoN0ovRKsDdsW60B8foOw.Dd2Ua56hSXGm712qKl5IheFUnaqVsDEkxCc.tXZRJ8dZ.yz.1iiwi9859UN4Rm+4AB6ro1.XLY8nGOzO2nBRWNwSuCstXZLDNwWqaFZ44DIRDJTPn9MpVspzmSShyT5BsFnqqKxmOuzFRlLIpWuNBBBDmAlFof8GL9A2YmcDGWj0eJdHYtALDPhKxH6OxtfdLO8IPsKO344gc1YGznQC..A.F.RX7nAG4gYA0mTiFMPxjICcBNyzcSTVV1FngoK8Hu1O.C7Cen2pSNkr8PeST6D2bL1TeU1XXQ83ommEDL7X1yrbj.XwAABrP+X47W3730e8WWVrQTXsk2zLDrMwOJwMteUzLaz6FvIlEJTHTnFvS+2nN0ONnEMySsX0Q0mDkXa5HO.v9owionnTbW9Zy6u981.eMmrZqtx1BS6JoRkRNTUo9V51sK788kz3ByfFzUD3hFxBSaQOBBxuOe97R7.R.PsXx.PNZ24Qfk9PYfhZQCuPQOoNaY6lYHjs1ZKAXi9vDWGP1eMZz.MZz.EKVTXpwwnRkJI4dMJxIyK7sZ0Bas0VvyyS79ddnzRcnEE.k4+CAZcGfJSPLsEq0Y8j81aOzoSmPOOMSY8y11q0.abNtiiCdWuq209lybj.XosTH.jDa+iewGG+qes+UTsZ0PGDC5c0GmdNzH0.62hf2O.wrwpQuPrToRB8epinFMZHK7lFWK3fTWFmXlQsiJEAm0Op.VyIN1te5IclOmn5+MA9.FYRcFKcTuMzRczsPnXQ862Wh0tVsZIVWiYNC5I4NNNHa1rRJegJltUqVgDMTabHJt0su8sw7yOenMEcccCwNiren6DnSsQkKWFau81hXaEJTPT.OWbSKpw6CAl4FOLUJyDCI8yOBpEDLTGVZmzLUpTHWtbv00E6t6tnZ0pB3QpToPwhECEL0b70T2Rl9AUfe.7C1uiTy1hF7hfVrXdFVpeVl0A87sfffPdGOvPqwlMaVbtyct8M25HAvx7X4Ie97HSlLnToR3Lm4L3EdgWHzNcT2GlhKXBLEk7tucTzKB0K1odOzYIBt.SepeXBLOssCSw7LABlVwCABGX378Zw00SnzhzYiMmt+v00U70GxlihjPE+Rl0lhpvr..CyIxlhhnQGwjlOGXje5vjIGqODTFXn9gpWut3Tl5B8QKFdITY5To2ruISlLhmtS1MwhEKjUDKVrnnje.HVzi.Bb9Q850Q850k6K6uYrIpAsJTnfzNXBzKQhDBSLBhw6CStjrtQQe4qsYcNsU+LuFyWSoJX+r1xdZFtZCbYCXROGSCjo8aK1lOyYNCVYkUfY4HUGV7g644gEWbQr4lahG+web7RuzKIlnjULMnj1Y0Hnm1YP0zEueCXoWraV+3.etb4DE+RPYly4o9Sh59NMsGa5wRWFGSGVzY0ARgWalaMXBAgXD6GUljzzRNDPUONwIy5DgX+dC89YtfgV9h5Gw00E4xkCyM2bRflSQtbbbP61skiY8kVZIYWZFVGj0BYkw1FvHWLfreXHyvfQly033HcdSpWHBdv5M0gDc2.x3ihmRFcLWYQOlm95EAn44DHiSUxfh5BMUpTRPYy5Be1NNCcHVto.O7Voq1v1qFzxFfkFLQ+dpncfvV0ihuQU7vTAEG20fPQwth+mWG++ku7kE.Yc4HSjPfQfQCFL.Ku7x3EewWDm3Dm.UpTAW6ZWSnxqCxQcwFyCScbc+tnqSZ.KVu3NjEJT.sZ0R7XWpnzn.rl1hoHfSRrPy5L+MZufue+9gVHSQmLcBS9aI3kFLhul5FBvdDJvE1Tg3.iXgQQd3yk+dJ1GCfcMPT+98EwHqWuN1d6sQtb4BkCvIy2BEJfpUqhc1YGL2bygfffPGRDNNNnToRXyM2DCFL.yN6rhATX3kQGBlgvCcLUpKKsSgtyN6H8qzZhb9NyJt0pUSXPx+n3eEJT.tttheVUrXQ46oNpzd9NcN14laNbyadSQL350qKr1zAWroHf1Xdommy4X5MRz9ZH6u45dMnD6eMYSMo2CLbyhKbgKXci3657gk4BXs6M7i+w+XYx3O3G7CrpaCf8GJMZVM5FF6zLYfMt6qYwlhkGGKGSJtZ1f79vcQCBBjD2O.DFWbAlo96hx5mQUOzOaahGNo1M+OWvRVMTrCM6WJlFYKRQQ3toT7MxrnUqVRFrHHHPVnQcPQwpngIHPOm2vbKESNbTWfjogCBagR1NpVspHhGvHPSsSaxmGAk43HaqIRjPzOF2fgJQVqLeJ9JE8j+dNNSqwxjFIcFUMvr1QioueoE8lYqABZQftffgV0jVCk5qia9zsaWwHDISjD852S7YL.HLfzLqzfVi6OV+zrf3bP5E9TI7b9MKQwvxT8Cb9LcL0eueueOL+7yuu4wGYYqg8ciuitrFLX.dnG5gvLyLy9prl.R5JuMjWMxudx68h5+j9NVz6tjKWt8QikAzp1YHOJDscZ+8lSdzKLJWtrXX.pTXRom.HZlYTOLZPbsBY4NvLmRweegBEPgBEDQXRmNM52uOpUqVn1RxjIkboN2.X80WeXvk6NROmDbf5GhLaHSPBlR1PYylUhiO8lh5.ctPggIit0WecDDLxUPHHG8epff.4.GksWpjdOOOwMHnqVnOIxIHKcCBlkGnH4lQnP2tcQ0pUEQCiEan24yT7BYLRWVg.ps6zV7H+.+.Ln+f8wzxDvxTjQyWq+SOexbdno9uFGPktPrf986iSbhSfG3Ad.qymORNWBME+fSjqVsJt0stElYlYvst0sjSUG1PLc5PS1L1XPnY5n2scR0qw8Y2M+V8mQcmvfpk69p0AioEUmT8W2F08OSq3w5eulUFWzyOmAzM0sBWzRVOjgA2MksA5eQzWo.fn6DcxSjYMABpLXvnLtIYCo6GnIxIyLJ9jdWeJNKM3AeF51IAaGLXfXTDBFxEbjkCYLn8Rb8FLr9qsTIMN.myy6MAOXNZm2esKlnMvfN6gRcYQ1ub7lN4JEWk4oK.DZSCc6wOvG852KD.k901.mLesFjW+G6SY+k1O4Lu1oY9J629k9k9kvG9C+gsBHdjxvx70Kt3hB0Ud1EBLxZUZ5h1p7ZqHZpGoIIJ2QUaZbeFqOTuPjNK8jYNYhhWw6idWpCR8QCjqe+zVzFNfaXnikNJ5Fqi5CkAxdf5pgl1mGMSDnKa1rgx+47ZzhSR1JwiGWN.Z4yT6mYzq1o0yzfOZPKFNTUqVUXrP8qwI8LKJPWdPKNNAb3wHe61sk5NPX+IiWCEGVq2NdcbNPPP.pUqVniDKpL5rYyhYlYFzsaWo+j.xzGs.fX4OlH+hEaX1ukFIvzXHz.G7XDSqCQSEva6OMiJs9nFGyKVLYiMs.U50DwiGGO8S+zQ96NRXXoK5c2bccwq+5uN5zoin38abiaDZG.yJt99.LxEGzrw36uWvvJJ8qY65rAjxcg4jLZIH.DxBSr8G08ebOaME6I0drA5a9Lzh1w5LUvsdxD8fat3g5wgKDoEinE1no1YeBYWxmYhDIDCUnY7wmIEghmdM.XeGvlj4A0UVud8Dlc54X74wSMbxbTC3v6EYb566K.C5XJU6xACFLPXfpcYGpuJpSSsdunN6H.JCeMMCT8lch0UuiGkyPPh.XZfdayOMYSYBnXiIj4mYSWTDbj0cBJatw5zN2NHXnqi76+G76ixkJuu07.GgIvOyJC.Dc5vNsm5odpPNGn4uwTTPR6jMFyIpGTFFSa89fbcrsQQCnUrzwSHvHekwTT3Ca8bZqql5KS+aIvIEIIa1rv22G0qWOj3R5b8EYPwCYVtnGXT.UyDlW73wQsZ0DOIWuX.XnnYkJURxRBz74Z88AfPL6X7+wmkmmm.RUpTIQ2Xr9qWz533HNlIcmABVw5EcoBFbuMZzPpOZ1LzYUYH7zqWOQ7ONef5qhL1HfNWTSGjzwYXPZy9c1mWtbYQMCjkj9vfvTbPBrnsJaTLqroepwIVnMfKtI03lSpGCL2j275FLX.VYkUvZqtVjqQNRyo6l.HdddRp9nSmN3AdfG.O3C9f6iJYTrZXGjd2Cds7yOJJGD.fwccZq8vPnfVLbvfAxw+8jXFFUYb51ab0IsUbs8GiaMt.i4gbB.y9Y9ep2p81aOgAg4FJLrk.Fcrmq0uA8QI5jw7.Nf5DQKlEOzZAfHRmdtFA.xlMKxlMqjkNISPp+IxxpXwh..BPiVGQ74RwjoADnHvjgGEqigACshJASAFNGMa1rRX7TudcYSBxXRG1O79PqLlJUJ4L.T22ZJppMkgy6u9j3gL0LUztM1V1DUz76zFIPOmzlJKFGaNca3xW9xhQL3FcglOOUy5ODE1AtvBKHSTRmNMtzSbIYWctHZb5nwl7v70GU.VQUlFPAstYXw2eTpwEXz4LGOnC.FeZu4nndwBmTF0um.pzBczat04EJd85f2UmWm.FI5tNMuPGljhGpYOw64d6sGJWtL777DE+yMnn3kTzLFrxTuPjMCAQ50qm.znyE5b9hqqqXzfYlYF366K4qcc1Vf0uBEJHAnLArz9xDCD5jISJ4ecNmj.stttnRkJHYxjBXjVjXs5S788E2DAXTvZSvZ51BbiDs30l9XkNcDaS2TiCfxbsl4ugeN6CroKMVlFIg3FNttt3JW4JxmaK8xbjqzc9dtKvxKubnJ74O24wpqtpLvoo9aidNessNZtSkdgftXJVvAUmVlLZFm9j3DGMU1hEKhRkJIKb.friMWfqEElKVzlaWWGMqCSCvUT61oue5EB.PBMjs1ZK42vEHzGtnHK862WVPSqKR80PmaLSlLRnxvIm5I6zoMY3xnYkpGmSlLIJWtL51sqj9b4hW1uQ1QYylE0qWWrXKaa7Y533HoXFlwMzy+nnazWfHHIYpALZNdgBEPpToP850EQVYciiykJUBNNCOEk2c2ckzIimmGlat4BkcXa2tM1byMQsZ0va8VuUH1a.1cTa8XqVxDSqBZBJYJNolMlICL85TeeegMLcKCSfyoU0Gr9WtbYb4Ke4wRD4dFCKV3YZFomlNcZbtyctP5fPufjcPrSfE8BN8BW8tClkCKClCRIpcTX8k6.yImLPo0otX1dzhRXput6kENQiLZnng.CSNajM.cXQZ3.5GTT2J7dwqirnoN8Z1rYn7GFuur8QO8dmc1QX2QFcbtAYsA.r4laFBvWybgokFMaLNWg0MJdHshqF.EXj9+HCPF.xjwLetzy1Y790rYSAvlhmoYhySpGBJRqqRKC1qWOI8w.LxJcSqnV72LIEra6y02+nl2oA2X+9gcslt9dxSdRr7xKuO29QWtmCXEKVLr3hKJcNttt3IexmTRNZrCUGhGbvQqvcM5s95L6rOnLPtWVBBFFjpbhHqObWc80wuWunR+c2qJ74vE0TA7oSmVxI5z60Y8QyVgJWmghDvnTDB+OAYbccC0Ay6jZ...H.jDQAQk+v3XJA1nKVXxZi8GbG6LYxHwVGSRdzJdD.fYygff.gokNS2RKxwTRLScMZ+DifjLDgzdUNmeRVOoSmFyM2bHQhDxoiiVWSttiRJezZeL0BmLYRL2byIrLIaJJxn47dM.Fmun0MkdMkVk.lh+o+rn.9roeYVGXg5IUeM5MRll4f99CO.arIFntbOGvB.XlYlQZT6s2dXgEVPRcDjMEestwpav5cJXQKJk963u8sS.KMs3hEKJJylJalSV0W63lnburn0Chd2MJpC08FEGjrqXckJwl9kjNuHwIxTI7ttCSYKzoG4hetvjGGWZFFbrTK1MAT2YmcD2BfhHoAHJVrnj1j0hspYGv7PkVjNdcT77hEKhYlYF4dw1odga4xkkqgVGUWmoNsBBBDqKpE6EX3bY8QhE+L87dSWwvbduo3clh0MIlWlyMhZtBKZojzkCBfE8+Ja5bUWtmCXQJ0j5uu+vvl3xW9xnPgBgD+yrCzriXbh+cXWvaaPh+9oUGQ1DKjkd85gLYxHhwPlLTmN.Xe6Nw+eulckt9RVETzUx1hwVGi0NVW0rGnhgCBBjqiLOH3EMDACKG8APKWXxeCEgrSmNR36nULKKkJURz6Eya6.HDvK0gFcPScak2K1NMAs3yk0OJ1GAS4bQsmqSvNsR3YeISrkTug..au81312913F23FX6s2NjQDX+KuGlh0xwNVLk7vFPkdL27OS88F07eSKDp00rYcZREx3rb4x3Ye1mMDSZaqyuu.XUpTIIohwXzZs0VCO7C+vxNJ15rMUZ23j61FCrCRw1.+g42Z964DgBEJH5EhC3jogdBhMiGbutvMM3hNd.OPFgLE7VqVMALfiOzgIoU7XJKVO1wqMHHP7iKdrZwEh55PPPfn6OsaLnAyIvPwhEEw9X1JPKRG.D+phYhzff.IDZHyNl36bbbjiVdclTfhbUoREIybv1IEooSmNne+9hnbc5zAUqVUBwFW2gYPTp.dJ1FC4Is6gnE2y15CSwCI.ssMfsI8Ae8jJ1teAAAg1jw1FtSagrrO9wONVd4kCA9Y6dceQjPp3cNHvN2m5odJjOedYxf16u4fEaTl63XqyV+atewPYbOCsnNTAuTg0.CSzeZqhoq2l5X3dY8Wydg5cRGieoSmFoSmVNa7zhDvIXNNNgNhpXdexbmXB.lHQBIvvY8P6RALDT3gUKOK+XeAEsNc5zgNYk0mIdZ1f7..kYTBc+Ma+DzBXn3sZiGv59byMGVbwEk5EyVA5mI8qMJ5G8GLZLCFz45McIPKEEj8CbdfVsI59csqXv9wn.slzF5SR7P80oC2Hc87vrYuiiCt7kur77L0YmtbOGvhMbpPRpLxlMahG4QdDbhSbhPVJxTOVrLoAD9a0e98KQprULYYoWz.LJaMRQnLaS2Oq6ZQjhEKlDNH7zdAXn0NY5JQqz4fffPmTxjkxt6tq.DqsBJEahmHyTI7Dnj8M.HjR3oxtY8gJ7122WXP0sa2PdKNmzSG9jG7C5i9K8lJ.P7fclpkAfDjxzT9LesySgG5iVD3gr2nH1UqVUzYY0pUwVaskDVOrcyE7ZWaIpMsYeJP3z2B+81XDcXKlpXwjTvciEBAf37rO8S+zgtu2y7z8IUXiYokVRLaNsHTmNcv69c+tEm4SS+VWzxeqoRyIj5AUSKfv+OocMNpaul5hiwnWoRkDWDv00UDafKxzVMSGRHZOVOpIHZwJMsbi40n+iLCz5KYvfARteBXT5.1wwYeoWkfffPwHoVDRpiG5Q4zeiLCKGlNX.BaHl74yK4JcsUH0LC888kTFLEUi8C54RwhESRGxUqVU.Jn+kQ.lxkKiYmcV47ETqarZ0pIIHOfgN9Z0pUEFG999x4SImG33L7.s3F23FnVsZhni51JcpU1Wy9W1+oAnLGuzFJQ6CX5way4mluNpB6G0DI3XEW2pMfgodmrozd1uPBFkJUBW4JWYerGsUtuHRniyviEn4me9PgGfiiCVas0voO8oQ61ssFnyiCfwjFpFfRqXx625DxVg58IUpTgN1o.fXhas9ghR2A2qJ5cR08W571MiQPp+MNYlSX0WmNmWoAAoaSvbEU1rYkTwhMPVp2qzoSGJutCfPtf.AJiGOtbxEo0UEEIiF.Id73XyM2T5mIXQtb4vRKsDVbwEEcxQG8k2KsHb.i.xZ2tsvNpToRRTdXSo1rXtAmsOSqWGsnRSZC3ih48lRJnmaZFRNQolFdezRMoABevG7AQ4xkCsgUTk6KhDBLbh+xKuL.F4YqDc8IdxmHz0Z95ntub2Fa58wFs0wsay8xhlIXhDIPoRkD2b..RpJQGJIlfsGEhKNs6nZ9L0gPBWrqO3Hz5zhh9QQqnx0M0UFvHVajssNqlBfP4HpBEJfDIRDR45j8Ai0NlVjAPHcnoWXS.UZ4PZsR1lY7QR1fzQeYF.EXjZKxkKmLGj5ii0epDd8bQ83m4my9Qy4nZKDpe1701FaiZ99gsnAYz8oZcXMths5gdytm7IexP9ekMlZrbeggEGTWXgEBcfXxc1N8CeZr7xKuOmbabENfaJyNWvaNQMJpo1d+QISFahHlJUJI9v3.kNkDqGfY6YR0oIU+GmXjluWWmYb1oMV.Yxv30ifGbRLWzxrLJODGzonElD5bbFZEYsGtaFal.CAM4w5NUtN.B4Q7jAKEWi5WRm7+..xmKON1wNFpToBZznQHlQ6s2dXqs1Bsa2VxwWLMQSQfbbbjryfdwEyPnqu95XiM1PrHoICKN+zD7gumf.bNt1kFz.91.4LYvXVtaTMB00GcHUcXNMthsmol06y7LOSnwvwc+tu.XwJJcuA8mEDDLLFhdpKKC9SKkWMvDK1jQdbfVQ0geTpuKN4i5TwwwQRUvZ82o80on14R2tzsgnp+Q88Qsqm4qoyGx7cD0+D8qLBxp8YNVG09wkICRtXh2OcZVgmvRjcA0SB0A0fACj3bj94E0CEvH+3hwsoddfmmGxkOGlc1YQkJU..DGiUC3RiCPfZlhX30QOoWe5D666KIAw81auPhPZptBy4sbtroEw0V6zFfk43kFv6nnnAE4XBKGjmi45vffgVZrRkJ3BW3B669E0889hHgTohIRj.KrvBxh.tnw22Gm6QOGJWtbjfKi69GEyLa.VucoOKcaE.xIsCqm.HT3ZDU87tYGRccwVcSe+4mGOdbgECy2SDjoXwhRJQgimZqMReRhh+RqtQFJ5COUlFVZ0pEpUqVHQC4BlffQoPFsyax5N24mmzMkKWVBCJ1G666KYvTxHiIEOFxP5MVHHniiibFE53LzENRDOQHu5mfILe3ywaywJNmUyBh.YleFqOrtaSsG5qUOddTOWW+L05vaZlKFUevC8POj.5OMR4beyJgrr5pqJgn.vnb2yLyLCtvEtfUwArcO02W8jB9ZRg1lYdueBZYJZm98EKVTThMvvcunOGwIz1Fn0rmNr0mnJZ.KsnIzpUbAbPPfjBiouMochQs+YQ+3hwan1+rX39.fP9wEYxvmsNbYJTnfbN+w7rEY3QeoZ0UWEm8rmUbECs22SPQldmAfH1GYVQPWB7vEqz66eq25sv0eqqis1ZKYrfsKBrzqWOq.VlVCS+aIfkM.L1uGkmfqYTOIlJ75mTwbcF06pNCMLtx3jtw0cXbEywkwc8rbeAvh5pB.nRkJxQJEqbddCSIuO0S8TxDXacDQ04ncJNaVNbbhEd+pnE2k8GLmYQKP44M5HwRS81rdeTpiMa2ecQagOJ9JigNeee479i9tE2vgohEfgKLYbDxLUg1IIotsXpVIVrXRT..LxoIAFkSnlYlYfiiSHwz32yEUL6fPWcfwRniiiXoZNWbvfQm1Ob7oSmNXwEWTzIFuujQllMo1EIXaVqyNVzy4ixsSXwTeNl2G8lV1XXcTTz5ci8K5bfkVT2n1b0FCd96ehm3IBceLAwMK22bqA8qevG7AEmOjtleud8vRKsDd7G+wCMHaa.w18kKRzcjZwvrMwIp6o4eGz1o92xmioHv.CW.PKFR86DKVLr0VaExarISGSQoOnkwU+zShLCOBZ9ZtfjGEWrelYdipUqBf8yffrtnyhpuN9LXFe..RLlRklqi2vLYxficrigSdxShyblyHNiI0wEAE2XiMP61sEci0sa2PwpG6+0dXN0gVylMEWofweH0kjdQK04kICHSKmp6KrwlRybQa.CsSjp0mklwmFzxLzdz.AG1hFHgrOYjanEQ0FqNsn7DbmLDoTUW8pWUvBLK1.zuu.XALxg1hEKFlYlYjPSfMdfgMzG+webTnPAqHx5OSaUE.6gdfMJy74bufokd2FVlzyglhWGNG.PzkkV7.f8qH1Ca8zVcLJ8fnW3oEMjWexjIky0Olmm788CkX.CBBDePqWudXqs1JTRLD.xY.X4xkQkJUD8ToMDAqC99CC5ZFapL8JS..ZL.lNmYL6wCOVpiMZUQxxZvfgYQ0M1XCbiabCr0VaI.AZwgbbbDPqnzSkIa+w84liE7Zihw0jrl1Q07aSFTbyGN2PyTTSLf.lZ0ZnC18986iyctygb4xIF5P+bhpbeyJgZQhVbwEwbyMWncD3qe3G9gwYNyYBMQvVivDzgWeTTv0fb72eupsx+OM6t466K40bFtF.Cy23LSE.DNjK3tTGE6dZ1OXqeVqKkACF.OWOILW3D3b4xIhzR8EwIqZGBLe97xg2P61sE87PlOEJT.G6XGCO7C+vnXwhgDQlrapUql35AzWnnuRQkdyqMHHPN3QYtpuc61HQ7DhqUP2W.XTt7pSmNxIFcpToBkNc3Fs54b1TEAYjFEfkVGP5MWs4BC7ZHvsYzfv62QoEB0Et1QyjksGZrCZQWfvVSj.a7+L1JuzktjTm0LR0L+MK2WcqA1QlJUJwIRscLEc0qdUjMa1PNMmoBnMo7poIGU7TomXcut8FEPq40Qc8vSyEpLS.HgwAPXlUlAB63JSRz1IM4Vq.ZV+bbcDuYGXDqqYmcV4DogeFyRAoRkB4ykGqrxJ3AevGDoSmVDMTG5KzEB51sqb3TPklS.1tc6J4+c5TqTo4L7czh4QFTDDqUqVnU6g.q2912VpibAi1wVY+sISWsH0QsQnop.3mQfLS2UvlqgXCvB.R.WaacwQMfE2zRCzpaeDnx75LYd0ueezrYSjISF7rO6yhO1G6iIeOAm0hU9NB.K95UVYkPYhS1f62uOd3G9gkD7WT2KyIOrwpArrwBQ+6teVhB.ieFiYNJBFCeEcJcAH7jfCqXglfX1zam1xaL7bXFbf0Ylen3Yr2JqrBle94EeJC.hOlA.jH4vSG5xkKG5.KkNt4fACjLXQiFMB4U85StFfQLNouRQ.pZ0pIdBOc3UJ1AaqDnZ80WW7f8ToRERWR59WlQFza5omKaNmRK1lt+kWKAy0yALYiYCbfyo0VYMpw0iJPKsdWMAz42Svd8IYsqqqvdl+1RkJgm7IeR76+6+6i+5+5+ZI8Rw6oYc1176X66StGTXCVqL7xkKiEVXA7y9Y+LwMGnNE50qGtxUtBdoW5krp7YVLEowTAelJ+CHb1f39QwF.kt9RPZdRtPlCj8PiFMDc+vIqrcLMt.htLNclLNPLO26XkNGD5YlLYRYgNO8mme94kCZApeHS8GkNc5g9wzcNnQ877DO+m4NpXwhIhdt4laJ.PlhcPGZUmxo0GOTu0a8Vg.zLMf.Wfw9Za52iGUVl8e5P0Q2WZNlaNWz1mw9Eafc78ZvCymilgxQ4ba8ZJS.K9L4lGbiMN+kqoWas0vEtvEvu7u7uLN0oNEFLXfnBDaGLt51sY49Ffk90z73m7ANIt10t19Tb4d6sGN9wONN+4OO9O+O+OCsSFQzA1ep0fE1AqU3qtdvemVYe2qJQc+M2gldFctb4BkL2XlqjNYI6Ctap2QARoE+1AN.2YXSqbbsIsox2ohw4DvLYxf50qKYT0LYxHaVQqglHwP1VUqVUDYjKX0hCkMaVIqOPlSZcJEKVLTnPAIdEGLXfn.dpakhEKhBEJfM2byPraX6vLA4QFt54WZKxo6mHKBc+J.rxHwTbIy4iZoD3yxF.llwssw1iRPKBfy1hV+U7Yn2DimjN4ymGGaoigOv+We.7rO6yhEVXAQ77BEJHgY0AArB39.fklcitiNHH.Ks3RRDwC.g9Osf3UtxUvK9hunfXaSrJ8.roNAz9EiYc5syhMVVbxb4xkQ61sQmNcDymWud8Ph0nsN0zVzfTl+2bQndwj4t1TrPNIsb4xBfEaaLikt2d6Iimrs.LzOlRDOgbR5PefhG9EZkvSwNI3sqqqnJAF7xjAEaOl5awwwQ9MZWCPqmMsHWDHT65ADbhaxocrWxbS2uRVS5OSuo7jVnZJ8f48lrsGGq4iphM2WvDHkJRe1YmEW3BW.O2y8b3IexmTRgPKt3hx7.JQkt8XZfgn5CtmCXoefbRk9XOe4kWF+ve3OTRTaTmGddd3jm7j3BW3B3a9M+lgl3vqKJp3bR6d6sWHK.ALxBLbBmoNB30.LYSGe2TzOKsnETA725V2B.iD6nVsZR7qYpCDy1t9+70lhSy2645A3DFXR+GAx3eT46Z1IEKVDarwFxwYV5zoQ73wCkn7XpLVNHN7GHJCmmhL6ryNHSlLheXwMxRkJE1d6skTQi938x00UhcvM1XCjKWNYNBA10F2f5ZgmfMrey7Dfl86bthdSF1WvExZqkpCQIfQy00ohF864y1zQR0YzSV+MspXTiw7OaK3iZtH+8biP9d1d3ZMB1vqg8MG6XGCW4JWAuu226COzC8PHUpTHQhDX94mOjWwq0MpttOs.u2WDIbbkkWdY75u9qK6FosVRPP.dxm7IwO7G9CEk3RYkAFA9LthFXxbvP+ZyE5S6f8jJS5dn02FUna1rYQwhECcPlxvZgdDOYJXikkMfIa.Q1DEzDfh9Hm99vuieN8GJfQrHXVZXvfg4NK5c5LkXy3QjfHbQIcOgtc6htc6JGU6lARrd7hLwH6L.DZND0AEUntVjyoc7Kp4YQwNZZ97nDs2lwYz2Kaajd2HJnVBHc8hfjTupZPWpu4yd1yhm9oeZ7bO2ygUWc0gyERl.YRmQjTxb93ciqW7NB.q4medbiabiP6HBLbf6QezGEO7C+v367c9N6SjuooncMB.HJYk.DZCALoIm2KJlCnCFLPxFmsZ0RNLDXF2jem4NnSZ21P+AG35MpOwwYTdKm6.pMouIHmo9WH.K0egiyvbEE8ird85g1saK5UhYDzRkJgLYxH+NlWvzd1t1OozLq0S70J7Uate11n3plLkLEIwFqE82a9Y7+z4msIlmtNZJFO+by1iMkaS12rcDEXmtLMfBlFgRqOMxzj5ljhCWnPA7tdWuK79deuObgKbArvBKHV3lVk0ltA022+WKfU73wwJqrBVe80kNNRGlCTW8pWE+3e7OVbLPBzLMEZpZt3ybG5woqq6GE8jRJRQud8BcrrS8BwS8kzoSKNCodgYTrlnBzswvh9VkIPUneeD+A.Aj.XXH2zsaWIWvy7Kkm2vi8ps1ZKA.g0edjxyI2TOmzZf999BSL9bzK5CBBD8fYBrvqSKxcTa1Y6y0.MZEOa96ziiletIXfoOXo+LafFl2K.HmrQli+bb8fTzmHPbLirqHv3YO6Ywy8bOGJTn.t4MuItvEt.d5m9oQhDIP5LoQxDIEqDy0uZ0Vn6aMCkoCZ4sc.Kfg9j0+y+y+CZznQncPIqh2065cgye9yi+i+i+i8sy5jJbQfdgfFzRuS4cCc5CaQOIk0Oxjfd5csZ0D.kc2cWwk.3NfZFjbRqm6PG7DX+hEpAknqBn8RZS.Kc8TqCEJ5GEOv2eX1LHa1rh91XPQSenR6brjMICcFSVtDjfKjzSz0i8ZKDZVWMESRynPOGXR.YZUUnYFEkuVEk0.owDzWq1AXABePoxeGeF172K86m14iZc2o8ARFv4rdLyLyfeyeyeSb9yedTqVMjMWVL+byKh3mISl84HoTkE7dqed54pGl0bui.vpToRXlYmA0pUKjGUqO4be5m9owK8RujnD2ChXaZ4w05xxTIlucvxRacDVG3friiClYlYjzdB02yt6tKRmNsDLwlh.553BWuQdDOAoFGSJ8t91zGhdAtloByNBddCy3FMa1TtFMnpFHTCZPcf0tcaYbQOlvwaxTlsEc8QC1ouGZcbpMXSThuEkH154OlKvroqJaettcYiMkt9aBzpknHJW4wF62nJ5eKWmwwFx1Z1YmEG+3GG+h+h+evhKtHZznApTohj5q4goBqe7+zm7LApHIDMogCS4cD.V..OvIe.byabSYxmdRd61sE+x5a8s9VQpTSaE8hLB.Zhve2xx5ton28lh0neM0Ozsu8sEqI0rYSIMKSq1XpTbMfDE8SqmJS8QYSQn5czMYVoW.ZxRfKt4tuTrP8uWqLbBhEDLJiexwHlWz4t2ZPUc8RCVoq+rnqu72YxR2lpFLmqYBrDEfilwr9yMuVah+YpWIyM03+02Gy16zV3ugrgVbwEwYO6YwS8TOENyYNiL+JUpTXkUVQxWYbtHkHhrf61sanSaIMqdy4YGFPq21Ar3.4JqrBJTnfnmCaGyQum2y6Q798oUrPNXycn0hVb+fU0zLIhhIXpOM9a0YNSpCO5qVz0.rAXYxpZb5kJpIRlh4Pwqo3PTWGzcFntU30wI3bmaBH0qWOIVBIXjoWOyckomua5qS72pqiZORmi8ZKPy+ax.6ft3w72EECK860hUp+La+dS.KsXWTuS5Demse2zTXN6e0UWEW9xWFW7hWDyO+7HWtbRxkj8+reTakdc6fLsXlGg9No1vV50aGF.12Q.XwEgOzC8P3V25Vhx6Xij67s5pqhG+web77O+yiToREBLB.6C.S+c.PXYYRK0j5s4NoSp9yhsIcZ58QQiW2Ov2qOBkRmNMle94wO+m+yQPvPKh0pUKr0VagYlYlPJKk9WD0WD6GzhjYJ5fV7Dcck8GTGTDDRCXw5NAsX31PSgqiqS9L33hlsFUpdxjIC0up+Ns9jLYESPRsX951flgkM1jDTPy7xDvPOFwEjzZlr8455JVkjuWOOkdQOK52qkDfgpDeOuOsZ0JTeTrXCO3aoyyp8Hcs0D0iA7ySjHAN+4OOtzktDdjG4QPoRkP5zoQlLYPoRkPxjIwst0sfWLOw+.0wKnVTbMSQ1N0iA1.qsslYRk21ArXgG9jjMA6Dzw6Uud8vktzkvO7G9Cw1aucnIm.go4ataqdQIUpMeF5qwbh5zVrAVoKS5doqCgXJ45AuXdgRKvLndYpVYkUVQnlS8JXRA27055mV7DMPAAgzrqHPCuVZEy985uuElrcaab3f1+BDNRItaJ1Fa33tFT2DXVmUIrs4i99Y6yhp8RPO87Q97ymOuDP4m7jmDqu953ke4WVpOz664yfQE.sjnFvlJTelYlAW7hWDO0S8T3XG6XhxySmNsb3n..YcXLuXgRTAbyznr3mdMktuvbih+WICKNv34ML83t3hKhW60dsPN1Gmz666KVL74e9mee.VrvciroOCstrz4S7Cqb0Sam93n7aS7LWGW33NZATtb4v7yOunOHJBIcIfb4xI6ho2IyTWBl0CM.DWHZ9Yl+Wyxh+FSGz0zyrOn8oQ86hp+9fvD179LNluZ1ClLh0.Y1pW5MHr4CXbtH8yL9YjYpm2vMp909090PPP.JVrHpUqF97e9OOdoW5kPmNcBkmtzaFSFP.PLXyC9fOHt7kuLtvEt.le94kyHS5U557aOCuJ19XlakqazwpqYenIHFAvLu9+WKfEKdddX4kWFW+5WOjCSBLpSue+93IexmDeuu22C0qWWVPqEUPuvW+YrnW3YNIzjk0jJiSGFrNvu2DfhuV1UGNg.o3ebR8RKsj.PQ2A..nd85nXwhxosL6mzKjL0eilEJ+uIfk12ln3O75HyKcept8XSzsCRwDzxFnhM8Do6eG22qq2lhPpG6zpkvltaLue51soKMPIEzhGq+sjUJMlByXpyM2bhnlyN6r3S9I+j3K9E+h3q7U9JxbCs+zoyU9YylEOzC8P3we7GGm5TmByLyLCyhF4xhhEJJAlNCt985tGbcFV+XdBitnBaqZWORSJPWL+btQpswxCR4sc.KfQGw4dddnRkJXlYlAu4a9lglHwNfc2cWrxJqfKdwKhm+4e98INmMlU1.szNnodREedSpXCHvlXO78lLbrYkN8q4jONQjSZpToBpWutjCpnkYZznAJVrXHqsY190+wqSK1iIaJticThCx6CEamWyA0z0lLOF22adMlhZE0uOpqgul.wlgXDaS5LnpteUOVpY5n+bxbhiK54a56A6C0mr0yO+73zm9zv00URayYxjA+J+J+JXt4lCeguvWPBasjISJ5ncokVBm5TmBW8pWEyN6rXlYlQTjNE+iFDgseVHiOFQBzoPICda8yQAbo0gmFv5vvtB3cH.V5Rtb4vxKuLt4MuonrQp3U5fhddd3c+te2369c+tgNC6llNAsBX05xxF.Df8E.QIGtMcCw+ap+HxnxKV3PewzhddddBnZ73ww7yOuDacz0F3IGCYAPk1R8KYCjgKDAFIJA+Lsxx0.V1XMwum5HIpIiSCPzcSYRLvhRGRruWaALZnA.H4xKyilN8+sM+Qq2NdslVBkeFUhNGeuwMtARjHAN9ZGGW5RWBYxjQzE07yOuDmkuu226C4ymGe0u5WU9MqrxJ3wdrGCm+7mGUpTY3YnvryfxkJG57RD.n+fgJMmo6Z8XnFnlaLpWOx92n.pL2fvzO+Nrk21ArLEcBXnmu+S+o+TTsZ0PM1tc6JonhEWbQboKcI7u9u9uBf8GSTZw6zOK8q4BNS4q0e+jTx63zEEKh3cNtfYFAs+QoAzLcCAshTcccQ4xkQhDIP974kfNtb4x3UdkWA0pUCkKWVDiTyRfS.0sasC+ouFBBw9UsXi7yzfe5fJl8CzNB9yD...B.IQTPTcQ+gAnZR5vxldSX6XRwolICASwxzikTTKFD2lfVlhfx5z3.xhxur50qGJTn.93e7ON1d6sQpTovEu3EQgBEPqVsvryNqDhSbtJ239QezGUR9fkJUByN6rRRRjV6SaAT8b5AXzXO2bjo4oNc5DhIOYOqO0fzqaiRmwb9tte3fp5EVdaGvBHb5ncvfAxQZOOofI5uiiCZ2tszgd0qdU7s9VeqPK9.1OKIcw1jbM6mwcs1JlfUl5rJlWLvL0oISJS1T19bceC8lbZ8HFOdyM2b3m7S9IXmc1AEKVDIRjHTDAXBXQcPoE6SXP4Gf9CFAXYJBootu3qihU0QIKpCZIpmuopAX+sVj33wiKmvOLg.BLZtnIvzAoNwBEQTqqxEVXAb9yedwsF1au8vbyMmDsCkJUBMa1LTvlO6ryhkWdY333HalkISF4.5vKlG7GLJKVnArHnLMDkVhCFoB5S0JOOuP4.eMfrMKFpMFis9fCZ4cD.V5Elrgc5SeZ7Vu0aEZAF0KCEMrRkJ3oe5mFe4u7WFoRkZeAboNuFEUQmj0z5ofcx5cHL2kjOGMqHGmvAar42aCfRyvSakOy5DquLnhonBat4lXwEWD+nezOBkKWFYxjITROTajAS.LaVIzTGWrcpWnpABzhHv9Rp72wUL0ul99YdcZEVy2aiUCP3M3zl0mLvLGK47F8hKl9aN9wONVe80EqyZy0JHaCtXl0E94ZKDxwUJpImW2oSGr5pqhb4xgxkKKp9fNRcmNcj4yYxjA999R5aISlLxY8I8Xcpr79CFcdGZNWi8cZc2ALDDaqs1Rlex4YrdywW1uy1jdtg9Zhx8GNL.Wui.vRWXiMSlLnRkJ3l27lRmO+N1w455hm9oeZ7C9A+.r4laJcvbxMOW7FWgS9MMSqlYitdoYHweu7YQHxWTLnL0wkIXltXtPk9fEOgh4QEeylMCwrhfVlfTlfS5+X62Fypn9i+FSfqoQ7O1+p6OmF1wSaIJ8WoKZiEv5DSmOKu7x3Mdi2PXQv9RcavrcnGecccEIC3XStb4vi7HOB9VequE51sKlc1YwoN0oP1rYwBKr.777vst0sPr3i7wJaRNP.EZsOW2gtCCYJy7wOqiZ+yiESVW5mgVLX8yTeMlh1Y956FFUlk2wBXUnPAbhSbBrwFaH6Bo8pX5qHG+3CUL4+7+7+bHyrxIfSpveiYFdj0Eyci0.P1DAzD7wDDRC3YNAR+e9mIaF95Nc5HGmVTbAdJ6vX5idmtMVTZQDsAVYJRnFTxbgCAproOnwATDkNLlVQIGmdthpDkX+1z+EOCCefG3AvFarw9XaBf8wVg8OZfONuUKNYlLYvu5u5uJVZokPrXwvEu3EwIO4IkLlJvv90DwSH2CNNn2nC.RvFyMn8C7EPqXIBGNSgjDvX9J+dsJVzm+jZPZy4k7YXpXcMytIM1LMk21ArLaHZvgicrigRkJgM1XCgwion.6s2d3xW9x3EewWD25V2R5bndHljHg5EcZEvq+NV+j3y6NotE80pAZhRuT5IIZ.Md+M2ol+Q1j70.Cm.Wem5X2l6hd85IIIOt6MYXJrnF3C+.+8wlxDzhOaS.Kc+gIiEyOeb.NS56i52Lt6m43j4yRWGiBvheGqatttnSmN3ZW6Z3QezGEKrvB3l27lgDITKNDAr0hdxmk1XE7y3ok8G6i8wvd6sGJUpj3RC72zePeTLaQYtotspmqvPYiOGOWuPJIW29L23TCZwW2tcaY7mVvTKpmYen98Z1XlfgGEk21ArXQ2nI3T1rYwwN1wvsu8sCA.Q8E.LbB2RKsDt5UuJ9betOmbuH08IUzK1zI5OMaM8.JCUFshvswdRKdgMPKSwJMqS55komkK5jwef3VG73wx2eXtPm9jioyfZpKJ8yyj8jY8vFHkF7YZAihRThntNcIJQLlDSNVz61a9855NGaVe80wZqsFNwINApUql3Py5ME48PawTMKob4xgNc5HyqI.EC2k0VaMjNcZI8Jwj0nmqm3U4ZcGomCkHYB4HfywYjRyokk0p1vbCSc6TWnx74QqVT.klfS5OS+daOiCa49xAo5AoXJRwxKuLxmOenqwTTstc6hqbkqfSdhSJmdGSamjdWC5LoZfJyEUAHPDMTm6t3DM9m9yMuVMXlIi.sBx0dWNURKUrJ28jmBxTjPl62IqKSOT2lygpeuMvMSfLa57xFn2j520+G.V2I9fp2pn..0axYx5xDPSCZ0pUKbyadSjOedLyLyHaDZy0Ez.V54eO2y8b3Ye1mU1Xg9JkiiCNwINAVYkUDK0QflACFHfil.VbdkqqKRDOARmNs77LcLUMSd87PyMMY+PPPfbZiy4wrcp2L179omWa5Gg2Mimlk2wvvhESqOL2bygicrigW+0ecY.POIgTkykKGdOu22Cdqa7VRG9zrau4jWe+QwZn4Dbx.ywYj061mEBcBqOJ1lz6ratiE+rwIpFY.p84INogAoJEQtQiFRX5v1jIfRT+wxjDET2eo2.YRhCZ6yL+bSqDNteqsx3XrZCPSK5jV7N9+adyahJUpfkVZITqVMwIcA1OiBfvYZid85gG7AeP7bO2ygJUpfhEKJN0oqqqDBNbCGx5hVulaJQqzoYqGDLL6bP2agOactT2l9SM6q3mwM5oChxS+Hdes0+pkDg0Wc+t4Fx2sk2Q.XEkdD3hx4laNb8qeczrYy8QQklvc2c2Em6bmCe6u82Fuxq7JBioIwzxbAF2kjfA5ACMPJ2gyTAllh7YBhYBNXBlXBXoykSCFLHjdoBBF5ToLNKcccEVVjQF+slrez5jvFfk92YNFYV+M+rCpNpLel1dd.ixaXiSLxw8c19dBJQcSoW7xm8VasEt8suMNyYNCRmNcH+xRO1xh98L0Qu3hKhO5G8iJdNN2Xj.ATw1ISlTFmYvHy4CbLVugclLYjL1pq6nSEadnbnEAzFidyMQ4yRKEgoDKlRdXSzv6Uk2QHRndgNKbA+fACvINwIPwhECAfDDDHTQ4D43wiiOzG5CIx8q2Ufc5riVeeLqG5E679C.QGZLKXxPlg5NflslSVrYAIRum.KTLO9GEkieNm7xD1W+98Q+d8kqg42ceeewJQzMGLEwzTGVrewDXxlmwqaCQsAidLLJ8DY66.1uCFpAR40y9S8mqYBYaQI+d1Wpe9lhipOqK4XMu1qcsqg81aOr1ZqEZtEuG862GW9xWV7GJZziXwhg4medzoSGTYgJXs0VCqs1Zv22WNrNHKJ8YXHAy7h4I5jLjxxccPxjIEGL000Uh2O1Ox1jsffW2Gy9LttgeW9742GXm43qVDQaVezTEO2s.Zui.vZbE1oWrXwP6v.L57QSu3aokVBW5RWRzyiVlZfQCX1xViZlFDnPyLhS9IfFYZoYaoEGPu3h0UScGQ8LQ.n81aO4DSl+wiAd9c60cO40TrBcg22nbcgnDOTqSJc+gM8XYiQl9281YwbQBKl.d7OtwHYjGDDHGhqbdSud8v0t10vLyLCle94Ews4h7d85gG8QeT7o9TeJr1Zqgtc6hicrigOxG4ifkWdYjJUJr3BKhJUpHfODPh+dtYmevnXLT6GUddCM3CsTc73wCE79lVd1zxzQIIfVh.1Gvr2vj982uKuiPjvIUbbbPkJUvMtwMjOiKjXPQyIPoSmF+B+B+B3ke4WF6ryNxumfWl5ZQCrnKjwBmbwmGYbQq9Py9xmiIHkVo1lhRo+bBHDBX6Nth.+byqiOSdnkx5FAaI3oISI1+cP9u4uS2l0e28hIwiCbLp5k9yzfv1DIzzZxwiGW1PP6s9W+5WGm3Dm.KrvBhKzzsaWwnP9993Lm4L325252Bsa2FOzC8PhdfbbbFdpcOnOpUqF5zoCJUpDh4EC60eOIg6kJUJwsBXNnBHrO.x5MYXIVvVoFCsUripv4qbdcPvn7XVlLYrto9a2k24UihnvPNQ67d5cF.FN3ztcar1Zqgm4YdF7O8O8OERTP8tojsDKZPK9Zx5QqKA57ejEDURJY5QFXl.SZ1Kl.XlVsaP+AXf+fPsUMXk9+.HTrdwBYXZSGUSB7JpOaZJ2MrqlFvtn1fYbEMfkoRxIaTJFu40qYqPKFVoRE47GfoM3DIRfJUlWbzzjISNDvw0Aau01h3UC5OPLdCywTbSFsSZ533fjoRNRrW286SSIRjHjB2IfkV2siSGtDjiNMMUCgiiiX4w2NXQMtx63Ar3fW1++auu0fipyy6+2d+r20dSqzJsZEBDf.q.VH.a.aQAhAiisIN1Feoo0YbZ6LM+S9PcMS6G5zz1oSm1LcFO9KsMeosia8TGmYhcB14CMMId7j35TiSLIZLXvXL2QfVo8h166d9+gMOu6y4UmUWLRvJ1yuAMZY0tmqumeuOO+dt751M750qXctiSHHLu92ZgQkJUvN24NwwN1wvDSLgFyck0ISuHRw22TS5mmkxDoT1rYgWudECR3+M56C.MtlIqqjfnpZ87ppYtvQaO8HrnApbvahaz0I8rVRORJ4W2LHac0xEVHVXQPuiol8cIxJ50.M5I6zXEpBKn6sW3BW.IRj.wiGGoRkB4ymGJJJHZznHTnv.n9xVW3vggCGNPpTopKFt855SQoJ.MdketwCnfUqVgC6NDEotEyZ0Dkzbh2wQ4SdKST0LhKdfinwkjqoshnkmvhtAY2tcDNbXjISFg6X7OipppvEtb4xgN6rSryctS7Zu1qIJtShTg6hGf1bwh6VCsM4gIl+cnUxFxxOt.rbWwj0NhSVoWgHSaeZaw++xZNATmvhR1PBDgE25R4sC+2xuV95eyvxgafKDKnVrjkxh4CznINJucLatda7g5IaT.U..lbxIwUu5UQhDIDcDzsrksfMsoMgN6rSLyLy.61sid5oGjOedjLYx5AkwZCBfb4xoYYQiOoLAJu9nTzgqCKQxPKiZxqFT5k+SM6ZG84AZLViNeWtbw+FAs7DVbDIRDb8qecwC4bKZ3ZQPVSs8sucb7ieb7we7Gqgzg6lD2JK9C2bBJpR8IK3HxsxkKiToRIr9h1NT+Bh2BW3ZVQVSomkTy0O.5SbQVBPk0A.DQEi6FpLVLVtn228VEziPi++4+cdCHTNGgnwM.MrNgb6+Nuy6Dtb4Bm8rmUy1zpUq3BW3BHPf.XngFBG7fGDczQGvjISHSlLhN5IUT5kJWOvM70nQpOSQtIRiCc61sn3nsZ0pF28n.NoppJrrhqeEeQMkGbA8HbZlUnzDbxM5uVIzxGkP9ClTeKmV0X32jnDciZKHkJUB986GiM1XhjyS1JolMKD+AYhXha0DmDXlYlAEJTPSVkKm9B7H7UnPATnXgYkBCz2k9gdeYg6kI3nWyOWj0hga0lrakeVuWH+9M6yuTi46gH4+NcefdMOBWbcL4BUWtbYL7vCim3IdB3ymO..w83t6tawZpXwhEQtb4PWc0EhEKlH.LzJZjc61Q0JU0brP6GJWrDSBZ1jPna5yvSsBd1jSi+IsUkqcU55fdAXP98HRM59Js8aE0uBXE.gEMnhrZgZTY7tlfbdOQ2vKUpDti63NvF23F0zan.ZTU5xVSAnel7RKFnz1mHSnAsjqd7xigmdBDwEuzXHhI8bQT18QNAE++SVXR5h.ncoCWNa2Az55y7glQRwu9H+44gKmtlJmDuz0U90Z5+Kq0H.zXcK+bgHmIwhIgj4QakRY.xhEU05qsi6ZW6RiELz8dhzHQhD3vG9vHPf.Hb3v3.G3.3a7M9FX0qd037m+7PUUUD4P5ZOewffFKPhXSmikKWFd85Uj2TUpTAtb5R3FVoRkPznQE2uUTTDiyoNy.sjwKeOf653BIEDn2mlbkDxuUEs7tDxEgDndOeOTnP3ZW6ZhAWbA3AzpIkhhB1912NN0oNkHaf4l+Nefq2AIhM+g2JUpf74yKlkR1kO4zQP1BGNYfrkPxGCy2u0a1S9uWNfdaa94kdgWmqkH28slUJT71cBYEfISlDVsxmngdehzoToRvkKWhIUnUJa61siCbfC.SlLge7O9GKh3agBEDqAjkJUBadyaFc2c2vlMaHZznvkKWXzQGEe5m9onZ0p3Tm5T3RW5RhzLPNUW3Vsvi.HQrQVHySeF53iH9IMknInsXsQ99ciB90ZSlZj.zshVWArBvBK9MQZ.WznQgUaytw3wEKmSzrt0sNL7vCqIQPoACyG3aGxBItUPTQpR8jJxkOdgJqmKe5I3tfvSR2KNAWyz7BXwWCcKEfu8ku9yIhj0XDPae2hdetqIxVKvibE2pWx0JZ0glhr2fCNHhDIhXUngxjcxhK+98im4YdFLxHiHxLcRvcReIJMEFXfAPWc0ETTTvniNJ750KRlLIxlMK9E+hegf7grlhlTTUsQ89QeFKVrn48TUqWKrTVoawhE3zoSg0hDIhcG0WcmsZw5RJoBO5f75G7VoNkMCs7DV.ZCmOP8bxJPGA.f19okdjPTIKL1XiA+98Kbsjm9AKziAduOmKnMuLajIi3tlnWDAk0mR15K8HsjulvcwSVX44Ruok5AjMyZKt6f76WMKwDo6MTO9hS9SnZ0pvmOeZx4MxJF5dzpW8pwC8POjnkQCzn6ZRqK.JJJ3K+k+xXcqacHPf.3AevGDO0S8ThGbc5zI762O5qu9DKtCJJJXngFBSO8zvmOe38e+2GYylU3REQLQGutb4Rz6zIqzUbpng.lDbmHlI2EAlcKwgN9WJAYUmbUSzpgVdWBAzVySppph7dIUpTZHcnYj4yzCT2b2DIRfctycNqjIcgt+4DK7x7grbfzPgSfnWtTQP983jKxulC8detNb5sOZ1ee4B78k7CZlLYRyhX.ehHxBEJZZppphdYN+gcxh6gFZHDHP.bpScJMuO8amtbhwFaLjJUJbzidTw9nXwhh75KWtbn6t6FO2y8bZDslZowkJUBNc4Tr3OnpphrYyhsu8sie0u5Wgb4xgZ0pgO7C+PDIRDgUS7xbgVC.4t+4xoKwjKVsZUTWgxjd.ZWZ5jcedo59EYwF252EhGH2rQq2QjD355vmkNPf.hYk3kl.8CAxBmZ0pgwFaLzUWcIxWoExMD4TgnYBfShVR6OdzBksVRu2qY++l8CAY29ZlNVxu9lAnyERmQYqbnbpSNi9ApGos0t10poM4P8Wbx5kd6sW7rO6yhXwhowBLxJK61pG9+G5gdHLxHiHtOEMZTQZKXwR8UhH2tciHQhfAFX.zSO8nQOJmJNEEdNQpzWe8gUspUgye9yCqVshie7iC61sKxVc9ZMna2t0D4MZ+wSSApqgVoREMUOAmDkWgFbsauQAQXIWP2shnkmvBP6Chzuc4xE5omdzzr73QOj9Nz+uZ0pHXvfXW6ZWvgiFYP7BAbBA8zahdMOuq3tONWZQMW5R0LhJ8Ht3WaHBa8zvZ4h.SOKHI86j0Tzhk5qv2gBER35mbimytc6XqacqXCaXChzRgH64amAGbPbfCb.DLXPw4I82TTTDQo62+2+2G6e+6Gewu3WDG4HGAG3.GPTgDEJT.QhDAaYKaA82e+Hb3vn6t6F.Z6FA7E2iZ0pgCcnCgZ0pgBEJfImbRbkqbEQWEgHrnE7Vd6dgV4kIhHEEEgH6UpTQD8OJgmIxN9Dx7Lbeo391bEIwVIzxSXMWVA0UWcIlYfWhA7vpy03nToRXm6bmn2d6UHx37A4G30ykO5yo2q0ifpYjTz2ctHnZlkVxD52rPyzshHr3jLjn2kJUBqZUqB6e+6W7vo7B0PwhEgGOdvgO7gQ3vgESJQaGxEvjIShCdvCh8t28B61si74yipUqu1VR4FUsZ0fWudwS8TOENzgNDhEKlPqFRfbud8JRABh.gr5oVsZhBhlhHL0ka6t6tQxjIgYylwG+weL74ymlrNmxXc9XTRqHZrJekoQUUUP1oppposEYwpkkzHD1r6ksxjWs7DVD3DWbKI13F2nfnhFPPVPw+7z+WQQA6e+6Wzyy45lP6GxsC99jSPPOLRuGWPcdtVQ8XcYhJdY7HSdo2OMijh99zwKsL0Sjw718LcdxSifkx6KxDrwhECgCGVXsC4lLu87tsssML5niJtWRVGw0gzue+3vG9vvqWuh55jRkARH6ToRguvW3Kf8su8g95qObnCcH7m9m9mJxOJ5dsOe9vZVyZvfCNnn.lIxuJUpHHTIM+H8mnUabtVpJJJHb3v39tu6CISlD..m4LmASN4jB2XIxIJhiz4IWvbd2d..ZHloHJRjmVLaANr6PjPoKEZL0L8FMHrVlfKWtPrdhI5Ti.MlYhzHf2DyLYxDRjHA1vF1f3AadD+3gIuYtL1LWyzSeI897y228y5fE926VgNDz02Z0puZT+vO7CKVopAzVyZEJT.1saGeguvW.qcsqU784hJa0pUjNcZbm24chG9POLb61MRkJETUqujrGMZTwCu974CO4S9j3O+O+OGOxi7HHd73hICLYpdynibC0mOeHXvfHZznh9dEut73oi.upJHKgnxWoRkJ3fG7fviGO37m+7vlca3se62VPnUsZ8dZEM1iFK5wiG.zHYW4j9bqyHw6oTxfbkjRjz1Qrh+r1jISHduwEkPAM.PtWPAzH0DhDIB14N2I73wilHOQydRaml8PudtlomNUxZVI+d54p3RsY4K2DWz1mqaHk5H268duX6ae6BWuoj.k2oL6ryNwi+3ONBEJDJTnfvBppUqBud8Be97gomdZr6w1M1yd1CV8pWMdvG7AwQNxQv5W+5QsZ06aT4ymGVrXQzQO6omdfGOdDtDxiVG.fYKMpPBfFB0awhEX2gcw4kOe9fWud0rRIY2t858xJqVQu81K9hewuHt10tFJWpL9fO3CvG8QejHQRI2JApSPA.w4NQ.xs3hWGf7tWK0r9b5zolhltcCq3Ir.pKrYjHQDECJcClb0iWBNj33qacqCaZSaRyBKAPiALyWXcaFgCOOq3et4inRus8Bc.I+yoW3tWNGXy21z4rISlvLyLCpToBN7gOL5omdzLoPkJUPwhEgWudwLyLC1vF1.t+Cd+hLROTnP3Nuy6Dc1Ymnb4xBKcNzgNjvBpt6tawC7TNSEHP.rl0rFzau8JJmFtNV7.x.znbgLat9xkEvusSEX0l3djGOdfa2tEthAT2sMOd7.ylMizoSiG5gdHDOdbboKcITpbI7lu4ahzoSC61sKhPHAt0VzwDOQMowspppZVAljy2v1UrhHOrlKPDKACFDW+5WG4xkSS1uSQJhz4whk587b2tci64dtGbxSdRb8qecM87JJph5E5345getNQ584WLjPeVvshAy7Y5oyexcbxJom8YeV7BuvKfjIShPgBAqVsh96ueQ6YoZ0pXr6cr5SnToJFd3gEAFgRvSfFK3B81auvue+3jm7jBK5nHKRqbxVsZEIRj.YxjQLQEcswrYyvl0FGi1rYCd83ENb3Pj.vTz7ra2tvcQRmPZeQQpylMaX+6e+3e8e8eEwhECm3Dm.u8a+1XzQGUSY5XylsYUCg7Z.EX1VXI6kvxkf6qTvJdBKhHxkKWHb3vhpnml4hFPpppJZavpp06lnIRj.aaaaCu4a9lhY6ndp0B0jaxxsl8YmOcvlOcmVrCN4VOPaStqt5crei.xMOxZVxxAZxBKVrfgGdXbfCb.7q9U+JLzPCg69tuaDOdbQJMP2m16d1KrXwRcKZb3PyBnK8yHiLB73wCxlMqvkuToRIxSJJwKUUqmfwSM0TBA0IxFdOFimzjzqykKWcqzsWWuIxRtb4xIzOxgCGvue+Hc5zXhIl.acqaEu+6+93Tm5TXngFB+ze5OE80WeHZznvrYyh1sLOhf78M8+4uVNPP7bxpcEq3Ir.Z7PJQXckqbE.zHsC304EouEQLs8sucbpO5T3SN6mHbOj9rMi.QOqt3uOmLZ9H8lq+9BkLg1+zOjdL2L.20YZ+R+PG+YylEO1i8X39u+6GVsZEABDPzu8oBNVQQQrXkFOdbwBuA06nnNdAkU3EJT.80WeHPf.33G+3Hc5zniN5PzD73QFlmrlzZ..crQQQjestPgB0GmXodjVmbxIQ4xkQt74D0.HMoGc82oSm3O7O7OD+0+0+0HYxjvmOe3m8y9Y3Nti6PbLPWuHWQ4KyW7OC8Z8Hu3nczRqaKnqIyloPMymQlLcmJnTRjSxxfXwhgse2aWTNHTeJZtlIatFnnmKfKFWA4DOKVbqX.LQVxy2JJBe7j4kt11We8gAGbPzau8pof1IRtMsoMgPgBIxOo3wiCOd7fpUqhHQhHRWDU054NEkVGzOEJTPTVMtb4RiElTT23cTSxRLWtbID7e5omVzOqt5UuJNyYNCN24NGRmJsnWsmMaVbwKdQQxcVnPArsssM7XO1igyd1yJx98O4S9DgU1b8n3GO76e7wIMypXtKhsaXEOgEOymA.BEJDBEJDpVqpHJMxM3LZFVJkFFcKihMtwMBUUUQ2Ec9FTHaFeyDWegP.ciPRQfHYIcY3We3V.JGIzaTPaWYBdJha.MxV70t10hssssgt6tanpVuiLv0GRQQQ3lH84W8pWM762ufLxoSmhsMcNySCAR2Id19ySOAJHLz4NMYFOUA5niNPvfAgWudQnPgPvfAQnPgfa2tgGOdD5PkLYRbwKdQToRE31saTrXQ7U9JeEr4MuYbpScJ..7pu5qJVx4o1aDYcEOARIHSfIW3y58dsSXEOgEOihApO.LTnPviaOhY44VaIWaVUqVEJJJXe6auHb3vZz3Xor.SuYgaECjkScD53fd.UNS2qToB73wChGON5u+9QvfAmUWBfaAhLwOOEJ3g8m2HG4eFfFV8QRBvS0BZ0YlJYnvgCKZjcqYMqAqYMqACLv.HRjHh2Ob3vn2d6E974CkKWViVWe0u5WEpppHUpT3Lm4L3G8i9QBRJEEEwhlZ6pUR2HXEOgE4NBMSuISlPjHQfe+9EVQwCOLudrnH0..jHQ+X6ae6.P6pOxJMHSXcy5gB4bcylMan+96QkKvYC..d3SRDEDUWbefhlFQXonnHRjSRSHdU.HawIm.i9gHcn+Fu.i4EAO4dI4hJuPeUTTfe+9EeVpaKPk3C8cn+dkp0O2762OhDIBhFMJhFMJ..RmNMLYxD14N2It669tw4O+4QkJUvO3G7CvwN1wDt1RjyFDVKdrx7oxl.Z..MfRdVUdXs4MJMJ2otm64dDgSGnQ8ssR.My0rEqFZKVPQWk.O8PLYxjfvhGsT4niQEJNMAC2RJ4HpQ22neSkhDmPieemb8jWOezuIWJ44FEPiEQDpI5Que0pUgYSlEVRQ5e0c2cCOd7f74yiToRgolZJ7jO4Sh.ABfImbRjISF7Zu1qIJDa59R6pac2HXEOgE8PJW7W.noqQBzngwQVbQyFSaCmNchvgCi8t28JpCuVAWBWHjM7+dyHrVt.+AZhDfhBFOgHUUUQM0FsFE5yR2urZ0pnqFvKGEtEN7RrhHkHPcjSxBK99fpuT9DVzOxVkQGOtb6RzBXjinH09W.ZzIZ6ryNQGczARlLIlZpoPWc0Edlm4YvDSLATUUwG+weLdm24czr7YsR0J9akXE+UL4npPCtTTTDZNPyFShvStGX1rYQ5MPtproMsIroMsIMKKXsBXwjSXK1u2MBnIKHwuoHz0UWcAfFhuawhEX1j1hIGng9WbqdIxIdNKw0dhmdJj1UNb3.JNTzXcFA9JLCPiNBgYylgOe9zjbmhH3YtQWCkHwjKLaJxxoSmVjjrABD.1rYCSM0T3QdjGA6YO6Am7jmDUpTAu0a8Vh0h.C7YCq3Ir3tLHiN5nC30qWQTh3spC50TwjVtbYwCA6cu0Efm2RdaEPyHeluPgubBxhJx84vgCicu6ciG3Ad.DLXPMsWE47IpYs4Wx5Gh7hSPwESmetJqYEmvxgCGBci3eO9B6fb5OPVSIaIN2xMZ+pppJVSHowbj35es+eeM31saL8zSiyd1yhidziJzqy.KdrhmvB.ZBiOe1Pa1rIlAUy.cylDjVNb3.yLyLhvZa1rYDOdbrssssUjyDJeLyakM582kyUrEatiQVhFKVL7.OvCfu1W6qguzW5Kgd6sWjISFgVUzwA20NdvP3KOVzuozOP9G59jISlzzcW4mSbPSNQm+hDc821aoj0phr9hJRYJIQoRygHonsmEKVD4DVoRkPGczA5ryNwktzkPG96.O1i8XXhIl.EJT.+nezOBm6bmSbMw.KNbaAgEeVad02C.DMZTQK9flo0l0FVaYxjIQBFRKMT1rYC6d26FwhESHHOWjV52yUNMsXzNZ9HLZVdZoWpD.LaKW3EhsbI6zrbEidMuHsIPBcawhErgMrA7TO8Sguw23af8su8gd5oGDKVL30qWg0sb8n3VwHm.k70YOJGsHKbnZ5it1yEgm96d73QnYEeb.MwD+ZZ0pUgZMssgHJELnsubEKP4LEsOJWtLlYlYDtFRZmQYJ+fCNHRmNMd3G9gwpV0pPxjIQlLYvq7Juhlq0bxVpOqY.8wsEklybAUUUwpkCUmY.P7Zp1wnGdHA2CDH.1yd1CdkW4UPgBED+MhPiVHEZE.OxS5AYQwo2ir1fdOtqOzCv7sMYwSf.AvF23FwHiLB5u+9EjIIRj.81auvhEK3JW4JhV2C2EOZeJmiU7HBp24gYKl0XkL+bWN2qnnFRtzyIj4VQYx7r2ubW836G90P48O+8jsBLTnPHSlL3QezGE+c+c+cHXvfX7wGGu+6+9h9LO+5.MYCOwaMPCbaOgEP8HFVpTIL0TSooB3Afn3a4OjRQHbqacq3W+q+03W9K+kZdHhu3.zJNandQJjaYBGzC2z0Dt0EbqqrZ0JhEKF1zl1jXwE0tc6vue+nmd5AwiGGczQGPUUEW4JWA0pUusDWtRYMhbCnsmkwSs.YBKMQ3UZs3iNF4K7DbK13mmxoGg38MYVSgOys3jHU4WCm02mcbKqeFsc762OTUUw9129vq9puJt7kuLpToBdi23MvHiLhFK5jqKSiTeX1nsfvhZqGzRxDMKL0Cusa2NrX0BJjudaLgZwsNb3.6YO6Am6bmCSN4j.n9fTpt3tYkmVKln+ISJwe3k2up3jT7dvDAhrRQQACLv.XjsLBV+5VO762Ob3vA5ryNQuw6Ew6MtHJaj36TmwTUUEVszXENVOQxIcs3oofdosfbaXge7ycqj+23aKYBKdjIku1J+9z0Bt1Y7iK42iGYSf5s44rYyhm3IdB727272.Od7fwGebbricLrksrEMGOxsxZCBKsnsfvBnNIDUzpTJNTrXQQ8qYtpYT1RYQSaiZ.cqacqCaYKaA+O+O+OBxMxxf4qHouYA8dfifdB6xsfxoSmhNgIYcI0UV23F2H15V2JhDIBBDH.73wChFMJRjHABFLnnsvPqNNJJJhVUsdQsiaAEOWnjSYAxhNNoirEOxYwNOYN4I.JkqUxtWwi3m70QhrjSXviZI+yyc4sYW+oZUbO6YO367c9N3Lm4LvoSm30dsWCCO7vZpmPJUa3Qmz.MPaCgE0KsKUpjX.AeARfxrYtnvTjF2wN1A9nO5ivoN0oDOXrTsH.rPA+gGYqqjsZ.nQqdg+44eN58xkKm3ZgppJ5omdvniNJti63NPmc1Ira2NhDIBhGOtPLc.HHoHQs4VwP6W61sKtFySvW53iWOeMyBIBbKx3mKjEIMqqbJagE+5grUb7qyx2a4IYJ+5tr6mx2S3VwFJTH73O9ii+p+p+JjKWN7q+0+Z7tu66h64dtG.zPWUpedYXg0rQaAgEci2oSm06vk0phpUpJx+JxhIZ0Qgd3hFflHQBr6cuabsqcMg.7spCjZVzD4DFbconh+tu95CirkQvcrw6PjGQgBEBqYMqAACFTDIUBj3vjULzClbW7nx1g65mrnz7xsYtDYlHW3tAR6e.shxKe8PO29niG4.KPjsxkqC2hN4s+bAxhIZht669tObzidT7ge3GBa1rg23MdCwh2JUClz0HCg2mMZKHrnAUTzrxkKGphFEgJ8dzCPb8QnGHtq65tv3iON9e+e+e0rPWtbak0bYYk7Lv5oqEuvhIhC531pUqXzQGEiLxHXsqcshqO81aunu95CgCGVy1VVTdhL.nwRRE.DYoNsxwnmETbBJyVlcz+3orQyr5g+9VsXcVVQQ4Hl72kGwOYg0k04SN0R3ee8zYR9d.0NkApStFLXP7nO5ih+h+h+BToRE7a9M+FL93iiN6rSQdew0ky.ZwssDVx4SC8vlWudgKWtvoN0ofYyVgUa00hhJOG2tcKzzgCWtbg8u+8iSe5SiImbRgVVTT13QNbwjPfy2Lz5YY.2cC52x4JEcLP5zQmS1rYCc1YmXqacqXKaYKHb3vvgCGhEdzd5oG32ue.nsahJ+.j7w.W+HY8bH8BosI.zneT0JUmkEVz1kuBZq29l1F5YEEs8jc6iKdNYIC86lIteyvBwhKdWe..XG6XGXzsNJdu+u2C81au3kdoWBqZUqBCN3fZ1m7tPBW+OZ6zNZA1ssDVjKIzMaYAa85KPcMXL2nsjPQNDnQK1kmDlqd0qFis6wvO36+ClUoUv0ugr7fWbueVAOwNoiK4jRkD7laMBoqzl27lQ974wjSNIhFMJ19cscroO2lDgau+96GQhDAwhESrMn0KulU5LyGHhjlooD25I4nSJCxZMdS2SuGZ0ivrZ0p5lqUjUy7thJcLK6B3MJnwUpp0SR0xkKiN6rS7kdjuD9MG+2HR7zW5kdIbjibD3zoSMZ9MWncirB31XBK.LqYl3tzzUzv3bW5rvlEGhvIyWDC3CxMatd2tLXvfXr6cLb7O333zm9zZx7Y9r0z9h2WyuQOOzSnc5gc61sKHY.p+f3l27lwN1wNPWc0EBFLnXQavmOehVIchDIDsiXfFV+PODuXhBpr6QzjDjqhxVpvIe3oLfL3I0pdVfQDVTT8juNY1j11GCk.wz4ImXUljco.jVY778C.Xe6ae38du2Ce2u62EJJJ33G+336889d3K+k+xhqGEKVbV8ad8rhscB2VSXMeVYoX0DpUSamsjmM1.MpIQZQQHb3vXe6ae3RW5RhUmGtFQz9bohrhOqOmjhaMEORb4xkCqd0qFG9vGF974CQiFUzaxCGIL5IVOh92jr9M2nASPlPRNQQkE3l9sdZTIKpN2kO55Mmfl2+qnOCWTe4iSdgUye+kqfoPiSra2tfH5O3O3O.iO93hd.+O7G9CQ73wwt28tAf9tYSStJWcBsK31ZBK.s2P4DK..Vq5AEQIMhvR+PVbQeVZ0cwsa2Xm6bm3C9fO.uy67NhGV30bHeeei5Rndt.xIvn8akJUfSmNwl27lEq3xQiFEwiGGSLwDvmOeXCaXCZZyJ.PrRzrTOvmaUUyHA3Vkomn1zmgbMsY4RE+32jISBqnHBBdlmSjWKDqTVJRq.9DJDIK4haO8zC95e8uNdtm64vTSMEb4xE91e6uM5qu9P+82ulz3faEe6FIEG2VelqWWbfFzWtbYDLTmZ9b71NCY0EsrQQhrqnn.2dbi8u+8iHQhHdvfFbwm4eonZ7IWLIKDo8AWj8PgBgememeG77O+yi+j+j+DbvCdPL7vCid5oGQvGndQNGb2eo7phKv8bosjdXw94AzJZurkPhOi45cXCB7j0TNgNqUqlHW6n1QL2ULxpK41Fymki8EK3VSRSxMxHifu9W+qiLYxfolZJjKWN729292hKe4KKNdoetYbL1piaqsvhOir7rR1rYCoqjGJJJhUVX5gFJQ+nG14yVqpphZUqgMtwMhwFaL75u9qqwJEhbgz.5FEbqO38cJmNcBOd7fcu6ci8rm8HzhJPf.hGFHBJdVay0LhNeZl.6KFqKjERm+9y0CZjECMKALqUqFTqoNKWBI8n345E823oPAMQirUa76O5kVBKktGxsthe8gRV46+9uejLYR7s+1eannnfIlXB7O9O9Ohm+4edDLXPw8Q92ucUKqaqIrlOblIdOr9naUCQ.f1kK7hEKBWtbIRPxLYx.e97gryjEG3.G.G6XGCm+7mWDEHJL7KUCjnnLQKkUtb4BCLv.Xu6cuXW6ZWhVliOe9PGczA5t6tAfVWIod8EkjryUQ8J+++rhEi0.D4fdhpCzHoQIKvJWtLxmOOxkKm3gVdVyS8+Lxkd4yklUH3KmfHR4cKCRdBe97ge2e2eWbkqbE7lu4ahUspUgSe5Si+9+9+d77O+yKR8DfYmNIsavx27a9M+l2pOHtUgIs+tHfRPTspGTKuYX0RMLSwZvhcmnZsYfcq0.f1YmIKvrXo9RpthhBN1wNlvMMxbedX34VdwERkaU.WmEZvLI1upZ8VjyN1wNvW8q9Uwi+3ONFbvAgCGNP2c2M5s2dEcKAdXz4D.oRkBgCGV2n0ciH7LWCJ9wOOoMotjQvfA0LY.QpmMaVToRE32ueQjWoZTjZ+vzZNHm.mtdx6aVxtXxabe7yQ426y54+BEbM1neyCriYylwl1zlvm7IeBdu268P3vgwLyLC94uyOGqcv0hN6rSw2iOgH8+oWKmxG2tYAVasEV1LaGWc5ygPJaDUrYFkqTE985BEqVDJNriR4p.y+1tSJ20FNA1V1xVvN1wNva8VukvJgBEJLq0cNt.7DQBmTi1tz.4pUqhhEKh96ueb228ciwFaLDKVLwChjf57nexOFkiPm7f4kJnmv4DYIk2Qz4J2cYxkLxxV6NZ35VsZ0PpToDK.oz9g.YwEccRNBjxe9VYPYBOc97m8m8mA+98iidzihDIR.UUU7s9VeK7rO6yhwFaLw4KOoQ0KXDxVrd6BZqIrpl0MpYNOb4DvTMfqc07vphcTsRdXyhcXxrSXAk0Lqn7OwhEC6e+6GiO933xW9xPQQQLXgdXkmqPxImpbT.KUpDTTTvm6y84v8bO2C15V2pPGCa1rgnQiht5pKg6EDwDOL2xypRV8sb0cIzKkHnyKpUHyINqUqFRmNsvktBEJLqnJxIfzKeoH82H2bal0Rs5fRaF5bLTnP3O5O5OBd73Ae2u62EwiWu887O8O8OgO4S9D7zO8SqI8YjiBNf1BC+1MKrZqcI7xe5jnPkzvjkhPQwB.b.yvALaFPsZMXF1gIS0z7fhYKlgInMqsiEKFRkJEFe7wQkJUzTecjKI7byha0fpppvEG+98icsqcgm4YdFbnCcHLzPCAWtbgt5pKjHQBzWe8IV11IBJ49LE8ZdaLF.HSlLvsa2ZbIao3A5lk+VkJUBYylEYxjQzQW6niNDDM4ymGSO8zhsgc61gWudEO3RVRPVkQq3QbBMdo2zrLquUGz4PgBEDobhKWtv5V25fYylwa8VuETUUgWudwwO9ww3iONFXfAPf.ADj+7yaU0FUpvsiVX0VSXYsjMb8bWGEKeEXwdE3yc2HWF.T0LfoZvj0Z.pZ0bvDLg5+q9CETi9q6t6FW5RWBW+5WWDUQto6zCeDYUwhEEjNIRj.G3.G.+d+d+dXee98gXcGCABD.c2c2X8qe8HTnPvoSmZBCOvrqIN53jKnK82ylMqXc+aoV3V9L5zwBUNPYxjQDPB+98KzZpVsZnPgBBxFa1ro65.H.D8SJdjLal0XqTAcdPVKVsZUr0stUzc2ciexO4mfomdZDOdbbwKdQ7t+h2E0pVCCMzPhOKmjhGw5U5WWjgI013j6H4ktDtlZJnZ42.qVMgDg+7XxKYBEymAJdLgxpUf4ZMRzP52UpTAUpVA0pVSr3CTrXQLwDSf268dO7e+e+eiwGebM8PdReFpA44wiGrt0sNbu268hgFZHDKVLX2tc31saDIRDDNb3Fcy.FQk7.PtUT7+N2c.UUUb4KeY30qW3wimkEKrjcGISlL3hW7hhbgxhEKnqt5RrTvWtbYL0TSIH2sZ0JBEJzrVF2o+tdMguaGfrH4zDf..SLwD..HYxj3HG4H3jm7j3Nuy6TLwvl27lwi83OFRzWBMMSQd2D41sqYs0DVSLw0wkybU3riSBfZna+6ENqF.olZBTQsHJA.qPKgE8Z9RKU974EC1HsY9k+x2GG8nuANwINgPbcUUUzQGcfQGcTryctSL7vCCEEE3wiG3xkKzYmcJL0muu3CnkitDf9htR5mQVkbkqbE3zoS3ymukzYeosirdZkKWFSN4jhbbqlZMDu23Z5tlkKWFYylUbdDHP.w4xb0q4ou+sCVPPmCbWBmYlYfKWtDQXkNGegW3Evq9puJ5pqtP73wQ974gCGNv9129vAO3AEsCHJEbtcirBnMmv5BWKOtVxy.2Q9HnpBnlcHrltVOxOyUQxzofMOAgkeaouPfWKWTTtnd7d5zogISlPxjIQwhEQkJUvwN1wvq+5uNJVrHtq65tv8du2K5qu9fISlfCGNP3vgQznQ03xGIjLgloSjLjsvha4y0t10fMa1Do9vR4C67F2GPCxlBEJfomdZTpTITtbYLv.CH5DpbsaJVrHpVsJ762ulLVWOKKkIFmKqOWoPnQGm7qKTWw0jISnToRHUpTvqWu3Mey2Dequ02BSO8zXcqacvqWuHe97n6XciG3fO.16d2qH5reV61Fsxnslv5zexUwT1eMzg2tQsz9P.GCfNC0GJUnBlL00PU0h.0rpQuDBTj8HwzE+e0ZnTwRHWtbnXwhBW.AffTRQQA81auHTnPKaQsSNisu90uNpVsJhFM5R99SdeyQtb4vzSOMpUqFBFLnnTm3jMb2B0qframAccpZ0pHa1r.ntKh+m+m+m3ke4WFd73ACLv.voKmHS5LX8qe83QdjGACO7vZB3CMghbehi+atFj78cqDZqIrpVsJNc1WGlq5AnfeD0y.vmmHnREfzYShr4lFVMW2Lc8l8l.QZwGHTsZUToZEnVSEoSmFkJUBQ5LBhDNhHQGWNSy.8HrJWtrHS3Wtf7voJUpfLYxfJUpHZsMzmi6pK077jCSuAgUMMR.TrXQXwhEL8zSiyd1yh+g+g+AbhSbB32uert0uNTsRcoI1111FdnG5gPhDIDDUzXAZazLKvnwyshVn0VSXA.b5o9wnRNyvi8tP2AG.VLaGp0.JVt.RN0Dvh45BfpWcAxcMglEizXh+2HQioL8d49AQ8RfzolZp5tNzc2K6j.7gTjqgjXxzCAxt4Qe11cBJ8.+5Ck.tTpg30qW7xu7Ki+8+8+cbgKbAzWe8gd5sGTsRcq9O3AOH1yd1ChFMpFWNkk4fq6Es8a0rtBvfvBG+ruMTr5FQC1K76pS.U.0Z.plpgr4lFExUooccAZlKB5cojd.TQQAABD..MhdyxEwkdDVoRkBYxjA8zSO2zIEHWmucH8CtU.8rDWUUUH6fa2tw0u90wK8RuDdkW4UPwhEwpW8pQ3HgQ1LYQrXwvAO3Awm+y+4EEyOEM14BshV31VSXUtbYb5y+gHn+PHbvNgES1PsZ.zXipUKgjISIlUirdhmU4xQtiCRHUpHbCGN7BZfxMJzivZlYlAISlD81ausLCBaEefnUCMasIfFaQ4yV1rYgOe9vEtvEvK7Bu.dq25sfaWtwpWypEiAFd3gwS9jOI5u+90nMl73Wd+6pUyJq1ZBK.Hh9h7MO5xR5zoEhmSlLSVUwIujAePE2kPpKBrbpOfLgEk8ySLwDHd73Ka62EKVIGYualXgzcQo7ZqRkJHb3v3cdm2Au3K9h3Dm3DHTnPXUqZUhINevG7AwgNzgzrrokOedQ.QZkivXaOgEpA.S.vTMTqVUXxjYXxjEnpB.UfxUJgYlYFjOed..cC6N8+kuTRQ9pToRh5Di2wOuY4RHk2XW5RWBIRjXIe+sPNd.z1BaZ1qa1my.0AuJF.ZPtPuOkKflLYBtb4B+G+G+G3+5+5+BSLwDXsqcsvue+XlYlAaXCa.O8S+zHQhDvtc6qXZ4xs0klC..pATqTIXxpYXxTcsq.pSXY1DfEqVDk9.vry5b4dFOubXHBKJbwNc5TSyz6lwCi7vVmNcZzQGcrruO0Cxj47yc5XTuqKs6DV7VDCMNRNe8Hw3oxxgFmkMaVrqcsKrsssMb0qdU7tu66hRkJgN6rSb0qdU7y+4+b30qWDOdbMEGub8o1JACKrlGPOHQYvNOxVzMYfYOyjrqhThQ50q2kEsAjsFQl.nVsZ3bm6bnqt5Rrbd0pNnz.KcHSlLvjo5cT1u+2+6iW7EeQjLYRr10tV3ymOLyLyfcu6ciuxW4qHVR0ZkcIr019uVDXxjoY0pcmK8qZF3o9vMavItLP6Cb4xErZ0Jld5owC+vOL9NemuC1yd2CNwINAtvEt.b5zI9o+zeJNxQNB9zO8SAvri9cqDLrvZd.20uYlYFQ1FawhkY0UQ4PlXnVs5qpz974aYwcv4yBKUUUbwKdQDHP.g3pF31evGWbsqcMQ+I6Ue0WEu3K9hvhEKXvAGTnI1S7DOAdfG3AZYs.2vBq4AzC6TI0PBb9Ygmeob8Jbw.ZvGecZz.sGvjISXlYlA..QhDQT9SO5i9n3e9e9eFQiFEG+3GGoSmF..u7K+xszZXYPXs..YsjMa1Ds9iEKwCIL5sRSsoNdpAZufa2tEcFCud8B61sipUqh0t10h+k+k+Ebe228gyctygqd0qpY4OqUDs0sH4ECHqTb3vgl7xZgnIDYQCu8Heq.7fDXf1CPSrRqEiTpOPZaYwhE7W9W9Wh3wii+s+s+MQxnx69FsRn0kJsEB7ab1rYSbydw.dn6uYBtUfeVBTfAVYC4rXm5G9z3WWtbgRkJg+3+3+X7bO2ygToRIjOf5oZsRvvBq4A7RgfdshhBJUpjHYRWn3VAgEG7L32.sGPNPLTRmRSfZwhEDIRDb8qec7jO4SJJN9V0TavvBq4A5k85lMaFNc5TyRltL3uO2ZLdmJcoj7PVOMdIFQCN4KPFK06eCzZB4jvUdwpkjnHTnPnPgBXKaYKBxpVQ8NMrvZd.mLheymxnXJMGZFjKSFdW.8lInbIidsAL.fVqtsa2tlIZaEEe2HOrVDPN2TVrM4e4N53MaH2m2MfA.zNtrUVvc.CBqa5XtpoNCXfaEPujNFn0broAg0xLZUpB9l0gDLP6MlqwEshiSZ8bRsECMiOegFwO8V7J3q5N2r.+X0XNJCPfOVPtJHZEGmXH59mQrPEOWuYrZ0l0x.suP15pVcqvMHrVDnYcHy4Byk1.2LFTLW6iV0AkF3lGZVuGyXQnnEEzCs2Ntrda.Cb6FZ6IrLfALvJGz5Yy2MYzJlMuFv.FPez1SXwal+Fv.Fn0FFtDZ.CXfULns2BKCX.CrxAFDVFv.FXECLHrLfALvJFXPXY.CXfULvfvx.Fv.qXfAgkALfAVw.CBKCX.CrhAFDVFv.FXECLHrLfALvJFXPXY.CXfULvfvx.Fv.qXfAgkALfAVw.CBKCX.CrhAFDVFv.FXECLHrLfALvJF7+GjnvzYzZ4a7S.....jTQNQjqBAlf" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "osc-gametrak",
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 1,
					"patching_rect" : [ 15.0, 45.0, 83.0, 20.0 ],
					"id" : "obj-1",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p Novelty Detect",
					"outlettype" : [ "FullPacket", "FullPacket", "FullPacket" ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 3,
					"patching_rect" : [ 15.0, 315.0, 99.0, 20.0 ],
					"id" : "obj-51",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 315.0, 148.0, 867.0, 777.0 ],
						"bglocked" : 0,
						"defrect" : [ 315.0, 148.0, 867.0, 777.0 ],
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
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.var @op union",
									"outlettype" : [ "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 6.0, 89.0, 99.0, 20.0 ],
									"id" : "obj-4",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"outlettype" : [ "FullPacket" ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 86.0, 57.0, 25.0, 25.0 ],
									"id" : "obj-3",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.if \"(/left/pluck < -0.05)\"",
									"outlettype" : [ "FullPacket", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 2,
									"patching_rect" : [ 346.0, 145.0, 138.0, 20.0 ],
									"id" : "obj-1",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.if \"(/left/pluck > 0.05)\"",
									"outlettype" : [ "FullPacket", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 2,
									"patching_rect" : [ 305.0, 117.0, 134.0, 20.0 ],
									"id" : "obj-2",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.if \"(/left/pluck < /pluck/lower/threshhold)\"",
									"outlettype" : [ "FullPacket", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 2,
									"patching_rect" : [ 48.0, 146.0, 233.0, 20.0 ],
									"id" : "obj-41",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.if \"(/left/pluck > /pluck/upper/threshhold)\"",
									"outlettype" : [ "FullPacket", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 2,
									"patching_rect" : [ 7.0, 118.0, 235.0, 20.0 ],
									"id" : "obj-38",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.call /left/radius c.novelty @as /left/pluck",
									"outlettype" : [ "FullPacket", "", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 3,
									"patching_rect" : [ 10.0, 36.0, 230.0, 20.0 ],
									"id" : "obj-37",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"outlettype" : [ "FullPacket" ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 9.0, 5.0, 25.0, 25.0 ],
									"id" : "obj-47",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 46.0, 177.0, 25.0, 25.0 ],
									"id" : "obj-48",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 7.0, 176.0, 25.0, 25.0 ],
									"id" : "obj-49",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 84.0, 178.0, 25.0, 25.0 ],
									"id" : "obj-50",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-37", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-41", 1 ],
									"destination" : [ "obj-50", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-41", 0 ],
									"destination" : [ "obj-48", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-38", 1 ],
									"destination" : [ "obj-41", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-38", 0 ],
									"destination" : [ "obj-49", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-38", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-4", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 1 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-47", 0 ],
									"destination" : [ "obj-37", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p Fuse",
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 15.0, 195.0, 47.0, 20.0 ],
					"id" : "obj-46",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 595.0, 44.0, 423.0, 130.0 ],
						"bglocked" : 0,
						"defrect" : [ 595.0, 44.0, 423.0, 130.0 ],
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
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.call /left/x /left/y atan2 @as /left/orientation",
									"outlettype" : [ "FullPacket", "", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 3,
									"patching_rect" : [ 4.0, 63.0, 246.0, 20.0 ],
									"id" : "obj-1",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.call /left/x /left/y expr \"sqrt($f1 * $f1 + $f2 * $f2)\" @as /left/radius",
									"outlettype" : [ "FullPacket", "", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 3,
									"patching_rect" : [ 4.0, 39.0, 359.0, 20.0 ],
									"id" : "obj-43",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"outlettype" : [ "FullPacket" ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 4.0, 5.0, 25.0, 25.0 ],
									"id" : "obj-44",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 4.0, 93.0, 25.0, 25.0 ],
									"id" : "obj-45",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-45", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-43", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-44", 0 ],
									"destination" : [ "obj-43", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"outlettype" : [ "bang" ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 60.0, 360.0, 20.0, 20.0 ],
					"id" : "obj-42"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"outlettype" : [ "bang" ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 15.0, 360.0, 20.0, 20.0 ],
					"id" : "obj-40"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.show",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 855.0, 600.0, 49.0, 20.0 ],
					"id" : "obj-2",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p Calibrate",
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 15.0, 165.0, 69.0, 20.0 ],
					"id" : "obj-32",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 230.0, 44.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 230.0, 44.0, 640.0, 480.0 ],
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
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.call /*/[xy] * 30. @prepending /degrees",
									"outlettype" : [ "FullPacket", "", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 3,
									"patching_rect" : [ 50.0, 100.0, 225.0, 20.0 ],
									"id" : "obj-29",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.call /*/z * 2. @prepending /meters",
									"outlettype" : [ "FullPacket", "", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 3,
									"patching_rect" : [ 50.0, 127.0, 199.0, 20.0 ],
									"id" : "obj-5",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"outlettype" : [ "FullPacket" ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 49.0, 66.0, 25.0, 25.0 ],
									"id" : "obj-30",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 50.0, 156.0, 25.0, 25.0 ],
									"id" : "obj-31",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-29", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p Neutralize",
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 15.0, 135.0, 75.0, 20.0 ],
					"id" : "obj-28",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 284.0, 148.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 284.0, 148.0, 640.0, 480.0 ],
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
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.call /*/z scale 4095 0 0. 1.",
									"outlettype" : [ "FullPacket", "", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 3,
									"patching_rect" : [ 50.0, 68.0, 157.0, 20.0 ],
									"id" : "obj-24",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.call /*/[xy] scale 0 4095 -1. 1.",
									"outlettype" : [ "FullPacket", "", "FullPacket" ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 3,
									"patching_rect" : [ 50.0, 91.0, 174.0, 20.0 ],
									"id" : "obj-6",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"outlettype" : [ "FullPacket" ],
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 50.0, 39.0, 25.0, 25.0 ],
									"id" : "obj-25",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 50.0, 116.0, 25.0, 25.0 ],
									"id" : "obj-27",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-25", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-27", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/right/y -0.971184 \n/left/z 0.00757021 \n/left/x 0.357265 \n/right/z 0.00757021 \n/left/y 0.24591 \n/right/x 0.159951 \n/degrees/right/y -29.1355 \n/degrees/left/x 10.7179 \n/degrees/left/y 7.37729 \n/degrees/right/x 4.79853 \n/meters/left/z 0.0151404 \n/meters/right/z 0.0151404 \n/left/radius 0.433716 \n/left/orientation 0.967958 \n/delta/left/orientation 0 \n/left/anticlockwise 1 \n/left/clockwise 0 \n/left/pluck 0.433716 \n/was/right/y -0.971184 \n/was/left/z 0.00757021 \n/was/left/x 0.357265 \n/was/right/z 0.00757021 \n/was/left/y 0.24591 \n/was/right/x 0.159951 \n/was/degrees/right/y -29.1355 \n/was/degrees/left/x 10.7179 \n/was/degrees/left/y 7.37729 \n/was/degrees/right/x 4.79853 \n/was/meters/left/z 0.0151404 \n/was/meters/right/z 0.0151404 \n/was/left/radius 0.433716 \n/was/left/orientation 0.967958 \n",
					"linecount" : 32,
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 1,
					"patching_rect" : [ 645.0, 15.0, 316.0, 448.0 ],
					"id" : "obj-7",
					"fontname" : "Arial"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-51", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-51", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 2 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-46", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 1 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 2 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-29", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-20", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
