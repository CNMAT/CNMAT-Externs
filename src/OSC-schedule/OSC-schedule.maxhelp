{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 202.0, 46.0, 590.0, 444.0 ],
		"bglocked" : 0,
		"defrect" : [ 202.0, 46.0, 590.0, 444.0 ],
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
					"maxclass" : "newobj",
					"text" : "p scheduler-delay-options",
					"fontname" : "Arial",
					"numinlets" : 0,
					"patching_rect" : [ 247.0, 158.0, 127.0, 17.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-1",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 442.0, 108.0, 705.0, 390.0 ],
						"bglocked" : 0,
						"defrect" : [ 442.0, 108.0, 705.0, 390.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "The largest recommended value is 8000 seconds, or about 2 hours to retain millisecond precision.",
									"linecount" : 5,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 80.0, 155.0, 113.0, 58.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-1"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "The maximum delay can be increased, but for values larger than a few hours, the precision of event timing may be degraded by the use of floating point calculations in the MaxMSP scheduler.",
									"linecount" : 3,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 334.0, 148.0, 303.0, 38.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-2"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "By default OSC-schedule will not delay a message more than 1000 seconds into the future.",
									"linecount" : 3,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 333.0, 101.0, 204.0, 38.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-3"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OSC-schedule @maxdelay 8000.",
									"fontname" : "Arial",
									"numinlets" : 0,
									"patching_rect" : [ 70.0, 102.0, 155.0, 17.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-4"
								}

							}
 ],
						"lines" : [  ]
					}
,
					"saved_object_attributes" : 					{
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
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p implementation-details",
					"fontname" : "Arial",
					"numinlets" : 0,
					"patching_rect" : [ 247.0, 179.0, 121.0, 17.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-2",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 385.0, 267.0, 645.0, 394.0 ],
						"bglocked" : 0,
						"defrect" : [ 385.0, 267.0, 645.0, 394.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "http://cnmat.berkeley.edu/publication/implementation_and_applications_open_sound_control_timestamps",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 67.0, 255.0, 521.0, 17.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-1"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "For more details see this paper:",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 67.0, 233.0, 283.0, 17.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-2"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "OSC-schedule can be used to implement forward synchronization or backward resynchronization to coordinate precise timing of events between multiple computers connected by a network with variable delay.",
									"linecount" : 6,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 170.0, 53.0, 185.0, 69.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-3"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "The priority queue is O(log n) for insertion, and O(1) to extract the maximum priority node.",
									"linecount" : 6,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 40.0, 134.0, 92.0, 69.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-4"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Internally OSC-schedule uses a heap priority queue to store messages sorted in time-order.",
									"linecount" : 5,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 40.0, 54.0, 100.0, 58.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-5"
								}

							}
 ],
						"lines" : [  ]
					}
,
					"saved_object_attributes" : 					{
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
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p scheduler-precision-options",
					"fontname" : "Arial",
					"numinlets" : 0,
					"patching_rect" : [ 247.0, 137.0, 144.0, 17.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-3",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 442.0, 108.0, 669.0, 592.0 ],
						"bglocked" : 0,
						"defrect" : [ 442.0, 108.0, 669.0, 592.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "See \"Scheduler Event Interval\" setting under Performance Options.",
									"linecount" : 7,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 377.0, 342.0, 66.0, 79.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-1"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "For optimal performance, the scheduler precision of OSC-schedule should be set to 1.5X the scheduler precision of MaxMSP.",
									"linecount" : 3,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 377.0, 129.0, 207.0, 38.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-2"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OSC-schedule @precision 0.0015",
									"fontname" : "Arial",
									"numinlets" : 0,
									"patching_rect" : [ 82.0, 207.0, 159.0, 17.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-3"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "The default scheduler precision is 3msec corresponding to the MaxMSP default setting of 2mesc precision.",
									"linecount" : 6,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 377.0, 250.0, 100.0, 69.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-4"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Here is a setting of 1.5msec, which is optimal for a scheduler event interval of 1msec.",
									"linecount" : 5,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 81.0, 135.0, 100.0, 58.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-5"
								}

							}
 ],
						"lines" : [  ]
					}
