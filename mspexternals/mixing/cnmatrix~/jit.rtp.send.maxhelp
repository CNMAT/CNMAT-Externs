{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 74.0, 44.0, 906.0, 690.0 ],
		"bglocked" : 0,
		"defrect" : [ 74.0, 44.0, 906.0, 690.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "meter~",
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"id" : "obj-30",
					"patching_rect" : [ 680.0, 510.0, 14.0, 75.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 1",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"hidden" : 1,
					"patching_rect" : [ 495.0, 365.0, 72.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-28",
					"fontname" : "Arial",
					"hidden" : 1,
					"patching_rect" : [ 435.0, 155.0, 60.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "3 0 1, 4 1 1, 1 1 1",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-27",
					"fontname" : "Arial",
					"hidden" : 1,
					"patching_rect" : [ 435.0, 180.0, 104.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "select between the sources",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-26",
					"fontname" : "MS Reference Sans Serif",
					"patching_rect" : [ 475.0, 390.0, 99.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 1",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"hidden" : 1,
					"patching_rect" : [ 20.0, 15.0, 72.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-39",
					"handoff" : "",
					"patching_rect" : [ 65.0, 610.0, 224.0, 28.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "http://www.cnmat.berkeley.edu/MAX/",
					"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"numoutlets" : 0,
					"id" : "obj-40",
					"fontname" : "Arial",
					"frgb" : [ 0.4, 0.4, 0.8, 1.0 ],
					"patching_rect" : [ 136.0, 620.0, 202.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\rmax launch_browser http://www.cnmat.berkeley.edu/MAX/",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"hidden" : 1,
					"patching_rect" : [ 68.0, 644.0, 300.0, 25.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "fpic",
					"numoutlets" : 0,
					"id" : "obj-47",
					"embed" : 1,
					"patching_rect" : [ 70.0, 555.0, 109.0, 48.0 ],
					"numinlets" : 1,
					"data" : [ 593, "", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "cnmatrix~ and all other CNMAT Max objects, can be found at:",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-48",
					"fontname" : "Arial",
					"patching_rect" : [ 68.0, 609.0, 197.0, 27.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "cnmatrix~.maxhelp version 0.1 by Yotam Mann & John MacCallum",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-50",
					"fontname" : "Arial",
					"patching_rect" : [ 180.0, 578.0, 149.0, 27.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Signal Routing with Jitter Matrices",
					"textcolor" : [ 1.0, 0.974289, 0.97435, 1.0 ],
					"numoutlets" : 0,
					"id" : "obj-55",
					"fontname" : "Orator Std Medium",
					"frgb" : [ 1.0, 0.974289, 0.97435, 1.0 ],
					"patching_rect" : [ 304.0, 66.0, 288.0, 22.0 ],
					"fontsize" : 13.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "cnmatrix~",
					"numoutlets" : 0,
					"id" : "obj-38",
					"fontname" : "Orator Std Slanted",
					"patching_rect" : [ 338.0, 30.0, 228.0, 50.0 ],
					"fontsize" : 36.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-53",
					"bordercolor" : [ 1.0, 0.64, 0.0, 1.0 ],
					"bgcolor" : [ 0.466667, 0.643137, 0.901961, 1.0 ],
					"patching_rect" : [ 280.0, 30.0, 320.0, 60.0 ],
					"numinlets" : 1,
					"shadow" : 30,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Input:",
					"numoutlets" : 0,
					"id" : "obj-21",
					"fontname" : "Arial",
					"patching_rect" : [ 515.0, 150.0, 42.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "5",
					"numoutlets" : 0,
					"id" : "obj-19",
					"fontname" : "Arial",
					"patching_rect" : [ 665.0, 150.0, 31.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "4",
					"numoutlets" : 0,
					"id" : "obj-17",
					"fontname" : "Arial",
					"patching_rect" : [ 640.0, 150.0, 18.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "3",
					"numoutlets" : 0,
					"id" : "obj-14",
					"fontname" : "Arial",
					"patching_rect" : [ 610.0, 150.0, 18.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "2",
					"numoutlets" : 0,
					"id" : "obj-13",
					"fontname" : "Arial",
					"patching_rect" : [ 580.0, 150.0, 18.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "1",
					"numoutlets" : 0,
					"id" : "obj-12",
					"fontname" : "Arial",
					"patching_rect" : [ 550.0, 150.0, 18.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Right",
					"numoutlets" : 0,
					"id" : "obj-11",
					"fontname" : "Arial",
					"patching_rect" : [ 690.0, 200.0, 43.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Left",
					"numoutlets" : 0,
					"id" : "obj-10",
					"fontname" : "Arial",
					"patching_rect" : [ 690.0, 170.0, 34.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "receive~ R3",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"patching_rect" : [ 717.0, 424.0, 75.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "receive~ L3",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"patching_rect" : [ 610.0, 425.0, 73.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "receive~ R2",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"patching_rect" : [ 697.0, 399.0, 75.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "receive~ L2",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"patching_rect" : [ 590.0, 400.0, 73.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "receive~ R1",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"patching_rect" : [ 677.0, 374.0, 75.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "receive~ L1",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"patching_rect" : [ 570.0, 375.0, 73.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "matrixctrl",
					"numoutlets" : 2,
					"rows" : 2,
					"columns" : 5,
					"outlettype" : [ "list", "list" ],
					"id" : "obj-134",
					"patching_rect" : [ 545.0, 170.0, 145.0, 52.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p cycles",
					"numoutlets" : 5,
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal" ],
					"id" : "obj-133",
					"fontname" : "Arial",
					"patching_rect" : [ 565.0, 235.0, 129.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 265.0, 354.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 265.0, 354.0 ],
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
									"maxclass" : "newobj",
									"text" : "cycle~ 550",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-116",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 160.0, 200.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 110",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-117",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 50.0, 100.0, 67.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 220",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-119",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 75.0, 125.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 440",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-120",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 135.0, 175.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 330",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-121",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 105.0, 150.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-124",
									"patching_rect" : [ 50.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-125",
									"patching_rect" : [ 75.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-126",
									"patching_rect" : [ 105.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-127",
									"patching_rect" : [ 135.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-128",
									"patching_rect" : [ 160.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-119", 0 ],
									"destination" : [ "obj-125", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-121", 0 ],
									"destination" : [ "obj-126", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-120", 0 ],
									"destination" : [ "obj-127", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-116", 0 ],
									"destination" : [ "obj-128", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-117", 0 ],
									"destination" : [ "obj-124", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 12.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 12.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "send~ L3",
					"numoutlets" : 0,
					"id" : "obj-132",
					"fontname" : "Arial",
					"patching_rect" : [ 565.0, 305.0, 60.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Change the time in milliseconds it takes to interpolate in smooth mode",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-131",
					"fontname" : "MS Reference Sans Serif",
					"patching_rect" : [ 180.0, 180.0, 158.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p cycles",
					"numoutlets" : 5,
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal" ],
					"id" : "obj-130",
					"fontname" : "Arial",
					"patching_rect" : [ 180.0, 245.0, 129.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 263.0, 339.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 263.0, 339.0 ],
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
									"maxclass" : "newobj",
									"text" : "cycle~ 550",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-116",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 160.0, 200.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 110",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-117",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 50.0, 100.0, 67.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 220",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-119",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 75.0, 125.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 440",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-120",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 135.0, 175.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 330",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-121",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 105.0, 150.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-124",
									"patching_rect" : [ 50.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-125",
									"patching_rect" : [ 75.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-126",
									"patching_rect" : [ 105.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-127",
									"patching_rect" : [ 135.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-128",
									"patching_rect" : [ 160.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-116", 0 ],
									"destination" : [ "obj-128", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-120", 0 ],
									"destination" : [ "obj-127", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-121", 0 ],
									"destination" : [ "obj-126", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-119", 0 ],
									"destination" : [ "obj-125", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-117", 0 ],
									"destination" : [ "obj-124", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 12.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 12.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p cycles",
					"numoutlets" : 5,
					"outlettype" : [ "signal", "signal", "signal", "signal", "signal" ],
					"id" : "obj-129",
					"fontname" : "Arial",
					"patching_rect" : [ 55.0, 380.0, 130.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 264.0, 357.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 264.0, 357.0 ],
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
									"maxclass" : "newobj",
									"text" : "cycle~ 550",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-116",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 160.0, 200.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 110",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-117",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 50.0, 100.0, 67.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 220",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-119",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 75.0, 125.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 440",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-120",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 135.0, 175.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 330",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"id" : "obj-121",
									"fontname" : "MS Reference Sans Serif",
									"patching_rect" : [ 105.0, 150.0, 68.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-124",
									"patching_rect" : [ 50.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-125",
									"patching_rect" : [ 75.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-126",
									"patching_rect" : [ 105.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-127",
									"patching_rect" : [ 135.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numoutlets" : 0,
									"id" : "obj-128",
									"patching_rect" : [ 160.0, 280.0, 25.0, 25.0 ],
									"numinlets" : 1,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-117", 0 ],
									"destination" : [ "obj-124", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-119", 0 ],
									"destination" : [ "obj-125", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-121", 0 ],
									"destination" : [ "obj-126", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-120", 0 ],
									"destination" : [ "obj-127", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-116", 0 ],
									"destination" : [ "obj-128", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 12.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"fontsize" : 12.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "send~ R2",
					"numoutlets" : 0,
					"id" : "obj-122",
					"fontname" : "Arial",
					"patching_rect" : [ 165.0, 480.0, 62.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cnmatrix~ 5 2 frame",
					"numoutlets" : 2,
					"color" : [ 0.466667, 0.643137, 0.901961, 1.0 ],
					"outlettype" : [ "signal", "signal" ],
					"id" : "obj-118",
					"fontname" : "MS Reference Sans Serif",
					"patching_rect" : [ 55.0, 430.0, 130.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 5
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "send~ R3",
					"numoutlets" : 0,
					"id" : "obj-114",
					"fontname" : "Arial",
					"patching_rect" : [ 675.0, 305.0, 62.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "send~ R1",
					"numoutlets" : 0,
					"id" : "obj-113",
					"fontname" : "Arial",
					"patching_rect" : [ 290.0, 340.0, 62.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "send~ L1",
					"numoutlets" : 0,
					"id" : "obj-112",
					"fontname" : "Arial",
					"patching_rect" : [ 180.0, 325.0, 60.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "send~ L2",
					"numoutlets" : 0,
					"id" : "obj-111",
					"fontname" : "Arial",
					"patching_rect" : [ 55.0, 460.0, 60.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cnmatrix~ 5 2 fast",
					"numoutlets" : 2,
					"color" : [ 0.466667, 0.643137, 0.901961, 1.0 ],
					"outlettype" : [ "signal", "signal" ],
					"id" : "obj-107",
					"fontname" : "MS Reference Sans Serif",
					"patching_rect" : [ 565.0, 270.0, 129.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 5
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-101",
					"fontname" : "Arial",
					"hidden" : 1,
					"patching_rect" : [ 495.0, 455.0, 60.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "100",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-100",
					"fontname" : "Arial",
					"hidden" : 1,
					"patching_rect" : [ 493.0, 476.0, 32.5, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-99",
					"fontname" : "Arial",
					"patching_rect" : [ 125.0, 185.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 1000",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-96",
					"fontname" : "Arial",
					"hidden" : 1,
					"patching_rect" : [ 125.0, 155.0, 92.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-95",
					"fontname" : "Arial",
					"minimum" : 0,
					"maximum" : 3,
					"patching_rect" : [ 495.0, 420.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "selector~ 3",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-93",
					"fontname" : "Arial",
					"patching_rect" : [ 660.0, 460.0, 75.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 4
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "selector~ 3",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"id" : "obj-92",
					"fontname" : "Arial",
					"patching_rect" : [ 553.0, 461.0, 74.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 4
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "stop",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-86",
					"fontname" : "Arial",
					"patching_rect" : [ 810.0, 576.0, 27.0, 15.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "startwindow",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-87",
					"fontname" : "Arial",
					"patching_rect" : [ 738.0, 576.0, 65.0, 15.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• start audio",
					"numoutlets" : 0,
					"id" : "obj-88",
					"fontname" : "Arial",
					"patching_rect" : [ 756.0, 557.0, 75.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"numoutlets" : 0,
					"id" : "obj-89",
					"bordercolor" : [ 0.466667, 0.643137, 0.901961, 1.0 ],
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"border" : 2,
					"patching_rect" : [ 725.0, 555.0, 123.0, 43.0 ],
					"numinlets" : 1,
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "slide $1",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-83",
					"fontname" : "Arial",
					"patching_rect" : [ 125.0, 215.0, 52.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess 0.5",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-66",
					"fontname" : "Arial",
					"hidden" : 1,
					"patching_rect" : [ 155.0, 62.0, 82.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"triscale" : 0.9,
					"id" : "obj-64",
					"fontname" : "Arial",
					"patching_rect" : [ 155.0, 89.0, 50.0, 20.0 ],
					"fontsize" : 11.595187,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "amp $1",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-18",
					"fontname" : "Arial",
					"patching_rect" : [ 155.0, 115.0, 47.0, 18.0 ],
					"fontsize" : 11.595187,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.normalize",
					"numoutlets" : 2,
					"outlettype" : [ "jit_matrix", "" ],
					"id" : "obj-65",
					"fontname" : "Arial",
					"patching_rect" : [ 20.0, 160.0, 72.0, 20.0 ],
					"fontsize" : 11.595187,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "jit.noise 1 float32 5 2",
					"numoutlets" : 2,
					"outlettype" : [ "jit_matrix", "" ],
					"id" : "obj-62",
					"fontname" : "Arial",
					"patching_rect" : [ 20.0, 115.0, 121.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"id" : "obj-56",
					"patching_rect" : [ 20.0, 45.0, 20.0, 20.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "qmetro 500",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-54",
					"fontname" : "Arial",
					"patching_rect" : [ 20.0, 80.0, 71.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "\"frame\" does linear interpolation over one signal vector.  Use for equal power panning.",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-45",
					"fontname" : "MS Reference Sans Serif",
					"patching_rect" : [ 190.0, 425.0, 186.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "\"fast\" is no interpolation at all.  Audible clicks may result.  Use for static routing only.",
					"linecount" : 4,
					"numoutlets" : 0,
					"id" : "obj-43",
					"fontname" : "MS Reference Sans Serif",
					"patching_rect" : [ 700.0, 240.0, 137.0, 62.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "\"smooth\" is default, uses the same formula as slide~.  Use this for an audio mixer.",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-42",
					"fontname" : "MS Reference Sans Serif",
					"patching_rect" : [ 315.0, 285.0, 167.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cnmatrix~ 5 2 smooth",
					"numoutlets" : 2,
					"color" : [ 0.466667, 0.643137, 0.901961, 1.0 ],
					"outlettype" : [ "signal", "signal" ],
					"id" : "obj-37",
					"fontname" : "MS Reference Sans Serif",
					"patching_rect" : [ 180.0, 295.0, 129.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 5
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The cnmatrix~ obj is just like matrix~ except it takes jitter matrices as input to set the gains.  The rows of the matrix represent the outputs, and the columns are the inputs (same as matrixctl).  ",
					"linecount" : 4,
					"numoutlets" : 0,
					"id" : "obj-8",
					"fontname" : "MS Reference Sans Serif",
					"patching_rect" : [ 605.0, 30.0, 271.0, 62.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "meter~",
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"id" : "obj-20",
					"patching_rect" : [ 570.0, 510.0, 14.0, 75.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dac~ 1 2",
					"numoutlets" : 0,
					"id" : "obj-7",
					"fontname" : "MS Reference Sans Serif",
					"patching_rect" : [ 550.0, 630.0, 130.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"orientation" : 2,
					"interp" : 100.0,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "int" ],
					"id" : "obj-15",
					"patching_rect" : [ 665.0, 510.0, 15.0, 74.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"orientation" : 2,
					"interp" : 100.0,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "int" ],
					"id" : "obj-16",
					"patching_rect" : [ 555.0, 510.0, 15.0, 74.0 ],
					"numinlets" : 2
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-134", 0 ],
					"destination" : [ "obj-107", 0 ],
					"hidden" : 0,
					"midpoints" : [ 554.5, 265.0, 574.5, 265.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-133", 4 ],
					"destination" : [ "obj-107", 4 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-133", 3 ],
					"destination" : [ "obj-107", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-133", 2 ],
					"destination" : [ "obj-107", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-133", 1 ],
					"destination" : [ "obj-107", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-133", 0 ],
					"destination" : [ "obj-107", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-107", 0 ],
					"destination" : [ "obj-132", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-107", 1 ],
					"destination" : [ "obj-114", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-65", 0 ],
					"destination" : [ "obj-118", 0 ],
					"hidden" : 0,
					"midpoints" : [ 29.5, 417.0, 64.5, 417.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-83", 0 ],
					"destination" : [ "obj-37", 0 ],
					"hidden" : 0,
					"midpoints" : [ 134.5, 282.0, 189.5, 282.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-130", 4 ],
					"destination" : [ "obj-37", 4 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-130", 3 ],
					"destination" : [ "obj-37", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-130", 2 ],
					"destination" : [ "obj-37", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-130", 1 ],
					"destination" : [ "obj-37", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-130", 0 ],
					"destination" : [ "obj-37", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-129", 0 ],
					"destination" : [ "obj-118", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-129", 1 ],
					"destination" : [ "obj-118", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-129", 2 ],
					"destination" : [ "obj-118", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-129", 3 ],
					"destination" : [ "obj-118", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-129", 4 ],
					"destination" : [ "obj-118", 4 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-65", 0 ],
					"destination" : [ "obj-37", 0 ],
					"hidden" : 0,
					"midpoints" : [ 29.5, 282.0, 189.5, 282.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-118", 1 ],
					"destination" : [ "obj-122", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-118", 0 ],
					"destination" : [ "obj-111", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 1 ],
					"destination" : [ "obj-113", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-112", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-101", 0 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-100", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-100", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 0 ],
					"destination" : [ "obj-99", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-99", 0 ],
					"destination" : [ "obj-83", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-92", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-93", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-86", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 819.5, 618.0, 559.5, 618.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-87", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 747.5, 618.0, 559.5, 618.0 ]
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
					"source" : [ "obj-54", 0 ],
					"destination" : [ "obj-62", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-64", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 0 ],
					"destination" : [ "obj-65", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-65", 0 ],
					"hidden" : 0,
					"midpoints" : [ 164.5, 149.0, 29.5, 149.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-64", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-92", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-93", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-92", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-93", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-92", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-93", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 1 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-56", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-134", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-95", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-95", 0 ],
					"destination" : [ "obj-92", 0 ],
					"hidden" : 0,
					"midpoints" : [ 504.5, 450.0, 562.5, 450.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-95", 0 ],
					"destination" : [ "obj-93", 0 ],
					"hidden" : 0,
					"midpoints" : [ 504.5, 449.5, 669.5, 449.5 ]
				}

			}
 ]
	}

}
