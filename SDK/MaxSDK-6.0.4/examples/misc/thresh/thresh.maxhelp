{
	"patcher" : 	{
		"rect" : [ 39.0, 76.0, 581.0, 343.0 ],
		"bglocked" : 0,
		"defrect" : [ 39.0, 76.0, 581.0, 343.0 ],
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
					"text" : "autohelp",
					"patching_rect" : [ 428.970001, 282.119995, 55.0, 20.0 ],
					"hidden" : 1,
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-2",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p a-warning-about-using-thresh",
					"patching_rect" : [ 316.398712, 253.352646, 170.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-29",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"rect" : [ 271.0, 228.0, 629.0, 504.0 ],
						"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
						"bglocked" : 0,
						"defrect" : [ 271.0, 228.0, 629.0, 504.0 ],
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
									"maxclass" : "button",
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 27.0, 395.75, 20.0, 20.0 ],
									"id" : "obj-1",
									"numinlets" : 1,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 49.5, 455.75, 20.0, 20.0 ],
									"id" : "obj-3",
									"numinlets" : 1,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Each time you send a value to a thresh object, its time period is reset. In this example, you'll never see anything sent to the message box, because the metro object is firing too quickly in  comparison to thresh's threshold time.",
									"linecount" : 3,
									"patching_rect" : [ 128.0, 362.0, 492.0, 46.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-47",
									"numinlets" : 1,
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"outlettype" : [ "" ],
									"patching_rect" : [ 70.5, 455.75, 46.0, 18.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-46",
									"numinlets" : 2,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "thresh 100",
									"outlettype" : [ "" ],
									"patching_rect" : [ 49.5, 422.75, 65.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-45",
									"numinlets" : 2,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "toggle",
									"outlettype" : [ "int" ],
									"patching_rect" : [ 49.5, 341.0, 20.0, 20.0 ],
									"id" : "obj-44",
									"numinlets" : 1,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 74",
									"outlettype" : [ "int" ],
									"patching_rect" : [ 49.5, 395.75, 66.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-41",
									"numinlets" : 2,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "metro 50",
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 49.5, 368.75, 56.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-40",
									"numinlets" : 2,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "This one is also good",
									"patching_rect" : [ 465.711914, 79.0, 120.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-33",
									"numinlets" : 1,
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 442.711914, 79.0, 20.0, 20.0 ],
									"id" : "obj-34",
									"numinlets" : 1,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 5",
									"outlettype" : [ "bang", "bang", "int" ],
									"patching_rect" : [ 515.545288, 137.0, 43.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-35",
									"numinlets" : 2,
									"numoutlets" : 3
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "1 2 3 4 5",
									"outlettype" : [ "" ],
									"patching_rect" : [ 442.211914, 238.0, 56.0, 18.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-36",
									"numinlets" : 2,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl reg",
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 442.211914, 207.0, 129.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-37",
									"numinlets" : 2,
									"numoutlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "trigger b b l 74",
									"outlettype" : [ "bang", "bang", "", "int" ],
									"patching_rect" : [ 442.211914, 101.0, 129.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-38",
									"numinlets" : 1,
									"numoutlets" : 4
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl group",
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 478.545227, 177.0, 52.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-39",
									"numinlets" : 2,
									"numoutlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Good - this is more like i!",
									"patching_rect" : [ 288.0, 79.0, 138.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-31",
									"numinlets" : 1,
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Uh oh - this is bad!",
									"patching_rect" : [ 67.0, 79.0, 107.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-29",
									"numinlets" : 1,
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 265.0, 79.0, 20.0, 20.0 ],
									"id" : "obj-22",
									"numinlets" : 1,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 5",
									"outlettype" : [ "bang", "bang", "int" ],
									"patching_rect" : [ 318.0, 136.0, 46.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-24",
									"numinlets" : 2,
									"numoutlets" : 3
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "1 2 3 4 5",
									"outlettype" : [ "" ],
									"patching_rect" : [ 264.5, 238.0, 56.0, 18.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-25",
									"numinlets" : 2,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl reg",
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 264.5, 207.0, 126.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-26",
									"numinlets" : 2,
									"numoutlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "trigger b b 74",
									"outlettype" : [ "bang", "bang", "int" ],
									"patching_rect" : [ 264.5, 101.0, 126.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-27",
									"numinlets" : 1,
									"numoutlets" : 3
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl group",
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 331.0, 174.0, 52.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-28",
									"numinlets" : 2,
									"numoutlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 47.0, 79.0, 20.0, 20.0 ],
									"id" : "obj-21",
									"numinlets" : 1,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "uzi 5",
									"outlettype" : [ "bang", "bang", "int" ],
									"patching_rect" : [ 101.0, 139.0, 46.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-19",
									"numinlets" : 2,
									"numoutlets" : 3
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "You will always see 74 here because thresh outputs the list after the last bang coming from the trigger object.",
									"linecount" : 3,
									"patching_rect" : [ 30.0, 255.0, 210.0, 46.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-9",
									"numinlets" : 1,
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "74",
									"outlettype" : [ "" ],
									"patching_rect" : [ 46.5, 235.0, 32.5, 18.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-8",
									"numinlets" : 2,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl reg",
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 46.5, 207.0, 128.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-5",
									"numinlets" : 2,
									"numoutlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "trigger b b 74",
									"outlettype" : [ "bang", "bang", "int" ],
									"patching_rect" : [ 46.5, 101.0, 128.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-2",
									"numinlets" : 1,
									"numoutlets" : 3
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "thresh 20",
									"outlettype" : [ "" ],
									"patching_rect" : [ 101.0, 177.0, 59.0, 20.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-23",
									"numinlets" : 2,
									"numoutlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Since thresh is a time-based object, take care that you don't use it in a non time-based context if you want  to keep the event order synchronized.",
									"linecount" : 2,
									"patching_rect" : [ 37.0, 20.0, 496.0, 33.0 ],
									"fontsize" : 10.0,
									"fontname" : "Arial",
									"id" : "obj-13",
									"numinlets" : 1,
									"numoutlets" : 0
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-40", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [ 59.0, 392.0, 36.5, 392.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-45", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-5", 1 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-8", 1 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 2 ],
									"destination" : [ "obj-23", 0 ],
									"hidden" : 0,
									"midpoints" : [ 137.5, 164.0, 110.5, 164.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-26", 0 ],
									"destination" : [ "obj-25", 1 ],
									"hidden" : 0,
									"midpoints" : [ 274.0, 234.0, 311.0, 234.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-26", 1 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 2 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 1 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-35", 2 ],
									"destination" : [ "obj-39", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-39", 0 ],
									"destination" : [ "obj-37", 1 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-37", 0 ],
									"destination" : [ "obj-36", 1 ],
									"hidden" : 0,
									"midpoints" : [ 451.711914, 234.0, 488.711914, 234.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-40", 0 ],
									"destination" : [ "obj-41", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-44", 0 ],
									"destination" : [ "obj-40", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-45", 0 ],
									"destination" : [ "obj-46", 1 ],
									"hidden" : 0,
									"midpoints" : [ 59.0, 447.0, 107.0, 447.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-41", 0 ],
									"destination" : [ "obj-45", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-38", 3 ],
									"destination" : [ "obj-37", 1 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-38", 2 ],
									"destination" : [ "obj-35", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-38", 1 ],
									"destination" : [ "obj-39", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-38", 0 ],
									"destination" : [ "obj-37", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-38", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 2 ],
									"destination" : [ "obj-5", 1 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 1 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-27", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 1 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 2 ],
									"destination" : [ "obj-26", 1 ],
									"hidden" : 0
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 0 ],
									"destination" : [ "obj-26", 0 ],
									"hidden" : 0
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontsize" : 10.0,
						"fontname" : "Arial",
						"default_fontsize" : 10.0,
						"globalpatchername" : "",
						"fontface" : 0,
						"default_fontface" : 0,
						"default_fontname" : "Arial"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• play a chord and single notes, and watch the difference in the Max window",
					"linecount" : 4,
					"patching_rect" : [ 127.610535, 94.578949, 116.0, 60.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-13",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "stripnote",
					"outlettype" : [ "int", "int" ],
					"patching_rect" : [ 33.0, 131.352646, 55.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-14",
					"numinlets" : 2,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "notein",
					"outlettype" : [ "int", "int", "int" ],
					"patching_rect" : [ 33.0, 101.352638, 90.028961, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-15",
					"numinlets" : 1,
					"numoutlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• Click here and watch the Max Window",
					"linecount" : 3,
					"patching_rect" : [ 372.398712, 93.352638, 84.0, 46.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-16",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "99",
					"outlettype" : [ "" ],
					"patching_rect" : [ 333.398712, 101.352638, 34.0, 18.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-17",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "thresh 40",
					"outlettype" : [ "" ],
					"patching_rect" : [ 316.398712, 182.352646, 58.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-18",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "thresh 60",
					"outlettype" : [ "" ],
					"patching_rect" : [ 394.398712, 182.352646, 58.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-19",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print t40",
					"patching_rect" : [ 316.398712, 216.352646, 55.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-20",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print t60",
					"patching_rect" : [ 394.398712, 216.352646, 55.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-21",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pipe 50",
					"outlettype" : [ "" ],
					"patching_rect" : [ 333.398712, 155.352646, 47.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-22",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "thresh 20",
					"outlettype" : [ "" ],
					"patching_rect" : [ 33.0, 247.186874, 66.902634, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-23",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"outlettype" : [ "int", "bang" ],
					"patching_rect" : [ 81.0, 163.352646, 50.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-24",
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Set threshold in milliseconds here",
					"linecount" : 2,
					"patching_rect" : [ 131.547379, 159.352646, 100.0, 33.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-25",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Argument sets the default threshold\n(5 msec if you don't type an argument)",
					"linecount" : 2,
					"patching_rect" : [ 100.0, 239.332932, 214.121094, 33.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-26",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print notes",
					"patching_rect" : [ 33.0, 288.427643, 65.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-27",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "thresh",
					"patching_rect" : [ 10.0, 8.0, 485.0, 30.0 ],
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 18.0,
					"fontname" : "Arial",
					"fontface" : 3,
					"id" : "obj-3",
					"numinlets" : 1,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "Combine numbers into a list when received close together",
					"patching_rect" : [ 10.0, 36.0, 485.0, 21.0 ],
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 11.0,
					"fontname" : "Arial",
					"id" : "obj-4",
					"numinlets" : 1,
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_description",
					"text" : "thresh collects items into a list if they appear within a certain specifiable amount of time.",
					"patching_rect" : [ 10.0, 57.0, 485.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-5",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textbutton",
					"varname" : "autohelp_top_ref",
					"outlettype" : [ "int", "", "int" ],
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"patching_rect" : [ 357.975403, 22.0, 142.024582, 14.66663 ],
					"underline" : 1,
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontsize" : 11.0,
					"fontname" : "Arial",
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"text" : "open thresh reference",
					"fontface" : 3,
					"id" : "obj-6",
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"spacing_x" : 0.0,
					"textcolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"textovercolor" : [ 0.33, 0.45, 0.6, 1.0 ],
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"numoutlets" : 3,
					"presentation_rect" : [ 0.0, 0.0, 142.024582, 14.66663 ],
					"spacing_y" : 0.0,
					"fontlink" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_see_title",
					"text" : "See Also:",
					"patching_rect" : [ 433.970001, 286.119995, 100.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"fontface" : 1,
					"id" : "obj-8",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"varname" : "autohelp_see_menu",
					"outlettype" : [ "int", "", "" ],
					"patching_rect" : [ 433.970001, 306.119995, 130.0, 20.0 ],
					"fontsize" : 10.0,
					"fontname" : "Arial",
					"id" : "obj-9",
					"numinlets" : 1,
					"types" : [  ],
					"numoutlets" : 3,
					"items" : [ "(Objects:)", ",", "Bondo", ",", "Buddy", ",", "Iter", ",", "Pack", ",", "Quickthresh", ",", "Zl", ",", "<separator>", ",", "(Tutorials:)", ",", "Tutorial_37" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel",
					"grad2" : [ 0.85, 0.85, 0.85, 1.0 ],
					"patching_rect" : [ 5.0, 5.0, 495.0, 52.0 ],
					"mode" : 1,
					"id" : "obj-7",
					"numinlets" : 1,
					"background" : 1,
					"grad1" : [ 0.27, 0.35, 0.47, 1.0 ],
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_see_panel",
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ],
					"patching_rect" : [ 428.970001, 282.119995, 140.0, 50.0 ],
					"border" : 2,
					"id" : "obj-10",
					"numinlets" : 1,
					"background" : 1,
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ],
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 1 ],
					"destination" : [ "obj-14", 1 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-23", 1 ],
					"hidden" : 0
				}

			}
 ]
	}

}
