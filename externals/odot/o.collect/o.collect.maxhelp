{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 100.0, 100.0, 664.0, 409.0 ],
		"bglocked" : 0,
		"defrect" : [ 100.0, 100.0, 664.0, 409.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 10.0, 10.0 ],
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
					"varname" : "basic_tab",
					"text" : "p basic",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 0,
					"id" : "obj-3",
					"fontname" : "Arial",
					"patching_rect" : [ 66.0, 82.0, 49.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 141.0, 270.0, 577.0, 381.0 ],
						"bglocked" : 0,
						"defrect" : [ 141.0, 270.0, 577.0, 381.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 8.0, 8.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"visible" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "accumulate messages and bundles into a single bundle (to be sent out via a bang)",
									"fontsize" : 12.0,
									"textcolor" : [ 0.49728, 0.49728, 0.49728, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"frgb" : [ 0.49728, 0.49728, 0.49728, 1.0 ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"patching_rect" : [ 16.0, 78.0, 452.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "o.collect ",
									"fontsize" : 48.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-2",
									"fontname" : "Arial",
									"patching_rect" : [ 16.0, 16.0, 519.0, 62.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "messages in the resulting bundle are in the order they were received",
									"linecount" : 2,
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-23",
									"fontname" : "Arial",
									"patching_rect" : [ 234.0, 276.0, 288.0, 34.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "accepts OSC-style Max messages and OSC bundles",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-22",
									"fontname" : "Arial",
									"patching_rect" : [ 234.0, 156.0, 318.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "bang triggers output and clears the internal buffer",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-21",
									"fontname" : "Arial",
									"patching_rect" : [ 234.0, 216.0, 272.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-15",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 114.0, 276.0, 80.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-14",
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 204.0, 216.0, 20.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/bar 456 \n",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-12",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 174.0, 156.0, 59.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "/foo 123",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-11",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 114.0, 156.0, 54.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.collect",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-5",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 114.0, 246.0, 82.0, 20.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-15", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p \"see also\"",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 0,
					"id" : "obj-2",
					"fontname" : "Arial",
					"patching_rect" : [ 66.0, 118.0, 74.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 0.0, 44.0, 664.0, 383.0 ],
						"bglocked" : 0,
						"defrect" : [ 0.0, 44.0, 664.0, 383.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 8.0, 8.0 ],
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
									"text" : "bag",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-4",
									"fontname" : "Arial",
									"outlettype" : [ "int" ],
									"patching_rect" : [ 280.25, 221.0, 32.5, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl group",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 2,
									"id" : "obj-19",
									"fontname" : "Arial",
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 270.0, 189.0, 53.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OpenSoundControl",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 3,
									"id" : "obj-18",
									"fontname" : "Arial",
									"outlettype" : [ "", "", "OSCTimeTag" ],
									"patching_rect" : [ 240.0, 162.0, 113.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "o.collect collects OSC messages in a buffer and outputs them when it receives a bang, similar to these objects:",
									"linecount" : 2,
									"fontsize" : 14.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-17",
									"fontname" : "Arial",
									"patching_rect" : [ 101.0, 88.0, 394.0, 39.0 ]
								}

							}
 ],
						"lines" : [  ]
					}
,
					"saved_object_attributes" : 					{
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0
					}

				}

			}
 ],
		"lines" : [  ]
	}

}
