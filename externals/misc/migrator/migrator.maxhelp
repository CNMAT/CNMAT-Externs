{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 658.0, 44.0, 679.0, 727.0 ],
		"bglocked" : 0,
		"defrect" : [ 658.0, 44.0, 679.0, 727.0 ],
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
		"metadata" : [  ],
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "tellmeeverything",
					"numoutlets" : 1,
					"bgcolor" : [ 0.6, 0.6, 1.0, 1.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-52",
					"fontname" : "Arial",
					"patching_rect" : [ 302.0, 441.0, 76.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"patching_rect" : [ 307.0, 407.0, 63.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p output modes",
					"numoutlets" : 0,
					"id" : "obj-2",
					"fontname" : "Arial",
					"patching_rect" : [ 498.0, 624.0, 77.0, 17.0 ],
					"numinlets" : 0,
					"fontsize" : 9.0,
					"color" : [ 0.611765, 0.701961, 1.0, 1.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 33.0, 59.0, 820.0, 338.0 ],
						"bglocked" : 0,
						"defrect" : [ 33.0, 59.0, 820.0, 338.0 ],
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
						"metadata" : [  ],
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "The second outlet is good for using the migrator to select random pitches from a timbre using the amplitudes of the timbre as probability of occurence.",
									"linecount" : 3,
									"numoutlets" : 0,
									"id" : "obj-1",
									"fontname" : "Arial",
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"patching_rect" : [ 464.0, 106.0, 243.0, 38.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "metro 250",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"patching_rect" : [ 384.0, 45.0, 58.0, 17.0 ],
									"numinlets" : 2,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"id" : "obj-3",
									"patching_rect" : [ 384.0, 25.0, 15.0, 15.0 ],
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "kslider",
									"numoutlets" : 2,
									"outlettype" : [ "int", "int" ],
									"presentation_rect" : [ 0.0, 0.0, 336.0, 54.0 ],
									"id" : "obj-4",
									"patching_rect" : [ 414.0, 188.0, 336.0, 54.0 ],
									"hkeycolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ftom",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"id" : "obj-5",
									"fontname" : "Arial",
									"patching_rect" : [ 414.0, 162.0, 31.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "migrator 0. 5 0.03",
									"numoutlets" : 4,
									"outlettype" : [ "list", "float", "int", "bang" ],
									"id" : "obj-6",
									"fontname" : "Arial",
									"patching_rect" : [ 384.0, 74.0, 101.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0,
									"color" : [ 0.4, 0.4, 0.8, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p data",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"id" : "obj-7",
									"fontname" : "Arial",
									"patching_rect" : [ 273.0, 46.0, 37.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 39.0, 59.0, 527.0, 752.0 ],
										"bglocked" : 0,
										"defrect" : [ 39.0, 59.0, 527.0, 752.0 ],
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
										"metadata" : [  ],
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numoutlets" : 0,
													"id" : "obj-1",
													"patching_rect" : [ 18.0, 622.0, 15.0, 15.0 ],
													"numinlets" : 1,
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"id" : "obj-2",
													"patching_rect" : [ 18.0, 64.0, 15.0, 15.0 ],
													"numinlets" : 0,
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "44.428703 0.001205 105.987946 0.005561 144.132431 0.002473 183.831909 0.00142 301.025726 0.004145 301.322327 0.004067 343.628357 0.001565 406.502655 0.005348 442.231934 0.003312 522.951904 0.005215 557.567505 0.015346 602.613892 0.002343 649.766907 0.004004 664.45575 0.002506 693.972351 0.015189 733.67865 0.002115 768.932007 0.009084 785.913269 0.012306 839.502014 0.006151 875.094177 0.005993 877.02771 0.009862 879.910217 0.005887 945.953247 0.007129 1012.840332 0.012604 1044.641846 0.007745 1115.045166 0.007925 1136.086426 0.00227 1175.845703 0.005228 1240.114258 0.003953 1258.286621 0.001868 1318.401489 0.020711 1345.764038 0.003522 1358.986084 0.006174 1385.278442 0.011233 1398.879517 0.014475 1431.438965 0.016383 1540.99292 0.014028 1635.66748 0.010117 1679.655151 0.006543 1750.221924 0.001598 1797.57 0.001072 1855.189087 0.007052 1916.9 0.002683 1943.572876 0.010738 1971.103516 0.00379 1982.150879 0.010626 2015.547485 0.018401 2034.003662 0.001115 2058.932861 0.012229 2114.809082 0.002263 2139.052246 0.035608 2227.550781 0.003884 2270.133057 0.014658 2319.095459 0.006923 2346.848877 0.005677 2374.3 0.003562 2410.141113 0.005541 2480.320801 0.005741 2545.730469 0.000959 2670.65 0.028469 2687.073242 0.015307 2722.737305 0.010995 2775.491455 0.00659 2854.015869 0.003388 2909.356934 0.002405 2935.9 0.002298 2942.420166 0.00388 2972.062744 0.008935 3040.323242 0.00169 3102.340576 0.010847 3243.788086 0.006875 3264.5979 0.012196 3360.085693 0.001174 3404.677246 0.00133 3561.624512 0.00218 3630.240967 0.005584 3784.453857 0.003829 3798.970459 0.017009 3808.286133 0.015151 3818.577637 0.007675 3833.684082 0.007295 3844.305908 0.018685 3867.879639 0.066747 3882.167725 0.005536 3910.677246 0.000969 3926.428467 0.002014 3946.515381 0.010572 4037.989014 0.01361 4065.206299 0.035311 4139.71582 0.004736 4223.914551 0.017057 4265.208984 0.006831 4339.583008 0.002159 4356.757812 0.006187 4366.714844 0.002608 4441.395996 0.003888 4479.712402 0.001311 4528.183105 0.021541 4550.86 0.003991 4605.637695 0.017566 4668.008301 0.024587 4702.873535 0.000914 4725.552246 0.001391 4745.598633 0.001155 4809.610352 0.008103 4825.925781 0.007497 4851.916016 0.006891 4871.803711 0.006932 4938.197266 0.000768 4964.739258 0.002903 5040.028809 0.003993 5049.277832 0.001507 5126.824707 0.000909 5192.424316 0.002485 5235.394531 0.001259 5268.115234 0.003396 5300.106934 0.016295 5321.609375 0.002972 5341.354004 0.001296 5374.025391 0.012623 5404.853027 0.004186 5428.912109 0.007369 5447.108398 0.002878",
													"linecount" : 31,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"id" : "obj-3",
													"fontname" : "Arial",
													"patching_rect" : [ 18.0, 143.0, 403.0, 325.0 ],
													"numinlets" : 2,
													"fontsize" : 9.0
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-2", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"default_fontsize" : 12.0,
										"fontname" : "Arial",
										"globalpatchername" : "",
										"fontface" : 0,
										"fontsize" : 12.0,
										"default_fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadbang",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"id" : "obj-8",
									"fontname" : "Arial",
									"patching_rect" : [ 273.0, 24.0, 48.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "output argument:",
									"numoutlets" : 0,
									"id" : "obj-9",
									"fontname" : "Arial",
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"patching_rect" : [ 135.0, 84.0, 157.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"id" : "obj-10",
									"patching_rect" : [ 48.0, 87.0, 15.0, 15.0 ],
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "1 = output each f / a pair separately.",
									"numoutlets" : 0,
									"id" : "obj-11",
									"fontname" : "Arial",
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"patching_rect" : [ 135.0, 112.0, 178.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "0 = concatenate output (default).",
									"numoutlets" : 0,
									"id" : "obj-12",
									"fontname" : "Arial",
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"patching_rect" : [ 135.0, 98.0, 157.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print",
									"numoutlets" : 0,
									"id" : "obj-13",
									"fontname" : "Arial",
									"patching_rect" : [ 37.0, 156.0, 32.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "migrator 0. 5 0.03",
									"numoutlets" : 4,
									"outlettype" : [ "list", "float", "int", "bang" ],
									"id" : "obj-14",
									"fontname" : "Arial",
									"patching_rect" : [ 37.0, 132.0, 101.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0,
									"color" : [ 0.4, 0.4, 0.8, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numoutlets" : 2,
									"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
									"outlettype" : [ "int", "bang" ],
									"id" : "obj-15",
									"fontname" : "Arial",
									"triscale" : 0.9,
									"patching_rect" : [ 66.0, 87.0, 44.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0,
									"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "output $1",
									"numoutlets" : 1,
									"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
									"outlettype" : [ "" ],
									"id" : "obj-16",
									"fontname" : "Arial",
									"patching_rect" : [ 66.0, 111.0, 53.0, 15.0 ],
									"numinlets" : 2,
									"fontsize" : 9.0
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [ 282.5, 66.0, 46.5, 66.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-16", 0 ],
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
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [ 282.5, 66.0, 393.5, 66.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 1 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Amplitude of all oscillators",
					"numoutlets" : 0,
					"id" : "obj-3",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 37.0, 365.0, 132.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"patching_rect" : [ 171.0, 360.0, 41.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "oscamp $1",
					"numoutlets" : 1,
					"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"patching_rect" : [ 171.0, 380.0, 58.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"patching_rect" : [ 171.0, 313.0, 41.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The right outlet sends out a bang when the list wraps around to the beginning.",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-7",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 382.0, 432.0, 178.0, 27.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The second outlet outputs the last frequency that was changed, and the third outlet keeps track of which partial is being updated.",
					"linecount" : 4,
					"numoutlets" : 0,
					"id" : "obj-8",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 461.0, 383.0, 154.0, 48.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-9",
					"patching_rect" : [ 445.0, 407.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"patching_rect" : [ 376.0, 407.0, 62.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"hidden" : 1,
					"id" : "obj-11",
					"fontname" : "Arial",
					"patching_rect" : [ 404.0, 89.0, 60.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Select a timbre here (Tibetan singing bowls)",
					"numoutlets" : 0,
					"id" : "obj-12",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 387.0, 131.0, 207.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"items" : [ "bowl", 1, ",", "bowl", 2, ",", "bowl", 3 ],
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"types" : [  ],
					"patching_rect" : [ 284.0, 129.0, 100.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0,
					"labelclick" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The number of updates it takes for each oscillator to fade in and out",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-14",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 51.0, 252.0, 119.0, 38.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-15",
					"fontname" : "Arial",
					"triscale" : 0.9,
					"patching_rect" : [ 171.0, 252.0, 44.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "fade $1",
					"numoutlets" : 1,
					"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"patching_rect" : [ 171.0, 273.0, 44.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Update the list manually",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-17",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 92.0, 179.0, 75.0, 27.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Do it smoothly",
					"numoutlets" : 0,
					"id" : "obj-18",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 378.0, 276.0, 72.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"hidden" : 1,
					"id" : "obj-19",
					"fontname" : "Arial",
					"patching_rect" : [ 334.0, 245.0, 16.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"hidden" : 1,
					"id" : "obj-20",
					"fontname" : "Arial",
					"patching_rect" : [ 334.0, 186.0, 16.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc_smooth 100",
					"numoutlets" : 1,
					"bgcolor" : [ 0.6, 0.6, 1.0, 1.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-21",
					"fontname" : "Arial",
					"patching_rect" : [ 284.0, 288.0, 90.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc_smooth 40",
					"numoutlets" : 1,
					"bgcolor" : [ 0.6, 0.6, 1.0, 1.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"patching_rect" : [ 284.0, 266.0, 84.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-23",
					"patching_rect" : [ 171.0, 206.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"id" : "obj-24",
					"patching_rect" : [ 171.0, 158.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "metro 55",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"patching_rect" : [ 171.0, 179.0, 52.0, 17.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r to_mig",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"hidden" : 1,
					"id" : "obj-26",
					"fontname" : "Arial",
					"patching_rect" : [ 187.0, 437.0, 49.0, 17.0 ],
					"numinlets" : 0,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p data",
					"numoutlets" : 0,
					"id" : "obj-27",
					"fontname" : "Arial",
					"patching_rect" : [ 284.0, 157.0, 37.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 39.0, 59.0, 1278.0, 764.0 ],
						"bglocked" : 0,
						"defrect" : [ 39.0, 59.0, 1278.0, 764.0 ],
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
						"metadata" : [  ],
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "sel 0 1 2",
									"numoutlets" : 4,
									"outlettype" : [ "bang", "bang", "bang", "" ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"patching_rect" : [ 18.0, 86.0, 53.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s to_mig",
									"numoutlets" : 0,
									"id" : "obj-2",
									"fontname" : "Arial",
									"patching_rect" : [ 18.0, 622.0, 49.0, 17.0 ],
									"numinlets" : 1,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"id" : "obj-3",
									"patching_rect" : [ 18.0, 64.0, 15.0, 15.0 ],
									"numinlets" : 0,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "175.19812 0.002034 451.858582 0.004866 513.681458 0.007675 517.54071 0.008458 519.529724 0.02438 864.523438 0.019631 973.432251 0.00872 981.132568 0.019179 981.411194 0.013594 1025.233398 0.004902 1081.151611 0.01215 1147.003418 0.018724 1185.029297 0.013841 1289.892456 0.020594 1378.960205 0.015598 1419.053833 0.029087 1458.422974 0.036118 1450.237183 0.003906 1500.18 0.016417 1538.967407 0.019964 1541.116821 0.025539 1671.823364 0.019539 1721.088013 0.008885 1774.229614 0.014743 1804.283203 0.009863 1841.236694 0.012723 1847.045166 0.010598 1893.956421 0.020795 1925.770874 0.03917 1963.034424 0.005978 1982.406494 0.029161 2057.286621 0.008428 2056.10376 0.002274 2188.61792 0.016549 2202.217285 0.024463 2199.20752 0.034725 2353.140625 0.007805 2431.002197 0.031862 2521.50415 0.008179 2523.201904 0.005362 2584.220215 0.021423 2644.24585 0.009208 2701.623779 0.003833 2717.206543 0.009904 2767.223145 0.006544 2905.861816 0.027882 2905.5 0.018217 2928.183105 0.059459 2925.677002 0.054909 3105.308838 0.013436 3161.457275 0.004742 3180.865723 0.005297 3180.770752 0.001737 3256.503906 0.019303 3394.640381 0.014353 3664.297852 0.075535 3717.210205 0.005791 3718.190186 0.00364 3731.671387 0.003093 3835.705078 0.009012 3874.22876 0.011176 3910.981689 0.003033 3954.828369 0.004757 3980.535645 0.020482 4093.140625 0.010689 4182.307617 0.003596 4212.882812 0.020079 4271.879395 0.044136 4374.732422 0.083343 4377.322266 0.002599 4398.521484 0.005501 4398.63916 0.002307 4450.605957 0.004987 4448.783691 0.002649 4484.793457 0.025919 4484.5 0.040162 4511.027832 0.083134 4511.42 0.099757 4601.559082 0.013841 4641.048828 0.003566 4742.364746 0.05283 4866.88916 0.014175 4871.108398 0.004071 4971.463379 0.045455 5025.327148 0.00359 5094.449219 0.00241 5094.627441 0.002506 5109.935059 0.005513 5114.229492 0.004693 5125.742188 0.003487 5165.979492 0.074761 5208.134277 0.006764 5207.321289 0.004664 5240.40918 0.026356 5260. 0.03818 5293.556641 0.036401 5339.111816 0.005738 5336.366699 0.003588 5409.041504 0.039268 5451.865234 0.152508 5447.689453 0.04088 5457.897949 0.023855 5494.966797 0.039587 5492.834473 0.009861 5556.8 0.010541 5595.554688 0.088238 5673.0625 0.057378 5671.020508 0.015261 5718.369141 0.003545 5831.181152 0.00461 5852.070801 0.144514 5850.666016 0.230989 5863.479004 0.004163 5990.077637 0.002586 6023.878418 0.010491 6026.344727 0.008217 6052.257812 0.007784 6184.495117 0.00615 6269.085449 0.055589 6324.729492 0.010279 6324.914062 0.003355 6366.211426 0.007105 6365.353516 0.005305 6462.601074 0.031332 6521.336914 0.033808",
									"linecount" : 37,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"patching_rect" : [ 830.0, 143.0, 344.0, 387.0 ],
									"numinlets" : 2,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "60.591526 0.002086 124.25 0.003168 307.1 0.002512 367.213715 0.003765 367.746796 0.001989 519.74939 0.005164 567.288696 0.002289 590.156128 0.007443 619.422546 0.011643 676.432739 0.007335 701.044312 0.024231 735.641479 0.001773 764.286499 0.007855 806.155762 0.014223 826.989014 0.002446 850.097473 0.009784 889.841431 0.003857 942.696106 0.007444 988.403076 0.005794 1044.174316 0.023583 1057.054443 0.002295 1079.5 0.002581 1093.581299 0.01268 1198.42688 0.006701 1213.115479 0.00916 1260.76355 0.008301 1274.40625 0.010897 1313.204224 0.015449 1352.276245 0.008848 1400.6604 0.004347 1461.260986 0.010449 1531.470337 0.001243 1544.140625 0.018057 1544.719116 0.014556 1605.071411 0.020525 1665.956177 0.004283 1708.20874 0.003154 1764.036499 0.013022 1909.519653 0.01005 1957.431519 0.010751 1998.817627 0.029841 2038.623413 0.004996 2049.492432 0.014974 2109.278076 0.022875 2187.15332 0.012909 2219.504395 0.002159 2245.522705 0.009398 2299.059082 0.016092 2322.217529 0.008339 2378.848145 0.018475 2400.836182 0.03366 2465.20166 0.024657 2495.965576 0.015261 2548.272949 0.018617 2588.419189 0.002373 2603.936279 0.03551 2637.756592 0.002988 2649.952637 0.024326 2704.208008 0.018459 2724.538086 0.008086 2749.833252 0.00621 2819.68335 0.020107 2915.210205 0.011573 2974.558838 0.009748 3088.5 0.010212 3142.38623 0.004082 3180.751953 0.008717 3204.660645 0.022658 3280.303711 0.005311 3304.842041 0.005914 3351.968994 0.006613 3390.501953 0.016885 3461.302734 0.010495 3551.084717 0.017463 3590.404541 0.003957 3682.258301 0.002133 3754.85498 0.01522 3787.292236 0.005505 3819.113281 0.013595 3835.401123 0.011465 3880.970459 0.002261 3905.647217 0.002781 4024.360352 0.016694 4098.225098 0.003273 4149.354492 0.003734 4201.872559 0.00644 4217.258789 0.009491 4271.950195 0.003152 4297.945312 0.005783 4336.766602 0.034716 4347.856445 0.029154 4367.552246 0.008778 4391.611816 0.053896 4491.630371 0.034129 4510.81 0.022093 4535.401855 0.008319 4580.155762 0.00808 4652.401367 0.024806 4686.225586 0.041847 4724.976074 0.011263 4748.547852 0.045624 4775.426758 0.004685 4817.439453 0.038121 4843.54248 0.013725 4885.476074 0.044347 4928.557617 0.001152 4968.395996 0.006177 5018.126465 0.009172 5058.202637 0.012613 5083.881836 0.001449 5103.901855 0.036719 5119.003418 0.002098 5129.096191 0.003227 5140.726562 0.012434 5168.713379 0.046841 5181.744629 0.002939 5207.71 0.077489 5229.788574 0.025113 5252.874023 0.012391 5331.532227 0.003302 5353.578125 0.013896 5363.876465 0.003411 5379.693848 0.017308 5425.149414 0.033027",
									"linecount" : 38,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"id" : "obj-5",
									"fontname" : "Arial",
									"patching_rect" : [ 424.0, 143.0, 340.0, 398.0 ],
									"numinlets" : 2,
									"fontsize" : 9.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "44.428703 0.001205 105.987946 0.005561 144.132431 0.002473 183.831909 0.00142 301.025726 0.004145 301.322327 0.004067 343.628357 0.001565 406.502655 0.005348 442.231934 0.003312 522.951904 0.005215 557.567505 0.015346 602.613892 0.002343 649.766907 0.004004 664.45575 0.002506 693.972351 0.015189 733.67865 0.002115 768.932007 0.009084 785.913269 0.012306 839.502014 0.006151 875.094177 0.005993 877.02771 0.009862 879.910217 0.005887 945.953247 0.007129 1012.840332 0.012604 1044.641846 0.007745 1115.045166 0.007925 1136.086426 0.00227 1175.845703 0.005228 1240.114258 0.003953 1258.286621 0.001868 1318.401489 0.020711 1345.764038 0.003522 1358.986084 0.006174 1385.278442 0.011233 1398.879517 0.014475 1431.438965 0.016383 1540.99292 0.014028 1635.66748 0.010117 1679.655151 0.006543 1750.221924 0.001598 1797.57 0.001072 1855.189087 0.007052 1916.9 0.002683 1943.572876 0.010738 1971.103516 0.00379 1982.150879 0.010626 2015.547485 0.018401 2034.003662 0.001115 2058.932861 0.012229 2114.809082 0.002263 2139.052246 0.035608 2227.550781 0.003884 2270.133057 0.014658 2319.095459 0.006923 2346.848877 0.005677 2374.3 0.003562 2410.141113 0.005541 2480.320801 0.005741 2545.730469 0.000959 2670.65 0.028469 2687.073242 0.015307 2722.737305 0.010995 2775.491455 0.00659 2854.015869 0.003388 2909.356934 0.002405 2935.9 0.002298 2942.420166 0.00388 2972.062744 0.008935 3040.323242 0.00169 3102.340576 0.010847 3243.788086 0.006875 3264.5979 0.012196 3360.085693 0.001174 3404.677246 0.00133 3561.624512 0.00218 3630.240967 0.005584 3784.453857 0.003829 3798.970459 0.017009 3808.286133 0.015151 3818.577637 0.007675 3833.684082 0.007295 3844.305908 0.018685 3867.879639 0.066747 3882.167725 0.005536 3910.677246 0.000969 3926.428467 0.002014 3946.515381 0.010572 4037.989014 0.01361 4065.206299 0.035311 4139.71582 0.004736 4223.914551 0.017057 4265.208984 0.006831 4339.583008 0.002159 4356.757812 0.006187 4366.714844 0.002608 4441.395996 0.003888 4479.712402 0.001311 4528.183105 0.021541 4550.86 0.003991 4605.637695 0.017566 4668.008301 0.024587 4702.873535 0.000914 4725.552246 0.001391 4745.598633 0.001155 4809.610352 0.008103 4825.925781 0.007497 4851.916016 0.006891 4871.803711 0.006932 4938.197266 0.000768 4964.739258 0.002903 5040.028809 0.003993 5049.277832 0.001507 5126.824707 0.000909 5192.424316 0.002485 5235.394531 0.001259 5268.115234 0.003396 5300.106934 0.016295 5321.609375 0.002972 5341.354004 0.001296 5374.025391 0.012623 5404.853027 0.004186 5428.912109 0.007369 5447.108398 0.002878",
									"linecount" : 38,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"id" : "obj-6",
									"fontname" : "Arial",
									"patching_rect" : [ 18.0, 143.0, 344.0, 398.0 ],
									"numinlets" : 2,
									"fontsize" : 9.0
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-2", 0 ],
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
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 1 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 2 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Use forcefeed to change harmony immediately",
					"numoutlets" : 0,
					"id" : "obj-28",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 368.0, 361.0, 258.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "220. 1. 233.08 1. 246.94 1. 261.63 1. 277.18 1. 293.67 1. 311.13 1. 329.63 1. 348.23 1. 370. 1. 392. 1. 415.31 1. 440. 1.",
					"linecount" : 2,
					"numoutlets" : 1,
					"bgcolor" : [ 0.611765, 0.701961, 1.0, 1.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"patching_rect" : [ 272.0, 328.0, 308.0, 25.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Update the list automatically",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-30",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 92.0, 120.0, 75.0, 27.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Standard deviation of Gaussian distribution in semitones",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-31",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 23.0, 317.0, 146.0, 27.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Change the number of frequency/amplitude pairs that are output",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-32",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 352.0, 211.0, 198.0, 27.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend forcefeed",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-33",
					"fontname" : "Arial",
					"patching_rect" : [ 272.0, 359.0, 91.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "var $1",
					"numoutlets" : 1,
					"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-34",
					"fontname" : "Arial",
					"patching_rect" : [ 171.0, 333.0, 41.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc 40",
					"numoutlets" : 1,
					"bgcolor" : [ 0.6, 0.6, 1.0, 1.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-35",
					"fontname" : "Arial",
					"patching_rect" : [ 284.0, 216.0, 46.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc 100",
					"numoutlets" : 1,
					"bgcolor" : [ 0.6, 0.6, 1.0, 1.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-36",
					"fontname" : "Arial",
					"patching_rect" : [ 284.0, 194.0, 52.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"id" : "obj-37",
					"patching_rect" : [ 171.0, 122.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "spectroscope~",
					"numoutlets" : 0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"peakcolor" : [ 0.952941, 0.8, 0.8, 1.0 ],
					"sonomedlocolor" : [ 0.498039, 0.0, 0.498039, 1.0 ],
					"id" : "obj-38",
					"fgcolor" : [ 0.878431, 0.878431, 0.878431, 1.0 ],
					"patching_rect" : [ 277.0, 466.0, 300.0, 100.0 ],
					"sonomedhicolor" : [ 1.0, 0.74902, 0.0, 1.0 ],
					"rounded" : 0,
					"markercolor" : [ 0.721569, 0.721569, 0.721569, 1.0 ],
					"bordercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"sonomedcolor" : [ 0.0, 0.74902, 0.498039, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc 10",
					"numoutlets" : 1,
					"bgcolor" : [ 0.6, 0.6, 1.0, 1.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-39",
					"fontname" : "Arial",
					"patching_rect" : [ 284.0, 238.0, 46.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sinusoids~",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-40",
					"fontname" : "Arial",
					"patching_rect" : [ 238.0, 437.0, 57.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "migrator @stdev 0.15 @nOsc 40 @fade 20",
					"numoutlets" : 4,
					"outlettype" : [ "list", "float", "int", "bang" ],
					"id" : "obj-41",
					"fontname" : "Arial",
					"patching_rect" : [ 236.0, 384.0, 183.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0,
					"color" : [ 0.4, 0.4, 0.8, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "stop",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-42",
					"fontname" : "Arial",
					"patching_rect" : [ 355.0, 614.0, 27.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "startwindow",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-43",
					"fontname" : "Arial",
					"patching_rect" : [ 286.0, 614.0, 65.0, 15.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• start audio",
					"numoutlets" : 0,
					"id" : "obj-44",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 298.0, 599.0, 69.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"id" : "obj-45",
					"border" : 2,
					"patching_rect" : [ 278.0, 593.0, 123.0, 43.0 ],
					"rounded" : 0,
					"bordercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numoutlets" : 2,
					"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"outlettype" : [ "signal", "int" ],
					"stripecolor" : [ 0.66667, 0.66667, 0.66667, 1.0 ],
					"id" : "obj-46",
					"patching_rect" : [ 239.0, 466.0, 22.0, 122.0 ],
					"numinlets" : 2,
					"orientation" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dac~",
					"numoutlets" : 0,
					"id" : "obj-47",
					"fontname" : "Arial",
					"patching_rect" : [ 238.0, 652.0, 31.0, 17.0 ],
					"numinlets" : 2,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Based on David Wessel's \"migrate\" abstraction",
					"numoutlets" : 0,
					"id" : "obj-48",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 66.0, 77.0, 267.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Spectral/harmonic transformation",
					"numoutlets" : 0,
					"id" : "obj-49",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"patching_rect" : [ 66.0, 61.0, 192.0, 17.0 ],
					"numinlets" : 1,
					"fontsize" : 9.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "migrator",
					"numoutlets" : 0,
					"id" : "obj-50",
					"fontname" : "Arial",
					"frgb" : [ 0.4, 0.4, 0.8, 1.0 ],
					"patching_rect" : [ 66.0, 33.0, 185.0, 27.0 ],
					"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"numinlets" : 1,
					"fontsize" : 18.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"bgcolor" : [ 0.87451, 0.933333, 1.0, 1.0 ],
					"id" : "obj-51",
					"border" : 5,
					"patching_rect" : [ 38.0, 24.0, 323.0, 77.0 ],
					"rounded" : 70,
					"bordercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-46", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [ 364.5, 642.0, 247.5, 642.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [ 295.5, 642.0, 247.5, 642.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-47", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [ 351.0, 223.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 180.5, 405.0, 233.0, 405.0, 233.0, 377.0, 245.5, 377.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 180.5, 293.0, 245.5, 293.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 180.0, 143.0, 245.5, 143.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 180.0, 228.0, 245.5, 228.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 293.5, 307.0, 245.5, 307.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 293.5, 285.0, 245.5, 285.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 293.5, 212.0, 245.5, 212.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 293.5, 234.0, 245.5, 234.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 293.5, 257.0, 245.5, 257.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 180.5, 352.0, 245.5, 352.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [ 180.5, 199.0, 245.5, 199.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 1 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 2 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 3 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-41", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
