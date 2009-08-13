{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 101.0, 44.0, 1303.0, 841.0 ],
		"bglocked" : 0,
		"defrect" : [ 101.0, 44.0, 1303.0, 841.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "fade 1",
					"id" : "obj-33",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 720.0, 159.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 720.0, 165.0, 44.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "fade 20",
					"id" : "obj-14",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 234.0, 607.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 660.0, 135.0, 51.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc 40",
					"id" : "obj-17",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 735.0, 240.0, 55.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc 100",
					"id" : "obj-10",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 60.0, 150.0, 62.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"id" : "obj-73",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 300.0, 195.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "log $1",
					"id" : "obj-68",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 300.0, 225.0, 96.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"id" : "obj-95",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 555.0, 30.0, 41.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll b3",
					"id" : "obj-96",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 555.0, 60.0, 59.5, 20.0 ],
					"coll_data" : 					{
						"count" : 55,
						"data" : [ 							{
								"key" : 2,
								"value" : [ 44.428703, 0.000948, 2.263747 ]
							}
, 							{
								"key" : 3,
								"value" : [ 105.105789, 0.005561, 8.283034 ]
							}
, 							{
								"key" : 4,
								"value" : [ 105.987946, 0.004728, 0.027602 ]
							}
, 							{
								"key" : 5,
								"value" : [ 301.025726, 0.004145, 0.537605 ]
							}
, 							{
								"key" : 6,
								"value" : [ 301.322327, 0.004067, 0.063896 ]
							}
, 							{
								"key" : 7,
								"value" : [ 343.628357, 0.001207, 5.041814 ]
							}
, 							{
								"key" : 8,
								"value" : [ 406.502655, 0.005348, 4.100026 ]
							}
, 							{
								"key" : 9,
								"value" : [ 522.951904, 0.005215, 9.696371 ]
							}
, 							{
								"key" : 10,
								"value" : [ 557.567505, 0.015346, 0.050048 ]
							}
, 							{
								"key" : 11,
								"value" : [ 664.45575, 0.001737, 2.906578 ]
							}
, 							{
								"key" : 12,
								"value" : [ 703.368286, 0.015189, 11.94272 ]
							}
, 							{
								"key" : 13,
								"value" : [ 733.67865, 0.002115, 6.350811 ]
							}
, 							{
								"key" : 14,
								"value" : [ 785.913269, 0.012306, 9.968545 ]
							}
, 							{
								"key" : 15,
								"value" : [ 871.747803, 0.009862, 4.749314 ]
							}
, 							{
								"key" : 16,
								"value" : [ 875.094177, 0.005993, 0.066342 ]
							}
, 							{
								"key" : 17,
								"value" : [ 879.910522, 0.005887, 0.063846 ]
							}
, 							{
								"key" : 18,
								"value" : [ 1012.840332, 0.012604, 10.789166 ]
							}
, 							{
								"key" : 19,
								"value" : [ 1048.988525, 0.007745, 8.985867 ]
							}
, 							{
								"key" : 20,
								"value" : [ 1115.045166, 0.007925, 0.271933 ]
							}
, 							{
								"key" : 21,
								"value" : [ 1175.845703, 0.005228, 0.140735 ]
							}
, 							{
								"key" : 22,
								"value" : [ 1240.114258, 0.003412, 0.414345 ]
							}
, 							{
								"key" : 23,
								"value" : [ 1313.572632, 0.020711, 11.370027 ]
							}
, 							{
								"key" : 24,
								"value" : [ 1347.044556, 0.003522, 4.318935 ]
							}
, 							{
								"key" : 25,
								"value" : [ 1435.089844, 0.004455, 3.906166 ]
							}
, 							{
								"key" : 26,
								"value" : [ 1679.655151, 0.006915, 0.208798 ]
							}
, 							{
								"key" : 27,
								"value" : [ 1750.221924, 0.001689, 0.283003 ]
							}
, 							{
								"key" : 28,
								"value" : [ 1797.569946, 0.001088, 0.192999 ]
							}
, 							{
								"key" : 29,
								"value" : [ 1917.950317, 0.002683, 6.9455 ]
							}
, 							{
								"key" : 30,
								"value" : [ 1943.572876, 0.010738, 9.73517 ]
							}
, 							{
								"key" : 31,
								"value" : [ 1968.741821, 0.00379, 5.022655 ]
							}
, 							{
								"key" : 32,
								"value" : [ 1982.093018, 0.010626, 10.998216 ]
							}
, 							{
								"key" : 78,
								"value" : [ 5040.164062, 0.001339, 2.630528 ]
							}
, 							{
								"key" : 79,
								"value" : [ 5191.727051, 0.002485, 6.57367 ]
							}
, 							{
								"key" : 80,
								"value" : [ 5235.740234, 0.001259, 2.565066 ]
							}
, 							{
								"key" : 81,
								"value" : [ 5268.115234, 0.002975, 0.101447 ]
							}
, 							{
								"key" : 82,
								"value" : [ 5300.748047, 0.016295, 5.282162 ]
							}
, 							{
								"key" : 83,
								"value" : [ 5321.609375, 0.002972, 3.476706 ]
							}
, 							{
								"key" : 84,
								"value" : [ 5374.025391, 0.012623, 0.387105 ]
							}
, 							{
								"key" : 85,
								"value" : [ 5404.853027, 0.004186, 3.840042 ]
							}
, 							{
								"key" : 86,
								"value" : [ 5478.959473, 0.001848, 2.972434 ]
							}
, 							{
								"key" : 87,
								"value" : [ 5549.34082, 0.002334, 6.440713 ]
							}
, 							{
								"key" : 88,
								"value" : [ 5611.518066, 0.002923, 6.918336 ]
							}
, 							{
								"key" : 89,
								"value" : [ 5809.298828, 0.007148, 5.917635 ]
							}
, 							{
								"key" : 90,
								"value" : [ 5847.330078, 0.005419, 4.114071 ]
							}
, 							{
								"key" : 91,
								"value" : [ 5931.84375, 0.002032, 3.073464 ]
							}
, 							{
								"key" : 92,
								"value" : [ 5986.87793, 0.000938, 2.252904 ]
							}
, 							{
								"key" : 93,
								"value" : [ 6068.418457, 0.005205, 4.071298 ]
							}
, 							{
								"key" : 94,
								"value" : [ 6210.710938, 0.002709, 6.76382 ]
							}
, 							{
								"key" : 95,
								"value" : [ 6287.410156, 0.000862, 4.326991 ]
							}
, 							{
								"key" : 96,
								"value" : [ 6465.839844, 0.006192, 8.711309 ]
							}
, 							{
								"key" : 97,
								"value" : [ 6576.297852, 0.001589, 5.62506 ]
							}
, 							{
								"key" : 98,
								"value" : [ 6730.796875, 0.006464, 8.602255 ]
							}
, 							{
								"key" : 99,
								"value" : [ 6985.117188, 0.001644, 5.697485 ]
							}
, 							{
								"key" : 100,
								"value" : [ 7018.568359, 0.001112, 4.866942 ]
							}
, 							{
								"key" : 101,
								"value" : [ 7568.255371, 0.001316, 5.224373 ]
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
					"maxclass" : "comment",
					"text" : "centroid",
					"id" : "obj-93",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 450.0, 495.0, 150.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"id" : "obj-94",
					"numinlets" : 1,
					"orientation" : 0,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"setminmax" : [ 0.0, 8000.0 ],
					"patching_rect" : [ 300.0, 480.0, 300.0, 16.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p interp_tester",
					"id" : "obj-92",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1095.0, 690.0, 87.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 0.0, 0.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 0.0, 0.0, 640.0, 480.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "flonum",
									"id" : "obj-78",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 2,
									"outlettype" : [ "float", "bang" ],
									"fontname" : "Arial",
									"patching_rect" : [ 110.0, 160.0, 183.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"id" : "obj-76",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 2,
									"outlettype" : [ "float", "bang" ],
									"fontname" : "Arial",
									"patching_rect" : [ 200.0, 100.0, 233.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "interp $1",
									"id" : "obj-74",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 200.0, 130.0, 57.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "route interp",
									"id" : "obj-73",
									"numinlets" : 1,
									"fontsize" : 9.0,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 50.0, 160.0, 56.0, 17.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "multislider",
									"id" : "obj-68",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"setminmax" : [ 0.0, 1.0 ],
									"setstyle" : 2,
									"patching_rect" : [ 50.0, 190.0, 320.0, 152.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"id" : "obj-90",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 50.0, 40.0, 25.0, 25.0 ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"id" : "obj-91",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 200.0, 402.0, 25.0, 25.0 ],
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-73", 0 ],
									"destination" : [ "obj-68", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 0 ],
									"destination" : [ "obj-74", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-73", 0 ],
									"destination" : [ "obj-78", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-90", 0 ],
									"destination" : [ "obj-73", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-74", 0 ],
									"destination" : [ "obj-91", 0 ],
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
						"globalpatchername" : "",
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 12.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "centroid",
					"id" : "obj-88",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 1080.0, 495.0, 150.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
					"id" : "obj-86",
					"numinlets" : 1,
					"orientation" : 0,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"setminmax" : [ 0.0, 8000.0 ],
					"patching_rect" : [ 930.0, 480.0, 300.0, 16.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-82",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 435.0, 330.0, 98.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "tellmeeverything",
					"id" : "obj-72",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 180.0, 300.0, 98.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"id" : "obj-71",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 945.0, 705.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p",
					"id" : "obj-70",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 990.0, 735.0, 36.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 70.0, 44.0, 1370.0, 765.0 ],
						"bglocked" : 0,
						"defrect" : [ 70.0, 44.0, 1370.0, 765.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "inlet",
									"id" : "obj-5",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 291.0, 535.0, 25.0, 25.0 ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadbang",
									"id" : "obj-2",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"fontname" : "Arial",
									"patching_rect" : [ 328.0, 8.0, 60.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ftom 0.",
									"id" : "obj-30",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 212.0, 168.0, 47.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ftom 0.",
									"id" : "obj-29",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 156.0, 168.0, 47.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "sel 0",
									"id" : "obj-28",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 132.0, 104.0, 36.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl slice 1",
									"id" : "obj-27",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 132.0, 76.0, 57.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t l l",
									"id" : "obj-26",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 116.0, 48.0, 32.5, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "prepend tri",
									"id" : "obj-24",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 288.0, 344.0, 68.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "+ 0.005",
									"id" : "obj-23",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 152.0, 288.0, 51.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "- 0.005",
									"id" : "obj-22",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 116.0, 288.0, 48.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "- 0.005",
									"id" : "obj-20",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 304.0, 280.0, 48.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0. 0. 0. 0. 0. 0. 0. 0.",
									"id" : "obj-21",
									"numinlets" : 9,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 288.0, 316.0, 156.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t f f f",
									"id" : "obj-15",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 3,
									"outlettype" : [ "float", "float", "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 288.0, 248.0, 46.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t f f f",
									"id" : "obj-14",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 3,
									"outlettype" : [ "float", "float", "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 156.0, 248.0, 46.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "reset",
									"id" : "obj-9",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 72.0, 436.0, 37.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t f f f f f",
									"id" : "obj-4",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 5,
									"outlettype" : [ "float", "float", "float", "float", "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 36.0, 248.0, 73.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 100. -1. 1.",
									"id" : "obj-3",
									"numinlets" : 6,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 36.0, 220.0, 109.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "unpack 0 0. 0.",
									"id" : "obj-1",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 3,
									"outlettype" : [ "int", "float", "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 116.0, 128.0, 86.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "name mig_gl",
									"id" : "obj-47",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 328.0, 32.0, 79.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0.",
									"id" : "obj-44",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 268.0, 436.0, 63.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0.",
									"id" : "obj-43",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 136.0, 436.0, 63.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "lineto $1 $2 0",
									"id" : "obj-37",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 268.0, 468.0, 83.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "moveto $1 $2 0",
									"id" : "obj-36",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 136.0, 468.0, 93.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 127. -1. 1.",
									"id" : "obj-34",
									"numinlets" : 6,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 288.0, 220.0, 109.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 127. -1. 1.",
									"id" : "obj-33",
									"numinlets" : 6,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 156.0, 220.0, 109.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "jit.fpsgui",
									"id" : "obj-52",
									"numinlets" : 1,
									"fontsize" : 11.595187,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 56.0, 660.0, 80.0, 35.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"id" : "obj-120",
									"numinlets" : 1,
									"fontsize" : 11.595187,
									"numoutlets" : 2,
									"outlettype" : [ "int", "bang" ],
									"fontname" : "Arial",
									"patching_rect" : [ 56.0, 572.0, 50.0, 20.0 ],
									"triscale" : 0.9
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"id" : "obj-123",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 12.0, 572.0, 20.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "qmetro 20",
									"id" : "obj-124",
									"numinlets" : 2,
									"fontsize" : 11.595187,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"fontname" : "Arial",
									"patching_rect" : [ 12.0, 598.0, 63.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b erase b",
									"id" : "obj-125",
									"numinlets" : 1,
									"fontsize" : 11.595187,
									"numoutlets" : 3,
									"outlettype" : [ "bang", "erase", "bang" ],
									"fontname" : "Arial",
									"patching_rect" : [ 12.0, 624.0, 63.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "jit.gl.render mig_gl",
									"id" : "obj-127",
									"numinlets" : 1,
									"fontsize" : 11.595187,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 12.0, 700.0, 105.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "jit.gl.sketch mig_gl",
									"id" : "obj-31",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 136.0, 500.0, 109.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p",
									"id" : "obj-25",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "jit_matrix" ],
									"fontname" : "Arial",
									"patching_rect" : [ 12.0, 728.0, 377.0, 20.0 ],
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 0.0, 0.0, 640.0, 480.0 ],
										"bglocked" : 0,
										"defrect" : [ 0.0, 0.0, 640.0, 480.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "/ 2.",
													"id" : "obj-19",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontname" : "Arial",
													"patching_rect" : [ 130.0, 152.0, 32.5, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "clear",
													"id" : "obj-18",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 322.0, 288.0, 37.0, 18.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "zl 2048 len",
													"id" : "obj-16",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 130.0, 128.0, 69.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "scale 0. 100. 0. 256.",
													"id" : "obj-15",
													"numinlets" : 6,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontname" : "Arial",
													"patching_rect" : [ 50.0, 232.0, 119.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "toggle",
													"id" : "obj-14",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"patching_rect" : [ 206.0, 332.0, 20.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "qmetro 1",
													"id" : "obj-12",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"fontname" : "Arial",
													"patching_rect" : [ 206.0, 360.0, 58.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "scale 0. 0.05 0. 1.",
													"id" : "obj-9",
													"numinlets" : 6,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontname" : "Arial",
													"patching_rect" : [ 322.0, 232.0, 105.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "print",
													"id" : "obj-8",
													"numinlets" : 1,
													"fontsize" : 12.0,
													"numoutlets" : 0,
													"fontname" : "Arial",
													"patching_rect" : [ 146.0, 324.0, 34.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "scale 0. 8000. 0. 256.",
													"id" : "obj-7",
													"numinlets" : 6,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontname" : "Arial",
													"patching_rect" : [ 182.0, 232.0, 125.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "unpack 0. 0.",
													"id" : "obj-6",
													"numinlets" : 1,
													"fontsize" : 12.0,
													"numoutlets" : 2,
													"outlettype" : [ "float", "float" ],
													"fontname" : "Arial",
													"patching_rect" : [ 182.0, 204.0, 76.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t 0 l clear l",
													"id" : "obj-5",
													"numinlets" : 1,
													"fontsize" : 12.0,
													"numoutlets" : 4,
													"outlettype" : [ "int", "", "clear", "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 100.0, 65.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "counter",
													"id" : "obj-4",
													"numinlets" : 5,
													"fontsize" : 12.0,
													"numoutlets" : 4,
													"outlettype" : [ "int", "", "", "int" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 184.0, 73.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack 0 0 0.",
													"id" : "obj-3",
													"numinlets" : 3,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 260.0, 69.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "setcell $1 $2 val $3",
													"id" : "obj-2",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 292.0, 113.0, 18.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b l",
													"id" : "obj-65",
													"numinlets" : 1,
													"fontsize" : 12.0,
													"numoutlets" : 2,
													"outlettype" : [ "bang", "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 156.0, 32.5, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "jit.matrix mcmc 1 float32 256 256",
													"id" : "obj-63",
													"numinlets" : 1,
													"fontsize" : 12.0,
													"numoutlets" : 2,
													"outlettype" : [ "jit_matrix", "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 170.0, 390.0, 187.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "zl iter 2",
													"id" : "obj-58",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 128.0, 49.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"id" : "obj-23",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 70.0, 40.0, 25.0, 25.0 ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"id" : "obj-24",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 170.0, 470.0, 25.0, 25.0 ],
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-6", 1 ],
													"destination" : [ "obj-9", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-3", 2 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-7", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-3", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-19", 0 ],
													"destination" : [ "obj-15", 2 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-16", 0 ],
													"destination" : [ "obj-19", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-63", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-18", 0 ],
													"destination" : [ "obj-63", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 3 ],
													"destination" : [ "obj-16", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 0 ],
													"destination" : [ "obj-4", 2 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 1 ],
													"destination" : [ "obj-58", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 2 ],
													"destination" : [ "obj-63", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-15", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-15", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-65", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-65", 1 ],
													"destination" : [ "obj-6", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-58", 0 ],
													"destination" : [ "obj-65", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-2", 0 ],
													"destination" : [ "obj-63", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-2", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-23", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-63", 0 ],
													"destination" : [ "obj-24", 0 ],
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
										"globalpatchername" : "",
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 12.0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "jit.pwindow",
									"id" : "obj-59",
									"numinlets" : 1,
									"srcrect" : [ 0, 0, 256, 256 ],
									"numoutlets" : 2,
									"dstrect" : [ 0, 0, 256, 256 ],
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 468.0, 0.0, 871.0, 767.0 ],
									"name" : "mig_gl"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"id" : "obj-66",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "list" ],
									"patching_rect" : [ 116.0, 18.0, 25.0, 25.0 ],
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-33", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-33", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 2 ],
									"destination" : [ "obj-30", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 1 ],
									"destination" : [ "obj-29", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-66", 0 ],
									"destination" : [ "obj-26", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-26", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 0 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-26", 1 ],
									"destination" : [ "obj-27", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-3", 0 ],
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
									"source" : [ "obj-15", 1 ],
									"destination" : [ "obj-20", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-21", 4 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-21", 7 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-21", 3 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 4 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-21", 6 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 3 ],
									"destination" : [ "obj-22", 0 ],
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
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 2 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-44", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 1 ],
									"destination" : [ "obj-43", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 2 ],
									"destination" : [ "obj-21", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-44", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-43", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-47", 0 ],
									"destination" : [ "obj-59", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-44", 0 ],
									"destination" : [ "obj-37", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-43", 0 ],
									"destination" : [ "obj-36", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-124", 0 ],
									"destination" : [ "obj-125", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-120", 0 ],
									"destination" : [ "obj-124", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-123", 0 ],
									"destination" : [ "obj-124", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-125", 2 ],
									"destination" : [ "obj-52", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-125", 0 ],
									"destination" : [ "obj-127", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-125", 1 ],
									"destination" : [ "obj-127", 0 ],
									"hidden" : 0,
									"midpoints" : [ 43.5, 660.0, 21.5, 660.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-47", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-37", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-36", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-123", 0 ],
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
						"globalpatchername" : "",
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 12.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"id" : "obj-69",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 675.0, 15.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "b 2",
					"id" : "obj-66",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "bang", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 705.0, 15.0, 32.5, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "temperature $1",
					"id" : "obj-40",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 810.0, 301.0, 92.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-65",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 810.0, 270.0, 98.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"id" : "obj-60",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 705.0, 45.0, 32.5, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 1.",
					"id" : "obj-63",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 750.0, 75.0, 32.5, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1000000000",
					"id" : "obj-64",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 765.0, 45.0, 77.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"id" : "obj-59",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1020.0, 30.0, 32.5, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 1.",
					"id" : "obj-58",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1065.0, 60.0, 32.5, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "100000000",
					"id" : "obj-57",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1080.0, 30.0, 71.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "sample_mode 1",
					"id" : "obj-56",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 60.0, 330.0, 96.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p",
					"id" : "obj-67",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"patching_rect" : [ 930.0, 735.0, 36.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 70.0, 44.0, 1370.0, 765.0 ],
						"bglocked" : 0,
						"defrect" : [ 70.0, 44.0, 1370.0, 765.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "inlet",
									"id" : "obj-5",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 276.0, 520.0, 25.0, 25.0 ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadbang",
									"id" : "obj-2",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"fontname" : "Arial",
									"patching_rect" : [ 328.0, 4.0, 60.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ftom 0.",
									"id" : "obj-30",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 212.0, 168.0, 47.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "ftom 0.",
									"id" : "obj-29",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 156.0, 168.0, 47.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "sel 0",
									"id" : "obj-28",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 132.0, 104.0, 36.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl slice 1",
									"id" : "obj-27",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 132.0, 76.0, 57.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t l l",
									"id" : "obj-26",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 116.0, 48.0, 32.5, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "prepend tri",
									"id" : "obj-24",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 288.0, 344.0, 68.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "+ 0.005",
									"id" : "obj-23",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 152.0, 288.0, 51.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "- 0.005",
									"id" : "obj-22",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 116.0, 288.0, 48.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "- 0.005",
									"id" : "obj-20",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 304.0, 280.0, 48.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0. 0. 0. 0. 0. 0. 0. 0.",
									"id" : "obj-21",
									"numinlets" : 9,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 288.0, 316.0, 156.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t f f f",
									"id" : "obj-15",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 3,
									"outlettype" : [ "float", "float", "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 288.0, 248.0, 46.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t f f f",
									"id" : "obj-14",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 3,
									"outlettype" : [ "float", "float", "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 156.0, 248.0, 46.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "reset",
									"id" : "obj-9",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 72.0, 436.0, 37.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t f f f f f",
									"id" : "obj-4",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 5,
									"outlettype" : [ "float", "float", "float", "float", "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 36.0, 248.0, 73.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 100. -1. 1.",
									"id" : "obj-3",
									"numinlets" : 6,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 36.0, 220.0, 109.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "unpack 0 0. 0.",
									"id" : "obj-1",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 3,
									"outlettype" : [ "int", "float", "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 116.0, 128.0, 86.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "name mcmc_gl",
									"id" : "obj-47",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 328.0, 32.0, 92.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0.",
									"id" : "obj-44",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 268.0, 436.0, 63.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0.",
									"id" : "obj-43",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 136.0, 436.0, 63.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "lineto $1 $2 0",
									"id" : "obj-37",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 268.0, 468.0, 83.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "moveto $1 $2 0",
									"id" : "obj-36",
									"numinlets" : 2,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 136.0, 468.0, 93.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 127. -1. 1.",
									"id" : "obj-34",
									"numinlets" : 6,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 288.0, 220.0, 109.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 127. -1. 1.",
									"id" : "obj-33",
									"numinlets" : 6,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"fontname" : "Arial",
									"patching_rect" : [ 156.0, 220.0, 109.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "jit.fpsgui",
									"id" : "obj-52",
									"numinlets" : 1,
									"fontsize" : 11.595187,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 56.0, 660.0, 80.0, 35.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number",
									"id" : "obj-120",
									"numinlets" : 1,
									"fontsize" : 11.595187,
									"numoutlets" : 2,
									"outlettype" : [ "int", "bang" ],
									"fontname" : "Arial",
									"patching_rect" : [ 56.0, 572.0, 50.0, 20.0 ],
									"triscale" : 0.9
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"id" : "obj-123",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 12.0, 572.0, 20.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "qmetro 20",
									"id" : "obj-124",
									"numinlets" : 2,
									"fontsize" : 11.595187,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"fontname" : "Arial",
									"patching_rect" : [ 12.0, 598.0, 63.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b erase b",
									"id" : "obj-125",
									"numinlets" : 1,
									"fontsize" : 11.595187,
									"numoutlets" : 3,
									"outlettype" : [ "bang", "erase", "bang" ],
									"fontname" : "Arial",
									"patching_rect" : [ 12.0, 624.0, 63.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "jit.gl.render mcmc_gl",
									"id" : "obj-127",
									"numinlets" : 1,
									"fontsize" : 11.595187,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 12.0, 700.0, 117.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "jit.gl.sketch mcmc_gl",
									"id" : "obj-31",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"fontname" : "Arial",
									"patching_rect" : [ 136.0, 500.0, 121.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "p",
									"id" : "obj-25",
									"numinlets" : 1,
									"fontsize" : 12.0,
									"numoutlets" : 1,
									"outlettype" : [ "jit_matrix" ],
									"fontname" : "Arial",
									"patching_rect" : [ 12.0, 728.0, 377.0, 20.0 ],
									"patcher" : 									{
										"fileversion" : 1,
										"rect" : [ 0.0, 0.0, 640.0, 480.0 ],
										"bglocked" : 0,
										"defrect" : [ 0.0, 0.0, 640.0, 480.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "/ 2.",
													"id" : "obj-19",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontname" : "Arial",
													"patching_rect" : [ 130.0, 152.0, 32.5, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "clear",
													"id" : "obj-18",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 322.0, 288.0, 37.0, 18.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "zl 2048 len",
													"id" : "obj-16",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 130.0, 128.0, 69.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "scale 0. 100. 0. 256.",
													"id" : "obj-15",
													"numinlets" : 6,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontname" : "Arial",
													"patching_rect" : [ 50.0, 232.0, 119.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "toggle",
													"id" : "obj-14",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"patching_rect" : [ 206.0, 332.0, 20.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "qmetro 1",
													"id" : "obj-12",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"fontname" : "Arial",
													"patching_rect" : [ 206.0, 360.0, 58.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "scale 0. 0.05 0. 1.",
													"id" : "obj-9",
													"numinlets" : 6,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontname" : "Arial",
													"patching_rect" : [ 322.0, 232.0, 105.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "print",
													"id" : "obj-8",
													"numinlets" : 1,
													"fontsize" : 12.0,
													"numoutlets" : 0,
													"fontname" : "Arial",
													"patching_rect" : [ 146.0, 324.0, 34.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "scale 0. 8000. 0. 256.",
													"id" : "obj-7",
													"numinlets" : 6,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"fontname" : "Arial",
													"patching_rect" : [ 182.0, 232.0, 125.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "unpack 0. 0.",
													"id" : "obj-6",
													"numinlets" : 1,
													"fontsize" : 12.0,
													"numoutlets" : 2,
													"outlettype" : [ "float", "float" ],
													"fontname" : "Arial",
													"patching_rect" : [ 182.0, 204.0, 76.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t 0 l clear l",
													"id" : "obj-5",
													"numinlets" : 1,
													"fontsize" : 12.0,
													"numoutlets" : 4,
													"outlettype" : [ "int", "", "clear", "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 100.0, 65.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "counter",
													"id" : "obj-4",
													"numinlets" : 5,
													"fontsize" : 12.0,
													"numoutlets" : 4,
													"outlettype" : [ "int", "", "", "int" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 184.0, 73.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "pack 0 0 0.",
													"id" : "obj-3",
													"numinlets" : 3,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 260.0, 69.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "message",
													"text" : "setcell $1 $2 val $3",
													"id" : "obj-2",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 292.0, 113.0, 18.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "t b l",
													"id" : "obj-65",
													"numinlets" : 1,
													"fontsize" : 12.0,
													"numoutlets" : 2,
													"outlettype" : [ "bang", "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 156.0, 32.5, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "jit.matrix mcmc 1 float32 256 256",
													"id" : "obj-63",
													"numinlets" : 1,
													"fontsize" : 12.0,
													"numoutlets" : 2,
													"outlettype" : [ "jit_matrix", "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 170.0, 390.0, 187.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "newobj",
													"text" : "zl iter 2",
													"id" : "obj-58",
													"numinlets" : 2,
													"fontsize" : 12.0,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"fontname" : "Arial",
													"patching_rect" : [ 70.0, 128.0, 49.0, 20.0 ]
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "inlet",
													"id" : "obj-23",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 70.0, 40.0, 25.0, 25.0 ],
													"comment" : ""
												}

											}
, 											{
												"box" : 												{
													"maxclass" : "outlet",
													"id" : "obj-24",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 170.0, 470.0, 25.0, 25.0 ],
													"comment" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"source" : [ "obj-63", 0 ],
													"destination" : [ "obj-24", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-23", 0 ],
													"destination" : [ "obj-5", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-3", 0 ],
													"destination" : [ "obj-2", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-2", 0 ],
													"destination" : [ "obj-63", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-58", 0 ],
													"destination" : [ "obj-65", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-65", 1 ],
													"destination" : [ "obj-6", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-65", 0 ],
													"destination" : [ "obj-4", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-4", 0 ],
													"destination" : [ "obj-15", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-15", 0 ],
													"destination" : [ "obj-3", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 2 ],
													"destination" : [ "obj-63", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 1 ],
													"destination" : [ "obj-58", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 0 ],
													"destination" : [ "obj-4", 2 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-5", 3 ],
													"destination" : [ "obj-16", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-18", 0 ],
													"destination" : [ "obj-63", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-14", 0 ],
													"destination" : [ "obj-12", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-12", 0 ],
													"destination" : [ "obj-63", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-16", 0 ],
													"destination" : [ "obj-19", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-19", 0 ],
													"destination" : [ "obj-15", 2 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-7", 0 ],
													"destination" : [ "obj-3", 1 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-6", 0 ],
													"destination" : [ "obj-7", 0 ],
													"hidden" : 0,
													"midpoints" : [  ]
												}

											}
, 											{
												"patchline" : 												{
													"source" : [ "obj-9", 0 ],
													"destination" : [ "obj-3", 2 ],
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
 ]
									}
,
									"saved_object_attributes" : 									{
										"fontface" : 0,
										"fontsize" : 12.0,
										"globalpatchername" : "",
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"fontname" : "Arial",
										"default_fontsize" : 12.0
									}

								}

							}
, 							{
								"box" : 								{
									"maxclass" : "jit.pwindow",
									"id" : "obj-59",
									"numinlets" : 1,
									"srcrect" : [ 0, 0, 256, 256 ],
									"numoutlets" : 2,
									"dstrect" : [ 0, 0, 256, 256 ],
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 468.0, 0.0, 871.0, 767.0 ],
									"name" : "mcmc_gl"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"id" : "obj-66",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "list" ],
									"patching_rect" : [ 116.0, 18.0, 25.0, 25.0 ],
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-125", 2 ],
									"destination" : [ "obj-52", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-123", 0 ],
									"destination" : [ "obj-124", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-120", 0 ],
									"destination" : [ "obj-124", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-124", 0 ],
									"destination" : [ "obj-125", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-43", 0 ],
									"destination" : [ "obj-36", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-44", 0 ],
									"destination" : [ "obj-37", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-125", 0 ],
									"destination" : [ "obj-127", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-125", 1 ],
									"destination" : [ "obj-127", 0 ],
									"hidden" : 0,
									"midpoints" : [ 43.5, 660.0, 21.5, 660.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-37", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-36", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-47", 0 ],
									"destination" : [ "obj-59", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-43", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-44", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 2 ],
									"destination" : [ "obj-21", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 1 ],
									"destination" : [ "obj-43", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-44", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 2 ],
									"destination" : [ "obj-21", 0 ],
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
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 3 ],
									"destination" : [ "obj-22", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-21", 6 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 4 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-21", 3 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-21", 7 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-20", 0 ],
									"destination" : [ "obj-21", 4 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 1 ],
									"destination" : [ "obj-20", 0 ],
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
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-26", 1 ],
									"destination" : [ "obj-27", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 0 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-26", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-66", 0 ],
									"destination" : [ "obj-26", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 1 ],
									"destination" : [ "obj-29", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 2 ],
									"destination" : [ "obj-30", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-33", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-33", 0 ],
									"destination" : [ "obj-14", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-47", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-123", 0 ],
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
						"globalpatchername" : "",
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 12.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"id" : "obj-62",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 480.0, 30.0, 41.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"id" : "obj-61",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 420.0, 30.0, 41.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll b2",
					"id" : "obj-55",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 480.0, 60.0, 59.5, 20.0 ],
					"coll_data" : 					{
						"count" : 62,
						"data" : [ 							{
								"key" : 40,
								"value" : [ 2480.320801, 0.006004, 0.214634 ]
							}
, 							{
								"key" : 41,
								"value" : [ 2545.730469, 0.000959, 4.55237 ]
							}
, 							{
								"key" : 42,
								"value" : [ 2670.649902, 0.029416, 0.148879 ]
							}
, 							{
								"key" : 43,
								"value" : [ 2687.073242, 0.016181, 0.143067 ]
							}
, 							{
								"key" : 44,
								"value" : [ 2775.491455, 0.00659, 4.321666 ]
							}
, 							{
								"key" : 45,
								"value" : [ 2854.015869, 0.003388, 9.518139 ]
							}
, 							{
								"key" : 46,
								"value" : [ 2909.356934, 0.002405, 0.163006 ]
							}
, 							{
								"key" : 47,
								"value" : [ 2942.420166, 0.00388, 7.519142 ]
							}
, 							{
								"key" : 48,
								"value" : [ 2972.062744, 0.002123, 4.195925 ]
							}
, 							{
								"key" : 49,
								"value" : [ 3264.5979, 0.012945, 0.562017 ]
							}
, 							{
								"key" : 50,
								"value" : [ 3405.526855, 0.00133, 2.623338 ]
							}
, 							{
								"key" : 51,
								"value" : [ 3562.357422, 0.00218, 3.148075 ]
							}
, 							{
								"key" : 52,
								"value" : [ 3783.184814, 0.003829, 3.745387 ]
							}
, 							{
								"key" : 53,
								"value" : [ 3798.970459, 0.017009, 10.655331 ]
							}
, 							{
								"key" : 54,
								"value" : [ 3818.577637, 0.007692, 0.132636 ]
							}
, 							{
								"key" : 55,
								"value" : [ 3833.684082, 0.018685, 0.106357 ]
							}
, 							{
								"key" : 56,
								"value" : [ 3844.305908, 0.013148, 0.326921 ]
							}
, 							{
								"key" : 57,
								"value" : [ 3882.167725, 0.066747, 0.296837 ]
							}
, 							{
								"key" : 58,
								"value" : [ 3910.677246, 0.000916, 0.391728 ]
							}
, 							{
								"key" : 59,
								"value" : [ 3926.656494, 0.002644, 3.352652 ]
							}
, 							{
								"key" : 60,
								"value" : [ 4037.989014, 0.008329, 6.303174 ]
							}
, 							{
								"key" : 61,
								"value" : [ 4065.206299, 0.009201, 2.063528 ]
							}
, 							{
								"key" : 62,
								"value" : [ 4139.70166, 0.001173, 2.490374 ]
							}
, 							{
								"key" : 63,
								"value" : [ 4226.937988, 0.017057, 10.66124 ]
							}
, 							{
								"key" : 64,
								"value" : [ 4260.552246, 0.006831, 8.772685 ]
							}
, 							{
								"key" : 65,
								"value" : [ 4339.583008, 0.001899, 0.058812 ]
							}
, 							{
								"key" : 66,
								"value" : [ 4367.380859, 0.002608, 2.670572 ]
							}
, 							{
								"key" : 67,
								"value" : [ 4479.712402, 0.001311, 5.216029 ]
							}
, 							{
								"key" : 68,
								"value" : [ 4522.817871, 0.012154, 6.449537 ]
							}
, 							{
								"key" : 69,
								"value" : [ 4550.859863, 0.00375, 1.221894 ]
							}
, 							{
								"key" : 70,
								"value" : [ 4605.637695, 0.017566, 12.928458 ]
							}
, 							{
								"key" : 71,
								"value" : [ 4666.940918, 0.024587, 5.718601 ]
							}
, 							{
								"key" : 72,
								"value" : [ 4745.598633, 0.001266, 0.36194 ]
							}
, 							{
								"key" : 73,
								"value" : [ 4809.610352, 0.004864, 0.6204 ]
							}
, 							{
								"key" : 74,
								"value" : [ 4825.925781, 0.007497, 0.182481 ]
							}
, 							{
								"key" : 75,
								"value" : [ 4851.306641, 0.006891, 4.368911 ]
							}
, 							{
								"key" : 76,
								"value" : [ 4871.682129, 0.006932, 5.236387 ]
							}
, 							{
								"key" : 77,
								"value" : [ 4964.739258, 0.002903, 6.903218 ]
							}
, 							{
								"key" : 78,
								"value" : [ 5040.164062, 0.001339, 2.630528 ]
							}
, 							{
								"key" : 79,
								"value" : [ 5191.727051, 0.002485, 6.57367 ]
							}
, 							{
								"key" : 80,
								"value" : [ 5235.740234, 0.001259, 2.565066 ]
							}
, 							{
								"key" : 81,
								"value" : [ 5268.115234, 0.002975, 0.101447 ]
							}
, 							{
								"key" : 82,
								"value" : [ 5300.748047, 0.016295, 5.282162 ]
							}
, 							{
								"key" : 83,
								"value" : [ 5321.609375, 0.002972, 3.476706 ]
							}
, 							{
								"key" : 84,
								"value" : [ 5374.025391, 0.012623, 0.387105 ]
							}
, 							{
								"key" : 85,
								"value" : [ 5404.853027, 0.004186, 3.840042 ]
							}
, 							{
								"key" : 86,
								"value" : [ 5478.959473, 0.001848, 2.972434 ]
							}
, 							{
								"key" : 87,
								"value" : [ 5549.34082, 0.002334, 6.440713 ]
							}
, 							{
								"key" : 88,
								"value" : [ 5611.518066, 0.002923, 6.918336 ]
							}
, 							{
								"key" : 89,
								"value" : [ 5809.298828, 0.007148, 5.917635 ]
							}
, 							{
								"key" : 90,
								"value" : [ 5847.330078, 0.005419, 4.114071 ]
							}
, 							{
								"key" : 91,
								"value" : [ 5931.84375, 0.002032, 3.073464 ]
							}
, 							{
								"key" : 92,
								"value" : [ 5986.87793, 0.000938, 2.252904 ]
							}
, 							{
								"key" : 93,
								"value" : [ 6068.418457, 0.005205, 4.071298 ]
							}
, 							{
								"key" : 94,
								"value" : [ 6210.710938, 0.002709, 6.76382 ]
							}
, 							{
								"key" : 95,
								"value" : [ 6287.410156, 0.000862, 4.326991 ]
							}
, 							{
								"key" : 96,
								"value" : [ 6465.839844, 0.006192, 8.711309 ]
							}
, 							{
								"key" : 97,
								"value" : [ 6576.297852, 0.001589, 5.62506 ]
							}
, 							{
								"key" : 98,
								"value" : [ 6730.796875, 0.006464, 8.602255 ]
							}
, 							{
								"key" : 99,
								"value" : [ 6985.117188, 0.001644, 5.697485 ]
							}
, 							{
								"key" : 100,
								"value" : [ 7018.568359, 0.001112, 4.866942 ]
							}
, 							{
								"key" : 101,
								"value" : [ 7568.255371, 0.001316, 5.224373 ]
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
					"maxclass" : "newobj",
					"text" : "coll b1",
					"id" : "obj-42",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 420.0, 60.0, 59.5, 20.0 ],
					"coll_data" : 					{
						"count" : 58,
						"data" : [ 							{
								"key" : 2,
								"value" : [ 44.428703, 0.000948, 2.263747 ]
							}
, 							{
								"key" : 3,
								"value" : [ 105.105789, 0.005561, 8.283034 ]
							}
, 							{
								"key" : 4,
								"value" : [ 105.987946, 0.004728, 0.027602 ]
							}
, 							{
								"key" : 5,
								"value" : [ 301.025726, 0.004145, 0.537605 ]
							}
, 							{
								"key" : 6,
								"value" : [ 301.322327, 0.004067, 0.063896 ]
							}
, 							{
								"key" : 7,
								"value" : [ 343.628357, 0.001207, 5.041814 ]
							}
, 							{
								"key" : 8,
								"value" : [ 406.502655, 0.005348, 4.100026 ]
							}
, 							{
								"key" : 9,
								"value" : [ 522.951904, 0.005215, 9.696371 ]
							}
, 							{
								"key" : 10,
								"value" : [ 557.567505, 0.015346, 0.050048 ]
							}
, 							{
								"key" : 11,
								"value" : [ 664.45575, 0.001737, 2.906578 ]
							}
, 							{
								"key" : 12,
								"value" : [ 703.368286, 0.015189, 11.94272 ]
							}
, 							{
								"key" : 13,
								"value" : [ 733.67865, 0.002115, 6.350811 ]
							}
, 							{
								"key" : 14,
								"value" : [ 785.913269, 0.012306, 9.968545 ]
							}
, 							{
								"key" : 15,
								"value" : [ 871.747803, 0.009862, 4.749314 ]
							}
, 							{
								"key" : 16,
								"value" : [ 875.094177, 0.005993, 0.066342 ]
							}
, 							{
								"key" : 17,
								"value" : [ 879.910522, 0.005887, 0.063846 ]
							}
, 							{
								"key" : 18,
								"value" : [ 1012.840332, 0.012604, 10.789166 ]
							}
, 							{
								"key" : 19,
								"value" : [ 1048.988525, 0.007745, 8.985867 ]
							}
, 							{
								"key" : 20,
								"value" : [ 1115.045166, 0.007925, 0.271933 ]
							}
, 							{
								"key" : 21,
								"value" : [ 1175.845703, 0.005228, 0.140735 ]
							}
, 							{
								"key" : 22,
								"value" : [ 1240.114258, 0.003412, 0.414345 ]
							}
, 							{
								"key" : 23,
								"value" : [ 1313.572632, 0.020711, 11.370027 ]
							}
, 							{
								"key" : 24,
								"value" : [ 1347.044556, 0.003522, 4.318935 ]
							}
, 							{
								"key" : 25,
								"value" : [ 1435.089844, 0.004455, 3.906166 ]
							}
, 							{
								"key" : 26,
								"value" : [ 1679.655151, 0.006915, 0.208798 ]
							}
, 							{
								"key" : 27,
								"value" : [ 1750.221924, 0.001689, 0.283003 ]
							}
, 							{
								"key" : 28,
								"value" : [ 1797.569946, 0.001088, 0.192999 ]
							}
, 							{
								"key" : 29,
								"value" : [ 1917.950317, 0.002683, 6.9455 ]
							}
, 							{
								"key" : 30,
								"value" : [ 1943.572876, 0.010738, 9.73517 ]
							}
, 							{
								"key" : 31,
								"value" : [ 1968.741821, 0.00379, 5.022655 ]
							}
, 							{
								"key" : 32,
								"value" : [ 1982.093018, 0.010626, 10.998216 ]
							}
, 							{
								"key" : 33,
								"value" : [ 2015.547485, 0.018401, 13.883597 ]
							}
, 							{
								"key" : 34,
								"value" : [ 2058.932861, 0.012229, 4.977602 ]
							}
, 							{
								"key" : 35,
								"value" : [ 2139.052246, 0.035985, 0.119983 ]
							}
, 							{
								"key" : 36,
								"value" : [ 2227.550781, 0.003884, 3.760629 ]
							}
, 							{
								"key" : 37,
								"value" : [ 2270.133057, 0.001783, 3.465316 ]
							}
, 							{
								"key" : 38,
								"value" : [ 2346.848877, 0.005677, 10.662461 ]
							}
, 							{
								"key" : 39,
								"value" : [ 2410.141113, 0.005541, 10.629949 ]
							}
, 							{
								"key" : 40,
								"value" : [ 2480.320801, 0.006004, 0.214634 ]
							}
, 							{
								"key" : 41,
								"value" : [ 2545.730469, 0.000959, 4.55237 ]
							}
, 							{
								"key" : 42,
								"value" : [ 2670.649902, 0.029416, 0.148879 ]
							}
, 							{
								"key" : 43,
								"value" : [ 2687.073242, 0.016181, 0.143067 ]
							}
, 							{
								"key" : 44,
								"value" : [ 2775.491455, 0.00659, 4.321666 ]
							}
, 							{
								"key" : 45,
								"value" : [ 2854.015869, 0.003388, 9.518139 ]
							}
, 							{
								"key" : 46,
								"value" : [ 2909.356934, 0.002405, 0.163006 ]
							}
, 							{
								"key" : 47,
								"value" : [ 2942.420166, 0.00388, 7.519142 ]
							}
, 							{
								"key" : 48,
								"value" : [ 2972.062744, 0.002123, 4.195925 ]
							}
, 							{
								"key" : 49,
								"value" : [ 3264.5979, 0.012945, 0.562017 ]
							}
, 							{
								"key" : 50,
								"value" : [ 3405.526855, 0.00133, 2.623338 ]
							}
, 							{
								"key" : 51,
								"value" : [ 3562.357422, 0.00218, 3.148075 ]
							}
, 							{
								"key" : 52,
								"value" : [ 3783.184814, 0.003829, 3.745387 ]
							}
, 							{
								"key" : 53,
								"value" : [ 3798.970459, 0.017009, 10.655331 ]
							}
, 							{
								"key" : 54,
								"value" : [ 3818.577637, 0.007692, 0.132636 ]
							}
, 							{
								"key" : 55,
								"value" : [ 3833.684082, 0.018685, 0.106357 ]
							}
, 							{
								"key" : 56,
								"value" : [ 3844.305908, 0.013148, 0.326921 ]
							}
, 							{
								"key" : 57,
								"value" : [ 3882.167725, 0.066747, 0.296837 ]
							}
, 							{
								"key" : 58,
								"value" : [ 3910.677246, 0.000916, 0.391728 ]
							}
, 							{
								"key" : 59,
								"value" : [ 3926.656494, 0.002644, 3.352652 ]
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
					"id" : "obj-41",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1110.0, 225.0, 50.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc 50",
					"id" : "obj-39",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 570.0, 105.0, 55.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "nothing",
					"id" : "obj-38",
					"numinlets" : 8,
					"fontsize" : 12.0,
					"numoutlets" : 8,
					"outlettype" : [ "signal", "", "", "", "", "", "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1005.0, 315.0, 113.5, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"id" : "obj-27",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 32.0, 236.0, 65.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "time_interval $1",
					"id" : "obj-26",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 30.0, 270.0, 95.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "list-accum 10000",
					"id" : "obj-16",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "", "int" ],
					"fontname" : "Arial",
					"patching_rect" : [ 323.0, 135.0, 101.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "zl slice 2",
					"id" : "obj-25",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 323.0, 100.0, 57.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll bowl1",
					"id" : "obj-13",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 323.0, 67.0, 63.0, 20.0 ],
					"coll_data" : 					{
						"count" : 100,
						"data" : [ 							{
								"key" : 2,
								"value" : [ 44.428703, 0.000948, 2.263747 ]
							}
, 							{
								"key" : 3,
								"value" : [ 105.105789, 0.005561, 8.283034 ]
							}
, 							{
								"key" : 4,
								"value" : [ 105.987946, 0.004728, 0.027602 ]
							}
, 							{
								"key" : 5,
								"value" : [ 301.025726, 0.004145, 0.537605 ]
							}
, 							{
								"key" : 6,
								"value" : [ 301.322327, 0.004067, 0.063896 ]
							}
, 							{
								"key" : 7,
								"value" : [ 343.628357, 0.001207, 5.041814 ]
							}
, 							{
								"key" : 8,
								"value" : [ 406.502655, 0.005348, 4.100026 ]
							}
, 							{
								"key" : 9,
								"value" : [ 522.951904, 0.005215, 9.696371 ]
							}
, 							{
								"key" : 10,
								"value" : [ 557.567505, 0.015346, 0.050048 ]
							}
, 							{
								"key" : 11,
								"value" : [ 664.45575, 0.001737, 2.906578 ]
							}
, 							{
								"key" : 12,
								"value" : [ 703.368286, 0.015189, 11.94272 ]
							}
, 							{
								"key" : 13,
								"value" : [ 733.67865, 0.002115, 6.350811 ]
							}
, 							{
								"key" : 14,
								"value" : [ 785.913269, 0.012306, 9.968545 ]
							}
, 							{
								"key" : 15,
								"value" : [ 871.747803, 0.009862, 4.749314 ]
							}
, 							{
								"key" : 16,
								"value" : [ 875.094177, 0.005993, 0.066342 ]
							}
, 							{
								"key" : 17,
								"value" : [ 879.910522, 0.005887, 0.063846 ]
							}
, 							{
								"key" : 18,
								"value" : [ 1012.840332, 0.012604, 10.789166 ]
							}
, 							{
								"key" : 19,
								"value" : [ 1048.988525, 0.007745, 8.985867 ]
							}
, 							{
								"key" : 20,
								"value" : [ 1115.045166, 0.007925, 0.271933 ]
							}
, 							{
								"key" : 21,
								"value" : [ 1175.845703, 0.005228, 0.140735 ]
							}
, 							{
								"key" : 22,
								"value" : [ 1240.114258, 0.003412, 0.414345 ]
							}
, 							{
								"key" : 23,
								"value" : [ 1313.572632, 0.020711, 11.370027 ]
							}
, 							{
								"key" : 24,
								"value" : [ 1347.044556, 0.003522, 4.318935 ]
							}
, 							{
								"key" : 25,
								"value" : [ 1435.089844, 0.004455, 3.906166 ]
							}
, 							{
								"key" : 26,
								"value" : [ 1679.655151, 0.006915, 0.208798 ]
							}
, 							{
								"key" : 27,
								"value" : [ 1750.221924, 0.001689, 0.283003 ]
							}
, 							{
								"key" : 28,
								"value" : [ 1797.569946, 0.001088, 0.192999 ]
							}
, 							{
								"key" : 29,
								"value" : [ 1917.950317, 0.002683, 6.9455 ]
							}
, 							{
								"key" : 30,
								"value" : [ 1943.572876, 0.010738, 9.73517 ]
							}
, 							{
								"key" : 31,
								"value" : [ 1968.741821, 0.00379, 5.022655 ]
							}
, 							{
								"key" : 32,
								"value" : [ 1982.093018, 0.010626, 10.998216 ]
							}
, 							{
								"key" : 33,
								"value" : [ 2015.547485, 0.018401, 13.883597 ]
							}
, 							{
								"key" : 34,
								"value" : [ 2058.932861, 0.012229, 4.977602 ]
							}
, 							{
								"key" : 35,
								"value" : [ 2139.052246, 0.035985, 0.119983 ]
							}
, 							{
								"key" : 36,
								"value" : [ 2227.550781, 0.003884, 3.760629 ]
							}
, 							{
								"key" : 37,
								"value" : [ 2270.133057, 0.001783, 3.465316 ]
							}
, 							{
								"key" : 38,
								"value" : [ 2346.848877, 0.005677, 10.662461 ]
							}
, 							{
								"key" : 39,
								"value" : [ 2410.141113, 0.005541, 10.629949 ]
							}
, 							{
								"key" : 40,
								"value" : [ 2480.320801, 0.006004, 0.214634 ]
							}
, 							{
								"key" : 41,
								"value" : [ 2545.730469, 0.000959, 4.55237 ]
							}
, 							{
								"key" : 42,
								"value" : [ 2670.649902, 0.029416, 0.148879 ]
							}
, 							{
								"key" : 43,
								"value" : [ 2687.073242, 0.016181, 0.143067 ]
							}
, 							{
								"key" : 44,
								"value" : [ 2775.491455, 0.00659, 4.321666 ]
							}
, 							{
								"key" : 45,
								"value" : [ 2854.015869, 0.003388, 9.518139 ]
							}
, 							{
								"key" : 46,
								"value" : [ 2909.356934, 0.002405, 0.163006 ]
							}
, 							{
								"key" : 47,
								"value" : [ 2942.420166, 0.00388, 7.519142 ]
							}
, 							{
								"key" : 48,
								"value" : [ 2972.062744, 0.002123, 4.195925 ]
							}
, 							{
								"key" : 49,
								"value" : [ 3264.5979, 0.012945, 0.562017 ]
							}
, 							{
								"key" : 50,
								"value" : [ 3405.526855, 0.00133, 2.623338 ]
							}
, 							{
								"key" : 51,
								"value" : [ 3562.357422, 0.00218, 3.148075 ]
							}
, 							{
								"key" : 52,
								"value" : [ 3783.184814, 0.003829, 3.745387 ]
							}
, 							{
								"key" : 53,
								"value" : [ 3798.970459, 0.017009, 10.655331 ]
							}
, 							{
								"key" : 54,
								"value" : [ 3818.577637, 0.007692, 0.132636 ]
							}
, 							{
								"key" : 55,
								"value" : [ 3833.684082, 0.018685, 0.106357 ]
							}
, 							{
								"key" : 56,
								"value" : [ 3844.305908, 0.013148, 0.326921 ]
							}
, 							{
								"key" : 57,
								"value" : [ 3882.167725, 0.066747, 0.296837 ]
							}
, 							{
								"key" : 58,
								"value" : [ 3910.677246, 0.000916, 0.391728 ]
							}
, 							{
								"key" : 59,
								"value" : [ 3926.656494, 0.002644, 3.352652 ]
							}
, 							{
								"key" : 60,
								"value" : [ 4037.989014, 0.008329, 6.303174 ]
							}
, 							{
								"key" : 61,
								"value" : [ 4065.206299, 0.009201, 2.063528 ]
							}
, 							{
								"key" : 62,
								"value" : [ 4139.70166, 0.001173, 2.490374 ]
							}
, 							{
								"key" : 63,
								"value" : [ 4226.937988, 0.017057, 10.66124 ]
							}
, 							{
								"key" : 64,
								"value" : [ 4260.552246, 0.006831, 8.772685 ]
							}
, 							{
								"key" : 65,
								"value" : [ 4339.583008, 0.001899, 0.058812 ]
							}
, 							{
								"key" : 66,
								"value" : [ 4367.380859, 0.002608, 2.670572 ]
							}
, 							{
								"key" : 67,
								"value" : [ 4479.712402, 0.001311, 5.216029 ]
							}
, 							{
								"key" : 68,
								"value" : [ 4522.817871, 0.012154, 6.449537 ]
							}
, 							{
								"key" : 69,
								"value" : [ 4550.859863, 0.00375, 1.221894 ]
							}
, 							{
								"key" : 70,
								"value" : [ 4605.637695, 0.017566, 12.928458 ]
							}
, 							{
								"key" : 71,
								"value" : [ 4666.940918, 0.024587, 5.718601 ]
							}
, 							{
								"key" : 72,
								"value" : [ 4745.598633, 0.001266, 0.36194 ]
							}
, 							{
								"key" : 73,
								"value" : [ 4809.610352, 0.004864, 0.6204 ]
							}
, 							{
								"key" : 74,
								"value" : [ 4825.925781, 0.007497, 0.182481 ]
							}
, 							{
								"key" : 75,
								"value" : [ 4851.306641, 0.006891, 4.368911 ]
							}
, 							{
								"key" : 76,
								"value" : [ 4871.682129, 0.006932, 5.236387 ]
							}
, 							{
								"key" : 77,
								"value" : [ 4964.739258, 0.002903, 6.903218 ]
							}
, 							{
								"key" : 78,
								"value" : [ 5040.164062, 0.001339, 2.630528 ]
							}
, 							{
								"key" : 79,
								"value" : [ 5191.727051, 0.002485, 6.57367 ]
							}
, 							{
								"key" : 80,
								"value" : [ 5235.740234, 0.001259, 2.565066 ]
							}
, 							{
								"key" : 81,
								"value" : [ 5268.115234, 0.002975, 0.101447 ]
							}
, 							{
								"key" : 82,
								"value" : [ 5300.748047, 0.016295, 5.282162 ]
							}
, 							{
								"key" : 83,
								"value" : [ 5321.609375, 0.002972, 3.476706 ]
							}
, 							{
								"key" : 84,
								"value" : [ 5374.025391, 0.012623, 0.387105 ]
							}
, 							{
								"key" : 85,
								"value" : [ 5404.853027, 0.004186, 3.840042 ]
							}
, 							{
								"key" : 86,
								"value" : [ 5478.959473, 0.001848, 2.972434 ]
							}
, 							{
								"key" : 87,
								"value" : [ 5549.34082, 0.002334, 6.440713 ]
							}
, 							{
								"key" : 88,
								"value" : [ 5611.518066, 0.002923, 6.918336 ]
							}
, 							{
								"key" : 89,
								"value" : [ 5809.298828, 0.007148, 5.917635 ]
							}
, 							{
								"key" : 90,
								"value" : [ 5847.330078, 0.005419, 4.114071 ]
							}
, 							{
								"key" : 91,
								"value" : [ 5931.84375, 0.002032, 3.073464 ]
							}
, 							{
								"key" : 92,
								"value" : [ 5986.87793, 0.000938, 2.252904 ]
							}
, 							{
								"key" : 93,
								"value" : [ 6068.418457, 0.005205, 4.071298 ]
							}
, 							{
								"key" : 94,
								"value" : [ 6210.710938, 0.002709, 6.76382 ]
							}
, 							{
								"key" : 95,
								"value" : [ 6287.410156, 0.000862, 4.326991 ]
							}
, 							{
								"key" : 96,
								"value" : [ 6465.839844, 0.006192, 8.711309 ]
							}
, 							{
								"key" : 97,
								"value" : [ 6576.297852, 0.001589, 5.62506 ]
							}
, 							{
								"key" : 98,
								"value" : [ 6730.796875, 0.006464, 8.602255 ]
							}
, 							{
								"key" : 99,
								"value" : [ 6985.117188, 0.001644, 5.697485 ]
							}
, 							{
								"key" : 100,
								"value" : [ 7018.568359, 0.001112, 4.866942 ]
							}
, 							{
								"key" : 101,
								"value" : [ 7568.255371, 0.001316, 5.224373 ]
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
					"text" : "dump",
					"id" : "obj-52",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 145.0, 21.0, 41.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "list-accum 10000",
					"id" : "obj-49",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "", "int" ],
					"fontname" : "Arial",
					"patching_rect" : [ 145.0, 128.0, 101.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "zl slice 2",
					"id" : "obj-48",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 145.0, 93.0, 57.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll tri",
					"id" : "obj-47",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 145.0, 52.0, 59.5, 20.0 ],
					"coll_data" : 					{
						"count" : 231,
						"data" : [ 							{
								"key" : 0,
								"value" : [ 2.370982, 0.000977, 0.088736 ]
							}
, 							{
								"key" : 1,
								"value" : [ 3.147822, 0.000685, 0.067035 ]
							}
, 							{
								"key" : 2,
								"value" : [ 4.03267, 0.000196, 0.121294 ]
							}
, 							{
								"key" : 3,
								"value" : [ 4.676418, 0.000353, 0.028545 ]
							}
, 							{
								"key" : 4,
								"value" : [ 5.461316, 0.00023, 0.024407 ]
							}
, 							{
								"key" : 5,
								"value" : [ 6.916585, 0.000501, 0.107991 ]
							}
, 							{
								"key" : 6,
								"value" : [ 6.850008, 0.000236, 0.016853 ]
							}
, 							{
								"key" : 7,
								"value" : [ 8.650642, 0.000308, 0.044592 ]
							}
, 							{
								"key" : 8,
								"value" : [ 10.097809, 0.000502, 0.171851 ]
							}
, 							{
								"key" : 9,
								"value" : [ 14.616128, 0.000437, 0.074887 ]
							}
, 							{
								"key" : 10,
								"value" : [ 17.344927, 0.00037, 0.005812 ]
							}
, 							{
								"key" : 11,
								"value" : [ 18.02301, 0.000588, 0.022159 ]
							}
, 							{
								"key" : 12,
								"value" : [ 18.467215, 0.001083, 0.044478 ]
							}
, 							{
								"key" : 13,
								"value" : [ 18.909594, 0.000263, 0.000165 ]
							}
, 							{
								"key" : 14,
								"value" : [ 21.372641, 0.000286, 0.001914 ]
							}
, 							{
								"key" : 15,
								"value" : [ 24.577496, 0.000242, 0.018715 ]
							}
, 							{
								"key" : 16,
								"value" : [ 25.031151, 0.000565, 0.033022 ]
							}
, 							{
								"key" : 17,
								"value" : [ 25.490702, 0.000517, 0.002789 ]
							}
, 							{
								"key" : 18,
								"value" : [ 26.080776, 0.000621, 0.361285 ]
							}
, 							{
								"key" : 19,
								"value" : [ 30.216333, 0.000779, 0.38017 ]
							}
, 							{
								"key" : 20,
								"value" : [ 30.076933, 0.000672, 0.028229 ]
							}
, 							{
								"key" : 21,
								"value" : [ 31.652561, 0.000229, 0.014388 ]
							}
, 							{
								"key" : 22,
								"value" : [ 37.075333, 0.000126, 0.028602 ]
							}
, 							{
								"key" : 23,
								"value" : [ 40.209911, 0.000285, 0.123738 ]
							}
, 							{
								"key" : 24,
								"value" : [ 40.263638, 0.000079, 0.006025 ]
							}
, 							{
								"key" : 25,
								"value" : [ 40.99865, 0.000131, 0.116327 ]
							}
, 							{
								"key" : 26,
								"value" : [ 44.251366, 0.000092, 0.045049 ]
							}
, 							{
								"key" : 27,
								"value" : [ 47.003525, 0.000109, 0.108479 ]
							}
, 							{
								"key" : 28,
								"value" : [ 47.987206, 0.000087, 0.013149 ]
							}
, 							{
								"key" : 29,
								"value" : [ 48.562244, 0.000126, 0.026356 ]
							}
, 							{
								"key" : 30,
								"value" : [ 48.668137, 0.000071, 0.012622 ]
							}
, 							{
								"key" : 31,
								"value" : [ 53.052734, 0.000089, 0.112803 ]
							}
, 							{
								"key" : 32,
								"value" : [ 54.181736, 0.000083, 0.097427 ]
							}
, 							{
								"key" : 33,
								"value" : [ 55.606316, 0.000131, 0.46446 ]
							}
, 							{
								"key" : 34,
								"value" : [ 57.221581, 0.000058, 0.014448 ]
							}
, 							{
								"key" : 35,
								"value" : [ 57.90868, 0.0001, 0.087117 ]
							}
, 							{
								"key" : 36,
								"value" : [ 63.684166, 0.000187, 0.01816 ]
							}
, 							{
								"key" : 37,
								"value" : [ 71.866562, 0.000087, 0.099189 ]
							}
, 							{
								"key" : 38,
								"value" : [ 73.068642, 0.000114, 0.110253 ]
							}
, 							{
								"key" : 39,
								"value" : [ 76.364601, 0.000159, 0.12423 ]
							}
, 							{
								"key" : 40,
								"value" : [ 77.971931, 0.0001, 0.10501 ]
							}
, 							{
								"key" : 41,
								"value" : [ 83.822426, 0.000259, 2.311362 ]
							}
, 							{
								"key" : 42,
								"value" : [ 83.877663, 0.00006, 0.004672 ]
							}
, 							{
								"key" : 43,
								"value" : [ 85.097122, 0.00009, 0.089502 ]
							}
, 							{
								"key" : 44,
								"value" : [ 84.995049, 0.00008, 0.095848 ]
							}
, 							{
								"key" : 45,
								"value" : [ 88.629547, 0.000108, 0.107983 ]
							}
, 							{
								"key" : 46,
								"value" : [ 90.886299, 0.00007, 0.008675 ]
							}
, 							{
								"key" : 47,
								"value" : [ 92.185562, 0.000108, 0.227866 ]
							}
, 							{
								"key" : 48,
								"value" : [ 94.793251, 0.000153, 0.245197 ]
							}
, 							{
								"key" : 49,
								"value" : [ 96.005188, 0.000223, 0.552919 ]
							}
, 							{
								"key" : 50,
								"value" : [ 96.731789, 0.000281, 0.223068 ]
							}
, 							{
								"key" : 51,
								"value" : [ 96.705017, 0.000119, 0.025483 ]
							}
, 							{
								"key" : 52,
								"value" : [ 103.860054, 0.000056, 0.323905 ]
							}
, 							{
								"key" : 53,
								"value" : [ 112.499352, 0.000102, 0.629293 ]
							}
, 							{
								"key" : 54,
								"value" : [ 119.871475, 0.00012, 0.033901 ]
							}
, 							{
								"key" : 55,
								"value" : [ 119.95504, 0.000124, 0.227871 ]
							}
, 							{
								"key" : 56,
								"value" : [ 904.405945, 0.000529, 0.278395 ]
							}
, 							{
								"key" : 57,
								"value" : [ 905.64856, 0.009636, 0.995312 ]
							}
, 							{
								"key" : 58,
								"value" : [ 905.648987, 0.007826, 0.295247 ]
							}
, 							{
								"key" : 59,
								"value" : [ 906.921936, 0.000621, 0.324925 ]
							}
, 							{
								"key" : 60,
								"value" : [ 930.742615, 0.000076, 1.619994 ]
							}
, 							{
								"key" : 61,
								"value" : [ 1472.695923, 0.000248, 0.102305 ]
							}
, 							{
								"key" : 62,
								"value" : [ 1472.724243, 0.00024, 0.056075 ]
							}
, 							{
								"key" : 63,
								"value" : [ 1649.182373, 0.000666, 0.460566 ]
							}
, 							{
								"key" : 64,
								"value" : [ 1649.192505, 0.000581, 0.274329 ]
							}
, 							{
								"key" : 65,
								"value" : [ 1702.973511, 0.000235, 0.240806 ]
							}
, 							{
								"key" : 66,
								"value" : [ 1705.794556, 0.00052, 0.084236 ]
							}
, 							{
								"key" : 67,
								"value" : [ 1705.779785, 0.000439, 0.086175 ]
							}
, 							{
								"key" : 68,
								"value" : [ 1707.357666, 0.000373, 0.059099 ]
							}
, 							{
								"key" : 69,
								"value" : [ 1707.378418, 0.000249, 0.029925 ]
							}
, 							{
								"key" : 70,
								"value" : [ 1708.581665, 0.003608, 0.103367 ]
							}
, 							{
								"key" : 71,
								"value" : [ 1708.629517, 0.002265, 0.033987 ]
							}
, 							{
								"key" : 72,
								"value" : [ 1709.844727, 0.026121, 0.039579 ]
							}
, 							{
								"key" : 73,
								"value" : [ 1711.077637, 0.003433, 0.073591 ]
							}
, 							{
								"key" : 74,
								"value" : [ 1711.099854, 0.003281, 0.069111 ]
							}
, 							{
								"key" : 75,
								"value" : [ 1712.336182, 0.000354, 0.076146 ]
							}
, 							{
								"key" : 76,
								"value" : [ 1713.087036, 0.000421, 0.329057 ]
							}
, 							{
								"key" : 77,
								"value" : [ 1713.370605, 0.00017, 0.105698 ]
							}
, 							{
								"key" : 78,
								"value" : [ 1713.900513, 0.000349, 0.079958 ]
							}
, 							{
								"key" : 79,
								"value" : [ 1716.717285, 0.000201, 0.230544 ]
							}
, 							{
								"key" : 80,
								"value" : [ 2443.841064, 0.000817, 21.186218 ]
							}
, 							{
								"key" : 81,
								"value" : [ 2581.275391, 0.000219, 0.201169 ]
							}
, 							{
								"key" : 82,
								"value" : [ 2581.124512, 0.000172, 0.111728 ]
							}
, 							{
								"key" : 83,
								"value" : [ 2938.228271, 0.000266, 0.156248 ]
							}
, 							{
								"key" : 84,
								"value" : [ 2938.272949, 0.000158, 0.119456 ]
							}
, 							{
								"key" : 85,
								"value" : [ 2939.752197, 0.000609, 0.107563 ]
							}
, 							{
								"key" : 86,
								"value" : [ 2940.948242, 0.003365, 0.17498 ]
							}
, 							{
								"key" : 87,
								"value" : [ 2941.035889, 0.001556, 0.074894 ]
							}
, 							{
								"key" : 88,
								"value" : [ 2942.340088, 0.008816, 0.125123 ]
							}
, 							{
								"key" : 89,
								"value" : [ 2942.24585, 0.006678, 0.083894 ]
							}
, 							{
								"key" : 90,
								"value" : [ 2943.515869, 0.002429, 0.127406 ]
							}
, 							{
								"key" : 91,
								"value" : [ 2944.737549, 0.000761, 0.128088 ]
							}
, 							{
								"key" : 92,
								"value" : [ 2944.726074, 0.000571, 0.100011 ]
							}
, 							{
								"key" : 93,
								"value" : [ 2946.231689, 0.000371, 0.192661 ]
							}
, 							{
								"key" : 94,
								"value" : [ 2946.179688, 0.000175, 0.125522 ]
							}
, 							{
								"key" : 95,
								"value" : [ 3111.110107, 0.00017, 6.331801 ]
							}
, 							{
								"key" : 96,
								"value" : [ 3113.830811, 0.000754, 17.896908 ]
							}
, 							{
								"key" : 97,
								"value" : [ 3382.561768, 0.001227, 0.162302 ]
							}
, 							{
								"key" : 98,
								"value" : [ 3383.839844, 0.000843, 0.151714 ]
							}
, 							{
								"key" : 99,
								"value" : [ 3385.101562, 0.000906, 0.138209 ]
							}
, 							{
								"key" : 100,
								"value" : [ 3385.061768, 0.000951, 0.115342 ]
							}
, 							{
								"key" : 101,
								"value" : [ 3419.642334, 0.000079, 0.190673 ]
							}
, 							{
								"key" : 102,
								"value" : [ 3521.042236, 0.000079, 0.759848 ]
							}
, 							{
								"key" : 103,
								"value" : [ 3666.778076, 0.00009, 2.607975 ]
							}
, 							{
								"key" : 104,
								"value" : [ 3685.875977, 0.000072, 1.457901 ]
							}
, 							{
								"key" : 105,
								"value" : [ 3721.315674, 0.000446, 0.189136 ]
							}
, 							{
								"key" : 106,
								"value" : [ 3722.324219, 0.001167, 0.200312 ]
							}
, 							{
								"key" : 107,
								"value" : [ 3722.334229, 0.00068, 0.180346 ]
							}
, 							{
								"key" : 108,
								"value" : [ 3723.80957, 0.001255, 0.16398 ]
							}
, 							{
								"key" : 109,
								"value" : [ 3723.874268, 0.001091, 0.134978 ]
							}
, 							{
								"key" : 110,
								"value" : [ 3725.093018, 0.010941, 0.167952 ]
							}
, 							{
								"key" : 111,
								"value" : [ 3725.10498, 0.008783, 0.149145 ]
							}
, 							{
								"key" : 112,
								"value" : [ 3726.334229, 0.015947, 0.134542 ]
							}
, 							{
								"key" : 113,
								"value" : [ 3727.583008, 0.010297, 0.164594 ]
							}
, 							{
								"key" : 114,
								"value" : [ 3728.844971, 0.002037, 0.177101 ]
							}
, 							{
								"key" : 115,
								"value" : [ 3730.469971, 0.001304, 0.258096 ]
							}
, 							{
								"key" : 116,
								"value" : [ 4334.208008, 0.000309, 0.21986 ]
							}
, 							{
								"key" : 117,
								"value" : [ 4335.368164, 0.000879, 0.214562 ]
							}
, 							{
								"key" : 118,
								"value" : [ 4335.398926, 0.000603, 0.145671 ]
							}
, 							{
								"key" : 119,
								"value" : [ 4336.630371, 0.005597, 2.174323 ]
							}
, 							{
								"key" : 120,
								"value" : [ 4336.538086, 0.00356, 0.146148 ]
							}
, 							{
								"key" : 121,
								"value" : [ 4337.658691, 0.000935, 0.195214 ]
							}
, 							{
								"key" : 122,
								"value" : [ 4337.665527, 0.000744, 0.162953 ]
							}
, 							{
								"key" : 123,
								"value" : [ 4338.873535, 0.000251, 0.173039 ]
							}
, 							{
								"key" : 124,
								"value" : [ 4343.054199, 0.000312, 0.794684 ]
							}
, 							{
								"key" : 125,
								"value" : [ 4396.020996, 0.000289, 0.178444 ]
							}
, 							{
								"key" : 126,
								"value" : [ 4397.257324, 0.000668, 0.180859 ]
							}
, 							{
								"key" : 127,
								"value" : [ 4397.314941, 0.000288, 0.084641 ]
							}
, 							{
								"key" : 128,
								"value" : [ 4398.521484, 0.000547, 0.119436 ]
							}
, 							{
								"key" : 129,
								"value" : [ 4398.499512, 0.000477, 0.118043 ]
							}
, 							{
								"key" : 130,
								"value" : [ 4399.867188, 0.000445, 0.168966 ]
							}
, 							{
								"key" : 131,
								"value" : [ 4400.987793, 0.000301, 0.179689 ]
							}
, 							{
								"key" : 132,
								"value" : [ 4484.846191, 0.000217, 7.646875 ]
							}
, 							{
								"key" : 133,
								"value" : [ 4652.130859, 0.000059, 0.166191 ]
							}
, 							{
								"key" : 134,
								"value" : [ 4739.751465, 0.002107, 0.285241 ]
							}
, 							{
								"key" : 135,
								"value" : [ 4741.005859, 0.003435, 0.259876 ]
							}
, 							{
								"key" : 136,
								"value" : [ 4742.050781, 0.022976, 0.378134 ]
							}
, 							{
								"key" : 137,
								"value" : [ 4742.202637, 0.009288, 0.269345 ]
							}
, 							{
								"key" : 138,
								"value" : [ 4743.505371, 0.003694, 0.256967 ]
							}
, 							{
								"key" : 139,
								"value" : [ 4744.713867, 0.002052, 0.28286 ]
							}
, 							{
								"key" : 140,
								"value" : [ 4746.025879, 0.001827, 0.901519 ]
							}
, 							{
								"key" : 141,
								"value" : [ 4746.12207, 0.000826, 0.267754 ]
							}
, 							{
								"key" : 142,
								"value" : [ 4762.961914, 0.000321, 0.306517 ]
							}
, 							{
								"key" : 143,
								"value" : [ 4764.544922, 0.000223, 0.241383 ]
							}
, 							{
								"key" : 144,
								"value" : [ 4765.677734, 0.001596, 0.342934 ]
							}
, 							{
								"key" : 145,
								"value" : [ 4767.056641, 0.003249, 0.297769 ]
							}
, 							{
								"key" : 146,
								"value" : [ 4766.944336, 0.002213, 0.252123 ]
							}
, 							{
								"key" : 147,
								"value" : [ 4768.177734, 0.001293, 0.291169 ]
							}
, 							{
								"key" : 148,
								"value" : [ 4771.010742, 0.000579, 0.371884 ]
							}
, 							{
								"key" : 149,
								"value" : [ 4770.876953, 0.000479, 0.330974 ]
							}
, 							{
								"key" : 150,
								"value" : [ 5975.770996, 0.000091, 1.619571 ]
							}
, 							{
								"key" : 151,
								"value" : [ 6007.495605, 0.000176, 1.465402 ]
							}
, 							{
								"key" : 152,
								"value" : [ 6011.061523, 0.000085, 0.78713 ]
							}
, 							{
								"key" : 153,
								"value" : [ 6020.581543, 0.000125, 0.914284 ]
							}
, 							{
								"key" : 154,
								"value" : [ 6044.175781, 0.005522, 0.537576 ]
							}
, 							{
								"key" : 155,
								"value" : [ 6046.422852, 0.030495, 0.513237 ]
							}
, 							{
								"key" : 156,
								"value" : [ 6047.077637, 0.147385, 6.51771 ]
							}
, 							{
								"key" : 157,
								"value" : [ 6047.660156, 0.073385, 0.335616 ]
							}
, 							{
								"key" : 158,
								"value" : [ 6047.724609, 0.040272, 0.426018 ]
							}
, 							{
								"key" : 159,
								"value" : [ 6048.936035, 0.026001, 0.413581 ]
							}
, 							{
								"key" : 160,
								"value" : [ 6051.634277, 0.006464, 0.666773 ]
							}
, 							{
								"key" : 161,
								"value" : [ 6051.333008, 0.002443, 0.376369 ]
							}
, 							{
								"key" : 162,
								"value" : [ 6053.029297, 0.010614, 0.861146 ]
							}
, 							{
								"key" : 163,
								"value" : [ 6056.543457, 0.005363, 1.126181 ]
							}
, 							{
								"key" : 164,
								"value" : [ 6149.657227, 0.000965, 0.347038 ]
							}
, 							{
								"key" : 165,
								"value" : [ 6150.874023, 0.003343, 0.429753 ]
							}
, 							{
								"key" : 166,
								"value" : [ 6152.144043, 0.007725, 0.498931 ]
							}
, 							{
								"key" : 167,
								"value" : [ 6153.183105, 0.003263, 0.441218 ]
							}
, 							{
								"key" : 168,
								"value" : [ 6156.019531, 0.001033, 0.425139 ]
							}
, 							{
								"key" : 169,
								"value" : [ 6181.320312, 0.000084, 1.566813 ]
							}
, 							{
								"key" : 170,
								"value" : [ 6188.064941, 0.000071, 1.45119 ]
							}
, 							{
								"key" : 171,
								"value" : [ 6191.319336, 0.000425, 5.277774 ]
							}
, 							{
								"key" : 172,
								"value" : [ 6194.421875, 0.0001, 1.677506 ]
							}
, 							{
								"key" : 173,
								"value" : [ 6204.275879, 0.000393, 5.17558 ]
							}
, 							{
								"key" : 174,
								"value" : [ 6279.669434, 0.000091, 3.239779 ]
							}
, 							{
								"key" : 175,
								"value" : [ 6379.604492, 0.000264, 4.834879 ]
							}
, 							{
								"key" : 176,
								"value" : [ 6754.256836, 0.00012, 5.281563 ]
							}
, 							{
								"key" : 177,
								"value" : [ 6953.789062, 0.000248, 4.565295 ]
							}
, 							{
								"key" : 178,
								"value" : [ 7080.391113, 0.000251, 8.633178 ]
							}
, 							{
								"key" : 179,
								"value" : [ 7525.257812, 0.000426, 0.660121 ]
							}
, 							{
								"key" : 180,
								"value" : [ 7598.208008, 0.003359, 1.072334 ]
							}
, 							{
								"key" : 181,
								"value" : [ 7600.178711, 0.004374, 0.876544 ]
							}
, 							{
								"key" : 182,
								"value" : [ 7604.671387, 0.029446, 0.771036 ]
							}
, 							{
								"key" : 183,
								"value" : [ 7604.525879, 0.012357, 0.63842 ]
							}
, 							{
								"key" : 184,
								"value" : [ 7728.901855, 0.000538, 0.464981 ]
							}
, 							{
								"key" : 185,
								"value" : [ 7731.916504, 0.002136, 0.438663 ]
							}
, 							{
								"key" : 186,
								"value" : [ 7734.371094, 0.003353, 0.556127 ]
							}
, 							{
								"key" : 187,
								"value" : [ 7757.749023, 0.00016, 0.996073 ]
							}
, 							{
								"key" : 188,
								"value" : [ 7757.760254, 0.000164, 0.476659 ]
							}
, 							{
								"key" : 189,
								"value" : [ 7767.63916, 0.000582, 5.695812 ]
							}
, 							{
								"key" : 190,
								"value" : [ 7892.619141, 0.000307, 9.188823 ]
							}
, 							{
								"key" : 191,
								"value" : [ 8380.03125, 0.00018, 6.43605 ]
							}
, 							{
								"key" : 192,
								"value" : [ 8495.056641, 0.000349, 7.686501 ]
							}
, 							{
								"key" : 193,
								"value" : [ 8750.536133, 0.091818, 12.407766 ]
							}
, 							{
								"key" : 194,
								"value" : [ 8754.862305, 0.059891, 1.480134 ]
							}
, 							{
								"key" : 195,
								"value" : [ 8785.723633, 0.000116, 1.780466 ]
							}
, 							{
								"key" : 196,
								"value" : [ 9146.708984, 0.000945, 1.584427 ]
							}
, 							{
								"key" : 197,
								"value" : [ 9160.636719, 0.013001, 0.926117 ]
							}
, 							{
								"key" : 198,
								"value" : [ 9162.811523, 0.00809, 0.818861 ]
							}
, 							{
								"key" : 199,
								"value" : [ 10225.063477, 0.001403, 0.800268 ]
							}
, 							{
								"key" : 200,
								"value" : [ 10226.65625, 0.011571, 1.207581 ]
							}
, 							{
								"key" : 201,
								"value" : [ 10227.160156, 0.018788, 20.606936 ]
							}
, 							{
								"key" : 202,
								"value" : [ 10226.920898, 0.008058, 1.049669 ]
							}
, 							{
								"key" : 203,
								"value" : [ 10310.756836, 0.007342, 0.783632 ]
							}
, 							{
								"key" : 204,
								"value" : [ 10315.826172, 0.011339, 1.014958 ]
							}
, 							{
								"key" : 205,
								"value" : [ 10351.799805, 0.000106, 2.197538 ]
							}
, 							{
								"key" : 206,
								"value" : [ 10703.47168, 0.008361, 1.168413 ]
							}
, 							{
								"key" : 207,
								"value" : [ 10706.880859, 0.020701, 1.360635 ]
							}
, 							{
								"key" : 208,
								"value" : [ 10747.875, 0.000095, 0.82204 ]
							}
, 							{
								"key" : 209,
								"value" : [ 10884.019531, 0.000227, 5.487315 ]
							}
, 							{
								"key" : 210,
								"value" : [ 10962.516602, 0.000137, 1.886979 ]
							}
, 							{
								"key" : 211,
								"value" : [ 11573.773438, 0.000162, 5.038076 ]
							}
, 							{
								"key" : 212,
								"value" : [ 11860.579102, 0.007288, 1.456691 ]
							}
, 							{
								"key" : 213,
								"value" : [ 11865.154297, 0.000747, 0.765472 ]
							}
, 							{
								"key" : 214,
								"value" : [ 11867.574219, 0.037705, 1.298528 ]
							}
, 							{
								"key" : 215,
								"value" : [ 11982.84375, 0.000327, 3.571186 ]
							}
, 							{
								"key" : 216,
								"value" : [ 12044.717773, 0.000291, 1.194368 ]
							}
, 							{
								"key" : 217,
								"value" : [ 12066.598633, 0.021866, 1.441055 ]
							}
, 							{
								"key" : 218,
								"value" : [ 12417.045898, 0.000191, 2.610245 ]
							}
, 							{
								"key" : 219,
								"value" : [ 12448.010742, 0.000641, 11.459399 ]
							}
, 							{
								"key" : 220,
								"value" : [ 12727.408203, 0.007705, 1.297352 ]
							}
, 							{
								"key" : 221,
								"value" : [ 13282.705078, 0.003958, 8.237914 ]
							}
, 							{
								"key" : 222,
								"value" : [ 13577.417969, 0.000397, 1.187673 ]
							}
, 							{
								"key" : 223,
								"value" : [ 13707.038086, 0.000947, 0.977996 ]
							}
, 							{
								"key" : 224,
								"value" : [ 13821.522461, 0.000428, 1.840012 ]
							}
, 							{
								"key" : 225,
								"value" : [ 14290.814453, 0.000307, 2.422517 ]
							}
, 							{
								"key" : 226,
								"value" : [ 15245.067383, 0.000141, 5.56082 ]
							}
, 							{
								"key" : 227,
								"value" : [ 15992.227539, 0.000446, 1.766853 ]
							}
, 							{
								"key" : 228,
								"value" : [ 16447.257812, 0.009251, 1.801313 ]
							}
, 							{
								"key" : 229,
								"value" : [ 18780.648438, 0.002101, 1.951221 ]
							}
, 							{
								"key" : 230,
								"value" : [ 21602.910156, 0.000466, 5.399398 ]
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
					"text" : "proposal_density_width $1",
					"id" : "obj-36",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 859.0, 199.0, 153.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-37",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 859.0, 168.0, 98.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "*~ 2",
					"id" : "obj-35",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1005.0, 285.0, 32.5, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "w0 $1",
					"id" : "obj-31",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 922.0, 111.0, 43.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-32",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 922.0, 80.0, 98.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "interp_alpha $1",
					"id" : "obj-30",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 815.0, 111.0, 93.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-29",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 815.0, 80.0, 98.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc 10",
					"id" : "obj-28",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 149.0, 171.0, 55.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "spectroscope~",
					"id" : "obj-24",
					"numinlets" : 2,
					"numoutlets" : 0,
					"domain" : [ 0.0, 8000.0 ],
					"patching_rect" : [ 930.0, 375.0, 300.0, 100.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sinusoids~",
					"id" : "obj-23",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 1005.0, 255.0, 68.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc 200",
					"id" : "obj-22",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 570.0, 146.0, 62.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"id" : "obj-21",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 60.0, 420.0, 65.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "selector~ 3",
					"id" : "obj-20",
					"numinlets" : 4,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 90.0, 451.0, 70.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "spectroscope~",
					"id" : "obj-19",
					"numinlets" : 2,
					"numoutlets" : 0,
					"domain" : [ 0.0, 8000.0 ],
					"patching_rect" : [ 613.0, 371.0, 300.0, 100.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sinusoids~",
					"id" : "obj-18",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 692.0, 337.0, 68.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "migrator",
					"id" : "obj-15",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 4,
					"outlettype" : [ "list", "list", "int", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 692.0, 266.0, 59.5, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "nOsc 60",
					"id" : "obj-12",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 84.0, 188.0, 55.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"id" : "obj-9",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 90.0, 645.0, 45.0, 45.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gain~",
					"id" : "obj-8",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "int" ],
					"patching_rect" : [ 91.0, 481.0, 20.0, 140.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "spectroscope~",
					"id" : "obj-7",
					"numinlets" : 2,
					"numoutlets" : 0,
					"domain" : [ 0.0, 8000.0 ],
					"patching_rect" : [ 301.0, 371.0, 300.0, 100.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sinusoids~",
					"id" : "obj-5",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"fontname" : "Arial",
					"patching_rect" : [ 241.0, 337.0, 68.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"id" : "obj-6",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 330.0, 330.0, 98.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route acceptance_rate cdf centroid",
					"id" : "obj-2",
					"numinlets" : 1,
					"fontsize" : 9.0,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 400.0, 299.0, 149.0, 17.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"id" : "obj-3",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 494.0, 144.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"id" : "obj-4",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 236.0, 177.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadmess dump",
					"id" : "obj-11",
					"numinlets" : 1,
					"fontsize" : 9.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 323.0, 40.0, 74.0, 17.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mcmc @nOsc 100 @time_interval 30 @sample_mode 0",
					"id" : "obj-1",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 3,
					"outlettype" : [ "list", "list", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 315.0, 270.0, 310.0, 20.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 2 ],
					"destination" : [ "obj-92", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-92", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 2 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-72", 0 ],
					"destination" : [ "obj-1", 0 ],
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
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-67", 0 ],
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
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
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
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-1", 0 ],
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
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-68", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-95", 0 ],
					"destination" : [ "obj-96", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 3 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 2 ],
					"destination" : [ "obj-94", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 2 ],
					"destination" : [ "obj-86", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 2 ],
					"destination" : [ "obj-82", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-65", 0 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 3 ],
					"destination" : [ "obj-49", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-49", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-20", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-20", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-9", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-36", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 3 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-20", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-35", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 3 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 3 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 0 ],
					"destination" : [ "obj-55", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-61", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-58", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 0 ],
					"destination" : [ "obj-58", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-58", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-60", 0 ],
					"destination" : [ "obj-63", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-64", 0 ],
					"destination" : [ "obj-63", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-63", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 1 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 1 ],
					"destination" : [ "obj-64", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-60", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-59", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-69", 0 ],
					"destination" : [ "obj-66", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 1 ],
					"destination" : [ "obj-70", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-71", 0 ],
					"destination" : [ "obj-70", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-71", 0 ],
					"destination" : [ "obj-67", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-73", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