,
					"saved_object_attributes" : 					{
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
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p queue-size-options",
					"fontname" : "Arial",
					"numinlets" : 0,
					"patching_rect" : [ 247.0, 115.0, 103.0, 17.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-4",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 10.0, 59.0, 649.0, 454.0 ],
						"bglocked" : 0,
						"defrect" : [ 10.0, 59.0, 649.0, 454.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Increasing the queue and/or packet size will increase the amount of memory used by OSC-schedule, proportionally.",
									"linecount" : 6,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 168.0, 224.0, 103.0, 69.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-1"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "The packetsize is the maximum length of an OSC message that may be stored, in bytes. The default is 1500.",
									"linecount" : 4,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 356.0, 195.0, 125.0, 48.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-2"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OSC-schedule @packetsize 4096 @queuesize 50",
									"fontname" : "Arial",
									"numinlets" : 0,
									"patching_rect" : [ 63.0, 151.0, 225.0, 17.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-3"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OSC-schedule @queuesize 100",
									"fontname" : "Arial",
									"numinlets" : 0,
									"patching_rect" : [ 64.0, 111.0, 146.0, 17.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-4"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "The queuesize is the number of messages that may be stored in the queue of pending events. Default is 100.",
									"linecount" : 5,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 356.0, 109.0, 100.0, 58.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-5"
								}

							}
 ],
						"lines" : [  ]
					}
,
					"saved_object_attributes" : 					{
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
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "OpenSoundControl",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 325.0, 272.0, 90.0, 17.0 ],
					"numoutlets" : 3,
					"fontsize" : 9.0,
					"outlettype" : [ "", "", "OSCTimeTag" ],
					"id" : "obj-5"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "OSC-timetag",
					"fontname" : "Arial",
					"numinlets" : 0,
					"patching_rect" : [ 247.0, 272.0, 67.0, 17.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p basic-usage",
					"fontname" : "Arial",
					"numinlets" : 0,
					"patching_rect" : [ 248.0, 93.0, 71.0, 17.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-7",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 288.0, 54.0, 1043.0, 717.0 ],
						"bglocked" : 0,
						"defrect" : [ 288.0, 54.0, 1043.0, 717.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "3. this message is scheduled 10 msec in the future;\rit will emerge from the leftmost outlet 10 msec after receipt.",
									"linecount" : 9,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 770.0, 109.0, 75.0, 100.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-1"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"maximum" : 10.0,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 802.0, 256.0, 47.0, 17.0 ],
									"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
									"triscale" : 0.9,
									"numoutlets" : 2,
									"fontsize" : 9.0,
									"outlettype" : [ "float", "bang" ],
									"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
									"minimum" : 0.0,
									"id" : "obj-2"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "loadmess 0.01",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 802.0, 231.0, 75.0, 17.0 ],
									"numoutlets" : 1,
									"fontsize" : 9.0,
									"outlettype" : [ "" ],
									"id" : "obj-3"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 79.0, 115.0, 24.0, 24.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"id" : "obj-4"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print bang",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 87.0, 540.0, 55.0, 17.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-5"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print msgs",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 160.0, 574.0, 58.0, 17.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-6"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print time",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 200.0, 544.0, 55.0, 17.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-7"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OpenSoundControl",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 120.0, 498.0, 90.0, 17.0 ],
									"numoutlets" : 3,
									"fontsize" : 9.0,
									"outlettype" : [ "", "", "OSCTimeTag" ],
									"id" : "obj-8"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 656.0, 113.0, 24.0, 24.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"id" : "obj-9"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 656.0, 197.0, 30.0, 17.0 ],
									"numoutlets" : 2,
									"fontsize" : 9.0,
									"outlettype" : [ "bang", "bang" ],
									"id" : "obj-10"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "/foo/bar 1, bang",
									"fontname" : "Arial",
									"numinlets" : 2,
									"patching_rect" : [ 607.0, 238.0, 87.0, 15.0 ],
									"numoutlets" : 1,
									"fontsize" : 9.0,
									"outlettype" : [ "" ],
									"id" : "obj-11"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "now",
									"fontname" : "Arial",
									"numinlets" : 2,
									"patching_rect" : [ 709.0, 238.0, 26.0, 15.0 ],
									"numoutlets" : 1,
									"fontsize" : 9.0,
									"outlettype" : [ "" ],
									"id" : "obj-12"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OSC-timetag",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 709.0, 261.0, 67.0, 17.0 ],
									"numoutlets" : 1,
									"fontsize" : 9.0,
									"outlettype" : [ "" ],
									"id" : "obj-13"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"patching_rect" : [ 349.0, 112.0, 24.0, 24.0 ],
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"id" : "obj-14"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 349.0, 163.0, 30.0, 17.0 ],
									"numoutlets" : 2,
									"fontsize" : 9.0,
									"outlettype" : [ "bang", "bang" ],
									"id" : "obj-15"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "/foo/bar 1, bang",
									"fontname" : "Arial",
									"numinlets" : 2,
									"patching_rect" : [ 300.0, 204.0, 87.0, 15.0 ],
									"numoutlets" : 1,
									"fontsize" : 9.0,
									"outlettype" : [ "" ],
									"id" : "obj-16"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "now",
									"fontname" : "Arial",
									"numinlets" : 2,
									"patching_rect" : [ 402.0, 204.0, 26.0, 15.0 ],
									"numoutlets" : 1,
									"fontsize" : 9.0,
									"outlettype" : [ "" ],
									"id" : "obj-17"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OSC-timetag",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 402.0, 228.0, 67.0, 17.0 ],
									"numoutlets" : 1,
									"fontsize" : 9.0,
									"outlettype" : [ "" ],
									"id" : "obj-18"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OSC-timetag @op +",
									"fontname" : "Arial",
									"numinlets" : 2,
									"patching_rect" : [ 709.0, 290.0, 97.0, 17.0 ],
									"numoutlets" : 1,
									"fontsize" : 9.0,
									"outlettype" : [ "" ],
									"id" : "obj-19"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print immediate",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 522.0, 543.0, 81.0, 17.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-20"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print missed",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 426.0, 554.0, 66.0, 17.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-21"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "print scheduled",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 315.0, 533.0, 78.0, 17.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-22"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "OSCTimeTag 0 1, /foo/bar 1, bang",
									"fontname" : "Arial",
									"numinlets" : 2,
									"patching_rect" : [ 79.0, 147.0, 168.0, 15.0 ],
									"numoutlets" : 1,
									"fontsize" : 9.0,
									"outlettype" : [ "" ],
									"id" : "obj-23"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OpenSoundControl",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 371.0, 347.0, 90.0, 17.0 ],
									"numoutlets" : 3,
									"fontsize" : 9.0,
									"outlettype" : [ "", "", "OSCTimeTag" ],
									"id" : "obj-24"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OSC-schedule",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 371.0, 436.0, 71.0, 17.0 ],
									"numoutlets" : 3,
									"fontsize" : 9.0,
									"outlettype" : [ "", "", "" ],
									"id" : "obj-25"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "1. this is an \"immediate\" message",
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 115.0, 115.0, 160.0, 17.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-26"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "2. this message is stamped with the current time--it will be late by the time OSC-schedule gets it, and therefore goes out the middle outlet (deadline missed)",
									"linecount" : 8,
									"fontname" : "Arial",
									"numinlets" : 1,
									"patching_rect" : [ 483.0, 112.0, 100.0, 89.0 ],
									"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numoutlets" : 0,
									"fontsize" : 9.0,
									"id" : "obj-27"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-19", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 1 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-25", 2 ],
									"destination" : [ "obj-20", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-25", 1 ],
									"destination" : [ "obj-21", 0 ],
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
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 1 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-24", 0 ],
									"destination" : [ "obj-25", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-23", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-24", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-25", 0 ],
									"destination" : [ "obj-22", 0 ],
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
									"source" : [ "obj-8", 2 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 1 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-25", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-23", 0 ],
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
						"default_fontsize" : 12.0,
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
					"text" : "Rescheduled messages appear on the leftmost outlet. Messages which have missed the scheduling deadline on the middle, and messages with the immediate tag on the rightmost.",
					"linecount" : 10,
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 48.0, 123.0, 98.0, 110.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-8"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "OSC-schedule by Andy Schmeder <andy@cnmat.berkeley.edu>",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 52.0, 321.0, 458.0, 17.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-9"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "OSC-schedule",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 38.0, 43.0, 100.0, 17.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-10"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Copyright 2008 UC Regents. All rights reserved.",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 52.0, 339.0, 303.0, 17.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-11"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Schedules delivery of an OSC bundle at a future time using the encoded timestamp.",
					"linecount" : 4,
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 48.0, 70.0, 100.0, 48.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-12"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "See also:",
					"fontname" : "Arial",
					"numinlets" : 1,
					"patching_rect" : [ 247.0, 254.0, 100.0, 17.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numoutlets" : 0,
					"fontsize" : 9.0,
					"id" : "obj-13"
				}

			}
 ],
		"lines" : [  ]
	}

}
