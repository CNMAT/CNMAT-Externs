{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 0.0, 44.0, 1680.0, 852.0 ],
		"bglocked" : 0,
		"defrect" : [ 0.0, 44.0, 1680.0, 852.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 1,
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
					"text" : "print",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1200.0, 424.0, 34.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-52",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dumpbeats",
					"presentation_linecount" : 3,
					"fontsize" : 12.0,
					"presentation_rect" : [ 225.0, 0.0, 37.0, 46.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 224.0, 0.0, 70.0, 18.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-46",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number~",
					"mode" : 2,
					"fontsize" : 12.0,
					"presentation_rect" : [ 520.0, 760.0, 117.0, 20.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 520.0, 760.0, 117.0, 20.0 ],
					"numoutlets" : 2,
					"presentation" : 1,
					"id" : "obj-53",
					"fontname" : "Arial",
					"sig" : 0.0,
					"outlettype" : [ "signal", "float" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 800.0, 440.0, 34.0, 20.0 ],
					"numoutlets" : 0,
					"id" : "obj-70",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"fontsize" : 12.0,
					"presentation_rect" : [ 248.0, 640.0, 41.0, 18.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 496.0, 528.0, 41.0, 18.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-50",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll icmc_demo.coll",
					"fontsize" : 12.0,
					"presentation_rect" : [ 248.0, 664.0, 115.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 496.0, 552.0, 115.0, 20.0 ],
					"numoutlets" : 4,
					"presentation" : 1,
					"id" : "obj-56",
					"fontname" : "Arial",
					"outlettype" : [ "", "", "", "" ],
					"saved_object_attributes" : 					{
						"embed" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"fontsize" : 12.0,
					"presentation_rect" : [ 120.0, 640.0, 41.0, 18.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 560.0, 432.0, 41.0, 18.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-57",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll icmc_demo2.coll",
					"fontsize" : 12.0,
					"presentation_rect" : [ 120.0, 664.0, 121.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 560.0, 456.0, 121.0, 20.0 ],
					"numoutlets" : 4,
					"presentation" : 1,
					"id" : "obj-58",
					"fontname" : "Arial",
					"outlettype" : [ "", "", "", "" ],
					"saved_object_attributes" : 					{
						"embed" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"fontsize" : 12.0,
					"presentation_rect" : [ 48.0, 640.0, 41.0, 18.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 496.0, 432.0, 41.0, 18.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-48",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll",
					"fontsize" : 12.0,
					"presentation_rect" : [ 48.0, 664.0, 59.5, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 496.0, 456.0, 59.5, 20.0 ],
					"numoutlets" : 4,
					"presentation" : 1,
					"id" : "obj-51",
					"fontname" : "Arial",
					"outlettype" : [ "", "", "", "" ],
					"coll_data" : 					{
						"count" : 5,
						"data" : [ 							{
								"key" : 0,
								"value" : [ 0, 0.000625, 2.5, 2.5, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 1,
								"value" : [ 0, 7.0, 2.5, 2.5, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 2,
								"value" : [ 1, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 3,
								"value" : [ 1, 3.0, 2.5, 2.5, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 4,
								"value" : [ 1, 7.0, 2.5, 2.5, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0 ]
							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"embed" : 1
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 328.0, 24.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"id" : "obj-49",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "hide functions",
					"fontsize" : 12.0,
					"presentation_rect" : [ 456.0, 496.0, 131.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 458.0, 493.0, 131.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-41",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend hidefunction",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 80.0, 448.0, 123.0, 20.0 ],
					"numoutlets" : 1,
					"id" : "obj-47",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "matrixctrl",
					"rows" : 1,
					"presentation_rect" : [ 400.0, 496.0, 45.0, 18.0 ],
					"numinlets" : 1,
					"columns" : 2,
					"patching_rect" : [ 80.0, 424.0, 45.0, 18.0 ],
					"numoutlets" : 2,
					"presentation" : 1,
					"id" : "obj-40",
					"outlettype" : [ "list", "list" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "show beat correction",
					"fontsize" : 12.0,
					"presentation_rect" : [ 70.0, 498.0, 121.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 368.0, 624.0, 131.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-24",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"presentation_rect" : [ 46.0, 498.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 344.0, 624.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-31",
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "showbeatcorrection $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 344.0, 656.0, 131.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-39",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "show tempo correction",
					"fontsize" : 12.0,
					"presentation_rect" : [ 240.0, 496.0, 131.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 528.0, 624.0, 131.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-21",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"presentation_rect" : [ 216.0, 496.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 504.0, 624.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-22",
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "showtempocorrection $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 504.0, 656.0, 141.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-23",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"presentation_rect" : [ 665.090942, 440.0, 40.909088, 69.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 832.0, 512.0, 20.0, 140.0 ],
					"numoutlets" : 2,
					"presentation" : 1,
					"id" : "obj-45",
					"outlettype" : [ "signal", "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"presentation_rect" : [ 616.0, 440.0, 40.909088, 69.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 808.0, 512.0, 20.0, 140.0 ],
					"numoutlets" : 2,
					"presentation" : 1,
					"id" : "obj-44",
					"outlettype" : [ "signal", "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 792.0, 488.0, 81.0, 20.0 ],
					"numoutlets" : 2,
					"id" : "obj-42",
					"fontname" : "Arial",
					"outlettype" : [ "signal", "signal" ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 622.0, 677.0, 549.0, 261.0 ],
						"bglocked" : 0,
						"defrect" : [ 622.0, 677.0, 549.0, 261.0 ],
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
									"maxclass" : "message",
									"text" : "0 0 1, 1 0 1",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 176.0, 96.0, 71.0, 18.0 ],
									"numoutlets" : 1,
									"id" : "obj-20",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadbang",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 176.0, 64.0, 60.0, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-18",
									"fontname" : "Arial",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 0.0, 56.0, 19.0, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-23",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patcher" : 									{
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"patching_rect" : [ 56.0, 216.0, 25.0, 25.0 ],
													"numoutlets" : 0,
													"id" : "obj-2",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack 0 0 0",
													"fontsize" : 12.0,
													"numinlets" : 3,
													"patching_rect" : [ 48.0, 168.0, 66.0, 20.0 ],
													"numoutlets" : 1,
													"id" : "obj-1",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "expr abs($i1 - 1)",
													"fontsize" : 12.0,
													"numinlets" : 1,
													"patching_rect" : [ 106.0, 124.0, 98.0, 20.0 ],
													"numoutlets" : 1,
													"id" : "obj-21",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "unpack 0 0 0",
													"fontsize" : 12.0,
													"numinlets" : 1,
													"patching_rect" : [ 50.0, 100.0, 79.0, 20.0 ],
													"numoutlets" : 3,
													"id" : "obj-20",
													"fontname" : "Arial",
													"outlettype" : [ "int", "int", "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
													"numoutlets" : 1,
													"id" : "obj-22",
													"outlettype" : [ "list" ],
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-1", 0 ],
													"destination" : [ "obj-2", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-21", 0 ],
													"destination" : [ "obj-1", 2 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-20", 1 ],
													"destination" : [ "obj-1", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-20", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-20", 2 ],
													"destination" : [ "obj-21", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-22", 0 ],
													"destination" : [ "obj-20", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"fontface" : 0,
										"fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 12.0,
										"globalpatchername" : ""
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 0.0, 24.0, 25.0, 25.0 ],
									"numoutlets" : 1,
									"id" : "obj-19",
									"outlettype" : [ "list" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "matrix~ 2 2 1.",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 64.0, 192.0, 84.0, 20.0 ],
									"numoutlets" : 3,
									"id" : "obj-14",
									"fontname" : "Arial",
									"outlettype" : [ "signal", "signal", "list" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 264.0, 152.0, 20.0, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-16",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "edge~",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 264.0, 120.0, 44.0, 20.0 ],
									"numoutlets" : 2,
									"id" : "obj-17",
									"fontname" : "Arial",
									"outlettype" : [ "bang", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 16.0, 152.0, 20.0, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-15",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "edge~",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 16.0, 120.0, 44.0, 20.0 ],
									"numoutlets" : 2,
									"id" : "obj-13",
									"fontname" : "Arial",
									"outlettype" : [ "bang", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "resonators~ 900 1. 7. 1700 0.5 15.",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 312.0, 144.0, 194.0, 20.0 ],
									"numoutlets" : 2,
									"id" : "obj-7",
									"fontname" : "Arial",
									"outlettype" : [ "signal", "list" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "*~ 100",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 312.0, 120.0, 46.0, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-8",
									"fontname" : "Arial",
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "te_breakout~ @numoutlets 1 @name icmc @function 1",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 312.0, 16.0, 306.0, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-9",
									"fontname" : "Arial",
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "<~",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 312.0, 96.0, 32.5, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-10",
									"fontname" : "Arial",
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "delay~ 1 1",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 328.0, 64.0, 66.0, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-11",
									"fontname" : "Arial",
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"patching_rect" : [ 312.0, 224.0, 25.0, 25.0 ],
									"numoutlets" : 0,
									"id" : "obj-12",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "resonators~ 440 1. 7. 800 0.5 15.",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 64.0, 144.0, 188.0, 20.0 ],
									"numoutlets" : 2,
									"id" : "obj-6",
									"fontname" : "Arial",
									"outlettype" : [ "signal", "list" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "*~ 100",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 64.0, 120.0, 46.0, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-5",
									"fontname" : "Arial",
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "te_breakout~ @numoutlets 1 @name icmc @function 0",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 64.0, 16.0, 306.0, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-4",
									"fontname" : "Arial",
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "<~",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 64.0, 96.0, 32.5, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-3",
									"fontname" : "Arial",
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "delay~ 1 1",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 80.0, 64.0, 66.0, 20.0 ],
									"numoutlets" : 1,
									"id" : "obj-2",
									"fontname" : "Arial",
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"patching_rect" : [ 64.0, 224.0, 25.0, 25.0 ],
									"numoutlets" : 0,
									"id" : "obj-1",
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-14", 1 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-14", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-3", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-10", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-17", 0 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-20", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 12.0,
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "function num",
					"fontsize" : 12.0,
					"presentation_rect" : [ 384.0, 464.0, 79.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 384.0, 536.0, 79.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-38",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "lock",
					"fontsize" : 12.0,
					"presentation_rect" : [ 360.0, 440.0, 70.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 376.0, 448.0, 70.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-37",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "time max",
					"fontsize" : 12.0,
					"presentation_rect" : [ 240.0, 440.0, 70.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 240.0, 528.0, 70.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-35",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "time min",
					"fontsize" : 12.0,
					"presentation_rect" : [ 240.0, 464.0, 67.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 272.0, 608.0, 67.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-36",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "tempo max",
					"fontsize" : 12.0,
					"presentation_rect" : [ 96.0, 440.0, 70.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 96.0, 528.0, 70.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-34",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "tempo min",
					"fontsize" : 12.0,
					"presentation_rect" : [ 96.0, 464.0, 67.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 120.0, 608.0, 67.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-32",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "snap to grid",
					"fontsize" : 12.0,
					"presentation_rect" : [ 520.0, 464.0, 73.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 712.0, 584.0, 73.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-30",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "show grid",
					"fontsize" : 12.0,
					"presentation_rect" : [ 520.0, 440.0, 66.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 648.0, 528.0, 66.0, 20.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-29",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"fontsize" : 12.0,
					"presentation_rect" : [ 48.0, 24.0, 32.5, 18.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 48.0, 0.0, 32.5, 18.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-28",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "line~ 0",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 48.0, 16.0, 46.0, 20.0 ],
					"numoutlets" : 2,
					"id" : "obj-27",
					"fontname" : "Arial",
					"outlettype" : [ "signal", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0., 30. 30000.",
					"fontsize" : 12.0,
					"presentation_rect" : [ 80.0, 24.0, 84.0, 18.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 80.0, 0.0, 84.0, 18.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-26",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"presentation_rect" : [ 712.0, 464.0, 45.0, 45.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 808.0, 656.0, 45.0, 45.0 ],
					"numoutlets" : 0,
					"presentation" : 1,
					"id" : "obj-25"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"presentation_rect" : [ 336.0, 440.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 352.0, 448.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-18",
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "locked $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 352.0, 480.0, 62.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-20",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontsize" : 12.0,
					"presentation_rect" : [ 336.0, 464.0, 50.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 336.0, 536.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"presentation" : 1,
					"id" : "obj-19",
					"fontname" : "Arial",
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setfunction $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 336.0, 568.0, 85.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-7",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 12.0,
					"presentation_rect" : [ 192.0, 440.0, 50.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 192.0, 528.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"presentation" : 1,
					"id" : "obj-13",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 12.0,
					"presentation_rect" : [ 192.0, 464.0, 50.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 224.0, 608.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"presentation" : 1,
					"id" : "obj-15",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "timemax $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 192.0, 560.0, 73.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-16",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "timemin $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 224.0, 640.0, 69.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-17",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 12.0,
					"presentation_rect" : [ 48.0, 440.0, 50.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 48.0, 528.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"presentation" : 1,
					"id" : "obj-10",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 12.0,
					"presentation_rect" : [ 48.0, 464.0, 50.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 72.0, 608.0, 50.0, 20.0 ],
					"numoutlets" : 2,
					"presentation" : 1,
					"id" : "obj-8",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear",
					"fontsize" : 12.0,
					"presentation_rect" : [ 168.0, 24.0, 37.0, 18.0 ],
					"numinlets" : 2,
					"patching_rect" : [ 168.0, 0.0, 37.0, 18.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-5",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 728.0, 496.0, 41.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-3",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"presentation_rect" : [ 496.0, 464.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 688.0, 584.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-12",
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"presentation_rect" : [ 496.0, 440.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 624.0, 528.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"presentation" : 1,
					"id" : "obj-11",
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 728.0, 520.0, 59.5, 20.0 ],
					"numoutlets" : 4,
					"id" : "obj-43",
					"fontname" : "Arial",
					"outlettype" : [ "", "", "", "" ],
					"coll_data" : 					{
						"count" : 10,
						"data" : [ 							{
								"key" : 0,
								"value" : [ 0, 0.0, 1.0, 1.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 1,
								"value" : [ 0, 11.999996, 3.14159, 3.14159, 0.5, 0.5, 2.0, 1.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 2,
								"value" : [ 0, 17.999998, 0.2, 1.414214, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 3,
								"value" : [ 0, 23.999992, 3.0, 3.0, 0.0, 0.5, 1.0, 3.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 4,
								"value" : [ 0, 29.999992, 1.967872, 1.967872, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 5,
								"value" : [ 1, 0.0, 1.0, 1.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 6,
								"value" : [ 1, 11.999999, 3.14159, 3.14159, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 7,
								"value" : [ 1, 18.0, 1.414214, 1.414214, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 8,
								"value" : [ 1, 24.002773, 3.0, 3.0, 0.0, 0.0, 1.0, 3.0, 2.0, 2.0 ]
							}
, 							{
								"key" : 9,
								"value" : [ 1, 29.999998, 1.967872, 1.967872, 0.0, 0.0, 1.0, 1.0, 2.0, 2.0 ]
							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"embed" : 1
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "showgrid $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 624.0, 560.0, 75.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-9",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "snap_to_grid 1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 688.0, 616.0, 90.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-6",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "freqmax $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 48.0, 560.0, 71.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-2",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "freqmin $1",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 72.0, 640.0, 67.0, 18.0 ],
					"numoutlets" : 1,
					"id" : "obj-4",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess selmode 5",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"patching_rect" : [ 888.0, 418.0, 117.0, 20.0 ],
					"numoutlets" : 1,
					"id" : "obj-74",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route cellblock dump selection",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1008.0, 416.0, 171.0, 20.0 ],
					"numoutlets" : 4,
					"id" : "obj-33",
					"fontname" : "Arial",
					"outlettype" : [ "", "", "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jit.cellblock",
					"selmode" : 5,
					"fontsize" : 12.0,
					"rows" : 100,
					"presentation_rect" : [ 888.0, 416.0, 761.0, 282.0 ],
					"cols" : 11,
					"numinlets" : 2,
					"patching_rect" : [ 888.0, 456.0, 761.0, 282.0 ],
					"numoutlets" : 4,
					"presentation" : 1,
					"id" : "obj-14",
					"fontname" : "Arial",
					"outlettype" : [ "list", "", "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "tempo_editor",
					"show_xaxis" : 1,
					"show_minor_ygrid" : 0,
					"fontsize" : 12.0,
					"show_minor_xgrid" : 0,
					"timemax" : 30.0,
					"presentation_rect" : [ 48.0, 48.0, 1601.0, 365.0 ],
					"freqmax" : 11.0,
					"show_major_xgrid" : 0,
					"numinlets" : 2,
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"show_yaxis" : 1,
					"patching_rect" : [ 48.0, 48.0, 1601.0, 365.0 ],
					"functions" : [ 6, 0.0, 6.0, 6.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 12.0, 1.0, 1.0, 0.0, 0.0, 0.1, 0.9, 1.0, 4.0, 2.0, 2.0, 0, 16.000626, 10.0, 10.0, 0.0, 0.0, 0.1, 0.9, 4.0, 4.0, 2.0, 2.0, 0, 25.0, 0.5, 6.0, 0.0, 0.0, 0.1, 0.9, 4.0, 2.5, 2.0, 2.0, 0, 29.0, 3.0, 3.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 30.0, 3.0, 3.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 5, 0.0, 6.0, 6.0, 0.0, 0.0, 0.1, 0.9, 1.0, 2.0, 2.0, 2.0, 0, 12.0, 1.0, 1.0, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 16.0, 10.0, 10.0, 0.0, 0.0, 0.1, 0.9, 0.4, 0.4, 2.0, 2.0, 0, 25.0, 3.0, 3.0, 0.5, 0.5, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 30.0, 3.0, 3.0, 0.5, 0.5, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
					"show_major_ygrid" : 0,
					"numoutlets" : 6,
					"presentation" : 1,
					"id" : "obj-1",
					"fontname" : "Arial",
					"textcolor" : [  ],
					"name" : "icmc",
					"correctionColor_box" : [ 1.0, 0.0, 0.0, 0.2 ],
					"timemin" : -1.0,
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "" ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-1", 2 ],
					"destination" : [ "obj-53", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-56", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-56", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-27", 0 ],
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
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-1", 0 ],
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
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-4", 0 ],
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
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
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
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-1", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-74", 0 ],
					"destination" : [ "obj-14", 0 ],
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
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-44", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 1 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 1 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-25", 1 ],
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
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-51", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
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
					"destination" : [ "obj-58", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 1 ],
					"destination" : [ "obj-70", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 5 ],
					"destination" : [ "obj-52", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 5 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
