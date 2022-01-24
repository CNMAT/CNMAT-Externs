{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 2,
			"revision" : 1,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 100.0, 100.0, 772.0, 596.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 8.0, 8.0 ],
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
		"subpatcher_template" : "",
		"showrootpatcherontab" : 0,
		"showontab" : 0,
		"assistshowspatchername" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 8,
							"minor" : 2,
							"revision" : 1,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"classnamespace" : "box",
						"rect" : [ 0.0, 26.0, 772.0, 570.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 8.0, 8.0 ],
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
						"subpatcher_template" : "",
						"showontab" : 1,
						"assistshowspatchername" : 0,
						"boxes" : [  ],
						"lines" : [  ]
					}
,
					"patching_rect" : [ 655.0, 164.5, 27.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p ?"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 8,
							"minor" : 2,
							"revision" : 1,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"classnamespace" : "box",
						"rect" : [ 100.0, 126.0, 772.0, 570.0 ],
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
						"subpatcher_template" : "",
						"showontab" : 1,
						"assistshowspatchername" : 0,
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-13",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 189.0, 146.0, 114.0, 20.0 ],
									"text" : "Activate with noise"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-12",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 31.0, 232.0, 114.0, 20.0 ],
									"text" : "Activate with float"
								}

							}
, 							{
								"box" : 								{
									"bubble" : 1,
									"bubbleside" : 3,
									"bubbleusescolors" : 1,
									"id" : "obj-6",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 31.0, 452.5, 38.0, 24.0 ],
									"text" : "1",
									"textjustification" : 1
								}

							}
, 							{
								"box" : 								{
									"bubble" : 1,
									"bubbleusescolors" : 1,
									"id" : "obj-4",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 73.0, 260.5, 38.0, 24.0 ],
									"text" : "2",
									"textjustification" : 1
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-11",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 372.0, 496.0, 34.0, 22.0 ],
									"text" : "print"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-10",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 321.0, 505.0, 24.0, 24.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 165.0, 386.0, 24.0, 24.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"linecount" : 5,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 432.0, 171.5, 270.0, 74.0 ],
									"style" : "default",
									"text" : "The resonators~ object can be excited with an input signal or by an impulse of arbitrary size. An incoming float is interpreted as the amplitude of an impulse. The impulse is taken to come at the first sample of the next signal vector computed."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 347.0, 366.208800999999994, 102.0, 22.0 ],
									"style" : "default",
									"text" : "r resonators-help"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 70.0, 453.5, 75.0, 22.0 ],
									"style" : "default",
									"text" : "startwindow"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 73.0, 439.5, 75.0, 20.0 ],
									"style" : "default",
									"text" : "• start audio"
								}

							}
, 							{
								"box" : 								{
									"data" : [ 593, "png", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ],
									"embed" : 1,
									"id" : "obj-15",
									"maxclass" : "fpic",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "jit_matrix" ],
									"patching_rect" : [ 209.0, 27.0, 109.0, 48.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-25",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 57.0, 98.5, 347.0, 33.0 ],
									"style" : "default",
									"text" : "A bank of 2-pole resonating filters with common input (the inlet) and outputs summed together (outlet)"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-26",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 57.0, 76.5, 118.0, 20.0 ],
									"style" : "default",
									"text" : "Resonant filter bank"
								}

							}
, 							{
								"box" : 								{
									"fontface" : 1,
									"fontsize" : 24.0,
									"id" : "obj-27",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 57.0, 42.0, 152.0, 33.0 ],
									"text" : "resonators~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-29",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 0,
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 8,
											"minor" : 2,
											"revision" : 1,
											"architecture" : "x64",
											"modernui" : 1
										}
,
										"classnamespace" : "box",
										"rect" : [ 40.0, 55.0, 324.0, 363.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 8.0, 8.0 ],
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
										"subpatcher_template" : "",
										"assistshowspatchername" : 0,
										"boxes" : [ 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-1",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 101.0, 272.0, 65.0, 15.0 ],
													"text" : "startwindow"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-2",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 0,
													"patching_rect" : [ 101.0, 293.0, 29.0, 17.0 ],
													"text" : "dac~"
												}

											}
, 											{
												"box" : 												{
													"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
													"fontface" : 0,
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-3",
													"interval" : 250.0,
													"maxclass" : "number~",
													"mode" : 2,
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "signal", "float" ],
													"patching_rect" : [ 77.0, 241.0, 51.0, 17.0 ],
													"sig" : 0.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
												}

											}
, 											{
												"box" : 												{
													"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
													"fontface" : 0,
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-4",
													"interval" : 250.0,
													"maxclass" : "number~",
													"mode" : 2,
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "signal", "float" ],
													"patching_rect" : [ 23.0, 241.0, 51.0, 17.0 ],
													"sig" : 0.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-5",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 4,
													"outlettype" : [ "signal", "signal", "float", "float" ],
													"patching_rect" : [ 23.0, 218.0, 53.0, 17.0 ],
													"text" : "minmax~"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-6",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 23.0, 177.0, 42.0, 17.0 ],
													"text" : "*~ 100"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-7",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "signal", "list" ],
													"patching_rect" : [ 23.0, 197.0, 115.0, 17.0 ],
													"text" : "resonators~ 480. 0.5 1."
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-8",
													"linecount" : 2,
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 23.0, 156.0, 34.0, 27.0 ],
													"text" : "noise~"
												}

											}
, 											{
												"box" : 												{
													"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
													"fontface" : 0,
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-9",
													"interval" : 250.0,
													"maxclass" : "number~",
													"mode" : 2,
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "signal", "float" ],
													"patching_rect" : [ 209.0, 242.0, 51.0, 17.0 ],
													"sig" : 0.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
												}

											}
