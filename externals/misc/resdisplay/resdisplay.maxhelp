{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 65.0, 44.0, 1086.0, 578.0 ],
		"bglocked" : 0,
		"defrect" : [ 65.0, 44.0, 1086.0, 578.0 ],
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
					"maxclass" : "message",
					"text" : "ymin_log -100, ymax_log 0",
					"patching_rect" : [ 380.0, 72.0, 154.0, 18.0 ],
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 378.0, 73.0, 0.0, 0.0 ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "xmin 0, xmax 22050",
					"patching_rect" : [ 380.0, 48.0, 118.0, 18.0 ],
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"id" : "obj-15",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"patching_rect" : [ 300.0, 20.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"id" : "obj-12",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "log $1",
					"patching_rect" : [ 300.0, 48.0, 43.0, 18.0 ],
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"patching_rect" : [ 232.0, 20.0, 20.0, 20.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"id" : "obj-13",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "route selected unselected",
					"patching_rect" : [ 44.0, 372.0, 147.0, 20.0 ],
					"numoutlets" : 3,
					"fontsize" : 12.0,
					"outlettype" : [ "", "", "" ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "resdisplay",
					"patching_rect" : [ 44.0, 400.0, 202.0, 89.0 ],
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numoutlets" : 1,
					"textcolor" : [  ],
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "resdisplay",
					"patching_rect" : [ 348.0, 400.0, 202.0, 89.0 ],
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numoutlets" : 1,
					"textcolor" : [  ],
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"id" : "obj-9",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jsui",
					"patching_rect" : [ 888.0, 376.0, 171.0, 78.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"jsarguments" : [  ],
					"filename" : "resonance-display.js",
					"id" : "obj-8",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "jsui",
					"patching_rect" : [ 556.0, 376.0, 171.0, 78.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"jsarguments" : [  ],
					"filename" : "resonance-display.js",
					"id" : "obj-7",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "mode $1",
					"patching_rect" : [ 232.0, 48.0, 57.0, 18.0 ],
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"patching_rect" : [ 44.0, 4.0, 41.0, 18.0 ],
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "zl group",
					"patching_rect" : [ 44.0, 72.0, 53.0, 20.0 ],
					"numoutlets" : 2,
					"fontsize" : 12.0,
					"outlettype" : [ "", "" ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll",
					"patching_rect" : [ 44.0, 44.0, 59.5, 20.0 ],
					"numoutlets" : 4,
					"fontsize" : 12.0,
					"outlettype" : [ "", "", "", "" ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"numinlets" : 1,
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
					"patching_rect" : [ 556.0, 100.0, 503.0, 265.0 ],
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"jsarguments" : [  ],
					"filename" : "resonance-display.js",
					"id" : "obj-4",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "resdisplay",
					"ymin_log" : -100.0,
					"patching_rect" : [ 44.0, 100.0, 505.0, 266.0 ],
					"ymax" : 0.1,
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numoutlets" : 1,
					"ymax_log" : 0.0,
					"textcolor" : [  ],
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
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
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 1 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-4", 0 ],
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
					"hidden" : 0,
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
					"source" : [ "obj-11", 1 ],
					"destination" : [ "obj-9", 0 ],
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
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
