{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 90.0, 112.0, 722.0, 329.0 ],
		"bglocked" : 0,
		"defrect" : [ 90.0, 112.0, 722.0, 329.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p \"similar objects\"",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 0,
					"presentation_rect" : [ 56.0, 90.0, 0.0, 0.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"patching_rect" : [ 56.0, 88.0, 105.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 518.0, 163.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 518.0, 163.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 14.0,
						"default_fontface" : 0,
						"default_fontname" : "Andale Mono",
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
									"text" : "prepend",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-6",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 195.0, 40.0, 55.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "o.prepend is similar to",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"presentation_rect" : [ 69.0, 38.0, 0.0, 0.0 ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"patching_rect" : [ 70.0, 40.0, 127.0, 20.0 ]
								}

							}
 ],
						"lines" : [  ]
					}
,
					"saved_object_attributes" : 					{
						"fontsize" : 14.0,
						"default_fontname" : "Andale Mono",
						"default_fontsize" : 14.0,
						"fontname" : "Andale Mono",
						"globalpatchername" : "",
						"default_fontface" : 0,
						"fontface" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p basic",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 0,
					"id" : "obj-1",
					"fontname" : "Arial",
					"patching_rect" : [ 56.0, 48.0, 49.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 620.0, 335.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 620.0, 335.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 14.0,
						"default_fontface" : 0,
						"default_fontname" : "Andale Mono",
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
									"maxclass" : "comment",
									"text" : "prepend /foo to every address in the incoming bundle",
									"linecount" : 3,
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-5",
									"fontname" : "Arial",
									"patching_rect" : [ 136.0, 159.0, 120.0, 48.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "o.prepend converts OSC-style Max messages into bundles before outputting.",
									"linecount" : 5,
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-2",
									"fontname" : "Arial",
									"patching_rect" : [ 448.0, 183.0, 115.0, 75.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Prepend a given namespace on any input address",
									"fontsize" : 12.0,
									"textcolor" : [ 0.49728, 0.49728, 0.49728, 1.0 ],
									"numinlets" : 1,
									"numoutlets" : 0,
									"frgb" : [ 0.49728, 0.49728, 0.49728, 1.0 ],
									"id" : "obj-12",
									"fontname" : "Arial",
									"patching_rect" : [ 15.0, 77.0, 328.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "o.prepend ",
									"fontsize" : 48.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-11",
									"fontname" : "Arial",
									"patching_rect" : [ 15.0, 15.0, 240.0, 62.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "prepend /foo to /bar.  ",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-30",
									"fontname" : "Arial",
									"patching_rect" : [ 352.0, 155.0, 127.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-29",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 352.0, 251.0, 91.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "/foo /bar 7 8 9",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-28",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 352.0, 183.0, 85.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.prepend",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-27",
									"fontname" : "Arial",
									"outlettype" : [ "FullPacket" ],
									"patching_rect" : [ 352.0, 215.0, 65.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-23",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 56.0, 255.0, 91.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/bar 1 2 3 \n/bloo 4 5 6 \n",
									"linecount" : 2,
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-22",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 56.0, 159.0, 72.0, 34.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.prepend /foo",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-21",
									"fontname" : "Arial",
									"outlettype" : [ "FullPacket" ],
									"patching_rect" : [ 56.0, 220.0, 88.0, 20.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-27", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-27", 0 ],
									"destination" : [ "obj-29", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-22", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-23", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontsize" : 14.0,
						"default_fontname" : "Andale Mono",
						"default_fontsize" : 14.0,
						"fontname" : "Andale Mono",
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
