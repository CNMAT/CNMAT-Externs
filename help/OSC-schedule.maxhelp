{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 7,
			"minor" : 0,
			"revision" : 4,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"rect" : [ 238.0, 139.0, 723.0, 502.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"bgcolor" : [ 0.666667, 0.698039, 0.717647, 0.0 ],
					"bgoncolor" : [ 0.666667, 0.698039, 0.717647, 0.0 ],
					"fontlink" : 1,
					"fontname" : "Arial Bold",
					"fontsize" : 10.0,
					"id" : "obj-15",
					"legacytextcolor" : 1,
					"maxclass" : "textbutton",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 22.0, 196.0, 203.919922, 19.171875 ],
					"presentation_rect" : [ 30.0, 30.0, 203.919922, 19.171875 ],
					"prototypename" : "M4L.display",
					"style" : "",
					"text" : "https://github.com/CNMAT/CNMAT-odot",
					"textcolor" : [ 0.011765, 0.396078, 0.752941, 1.0 ],
					"texton" : "https://github.com/CNMAT/CNMAT-odot",
					"textoncolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"textovercolor" : [ 0.317647, 0.654902, 0.976471, 1.0 ],
					"usebgoncolor" : 1,
					"usetextovercolor" : 1
				}

			}
, 			{
				"box" : 				{
					"hidden" : 1,
					"id" : "obj-16",
					"linecount" : 3,
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 22.0, 223.0, 222.0, 49.0 ],
					"style" : "default",
					"text" : ";\rmax launch_browser https://github.com/CNMAT/CNMAT-odot"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-22",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 22.0, 159.0, 272.0, 33.0 ],
					"style" : "",
					"text" : "Note: for the current version of CNMAT OSC objects please see the Odot library, available at: "
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-44",
					"items" : [ "OSC-timetag", ",", "OpenSoundControl" ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 275.0, 366.0, 141.0, 22.0 ],
					"prefix_mode" : 2,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"hidden" : 1,
					"id" : "obj-21",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 336.0, 402.0, 138.0, 22.0 ],
					"style" : "",
					"text" : "sprintf load %s.maxhelp"
				}

			}
, 			{
				"box" : 				{
					"hidden" : 1,
					"id" : "obj-26",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 336.0, 427.0, 53.0, 22.0 ],
					"style" : "",
					"text" : "pcontrol"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-27",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 212.0, 366.0, 61.0, 20.0 ],
					"style" : "",
					"text" : "See also:"
				}

			}
, 			{
				"box" : 				{
					"data" : [ 593, "png", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ],
					"embed" : 1,
					"id" : "obj-14",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "jit_matrix" ],
					"patching_rect" : [ 206.0, 9.0, 109.0, 48.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 4,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 442.0, 108.0, 705.0, 390.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 71.0, 184.0, 531.0, 20.0 ],
									"style" : "default",
									"text" : "The largest recommended value is 8000 seconds, or about 2 hours to retain millisecond precision."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 297.0, 107.0, 368.0, 47.0 ],
									"style" : "default",
									"text" : "The maximum delay can be increased, but for values larger than a few hours, the precision of event timing may be degraded by the use of floating point calculations in the MaxMSP scheduler."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 71.0, 65.0, 330.0, 33.0 ],
									"style" : "default",
									"text" : "By default OSC-schedule will not delay a message more than 1000 seconds into the future."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "FullPacket", "FullPacket", "FullPacket" ],
									"patching_rect" : [ 71.0, 112.0, 205.0, 22.0 ],
									"style" : "default",
									"text" : "OSC-schedule @maxdelay 8000."
								}

							}
 ],
						"lines" : [  ]
					}
