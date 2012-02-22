{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 215.0, 341.0, 521.0, 296.0 ],
		"bglocked" : 0,
		"defrect" : [ 215.0, 341.0, 521.0, 296.0 ],
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
					"text" : "p basic",
					"fontsize" : 12.0,
					"numinlets" : 0,
					"numoutlets" : 0,
					"id" : "obj-3",
					"fontname" : "Arial",
					"patching_rect" : [ 50.0, 70.0, 49.0, 20.0 ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 532.0, 327.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 532.0, 327.0 ],
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
						"visible" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "print the contents of an OSC bundle to the Max window",
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
									"text" : "o.print ",
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
									"maxclass" : "newobj",
									"text" : "prepend load",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"hidden" : 1,
									"numoutlets" : 1,
									"id" : "obj-4",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 405.0, 261.0, 81.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pcontrol",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"hidden" : 1,
									"numoutlets" : 1,
									"id" : "obj-6",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 405.0, 285.0, 53.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "printit.maxhelp",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-7",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 405.0, 265.0, 89.0, 18.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "see also:",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-27",
									"fontname" : "Arial",
									"patching_rect" : [ 405.0, 241.0, 59.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/foo 1 2. \"bar\" \n",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-8",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 195.0, 135.0, 81.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.print foo",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-9",
									"fontname" : "Arial",
									"patching_rect" : [ 195.0, 165.0, 64.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "o.message",
									"text" : "/foo 1 2. \"bar\" \n",
									"fontsize" : 12.0,
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"numinlets" : 2,
									"numoutlets" : 1,
									"id" : "obj-2",
									"fontname" : "Arial",
									"outlettype" : [ "" ],
									"patching_rect" : [ 53.0, 135.0, 81.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.print",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"id" : "obj-1",
									"fontname" : "Arial",
									"patching_rect" : [ 53.0, 165.0, 44.0, 20.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 1,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-9", 0 ],
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
