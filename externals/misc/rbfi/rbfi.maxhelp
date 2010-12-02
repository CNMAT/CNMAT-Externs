{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 16.0, 44.0, 1147.0, 718.0 ],
		"bglocked" : 0,
		"defrect" : [ 16.0, 44.0, 1147.0, 718.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 4.0, 4.0 ],
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
					"text" : "p 3rd_currCoords",
					"numinlets" : 1,
					"patching_rect" : [ 261.0, 636.0, 104.0, 20.0 ],
					"id" : "obj-54",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 580.0, 318.0, 469.0, 219.0 ],
						"bglocked" : 0,
						"defrect" : [ 580.0, 318.0, 469.0, 219.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
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
									"text" : "<< when you select a point via shift-click, the current list of points and their weights are output.  ",
									"linecount" : 4,
									"numinlets" : 1,
									"patching_rect" : [ 240.0, 104.0, 150.0, 52.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"numinlets" : 2,
									"patching_rect" : [ 50.0, 100.0, 179.0, 18.0 ],
									"id" : "obj-8",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 210.0, 40.0, 25.0, 25.0 ],
									"id" : "obj-53",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-53", 0 ],
									"destination" : [ "obj-8", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p 2nd_distances",
					"numinlets" : 1,
					"patching_rect" : [ 140.0, 636.0, 98.0, 20.0 ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 131.0, 181.0, 778.0, 476.0 ],
						"bglocked" : 0,
						"defrect" : [ 131.0, 181.0, 778.0, 476.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
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
									"text" : "the 3rd outlet outputs a list of distances from the cursor",
									"numinlets" : 1,
									"patching_rect" : [ 44.0, 48.0, 254.0, 18.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "/p5 1.448055 /p4 1.395685 /p3 1.248717 /p2 1.241938 /p1 1.120474 /p0 0.953458",
									"numinlets" : 2,
									"patching_rect" : [ 44.0, 76.0, 639.0, 16.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 664.0, 36.0, 25.0, 25.0 ],
									"id" : "obj-68",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-68", 0 ],
									"destination" : [ "obj-2", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"numinlets" : 1,
					"patching_rect" : [ 536.0, 20.0, 56.0, 19.0 ],
					"id" : "obj-78",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"fontsize" : 11.0,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "uzi 6",
					"numinlets" : 2,
					"patching_rect" : [ 536.0, 52.0, 44.0, 19.0 ],
					"id" : "obj-73",
					"fontname" : "Arial",
					"numoutlets" : 3,
					"outlettype" : [ "bang", "bang", "int" ],
					"fontsize" : 11.0,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "add_point",
					"numinlets" : 2,
					"patching_rect" : [ 536.0, 80.0, 59.0, 17.0 ],
					"id" : "obj-74",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 11.0,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "click-drag: \n-- explore the space and output weights",
					"linecount" : 2,
					"numinlets" : 1,
					"patching_rect" : [ 536.0, 324.0, 233.0, 34.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "rbfi is a radial basis function interpolation tool that allows you to utilize multiple bases in tandem.  This idea originates back to David Wessel in conceiving of a high dimensional space output from a low dimentional control structure.  ",
					"linecount" : 4,
					"numinlets" : 1,
					"patching_rect" : [ 100.0, 24.0, 405.0, 71.0 ],
					"id" : "obj-71",
					"fontname" : "Arial",
					"textcolor" : [ 0.686275, 0.784314, 0.701961, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 14.0,
					"frgb" : [ 0.686275, 0.784314, 0.701961, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "< the combined weights' output represents how the circles interact as you have placed them in xy space.  note how the range of influence yields a mixed output whereby you may have more than one weight present.",
					"linecount" : 5,
					"numinlets" : 1,
					"patching_rect" : [ 840.0, 460.0, 247.0, 75.0 ],
					"id" : "obj-70",
					"fontname" : "Arial",
					"textcolor" : [ 0.686275, 0.784314, 0.701961, 1.0 ],
					"numoutlets" : 0,
					"fontface" : 2,
					"fontsize" : 12.0,
					"frgb" : [ 0.686275, 0.784314, 0.701961, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p 4th_mouse_output",
					"numinlets" : 1,
					"patching_rect" : [ 381.0, 636.0, 120.0, 20.0 ],
					"id" : "obj-69",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 554.0, 455.0, 257.0, 208.0 ],
						"bglocked" : 0,
						"defrect" : [ 554.0, 455.0, 257.0, 208.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 120.0, 116.0, 30.0, 30.0 ],
									"id" : "obj-26",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 48.0, 116.0, 30.0, 30.0 ],
									"id" : "obj-23",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "1.524 0.042",
									"numinlets" : 2,
									"patching_rect" : [ 96.0, 156.0, 114.0, 18.0 ],
									"id" : "obj-37",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "route mousedown mouseup",
									"numinlets" : 1,
									"patching_rect" : [ 48.0, 80.0, 162.0, 20.0 ],
									"id" : "obj-22",
									"fontname" : "Arial",
									"numoutlets" : 3,
									"outlettype" : [ "", "", "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 48.0, 40.0, 25.0, 25.0 ],
									"id" : "obj-68",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-22", 2 ],
									"destination" : [ "obj-37", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 1 ],
									"destination" : [ "obj-26", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-68", 0 ],
									"destination" : [ "obj-22", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p see_list_output",
					"numinlets" : 1,
					"bgcolor" : [ 0.380392, 0.447059, 0.501961, 1.0 ],
					"patching_rect" : [ 632.0, 432.0, 101.0, 20.0 ],
					"id" : "obj-67",
					"fontname" : "Arial",
					"color" : [ 0.85098, 0.85098, 0.85098, 1.0 ],
					"textcolor" : [ 0.964706, 0.94902, 0.945098, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 640.0, 480.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
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
									"text" : "/p5 0.084868 /p4 0.095916 /p3 0.138808 /p2 0.141341 /p1 0.198954 /p0 0.340112",
									"linecount" : 2,
									"numinlets" : 2,
									"patching_rect" : [ 50.0, 100.0, 240.0, 32.0 ],
									"id" : "obj-21",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 271.0, 40.0, 25.0, 25.0 ],
									"id" : "obj-66",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-66", 0 ],
									"destination" : [ "obj-21", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\rmax launch_browser http://www.cnmat.berkeley.edu/downloads/",
					"linecount" : 2,
					"numinlets" : 2,
					"patching_rect" : [ 865.0, 658.0, 266.0, 25.0 ],
					"id" : "obj-15",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "fpic",
					"numinlets" : 1,
					"patching_rect" : [ 867.0, 557.0, 109.0, 48.0 ],
					"pic" : "cnmat_wht-trans.png",
					"id" : "obj-64",
					"numoutlets" : 0,
					"embed" : 1,
					"data" : [ 593, "", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "rbfi, and all other CNMAT Max objects, can be found at:",
					"numinlets" : 1,
					"patching_rect" : [ 865.0, 611.0, 231.0, 17.0 ],
					"id" : "obj-18",
					"fontname" : "Arial",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "rbfi.help version 1.0 by Jeff Lubow and John MacCallum",
					"linecount" : 2,
					"numinlets" : 1,
					"patching_rect" : [ 979.0, 581.0, 147.0, 27.0 ],
					"id" : "obj-65",
					"fontname" : "Arial",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "note: to use rbfi to its fullest, help in locked patchers can _NOT_ be on.",
					"linecount" : 3,
					"numinlets" : 1,
					"patching_rect" : [ 955.0, 36.0, 172.0, 48.0 ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"textcolor" : [ 0.960784, 0.811765, 1.0, 1.0 ],
					"numoutlets" : 0,
					"fontface" : 2,
					"fontsize" : 12.0,
					"frgb" : [ 0.960784, 0.811765, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p routing_weights",
					"numinlets" : 0,
					"bgcolor" : [ 0.380392, 0.447059, 0.501961, 1.0 ],
					"patching_rect" : [ 796.0, 257.0, 312.0, 32.0 ],
					"id" : "obj-59",
					"fontname" : "Arial",
					"color" : [ 0.85098, 0.85098, 0.85098, 1.0 ],
					"textcolor" : [ 0.964706, 0.94902, 0.945098, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 22.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 648.0, 194.0, 483.0, 266.0 ],
						"bglocked" : 0,
						"defrect" : [ 648.0, 194.0, 483.0, 266.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
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
									"text" : "these are the respective weights of bob, sammy, and jims.",
									"linecount" : 3,
									"numinlets" : 1,
									"patching_rect" : [ 60.0, 196.0, 109.0, 41.0 ],
									"id" : "obj-24",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "click this.",
									"numinlets" : 1,
									"patching_rect" : [ 248.0, 88.0, 51.0, 18.0 ],
									"id" : "obj-22",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "route bob sammy jims",
									"numinlets" : 1,
									"patching_rect" : [ 40.0, 128.0, 128.0, 20.0 ],
									"id" : "obj-18",
									"fontname" : "Arial",
									"numoutlets" : 4,
									"outlettype" : [ "", "", "", "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s rbfi",
									"numinlets" : 1,
									"patching_rect" : [ 248.0, 172.0, 52.0, 18.0 ],
									"id" : "obj-11",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "clear, add_point name bob, add_point name sammy, add_point name jims, move 0.5 0.5",
									"linecount" : 2,
									"numinlets" : 2,
									"patching_rect" : [ 248.0, 104.0, 211.0, 27.0 ],
									"id" : "obj-8",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"patching_rect" : [ 168.0, 172.0, 50.0, 18.0 ],
									"id" : "obj-5",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "bang" ],
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"patching_rect" : [ 104.0, 172.0, 50.0, 18.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "bang" ],
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"patching_rect" : [ 40.0, 172.0, 50.0, 18.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "bang" ],
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "r rbfi_weights",
									"numinlets" : 0,
									"patching_rect" : [ 40.0, 28.0, 82.0, 20.0 ],
									"id" : "obj-46",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-46", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 2 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 1 ],
									"destination" : [ "obj-4", 0 ],
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
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p misc_options",
					"numinlets" : 0,
					"bgcolor" : [ 0.380392, 0.447059, 0.501961, 1.0 ],
					"patching_rect" : [ 796.0, 214.0, 312.0, 32.0 ],
					"id" : "obj-50",
					"fontname" : "Arial",
					"color" : [ 0.85098, 0.85098, 0.85098, 1.0 ],
					"textcolor" : [ 0.964706, 0.94902, 0.945098, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 22.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 559.0, 80.0, 659.0, 305.0 ],
						"bglocked" : 0,
						"defrect" : [ 559.0, 80.0, 659.0, 305.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
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
									"text" : "xmin 0, xmax 1, ymin 0, ymax 1",
									"numinlets" : 2,
									"patching_rect" : [ 372.0, 88.0, 165.0, 17.0 ],
									"id" : "obj-7",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 11.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "< automate a tour",
									"numinlets" : 1,
									"patching_rect" : [ 72.0, 132.0, 89.0, 18.0 ],
									"id" : "obj-13",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p takeTour",
									"numinlets" : 1,
									"patching_rect" : [ 44.0, 164.0, 62.0, 19.0 ],
									"id" : "obj-51",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 11.0,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 640.0, 44.0, 640.0, 480.0 ],
										"bglocked" : 0,
										"defrect" : [ 640.0, 44.0, 640.0, 480.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "s rbfi",
													"numinlets" : 1,
													"patching_rect" : [ 82.0, 340.0, 37.0, 20.0 ],
													"id" : "obj-1",
													"fontname" : "Arial",
													"numoutlets" : 0,
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "line 0.",
													"numinlets" : 3,
													"patching_rect" : [ 139.0, 140.0, 46.0, 20.0 ],
													"id" : "obj-109",
													"fontname" : "Arial",
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pak 0.8 0.8",
													"numinlets" : 2,
													"patching_rect" : [ 325.0, 216.0, 70.0, 20.0 ],
													"id" : "obj-108",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "line 0.",
													"numinlets" : 3,
													"patching_rect" : [ 245.0, 180.0, 46.0, 20.0 ],
													"id" : "obj-107",
													"fontname" : "Arial",
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "0.2, 0.8 2000",
													"numinlets" : 2,
													"patching_rect" : [ 266.0, 100.0, 81.0, 18.0 ],
													"id" : "obj-106",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "!- 1.",
													"numinlets" : 2,
													"patching_rect" : [ 139.0, 180.0, 32.5, 20.0 ],
													"id" : "obj-105",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "0.5, 0.8 2000",
													"numinlets" : 2,
													"patching_rect" : [ 167.0, 100.0, 81.0, 18.0 ],
													"id" : "obj-103",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pak 0.8 0.8",
													"numinlets" : 2,
													"patching_rect" : [ 196.0, 216.0, 70.0, 20.0 ],
													"id" : "obj-94",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "0.2, 0.5 2000",
													"numinlets" : 2,
													"patching_rect" : [ 65.0, 100.0, 81.0, 18.0 ],
													"id" : "obj-92",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "line 0.",
													"numinlets" : 3,
													"patching_rect" : [ 327.0, 180.0, 46.0, 20.0 ],
													"id" : "obj-91",
													"fontname" : "Arial",
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "0.8, 0.2 2000",
													"numinlets" : 2,
													"patching_rect" : [ 375.0, 100.0, 81.0, 18.0 ],
													"id" : "obj-90",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "prepend move",
													"numinlets" : 1,
													"patching_rect" : [ 82.0, 276.0, 87.0, 20.0 ],
													"id" : "obj-50",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack 0.2 0.8",
													"numinlets" : 2,
													"patching_rect" : [ 82.0, 216.0, 76.0, 20.0 ],
													"id" : "obj-46",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "line 0.",
													"numinlets" : 3,
													"patching_rect" : [ 82.0, 140.0, 46.0, 20.0 ],
													"id" : "obj-43",
													"fontname" : "Arial",
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 65.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-48",
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-105", 0 ],
													"destination" : [ "obj-46", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-109", 0 ],
													"destination" : [ "obj-105", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-109", 0 ],
													"destination" : [ "obj-46", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-50", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-103", 0 ],
													"destination" : [ "obj-109", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-43", 0 ],
													"destination" : [ "obj-46", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-106", 0 ],
													"destination" : [ "obj-107", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-107", 0 ],
													"destination" : [ "obj-94", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-94", 0 ],
													"destination" : [ "obj-50", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-43", 0 ],
													"destination" : [ "obj-46", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-92", 0 ],
													"destination" : [ "obj-43", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-46", 0 ],
													"destination" : [ "obj-50", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-90", 0 ],
													"destination" : [ "obj-91", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-43", 1 ],
													"destination" : [ "obj-103", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-109", 1 ],
													"destination" : [ "obj-106", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-107", 1 ],
													"destination" : [ "obj-90", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-91", 0 ],
													"destination" : [ "obj-108", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-108", 0 ],
													"destination" : [ "obj-50", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-48", 0 ],
													"destination" : [ "obj-92", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 12.0,
										"globalpatchername" : "",
										"fontface" : 0,
										"fontsize" : 12.0,
										"default_fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 44.0, 132.0, 20.0, 20.0 ],
									"id" : "obj-10",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "always_draw_circles 1",
									"numinlets" : 2,
									"patching_rect" : [ 332.0, 160.0, 122.0, 17.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 11.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "alter the radii of all points",
									"linecount" : 2,
									"numinlets" : 1,
									"patching_rect" : [ 536.0, 188.0, 103.0, 32.0 ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 11.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"patching_rect" : [ 512.0, 160.0, 48.0, 19.0 ],
									"id" : "obj-6",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"maximum" : 2.0,
									"outlettype" : [ "float", "bang" ],
									"fontsize" : 11.0,
									"minimum" : -2.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "xmin 0, xmax 200, ymin 0, ymax 1",
									"numinlets" : 2,
									"patching_rect" : [ 360.0, 68.0, 177.0, 17.0 ],
									"id" : "obj-63",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 11.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"patching_rect" : [ 456.0, 160.0, 47.0, 19.0 ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"maximum" : 2.0,
									"outlettype" : [ "float", "bang" ],
									"fontsize" : 11.0,
									"minimum" : -2.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pak 0.05 0.1",
									"numinlets" : 2,
									"patching_rect" : [ 456.0, 188.0, 75.0, 19.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 11.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "inner_radius $1, outer_radius $2",
									"numinlets" : 2,
									"patching_rect" : [ 456.0, 224.0, 169.0, 17.0 ],
									"id" : "obj-11",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 11.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 176.0, 24.0, 46.0, 46.0 ],
									"id" : "obj-35",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p make_points",
									"numinlets" : 1,
									"patching_rect" : [ 176.0, 88.0, 89.0, 20.0 ],
									"id" : "obj-34",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 50.0, 94.0, 640.0, 480.0 ],
										"bglocked" : 0,
										"defrect" : [ 50.0, 94.0, 640.0, 480.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 10.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 0,
										"gridsize" : [ 4.0, 4.0 ],
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
													"text" : "sprintf add_point name ps_%d",
													"numinlets" : 1,
													"patching_rect" : [ 210.0, 140.0, 173.0, 20.0 ],
													"id" : "obj-26",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b b clear",
													"numinlets" : 1,
													"patching_rect" : [ 50.0, 100.0, 63.0, 20.0 ],
													"id" : "obj-57",
													"fontname" : "Arial",
													"numoutlets" : 3,
													"outlettype" : [ "bang", "bang", "clear" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "uzi 4",
													"numinlets" : 2,
													"patching_rect" : [ 56.0, 152.0, 46.0, 20.0 ],
													"id" : "obj-52",
													"fontname" : "Arial",
													"numoutlets" : 3,
													"outlettype" : [ "bang", "bang", "int" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-28",
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"patching_rect" : [ 98.333313, 268.0, 25.0, 25.0 ],
													"id" : "obj-29",
													"numoutlets" : 0,
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-26", 0 ],
													"destination" : [ "obj-29", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-57", 2 ],
													"destination" : [ "obj-29", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-28", 0 ],
													"destination" : [ "obj-57", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-52", 2 ],
													"destination" : [ "obj-26", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-57", 1 ],
													"destination" : [ "obj-52", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 10.0,
										"globalpatchername" : "",
										"fontface" : 0,
										"fontsize" : 10.0,
										"default_fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p make_points_with_null",
									"numinlets" : 1,
									"patching_rect" : [ 20.0, 88.0, 142.0, 20.0 ],
									"id" : "obj-30",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 25.0, 69.0, 640.0, 480.0 ],
										"bglocked" : 0,
										"defrect" : [ 25.0, 69.0, 640.0, 480.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 10.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 0,
										"gridsize" : [ 4.0, 4.0 ],
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
													"text" : "sprintf add_point name ps_%d inner_radius 0.1 outer_radius 0.5",
													"numinlets" : 1,
													"patching_rect" : [ 210.0, 140.0, 353.0, 20.0 ],
													"id" : "obj-26",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b b clear",
													"numinlets" : 1,
													"patching_rect" : [ 50.0, 100.0, 63.0, 20.0 ],
													"id" : "obj-57",
													"fontname" : "Arial",
													"numoutlets" : 3,
													"outlettype" : [ "bang", "bang", "clear" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "add_point name null rgb 0. 0. 0. outer_radius 0.00001 inner_radius 1. coords 0.5 0.5 locked 1",
													"numinlets" : 2,
													"patching_rect" : [ 50.0, 176.0, 507.0, 18.0 ],
													"id" : "obj-54",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "uzi 4",
													"numinlets" : 2,
													"patching_rect" : [ 146.0, 124.0, 46.0, 20.0 ],
													"id" : "obj-52",
													"fontname" : "Arial",
													"numoutlets" : 3,
													"outlettype" : [ "bang", "bang", "int" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-28",
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"patching_rect" : [ 98.333313, 268.0, 25.0, 25.0 ],
													"id" : "obj-29",
													"numoutlets" : 0,
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-52", 2 ],
													"destination" : [ "obj-26", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-26", 0 ],
													"destination" : [ "obj-29", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-57", 0 ],
													"destination" : [ "obj-54", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-57", 1 ],
													"destination" : [ "obj-52", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-28", 0 ],
													"destination" : [ "obj-57", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-54", 0 ],
													"destination" : [ "obj-29", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-57", 2 ],
													"destination" : [ "obj-29", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 10.0,
										"globalpatchername" : "",
										"fontface" : 0,
										"fontsize" : 10.0,
										"default_fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s rbfi",
									"numinlets" : 1,
									"patching_rect" : [ 176.0, 276.0, 37.0, 20.0 ],
									"id" : "obj-58",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 20.0, 24.0, 46.0, 46.0 ],
									"id" : "obj-56",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "change frame size - this remaps the output you get from the 3rd (mouse) outlet.  \nthe defaults are:\nxmin 0\nxmax 1\nymin 0\nymax 1",
									"linecount" : 7,
									"numinlets" : 1,
									"patching_rect" : [ 304.0, 16.0, 229.0, 95.0 ],
									"id" : "obj-5",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 11.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-51", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-58", 0 ],
									"hidden" : 0,
									"midpoints" : [ 341.5, 244.0, 185.5, 244.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-63", 0 ],
									"destination" : [ "obj-58", 0 ],
									"hidden" : 0,
									"midpoints" : [ 369.5, 131.0, 185.5, 131.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-58", 0 ],
									"hidden" : 0,
									"midpoints" : [ 465.5, 254.0, 185.5, 254.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-2", 1 ],
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
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-35", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-58", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-56", 0 ],
									"destination" : [ "obj-30", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-58", 0 ],
									"hidden" : 0,
									"midpoints" : [ 29.5, 223.5, 185.5, 223.5 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-58", 0 ],
									"hidden" : 0,
									"midpoints" : [ 381.5, 137.0, 185.5, 137.0 ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p weights_with_null",
					"numinlets" : 0,
					"bgcolor" : [ 0.380392, 0.447059, 0.501961, 1.0 ],
					"patching_rect" : [ 796.0, 300.0, 312.0, 32.0 ],
					"id" : "obj-48",
					"fontname" : "Arial",
					"color" : [ 0.85098, 0.85098, 0.85098, 1.0 ],
					"textcolor" : [ 0.964706, 0.94902, 0.945098, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 22.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 343.0, 149.0, 733.0, 291.0 ],
						"bglocked" : 0,
						"defrect" : [ 343.0, 149.0, 733.0, 291.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
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
									"text" : "p to_coords",
									"numinlets" : 2,
									"patching_rect" : [ 592.0, 172.0, 63.0, 18.0 ],
									"id" : "obj-21",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0,
									"hidden" : 1,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 50.0, 94.0, 232.0, 284.0 ],
										"bglocked" : 0,
										"defrect" : [ 50.0, 94.0, 232.0, 284.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 10.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 0,
										"gridsize" : [ 4.0, 4.0 ],
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
													"text" : "/ 127.",
													"numinlets" : 2,
													"patching_rect" : [ 130.0, 100.0, 36.0, 18.0 ],
													"id" : "obj-12",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontsize" : 10.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "/ 127.",
													"numinlets" : 2,
													"patching_rect" : [ 50.0, 100.0, 36.0, 18.0 ],
													"id" : "obj-11",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontsize" : 10.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pak 0. 0.",
													"numinlets" : 2,
													"patching_rect" : [ 50.0, 120.0, 57.0, 20.0 ],
													"id" : "obj-20",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-14",
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 130.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-16",
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"patching_rect" : [ 50.0, 204.5, 25.0, 25.0 ],
													"id" : "obj-17",
													"numoutlets" : 0,
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-20", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-11", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-16", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-20", 0 ],
													"destination" : [ "obj-17", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-20", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 10.0,
										"globalpatchername" : "",
										"fontface" : 0,
										"fontsize" : 10.0,
										"default_fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "pictslider",
									"numinlets" : 2,
									"patching_rect" : [ 592.0, 116.0, 100.0, 100.0 ],
									"id" : "obj-22",
									"numoutlets" : 2,
									"outlettype" : [ "int", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "prepend move",
									"numinlets" : 1,
									"patching_rect" : [ 592.0, 216.0, 87.0, 20.0 ],
									"id" : "obj-30",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "add_point name bobby outer_radius 0.3, add_point name fizzpop outer_radius 0.3",
									"linecount" : 2,
									"numinlets" : 2,
									"patching_rect" : [ 340.0, 112.0, 240.0, 32.0 ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s rbfi",
									"numinlets" : 1,
									"patching_rect" : [ 304.0, 256.0, 32.0, 18.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "consider this as a way to create \"dead space\" in your interpolation where weights can go to zero (could be useful for gain).",
									"linecount" : 2,
									"numinlets" : 1,
									"patching_rect" : [ 308.0, 24.0, 305.0, 29.0 ],
									"id" : "obj-6",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "clear, add_point name null rgb 0. 0. 0. inner_radius 0.01 outer_radius 1. coords 0.5 0.5 locked 1",
									"linecount" : 2,
									"numinlets" : 2,
									"patching_rect" : [ 304.0, 64.0, 288.0, 32.0 ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "r rbfi_weights",
									"numinlets" : 0,
									"patching_rect" : [ 112.0, 56.0, 82.0, 20.0 ],
									"id" : "obj-46",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p catch_weights-nonull",
									"numinlets" : 1,
									"patching_rect" : [ 112.0, 84.0, 131.0, 20.0 ],
									"id" : "obj-65",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 25.0, 69.0, 238.0, 327.0 ],
										"bglocked" : 0,
										"defrect" : [ 25.0, 69.0, 238.0, 327.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 10.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 0,
										"gridsize" : [ 4.0, 4.0 ],
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
													"text" : "route done null",
													"numinlets" : 1,
													"patching_rect" : [ 50.0, 100.0, 90.0, 20.0 ],
													"id" : "obj-24",
													"fontname" : "Arial",
													"numoutlets" : 3,
													"outlettype" : [ "", "", "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "zl 1024 group",
													"numinlets" : 2,
													"patching_rect" : [ 52.0, 180.0, 83.0, 20.0 ],
													"id" : "obj-23",
													"fontname" : "Arial",
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "zl slice 1",
													"numinlets" : 2,
													"patching_rect" : [ 100.0, 132.0, 57.0, 20.0 ],
													"id" : "obj-22",
													"fontname" : "Arial",
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-45",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"patching_rect" : [ 50.0, 248.0, 25.0, 25.0 ],
													"id" : "obj-46",
													"numoutlets" : 0,
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-24", 0 ],
													"destination" : [ "obj-23", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-45", 0 ],
													"destination" : [ "obj-24", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-24", 2 ],
													"destination" : [ "obj-22", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-22", 1 ],
													"destination" : [ "obj-23", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-23", 0 ],
													"destination" : [ "obj-46", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 10.0,
										"globalpatchername" : "",
										"fontface" : 0,
										"fontsize" : 10.0,
										"default_fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"numinlets" : 1,
									"patching_rect" : [ 38.0, 180.0, 50.0, 20.0 ],
									"id" : "obj-66",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "int", "bang" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl len",
									"numinlets" : 2,
									"patching_rect" : [ 38.0, 132.0, 69.0, 20.0 ],
									"id" : "obj-67",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "multislider",
									"numinlets" : 1,
									"patching_rect" : [ 112.0, 120.0, 153.0, 89.689651 ],
									"id" : "obj-68",
									"size" : 6,
									"numoutlets" : 2,
									"setminmax" : [ 0.0, 1.0 ],
									"outlettype" : [ "", "" ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-46", 0 ],
									"destination" : [ "obj-65", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-65", 0 ],
									"destination" : [ "obj-67", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-65", 0 ],
									"destination" : [ "obj-68", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-67", 0 ],
									"destination" : [ "obj-66", 0 ],
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
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 1 ],
									"destination" : [ "obj-21", 1 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-30", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s rbfi_weights",
					"numinlets" : 1,
					"patching_rect" : [ 20.0, 636.0, 85.0, 20.0 ],
					"id" : "obj-44",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p dump/save/load",
					"numinlets" : 1,
					"bgcolor" : [ 0.380392, 0.447059, 0.501961, 1.0 ],
					"patching_rect" : [ 704.0, 620.0, 137.0, 24.0 ],
					"id" : "obj-43",
					"fontname" : "Arial",
					"color" : [ 0.85098, 0.85098, 0.85098, 1.0 ],
					"textcolor" : [ 0.964706, 0.94902, 0.945098, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 15.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 548.0, 312.0, 489.0, 279.0 ],
						"bgcolor" : [ 0.266667, 0.223529, 0.223529, 1.0 ],
						"bglocked" : 0,
						"defrect" : [ 548.0, 312.0, 489.0, 279.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
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
									"patching_rect" : [ 144.0, 80.0, 25.0, 25.0 ],
									"id" : "obj-3",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "< dump the coll contents back into rbfi for recall\nof presets.",
									"linecount" : 4,
									"numinlets" : 1,
									"patching_rect" : [ 356.0, 96.0, 109.0, 62.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"textcolor" : [ 0.92549, 0.92549, 0.92549, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"frgb" : [ 0.92549, 0.92549, 0.92549, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s rbfi",
									"numinlets" : 1,
									"patching_rect" : [ 28.0, 232.0, 38.0, 20.0 ],
									"id" : "obj-64",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p coll_management",
									"numinlets" : 2,
									"patching_rect" : [ 216.0, 136.0, 115.0, 20.0 ],
									"id" : "obj-63",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 50.0, 94.0, 365.0, 436.0 ],
										"bglocked" : 0,
										"defrect" : [ 50.0, 94.0, 365.0, 436.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 10.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 0,
										"gridsize" : [ 4.0, 4.0 ],
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
													"text" : "print done_dumping",
													"numinlets" : 1,
													"patching_rect" : [ 192.0, 200.0, 99.0, 18.0 ],
													"id" : "obj-1",
													"fontname" : "Arial",
													"numoutlets" : 0,
													"fontsize" : 10.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "route symbol",
													"numinlets" : 1,
													"patching_rect" : [ 84.0, 244.0, 79.0, 20.0 ],
													"id" : "obj-44",
													"fontname" : "Arial",
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "prepend",
													"numinlets" : 1,
													"patching_rect" : [ 84.0, 220.0, 55.0, 20.0 ],
													"id" : "obj-42",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "prepend set",
													"numinlets" : 1,
													"patching_rect" : [ 100.0, 196.0, 74.0, 20.0 ],
													"id" : "obj-32",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "route done space",
													"numinlets" : 1,
													"patching_rect" : [ 34.0, 80.0, 103.0, 20.0 ],
													"id" : "obj-41",
													"fontname" : "Arial",
													"numoutlets" : 3,
													"outlettype" : [ "", "", "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "prepend store",
													"numinlets" : 1,
													"patching_rect" : [ 84.0, 124.0, 85.0, 20.0 ],
													"id" : "obj-40",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "coll",
													"numinlets" : 1,
													"patching_rect" : [ 84.0, 172.0, 59.5, 20.0 ],
													"id" : "obj-21",
													"fontname" : "Arial",
													"numoutlets" : 4,
													"outlettype" : [ "", "", "", "" ],
													"fontsize" : 12.0,
													"saved_object_attributes" : 													{
														"embed" : 0
													}

												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 34.0, 20.0, 25.0, 25.0 ],
													"id" : "obj-60",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 144.0, 20.0, 25.0, 25.0 ],
													"id" : "obj-61",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"patching_rect" : [ 84.0, 324.0, 25.0, 25.0 ],
													"id" : "obj-62",
													"numoutlets" : 0,
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-41", 2 ],
													"destination" : [ "obj-40", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-60", 0 ],
													"destination" : [ "obj-41", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-41", 0 ],
													"destination" : [ "obj-1", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-44", 0 ],
													"destination" : [ "obj-62", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-61", 0 ],
													"destination" : [ "obj-21", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-40", 0 ],
													"destination" : [ "obj-21", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-21", 1 ],
													"destination" : [ "obj-32", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-21", 0 ],
													"destination" : [ "obj-42", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-32", 0 ],
													"destination" : [ "obj-42", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-42", 0 ],
													"destination" : [ "obj-44", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 10.0,
										"globalpatchername" : "",
										"fontface" : 0,
										"fontsize" : 10.0,
										"default_fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print dump",
									"numinlets" : 1,
									"patching_rect" : [ 144.0, 136.0, 69.0, 20.0 ],
									"id" : "obj-11",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "dump",
									"numinlets" : 2,
									"bgcolor" : [ 0.062745, 0.615686, 0.090196, 1.0 ],
									"patching_rect" : [ 312.0, 96.0, 42.0, 18.0 ],
									"id" : "obj-31",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0,
									"bgcolor2" : [ 0.062745, 0.615686, 0.090196, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "< dump the entire list of parameters out the right-most outlet.",
									"numinlets" : 1,
									"bgcolor" : [ 0.333333, 0.094118, 0.094118, 1.0 ],
									"patching_rect" : [ 88.0, 24.0, 333.0, 20.0 ],
									"id" : "obj-42",
									"fontname" : "Arial",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "dump",
									"numinlets" : 2,
									"bgcolor" : [ 0.333333, 0.094118, 0.094118, 1.0 ],
									"patching_rect" : [ 28.0, 24.0, 56.0, 24.0 ],
									"id" : "obj-27",
									"fontname" : "Arial",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 17.042124,
									"bgcolor2" : [ 0.333333, 0.094118, 0.094118, 1.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-27", 0 ],
									"destination" : [ "obj-64", 0 ],
									"hidden" : 0,
									"color" : [ 0.556863, 0.776471, 0.847059, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"color" : [ 0.556863, 0.776471, 0.847059, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-63", 0 ],
									"hidden" : 0,
									"color" : [ 0.556863, 0.776471, 0.847059, 1.0 ],
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-31", 0 ],
									"destination" : [ "obj-63", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-63", 0 ],
									"destination" : [ "obj-64", 0 ],
									"hidden" : 0,
									"color" : [ 0.556863, 0.776471, 0.847059, 1.0 ],
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpatcher",
					"prototypename" : "cnmat_banner",
					"numinlets" : 0,
					"patching_rect" : [ 615.0, 20.0, 511.0, 76.0 ],
					"id" : "obj-40",
					"numoutlets" : 0,
					"name" : "banner.maxpat",
					"offset" : [ -18.0, -68.0 ],
					"args" : [ "rbfi", "xy radial basis function interpolator" ],
					"lockeddragscroll" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "shift-drag:\n-- if within the area of the inner circle, dragging will move a point to new xy\n-- if on the inner circle, dragging changes the radius of said circle.\n-- if on the outer circle, dragging alters the influence (spread) of the circle.",
					"linecount" : 7,
					"numinlets" : 1,
					"patching_rect" : [ 536.0, 220.0, 237.0, 103.0 ],
					"id" : "obj-38",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "MOUSE XY USER INTERACTION:",
					"numinlets" : 1,
					"patching_rect" : [ 536.0, 120.0, 231.0, 23.0 ],
					"id" : "obj-33",
					"fontname" : "Arial",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numoutlets" : 0,
					"fontface" : 3,
					"fontsize" : 14.0,
					"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p drawing_options",
					"numinlets" : 0,
					"bgcolor" : [ 0.380392, 0.447059, 0.501961, 1.0 ],
					"patching_rect" : [ 796.0, 171.0, 312.0, 32.0 ],
					"id" : "obj-32",
					"fontname" : "Arial",
					"color" : [ 0.85098, 0.85098, 0.85098, 1.0 ],
					"textcolor" : [ 0.964706, 0.94902, 0.945098, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 22.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 706.0, 267.0, 357.0, 180.0 ],
						"bglocked" : 0,
						"defrect" : [ 706.0, 267.0, 357.0, 180.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
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
									"text" : "With these two flags, you can change the way that rbfi draws its circles.",
									"linecount" : 3,
									"numinlets" : 1,
									"patching_rect" : [ 180.0, 88.0, 150.0, 41.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s rbfi",
									"numinlets" : 1,
									"patching_rect" : [ 81.0, 133.0, 32.0, 18.0 ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numinlets" : 1,
									"patching_rect" : [ 188.0, 16.0, 20.0, 20.0 ],
									"id" : "obj-13",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numinlets" : 1,
									"patching_rect" : [ 32.0, 16.0, 20.0, 20.0 ],
									"id" : "obj-12",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "always_draw_labels $1",
									"numinlets" : 2,
									"patching_rect" : [ 188.0, 44.0, 135.0, 18.0 ],
									"id" : "obj-9",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "always_draw_circles $1",
									"numinlets" : 2,
									"patching_rect" : [ 32.0, 44.0, 137.0, 18.0 ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p adding_and_altering_points",
					"numinlets" : 0,
					"bgcolor" : [ 0.380392, 0.447059, 0.501961, 1.0 ],
					"patching_rect" : [ 796.0, 128.0, 312.0, 32.0 ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"color" : [ 0.85098, 0.85098, 0.85098, 1.0 ],
					"textcolor" : [ 0.964706, 0.94902, 0.945098, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 22.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 132.0, 131.0, 978.0, 426.0 ],
						"bgcolor" : [ 0.298039, 0.352941, 0.368627, 1.0 ],
						"bglocked" : 0,
						"defrect" : [ 132.0, 131.0, 978.0, 426.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
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
									"text" : "Adding and altering points:",
									"numinlets" : 1,
									"patching_rect" : [ 12.0, 12.0, 337.0, 24.0 ],
									"id" : "obj-88",
									"fontname" : "Arial",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"numoutlets" : 0,
									"fontface" : 1,
									"fontsize" : 15.0,
									"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "both",
									"numinlets" : 1,
									"patching_rect" : [ 784.0, 308.0, 30.0, 18.0 ],
									"id" : "obj-86",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "move cursor (x)",
									"numinlets" : 1,
									"patching_rect" : [ 836.0, 356.0, 80.0, 18.0 ],
									"id" : "obj-85",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "move point",
									"numinlets" : 1,
									"patching_rect" : [ 696.0, 356.0, 68.0, 18.0 ],
									"id" : "obj-84",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "the move message emulates your cursor movement through the space in 2 dimensions.",
									"linecount" : 2,
									"numinlets" : 1,
									"patching_rect" : [ 716.0, 136.0, 224.0, 29.0 ],
									"id" : "obj-82",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "... or delete a point.",
									"numinlets" : 1,
									"patching_rect" : [ 360.0, 344.0, 177.0, 18.0 ],
									"id" : "obj-81",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t f f",
									"numinlets" : 1,
									"patching_rect" : [ 56.0, 372.0, 44.5, 18.0 ],
									"id" : "obj-79",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ],
									"fontsize" : 10.0,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack f f",
									"numinlets" : 2,
									"patching_rect" : [ 56.0, 424.0, 43.0, 18.0 ],
									"id" : "obj-78",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "- 0.1",
									"numinlets" : 2,
									"patching_rect" : [ 56.0, 396.0, 32.5, 18.0 ],
									"id" : "obj-77",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontsize" : 10.0,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"numinlets" : 1,
									"patching_rect" : [ 220.0, 364.0, 50.0, 18.0 ],
									"id" : "obj-76",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"maximum" : 0.63,
									"outlettype" : [ "float", "bang" ],
									"fontsize" : 10.0,
									"minimum" : 0.14
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "bar rgb 1. 1. 1.",
									"numinlets" : 2,
									"patching_rect" : [ 12.0, 336.0, 114.0, 18.0 ],
									"id" : "obj-74",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "you can alter existing points by addressing them by name.",
									"numinlets" : 1,
									"patching_rect" : [ 12.0, 312.0, 268.0, 18.0 ],
									"id" : "obj-71",
									"fontname" : "Arial",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "< the colors are assigned",
									"numinlets" : 1,
									"patching_rect" : [ 216.0, 284.0, 123.0, 18.0 ],
									"id" : "obj-70",
									"fontname" : "Arial",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "adding a new point with the same name overwrites the previous one.  click on the message below a number of times.",
									"linecount" : 2,
									"numinlets" : 1,
									"patching_rect" : [ 12.0, 244.0, 295.0, 29.0 ],
									"id" : "obj-69",
									"fontname" : "Arial",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "click on the toggle below and choose a point to see evidence of this.",
									"linecount" : 2,
									"numinlets" : 1,
									"patching_rect" : [ 360.0, 248.0, 177.0, 29.0 ],
									"id" : "obj-68",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p send_coords_to_points",
									"numinlets" : 2,
									"patching_rect" : [ 360.0, 312.0, 211.0, 18.0 ],
									"id" : "obj-67",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 0.0, 0.0, 640.0, 480.0 ],
										"bglocked" : 0,
										"defrect" : [ 0.0, 0.0, 640.0, 480.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 10.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 0,
										"gridsize" : [ 4.0, 4.0 ],
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
													"text" : "s rbfi",
													"numinlets" : 1,
													"patching_rect" : [ 50.0, 164.0, 37.0, 20.0 ],
													"id" : "obj-64",
													"fontname" : "Arial",
													"numoutlets" : 0,
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "prepend yesmove coords",
													"numinlets" : 1,
													"patching_rect" : [ 170.0, 140.0, 123.0, 18.0 ],
													"id" : "obj-63",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 10.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "prepend nomove coords",
													"numinlets" : 1,
													"patching_rect" : [ 50.0, 140.0, 118.0, 18.0 ],
													"id" : "obj-52",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 10.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "gate 2",
													"numinlets" : 2,
													"patching_rect" : [ 50.0, 112.0, 38.0, 18.0 ],
													"id" : "obj-54",
													"fontname" : "Arial",
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontsize" : 10.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "p gen_new_coords",
													"numinlets" : 1,
													"patching_rect" : [ 126.0, 100.0, 95.0, 18.0 ],
													"id" : "obj-47",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 10.0,
													"patcher" : 													{
														"fileversion" : 1,
														"rect" : [ 220.0, 88.0, 187.0, 318.0 ],
														"bglocked" : 0,
														"defrect" : [ 220.0, 88.0, 187.0, 318.0 ],
														"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
														"openinpresentation" : 0,
														"default_fontsize" : 10.0,
														"default_fontface" : 0,
														"default_fontname" : "Arial",
														"gridonopen" : 0,
														"gridsize" : [ 4.0, 4.0 ],
														"gridsnaponopen" : 0,
														"toolbarvisible" : 1,
														"boxanimatetime" : 200,
														"imprint" : 0,
														"enablehscroll" : 1,
														"enablevscroll" : 1,
														"devicewidth" : 0.0,
														"boxes" : [ 															{
																"box" : 																{
																	"maxclass" : "outlet",
																	"numinlets" : 1,
																	"patching_rect" : [ 36.0, 236.0, 25.0, 25.0 ],
																	"id" : "obj-15",
																	"numoutlets" : 0,
																	"comment" : ""
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "inlet",
																	"numinlets" : 0,
																	"patching_rect" : [ 36.0, 24.0, 25.0, 25.0 ],
																	"id" : "obj-14",
																	"numoutlets" : 1,
																	"outlettype" : [ "bang" ],
																	"comment" : ""
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "b",
																	"numinlets" : 1,
																	"patching_rect" : [ 36.0, 56.0, 32.5, 18.0 ],
																	"id" : "obj-13",
																	"fontname" : "Arial",
																	"numoutlets" : 2,
																	"outlettype" : [ "bang", "bang" ],
																	"fontsize" : 10.0
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "pack f f",
																	"numinlets" : 2,
																	"patching_rect" : [ 36.0, 152.0, 43.0, 18.0 ],
																	"id" : "obj-12",
																	"fontname" : "Arial",
																	"numoutlets" : 1,
																	"outlettype" : [ "" ],
																	"fontsize" : 10.0
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "/ 100.",
																	"numinlets" : 2,
																	"patching_rect" : [ 36.0, 116.0, 36.0, 18.0 ],
																	"id" : "obj-9",
																	"fontname" : "Arial",
																	"numoutlets" : 1,
																	"outlettype" : [ "float" ],
																	"fontsize" : 10.0
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "random 100",
																	"numinlets" : 2,
																	"patching_rect" : [ 36.0, 88.0, 64.0, 18.0 ],
																	"id" : "obj-10",
																	"fontname" : "Arial",
																	"numoutlets" : 1,
																	"outlettype" : [ "int" ],
																	"fontsize" : 10.0
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "/ 100.",
																	"numinlets" : 2,
																	"patching_rect" : [ 104.0, 116.0, 36.0, 18.0 ],
																	"id" : "obj-3",
																	"fontname" : "Arial",
																	"numoutlets" : 1,
																	"outlettype" : [ "float" ],
																	"fontsize" : 10.0
																}

															}
, 															{
																"box" : 																{
																	"maxclass" : "newobj",
																	"text" : "random 100",
																	"numinlets" : 2,
																	"patching_rect" : [ 104.0, 88.0, 64.0, 18.0 ],
																	"id" : "obj-2",
																	"fontname" : "Arial",
																	"numoutlets" : 1,
																	"outlettype" : [ "int" ],
																	"fontsize" : 10.0
																}

															}
 ],
														"lines" : [ 															{
																"patchline" : 																{
																	"source" : [ "obj-12", 0 ],
																	"destination" : [ "obj-15", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-14", 0 ],
																	"destination" : [ "obj-13", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-13", 0 ],
																	"destination" : [ "obj-10", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-13", 1 ],
																	"destination" : [ "obj-2", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-9", 0 ],
																	"destination" : [ "obj-12", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-3", 0 ],
																	"destination" : [ "obj-12", 1 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-10", 0 ],
																	"destination" : [ "obj-9", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
, 															{
																"patchline" : 																{
																	"source" : [ "obj-2", 0 ],
																	"destination" : [ "obj-3", 0 ],
																	"hidden" : 0,
																	"midpoints" : [  ]
																}

															}
 ]
													}
,
													"saved_object_attributes" : 													{
														"default_fontname" : "Arial",
														"fontname" : "Arial",
														"default_fontsize" : 10.0,
														"globalpatchername" : "",
														"fontface" : 0,
														"fontsize" : 10.0,
														"default_fontface" : 0
													}

												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-65",
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 126.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-66",
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-66", 0 ],
													"destination" : [ "obj-47", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-65", 0 ],
													"destination" : [ "obj-54", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-63", 0 ],
													"destination" : [ "obj-64", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-52", 0 ],
													"destination" : [ "obj-64", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-54", 1 ],
													"destination" : [ "obj-63", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-54", 0 ],
													"destination" : [ "obj-52", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-47", 0 ],
													"destination" : [ "obj-54", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 10.0,
										"globalpatchername" : "",
										"fontface" : 0,
										"fontsize" : 10.0,
										"default_fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"numinlets" : 1,
									"patching_rect" : [ 552.0, 249.0, 20.0, 20.0 ],
									"id" : "obj-62",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "umenu",
									"types" : [  ],
									"numinlets" : 1,
									"patching_rect" : [ 360.0, 284.0, 110.0, 18.0 ],
									"id" : "obj-60",
									"fontname" : "Arial",
									"numoutlets" : 3,
									"outlettype" : [ "int", "", "" ],
									"fontsize" : 10.0,
									"items" : [ "off", ",", "won't move", ",", "will move" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "metro 100",
									"numinlets" : 2,
									"patching_rect" : [ 552.0, 272.0, 56.0, 18.0 ],
									"id" : "obj-53",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 12.0, 140.0, 20.0, 20.0 ],
									"id" : "obj-42",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 20",
									"numinlets" : 2,
									"patching_rect" : [ 12.0, 168.0, 46.0, 18.0 ],
									"id" : "obj-40",
									"fontname" : "Arial",
									"numoutlets" : 3,
									"outlettype" : [ "bang", "bang", "int" ],
									"fontsize" : 10.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "add_point",
									"numinlets" : 2,
									"patching_rect" : [ 12.0, 192.0, 63.0, 18.0 ],
									"id" : "obj-39",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "clear, add_point name nomove rgb 0.2 0.2 0.2 coords 0.2 0.8 locked 1, add_point name yesmove coords 0.2 0.2, add_point coords 0.5 0.5, add_point coords 0.8 0.8, add_point coords 0.8 0.2",
									"linecount" : 4,
									"numinlets" : 2,
									"patching_rect" : [ 360.0, 172.0, 280.0, 60.0 ],
									"id" : "obj-46",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "< add 20 points with the default parameters",
									"numinlets" : 1,
									"patching_rect" : [ 36.0, 140.0, 203.0, 18.0 ],
									"id" : "obj-29",
									"fontname" : "Arial",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "available parameters for a point:\ncoords (float x, float y)\nname (symbol)\ninner_radius (float between 0-1)\nouter_radius (float between 0-1)\nlocked (int flag)\nrgb (float r, float g, float b, float a)",
									"linecount" : 7,
									"numinlets" : 1,
									"patching_rect" : [ 328.0, 16.0, 212.0, 95.0 ],
									"id" : "obj-28",
									"fontname" : "Arial",
									"textcolor" : [ 0.756863, 0.756863, 0.756863, 1.0 ],
									"numoutlets" : 0,
									"fontface" : 2,
									"fontsize" : 11.0,
									"frgb" : [ 0.756863, 0.756863, 0.756863, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "to create a point programatically, use the (obligatory) add_point message.  all arguments are optional, can be given in any order, and are given in name-value pairs.",
									"linecount" : 4,
									"numinlets" : 1,
									"patching_rect" : [ 12.0, 44.0, 308.0, 75.0 ],
									"id" : "obj-25",
									"fontname" : "Arial",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"numoutlets" : 0,
									"fontface" : 2,
									"fontsize" : 15.0,
									"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "pictslider",
									"numinlets" : 2,
									"patching_rect" : [ 780.0, 264.0, 40.0, 40.0 ],
									"id" : "obj-11",
									"numoutlets" : 2,
									"outlettype" : [ "int", "int" ],
									"scaleknob" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p to_coords",
									"numinlets" : 2,
									"patching_rect" : [ 824.0, 308.0, 63.0, 18.0 ],
									"id" : "obj-21",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0,
									"hidden" : 1,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 50.0, 94.0, 232.0, 284.0 ],
										"bglocked" : 0,
										"defrect" : [ 50.0, 94.0, 232.0, 284.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 10.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 0,
										"gridsize" : [ 4.0, 4.0 ],
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
													"text" : "/ 127.",
													"numinlets" : 2,
													"patching_rect" : [ 130.0, 100.0, 36.0, 18.0 ],
													"id" : "obj-12",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontsize" : 10.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "/ 127.",
													"numinlets" : 2,
													"patching_rect" : [ 50.0, 100.0, 36.0, 18.0 ],
													"id" : "obj-11",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontsize" : 10.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pak 0. 0.",
													"numinlets" : 2,
													"patching_rect" : [ 50.0, 120.0, 57.0, 20.0 ],
													"id" : "obj-20",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-14",
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 130.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-16",
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"patching_rect" : [ 50.0, 204.5, 25.0, 25.0 ],
													"id" : "obj-17",
													"numoutlets" : 0,
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-20", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-20", 0 ],
													"destination" : [ "obj-17", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-16", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-11", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-20", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 10.0,
										"globalpatchername" : "",
										"fontface" : 0,
										"fontsize" : 10.0,
										"default_fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "pictslider",
									"numinlets" : 2,
									"patching_rect" : [ 824.0, 252.0, 100.0, 100.0 ],
									"id" : "obj-22",
									"numoutlets" : 2,
									"outlettype" : [ "int", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p to_coords",
									"numinlets" : 2,
									"patching_rect" : [ 676.0, 308.0, 63.0, 18.0 ],
									"id" : "obj-19",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 10.0,
									"hidden" : 1,
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 25.0, 69.0, 232.0, 273.0 ],
										"bglocked" : 0,
										"defrect" : [ 25.0, 69.0, 232.0, 273.0 ],
										"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
										"openinpresentation" : 0,
										"default_fontsize" : 10.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 0,
										"gridsize" : [ 4.0, 4.0 ],
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
													"text" : "/ 127.",
													"numinlets" : 2,
													"patching_rect" : [ 130.0, 100.0, 36.0, 18.0 ],
													"id" : "obj-12",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontsize" : 10.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "/ 127.",
													"numinlets" : 2,
													"patching_rect" : [ 50.0, 100.0, 36.0, 18.0 ],
													"id" : "obj-11",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontsize" : 10.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pak 0. 0.",
													"numinlets" : 2,
													"patching_rect" : [ 50.0, 120.0, 57.0, 20.0 ],
													"id" : "obj-20",
													"fontname" : "Arial",
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontsize" : 12.0
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-14",
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"numinlets" : 0,
													"patching_rect" : [ 130.0, 40.0, 25.0, 25.0 ],
													"id" : "obj-16",
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"numinlets" : 1,
													"patching_rect" : [ 50.0, 204.5, 25.0, 25.0 ],
													"id" : "obj-17",
													"numoutlets" : 0,
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-11", 0 ],
													"destination" : [ "obj-20", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-11", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-16", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-20", 0 ],
													"destination" : [ "obj-17", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-20", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
 ]
									}
,
									"saved_object_attributes" : 									{
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 10.0,
										"globalpatchername" : "",
										"fontface" : 0,
										"fontsize" : 10.0,
										"default_fontface" : 0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "pictslider",
									"numinlets" : 2,
									"patching_rect" : [ 676.0, 252.0, 100.0, 100.0 ],
									"id" : "obj-10",
									"numoutlets" : 2,
									"outlettype" : [ "int", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "clear, add_point name perfect coords 1 1 inner_radius 0.1 outer_radius 0.3, add_point name garbage coords 0 0 inner_radius 0.1 outer_radius 0.2, add_point name blank_space coords 0.5 0.5 rgb 0. 0. 0. inner_radius 0.01 outer_radius 0.1",
									"linecount" : 5,
									"numinlets" : 2,
									"patching_rect" : [ 672.0, 168.0, 281.0, 73.0 ],
									"id" : "obj-9",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "< clear all of the points",
									"numinlets" : 1,
									"patching_rect" : [ 124.0, 192.0, 111.0, 18.0 ],
									"id" : "obj-8",
									"fontname" : "Arial",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "you can lock points from being edited graphically or with the coords argument...",
									"linecount" : 2,
									"numinlets" : 1,
									"patching_rect" : [ 360.0, 140.0, 279.0, 29.0 ],
									"id" : "obj-7",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "prepend move",
									"numinlets" : 1,
									"patching_rect" : [ 824.0, 376.0, 87.0, 20.0 ],
									"id" : "obj-30",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "delete_point nomove",
									"numinlets" : 2,
									"patching_rect" : [ 360.0, 364.0, 123.0, 18.0 ],
									"id" : "obj-23",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "bar inner_radius $1 outer_radius $2",
									"numinlets" : 2,
									"patching_rect" : [ 12.0, 364.0, 202.0, 18.0 ],
									"id" : "obj-36",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "add_point name bar coords 0.5 0.5",
									"numinlets" : 2,
									"patching_rect" : [ 12.0, 284.0, 198.0, 18.0 ],
									"id" : "obj-35",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "clear",
									"numinlets" : 2,
									"patching_rect" : [ 80.0, 192.0, 37.0, 18.0 ],
									"id" : "obj-18",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "s rbfi",
									"numinlets" : 1,
									"patching_rect" : [ 352.0, 432.0, 37.0, 20.0 ],
									"id" : "obj-24",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 12.0,
									"hidden" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "perfect coords $1 $2",
									"numinlets" : 2,
									"patching_rect" : [ 676.0, 380.0, 120.0, 18.0 ],
									"id" : "obj-15",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "clear, add_point name perfect, add_point name garbage",
									"numinlets" : 2,
									"patching_rect" : [ 12.0, 220.0, 309.0, 18.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "panel",
									"numinlets" : 1,
									"bgcolor" : [ 0.368627, 0.415686, 0.466667, 1.0 ],
									"patching_rect" : [ 8.0, 132.0, 332.0, 276.0 ],
									"id" : "obj-27",
									"rounded" : 20,
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "panel",
									"numinlets" : 1,
									"bgcolor" : [ 0.368627, 0.415686, 0.466667, 1.0 ],
									"patching_rect" : [ 348.0, 132.0, 300.0, 276.0 ],
									"id" : "obj-31",
									"rounded" : 20,
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "panel",
									"numinlets" : 1,
									"bgcolor" : [ 0.368627, 0.415686, 0.466667, 1.0 ],
									"patching_rect" : [ 656.0, 132.0, 312.0, 276.0 ],
									"id" : "obj-37",
									"rounded" : 20,
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "panel",
									"numinlets" : 1,
									"bgcolor" : [ 0.188235, 0.231373, 0.27451, 1.0 ],
									"patching_rect" : [ 4.0, 4.0, 964.0, 124.0 ],
									"id" : "obj-87",
									"rounded" : 20,
									"numoutlets" : 0
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-30", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 1 ],
									"destination" : [ "obj-21", 1 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 1 ],
									"destination" : [ "obj-19", 1 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-46", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-36", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-35", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-22", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 1 ],
									"destination" : [ "obj-22", 1 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 1 ],
									"destination" : [ "obj-10", 1 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-40", 0 ],
									"destination" : [ "obj-39", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-42", 0 ],
									"destination" : [ "obj-40", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-39", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-42", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 0,
									"midpoints" : [ 21.5, 164.5, 89.5, 164.5 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-62", 0 ],
									"destination" : [ "obj-53", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-53", 0 ],
									"destination" : [ "obj-67", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-60", 0 ],
									"destination" : [ "obj-67", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-74", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 0 ],
									"destination" : [ "obj-79", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-79", 1 ],
									"destination" : [ "obj-78", 1 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-79", 0 ],
									"destination" : [ "obj-77", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-77", 0 ],
									"destination" : [ "obj-78", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-78", 0 ],
									"destination" : [ "obj-36", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r rbfi",
					"numinlets" : 0,
					"bgcolor" : [ 0.380392, 0.447059, 0.501961, 1.0 ],
					"patching_rect" : [ 20.0, 20.0, 51.0, 30.0 ],
					"id" : "obj-45",
					"fontname" : "Arial",
					"color" : [ 0.85098, 0.85098, 0.85098, 1.0 ],
					"textcolor" : [ 0.964706, 0.94902, 0.945098, 1.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontsize" : 20.516645
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p catch_weights",
					"numinlets" : 1,
					"bgcolor" : [ 0.380392, 0.447059, 0.501961, 1.0 ],
					"patching_rect" : [ 536.0, 384.0, 115.0, 23.0 ],
					"id" : "obj-47",
					"fontname" : "Arial",
					"color" : [ 0.85098, 0.85098, 0.85098, 1.0 ],
					"textcolor" : [ 0.964706, 0.94902, 0.945098, 1.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"fontsize" : 14.0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 584.0, 300.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 584.0, 300.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 10.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 4.0, 4.0 ],
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
									"text" : "zl rev",
									"numinlets" : 2,
									"patching_rect" : [ 52.0, 220.0, 39.0, 20.0 ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "< we slice off the name so that the item can be used for multislider viewing, but the name can also be routed.  see \"p routing\" for more information on this.",
									"linecount" : 4,
									"numinlets" : 1,
									"patching_rect" : [ 172.0, 108.0, 187.0, 52.0 ],
									"id" : "obj-9",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "< \"done\" sends the list out",
									"numinlets" : 1,
									"patching_rect" : [ 136.0, 184.0, 150.0, 18.0 ],
									"id" : "obj-8",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "< the weights come out sequentially in pairs of <name> <weight>",
									"linecount" : 2,
									"numinlets" : 1,
									"patching_rect" : [ 128.0, 68.0, 187.0, 29.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"numoutlets" : 0,
									"fontsize" : 10.0,
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl 1024 group",
									"numinlets" : 2,
									"patching_rect" : [ 312.0, 192.0, 83.0, 20.0 ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"patching_rect" : [ 312.0, 260.0, 25.0, 25.0 ],
									"id" : "obj-2",
									"numoutlets" : 0,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "route done",
									"numinlets" : 1,
									"patching_rect" : [ 52.0, 68.0, 68.0, 20.0 ],
									"id" : "obj-24",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl 1024 group",
									"numinlets" : 2,
									"patching_rect" : [ 52.0, 180.0, 83.0, 20.0 ],
									"id" : "obj-23",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl slice 1",
									"numinlets" : 2,
									"patching_rect" : [ 100.0, 108.0, 57.0, 20.0 ],
									"id" : "obj-22",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontsize" : 12.0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 52.0, 32.0, 25.0, 25.0 ],
									"id" : "obj-45",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"patching_rect" : [ 52.0, 256.0, 25.0, 25.0 ],
									"id" : "obj-46",
									"numoutlets" : 0,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 1 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 1 ],
									"destination" : [ "obj-22", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-45", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 1 ],
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
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-46", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 10.0,
						"default_fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"patching_rect" : [ 536.0, 444.0, 50.0, 20.0 ],
					"id" : "obj-19",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "zl len",
					"numinlets" : 2,
					"bgcolor" : [ 0.380392, 0.447059, 0.501961, 1.0 ],
					"patching_rect" : [ 536.0, 416.0, 69.0, 20.0 ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"color" : [ 0.85098, 0.85098, 0.85098, 1.0 ],
					"textcolor" : [ 0.964706, 0.94902, 0.945098, 1.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"fontsize" : 12.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "command-shift-click:\n-- delete a point",
					"linecount" : 2,
					"numinlets" : 1,
					"patching_rect" : [ 536.0, 184.0, 207.0, 34.0 ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "option-shift-click: \n-- create new points",
					"linecount" : 2,
					"numinlets" : 1,
					"patching_rect" : [ 536.0, 148.0, 216.0, 34.0 ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"numinlets" : 1,
					"patching_rect" : [ 604.0, 460.0, 232.0, 136.0 ],
					"id" : "obj-25",
					"size" : 6,
					"numoutlets" : 2,
					"setminmax" : [ 0.0, 1.0 ],
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "rbfi",
					"numinlets" : 1,
					"line_width" : 0.0,
					"patching_rect" : [ 20.0, 116.0, 500.0, 500.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"textcolor" : [  ],
					"numoutlets" : 5,
					"always_draw_labels" : 1,
					"outlettype" : [ "", "", "", "", "" ],
					"rbf" : 0,
					"fontsize" : 12.0,
					"spaces" : [ "space", 6, 0.935253, 0.791972, "/p0", 0.75, 0.375, 0.1875, 0.0, 3.321928, 440409.0, 0.05, 0.2, 0, 0.671433, 0.769042, "/p1", 0.75, 0.5625, 0.1875, 0.0, 3.321928, 440409.0, 0.05, 0.2, 0, 0.290664, 0.187917, "/p2", 0.75, 0.75, 0.1875, 0.0, 3.321928, 440409.0, 0.05, 0.2, 0, 0.315545, 0.356532, "/p3", 0.5625, 0.75, 0.1875, 0.0, 3.321928, 440409.0, 0.05, 0.2, 0, 0.227081, 0.557692, "/p4", 0.375, 0.75, 0.1875, 0.0, 3.321928, 440409.0, 0.05, 0.2, 0, 0.131698, 0.43994, "/p5", 0.1875, 0.75, 0.1875, 0.0, 3.321928, 440409.0, 0.05, 0.2, 0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"bgcolor" : [ 0.34902, 0.498039, 0.52549, 1.0 ],
					"patching_rect" : [ 532.0, 116.0, 240.0, 248.0 ],
					"id" : "obj-49",
					"rounded" : 15,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"bgcolor" : [ 0.192157, 0.34902, 0.333333, 1.0 ],
					"patching_rect" : [ 784.0, 116.0, 336.0, 228.0 ],
					"id" : "obj-58",
					"rounded" : 15,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "John MacCallum ©2010, UC Regents. All Rights Reserved",
					"numinlets" : 1,
					"patching_rect" : [ 864.0, 640.0, 244.0, 17.0 ],
					"id" : "obj-63",
					"fontname" : "Arial",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"frgb" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "http://www.cnmat.berkeley.edu/downloads/",
					"numinlets" : 1,
					"patching_rect" : [ 920.0, 624.0, 180.0, 17.0 ],
					"id" : "obj-62",
					"fontname" : "Arial",
					"textcolor" : [ 0.631373, 0.631373, 0.890196, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"frgb" : [ 0.631373, 0.631373, 0.890196, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numinlets" : 1,
					"bgcolor" : [ 0.145098, 0.192157, 0.2, 1.0 ],
					"patching_rect" : [ 8.0, 4.0, 1132.0, 660.0 ],
					"id" : "obj-60",
					"rounded" : 30,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numinlets" : 1,
					"patching_rect" : [ 920.0, 624.0, 180.289139, 16.495062 ],
					"id" : "obj-13",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"handoff" : ""
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"color" : [ 0.768627, 0.866667, 0.894118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 2 ],
					"destination" : [ "obj-54", 0 ],
					"hidden" : 0,
					"color" : [ 0.768627, 0.866667, 0.894118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-44", 0 ],
					"hidden" : 0,
					"color" : [ 0.768627, 0.866667, 0.894118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-74", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [ 545.5, 106.0, 29.5, 106.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-73", 0 ],
					"destination" : [ "obj-74", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-78", 0 ],
					"destination" : [ "obj-73", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 3 ],
					"destination" : [ "obj-69", 0 ],
					"hidden" : 0,
					"color" : [ 0.768627, 0.866667, 0.894118, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 1 ],
					"destination" : [ "obj-67", 0 ],
					"hidden" : 0,
					"color" : [ 0.360784, 0.686275, 0.368627, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 1 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 4 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"color" : [ 0.490196, 0.745098, 0.54902, 1.0 ],
					"midpoints" : [ 510.5, 617.5, 713.5, 617.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"midpoints" : [ 29.5, 626.0, 530.5, 626.0, 530.5, 378.0, 545.5, 378.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"color" : [ 0.882353, 0.92549, 0.941176, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"color" : [ 0.796078, 0.890196, 0.917647, 1.0 ],
					"midpoints" : [ 545.5, 411.0, 613.5, 411.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"color" : [ 0.882353, 0.92549, 0.941176, 1.0 ],
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"color" : [ 0.92549, 0.980392, 1.0, 1.0 ],
					"midpoints" : [  ]
				}

			}
 ]
	}

}