,
					"patching_rect" : [ 370.0, 246.870361, 148.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p scheduler-delay-options"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 4,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 385.0, 267.0, 755.0, 390.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 67.0, 255.0, 570.0, 20.0 ],
									"style" : "default",
									"text" : "http://cnmat.berkeley.edu/publication/implementation_and_applications_open_sound_control_timestamps"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 67.0, 233.0, 179.0, 20.0 ],
									"style" : "default",
									"text" : "For more details see this paper:"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 41.0, 101.0, 532.0, 47.0 ],
									"style" : "default",
									"text" : "OSC-schedule can be used to implement forward synchronization or backward resynchronization to coordinate precise timing of events between multiple computers connected by a network with variable delay."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 41.0, 159.0, 487.0, 20.0 ],
									"style" : "default",
									"text" : "The priority queue is O(log n) for insertion, and O(1) to extract the maximum priority node."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 41.0, 68.0, 497.0, 20.0 ],
									"style" : "default",
									"text" : "Internally OSC-schedule uses a heap priority queue to store messages sorted in time-order."
								}

							}
 ],
						"lines" : [  ]
					}
,
					"patching_rect" : [ 370.0, 275.259277, 141.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p implementation-details"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 4,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 442.0, 108.0, 669.0, 592.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 117.0, 280.0, 367.0, 20.0 ],
									"style" : "default",
									"text" : "See \"Scheduler Event Interval\" setting under Performance Options."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 117.0, 146.0, 253.0, 47.0 ],
									"style" : "default",
									"text" : "For optimal performance, the scheduler precision of OSC-schedule should be set to 1.5X the scheduler precision of MaxMSP."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "FullPacket", "FullPacket", "FullPacket" ],
									"patching_rect" : [ 117.0, 98.0, 209.0, 22.0 ],
									"style" : "default",
									"text" : "OSC-schedule @precision 0.0015"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 117.0, 212.0, 246.0, 47.0 ],
									"style" : "default",
									"text" : "The default scheduler precision is 3msec corresponding to the MaxMSP default setting of 2mesc precision."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 117.0, 65.0, 463.0, 20.0 ],
									"style" : "default",
									"text" : "Here is a setting of 1.5msec, which is optimal for a scheduler event interval of 1msec."
								}

							}
 ],
						"lines" : [  ]
					}
,
					"patching_rect" : [ 370.0, 218.481476, 167.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p scheduler-precision-options"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-4",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 4,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 262.0, 145.0, 821.0, 449.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 87.0, 271.0, 623.0, 20.0 ],
									"style" : "default",
									"text" : "Increasing the queue and/or packet size will increase the amount of memory used by OSC-schedule, proportionally."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 420.0, 186.0, 274.0, 47.0 ],
									"style" : "default",
									"text" : "The packetsize is the maximum length of an OSC message that may be stored, in bytes. The default is 1500."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "FullPacket", "FullPacket", "FullPacket" ],
									"patching_rect" : [ 130.0, 190.0, 278.0, 22.0 ],
									"style" : "default",
									"text" : "OSC-schedule @packetsize 4096 @queuesize 50"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "FullPacket", "FullPacket", "FullPacket" ],
									"patching_rect" : [ 130.0, 95.0, 182.0, 22.0 ],
									"style" : "default",
									"text" : "OSC-schedule @queuesize 100"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 333.0, 82.5, 265.0, 47.0 ],
									"style" : "default",
									"text" : "The queuesize is the number of messages that may be stored in the queue of pending events. Default is 100."
								}

							}
 ],
						"lines" : [  ]
					}
