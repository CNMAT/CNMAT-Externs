{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 15.0, 49.0, 664.0, 688.0 ],
		"bglocked" : 0,
		"defrect" : [ 15.0, 49.0, 664.0, 688.0 ],
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
					"text" : "*~ 0.2",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 108.5, 589.0, 41.0, 20.0 ],
					"id" : "obj-44",
					"fontname" : "Arial",
					"outlettype" : [ "signal" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 131.0, 561.0, 50.0, 20.0 ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"varname" : "autohelp_dac",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 56.0, 626.0, 45.0, 45.0 ],
					"id" : "obj-12",
					"local" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 0.2",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 20.0, 589.0, 41.0, 20.0 ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"outlettype" : [ "signal" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_dac_text",
					"text" : "start audio",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 15.0, 632.0, 38.0, 33.0 ],
					"id" : "obj-18",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "startwinwdow_panel",
					"numinlets" : 1,
					"border" : 2,
					"numoutlets" : 0,
					"bordercolor" : [ 0.392157, 0.792157, 0.117647, 1.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"patching_rect" : [ 8.0, 621.0, 100.0, 55.0 ],
					"id" : "obj-21"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "autohelp",
					"fontsize" : 11.595187,
					"hidden" : 1,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 504.0, 612.0, 55.0, 20.0 ],
					"id" : "obj-1",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p \"More features\"",
					"fontsize" : 11.595187,
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 511.0, 525.0, 100.0, 20.0 ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 107.0, 75.0, 803.0, 448.0 ],
						"bglocked" : 0,
						"defrect" : [ 107.0, 75.0, 803.0, 448.0 ],
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
									"maxclass" : "newobj",
									"text" : "*~ 0.2",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 177.75, 249.0, 41.0, 20.0 ],
									"id" : "obj-44",
									"fontname" : "Arial",
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 2,
									"patching_rect" : [ 200.0, 220.0, 50.0, 20.0 ],
									"id" : "obj-10",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "ezdac~",
									"varname" : "autohelp_dac",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 130.0, 280.0, 45.0, 45.0 ],
									"id" : "obj-24",
									"local" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "*~ 0.2",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 130.0, 249.0, 41.0, 20.0 ],
									"id" : "obj-25",
									"fontname" : "Arial",
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p modulo_output",
									"fontsize" : 11.595187,
									"numinlets" : 0,
									"numoutlets" : 0,
									"patching_rect" : [ 697.0, 114.0, 97.0, 20.0 ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 466.0, 195.0, 437.0, 130.0 ],
										"bglocked" : 0,
										"defrect" : [ 466.0, 195.0, 437.0, 130.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "modulo output: if a soundfile's # of channels is less than sfplay~'s # of outputs, with modulo output turned on, sfplay~ will reduplicate the soundfile's channels across all of sfplay~'s outputs rather than outputting zero as is otherwise the case--e.g. if a sound file is mono, and sfplay~ has 2 outputs, with modulo output on, the mono file will be played on both outputs, or if a sound file is stereo and sfplay~ has 4 outputs, the left channel will be played on outputs 1 and 3, while the right will be played on outputs 2 and 4, etc.",
													"linecount" : 7,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 15.0, 414.0, 100.0 ],
													"id" : "obj-1",
													"fontname" : "Arial"
												}

											}
 ],
										"lines" : [  ]
									}
,
									"saved_object_attributes" : 									{
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
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 674.0, 114.0, 20.0, 20.0 ],
									"id" : "obj-2",
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "modout $1",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 674.0, 139.0, 65.0, 18.0 ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "name bob",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 608.0, 139.0, 62.0, 18.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "dbl click for info regarding buffer size and channels > 8",
									"linecount" : 2,
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 515.0, 345.0, 154.0, 33.0 ],
									"id" : "obj-5",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p buffer_size",
									"fontsize" : 11.595187,
									"numinlets" : 0,
									"numoutlets" : 0,
									"patching_rect" : [ 534.0, 380.0, 77.0, 20.0 ],
									"id" : "obj-6",
									"fontname" : "Arial",
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 96.0, 156.0, 595.0, 412.0 ],
										"bglocked" : 0,
										"defrect" : [ 96.0, 156.0, 595.0, 412.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "262080",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 168.0, 324.0, 118.0, 20.0 ],
													"id" : "obj-1",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "262080",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 324.0, 95.0, 20.0 ],
													"id" : "obj-2",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "13",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 324.0, 48.0, 20.0 ],
													"id" : "obj-3",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "20160",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 168.0, 339.0, 118.0, 20.0 ],
													"id" : "obj-4",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "20160",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 339.0, 95.0, 20.0 ],
													"id" : "obj-5",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "14",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 339.0, 48.0, 20.0 ],
													"id" : "obj-6",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "20160",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 168.0, 369.0, 118.0, 20.0 ],
													"id" : "obj-7",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "40320",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 369.0, 95.0, 20.0 ],
													"id" : "obj-8",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "16",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 369.0, 48.0, 20.0 ],
													"id" : "obj-9",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "20160",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 168.0, 354.0, 118.0, 20.0 ],
													"id" : "obj-10",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "60480",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 354.0, 95.0, 20.0 ],
													"id" : "obj-11",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "15",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 354.0, 48.0, 20.0 ],
													"id" : "obj-12",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "20160",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 168.0, 309.0, 118.0, 20.0 ],
													"id" : "obj-13",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "60480",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 309.0, 95.0, 20.0 ],
													"id" : "obj-14",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "12",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 309.0, 48.0, 20.0 ],
													"id" : "obj-15",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "221760",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 168.0, 294.0, 118.0, 20.0 ],
													"id" : "obj-16",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "221760",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 294.0, 95.0, 20.0 ],
													"id" : "obj-17",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "11",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 294.0, 48.0, 20.0 ],
													"id" : "obj-18",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "20160",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 168.0, 279.0, 118.0, 20.0 ],
													"id" : "obj-19",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "20160",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 279.0, 95.0, 20.0 ],
													"id" : "obj-20",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "10",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 279.0, 48.0, 20.0 ],
													"id" : "obj-21",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "60480",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 168.0, 264.0, 118.0, 20.0 ],
													"id" : "obj-22",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "181440",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 264.0, 95.0, 20.0 ],
													"id" : "obj-23",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "9",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 264.0, 48.0, 20.0 ],
													"id" : "obj-24",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "for bytewidths except 3, and 8",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 168.0, 247.0, 163.0, 20.0 ],
													"id" : "obj-25",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "for all bytewidths",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 247.0, 95.0, 20.0 ],
													"id" : "obj-26",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "channels",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 247.0, 55.0, 20.0 ],
													"id" : "obj-27",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "Here is a list with some useful values",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 224.0, 459.0, 20.0 ],
													"id" : "obj-28",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "Soundfiles with 9-16 channels are now supported, but it is up to the user to ensure that the disk buffer is an integer multiple of 20160 and the product of the number of channels and the sample type bytewidth-e.g. if one wishes to use 9 channel sound files of all possible bytewidths the diskbuffer must be equal to a multiple 181440=9*20160 bytes. However, if one wishes to use 9 channel sound files that have bytewidths other than 3, the diskbuffer size may be a multiple of 60480=3*20160 bytes because 3 is a factor of 20160.",
													"linecount" : 5,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 145.0, 563.0, 73.0 ],
													"id" : "obj-29",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "The diskbuffer size is rounded to the the nearest multiple of 20160 bytes. At first this might seem like an odd number but it is equal to 8 factorial over 2 (8! = 8*7*6*5*4*3*2*1). This is used so that we can ensure that there is no frame wrap around as we wrap around the diskbuffer end. There will be no frame wrap if the diskbufffer size is an integer multiple of the product of the number of channels and the sample type bytewidth of the soundfile being played. If the diskbuffer size is a multiple of 20160, there will be no wrap for all of the channel/sampletype pairs possible for 8 channels or less (possible bytewidths are 1, 2, 3, 4, and 8--8bit, 16bit, 24bit, 32bit, and 64bit respectively).",
													"linecount" : 7,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 31.0, 527.0, 100.0 ],
													"id" : "obj-30",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "More than 8 channels:",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 130.0, 185.0, 20.0 ],
													"id" : "obj-31",
													"fontname" : "Arial"
												}

											}
 ],
										"lines" : [  ]
									}
