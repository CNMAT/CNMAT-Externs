{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 2.0, 49.0, 1012.0, 644.0 ],
		"bglocked" : 0,
		"defrect" : [ 2.0, 49.0, 1012.0, 644.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 20.0,
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
					"maxclass" : "newobj",
					"text" : "o.if /key/is == down",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 36.0, 161.0, 182.0, 29.0 ],
					"outlettype" : [ "FullPacket", "FullPacket" ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Synthesizer Signal Processing",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 233.0, 372.0, 418.0, 29.0 ],
					"id" : "obj-12",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Control Structure\n\n(e.g. linearly scaled mapped parameters)",
					"linecount" : 4,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 630.0, 202.0, 214.0, 98.0 ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Gesture Acquisition",
					"linecount" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 578.0, 57.0, 150.0, 52.0 ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Keyboard Synthesizer design pattern\nAdrian Freed 2011\n",
					"linecount" : 3,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 508.0, 485.0, 251.0, 98.0 ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /fuzz = /velocity * 12.",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 245.0, 291.0, 256.0, 29.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /attack = (1.0 - /velocity) *200.",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 246.0, 243.0, 337.0, 29.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p fuzzy synth",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 50.0, 372.0, 127.0, 29.0 ],
					"outlettype" : [ "signal", "FullPacket" ],
					"id" : "obj-33",
					"fontname" : "Arial",
					"fontsize" : 20.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 418.0, 93.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 418.0, 93.0, 640.0, 480.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 20.0,
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
									"maxclass" : "comment",
									"text" : "Use the delegation outlet to recover the control parameter for this object that incorporates the Sine Synth behavior",
									"linecount" : 4,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 296.0, 87.0, 317.0, 98.0 ],
									"id" : "obj-5",
									"fontname" : "Arial",
									"fontsize" : 20.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "use tanh~ as a handy   monotonic odd function",
									"linecount" : 2,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 133.0, 200.0, 300.0, 52.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"fontsize" : 20.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.route /fuzz",
									"numinlets" : 1,
									"numoutlets" : 2,
									"patching_rect" : [ 154.0, 136.0, 121.0, 29.0 ],
									"outlettype" : [ "", "FullPacket" ],
									"id" : "obj-28",
									"fontname" : "Arial",
									"fontsize" : 20.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "*~",
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 51.0, 153.0, 32.5, 29.0 ],
									"outlettype" : [ "signal" ],
									"id" : "obj-27",
									"fontname" : "Arial",
									"fontsize" : 20.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "tanh~",
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 50.0, 215.0, 61.0, 29.0 ],
									"outlettype" : [ "signal" ],
									"id" : "obj-22",
									"fontname" : "Arial",
									"fontsize" : 20.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p Sine Synth",
									"numinlets" : 1,
									"numoutlets" : 2,
									"patching_rect" : [ 50.0, 100.0, 124.0, 29.0 ],
									"outlettype" : [ "signal", "FullPacket" ],
									"id" : "obj-20",
									"fontname" : "Arial",
									"fontsize" : 20.0,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 358.0, 79.0, 944.0, 509.0 ],
										"bglocked" : 0,
										"defrect" : [ 358.0, 79.0, 944.0, 509.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 20.0,
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
													"maxclass" : "newobj",
													"text" : "o.print",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 212.0, 87.0, 66.0, 29.0 ],
													"id" : "obj-14",
													"fontname" : "Arial",
													"fontsize" : 20.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 204.0, 114.0, 50.0, 27.0 ],
													"outlettype" : [ "" ],
													"id" : "obj-11",
													"fontname" : "Arial",
													"fontsize" : 20.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "p sine wave",
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 40.0, 83.0, 116.0, 29.0 ],
													"outlettype" : [ "signal", "FullPacket" ],
													"id" : "obj-7",
													"fontname" : "Arial",
													"fontsize" : 20.0,
													"patcher" : 													{
														"fileversion" : 1,
														"rect" : [ 30.0, 345.0, 640.0, 480.0 ],
														"bglocked" : 0,
														"defrect" : [ 30.0, 345.0, 640.0, 480.0 ],
														"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
														"openinpresentation" : 0,
														"default_fontsize" : 20.0,
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
														"boxes" : [ 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "o.route /frequency",
																	"numinlets" : 1,
																	"numoutlets" : 2,
																	"patching_rect" : [ 50.0, 100.0, 172.0, 29.0 ],
																	"outlettype" : [ "", "FullPacket" ],
																	"id" : "obj-1",
																	"fontname" : "Arial",
																	"fontsize" : 20.0
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "cycle~",
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"patching_rect" : [ 61.0, 227.0, 68.0, 29.0 ],
																	"outlettype" : [ "signal" ],
																	"id" : "obj-2",
																	"fontname" : "Arial",
																	"fontsize" : 20.0
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "inlet",
																	"numinlets" : 0,
																	"numoutlets" : 1,
																	"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
																	"outlettype" : [ "FullPacket" ],
																	"id" : "obj-3",
																	"comment" : ""
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "outlet",
																	"numinlets" : 1,
																	"numoutlets" : 0,
																	"patching_rect" : [ 61.0, 316.0, 25.0, 25.0 ],
																	"id" : "obj-4",
																	"comment" : ""
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "outlet",
																	"numinlets" : 1,
																	"numoutlets" : 0,
																	"patching_rect" : [ 203.0, 316.0, 25.0, 25.0 ],
																	"id" : "obj-5",
																	"comment" : ""
																}

															}
 ],
														"lines" : [ 															{
																"patchline" : 																{
																	"source" : [ "obj-1", 1 ],
																	"destination" : [ "obj-5", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-2", 0 ],
																	"destination" : [ "obj-4", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-3", 0 ],
																	"destination" : [ "obj-1", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-1", 0 ],
																	"destination" : [ "obj-2", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
 ]
													}
,
													"saved_object_attributes" : 													{
														"default_fontface" : 0,
														"fontname" : "Arial",
														"default_fontname" : "Arial",
														"globalpatchername" : "",
														"default_fontsize" : 20.0,
														"fontface" : 0,
														"fontsize" : 20.0
													}

												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "adsr~ 20 30 0.6 100",
													"numinlets" : 5,
													"numoutlets" : 4,
													"patching_rect" : [ 223.0, 354.0, 189.0, 29.0 ],
													"outlettype" : [ "signal", "signal", "", "" ],
													"id" : "obj-13",
													"fontname" : "Arial",
													"fontsize" : 20.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "0",
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 217.0, 260.0, 32.5, 27.0 ],
													"outlettype" : [ "" ],
													"id" : "obj-12",
													"fontname" : "Arial",
													"fontsize" : 20.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "select up",
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 190.0, 211.0, 91.0, 29.0 ],
													"outlettype" : [ "bang", "" ],
													"id" : "obj-9",
													"fontname" : "Arial",
													"fontsize" : 20.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "*~",
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 41.0, 418.0, 32.5, 29.0 ],
													"outlettype" : [ "signal" ],
													"id" : "obj-8",
													"fontname" : "Arial",
													"fontsize" : 20.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "o.route /amplitude /key/is /attack /decay /sustain /release",
													"numinlets" : 1,
													"numoutlets" : 7,
													"patching_rect" : [ 116.0, 147.0, 513.0, 29.0 ],
													"outlettype" : [ "", "", "", "", "", "", "FullPacket" ],
													"id" : "obj-6",
													"fontname" : "Arial",
													"fontsize" : 20.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"patching_rect" : [ 49.0, 27.0, 25.0, 25.0 ],
													"outlettype" : [ "FullPacket" ],
													"id" : "obj-17",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 41.0, 467.0, 25.0, 25.0 ],
													"id" : "obj-18",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 612.0, 459.0, 25.0, 25.0 ],
													"id" : "obj-19",
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-8", 0 ],
													"destination" : [ "obj-18", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-8", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-13", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-17", 0 ],
													"destination" : [ "obj-7", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 1 ],
													"destination" : [ "obj-6", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-13", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 1 ],
													"destination" : [ "obj-9", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 2 ],
													"destination" : [ "obj-13", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 3 ],
													"destination" : [ "obj-13", 2 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 4 ],
													"destination" : [ "obj-13", 3 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 5 ],
													"destination" : [ "obj-13", 4 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 6 ],
													"destination" : [ "obj-19", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 1 ],
													"destination" : [ "obj-14", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontface" : 0,
										"fontname" : "Arial",
										"default_fontname" : "Arial",
										"globalpatchername" : "",
										"default_fontsize" : 20.0,
										"fontface" : 0,
										"fontsize" : 20.0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
									"outlettype" : [ "FullPacket" ],
									"id" : "obj-30",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 50.0, 280.0, 25.0, 25.0 ],
									"id" : "obj-31",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 256.0, 280.0, 25.0, 25.0 ],
									"id" : "obj-32",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-27", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 0 ],
									"destination" : [ "obj-22", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 1 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-27", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-20", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 1 ],
									"destination" : [ "obj-32", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"fontname" : "Arial",
						"default_fontname" : "Arial",
						"globalpatchername" : "",
						"default_fontsize" : 20.0,
						"fontface" : 0,
						"fontsize" : 20.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 160.0, 454.0, 25.0, 25.0 ],
					"id" : "obj-29",
					"comment" : ""
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /amplitude = /velocity",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 246.0, 198.0, 259.0, 29.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-21",
					"fontname" : "Arial",
					"fontsize" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 48.0, 442.0, 45.0, 45.0 ],
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/key/is up \n/midipitch 67. \n/frequency 391.995422 \n",
					"linecount" : 3,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 124.0, 102.0, 315.0, 48.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"numinlets" : 1,
					"lockeddragscroll" : 1,
					"numoutlets" : 1,
					"args" : [  ],
					"patching_rect" : [ 32.0, 11.0, 516.0, 60.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-3",
					"name" : "o.io.manual.maxpat"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 1 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-5", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 1 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