,
					"patching_rect" : [ 370.0, 188.740738, 122.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p queue-size-options"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 4,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 288.0, 79.0, 1043.0, 717.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-1",
									"linecount" : 3,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 698.0, 109.0, 225.0, 47.0 ],
									"style" : "default",
									"text" : "3. this message is scheduled 10 msec in the future; it will emerge from the leftmost outlet 10 msec after receipt."
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-2",
									"maxclass" : "flonum",
									"maximum" : 10.0,
									"minimum" : 0.0,
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 817.0, 257.0, 56.0, 22.0 ],
									"style" : "default",
									"triscale" : 0.9
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 817.0, 232.0, 98.0, 22.0 ],
									"style" : "default",
									"text" : "loadmess 0.01"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 48.0, 115.0, 24.0, 24.0 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 87.0, 570.0, 64.0, 22.0 ],
									"style" : "default",
									"text" : "print bang"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 134.0, 543.0, 66.0, 22.0 ],
									"style" : "default",
									"text" : "print msgs"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 181.0, 514.0, 60.0, 22.0 ],
									"style" : "default",
									"text" : "print time"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "", "", "OSCTimeTag" ],
									"patching_rect" : [ 87.0, 480.0, 113.0, 22.0 ],
									"style" : "default",
									"text" : "OpenSoundControl"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 656.0, 113.0, 24.0, 24.0 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-10",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "bang" ],
									"patching_rect" : [ 656.0, 197.0, 37.0, 22.0 ],
									"style" : "default",
									"text" : "t b b"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-11",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 607.0, 235.0, 95.0, 22.0 ],
									"style" : "default",
									"text" : "/foo/bar 1, bang"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-12",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 709.0, 235.0, 33.0, 22.0 ],
									"style" : "default",
									"text" : "now"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-13",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "OSCTimeTag" ],
									"patching_rect" : [ 709.0, 261.0, 86.0, 22.0 ],
									"style" : "default",
									"text" : "OSC-timetag"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-14",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 353.0, 113.0, 24.0, 24.0 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-15",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "bang" ],
									"patching_rect" : [ 353.0, 164.0, 37.0, 22.0 ],
									"style" : "default",
									"text" : "t b b"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-16",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 257.0, 224.0, 95.0, 22.0 ],
									"style" : "default",
									"text" : "/foo/bar 1, bang"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-17",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 371.0, 198.0, 33.0, 22.0 ],
									"style" : "default",
									"text" : "now"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-18",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "OSCTimeTag" ],
									"patching_rect" : [ 371.0, 232.0, 86.0, 22.0 ],
									"style" : "default",
									"text" : "OSC-timetag"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-19",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "OSCTimeTag" ],
									"patching_rect" : [ 709.0, 290.0, 127.0, 22.0 ],
									"style" : "default",
									"text" : "OSC-timetag @op +"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-20",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 445.0, 473.0, 93.0, 22.0 ],
									"style" : "default",
									"text" : "print immediate"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-21",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 408.0, 500.0, 75.0, 22.0 ],
									"style" : "default",
									"text" : "print missed"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-22",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 371.0, 535.0, 92.0, 22.0 ],
									"style" : "default",
									"text" : "print scheduled"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-23",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 48.0, 147.0, 193.0, 22.0 ],
									"style" : "default",
									"text" : "OSCTimeTag 0 1, /foo/bar 1, bang"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-24",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "", "", "OSCTimeTag" ],
									"patching_rect" : [ 371.0, 347.0, 117.0, 22.0 ],
									"style" : "default",
									"text" : "OpenSoundControl"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-25",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "FullPacket", "FullPacket", "FullPacket" ],
									"patching_rect" : [ 371.0, 436.0, 93.0, 22.0 ],
									"style" : "default",
									"text" : "OSC-schedule"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-26",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 84.0, 115.0, 211.0, 20.0 ],
									"style" : "default",
									"text" : "1. this is an \"immediate\" message"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-27",
									"linecount" : 4,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 404.0, 110.0, 235.0, 60.0 ],
									"style" : "default",
									"text" : "2. this message is stamped with the current time--it will be late by the time OSC-schedule gets it, and therefore goes out the middle outlet (deadline missed)"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-10", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-10", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-24", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-19", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-16", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-24", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-18", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-17", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-24", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-18", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-24", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-19", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-2", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-24", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-23", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-25", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-24", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-20", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-25", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-25", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-25", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-25", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-23", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 371.0, 159.0, 85.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p basic-usage"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"linecount" : 3,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 22.0, 95.0, 457.0, 47.0 ],
					"style" : "default",
					"text" : "Rescheduled messages appear on the leftmost outlet. Messages which have missed the scheduling deadline on the middle, and messages with the immediate tag on the rightmost."
				}

			}
, 			{
				"box" : 				{
					"fontface" : 1,
					"fontsize" : 24.0,
					"id" : "obj-10",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 22.0, 24.0, 175.0, 33.0 ],
					"style" : "",
					"text" : "OSC-schedule"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-12",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 22.0, 69.0, 457.0, 20.0 ],
					"style" : "default",
					"text" : "Schedules delivery of an OSC bundle at a future time using the encoded timestamp."
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-15", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-44", 1 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "OSC-schedule.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "OpenSoundControl.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "OSC-timetag.mxo",
				"type" : "iLaX"
			}
 ],
		"embedsnapshot" : 0
	}

}