,
									"saved_object_attributes" : 									{
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
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p more_cues",
									"fontsize" : 11.595187,
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 415.0, 139.0, 78.0, 20.0 ],
									"id" : "obj-7",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 320.0, 77.0, 719.0, 282.0 ],
										"bglocked" : 0,
										"defrect" : [ 320.0, 77.0, 719.0, 282.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "note:;\rcue preloading and seek are always deferred to low priority. pause, resume, and int messages are not. if you have problems with these messages arriving before you want them to in overdrive mode(i.e. before you've finished seeking to a new location or preloaded the most recent cue), then use the defer object.",
													"linecount" : 7,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 391.0, 65.0, 290.0, 100.0 ],
													"id" : "obj-1",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "speed",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 282.0, 22.0, 61.0, 20.0 ],
													"id" : "obj-2",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 281.0, 41.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-3",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "button",
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 214.0, 73.0, 20.0, 20.0 ],
													"id" : "obj-4",
													"outlettype" : [ "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "length",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 216.0, 22.0, 61.0, 20.0 ],
													"id" : "obj-5",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "speedlim 100",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 15.0, 98.0, 80.0, 20.0 ],
													"id" : "obj-6",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 214.0, 41.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-7",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "+ 500.",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 214.0, 101.0, 43.0, 20.0 ],
													"id" : "obj-8",
													"fontname" : "Arial",
													"outlettype" : [ "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "set $1",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 147.0, 101.0, 40.0, 18.0 ],
													"id" : "obj-9",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "button",
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 15.0, 69.0, 20.0, 20.0 ],
													"id" : "obj-10",
													"outlettype" : [ "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 147.0, 41.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-11",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack 0. 0. 1.",
													"fontsize" : 11.595187,
													"numinlets" : 3,
													"numoutlets" : 1,
													"patching_rect" : [ 147.0, 128.0, 153.0, 20.0 ],
													"id" : "obj-12",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "2",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 365.0, 41.0, 32.5, 18.0 ],
													"id" : "obj-13",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "preload 2 $1 $2 1 $3",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 147.0, 151.0, 115.0, 18.0 ],
													"id" : "obj-14",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 147.0, 209.0, 25.0, 25.0 ],
													"id" : "obj-15",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "start",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 147.0, 22.0, 61.0, 20.0 ],
													"id" : "obj-16",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "this speedlim is a protective measure against preloading too often. in this example it shouldn't be much of a problem though",
													"linecount" : 6,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 15.0, 127.0, 130.0, 86.0 ],
													"id" : "obj-17",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "jump to cue",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 341.0, 22.0, 100.0, 20.0 ],
													"id" : "obj-18",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "preload <cue> (optional filename) <start> <end> <bidirectional buffer flag> <speed>;\ryou might notice two new arguemnts:;\r- bidirectional buffer flag should be set to 1 if you want to be able play back cues in reverse, and 0 if you'd rather save space. (default is 0);\r- speed, relative to the global speed either set by the speed message or signal input(default is 1.);\r",
													"linecount" : 5,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 178.0, 193.0, 513.0, 73.0 ],
													"id" : "obj-19",
													"fontname" : "Arial"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-10", 0 ],
													"hidden" : 0,
													"midpoints" : [ 223.5, 66.0, 24.5, 66.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-10", 0 ],
													"hidden" : 0,
													"midpoints" : [ 156.5, 66.0, 24.5, 66.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-10", 0 ],
													"hidden" : 0,
													"midpoints" : [ 290.5, 66.0, 24.5, 66.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-6", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-9", 0 ],
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
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"midpoints" : [ 24.5, 125.0, 156.5, 125.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-14", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-15", 0 ],
													"hidden" : 0,
													"midpoints" : [ 374.5, 181.0, 156.5, 181.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-15", 0 ],
													"hidden" : 0,
													"midpoints" : [ 156.5, 175.0, 156.5, 175.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [ 223.5, 71.0, 223.5, 71.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"midpoints" : [ 156.5, 95.0, 223.5, 95.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-8", 0 ],
													"destination" : [ "obj-12", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-8", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-12", 2 ],
													"hidden" : 0,
													"midpoints" : [ 290.5, 100.0, 290.5, 100.0 ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
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
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "dbl click to see examples of the signal input support",
									"linecount" : 2,
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 515.0, 275.0, 146.0, 33.0 ],
									"id" : "obj-8",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p signal_inputs",
									"fontsize" : 11.595187,
									"numinlets" : 0,
									"numoutlets" : 0,
									"patching_rect" : [ 533.0, 307.0, 88.0, 20.0 ],
									"id" : "obj-11",
									"fontname" : "Arial",
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 55.0, 71.0, 549.0, 400.0 ],
										"bglocked" : 0,
										"defrect" : [ 55.0, 71.0, 549.0, 400.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "*~ 0.2",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 261.5, 289.0, 41.0, 20.0 ],
													"id" : "obj-44",
													"fontname" : "Arial",
													"outlettype" : [ "signal" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 283.5, 261.0, 50.0, 20.0 ],
													"id" : "obj-14",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "ezdac~",
													"varname" : "autohelp_dac",
													"numinlets" : 2,
													"numoutlets" : 0,
													"patching_rect" : [ 216.0, 320.0, 45.0, 45.0 ],
													"id" : "obj-16",
													"local" : 1
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "*~ 0.2",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 216.0, 289.0, 41.0, 20.0 ],
													"id" : "obj-17",
													"fontname" : "Arial",
													"outlettype" : [ "signal" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "The left input signal may be used to sample accurately trigger cues. The input signal is monitored and when it's integer component changes it switiches to that cue if it exists. Negative values are ignored and are useful in the below example which loops over the same cue.",
													"linecount" : 3,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 12.0, 10.0, 511.0, 46.0 ],
													"id" : "obj-24",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "500",
													"fontsize" : 11.595187,
													"hidden" : 1,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 271.0, 75.0, 28.0, 18.0 ],
													"id" : "obj-1",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "Currently the maximum number of cue switches per vector is 16.",
													"linecount" : 2,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 333.0, 72.0, 174.0, 33.0 ],
													"id" : "obj-2",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"minimum" : 1.0,
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 58.0, 96.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-3",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "cue",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 120.0, 77.0, 36.0, 20.0 ],
													"id" : "obj-4",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "number",
													"minimum" : 0,
													"maximum" : 2,
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 111.0, 96.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-5",
													"fontname" : "Arial",
													"outlettype" : [ "int", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "preload 2 10000 11000 1",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 164.0, 99.0, 140.0, 18.0 ],
													"id" : "obj-6",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "p loop cue",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 58.0, 120.0, 72.0, 20.0 ],
													"id" : "obj-7",
													"fontname" : "Arial",
													"outlettype" : [ "signal" ],
													"patcher" : 													{
														"fileversion" : 1,
														"rect" : [ 40.0, 55.0, 400.0, 326.0 ],
														"bglocked" : 0,
														"defrect" : [ 40.0, 55.0, 400.0, 326.0 ],
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
														"boxes" : [ 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "expr 1/($f1*0.001)",
																	"fontsize" : 11.595187,
																	"numinlets" : 1,
																	"numoutlets" : 1,
																	"patching_rect" : [ 33.0, 74.158882, 103.0, 20.0 ],
																	"id" : "obj-1",
																	"fontname" : "Arial",
																	"outlettype" : [ "" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "*~ 0",
																	"fontsize" : 11.595187,
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"patching_rect" : [ 33.0, 234.532715, 32.5, 20.0 ],
																	"id" : "obj-2",
																	"fontname" : "Arial",
																	"outlettype" : [ "signal" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "-~ 1",
																	"fontsize" : 11.595187,
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"patching_rect" : [ 33.0, 193.224304, 32.5, 20.0 ],
																	"id" : "obj-3",
																	"fontname" : "Arial",
																	"outlettype" : [ "signal" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "*~ 2",
																	"fontsize" : 11.595187,
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"patching_rect" : [ 33.0, 166.495331, 32.5, 20.0 ],
																	"id" : "obj-4",
																	"fontname" : "Arial",
																	"outlettype" : [ "signal" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : ">~ 0.5",
																	"fontsize" : 11.595187,
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"patching_rect" : [ 33.0, 138.551407, 43.0, 20.0 ],
																	"id" : "obj-5",
																	"fontname" : "Arial",
																	"outlettype" : [ "signal" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "phasor~ 1.",
																	"fontsize" : 11.595187,
																	"numinlets" : 2,
																	"numoutlets" : 1,
																	"patching_rect" : [ 33.0, 105.747665, 66.0, 20.0 ],
																	"id" : "obj-6",
																	"fontname" : "Arial",
																	"outlettype" : [ "signal" ]
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "outlet",
																	"numinlets" : 1,
																	"numoutlets" : 0,
																	"patching_rect" : [ 33.0, 286.775696, 25.0, 25.0 ],
																	"id" : "obj-7",
																	"comment" : ""
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "inlet",
																	"numinlets" : 0,
																	"numoutlets" : 1,
																	"patching_rect" : [ 252.668121, 45.0, 25.0, 25.0 ],
																	"id" : "obj-8",
																	"outlettype" : [ "int" ],
																	"comment" : ""
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "inlet",
																	"numinlets" : 0,
																	"numoutlets" : 1,
																	"patching_rect" : [ 33.0, 45.0, 25.0, 25.0 ],
																	"id" : "obj-9",
																	"outlettype" : [ "float" ],
																	"comment" : ""
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "comment",
																	"text" : "oscillate between cue number and negative cue number to retrigger cue (zero would stop the cue)",
																	"linecount" : 2,
																	"fontsize" : 11.595187,
																	"numinlets" : 1,
																	"numoutlets" : 0,
																	"patching_rect" : [ 79.908295, 236.962616, 314.0, 33.0 ],
																	"id" : "obj-10",
																	"fontname" : "Arial"
																}

															}
 ],
														"lines" : [ 															{
																"patchline" : 																{
																	"source" : [ "obj-9", 0 ],
																	"destination" : [ "obj-1", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-1", 0 ],
																	"destination" : [ "obj-6", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-6", 0 ],
																	"destination" : [ "obj-5", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-5", 0 ],
																	"destination" : [ "obj-4", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-4", 0 ],
																	"destination" : [ "obj-3", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-3", 0 ],
																	"destination" : [ "obj-2", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-2", 0 ],
																	"destination" : [ "obj-7", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-8", 0 ],
																	"destination" : [ "obj-2", 1 ],
																	"hidden" : 0,
																	"midpoints" : [ 262.168121, 218.738312, 56.0, 218.738312 ]
																}

															}
 ]
													}
,
													"saved_object_attributes" : 													{
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
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "1",
													"fontsize" : 11.595187,
													"hidden" : 1,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 296.0, 57.0, 18.0, 18.0 ],
													"id" : "obj-8",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "loadbang",
													"fontsize" : 11.595187,
													"hidden" : 1,
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 296.0, 31.0, 56.0, 20.0 ],
													"id" : "obj-9",
													"fontname" : "Arial",
													"outlettype" : [ "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 366.0, 126.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-10",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 307.0, 126.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-11",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack 0. 5000.",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 307.0, 150.0, 78.0, 20.0 ],
													"id" : "obj-12",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "line~",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 2,
													"patching_rect" : [ 307.0, 178.0, 33.0, 20.0 ],
													"id" : "obj-13",
													"fontname" : "Arial",
													"outlettype" : [ "signal", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "open",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 13.0, 122.0, 36.0, 18.0 ],
													"id" : "obj-21",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "sfplay~ 2",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 3,
													"patching_rect" : [ 216.0, 210.0, 110.0, 20.0 ],
													"id" : "obj-22",
													"fontname" : "Arial",
													"outlettype" : [ "signal", "signal", "bang" ],
													"save" : [ "#N", "sfplay~", "", 2, 40320, 0, "", ";" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "ms",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 68.0, 77.0, 36.0, 20.0 ],
													"id" : "obj-23",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "The right, speed signal input is sampled only once per vector",
													"linecount" : 2,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 343.0, 173.0, 169.0, 33.0 ],
													"id" : "obj-25",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"varname" : "autohelp_dac_text",
													"text" : "start audio",
													"linecount" : 2,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 175.0, 327.0, 38.0, 33.0 ],
													"id" : "obj-26",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "panel",
													"varname" : "startwinwdow_panel",
													"numinlets" : 1,
													"border" : 2,
													"numoutlets" : 0,
													"bordercolor" : [ 0.392157, 0.792157, 0.117647, 1.0 ],
													"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
													"patching_rect" : [ 168.0, 316.0, 100.0, 55.0 ],
													"id" : "obj-29"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-1", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 1,
													"midpoints" : [ 280.5, 73.0, 67.5, 73.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-7", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 0 ],
													"destination" : [ "obj-7", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-21", 0 ],
													"destination" : [ "obj-22", 0 ],
													"hidden" : 0,
													"midpoints" : [ 22.5, 183.0, 225.5, 183.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-22", 0 ],
													"hidden" : 0,
													"midpoints" : [ 173.5, 156.0, 225.5, 156.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-22", 0 ],
													"hidden" : 0,
													"midpoints" : [ 67.5, 174.0, 225.5, 174.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 1,
													"midpoints" : [ 305.5, 55.0, 280.5, 55.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 1,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-8", 0 ],
													"destination" : [ "obj-11", 0 ],
													"hidden" : 1,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-12", 0 ],
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
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-22", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-12", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-44", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-17", 1 ],
													"hidden" : 0,
													"midpoints" : [ 293.0, 285.0, 247.5, 285.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-22", 0 ],
													"destination" : [ "obj-17", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-22", 1 ],
													"destination" : [ "obj-44", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-17", 0 ],
													"destination" : [ "obj-16", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-44", 0 ],
													"destination" : [ "obj-16", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
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
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p jump",
									"fontsize" : 11.595187,
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 559.0, 139.0, 45.0, 20.0 ],
									"id" : "obj-12",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 423.0, 54.0, 497.0, 263.0 ],
										"bglocked" : 0,
										"defrect" : [ 423.0, 54.0, 497.0, 263.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "2. jump to cue",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 395.0, 24.0, 83.0, 20.0 ],
													"id" : "obj-1",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "deferred since the loading of cues are deferred",
													"linecount" : 3,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 299.0, 139.0, 100.0, 46.0 ],
													"id" : "obj-2",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "defer",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 250.0, 165.0, 37.0, 20.0 ],
													"id" : "obj-3",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "delay 1",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 250.0, 140.0, 48.0, 20.0 ],
													"id" : "obj-4",
													"fontname" : "Arial",
													"outlettype" : [ "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "toggle",
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 250.0, 85.0, 20.0, 20.0 ],
													"id" : "obj-5",
													"outlettype" : [ "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "gate",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 250.0, 107.0, 33.0, 20.0 ],
													"id" : "obj-6",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "speedlim 100",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 31.0, 107.0, 80.0, 20.0 ],
													"id" : "obj-7",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "set $1",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 142.0, 107.0, 42.0, 18.0 ],
													"id" : "obj-8",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "button",
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 31.0, 85.0, 20.0, 20.0 ],
													"id" : "obj-9",
													"outlettype" : [ "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 142.0, 58.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-10",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack 0. 0. 1",
													"fontsize" : 11.595187,
													"numinlets" : 3,
													"numoutlets" : 1,
													"patching_rect" : [ 142.0, 137.0, 68.0, 20.0 ],
													"id" : "obj-11",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "2",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 414.0, 58.0, 18.0, 18.0 ],
													"id" : "obj-12",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "preload 2 $1 $2 $3",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 142.0, 166.0, 107.0, 18.0 ],
													"id" : "obj-13",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 142.0, 225.0, 25.0, 25.0 ],
													"id" : "obj-14",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "1. set cue point",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 140.0, 18.0, 133.0, 20.0 ],
													"id" : "obj-15",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "start",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 143.0, 39.0, 61.0, 20.0 ],
													"id" : "obj-16",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "this speedlim is a protective measure against preloading too often. in this example it shouldn't be much of a problem though",
													"linecount" : 6,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 14.0, 136.0, 130.0, 86.0 ],
													"id" : "obj-17",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "like seek(but also works in reverse)",
													"linecount" : 2,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 287.0, 93.0, 101.0, 33.0 ],
													"id" : "obj-18",
													"fontname" : "Arial"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-9", 0 ],
													"hidden" : 0,
													"midpoints" : [ 151.5, 82.0, 40.5, 82.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-7", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-11", 0 ],
													"hidden" : 0,
													"midpoints" : [ 40.5, 130.0, 151.5, 130.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-8", 0 ],
													"destination" : [ "obj-11", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-13", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-14", 0 ],
													"hidden" : 0,
													"midpoints" : [ 423.5, 209.0, 151.5, 209.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-14", 0 ],
													"hidden" : 0,
													"midpoints" : [ 151.5, 184.0, 151.5, 184.0 ]
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
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-6", 1 ],
													"hidden" : 0,
													"midpoints" : [ 40.5, 130.0, 235.0, 130.0, 235.0, 81.0, 273.5, 81.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"midpoints" : [ 259.5, 190.0, 406.0, 190.0, 406.0, 53.0, 421.0, 53.0 ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
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
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "print",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 357.0, 141.0, 33.0, 18.0 ],
									"id" : "obj-13",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "dbl click for a slightly nicer way to view position",
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 247.0, 417.0, 252.0, 20.0 ],
									"id" : "obj-14",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p h:m:s:ms",
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 181.0, 415.0, 67.0, 20.0 ],
									"id" : "obj-15",
									"fontname" : "Arial",
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 51.0, 380.0, 378.0, 303.0 ],
										"bglocked" : 0,
										"defrect" : [ 51.0, 380.0, 378.0, 303.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "expr $f1-$i2",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 257.0, 218.0, 72.0, 20.0 ],
													"id" : "obj-1",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "* 1000.",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 257.0, 242.0, 47.0, 20.0 ],
													"id" : "obj-2",
													"fontname" : "Arial",
													"outlettype" : [ "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "expr $f1-$i2",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 180.0, 168.0, 72.0, 20.0 ],
													"id" : "obj-3",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "* 60.",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 180.0, 192.0, 34.0, 20.0 ],
													"id" : "obj-4",
													"fontname" : "Arial",
													"outlettype" : [ "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "number",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 115.0, 11.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-5",
													"fontname" : "Arial",
													"outlettype" : [ "int", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "toggle",
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 71.0, 11.0, 20.0, 20.0 ],
													"id" : "obj-6",
													"outlettype" : [ "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "expr $f1-$i2",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 42.0, 120.0, 72.0, 20.0 ],
													"id" : "obj-7",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "number",
													"triangle" : 0,
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 257.333344, 267.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-8",
													"fontname" : "Arial",
													"outlettype" : [ "int", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "ms",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 307.0, 267.0, 26.0, 20.0 ],
													"id" : "obj-9",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "number",
													"triangle" : 0,
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 180.333328, 267.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-10",
													"fontname" : "Arial",
													"outlettype" : [ "int", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "s",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 230.0, 267.0, 19.0, 20.0 ],
													"id" : "obj-11",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "number",
													"triangle" : 0,
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 103.333336, 267.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-12",
													"fontname" : "Arial",
													"outlettype" : [ "int", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "* 60.",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 103.0, 145.0, 34.0, 20.0 ],
													"id" : "obj-13",
													"fontname" : "Arial",
													"outlettype" : [ "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "m",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 153.0, 267.0, 20.0, 20.0 ],
													"id" : "obj-14",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "number",
													"triangle" : 0,
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 26.0, 267.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-15",
													"fontname" : "Arial",
													"outlettype" : [ "int", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "/ 3600000.",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 26.0, 97.0, 65.0, 20.0 ],
													"id" : "obj-16",
													"fontname" : "Arial",
													"outlettype" : [ "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "metro 100",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 71.0, 37.0, 63.0, 20.0 ],
													"id" : "obj-17",
													"fontname" : "Arial",
													"outlettype" : [ "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "snapshot~",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 26.0, 70.0, 64.0, 20.0 ],
													"id" : "obj-18",
													"fontname" : "Arial",
													"outlettype" : [ "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"patching_rect" : [ 26.0, 13.0, 25.0, 25.0 ],
													"id" : "obj-19",
													"outlettype" : [ "signal" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "h",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 76.0, 267.0, 19.0, 20.0 ],
													"id" : "obj-20",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "<= Turn on metronome to see clock",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 166.0, 11.0, 198.0, 20.0 ],
													"id" : "obj-21",
													"fontname" : "Arial"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-19", 0 ],
													"destination" : [ "obj-18", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-17", 0 ],
													"destination" : [ "obj-18", 0 ],
													"hidden" : 0,
													"midpoints" : [ 80.5, 63.0, 35.5, 63.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-18", 0 ],
													"destination" : [ "obj-16", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-16", 0 ],
													"destination" : [ "obj-15", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-16", 0 ],
													"destination" : [ "obj-7", 0 ],
													"hidden" : 0,
													"midpoints" : [ 35.5, 117.0, 51.5, 117.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-17", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-16", 0 ],
													"destination" : [ "obj-7", 1 ],
													"hidden" : 0,
													"midpoints" : [ 35.5, 116.0, 104.5, 116.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 0 ],
													"destination" : [ "obj-17", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-13", 0 ],
													"hidden" : 0,
													"midpoints" : [ 51.5, 141.0, 112.5, 141.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"midpoints" : [ 112.5, 164.0, 189.5, 164.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-3", 1 ],
													"hidden" : 0,
													"midpoints" : [ 112.5, 164.0, 242.5, 164.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [ 189.5, 189.0, 189.5, 189.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-10", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [ 189.5, 212.0, 266.5, 212.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-1", 1 ],
													"hidden" : 0,
													"midpoints" : [ 189.5, 212.0, 319.5, 212.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-1", 0 ],
													"destination" : [ "obj-2", 0 ],
													"hidden" : 0,
													"midpoints" : [ 266.5, 238.0, 266.5, 238.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-2", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
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
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "current position roundoff error (ms)",
									"linecount" : 2,
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 336.0, 340.0, 105.0, 33.0 ],
									"id" : "obj-16",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 321.0, 214.0, 20.0, 20.0 ],
									"id" : "obj-17",
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p looping",
									"fontsize" : 11.595187,
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 498.0, 139.0, 58.0, 20.0 ],
									"id" : "obj-18",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 322.0, 438.0, 612.0, 254.0 ],
										"bglocked" : 0,
										"defrect" : [ 322.0, 438.0, 612.0, 254.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "3. jump to cue",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 486.0, 11.0, 119.0, 20.0 ],
													"id" : "obj-20",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "2. turn on looping",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 344.0, 11.0, 119.0, 20.0 ],
													"id" : "obj-21",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "toggle",
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 394.0, 32.0, 20.0, 20.0 ],
													"id" : "obj-22",
													"outlettype" : [ "int" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "loop $1",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 394.0, 55.0, 49.0, 18.0 ],
													"id" : "obj-23",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "1. set cue point",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 172.0, 11.0, 133.0, 20.0 ],
													"id" : "obj-24",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "speed",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 284.0, 32.0, 61.0, 20.0 ],
													"id" : "obj-2",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 283.0, 51.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-3",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "button",
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 216.0, 83.0, 20.0, 20.0 ],
													"id" : "obj-4",
													"outlettype" : [ "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "length",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 218.0, 32.0, 61.0, 20.0 ],
													"id" : "obj-5",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "speedlim 100",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 17.0, 108.0, 80.0, 20.0 ],
													"id" : "obj-6",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 216.0, 51.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-7",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "+ 500.",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 216.0, 111.0, 43.0, 20.0 ],
													"id" : "obj-8",
													"fontname" : "Arial",
													"outlettype" : [ "float" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "set $1",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 149.0, 111.0, 40.0, 18.0 ],
													"id" : "obj-9",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "button",
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 17.0, 79.0, 20.0, 20.0 ],
													"id" : "obj-10",
													"outlettype" : [ "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "flonum",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 149.0, 51.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-11",
													"fontname" : "Arial",
													"outlettype" : [ "float", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack 0. 0. 1.",
													"fontsize" : 11.595187,
													"numinlets" : 3,
													"numoutlets" : 1,
													"patching_rect" : [ 149.0, 138.0, 153.0, 20.0 ],
													"id" : "obj-12",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "2",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 521.0, 51.0, 32.5, 18.0 ],
													"id" : "obj-13",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "preload 2 $1 $2 1 $3",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 149.0, 161.0, 115.0, 18.0 ],
													"id" : "obj-14",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 149.0, 219.0, 25.0, 25.0 ],
													"id" : "obj-15",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "start",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 149.0, 32.0, 61.0, 20.0 ],
													"id" : "obj-16",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "this speedlim is a protective measure against preloading too often. in this example it shouldn't be much of a problem though",
													"linecount" : 6,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 17.0, 137.0, 130.0, 86.0 ],
													"id" : "obj-17",
													"fontname" : "Arial"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-23", 0 ],
													"destination" : [ "obj-15", 0 ],
													"hidden" : 0,
													"midpoints" : [ 403.5, 191.0, 158.5, 191.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-22", 0 ],
													"destination" : [ "obj-23", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-10", 0 ],
													"hidden" : 0,
													"midpoints" : [ 225.5, 76.0, 26.5, 76.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-10", 0 ],
													"hidden" : 0,
													"midpoints" : [ 158.5, 76.0, 26.5, 76.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-10", 0 ],
													"hidden" : 0,
													"midpoints" : [ 292.5, 76.0, 26.5, 76.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-6", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-9", 0 ],
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
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"midpoints" : [ 26.5, 135.0, 158.5, 135.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-14", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-15", 0 ],
													"hidden" : 0,
													"midpoints" : [ 530.5, 191.0, 158.5, 191.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-15", 0 ],
													"hidden" : 0,
													"midpoints" : [ 158.5, 185.0, 158.5, 185.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [ 225.5, 81.0, 225.5, 81.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"midpoints" : [ 158.5, 105.0, 225.5, 105.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-8", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-8", 0 ],
													"destination" : [ "obj-12", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-8", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-12", 2 ],
													"hidden" : 0,
													"midpoints" : [ 292.5, 110.0, 292.5, 110.0 ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
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
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p raw_data_and_force_header",
									"fontsize" : 11.595187,
									"numinlets" : 0,
									"numoutlets" : 1,
									"patching_rect" : [ 397.0, 115.0, 169.0, 20.0 ],
									"id" : "obj-19",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 20.0, 52.0, 681.0, 209.0 ],
										"bglocked" : 0,
										"defrect" : [ 20.0, 52.0, 681.0, 209.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "openraw",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 10.0, 88.0, 55.0, 18.0 ],
													"id" : "obj-1",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "number of channels",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 538.0, 66.0, 110.0, 20.0 ],
													"id" : "obj-2",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "sampling rate (Hz)",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 422.0, 66.0, 103.0, 20.0 ],
													"id" : "obj-3",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "sample start offset in bytes. useful for aligning your samples and avoiding playback of header information",
													"linecount" : 5,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 270.0, 11.0, 127.0, 73.0 ],
													"id" : "obj-4",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 10.0, 159.0, 25.0, 25.0 ],
													"id" : "obj-5",
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "prepend samptype",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 1,
													"patching_rect" : [ 111.0, 115.0, 107.0, 20.0 ],
													"id" : "obj-6",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "umenu",
													"fontsize" : 11.595187,
													"items" : [ "int8", ",", "int16", ",", "int24", ",", "int32", ",", "float32", ",", "float64", ",", "mulaw", ",", "alaw" ],
													"numinlets" : 1,
													"numoutlets" : 3,
													"types" : [  ],
													"patching_rect" : [ 111.0, 88.0, 100.0, 20.0 ],
													"id" : "obj-7",
													"fontname" : "Arial",
													"outlettype" : [ "int", "", "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "number",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 565.0, 88.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-8",
													"fontname" : "Arial",
													"outlettype" : [ "int", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "srcchans $1",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 565.0, 115.0, 73.0, 18.0 ],
													"id" : "obj-9",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "number",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 441.0, 88.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-10",
													"fontname" : "Arial",
													"outlettype" : [ "int", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "srate $1",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 441.0, 115.0, 52.0, 18.0 ],
													"id" : "obj-11",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "number",
													"fontsize" : 11.595187,
													"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
													"numinlets" : 1,
													"numoutlets" : 2,
													"patching_rect" : [ 296.0, 88.0, 50.0, 20.0 ],
													"triscale" : 0.9,
													"id" : "obj-12",
													"fontname" : "Arial",
													"outlettype" : [ "int", "bang" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "offset $1",
													"fontsize" : 11.595187,
													"numinlets" : 2,
													"numoutlets" : 1,
													"patching_rect" : [ 296.0, 115.0, 55.0, 18.0 ],
													"id" : "obj-13",
													"fontname" : "Arial",
													"outlettype" : [ "" ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "sampletype takes an optional third arg for byteswap:\n0=noswap, 1=swap",
													"linecount" : 3,
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 106.0, 38.0, 162.0, 46.0 ],
													"id" : "obj-14",
													"fontname" : "Arial"
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "comment",
													"text" : "openraw defaults to 16bit stereo, 44100 Hz, offset=0",
													"fontsize" : 11.595187,
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 37.0, 162.0, 280.0, 20.0 ],
													"id" : "obj-15",
													"fontname" : "Arial"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-1", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [ 574.5, 143.0, 19.5, 143.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [ 450.5, 143.0, 19.5, 143.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-13", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [ 305.5, 143.0, 19.5, 143.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [ 120.5, 143.0, 19.5, 143.0 ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 1 ],
													"destination" : [ "obj-6", 0 ],
													"hidden" : 0,
													"midpoints" : [ 161.0, 111.0, 120.5, 111.0 ]
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
													"source" : [ "obj-10", 0 ],
													"destination" : [ "obj-11", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-8", 0 ],
													"destination" : [ "obj-9", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
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
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 11.595187,
									"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
									"numinlets" : 1,
									"numoutlets" : 2,
									"patching_rect" : [ 303.0, 117.0, 50.0, 20.0 ],
									"triscale" : 0.9,
									"id" : "obj-20",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "seek $1",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 303.0, 141.0, 51.0, 18.0 ],
									"id" : "obj-21",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number~",
									"interval" : 250.0,
									"sig" : 0.0,
									"fontsize" : 11.595187,
									"mode" : 2,
									"numinlets" : 2,
									"numoutlets" : 2,
									"patching_rect" : [ 335.0, 320.0, 102.0, 20.0 ],
									"id" : "obj-22",
									"fontname" : "Arial",
									"outlettype" : [ "signal", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number~",
									"interval" : 250.0,
									"sig" : 0.0,
									"fontsize" : 11.595187,
									"mode" : 2,
									"numinlets" : 2,
									"numoutlets" : 2,
									"patching_rect" : [ 226.0, 320.0, 102.0, 20.0 ],
									"id" : "obj-23",
									"fontname" : "Arial",
									"outlettype" : [ "signal", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "resume",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 251.0, 141.0, 49.0, 18.0 ],
									"id" : "obj-27",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "pause",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 206.0, 141.0, 42.0, 18.0 ],
									"id" : "obj-28",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 11.595187,
									"presentation_rect" : [ 0.0, 0.0, 50.0, 18.0 ],
									"numinlets" : 1,
									"numoutlets" : 2,
									"patching_rect" : [ 145.0, 117.0, 50.0, 20.0 ],
									"triscale" : 0.9,
									"id" : "obj-30",
									"fontname" : "Arial",
									"outlettype" : [ "float", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "speed $1",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 145.0, 141.0, 58.0, 18.0 ],
									"id" : "obj-31",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"patching_rect" : [ 100.0, 144.0, 20.0, 20.0 ],
									"id" : "obj-32",
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "open",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 1,
									"patching_rect" : [ 48.0, 144.0, 36.0, 18.0 ],
									"id" : "obj-33",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "sfplay~ 2 60000 2",
									"fontsize" : 11.595187,
									"numinlets" : 2,
									"numoutlets" : 5,
									"patching_rect" : [ 130.0, 185.0, 210.0, 20.0 ],
									"id" : "obj-34",
									"fontname" : "Arial",
									"outlettype" : [ "signal", "signal", "signal", "signal", "bang" ],
									"save" : [ "#N", "sfplay~", "", 2, 60000, 2, "", ";" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Sfplay~ supports variable speed playback, sample accurate cue looping and triggering, signal outputs for playback position, correct playback of files with sampling rates different than the current MSP sampling rate, support for new file formats including NeXT/SUN(.au), WAVE, and Raw Data, support for new sample types including 32bit float, 64bit double, 8bit mulaw, and 8bit a-law, and forced header munging.",
									"linecount" : 4,
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 25.0, 29.0, 601.0, 60.0 ],
									"id" : "obj-36",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "args: (optional sflist~ name), channels, diskbuffer size(0=use default), # of position outlets, (optional name for sfplay~);\rIn this patch I have the disk buffer set high so that we can playback from disk at high speeds., and two position signal outputs. If the # of position outlets were set to 1, we would only see the single 32bit precision outlet.",
									"linecount" : 5,
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 351.0, 181.0, 382.0, 73.0 ],
									"id" : "obj-37",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "current position(ms)",
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 221.0, 342.0, 111.0, 20.0 ],
									"id" : "obj-38",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "add these two signals using \"hr\" objects to get sample accurate position values for long files",
									"linecount" : 2,
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 203.0, 374.0, 248.0, 33.0 ],
									"id" : "obj-39",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "dbl click for more",
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 432.0, 95.0, 98.0, 20.0 ],
									"id" : "obj-40",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "start/stop",
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 85.0, 120.0, 58.0, 20.0 ],
									"id" : "obj-41",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"varname" : "autohelp_dac_text",
									"text" : "start audio",
									"linecount" : 2,
									"fontsize" : 11.595187,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 89.0, 287.0, 38.0, 33.0 ],
									"id" : "obj-26",
									"fontname" : "Arial"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "panel",
									"varname" : "startwinwdow_panel",
									"numinlets" : 1,
									"border" : 2,
									"numoutlets" : 0,
									"bordercolor" : [ 0.392157, 0.792157, 0.117647, 1.0 ],
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"patching_rect" : [ 82.0, 276.0, 100.0, 55.0 ],
									"id" : "obj-29"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-34", 2 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [ 235.0, 311.0, 190.5, 311.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 683.5, 171.0, 139.5, 171.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 568.5, 171.0, 139.5, 171.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 406.5, 171.0, 390.0, 171.0, 390.0, 171.0, 139.5, 171.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 312.5, 171.0, 139.5, 171.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 215.5, 171.0, 139.5, 171.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 260.5, 171.0, 139.5, 171.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-31", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 154.5, 171.0, 139.5, 171.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-32", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 109.5, 180.0, 139.5, 180.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-33", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 57.5, 180.0, 139.5, 180.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 366.5, 171.0, 139.5, 171.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 424.5, 171.0, 139.5, 171.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 617.5, 171.0, 139.5, 171.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 2 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"midpoints" : [ 235.0, 206.0, 235.5, 317.0 ]
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
									"source" : [ "obj-34", 3 ],
									"destination" : [ "obj-22", 0 ],
									"hidden" : 0,
									"midpoints" : [ 282.75, 305.0, 344.5, 305.0 ]
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
									"source" : [ "obj-34", 4 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-44", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-25", 1 ],
									"hidden" : 0,
									"midpoints" : [ 209.5, 245.0, 161.5, 245.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 1 ],
									"destination" : [ "obj-44", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-25", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [ 507.5, 171.0, 139.5, 171.0 ]
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
									"source" : [ "obj-44", 0 ],
									"destination" : [ "obj-24", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
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
					"maxclass" : "message",
					"text" : "set",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 510.0, 388.0, 32.5, 18.0 ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "set foo",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 329.0, 357.0, 43.0, 18.0 ],
					"id" : "obj-15",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "foo 2",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 155.0, 234.0, 34.0, 18.0 ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "embed 0",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 527.0, 460.0, 54.0, 18.0 ],
					"id" : "obj-19",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "embed 1",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 329.0, 460.0, 54.0, 18.0 ],
					"id" : "obj-20",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "signal out Ch 1",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 29.0, 520.0, 61.0, 33.0 ],
					"id" : "obj-22",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 329.0, 430.0, 34.0, 18.0 ],
					"id" : "obj-23",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear 2 3",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 329.0, 404.0, 53.0, 18.0 ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear 4",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 329.0, 381.0, 45.0, 18.0 ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "define/redefine cue 5 to start at 900 ms in the file foo.aiff and play to 1200 ms",
					"linecount" : 3,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 479.0, 313.0, 151.0, 46.0 ],
					"id" : "obj-26",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "preload 5 foo.aiff 900 1020",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 329.0, 327.0, 149.0, 18.0 ],
					"id" : "obj-27",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "define/redefine cue 4 to start at 800 ms in the file foo.aiff and play to 1200 ms",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 438.0, 278.0, 213.0, 33.0 ],
					"id" : "obj-28",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "preload 4 800 1200",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 329.0, 286.0, 110.0, 18.0 ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "define/redefine cue 3 to start at 800 ms in the file foo.aiff and play to the end",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 447.0, 238.0, 213.0, 33.0 ],
					"id" : "obj-30",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "preload 3 foo.aiff 800",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 329.0, 246.0, 119.0, 18.0 ],
					"id" : "obj-31",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "preload 2 700",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 329.0, 209.0, 81.0, 18.0 ],
					"id" : "obj-32",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 3 4",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 103.0, 433.0, 46.0, 18.0 ],
					"id" : "obj-33",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "seek 300",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 103.0, 399.0, 58.0, 18.0 ],
					"id" : "obj-34",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "seek 300 600",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 97.0, 363.0, 80.0, 18.0 ],
					"id" : "obj-35",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "resume",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 92.0, 325.0, 49.0, 18.0 ],
					"id" : "obj-36",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "pause",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 86.0, 292.0, 42.0, 18.0 ],
					"id" : "obj-37",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 79.0, 266.0, 18.0, 18.0 ],
					"id" : "obj-38",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "2",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 69.0, 234.0, 18.0, 18.0 ],
					"id" : "obj-39",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 56.0, 206.0, 18.0, 18.0 ],
					"id" : "obj-40",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "open",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 20.0, 179.0, 36.0, 18.0 ],
					"id" : "obj-41",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 197.0, 539.0, 20.0, 20.0 ],
					"id" : "obj-42",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sfplay~ 2",
					"fontsize" : 11.595187,
					"numinlets" : 2,
					"numoutlets" : 3,
					"patching_rect" : [ 20.0, 497.0, 196.0, 20.0 ],
					"id" : "obj-43",
					"fontname" : "Arial",
					"outlettype" : [ "signal", "signal", "bang" ],
					"save" : [ "#N", "sfplay~", "", 2, 40320, 0, "", ";" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "bang when cue, file, seek, or list of cues is finished.",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 217.0, 539.0, 274.0, 20.0 ],
					"id" : "obj-45",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "open an audio file or set the current file",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 56.0, 172.0, 117.0, 33.0 ],
					"id" : "obj-46",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play current file from beginning to end",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 73.0, 204.0, 221.0, 20.0 ],
					"id" : "obj-47",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play cue 2 (if defined)",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 86.0, 226.0, 65.0, 33.0 ],
					"id" : "obj-48",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "stop playing",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 98.0, 265.0, 70.0, 20.0 ],
					"id" : "obj-49",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "pause playback",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 128.0, 291.0, 91.0, 20.0 ],
					"id" : "obj-50",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "resume from where paused, unless another repositioning message was received",
					"linecount" : 3,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 141.0, 309.0, 157.0, 46.0 ],
					"id" : "obj-51",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play cues, one after another. Cues not defined are skipped.",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 149.0, 425.0, 163.0, 33.0 ],
					"id" : "obj-52",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play current file from 300 ms to 600 ms",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 176.0, 356.0, 117.0, 33.0 ],
					"id" : "obj-53",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play from 300 ms to the end of the file",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 161.0, 391.0, 114.0, 33.0 ],
					"id" : "obj-54",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "define/redefine cue 2 to start at 700 ms in the current file and play to the end",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 412.0, 201.0, 213.0, 33.0 ],
					"id" : "obj-55",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "arguments set optional sflist~ object to take cues from, number of channels (default 1, max 32) and \"buffer size\" (default 80320 per channel, minimum 20160)",
					"linecount" : 3,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 218.0, 485.0, 304.0, 46.0 ],
					"id" : "obj-56",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Defining Cues:",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 326.0, 186.0, 85.0, 20.0 ],
					"id" : "obj-57",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "remove cue 4",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 372.0, 381.0, 100.0, 20.0 ],
					"id" : "obj-58",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "remove cues 2 and 3",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 380.0, 404.0, 117.0, 20.0 ],
					"id" : "obj-59",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "remove all cues",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 367.0, 430.0, 90.0, 20.0 ],
					"id" : "obj-60",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "signal out Ch 2",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 130.0, 521.0, 61.0, 33.0 ],
					"id" : "obj-61",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "saves cues with patcher, reload when reopening.",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 385.0, 452.0, 139.0, 33.0 ],
					"id" : "obj-62",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "don't save cues (default)",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 582.0, 451.0, 79.0, 33.0 ],
					"id" : "obj-63",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "play cue 2 from sflist~ object named foo",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 190.0, 226.0, 124.0, 33.0 ],
					"id" : "obj-65",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "int plays cues from sflist~ foo object",
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 373.0, 357.0, 192.0, 20.0 ],
					"id" : "obj-66",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "int plays cues defined internally",
					"linecount" : 2,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 542.0, 381.0, 96.0, 33.0 ],
					"id" : "obj-67",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "sfplay~",
					"fontface" : 3,
					"fontsize" : 20.871338,
					"frgb" : [ 0.2, 0.2, 0.2, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 8.0, 485.0, 30.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"textcolor" : [ 0.2, 0.2, 0.2, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "Play audio file from disk",
					"fontsize" : 12.754705,
					"frgb" : [ 0.2, 0.2, 0.2, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 36.0, 485.0, 21.0 ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"textcolor" : [ 0.2, 0.2, 0.2, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_description",
					"text" : "sfplay~ plays AIFF, SD II, NeXT/SUN(.au), WAVE, and Raw Data files of 1-8 tracks from disk. To play a file, you send sfplay~ the open message, then send it a 1 to start and a 0 to stop. open takes an argument to specify a filename in the search path. You can also create additional cues with the preload message. These can reference other files, all of which are simultaneously accessible. The open message sets the \"current\" file: the one that plays back from the beginning when 1 is sent and is used as the default for the preload message. sfplay~ can also connect to the cues defined in an sflist~ object. Since multiple sfplay~ objects can reference the same sflist~, this allows you to store a global list of cues.",
					"linecount" : 7,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 57.0, 565.0, 100.0 ],
					"id" : "obj-4",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textbutton",
					"varname" : "autohelp_top_ref",
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"spacing_x" : 0.0,
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontface" : 3,
					"fontsize" : 12.754705,
					"spacing_y" : 0.0,
					"presentation_rect" : [ 0.0, 0.0, 147.299072, 14.666666 ],
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"numinlets" : 1,
					"fontlink" : 1,
					"textovercolor" : [ 0.4, 0.5, 0.65, 1.0 ],
					"numoutlets" : 3,
					"text" : "open sfplay~ reference",
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"patching_rect" : [ 352.700928, 22.0, 147.299072, 14.666666 ],
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"outlettype" : [ "int", "", "int" ],
					"underline" : 1,
					"textcolor" : [ 0.283, 0.365, 0.488, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_see_title",
					"text" : "See Also:",
					"fontface" : 1,
					"fontsize" : 11.595187,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 509.0, 616.0, 100.0, 20.0 ],
					"id" : "obj-7",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"varname" : "autohelp_see_menu",
					"fontsize" : 11.595187,
					"items" : [ "(Objects:)", ",", "buffer~", ",", "groove~", ",", "play~", ",", "sfinfo~", ",", "sflist~", ",", "sfrecord~", ",", "<separator>", ",", "(Tutorials:)", ",", "MSP Tutorial 16: Record and Play Audio Files" ],
					"numinlets" : 1,
					"numoutlets" : 3,
					"types" : [  ],
					"patching_rect" : [ 509.0, 636.0, 130.0, 20.0 ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"outlettype" : [ "int", "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel",
					"grad2" : [ 0.9, 0.9, 0.9, 1.0 ],
					"mode" : 1,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 5.0, 5.0, 495.0, 52.0 ],
					"id" : "obj-6",
					"grad1" : [ 0.88, 0.98, 0.78, 1.0 ],
					"background" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_see_panel",
					"numinlets" : 1,
					"border" : 2,
					"numoutlets" : 0,
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ],
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ],
					"patching_rect" : [ 504.0, 612.0, 140.0, 50.0 ],
					"id" : "obj-9",
					"background" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 2 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-17", 1 ],
					"hidden" : 0,
					"midpoints" : [ 140.5, 585.0, 51.5, 585.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [ 29.5, 614.0, 65.5, 614.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 1 ],
					"destination" : [ "obj-44", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-44", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 0 ],
					"destination" : [ "obj-12", 1 ],
					"hidden" : 0,
					"midpoints" : [ 118.0, 614.0, 91.5, 614.0 ]
				}

			}
 ]
	}

}
