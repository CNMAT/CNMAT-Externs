{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 70.0, 44.0, 861.0, 773.0 ],
		"bglocked" : 0,
		"defrect" : [ 70.0, 44.0, 861.0, 773.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 4.0, 4.0 ],
		"gridsnaponopen" : 2,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Displays resonance models ",
					"fontsize" : 14.0,
					"presentation_rect" : [ 719.0, 43.0, 0.0, 0.0 ],
					"textcolor" : [ 1.0, 0.974289, 0.97435, 1.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 452.0, 48.0, 235.0, 23.0 ],
					"numoutlets" : 0,
					"id" : "obj-55",
					"frgb" : [ 1.0, 0.974289, 0.97435, 1.0 ],
					"fontname" : "Orator Std Medium"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "resdisplay",
					"fontsize" : 36.0,
					"presentation_rect" : [ 379.0, 15.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 448.0, 12.0, 228.0, 50.0 ],
					"numoutlets" : 0,
					"id" : "obj-38",
					"fontname" : "Orator Std Slanted"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Adjust the y-axis min and max for the linear scale view mode",
					"linecount" : 4,
					"fontsize" : 11.0,
					"presentation_rect" : [ 789.0, 152.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 720.0, 276.0, 110.0, 57.0 ],
					"numoutlets" : 0,
					"id" : "obj-36",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Adjust the y-axis min and max for the logarithmic view mode",
					"linecount" : 4,
					"fontsize" : 11.0,
					"presentation_rect" : [ 559.0, 151.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 704.0, 124.0, 113.0, 57.0 ],
					"numoutlets" : 0,
					"id" : "obj-32",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Put the y-axis  on a logarithmic scale",
					"linecount" : 2,
					"fontsize" : 11.0,
					"presentation_rect" : [ 455.0, 100.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 456.0, 124.0, 100.0, 32.0 ],
					"numoutlets" : 0,
					"id" : "obj-30",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Change the min and max frequencies to display",
					"linecount" : 4,
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 288.0, 144.0, 89.0, 57.0 ],
					"numoutlets" : 0,
					"id" : "obj-72",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Switch between two view modes ",
					"linecount" : 2,
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 208.0, 44.0, 89.0, 32.0 ],
					"numoutlets" : 0,
					"id" : "obj-70",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numinlets" : 1,
					"patching_rect" : [ 572.0, 868.0, 213.0, 28.0 ],
					"numoutlets" : 4,
					"id" : "obj-39",
					"outlettype" : [ "bang", "bang", "", "int" ],
					"handoff" : ""
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "http://www.cnmat.berkeley.edu/MAX/",
					"fontsize" : 9.0,
					"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 624.0, 880.0, 202.0, 17.0 ],
					"numoutlets" : 0,
					"id" : "obj-40",
					"frgb" : [ 0.4, 0.4, 0.8, 1.0 ],
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\rmax launch_browser http://www.cnmat.berkeley.edu/MAX/",
					"linecount" : 2,
					"fontsize" : 9.0,
					"numinlets" : 2,
					"patching_rect" : [ 572.0, 904.0, 300.0, 25.0 ],
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-45",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "fpic",
					"numinlets" : 1,
					"patching_rect" : [ 574.0, 815.0, 109.0, 48.0 ],
					"numoutlets" : 0,
					"embed" : 1,
					"id" : "obj-47",
					"data" : [ 593, "", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "resdiplay and all other CNMAT Max objects, can be found at:",
					"linecount" : 2,
					"fontsize" : 9.0,
					"numinlets" : 1,
					"patching_rect" : [ 572.0, 869.0, 187.0, 27.0 ],
					"numoutlets" : 0,
					"id" : "obj-48",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "resdisplay.maxhelp version 0.1 by Yotam Mann & John MacCallum, adapted from A. Freed & M. Zbyszynski resonance-diplay.help",
					"linecount" : 4,
					"fontsize" : 9.0,
					"numinlets" : 1,
					"patching_rect" : [ 684.0, 816.0, 178.0, 48.0 ],
					"numoutlets" : 0,
					"id" : "obj-50",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 0.7",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 660.0, 236.0, 90.0, 19.0 ],
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-15",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess -0.01",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 588.0, 216.0, 100.0, 19.0 ],
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-18",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 660.0, 260.0, 71.0, 19.0 ],
					"numoutlets" : 2,
					"id" : "obj-19",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 588.0, 260.0, 66.0, 19.0 ],
					"numoutlets" : 2,
					"id" : "obj-20",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak -0.01 0.07",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 588.0, 288.0, 92.0, 19.0 ],
					"numoutlets" : 1,
					"id" : "obj-24",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ampmin $1, ampmax $2",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 588.0, 312.0, 129.0, 17.0 ],
					"numoutlets" : 1,
					"id" : "obj-29",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 0",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 652.0, 108.0, 81.0, 19.0 ],
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-64",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess -100",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 588.0, 108.0, 97.0, 19.0 ],
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-65",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 652.0, 132.0, 50.0, 19.0 ],
					"numoutlets" : 2,
					"id" : "obj-66",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 588.0, 132.0, 48.0, 19.0 ],
					"numoutlets" : 2,
					"id" : "obj-67",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak -100. 0.",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 588.0, 160.0, 84.0, 19.0 ],
					"numoutlets" : 1,
					"id" : "obj-68",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 22050",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 216.0, 112.0, 105.0, 19.0 ],
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-62",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 0",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 152.0, 112.0, 81.0, 19.0 ],
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-61",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 216.0, 132.0, 70.0, 19.0 ],
					"numoutlets" : 2,
					"id" : "obj-59",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 152.0, 132.0, 42.0, 19.0 ],
					"numoutlets" : 2,
					"id" : "obj-56",
					"fontname" : "Arial",
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pak 22050. 0.",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 152.0, 160.0, 84.0, 19.0 ],
					"numoutlets" : 1,
					"id" : "obj-54",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 1",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 408.0, 96.0, 80.0, 19.0 ],
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-49",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numinlets" : 2,
					"patching_rect" : [ 40.0, 812.0, 19.0, 67.0 ],
					"numoutlets" : 2,
					"id" : "obj-35",
					"outlettype" : [ "signal", "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"numinlets" : 2,
					"patching_rect" : [ 344.0, 816.0, 19.0, 67.0 ],
					"numoutlets" : 2,
					"id" : "obj-34",
					"outlettype" : [ "signal", "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 28.0, 124.0, 72.0, 19.0 ],
					"numoutlets" : 1,
					"id" : "obj-33",
					"fontname" : "Arial",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Unselected resonances ",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 384.0, 620.0, 145.0, 19.0 ],
					"numoutlets" : 0,
					"id" : "obj-31",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Shift P plays unselected resonance model",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 476.0, 712.0, 221.0, 19.0 ],
					"numoutlets" : 0,
					"id" : "obj-69",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
					"setstyle" : 1,
					"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
					"bordercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
					"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
					"setminmax" : [ 0.0, 2.0 ],
					"slidercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"numinlets" : 1,
					"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
					"patching_rect" : [ 157.0, 741.0, 97.0, 18.0 ],
					"numoutlets" : 2,
					"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
					"id" : "obj-21",
					"outlettype" : [ "", "" ],
					"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
					"orientation" : 0,
					"contdata" : 1,
					"bgcolor" : [ 0.913725, 0.933333, 1.0, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p clear-model-impulse",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 343.0, 767.0, 137.0, 19.0 ],
					"numoutlets" : 1,
					"id" : "obj-22",
					"fontname" : "Arial",
					"outlettype" : [ "float" ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 10.0, 59.0, 600.0, 426.0 ],
						"bglocked" : 0,
						"defrect" : [ 10.0, 59.0, 600.0, 426.0 ],
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
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 166.0, 91.0, 15.0, 15.0 ],
									"numoutlets" : 1,
									"id" : "obj-1",
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"patching_rect" : [ 75.0, 187.0, 15.0, 15.0 ],
									"numoutlets" : 0,
									"id" : "obj-2",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "f",
									"fontsize" : 9.0,
									"numinlets" : 2,
									"patching_rect" : [ 60.0, 129.0, 27.0, 17.0 ],
									"numoutlets" : 1,
									"id" : "obj-3",
									"fontname" : "Arial",
									"outlettype" : [ "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 60.0, 27.0, 15.0, 15.0 ],
									"numoutlets" : 1,
									"id" : "obj-4",
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "clear",
									"fontsize" : 9.0,
									"numinlets" : 2,
									"patching_rect" : [ 132.0, 142.0, 32.0, 15.0 ],
									"numoutlets" : 1,
									"id" : "obj-5",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b l b",
									"fontsize" : 9.0,
									"numinlets" : 1,
									"patching_rect" : [ 60.0, 60.0, 83.0, 17.0 ],
									"numoutlets" : 3,
									"id" : "obj-6",
									"fontname" : "Arial",
									"outlettype" : [ "bang", "", "bang" ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-3", 0 ],
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
									"source" : [ "obj-6", 1 ],
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
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-3", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 2 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontface" : 0,
						"default_fontsize" : 12.0,
						"fontsize" : 12.0,
						"fontname" : "Arial",
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p clear-model-impulse",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 40.0, 764.0, 137.0, 19.0 ],
					"numoutlets" : 1,
					"id" : "obj-23",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 10.0, 59.0, 600.0, 426.0 ],
						"bglocked" : 0,
						"defrect" : [ 10.0, 59.0, 600.0, 426.0 ],
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
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 166.0, 91.0, 15.0, 15.0 ],
									"numoutlets" : 1,
									"id" : "obj-1",
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"patching_rect" : [ 75.0, 187.0, 15.0, 15.0 ],
									"numoutlets" : 0,
									"id" : "obj-2",
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "f",
									"fontsize" : 9.0,
									"numinlets" : 2,
									"patching_rect" : [ 60.0, 129.0, 27.0, 17.0 ],
									"numoutlets" : 1,
									"id" : "obj-3",
									"fontname" : "Arial",
									"outlettype" : [ "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 60.0, 27.0, 15.0, 15.0 ],
									"numoutlets" : 1,
									"id" : "obj-4",
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "clear",
									"fontsize" : 9.0,
									"numinlets" : 2,
									"patching_rect" : [ 132.0, 142.0, 32.0, 15.0 ],
									"numoutlets" : 1,
									"id" : "obj-5",
									"fontname" : "Arial",
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b l b",
									"fontsize" : 9.0,
									"numinlets" : 1,
									"patching_rect" : [ 60.0, 60.0, 83.0, 17.0 ],
									"numoutlets" : 3,
									"id" : "obj-6",
									"fontname" : "Arial",
									"outlettype" : [ "bang", "", "bang" ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-3", 0 ],
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
									"source" : [ "obj-6", 1 ],
									"destination" : [ "obj-2", 0 ],
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
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-3", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 2 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontface" : 0,
						"default_fontsize" : 12.0,
						"fontsize" : 12.0,
						"fontname" : "Arial",
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "stop",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 176.0, 840.0, 45.0, 17.0 ],
					"numoutlets" : 1,
					"id" : "obj-25",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "startwindow",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 107.0, 841.0, 67.0, 17.0 ],
					"numoutlets" : 1,
					"id" : "obj-26",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• start audio",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 118.0, 823.0, 108.0, 19.0 ],
					"numoutlets" : 0,
					"id" : "obj-27",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"bordercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"rounded" : 0,
					"numinlets" : 1,
					"patching_rect" : [ 94.0, 820.0, 123.0, 43.0 ],
					"numoutlets" : 0,
					"id" : "obj-28",
					"border" : 2,
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 382.0, 652.0, 54.0, 19.0 ],
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-41",
					"fontname" : "Arial",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
					"setstyle" : 1,
					"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
					"bordercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
					"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
					"setminmax" : [ 0.0, 2.0 ],
					"slidercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"numinlets" : 1,
					"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
					"patching_rect" : [ 460.0, 743.0, 97.0, 18.0 ],
					"numoutlets" : 2,
					"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
					"id" : "obj-42",
					"outlettype" : [ "", "" ],
					"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
					"orientation" : 0,
					"contdata" : 1,
					"bgcolor" : [ 0.913725, 0.933333, 1.0, 1.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"patching_rect" : [ 460.0, 720.0, 15.0, 15.0 ],
					"numoutlets" : 1,
					"id" : "obj-43",
					"fgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "resonators~",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 343.0, 791.0, 90.0, 19.0 ],
					"numoutlets" : 2,
					"id" : "obj-44",
					"fontname" : "Arial",
					"outlettype" : [ "signal", "list" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.5",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 382.0, 675.0, 36.0, 17.0 ],
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-52",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"patching_rect" : [ 157.0, 719.0, 15.0, 15.0 ],
					"numoutlets" : 1,
					"id" : "obj-57",
					"fgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dac~",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 40.0, 892.0, 322.0, 19.0 ],
					"numoutlets" : 0,
					"id" : "obj-58",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "space bar or 'p' on keyboard excites model",
					"linecount" : 2,
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 172.0, 709.0, 150.0, 32.0 ],
					"numoutlets" : 0,
					"id" : "obj-60",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Selected resonances ",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 72.0, 620.0, 134.0, 19.0 ],
					"numoutlets" : 0,
					"id" : "obj-63",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "resonators~",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 40.0, 787.0, 90.0, 19.0 ],
					"numoutlets" : 2,
					"id" : "obj-74",
					"fontname" : "Arial",
					"outlettype" : [ "signal", "list" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "key",
					"fontsize" : 11.0,
					"numinlets" : 0,
					"patching_rect" : [ 230.0, 656.0, 52.0, 19.0 ],
					"numoutlets" : 4,
					"id" : "obj-75",
					"fontname" : "Arial",
					"outlettype" : [ "int", "int", "int", "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "select 32 112 80",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 230.0, 680.0, 116.0, 19.0 ],
					"numoutlets" : 4,
					"id" : "obj-76",
					"fontname" : "Arial",
					"outlettype" : [ "bang", "bang", "bang", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Click and drag the mouse in the window to move the selection cursors",
					"linecount" : 4,
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 552.0, 364.0, 121.0, 57.0 ],
					"numoutlets" : 0,
					"id" : "obj-71",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Resonance Models are lists of (frequency, gain, decayrate) triples",
					"linecount" : 2,
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 624.0, 544.0, 248.0, 32.0 ],
					"numoutlets" : 0,
					"id" : "obj-73",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "freqmin $1, freqmax $2",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 152.0, 184.0, 125.0, 17.0 ],
					"numoutlets" : 1,
					"id" : "obj-17",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ampmin_log $1, ampmax_log $2",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 588.0, 184.0, 171.0, 17.0 ],
					"numoutlets" : 1,
					"id" : "obj-16",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"patching_rect" : [ 408.0, 128.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"id" : "obj-12",
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "log $1",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 408.0, 160.0, 46.0, 17.0 ],
					"numoutlets" : 1,
					"id" : "obj-14",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numinlets" : 1,
					"patching_rect" : [ 152.0, 48.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"id" : "obj-13",
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route selected unselected",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 40.0, 504.0, 151.0, 19.0 ],
					"numoutlets" : 3,
					"id" : "obj-11",
					"fontname" : "Arial",
					"outlettype" : [ "", "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "resdisplay",
					"fontsize" : 11.0,
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"textcolor" : [  ],
					"numinlets" : 1,
					"patching_rect" : [ 40.0, 532.0, 202.0, 89.0 ],
					"numoutlets" : 1,
					"id" : "obj-10",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "resdisplay",
					"fontsize" : 11.0,
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"textcolor" : [  ],
					"numinlets" : 1,
					"patching_rect" : [ 344.0, 532.0, 202.0, 89.0 ],
					"numoutlets" : 1,
					"id" : "obj-9",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jsui",
					"numinlets" : 1,
					"jsarguments" : [  ],
					"patching_rect" : [ 1344.0, 520.0, 171.0, 78.0 ],
					"hidden" : 1,
					"numoutlets" : 2,
					"id" : "obj-8",
					"outlettype" : [ "", "" ],
					"filename" : "resonance-display.js"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jsui",
					"numinlets" : 1,
					"jsarguments" : [  ],
					"patching_rect" : [ 1012.0, 520.0, 171.0, 78.0 ],
					"hidden" : 1,
					"numoutlets" : 2,
					"id" : "obj-7",
					"outlettype" : [ "", "" ],
					"filename" : "resonance-display.js"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "mode $1",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 152.0, 76.0, 57.0, 17.0 ],
					"numoutlets" : 1,
					"id" : "obj-5",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 28.0, 152.0, 50.0, 17.0 ],
					"numoutlets" : 1,
					"id" : "obj-6",
					"fontname" : "Arial",
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "zl group",
					"fontsize" : 11.0,
					"numinlets" : 2,
					"patching_rect" : [ 28.0, 204.0, 67.0, 19.0 ],
					"numoutlets" : 2,
					"id" : "obj-3",
					"fontname" : "Arial",
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll",
					"fontsize" : 11.0,
					"numinlets" : 1,
					"patching_rect" : [ 28.0, 176.0, 70.0, 19.0 ],
					"numoutlets" : 4,
					"id" : "obj-2",
					"fontname" : "Arial",
					"outlettype" : [ "", "", "", "" ],
					"coll_data" : 					{
						"count" : 78,
						"data" : [ 							{
								"key" : 0,
								"value" : [ 3.095031, 0.000501, 0.839865 ]
							}
, 							{
								"key" : 1,
								"value" : [ 3.330885, 0.000383, 0.02572 ]
							}
, 							{
								"key" : 2,
								"value" : [ 4.357238, 0.000366, 0.091906 ]
							}
, 							{
								"key" : 3,
								"value" : [ 13.315922, 0.000814, 0.291133 ]
							}
, 							{
								"key" : 4,
								"value" : [ 18.368757, 0.000347, 0.089799 ]
							}
, 							{
								"key" : 5,
								"value" : [ 20.73811, 0.000846, 0.151669 ]
							}
, 							{
								"key" : 6,
								"value" : [ 20.918432, 0.000405, 0.009408 ]
							}
, 							{
								"key" : 7,
								"value" : [ 26.803427, 0.000471, 0.329232 ]
							}
, 							{
								"key" : 8,
								"value" : [ 743.717712, 0.00115, 0.121514 ]
							}
, 							{
								"key" : 9,
								"value" : [ 743.77417, 0.000735, 0.09385 ]
							}
, 							{
								"key" : 10,
								"value" : [ 1335.228882, 0.001974, 0.487853 ]
							}
, 							{
								"key" : 11,
								"value" : [ 1342.429199, 0.000544, 0.168792 ]
							}
, 							{
								"key" : 12,
								"value" : [ 1343.737549, 0.005969, 0.120547 ]
							}
, 							{
								"key" : 13,
								"value" : [ 1343.695557, 0.005808, 0.120195 ]
							}
, 							{
								"key" : 14,
								"value" : [ 1344.899536, 0.000566, 0.177788 ]
							}
, 							{
								"key" : 15,
								"value" : [ 2136.942139, 0.000469, 0.478475 ]
							}
, 							{
								"key" : 16,
								"value" : [ 2137.040771, 0.00035, 0.36055 ]
							}
, 							{
								"key" : 17,
								"value" : [ 2417.411621, 0.000891, 0.143154 ]
							}
, 							{
								"key" : 18,
								"value" : [ 2417.401123, 0.000614, 0.111062 ]
							}
, 							{
								"key" : 19,
								"value" : [ 2418.432373, 0.00777, 3.497395 ]
							}
, 							{
								"key" : 20,
								"value" : [ 2418.815674, 0.009791, 0.599523 ]
							}
, 							{
								"key" : 21,
								"value" : [ 2418.635498, 0.003645, 0.037407 ]
							}
, 							{
								"key" : 22,
								"value" : [ 2419.850586, 0.000768, 0.118822 ]
							}
, 							{
								"key" : 23,
								"value" : [ 2419.895752, 0.000561, 0.100268 ]
							}
, 							{
								"key" : 24,
								"value" : [ 2979.344971, 0.000978, 0.159548 ]
							}
, 							{
								"key" : 25,
								"value" : [ 2980.461426, 0.00384, 0.068993 ]
							}
, 							{
								"key" : 26,
								"value" : [ 2980.629395, 0.004501, 0.089458 ]
							}
, 							{
								"key" : 27,
								"value" : [ 2980.994873, 0.005189, 6.459174 ]
							}
, 							{
								"key" : 28,
								"value" : [ 2981.914307, 0.000679, 0.123289 ]
							}
, 							{
								"key" : 29,
								"value" : [ 3347.789795, 0.000294, 0.056223 ]
							}
, 							{
								"key" : 30,
								"value" : [ 3347.856689, 0.000338, 0.177313 ]
							}
, 							{
								"key" : 31,
								"value" : [ 3348.395996, 0.000294, 2.651337 ]
							}
, 							{
								"key" : 32,
								"value" : [ 3709.87915, 0.003407, 0.272955 ]
							}
, 							{
								"key" : 33,
								"value" : [ 3710.599854, 0.01577, 1.983374 ]
							}
, 							{
								"key" : 34,
								"value" : [ 3711.255615, 0.020305, 8.268732 ]
							}
, 							{
								"key" : 35,
								"value" : [ 3710.978516, 0.009856, 0.153543 ]
							}
, 							{
								"key" : 36,
								"value" : [ 3712.211914, 0.003078, 0.274401 ]
							}
, 							{
								"key" : 37,
								"value" : [ 3741.925781, 0.00233, 2.698667 ]
							}
, 							{
								"key" : 38,
								"value" : [ 3742.080566, 0.002687, 0.243388 ]
							}
, 							{
								"key" : 39,
								"value" : [ 4832.230957, 0.002107, 0.322213 ]
							}
, 							{
								"key" : 40,
								"value" : [ 4833.591309, 0.003225, 0.160955 ]
							}
, 							{
								"key" : 41,
								"value" : [ 4833.519531, 0.002728, 0.175199 ]
							}
, 							{
								"key" : 42,
								"value" : [ 4834.799805, 0.001794, 0.299308 ]
							}
, 							{
								"key" : 43,
								"value" : [ 4880.425781, 0.007072, 0.315391 ]
							}
, 							{
								"key" : 44,
								"value" : [ 4881.89502, 0.012946, 0.17373 ]
							}
, 							{
								"key" : 45,
								"value" : [ 4881.697266, 0.013827, 0.226904 ]
							}
, 							{
								"key" : 46,
								"value" : [ 4882.984375, 0.006824, 0.299238 ]
							}
, 							{
								"key" : 47,
								"value" : [ 4887.930176, 0.002358, 1.334426 ]
							}
, 							{
								"key" : 48,
								"value" : [ 5971.915527, 0.017859, 8.098516 ]
							}
, 							{
								"key" : 49,
								"value" : [ 5972.330566, 0.011399, 0.341798 ]
							}
, 							{
								"key" : 50,
								"value" : [ 5973.62793, 0.004389, 0.359034 ]
							}
, 							{
								"key" : 51,
								"value" : [ 5977.212402, 0.001184, 0.800688 ]
							}
, 							{
								"key" : 52,
								"value" : [ 6082.070312, 0.011231, 1.870835 ]
							}
, 							{
								"key" : 53,
								"value" : [ 6083.140625, 0.00832, 0.267764 ]
							}
, 							{
								"key" : 54,
								"value" : [ 6082.95459, 0.001195, 0.140977 ]
							}
, 							{
								"key" : 55,
								"value" : [ 6089.63916, 0.007269, 1.647108 ]
							}
, 							{
								"key" : 56,
								"value" : [ 6254.398438, 0.002975, 0.308795 ]
							}
, 							{
								"key" : 57,
								"value" : [ 6937.492188, 0.004246, 1.307089 ]
							}
, 							{
								"key" : 58,
								"value" : [ 6943.527832, 0.01957, 0.573291 ]
							}
, 							{
								"key" : 59,
								"value" : [ 6943.588379, 0.008998, 0.423399 ]
							}
, 							{
								"key" : 60,
								"value" : [ 6943.96582, 0.016696, 4.0046 ]
							}
, 							{
								"key" : 61,
								"value" : [ 8125.540527, 0.00933, 0.780285 ]
							}
, 							{
								"key" : 62,
								"value" : [ 8325.910156, 0.028568, 17.443123 ]
							}
, 							{
								"key" : 63,
								"value" : [ 9194.033203, 0.013735, 0.968786 ]
							}
, 							{
								"key" : 64,
								"value" : [ 9196.920898, 0.030152, 1.099143 ]
							}
, 							{
								"key" : 65,
								"value" : [ 10417.199219, 0.000874, 4.096674 ]
							}
, 							{
								"key" : 66,
								"value" : [ 11041.389648, 0.00752, 3.475664 ]
							}
, 							{
								"key" : 67,
								"value" : [ 11139.416992, 0.025936, 1.345923 ]
							}
, 							{
								"key" : 68,
								"value" : [ 11139.322266, 0.026413, 4.308785 ]
							}
, 							{
								"key" : 69,
								"value" : [ 11261.237305, 0.001519, 1.705068 ]
							}
, 							{
								"key" : 70,
								"value" : [ 11942.694336, 0.001555, 1.182794 ]
							}
, 							{
								"key" : 71,
								"value" : [ 11948.686523, 0.02275, 4.20978 ]
							}
, 							{
								"key" : 72,
								"value" : [ 14452.239258, 0.068537, 19.764368 ]
							}
, 							{
								"key" : 73,
								"value" : [ 15806.329102, 0.001677, 9.92162 ]
							}
, 							{
								"key" : 74,
								"value" : [ 16633.75, 0.000376, 1.489159 ]
							}
, 							{
								"key" : 75,
								"value" : [ 18284.626953, 0.00262, 2.776374 ]
							}
, 							{
								"key" : 76,
								"value" : [ 20479.398438, 0.000953, 8.422816 ]
							}
, 							{
								"key" : 77,
								"value" : [ 21863.076172, 0.000663, 3.040938 ]
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
					"maxclass" : "jsui",
					"numinlets" : 1,
					"jsarguments" : [  ],
					"patching_rect" : [ 1012.0, 244.0, 503.0, 265.0 ],
					"hidden" : 1,
					"numoutlets" : 2,
					"id" : "obj-4",
					"outlettype" : [ "", "" ],
					"filename" : "resonance-display.js"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "resdisplay",
					"ampmin_log" : -103.0,
					"fontsize" : 11.0,
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"textcolor" : [  ],
					"numinlets" : 1,
					"ampmax" : 0.07,
					"patching_rect" : [ 40.0, 232.0, 505.0, 266.0 ],
					"numoutlets" : 1,
					"id" : "obj-1",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"ampmin" : -0.01
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"shadow" : 30,
					"bordercolor" : [ 1.0, 0.64, 0.0, 1.0 ],
					"rounded" : 0,
					"numinlets" : 1,
					"patching_rect" : [ 400.0, 12.0, 320.0, 60.0 ],
					"numoutlets" : 0,
					"id" : "obj-53",
					"bgcolor" : [ 0.454902, 0.568627, 0.733333, 1.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-11", 1 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [ 115.5, 528.0, 353.5, 528.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 1 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 115.5, 528.0, 303.0, 528.0, 303.0, 652.0, 352.5, 652.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [ 49.5, 528.0, 29.0, 528.0, 29.0, 718.0, 47.0, 718.0, 49.5, 763.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-24", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-61", 0 ],
					"destination" : [ "obj-56", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 0 ],
					"destination" : [ "obj-59", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-65", 0 ],
					"destination" : [ "obj-67", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-67", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-68", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-68", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-64", 0 ],
					"destination" : [ "obj-66", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-56", 0 ],
					"destination" : [ "obj-54", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 0 ],
					"destination" : [ "obj-54", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-54", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-58", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-74", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-58", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-74", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-58", 0 ],
					"hidden" : 0,
					"midpoints" : [ 116.5, 883.0, 49.5, 883.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-58", 0 ],
					"hidden" : 0,
					"midpoints" : [ 185.5, 883.0, 49.5, 883.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 1 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 0,
					"midpoints" : [ 271.833344, 707.0, 166.0, 707.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 0 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 0,
					"midpoints" : [ 239.5, 707.0, 166.0, 707.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-23", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-75", 0 ],
					"destination" : [ "obj-76", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-44", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-52", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 2 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [ 304.166656, 708.0, 469.0, 708.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-22", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 1 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 3 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 1 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