, 											{
												"box" : 												{
													"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
													"fontface" : 0,
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-10",
													"interval" : 250.0,
													"maxclass" : "number~",
													"mode" : 2,
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "signal", "float" ],
													"patching_rect" : [ 155.0, 242.0, 51.0, 17.0 ],
													"sig" : 0.0,
													"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-11",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 4,
													"outlettype" : [ "signal", "signal", "float", "float" ],
													"patching_rect" : [ 155.0, 219.0, 53.0, 17.0 ],
													"text" : "minmax~"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-12",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 155.0, 178.0, 42.0, 17.0 ],
													"text" : "*~ 100"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-13",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "signal", "list" ],
													"patching_rect" : [ 155.0, 198.0, 115.0, 17.0 ],
													"text" : "resonators~ 480. 0.5 1."
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-14",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 155.0, 157.0, 62.0, 17.0 ],
													"text" : "cycle~ 480."
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-15",
													"linecount" : 5,
													"maxclass" : "comment",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 13.0, 69.0, 275.0, 58.0 ],
													"text" : "Unless your model has lots of very wide resonances, you'll probably have to boost input signals a lot to hear them. The issue is that only the spectral energy that lines up with the filters will appear in the output. But beware: energy focused at the same frequency as a resonance will come through loud and clear."
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 9.0,
													"id" : "obj-16",
													"linecount" : 2,
													"maxclass" : "comment",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 37.0, 269.0, 27.0 ],
													"text" : "Why does this example multiply the noise~ amplitude by 100 before passing the signal through the resonant filter?"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-2", 0 ],
													"source" : [ "obj-1", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"source" : [ "obj-11", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-9", 0 ],
													"source" : [ "obj-11", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-13", 0 ],
													"source" : [ "obj-12", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-11", 0 ],
													"source" : [ "obj-13", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-12", 0 ],
													"source" : [ "obj-14", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-3", 0 ],
													"source" : [ "obj-5", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-4", 0 ],
													"source" : [ "obj-5", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-7", 0 ],
													"source" : [ "obj-6", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-5", 0 ],
													"source" : [ "obj-7", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-6", 0 ],
													"source" : [ "obj-8", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 288.0, 197.5, 50.0, 22.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"tags" : ""
									}
,
									"style" : "default",
									"text" : "p why?"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-31",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 503.0, 429.917572000000007, 99.0, 22.0 ],
									"style" : "default",
									"text" : "tellmeeverything"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-32",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 280.0, 325.5, 202.0, 22.0 ],
									"style" : "default",
									"text" : "640. 0.5 4. 843.5 0.4 2. 2041. 0.3 8."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-33",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 189.0, 169.5, 24.0, 24.0 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-34",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 189.0, 235.5, 62.0, 22.0 ],
									"style" : "default",
									"text" : "selector~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-35",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 232.0, 197.5, 50.0, 22.0 ],
									"style" : "default",
									"text" : "*~ 100."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-36",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 232.0, 169.5, 47.0, 22.0 ],
									"style" : "default",
									"text" : "noise~"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-37",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 503.0, 351.208800999999994, 38.0, 22.0 ],
									"style" : "default",
									"text" : "clear"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-39",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 45.0, 260.5, 24.0, 24.0 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-40",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 45.0, 286.5, 66.0, 22.0 ],
									"style" : "default",
									"text" : "metro 600"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-41",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 45.0, 314.5, 36.0, 22.0 ],
									"style" : "default",
									"text" : "0.5"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-42",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 561.5, 382.208800999999994, 53.0, 22.0 ],
									"style" : "default",
									"text" : "squelch"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-43",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 245.5, 295.5, 135.0, 22.0 ],
									"style" : "default",
									"text" : "540. 0.5 4. 543.5 0.4 2."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-44",
									"maxclass" : "ezdac~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 189.0, 495.5, 45.0, 45.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-45",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "signal", "list" ],
									"patching_rect" : [ 189.0, 465.5, 178.0, 22.0 ],
									"style" : "default",
									"text" : "resonators~ 480. 0.5 1."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-46",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 85.0, 314.5, 92.0, 33.0 ],
									"style" : "default",
									"text" : "Float: Impulse amplitude"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-47",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 564.0, 360.208800999999994, 66.0, 20.0 ],
									"style" : "default",
									"text" : "fast decay"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-48",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 280.0, 271.5, 187.0, 20.0 ],
									"style" : "default",
									"text" : "frequency, gain, decayrate triples"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-49",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 503.0, 325.5, 158.0, 20.0 ],
									"style" : "default",
									"text" : "set filter state variables to 0"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-50",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 288.0, 171.5, 98.0, 20.0 ],
									"style" : "default",
									"text" : "Excitation signal"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"source" : [ "obj-31", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"source" : [ "obj-32", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-34", 0 ],
									"source" : [ "obj-33", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"source" : [ "obj-34", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-34", 1 ],
									"source" : [ "obj-35", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-35", 0 ],
									"source" : [ "obj-36", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"source" : [ "obj-37", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-40", 0 ],
									"source" : [ "obj-39", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-41", 0 ],
									"source" : [ "obj-40", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"source" : [ "obj-41", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"source" : [ "obj-42", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"source" : [ "obj-43", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"order" : 1,
									"source" : [ "obj-45", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"order" : 0,
									"source" : [ "obj-45", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-44", 1 ],
									"order" : 0,
									"source" : [ "obj-45", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-44", 0 ],
									"order" : 1,
									"source" : [ "obj-45", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-45", 0 ],
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-44", 0 ],
									"midpoints" : [ 79.5, 488.5, 198.5, 488.5 ],
									"source" : [ "obj-8", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 196.0, 164.5, 49.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"tags" : ""
					}
,
					"text" : "p basic"
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
							"major" : 8,
							"minor" : 2,
							"revision" : 1,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"classnamespace" : "box",
						"rect" : [ 0.0, 26.0, 772.0, 570.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 8.0, 8.0 ],
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
						"subpatcher_template" : "",
						"showontab" : 1,
						"assistshowspatchername" : 0,
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-3",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 468.0, 428.0, 71.0, 22.0 ],
									"style" : "default",
									"text" : "what-is-this"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 8,
											"minor" : 2,
											"revision" : 1,
											"architecture" : "x64",
											"modernui" : 1
										}
,
										"classnamespace" : "box",
										"rect" : [ 60.0, 79.0, 307.0, 454.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 8.0, 8.0 ],
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
										"subpatcher_template" : "",
										"assistshowspatchername" : 0,
										"boxes" : [ 											{
												"box" : 												{
													"id" : "obj-1",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "float", "" ],
													"patching_rect" : [ 93.0, 281.215514999999982, 40.0, 22.0 ],
													"style" : "default",
													"text" : "t 0.5 l"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-2",
													"index" : 1,
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 93.0, 340.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-3",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"patching_rect" : [ 93.0, 185.086211999999989, 79.0, 22.0 ],
													"style" : "default",
													"text" : "route symbol"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-4",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 93.0, 217.129302999999993, 27.0, 22.0 ],
													"style" : "default",
													"text" : "iter"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-5",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"patching_rect" : [ 93.0, 249.172424000000007, 181.0, 22.0 ],
													"style" : "default",
													"text" : "zl group 4096 @zlmaxsize 4096"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-6",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 93.0, 100.0, 83.0, 22.0 ],
													"style" : "default",
													"text" : "prepend refer"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-7",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 38.0, 101.0, 41.0, 22.0 ],
													"style" : "default",
													"text" : "dump"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-8",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "bang", "" ],
													"patching_rect" : [ 38.0, 65.0, 74.0, 22.0 ],
													"style" : "default",
													"text" : "t b s"
												}

											}
, 											{
												"box" : 												{
													"coll_data" : 													{
														"count" : 44,
														"data" : [ 															{
																"key" : 2,
																"value" : [ 68.870093999999995, 0.018847, 1.025365 ]
															}
, 															{
																"key" : 3,
																"value" : [ 76.798737000000003, 0.001387, 0.812959 ]
															}
, 															{
																"key" : 4,
																"value" : [ 153.632995999999991, 0.000157, 1.709837 ]
															}
, 															{
																"key" : 5,
																"value" : [ 331.670714999999973, 0.005563, 2.718331 ]
															}
, 															{
																"key" : 6,
																"value" : [ 584.910216999999989, 0.003633, 8.470998 ]
															}
, 															{
																"key" : 7,
																"value" : [ 716.436279000000013, 0.000422, 3.576834 ]
															}
, 															{
																"key" : 8,
																"value" : [ 951.031432999999993, 0.000499, 8.975781 ]
															}
, 															{
																"key" : 9,
																"value" : [ 1081.078735000000052, 0.000219, 1.956324 ]
															}
, 															{
																"key" : 10,
																"value" : [ 1723.979125999999951, 0.000256, 2.408033 ]
															}
, 															{
																"key" : 11,
																"value" : [ 1803.259154999999964, 0.002516, 6.264031 ]
															}
, 															{
																"key" : 12,
																"value" : [ 1874.294311999999991, 0.000192, 3.197672 ]
															}
, 															{
																"key" : 13,
																"value" : [ 1943.138672000000042, 0.000867, 2.433625 ]
															}
, 															{
																"key" : 14,
																"value" : [ 2013.626831000000038, 0.001289, 6.823446 ]
															}
, 															{
																"key" : 15,
																"value" : [ 2123.859618999999839, 0.001192, 6.114969 ]
															}
, 															{
																"key" : 16,
																"value" : [ 3161.855712999999923, 0.000241, 3.362504 ]
															}
, 															{
																"key" : 17,
																"value" : [ 3276.757567999999992, 0.000268, 1.102739 ]
															}
, 															{
																"key" : 18,
																"value" : [ 3352.714599999999791, 0.000177, 2.006015 ]
															}
, 															{
																"key" : 19,
																"value" : [ 3461.062011999999868, 0.000656, 4.602165 ]
															}
, 															{
																"key" : 20,
																"value" : [ 3536.530272999999852, 0.001187, 5.335497 ]
															}
, 															{
																"key" : 21,
																"value" : [ 3605.865233999999873, 0.000268, 4.084842 ]
															}
, 															{
																"key" : 22,
																"value" : [ 4844.053710999999566, 0.000419, 6.805682 ]
															}
, 															{
																"key" : 23,
																"value" : [ 4914.829101999999693, 0.000493, 8.114322 ]
															}
, 															{
																"key" : 24,
																"value" : [ 5588.390625, 0.0008, 9.56846 ]
															}
, 															{
																"key" : 25,
																"value" : [ 8557.279296999999133, 0.000664, 4.417527 ]
															}
, 															{
																"key" : 26,
																"value" : [ 8670.727538999999524, 0.000128, 1.556648 ]
															}
, 															{
																"key" : 27,
																"value" : [ 10511.861328000000867, 0.010369, 13.224074999999999 ]
															}
, 															{
																"key" : 28,
																"value" : [ 12354.094726999999693, 0.002859, 10.128166 ]
															}
, 															{
																"key" : 29,
																"value" : [ 12423.429688000000169, 0.000318, 3.70574 ]
															}
, 															{
																"key" : 30,
																"value" : [ 13866.517578000000867, 0.014462, 16.493029 ]
															}
, 															{
																"key" : 31,
																"value" : [ 13931.472655999999915, 0.001243, 5.532115 ]
															}
, 															{
																"key" : 32,
																"value" : [ 15215.597655999999915, 0.00048, 6.74961 ]
															}
, 															{
																"key" : 33,
																"value" : [ 15447.731444999999439, 0.014499, 15.247563 ]
															}
, 															{
																"key" : 34,
																"value" : [ 15513.826171999999133, 0.002359, 6.18425 ]
															}
, 															{
																"key" : 35,
																"value" : [ 15789.638671999999133, 0.002187, 6.090732 ]
															}
, 															{
																"key" : 36,
																"value" : [ 16195.080078000000867, 0.011736, 17.267841000000001 ]
															}
, 															{
																"key" : 37,
																"value" : [ 17310.613281000001734, 0.001164, 7.856104 ]
															}
, 															{
																"key" : 38,
																"value" : [ 17390.949218999998266, 0.002608, 12.661161999999999 ]
															}
, 															{
																"key" : 39,
																"value" : [ 17483.601561999999831, 0.014441, 16.258586999999999 ]
															}
, 															{
																"key" : 40,
																"value" : [ 17534.939452999999048, 0.002646, 7.879301 ]
															}
, 															{
																"key" : 41,
																"value" : [ 17606.703125, 0.000501, 6.552191 ]
															}
, 															{
																"key" : 42,
																"value" : [ 19207.242188000000169, 0.001623, 7.808167 ]
															}
, 															{
																"key" : 43,
																"value" : [ 19279.248047000000952, 0.000914, 8.128489 ]
															}
, 															{
																"key" : 44,
																"value" : [ 19602.009765999999217, 0.00296, 10.865641 ]
															}
, 															{
																"key" : 45,
																"value" : [ 21790.335938000000169, 0.007668, 10.993763 ]
															}
 ]
													}
,
													"id" : "obj-9",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 4,
													"outlettype" : [ "", "", "", "" ],
													"patching_rect" : [ 93.0, 153.043105999999995, 132.0, 22.0 ],
													"saved_object_attributes" : 													{
														"embed" : 1,
														"precision" : 6
													}
,
													"style" : "default",
													"text" : "coll no-probm 1"
												}

											}
, 											{
												"box" : 												{
													"comment" : "(sym) coll name",
													"id" : "obj-10",
													"index" : 1,
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 38.0, 26.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-2", 0 ],
													"source" : [ "obj-1", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-2", 0 ],
													"source" : [ "obj-1", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-8", 0 ],
													"source" : [ "obj-10", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-4", 0 ],
													"source" : [ "obj-3", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-4", 0 ],
													"source" : [ "obj-3", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-5", 0 ],
													"source" : [ "obj-4", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-1", 0 ],
													"source" : [ "obj-5", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-9", 0 ],
													"source" : [ "obj-6", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-9", 0 ],
													"source" : [ "obj-7", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-6", 0 ],
													"source" : [ "obj-8", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-7", 0 ],
													"source" : [ "obj-8", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-3", 0 ],
													"source" : [ "obj-9", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-5", 0 ],
													"source" : [ "obj-9", 3 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 468.0, 452.0, 98.0, 22.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"tags" : ""
									}
,
									"style" : "default",
									"text" : "p coll-listdumper"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 468.0, 477.0, 103.0, 22.0 ],
									"style" : "default",
									"text" : "s resonators-help"
								}

							}
, 							{
								"box" : 								{
									"coll_data" : 									{
										"count" : 44,
										"data" : [ 											{
												"key" : 2,
												"value" : [ 68.870093999999995, 0.018847, 1.025365 ]
											}
, 											{
												"key" : 3,
												"value" : [ 76.798737000000003, 0.001387, 0.812959 ]
											}
, 											{
												"key" : 4,
												"value" : [ 153.632995999999991, 0.000157, 1.709837 ]
											}
, 											{
												"key" : 5,
												"value" : [ 331.670714999999973, 0.005563, 2.718331 ]
											}
, 											{
												"key" : 6,
												"value" : [ 584.910216999999989, 0.003633, 8.470998 ]
											}
, 											{
												"key" : 7,
												"value" : [ 716.436279000000013, 0.000422, 3.576834 ]
											}
, 											{
												"key" : 8,
												"value" : [ 951.031432999999993, 0.000499, 8.975781 ]
											}
, 											{
												"key" : 9,
												"value" : [ 1081.078735000000052, 0.000219, 1.956324 ]
											}
, 											{
												"key" : 10,
												"value" : [ 1723.979125999999951, 0.000256, 2.408033 ]
											}
, 											{
												"key" : 11,
												"value" : [ 1803.259154999999964, 0.002516, 6.264031 ]
											}
, 											{
												"key" : 12,
												"value" : [ 1874.294311999999991, 0.000192, 3.197672 ]
											}
, 											{
												"key" : 13,
												"value" : [ 1943.138672000000042, 0.000867, 2.433625 ]
											}
, 											{
												"key" : 14,
												"value" : [ 2013.626831000000038, 0.001289, 6.823446 ]
											}
, 											{
												"key" : 15,
												"value" : [ 2123.859618999999839, 0.001192, 6.114969 ]
											}
, 											{
												"key" : 16,
												"value" : [ 3161.855712999999923, 0.000241, 3.362504 ]
											}
, 											{
												"key" : 17,
												"value" : [ 3276.757567999999992, 0.000268, 1.102739 ]
											}
, 											{
												"key" : 18,
												"value" : [ 3352.714599999999791, 0.000177, 2.006015 ]
											}
, 											{
												"key" : 19,
												"value" : [ 3461.062011999999868, 0.000656, 4.602165 ]
											}
, 											{
												"key" : 20,
												"value" : [ 3536.530272999999852, 0.001187, 5.335497 ]
											}
, 											{
												"key" : 21,
												"value" : [ 3605.865233999999873, 0.000268, 4.084842 ]
											}
, 											{
												"key" : 22,
												"value" : [ 4844.053710999999566, 0.000419, 6.805682 ]
											}
, 											{
												"key" : 23,
												"value" : [ 4914.829101999999693, 0.000493, 8.114322 ]
											}
, 											{
												"key" : 24,
												"value" : [ 5588.390625, 0.0008, 9.56846 ]
											}
, 											{
												"key" : 25,
												"value" : [ 8557.279296999999133, 0.000664, 4.417527 ]
											}
, 											{
												"key" : 26,
												"value" : [ 8670.727538999999524, 0.000128, 1.556648 ]
											}
, 											{
												"key" : 27,
												"value" : [ 10511.861328000000867, 0.010369, 13.224074999999999 ]
											}
, 											{
												"key" : 28,
												"value" : [ 12354.094726999999693, 0.002859, 10.128166 ]
											}
, 											{
												"key" : 29,
												"value" : [ 12423.429688000000169, 0.000318, 3.70574 ]
											}
, 											{
												"key" : 30,
												"value" : [ 13866.517578000000867, 0.014462, 16.493029 ]
											}
, 											{
												"key" : 31,
												"value" : [ 13931.472655999999915, 0.001243, 5.532115 ]
											}
, 											{
												"key" : 32,
												"value" : [ 15215.597655999999915, 0.00048, 6.74961 ]
											}
, 											{
												"key" : 33,
												"value" : [ 15447.731444999999439, 0.014499, 15.247563 ]
											}
, 											{
												"key" : 34,
												"value" : [ 15513.826171999999133, 0.002359, 6.18425 ]
											}
, 											{
												"key" : 35,
												"value" : [ 15789.638671999999133, 0.002187, 6.090732 ]
											}
, 											{
												"key" : 36,
												"value" : [ 16195.080078000000867, 0.011736, 17.267841000000001 ]
											}
, 											{
												"key" : 37,
												"value" : [ 17310.613281000001734, 0.001164, 7.856104 ]
											}
, 											{
												"key" : 38,
												"value" : [ 17390.949218999998266, 0.002608, 12.661161999999999 ]
											}
, 											{
												"key" : 39,
												"value" : [ 17483.601561999999831, 0.014441, 16.258586999999999 ]
											}
, 											{
												"key" : 40,
												"value" : [ 17534.939452999999048, 0.002646, 7.879301 ]
											}
, 											{
												"key" : 41,
												"value" : [ 17606.703125, 0.000501, 6.552191 ]
											}
, 											{
												"key" : 42,
												"value" : [ 19207.242188000000169, 0.001623, 7.808167 ]
											}
, 											{
												"key" : 43,
												"value" : [ 19279.248047000000952, 0.000914, 8.128489 ]
											}
, 											{
												"key" : 44,
												"value" : [ 19602.009765999999217, 0.00296, 10.865641 ]
											}
, 											{
												"key" : 45,
												"value" : [ 21790.335938000000169, 0.007668, 10.993763 ]
											}
 ]
									}
,
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "", "", "", "" ],
									"patching_rect" : [ 363.0, 452.0, 103.0, 22.0 ],
									"saved_object_attributes" : 									{
										"embed" : 1,
										"precision" : 6
									}
,
									"style" : "default",
									"text" : "coll what-is-this 1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"linecount" : 6,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 215.0, 428.0, 127.0, 87.0 ],
									"style" : "default",
									"text" : "Storing them in separate collections with one frequency component per index makes it easier to read"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 215.0, 405.0, 27.0, 20.0 ],
									"style" : "default",
									"text" : "3..."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-10",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 214.0, 273.0, 27.0, 20.0 ],
									"style" : "default",
									"text" : "2..."
								}

							}
, 							{
								"box" : 								{
									"coll_data" : 									{
										"count" : 1,
										"data" : [ 											{
												"key" : "tubularbell",
												"value" : [ 55.033996999999999, 0.007688, 0.22365, 57.453999000000003, 0.001074, 0.275675, 64.974991000000003, 0.000344, 1.205618, 67.100998000000004, 0.000343, 1.19569, 74.752990999999994, 0.00055, 0.422953, 80.709984000000006, 0.000554, 4.355912, 89.092995000000002, 0.000561, 2.821024, 99.332001000000005, 0.000362, 0.778361, 106.003997999999996, 0.001197, 0.982313, 130.531997999999987, 0.000953, 1.696837, 137.360001000000011, 0.000392, 2.690805, 148.563004000000006, 0.000366, 2.265277, 157.608001999999999, 0.000513, 3.611323, 165.25, 0.001761, 3.820774, 174.370987000000014, 0.002966, 1.162609, 181.643981999999994, 0.000576, 1.001477, 186.369979999999998, 0.000834, 1.611794, 191.063964999999996, 0.003771, 2.196068, 206.147995000000009, 0.000571, 2.013352, 210.743973000000011, 0.001398, 0.995539, 214.289963, 0.002871, 2.079766, 240.306000000000012, 0.001051, 13.982533, 287.57098400000001, 0.023614, 5.610853, 293.052002000000016, 0.02252, 1.44152, 371.285004000000015, 0.000913, 1.621439, 571.859009000000015, 0.051998, 0.820176, 935.881896999999981, 0.172225, 0.288869, 1367.75, 0.039492, 4.170716, 1370.703125, 0.071488, 3.365682, 1376.578003000000081, 0.088138, 1.652226, 1557.43396000000007, 0.146852, 3.653641, 1879.739501999999902, 0.081108, 1.590494, 1881.360961999999972, 0.050548, 2.569069, 1886.347168000000011, 0.100205, 3.356101, 2238.96875, 0.004383, 3.748203, 2424.685547000000042, 0.003074, 7.335242, 2443.835938000000169, 0.012659, 1.345984, 3051.962891000000127, 0.044948, 6.99262, 3126.86792000000014, 0.017701, 6.850494, 3702.765625, 0.016628, 2.782289, 3989.600586000000021, 0.002295, 16.584969000000001, 4378.521972999999889, 0.018616, 9.424932, 4545.341797000000042, 0.004602, 75.613929999999996, 4705.530762000000323, 0.008206, 5.640258, 4712.428222999999889, 0.013113, 89.576363000000001, 5085.185547000000042, 0.004341, 24.564522, 5191.1875, 0.001106, 29.310265000000001, 5804.791503999999804, 0.002701, 34.254292, 6206.34375, 0.000584, 16.682171, 6238.568847999999889, 0.000975, 18.588646000000001, 6315.72021500000028, 0.004216, 6.86815, 6530.737793000000238, 0.001939, 15.251987, 6662.9375, 0.000525, 3.668626, 7931.773925999999847, 0.004198, 8.956491, 8014.837891000000127, 0.00192, 26.619676999999999, 8839.701171999999133, 0.000511, 46.775199999999998, 9483.171875, 0.002546, 33.862347, 9563.246094000000085, 0.001693, 8.390629000000001, 9952.574219000000085, 0.000427, 47.439774, 11552.598632999999609, 0.000343, 11.630795000000001, 11593.709961000000476, 0.000969, 29.637774, 11601.259765999999217, 0.000865, 40.935738000000001, 12221.638671999999133, 0.000526, 26.091555, 12282.230469000000085, 0.004292, 74.684524999999994, 12834.739257999999609, 0.000656, 14.306746, 12969.5, 0.000366, 20.928431, 12993.409180000000561, 0.000931, 56.391800000000003 ]
											}
 ]
									}
,
									"id" : "obj-16",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "", "", "", "" ],
									"patching_rect" : [ 429.0, 309.0, 61.0, 22.0 ],
									"saved_object_attributes" : 									{
										"embed" : 1,
										"precision" : 6
									}
,
									"style" : "default",
									"text" : "coll bell 1"
								}

							}
, 							{
								"box" : 								{
									"coll_data" : 									{
										"count" : 1,
										"data" : [ 											{
												"key" : "cbass",
												"value" : [ 41.5, 0.08, 0.388112, 83.399994000000007, 0.4, 0.73463, 115.0, 0.3, 5.821497, 125.169990999999996, 0.14, 0.93076, 126.299987999999999, 0.18, 0.717288, 166.25, 0.07, 0.970249, 167.599990999999989, 0.07, 0.831642, 207.269973999999991, 0.03, 1.474098, 208.899993999999992, 0.04, 1.171217, 248.899993999999992, 0.02, 1.669536, 252.139969000000008, 0.02, 1.512206, 286.399993999999992, 0.0125, 1.925262, 287.399993999999992, 0.0125, 1.512206, 333.299987999999985, 0.0125, 1.512206, 335.099976000000026, 0.0135, 0.900255, 375.0, 0.035, 1.670228, 376.799987999999985, 0.035, 1.108857, 415.699981999999977, 0.006, 1.17816, 418.799987999999985, 0.007, 0.693035, 458.299987999999985, 0.025, 4.075054, 461.599976000000026, 0.025, 2.910748, 498.0, 0.007, 2.910748, 499.0, 0.008, 0.977192, 543.0, 0.006, 5.544283, 546.0, 0.0065, 2.910748, 586.0, 0.0125, 11.088566, 591.0, 0.0125, 3.32657, 627.0, 0.0045, 3.645379, 631.0, 0.0045, 2.910748, 667.0, 0.00125, 4.075054, 673.0, 0.00125, 2.199115, 710.0, 0.0015, 5.329429, 713.0, 0.0015, 2.307814, 752.0, 0.0015, 2.349377, 757.0, 0.0065, 19.557445999999999, 793.0, 0.001, 4.892842, 796.0, 0.002, 7.297668, 835.0, 0.00075, 1.884956, 840.0, 0.0017, 7.33232, 875.0, 0.003, 2.633534, 876.69964600000003, 0.006, 8.385726999999999, 917.0, 0.002, 8.870851999999999, 957.299987999999985, 0.003, 8.385726999999999, 968.0, 0.002, 3.880998, 1002.0, 0.002594, 5.472906, 1043.899292000000059, 0.002174, 5.73171, 1086.19958500000007, 0.001819, 6.005406, 1126.09997599999997, 0.001537, 6.27552, 1170.0, 0.001277, 6.586789, 1212.0, 0.00107, 6.899032, 1254.0, 0.000896, 7.226071, 1291.0, 0.000766, 7.526973, 1332.0, 0.000645, 7.875093, 1377.0, 0.000533, 8.275741, 1419.0, 0.000447, 8.668032, 1462.0, 0.000373, 9.088941999999999, 1504.0, 0.000312, 9.519773000000001, 1546.0, 0.000261, 9.971066, 1587.0, 0.00022, 10.432219999999999 ]
											}
 ]
									}
,
									"id" : "obj-17",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "", "", "", "" ],
									"patching_rect" : [ 504.0, 309.0, 67.0, 22.0 ],
									"saved_object_attributes" : 									{
										"embed" : 1,
										"precision" : 6
									}
,
									"style" : "default",
									"text" : "coll bass 1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-18",
									"linecount" : 4,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 214.0, 295.0, 122.0, 60.0 ],
									"style" : "default",
									"text" : "Store them in separate collections and switch between them using refer"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-19",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 356.0, 276.0, 41.0, 22.0 ],
									"style" : "default",
									"text" : "dump"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-20",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 337.0, 340.0, 103.0, 22.0 ],
									"style" : "default",
									"text" : "s resonators-help"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-21",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "float", "bang", "" ],
									"patching_rect" : [ 337.0, 249.0, 57.0, 22.0 ],
									"style" : "default",
									"text" : "t 0.5 b s"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-22",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 406.5, 196.0, 64.0, 22.0 ],
									"style" : "default",
									"text" : "refer bass"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-23",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 337.0, 196.0, 57.0, 22.0 ],
									"style" : "default",
									"text" : "refer bell"
								}

							}
, 							{
								"box" : 								{
									"coll_data" : 									{
										"count" : 1,
										"data" : [ 											{
												"key" : "cbass",
												"value" : [ 41.5, 0.08, 0.388112, 83.399994000000007, 0.4, 0.73463, 115.0, 0.3, 5.821497, 125.169990999999996, 0.14, 0.93076, 126.299987999999999, 0.18, 0.717288, 166.25, 0.07, 0.970249, 167.599990999999989, 0.07, 0.831642, 207.269973999999991, 0.03, 1.474098, 208.899993999999992, 0.04, 1.171217, 248.899993999999992, 0.02, 1.669536, 252.139969000000008, 0.02, 1.512206, 286.399993999999992, 0.0125, 1.925262, 287.399993999999992, 0.0125, 1.512206, 333.299987999999985, 0.0125, 1.512206, 335.099976000000026, 0.0135, 0.900255, 375.0, 0.035, 1.670228, 376.799987999999985, 0.035, 1.108857, 415.699981999999977, 0.006, 1.17816, 418.799987999999985, 0.007, 0.693035, 458.299987999999985, 0.025, 4.075054, 461.599976000000026, 0.025, 2.910748, 498.0, 0.007, 2.910748, 499.0, 0.008, 0.977192, 543.0, 0.006, 5.544283, 546.0, 0.0065, 2.910748, 586.0, 0.0125, 11.088566, 591.0, 0.0125, 3.32657, 627.0, 0.0045, 3.645379, 631.0, 0.0045, 2.910748, 667.0, 0.00125, 4.075054, 673.0, 0.00125, 2.199115, 710.0, 0.0015, 5.329429, 713.0, 0.0015, 2.307814, 752.0, 0.0015, 2.349377, 757.0, 0.0065, 19.557445999999999, 793.0, 0.001, 4.892842, 796.0, 0.002, 7.297668, 835.0, 0.00075, 1.884956, 840.0, 0.0017, 7.33232, 875.0, 0.003, 2.633534, 876.69964600000003, 0.006, 8.385726999999999, 917.0, 0.002, 8.870851999999999, 957.299987999999985, 0.003, 8.385726999999999, 968.0, 0.002, 3.880998, 1002.0, 0.002594, 5.472906, 1043.899292000000059, 0.002174, 5.73171, 1086.19958500000007, 0.001819, 6.005406, 1126.09997599999997, 0.001537, 6.27552, 1170.0, 0.001277, 6.586789, 1212.0, 0.00107, 6.899032, 1254.0, 0.000896, 7.226071, 1291.0, 0.000766, 7.526973, 1332.0, 0.000645, 7.875093, 1377.0, 0.000533, 8.275741, 1419.0, 0.000447, 8.668032, 1462.0, 0.000373, 9.088941999999999, 1504.0, 0.000312, 9.519773000000001, 1546.0, 0.000261, 9.971066, 1587.0, 0.00022, 10.432219999999999 ]
											}
 ]
									}
,
									"id" : "obj-24",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "", "", "", "" ],
									"patching_rect" : [ 356.0, 309.0, 50.5, 22.0 ],
									"saved_object_attributes" : 									{
										"embed" : 1,
										"precision" : 6
									}
,
									"style" : "default",
									"text" : "coll"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-25",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "float", "" ],
									"patching_rect" : [ 374.0, 69.833327999999995, 43.0, 22.0 ],
									"style" : "default",
									"text" : "t 0.5 s"
								}

							}
, 							{
								"box" : 								{
									"allowdrag" : 0,
									"id" : "obj-26",
									"items" : [ "bell", ",", "bass" ],
									"maxclass" : "umenu",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "int", "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 333.5, 39.0, 100.0, 22.0 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"coll_data" : 									{
										"count" : 2,
										"data" : [ 											{
												"key" : "bass",
												"value" : [ 41.5, 0.08, 0.388112, 83.399994000000007, 0.4, 0.73463, 115.0, 0.3, 5.821497, 125.169990999999996, 0.14, 0.93076, 126.299987999999999, 0.18, 0.717288, 166.25, 0.07, 0.970249, 167.599990999999989, 0.07, 0.831642, 207.269973999999991, 0.03, 1.474098, 208.899993999999992, 0.04, 1.171217, 248.899993999999992, 0.02, 1.669536, 252.139969000000008, 0.02, 1.512206, 286.399993999999992, 0.0125, 1.925262, 287.399993999999992, 0.0125, 1.512206, 333.299987999999985, 0.0125, 1.512206, 335.099976000000026, 0.0135, 0.900255, 375.0, 0.035, 1.670228, 376.799987999999985, 0.035, 1.108857, 415.699981999999977, 0.006, 1.17816, 418.799987999999985, 0.007, 0.693035, 458.299987999999985, 0.025, 4.075054, 461.599976000000026, 0.025, 2.910748, 498.0, 0.007, 2.910748, 499.0, 0.008, 0.977192, 543.0, 0.006, 5.544283, 546.0, 0.0065, 2.910748, 586.0, 0.0125, 11.088566, 591.0, 0.0125, 3.32657, 627.0, 0.0045, 3.645379, 631.0, 0.0045, 2.910748, 667.0, 0.00125, 4.075054, 673.0, 0.00125, 2.199115, 710.0, 0.0015, 5.329429, 713.0, 0.0015, 2.307814, 752.0, 0.0015, 2.349377, 757.0, 0.0065, 19.557445999999999, 793.0, 0.001, 4.892842, 796.0, 0.002, 7.297668, 835.0, 0.00075, 1.884956, 840.0, 0.0017, 7.33232, 875.0, 0.003, 2.633534, 876.69964600000003, 0.006, 8.385726999999999, 917.0, 0.002, 8.870851999999999, 957.299987999999985, 0.003, 8.385726999999999, 968.0, 0.002, 3.880998, 1002.0, 0.002594, 5.472906, 1043.899292000000059, 0.002174, 5.73171, 1086.19958500000007, 0.001819, 6.005406, 1126.09997599999997, 0.001537, 6.27552, 1170.0, 0.001277, 6.586789, 1212.0, 0.00107, 6.899032, 1254.0, 0.000896, 7.226071, 1291.0, 0.000766, 7.526973, 1332.0, 0.000645, 7.875093, 1377.0, 0.000533, 8.275741, 1419.0, 0.000447, 8.668032, 1462.0, 0.000373, 9.088941999999999, 1504.0, 0.000312, 9.519773000000001, 1546.0, 0.000261, 9.971066, 1587.0, 0.00022, 10.432219999999999 ]
											}
, 											{
												"key" : "bell",
												"value" : [ 55.033996999999999, 0.007688, 0.22365, 57.453999000000003, 0.001074, 0.275675, 64.974991000000003, 0.000344, 1.205618, 67.100998000000004, 0.000343, 1.19569, 74.752990999999994, 0.00055, 0.422953, 80.709984000000006, 0.000554, 4.355912, 89.092995000000002, 0.000561, 2.821024, 99.332001000000005, 0.000362, 0.778361, 106.003997999999996, 0.001197, 0.982313, 130.531997999999987, 0.000953, 1.696837, 137.360001000000011, 0.000392, 2.690805, 148.563004000000006, 0.000366, 2.265277, 157.608001999999999, 0.000513, 3.611323, 165.25, 0.001761, 3.820774, 174.370987000000014, 0.002966, 1.162609, 181.643981999999994, 0.000576, 1.001477, 186.369979999999998, 0.000834, 1.611794, 191.063964999999996, 0.003771, 2.196068, 206.147995000000009, 0.000571, 2.013352, 210.743973000000011, 0.001398, 0.995539, 214.289963, 0.002871, 2.079766, 240.306000000000012, 0.001051, 13.982533, 287.57098400000001, 0.023614, 5.610853, 293.052002000000016, 0.02252, 1.44152, 371.285004000000015, 0.000913, 1.621439, 571.859009000000015, 0.051998, 0.820176, 935.881896999999981, 0.172225, 0.288869, 1367.75, 0.039492, 4.170716, 1370.703125, 0.071488, 3.365682, 1376.578003000000081, 0.088138, 1.652226, 1557.43396000000007, 0.146852, 3.653641, 1879.739501999999902, 0.081108, 1.590494, 1881.360961999999972, 0.050548, 2.569069, 1886.347168000000011, 0.100205, 3.356101, 2238.96875, 0.004383, 3.748203, 2424.685547000000042, 0.003074, 7.335242, 2443.835938000000169, 0.012659, 1.345984, 3051.962891000000127, 0.044948, 6.99262, 3126.86792000000014, 0.017701, 6.850494, 3702.765625, 0.016628, 2.782289, 3989.600586000000021, 0.002295, 16.584969000000001, 4378.521972999999889, 0.018616, 9.424932, 4545.341797000000042, 0.004602, 75.613929999999996, 4705.530762000000323, 0.008206, 5.640258, 4712.428222999999889, 0.013113, 89.576363000000001, 5085.185547000000042, 0.004341, 24.564522, 5191.1875, 0.001106, 29.310265000000001, 5804.791503999999804, 0.002701, 34.254292, 6206.34375, 0.000584, 16.682171, 6238.568847999999889, 0.000975, 18.588646000000001, 6315.72021500000028, 0.004216, 6.86815, 6530.737793000000238, 0.001939, 15.251987, 6662.9375, 0.000525, 3.668626, 7931.773925999999847, 0.004198, 8.956491, 8014.837891000000127, 0.00192, 26.619676999999999, 8839.701171999999133, 0.000511, 46.775199999999998, 9483.171875, 0.002546, 33.862347, 9563.246094000000085, 0.001693, 8.390629000000001, 9952.574219000000085, 0.000427, 47.439774, 11552.598632999999609, 0.000343, 11.630795000000001, 11593.709961000000476, 0.000969, 29.637774, 11601.259765999999217, 0.000865, 40.935738000000001, 12221.638671999999133, 0.000526, 26.091555, 12282.230469000000085, 0.004292, 74.684524999999994, 12834.739257999999609, 0.000656, 14.306746, 12969.5, 0.000366, 20.928431, 12993.409180000000561, 0.000931, 56.391800000000003 ]
											}
 ]
									}
,
									"id" : "obj-27",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "", "", "", "" ],
									"patching_rect" : [ 398.0, 100.666663999999997, 81.0, 22.0 ],
									"saved_object_attributes" : 									{
										"embed" : 1,
										"precision" : 6
									}
,
									"style" : "default",
									"text" : "coll models 1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-28",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 374.0, 131.5, 103.0, 22.0 ],
									"style" : "default",
									"text" : "s resonators-help"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-29",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 209.0, 66.833327999999995, 160.0, 47.0 ],
									"style" : "default",
									"text" : "Store models in a single collection and look them up by name"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-31",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 209.0, 44.833328000000002, 27.0, 20.0 ],
									"style" : "default",
									"text" : "1..."
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-24", 0 ],
									"source" : [ "obj-19", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-19", 0 ],
									"source" : [ "obj-21", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-20", 0 ],
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-24", 0 ],
									"source" : [ "obj-21", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 0 ],
									"source" : [ "obj-22", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 0 ],
									"source" : [ "obj-23", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-20", 0 ],
									"source" : [ "obj-24", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-27", 0 ],
									"source" : [ "obj-25", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-28", 0 ],
									"source" : [ "obj-25", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-25", 0 ],
									"source" : [ "obj-26", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-28", 0 ],
									"source" : [ "obj-27", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 0 ],
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"source" : [ "obj-4", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 527.0, 164.5, 118.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p \"models in coll #2\""
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
							"major" : 8,
							"minor" : 2,
							"revision" : 1,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"classnamespace" : "box",
						"rect" : [ 0.0, 26.0, 772.0, 570.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 8.0, 8.0 ],
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
						"subpatcher_template" : "",
						"showontab" : 1,
						"assistshowspatchername" : 0,
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 247.0, 216.0, 24.0, 24.0 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "float", "dump", "clear" ],
									"patching_rect" : [ 247.0, 251.0, 97.0, 22.0 ],
									"style" : "default",
									"text" : "t 0.4 dump clear"
								}

							}
, 							{
								"box" : 								{
									"coll_data" : 									{
										"count" : 1,
										"data" : [ 											{
												"key" : "resonances",
												"value" : [ 41.5, 0.08, 0.388112, 83.400002000000001, 0.4, 0.73463, 115.0, 0.3, 5.821497, 125.169998000000007, 0.14, 0.93076, 126.300003000000004, 0.18, 0.717288, 166.25, 0.07, 0.970249, 167.600006000000008, 0.07, 0.831642, 207.270004, 0.03, 1.474098, 208.899993999999992, 0.04, 1.171217, 248.899993999999992, 0.02, 1.669536, 252.139998999999989, 0.02, 1.512206, 286.399993999999992, 0.0125, 1.925262, 287.399993999999992, 0.0125, 1.512206, 333.299987999999985, 0.0125, 1.512206, 335.100006000000008, 0.0135, 0.900255, 375.0, 0.035, 1.670228, 376.799987999999985, 0.035, 1.108857, 415.700012000000015, 0.006, 1.17816, 418.799987999999985, 0.007, 0.693035, 458.299987999999985, 0.025, 4.075054, 461.600006000000008, 0.025, 2.910748, 498.0, 0.007, 2.910748, 499.0, 0.008, 0.977192, 543.0, 0.006, 5.544283, 546.0, 0.0065, 2.910748, 586.0, 0.0125, 11.088566, 591.0, 0.0125, 3.32657, 627.0, 0.0045, 3.645379, 631.0, 0.0045, 2.910748, 667.0, 0.00125, 4.075054, 673.0, 0.00125, 2.2, 710.0, 0.0015, 5.329429, 713.0, 0.0015, 2.307814, 752.0, 0.0015, 2.349377, 757.0, 0.0065, 19.557445999999999, 793.0, 0.001, 4.892842, 796.0, 0.002, 7.297668, 835.0, 0.00075, 1.884956, 840.0, 0.0017, 7.33232, 875.0, 0.003, 2.633534, 876.700012000000015, 0.006, 8.385726999999999, 917.0, 0.002, 8.870851999999999, 957.299987999999985, 0.003, 8.385726999999999, 968.0, 0.002, 3.880998, 1002.0, 0.002594, 5.472906, 1043.90002400000003, 0.002174, 5.73171, 1086.199951000000056, 0.001819, 6.005406, 1126.09997599999997, 0.001537, 6.27552, 1170.0, 0.001277, 6.586789, 1212.0, 0.00107, 6.9, 1254.0, 0.000896, 7.226071, 1291.0, 0.000766, 7.526973, 1332.0, 0.000645, 7.875093, 1377.0, 0.000533, 8.275741, 1419.0, 0.000447, 8.668032, 1462.0, 0.000373, 9.088941999999999, 1504.0, 0.000312, 9.519773000000001, 1546.0, 0.000261, 9.971066, 1587.0, 0.00022, 10.432219999999999 ]
											}
 ]
									}
,
									"id" : "obj-3",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "", "", "", "" ],
									"patching_rect" : [ 286.0, 282.0, 50.5, 22.0 ],
									"saved_object_attributes" : 									{
										"embed" : 1,
										"precision" : 6
									}
,
									"style" : "default",
									"text" : "coll"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 201.0, 175.0, 356.0, 22.0 ],
									"style" : "default",
									"text" : "241.5 0.08 0.4 383.4 0.4 0.73 1115. 0.3 5.8 925.18 0.14 0.93, 0.3"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 201.0, 355.0, 103.0, 22.0 ],
									"style" : "default",
									"text" : "s resonators-help"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 201.0, 148.0, 407.0, 20.0 ],
									"style" : "default",
									"text" : "Message boxes are unwieldy for models with more than a few resonances."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"linecount" : 4,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 348.0, 306.0, 175.0, 60.0 ],
									"style" : "default",
									"text" : "Collections can be used to store lists of floats and provide a textual way to view and edit resonance models."
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-3", 0 ],
									"source" : [ "obj-2", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"source" : [ "obj-2", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"source" : [ "obj-4", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 397.0, 164.5, 119.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p \"models in coll #1\""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-30",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 8,
							"minor" : 2,
							"revision" : 1,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"classnamespace" : "box",
						"rect" : [ 0.0, 26.0, 772.0, 570.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 8.0, 8.0 ],
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
						"subpatcher_template" : "",
						"showontab" : 1,
						"assistshowspatchername" : 0,
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "signal", "list" ],
									"patching_rect" : [ 290.5, 320.0, 177.0, 22.0 ],
									"style" : "default",
									"text" : "resonators~ smooth 480. 0.5 1."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"linecount" : 8,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 279.0, 189.0, 200.0, 114.0 ],
									"style" : "default",
									"text" : "If you expect to change the parameters of the resonators in real-time you probably want to use the \"smooth\" option as shown below. It slows things down slightly but the linearly interpolated filter coefficients will yield less artifacts as you change parameters."
								}

							}
 ],
						"lines" : [  ]
					}
,
					"patching_rect" : [ 254.0, 164.5, 129.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p smooth-interpolation"
				}

			}
 ],
		"lines" : [  ],
		"dependency_cache" : [ 			{
				"name" : "resonators~.mxo",
				"type" : "iLaX"
			}
 ],
		"autosave" : 0
	}

}
