{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 462.0, 44.0, 640.0, 506.0 ],
		"bglocked" : 0,
		"defrect" : [ 462.0, 44.0, 640.0, 506.0 ],
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
					"maxclass" : "newobj",
					"text" : "p foo",
					"patching_rect" : [ 188.0, 373.0, 37.0, 20.0 ],
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-13",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 640.0, 480.0 ],
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
									"text" : "cmmjl_test",
									"patching_rect" : [ 148.0, 155.0, 68.0, 20.0 ],
									"fontname" : "Arial",
									"outlettype" : [ "cmmjl_test", "" ],
									"fontsize" : 12.0,
									"id" : "obj-1",
									"numinlets" : 1,
									"numoutlets" : 2,
									"saved_object_attributes" : 									{
										"OSC-address" : "/cmmjl_test/5"
									}

								}

							}
 ],
						"lines" : [  ]
					}
,
					"saved_object_attributes" : 					{
						"fontname" : "Arial",
						"default_fontface" : 0,
						"fontface" : 0,
						"default_fontname" : "Arial",
						"fontsize" : 12.0,
						"default_fontsize" : 12.0,
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "OSC-address /foo/23",
					"patching_rect" : [ 47.0, 293.0, 123.0, 18.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"id" : "obj-12",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"varname" : "shithole[2]",
					"text" : "cmmjl_test",
					"patching_rect" : [ 375.0, 404.0, 68.0, 20.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "cmmjl_test", "" ],
					"fontsize" : 12.0,
					"id" : "obj-11",
					"numinlets" : 1,
					"numoutlets" : 2,
					"saved_object_attributes" : 					{
						"OSC-address" : "/cmmjl_test/1"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"varname" : "shithole[1]",
					"text" : "cmmjl_test",
					"patching_rect" : [ 118.0, 332.0, 68.0, 20.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "cmmjl_test", "" ],
					"fontsize" : 12.0,
					"id" : "obj-8",
					"numinlets" : 1,
					"numoutlets" : 2,
					"saved_object_attributes" : 					{
						"OSC-address" : "/foo/23"
					}
,
					"frozen_object_attributes" : 					{
						"OSC-address" : "/foo/23"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "error",
					"patching_rect" : [ 328.0, 212.0, 36.0, 18.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"id" : "obj-10",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "error",
					"patching_rect" : [ 238.0, 185.0, 36.0, 18.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"id" : "obj-7",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "seterrorcb",
					"patching_rect" : [ 228.0, 235.0, 65.0, 18.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"id" : "obj-9",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"varname" : "foo",
					"text" : "cmmjl_test",
					"patching_rect" : [ 357.0, 266.0, 68.0, 20.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "cmmjl_test", "" ],
					"fontsize" : 12.0,
					"id" : "obj-6",
					"numinlets" : 1,
					"numoutlets" : 2,
					"saved_object_attributes" : 					{
						"OSC-address" : "/cmmjl_test/3"
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/foo/bar 19, bang",
					"patching_rect" : [ 115.0, 76.0, 101.0, 18.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"id" : "obj-2",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"patching_rect" : [ 205.0, 306.0, 34.0, 20.0 ],
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-5",
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/foo 19, bang",
					"patching_rect" : [ 205.0, 103.0, 81.0, 18.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"id" : "obj-4",
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "OpenSoundControl",
					"patching_rect" : [ 205.0, 131.0, 113.0, 20.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "", "", "OSCTimeTag" ],
					"fontsize" : 12.0,
					"id" : "obj-1",
					"numinlets" : 1,
					"numoutlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"varname" : "shithole",
					"text" : "cmmjl_test",
					"patching_rect" : [ 205.0, 266.0, 68.0, 20.0 ],
					"fontname" : "Arial",
					"outlettype" : [ "cmmjl_test", "" ],
					"fontsize" : 12.0,
					"id" : "obj-3",
					"numinlets" : 1,
					"numoutlets" : 2,
					"saved_object_attributes" : 					{
						"OSC-address" : "/cmmjl_test/4"
					}

				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
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
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
