{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 182.0, 84.0, 578.0, 559.0 ],
		"bglocked" : 0,
		"defrect" : [ 182.0, 84.0, 578.0, 559.0 ],
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
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p o.route",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 0,
					"id" : "obj-22",
					"fontname" : "Arial",
					"patching_rect" : [ 114.0, 183.0, 58.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 0.0, 44.0, 578.0, 533.0 ],
						"bglocked" : 0,
						"defrect" : [ 0.0, 44.0, 578.0, 533.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 5.0, 5.0 ],
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
									"text" : "o.route dispatches messages through an OSC address hierarchy.",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-6",
									"fontname" : "Arial",
									"patching_rect" : [ 45.0, 26.0, 460.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-27",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 335.0, 386.0, 83.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-26",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 190.0, 386.0, 83.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-24",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 67.0, 323.0, 83.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "o.route strips off the portion of the address that matched",
									"linecount" : 2,
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-23",
									"fontname" : "Arial",
									"patching_rect" : [ 277.0, 200.0, 161.0, 34.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-17",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ],
									"patching_rect" : [ 311.0, 93.0, 50.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/mysynth/*/freq \"$1\" \n/mysynth/*/gain 0.5 \n/nada \"rien\" \n",
									"linecount" : 4,
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-18",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 311.0, 123.0, 111.0, 62.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-15",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ],
									"patching_rect" : [ 361.0, 460.0, 50.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-16",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ],
									"patching_rect" : [ 311.0, 460.0, 50.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-14",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ],
									"patching_rect" : [ 217.0, 460.0, 50.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-13",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ],
									"patching_rect" : [ 167.0, 460.0, 50.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "<--unmatched (delegation)",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-11",
									"fontname" : "Arial",
									"patching_rect" : [ 277.0, 250.0, 150.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-9",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 197.0, 250.0, 69.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.route /freq /gain",
									"fontsize" : 12.0,
									"numinlets" : 3,
									"numoutlets" : 3,
									"id" : "obj-8",
									"fontname" : "Arial",
									"outlettype" : [ "", "", "FullPacket" ],
									"patching_rect" : [ 311.0, 430.0, 105.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-7",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ],
									"patching_rect" : [ 167.0, 93.0, 50.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/mysynth/2/freq 440 \n/mysynth/2/gain \"$1\" \n/nada \"rien\" \n",
									"linecount" : 3,
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-5",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 167.0, 123.0, 118.0, 48.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.route /freq /gain",
									"fontsize" : 12.0,
									"numinlets" : 3,
									"numoutlets" : 3,
									"id" : "obj-4",
									"fontname" : "Arial",
									"outlettype" : [ "", "", "FullPacket" ],
									"patching_rect" : [ 167.0, 430.0, 105.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.route /1 /2",
									"fontsize" : 12.0,
									"numinlets" : 3,
									"numoutlets" : 3,
									"id" : "obj-3",
									"fontname" : "Arial",
									"outlettype" : [ "", "", "FullPacket" ],
									"patching_rect" : [ 167.0, 323.0, 307.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.route /mysynth",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 2,
									"id" : "obj-2",
									"fontname" : "Arial",
									"outlettype" : [ "", "FullPacket" ],
									"patching_rect" : [ 167.0, 200.0, 99.0, 20.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-16", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 1 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 1 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 1 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 1 ],
									"destination" : [ "obj-27", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-26", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 1 ],
									"destination" : [ "obj-9", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-24", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-2", 0 ],
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
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p \"see also OSC-route\"",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 0,
					"id" : "obj-21",
					"fontname" : "Arial",
					"patching_rect" : [ 245.0, 183.0, 134.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 0.0, 26.0, 578.0, 533.0 ],
						"bglocked" : 0,
						"defrect" : [ 0.0, 26.0, 578.0, 533.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 5.0, 5.0 ],
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
									"text" : "See the OSC-route help file for an overview of the OSC pattern matching syntax",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-22",
									"fontname" : "Arial",
									"patching_rect" : [ 55.0, 49.0, 436.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p who_cares?",
									"fontsize" : 12.0,
									"numinlets" : 0,
									"numoutlets" : 0,
									"id" : "obj-20",
									"fontname" : "Arial",
									"patching_rect" : [ 195.0, 140.0, 85.0, 20.0 ],
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 341.0, 44.0, 532.0, 577.0 ],
										"bglocked" : 0,
										"defrect" : [ 341.0, 44.0, 532.0, 577.0 ],
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
													"maxclass" : "button",
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-13",
													"outlettype" : [ "bang" ],
													"patching_rect" : [ 64.0, 316.0, 20.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "Here, since o.route received all of the messages bundled together, it can dispatch them through the outlets in right to left order observing normal Max convention.",
													"linecount" : 4,
													"fontsize" : 12.0,
													"numinlets" : 1,
													"numoutlets" : 0,
													"id" : "obj-14",
													"fontname" : "Arial",
													"patching_rect" : [ 192.0, 400.0, 252.0, 62.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "/gain 1.",
													"fontsize" : 12.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-15",
													"fontname" : "Arial",
													"outlettype" : [ "" ],
													"patching_rect" : [ 112.0, 340.0, 50.0, 18.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "/freq 440.",
													"fontsize" : 12.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-16",
													"fontname" : "Arial",
													"outlettype" : [ "" ],
													"patching_rect" : [ 88.0, 316.0, 61.0, 18.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "*~ 0.",
													"fontsize" : 12.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-17",
													"fontname" : "Arial",
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 64.0, 488.0, 36.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "cycle~ 440.",
													"fontsize" : 12.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-18",
													"fontname" : "Arial",
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 64.0, 456.0, 72.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "o.route /gain /freq",
													"fontsize" : 12.0,
													"numinlets" : 3,
													"numoutlets" : 3,
													"id" : "obj-19",
													"fontname" : "Arial",
													"outlettype" : [ "", "", "FullPacket" ],
													"patching_rect" : [ 64.0, 400.0, 105.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "o.collect",
													"fontsize" : 12.0,
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-21",
													"fontname" : "Arial",
													"outlettype" : [ "" ],
													"patching_rect" : [ 64.0, 368.0, 82.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "button",
													"numinlets" : 1,
													"numoutlets" : 1,
													"id" : "obj-12",
													"outlettype" : [ "bang" ],
													"patching_rect" : [ 178.0, 55.0, 20.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "In this case, the order that OSC-route will dispatch its messages will be determined by the order in which the messages were added to the OSC bundle.  In many cases this is not a problem, however, it is not always possible to ensure that the contents of the bundle will be in the order you want them in.  ",
													"linecount" : 7,
													"fontsize" : 12.0,
													"numinlets" : 1,
													"numoutlets" : 0,
													"id" : "obj-10",
													"fontname" : "Arial",
													"patching_rect" : [ 208.0, 48.0, 252.0, 103.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "/gain 1.",
													"fontsize" : 12.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-9",
													"fontname" : "Arial",
													"outlettype" : [ "" ],
													"patching_rect" : [ 117.0, 55.0, 50.0, 18.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "/freq 440.",
													"fontsize" : 12.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-8",
													"fontname" : "Arial",
													"outlettype" : [ "" ],
													"patching_rect" : [ 53.0, 55.0, 61.0, 18.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "*~ 0.",
													"fontsize" : 12.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-6",
													"fontname" : "Arial",
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 101.0, 239.0, 36.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "cycle~ 440.",
													"fontsize" : 12.0,
													"numinlets" : 2,
													"numoutlets" : 1,
													"id" : "obj-5",
													"fontname" : "Arial",
													"outlettype" : [ "signal" ],
													"patching_rect" : [ 101.0, 207.0, 72.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "OSC-route /freq /gain",
													"fontsize" : 12.0,
													"numinlets" : 1,
													"numoutlets" : 3,
													"id" : "obj-4",
													"fontname" : "Arial",
													"outlettype" : [ "", "", "" ],
													"patching_rect" : [ 101.0, 151.0, 125.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "OpenSoundControl",
													"fontsize" : 12.0,
													"numinlets" : 1,
													"numoutlets" : 3,
													"id" : "obj-3",
													"fontname" : "Arial",
													"outlettype" : [ "", "", "OSCTimeTag" ],
													"patching_rect" : [ 53.0, 119.0, 113.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "OpenSoundControl",
													"fontsize" : 12.0,
													"numinlets" : 1,
													"numoutlets" : 3,
													"id" : "obj-1",
													"fontname" : "Arial",
													"outlettype" : [ "", "", "OSCTimeTag" ],
													"patching_rect" : [ 53.0, 87.0, 113.0, 20.0 ]
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-8", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 0 ],
													"destination" : [ "obj-6", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 1 ],
													"destination" : [ "obj-6", 1 ],
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
													"source" : [ "obj-3", 1 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-21", 0 ],
													"destination" : [ "obj-19", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-19", 1 ],
													"destination" : [ "obj-18", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-19", 0 ],
													"destination" : [ "obj-17", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-18", 0 ],
													"destination" : [ "obj-17", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-16", 0 ],
													"destination" : [ "obj-21", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-15", 0 ],
													"destination" : [ "obj-21", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-21", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-1", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"fontsize" : 12.0,
										"default_fontname" : "Arial",
										"default_fontsize" : 12.0,
										"fontname" : "Arial",
										"globalpatchername" : "",
										"default_fontface" : 0,
										"fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "o.route behaves like OSC-route except that it has the ability to operate on the contents of OSC bundles.",
									"linecount" : 2,
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-19",
									"fontname" : "Arial",
									"patching_rect" : [ 55.0, 99.0, 436.0, 34.0 ]
								}

							}
 ],
						"lines" : [  ]
					}
,
					"saved_object_attributes" : 					{
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p o.atomize",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 0,
					"id" : "obj-6",
					"fontname" : "Arial",
					"patching_rect" : [ 172.0, 183.0, 71.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 0.0, 44.0, 578.0, 533.0 ],
						"bglocked" : 0,
						"defrect" : [ 0.0, 44.0, 578.0, 533.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 5.0, 5.0 ],
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
									"text" : "with no arguments, o.atomize unpacks the bundle and outputs each message as a Max message.",
									"linecount" : 3,
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-43",
									"fontname" : "Arial",
									"patching_rect" : [ 313.0, 359.0, 217.0, 48.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-42",
									"fontname" : "Arial",
									"patching_rect" : [ 160.0, 449.0, 34.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/mysynth/2/freq 440. \n/mysynth/2/gain 0.5 \n/nada \"rien\" \n",
									"linecount" : 3,
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-41",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 160.0, 359.0, 121.0, 48.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.atomize",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-40",
									"fontname" : "Arial",
									"outlettype" : [ "FullPacket" ],
									"patching_rect" : [ 160.0, 419.0, 61.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "o.atomize dispatches messages through an OSC address hierarchy as Max messages.",
									"linecount" : 2,
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-32",
									"fontname" : "Arial",
									"patching_rect" : [ 112.0, 19.0, 325.0, 34.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-31",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 265.0, 280.0, 119.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-30",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 112.0, 280.0, 119.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.atomize /mysynth/2/freq /mysynth/2/gain",
									"fontsize" : 12.0,
									"numinlets" : 3,
									"numoutlets" : 3,
									"id" : "obj-28",
									"fontname" : "Arial",
									"outlettype" : [ "", "", "FullPacket" ],
									"patching_rect" : [ 160.0, 221.0, 233.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-17",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ],
									"patching_rect" : [ 305.0, 78.0, 50.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/mysynth/*/freq \"$1\" \n/mysynth/*/gain 0.5 \n/nada \"rien\" \n",
									"linecount" : 4,
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-18",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 305.0, 108.0, 111.0, 62.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-7",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ],
									"patching_rect" : [ 160.0, 78.0, 50.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/mysynth/2/freq 440 \n/mysynth/2/gain \"$1\" \n/nada \"rien\" \n",
									"linecount" : 3,
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-5",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 160.0, 108.0, 118.0, 48.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-41", 0 ],
									"destination" : [ "obj-40", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-40", 0 ],
									"destination" : [ "obj-42", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 1 ],
									"destination" : [ "obj-31", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-30", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-28", 0 ],
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
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p o.select",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 0,
					"id" : "obj-1",
					"fontname" : "Arial",
					"patching_rect" : [ 40.0, 183.0, 62.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 182.0, 110.0, 578.0, 533.0 ],
						"bglocked" : 0,
						"defrect" : [ 182.0, 110.0, 578.0, 533.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 5.0, 5.0 ],
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
									"text" : "o.select dispatches messages through an OSC address hierarchy preserving the addresses that matched.",
									"linecount" : 2,
									"fontsize" : 12.0,
									"textcolor" : [ 0.49728, 0.49728, 0.49728, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"frgb" : [ 0.49728, 0.49728, 0.49728, 1.0 ],
									"id" : "obj-12",
									"fontname" : "Arial",
									"patching_rect" : [ 30.0, 77.0, 318.0, 34.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "o.select ",
									"fontsize" : 48.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-1",
									"fontname" : "Arial",
									"patching_rect" : [ 30.0, 20.0, 190.0, 62.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-31",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 200.0, 360.0, 120.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-30",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 47.0, 360.0, 121.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.select /mysynth/2/freq /mysynth/2/gain",
									"fontsize" : 12.0,
									"numinlets" : 3,
									"numoutlets" : 3,
									"id" : "obj-28",
									"fontname" : "Arial",
									"outlettype" : [ "", "", "FullPacket" ],
									"patching_rect" : [ 95.0, 301.0, 225.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-17",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ],
									"patching_rect" : [ 215.0, 158.0, 50.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/mysynth/*/freq \"$1\" \n/mysynth/*/gain 0.5 \n/nada \"rien\" \n",
									"linecount" : 4,
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-18",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 215.0, 188.0, 111.0, 62.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"id" : "obj-7",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ],
									"patching_rect" : [ 95.0, 158.0, 50.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/mysynth/2/freq \"$1\" \n/mysynth/2/gain 0.5 \n/nada \"rien\" \n",
									"linecount" : 4,
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-5",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 95.0, 188.0, 111.0, 62.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 1 ],
									"destination" : [ "obj-31", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-30", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-28", 0 ],
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
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0
					}

				}

			}
 ],
		"lines" : [  ]
	}

}
