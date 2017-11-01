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
		"rect" : [ 100.0, 100.0, 804.0, 581.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 10.0, 10.0 ],
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
					"id" : "obj-2",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 427.0, 259.0, 24.0, 24.0 ],
					"style" : "default"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 519.0, 276.0, 118.0, 33.0 ],
					"style" : "default",
					"text" : "fill shorter lists with zeros (see example)"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-4",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 427.0, 288.0, 77.0, 22.0 ],
					"style" : "default",
					"text" : "zeropad $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-5",
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
						"rect" : [ 65.0, 79.0, 1265.0, 529.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 10.0, 10.0 ],
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
									"patching_rect" : [ 670.0, 338.0, 88.0, 47.0 ],
									"style" : "default",
									"text" : "creates change here -->"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 649.0, 164.0, 107.0, 20.0 ],
									"style" : "default",
									"text" : "change here -->"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-3",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 653.5, 48.0, 284.0, 33.0 ],
									"style" : "default",
									"text" : "The last list received sets the length. The only thing different in this example is the order of the lists."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 20.0, 432.0, 74.0, 20.0 ],
									"style" : "default",
									"text" : "example 2:"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 784.0, 288.0, 309.0, 22.0 ],
									"style" : "default",
									"text" : "1. 2. 3."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "bang", "bang", "bang" ],
									"patching_rect" : [ 760.0, 118.0, 44.0, 22.0 ],
									"style" : "default",
									"text" : "t b b b"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 760.0, 88.0, 60.0, 22.0 ],
									"style" : "default",
									"text" : "loadbang"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-9",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 888.0, 151.0, 69.0, 22.0 ],
									"style" : "default",
									"text" : "zeropad 1"
								}

							}
, 							{
								"box" : 								{
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"id" : "obj-10",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 760.0, 324.0, 135.0, 93.0 ],
									"setminmax" : [ 1.0, 4.0 ],
									"setstyle" : 1,
									"size" : 3,
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-11",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 833.0, 164.0, 54.0, 22.0 ],
									"style" : "default",
									"text" : "4. 3. 2."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-12",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 760.0, 188.0, 71.0, 22.0 ],
									"style" : "default",
									"text" : "1. 2. 3. 4."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-13",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "list", "bang" ],
									"patching_rect" : [ 760.0, 257.0, 101.0, 22.0 ],
									"style" : "default",
									"text" : "list-interpolate"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-14",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 784.0, 223.0, 77.0, 20.0 ],
									"style" : "",
									"text" : "<- move ->",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"contdata" : 1,
									"id" : "obj-15",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"orientation" : 0,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 775.0, 215.0, 81.0, 36.0 ],
									"setminmax" : [ 0.0, 1.0 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-16",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 326.0, 67.0, 71.0, 20.0 ],
									"style" : "default",
									"text" : "without:"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-17",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 343.0, 288.0, 293.0, 22.0 ],
									"style" : "default",
									"text" : "1. 2. 3. 4."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-19",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "bang", "bang", "bang" ],
									"patching_rect" : [ 319.0, 122.0, 44.0, 22.0 ],
									"style" : "default",
									"text" : "t b b b"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-20",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 319.0, 93.0, 60.0, 22.0 ],
									"style" : "default",
									"text" : "loadbang"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-21",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 455.0, 151.0, 69.0, 22.0 ],
									"style" : "default",
									"text" : "zeropad 0"
								}

							}
, 							{
								"box" : 								{
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"id" : "obj-22",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 319.0, 319.0, 135.0, 93.0 ],
									"setminmax" : [ 1.0, 4.0 ],
									"setstyle" : 1,
									"size" : 4,
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-23",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 400.0, 164.0, 54.0, 22.0 ],
									"style" : "default",
									"text" : "4. 3. 2."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-24",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 319.0, 183.0, 71.0, 22.0 ],
									"style" : "default",
									"text" : "1. 2. 3. 4."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-25",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "list", "bang" ],
									"patching_rect" : [ 319.0, 252.0, 101.0, 22.0 ],
									"style" : "default",
									"text" : "list-interpolate"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-26",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 343.0, 217.0, 77.0, 20.0 ],
									"style" : "",
									"text" : "<- move ->",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"contdata" : 1,
									"id" : "obj-27",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"orientation" : 0,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 334.0, 210.0, 81.0, 36.0 ],
									"setminmax" : [ 0.0, 1.0 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-28",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 31.0, 67.0, 35.0, 20.0 ],
									"style" : "default",
									"text" : "with:"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-29",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 39.0, 294.0, 221.0, 22.0 ],
									"style" : "default",
									"text" : "4. 3. 2. 0."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-31",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "bang", "bang", "bang" ],
									"patching_rect" : [ 24.0, 122.0, 44.0, 22.0 ],
									"style" : "default",
									"text" : "t b b b"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-32",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 24.0, 93.0, 60.0, 22.0 ],
									"style" : "default",
									"text" : "loadbang"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-33",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 134.0, 151.0, 69.0, 22.0 ],
									"style" : "default",
									"text" : "zeropad 1"
								}

							}
, 							{
								"box" : 								{
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"id" : "obj-34",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 24.0, 330.0, 135.0, 93.0 ],
									"setminmax" : [ 1.0, 4.0 ],
									"setstyle" : 1,
									"size" : 4,
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-35",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 79.0, 164.0, 54.0, 22.0 ],
									"style" : "default",
									"text" : "4. 3. 2."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-36",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 24.0, 183.0, 71.0, 22.0 ],
									"style" : "default",
									"text" : "1. 2. 3. 4."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-37",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "list", "bang" ],
									"patching_rect" : [ 24.0, 252.0, 101.0, 22.0 ],
									"style" : "default",
									"text" : "list-interpolate"
								}

							}
, 							{
								"box" : 								{
									"fontsize" : 16.0,
									"id" : "obj-38",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 18.5, 6.0, 91.0, 24.0 ],
									"style" : "",
									"text" : "zeropad"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-39",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 18.5, 32.0, 364.0, 33.0 ],
									"style" : "default",
									"text" : "If your lists are of different lengths, you can fill up the shorter lists with zeros by sending list-interpolate the message \"zeropad 1.\""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-40",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 41.0, 217.0, 77.0, 20.0 ],
									"style" : "",
									"text" : "<- move ->",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"contdata" : 1,
									"id" : "obj-41",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"orientation" : 0,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 39.0, 210.0, 81.0, 36.0 ],
									"setminmax" : [ 0.0, 1.0 ],
									"style" : "default"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 842.5, 210.0, 769.5, 210.0 ],
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 769.5, 210.0, 769.5, 210.0 ],
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 784.5, 254.0, 769.5, 254.0 ],
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-23", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 1 ]
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
									"destination" : [ "obj-19", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-20", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-25", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 464.5, 205.0, 328.5, 205.0 ],
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-25", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 409.5, 205.0, 328.5, 205.0 ],
									"source" : [ "obj-23", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-25", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 328.5, 205.0, 328.5, 205.0 ],
									"source" : [ "obj-24", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-25", 0 ]
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
									"destination" : [ "obj-25", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 343.5, 249.0, 328.5, 249.0 ],
									"source" : [ "obj-27", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-33", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-31", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-35", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-31", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-36", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-31", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-31", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-32", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-37", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 143.5, 205.0, 33.5, 205.0 ],
									"source" : [ "obj-33", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-37", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 88.5, 205.0, 33.5, 205.0 ],
									"source" : [ "obj-35", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-37", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 33.5, 205.0, 33.5, 205.0 ],
									"source" : [ "obj-36", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-29", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-37", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-34", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-37", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-37", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 48.5, 249.0, 33.5, 249.0 ],
									"source" : [ "obj-41", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-9", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 897.5, 210.0, 769.5, 210.0 ],
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 647.0, 440.0, 69.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p zeropad"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 289.0, 404.0, 221.0, 20.0 ],
					"style" : "default",
					"text" : "(note this can be greater than 256)"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 289.0, 391.0, 226.0, 20.0 ],
					"style" : "default",
					"text" : "second: maximum length of list"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 289.0, 378.0, 224.0, 20.0 ],
					"style" : "default",
					"text" : "first: number of steps"
				}

			}
, 			{
				"box" : 				{
					"data" : [ 593, "png", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ],
					"embed" : 1,
					"id" : "obj-13",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "jit_matrix" ],
					"patching_rect" : [ 272.0, 32.0, 109.0, 48.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-16",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 52.0, 60.0, 203.0, 20.0 ],
					"style" : "default",
					"text" : "linear interpolation between two lists"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 1,
					"fontsize" : 24.0,
					"id" : "obj-17",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 52.0, 32.0, 176.0, 33.0 ],
					"style" : "",
					"text" : "list-interpolate"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-19",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 643.0, 357.0, 70.0, 20.0 ],
					"style" : "default",
					"text" : "examples:"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-20",
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
						"rect" : [ 60.0, 79.0, 661.0, 566.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 10.0, 10.0 ],
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
									"maxclass" : "led",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 302.0, 274.0, 17.0, 17.0 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-2",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 360.0, 154.0, 33.0, 17.0 ],
									"style" : "",
									"text" : "steps",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-3",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 351.0, 128.0, 53.0, 17.0 ],
									"style" : "",
									"text" : "time (ms)",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-4",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 273.0, 152.0, 33.0, 17.0 ],
									"style" : "",
									"text" : "steps",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-5",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 279.0, 139.0, 17.0, 17.0 ],
									"style" : "",
									"text" : "of",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-6",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 269.0, 126.0, 42.0, 17.0 ],
									"style" : "",
									"text" : "number",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.0 ],
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-7",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 355.0, 141.0, 45.0, 17.0 ],
									"style" : "",
									"text" : "between",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.611765, 0.701961, 1.0, 1.0 ],
									"id" : "obj-8",
									"knobcolor" : [ 0.2, 0.2, 0.2, 1.0 ],
									"maxclass" : "slider",
									"min" : 1.0,
									"numinlets" : 1,
									"numoutlets" : 1,
									"orientation" : 2,
									"outlettype" : [ "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 345.0, 98.0, 67.0, 99.0 ],
									"size" : 100.0,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.6, 0.6, 1.0, 1.0 ],
									"id" : "obj-9",
									"knobcolor" : [ 0.2, 0.2, 0.2, 1.0 ],
									"maxclass" : "slider",
									"numinlets" : 1,
									"numoutlets" : 1,
									"orientation" : 2,
									"outlettype" : [ "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 254.0, 98.0, 67.0, 99.0 ],
									"size" : 300.0,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-10",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 26.0, 113.0, 32.0, 17.0 ],
									"style" : "",
									"text" : "boom",
									"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-11",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 18.0, 98.0, 45.0, 45.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-12",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 18.0, 151.0, 44.0, 22.0 ],
									"style" : "default",
									"text" : "del 10"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-13",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "int", "int" ],
									"patching_rect" : [ 254.0, 65.0, 54.0, 22.0 ],
									"style" : "default",
									"text" : "t 200 10"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-14",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 254.0, 39.0, 60.0, 22.0 ],
									"style" : "default",
									"text" : "loadbang"
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.972549, 0.27451, 0.0, 1.0 ],
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"contdata" : 1,
									"id" : "obj-15",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 220.0, 300.0, 169.0, 99.0 ],
									"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
									"setminmax" : [ 0.0, 1.0 ],
									"setstyle" : 1,
									"size" : 253,
									"slidercolor" : [ 0.937255, 1.0, 0.168627, 1.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-16",
									"maxclass" : "preset",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "preset", "int", "preset", "int" ],
									"patching_rect" : [ 74.0, 47.0, 99.0, 17.0 ],
									"preset_data" : [ 										{
											"number" : 1,
											"data" : [ 257, "obj-20", "multislider", "list", 0.196429, 0.199176, 0.201923, 0.20467, 0.207418, 0.210165, 0.212912, 0.215659, 0.218407, 0.221154, 0.223901, 0.226648, 0.229396, 0.232143, 0.236607, 0.241071, 0.245536, 0.25, 0.254464, 0.258929, 0.263393, 0.267857, 0.275974, 0.284091, 0.292208, 0.300325, 0.308442, 0.316558, 0.324675, 0.332792, 0.340909, 0.349026, 0.357143, 0.372449, 0.387755, 0.403061, 0.418367, 0.433673, 0.44898, 0.464286, 0.473214, 0.482143, 0.5, 0.517857, 0.526786, 0.535714, 0.540179, 0.544643, 0.549107, 0.553571, 0.550325, 0.547078, 0.543831, 0.540584, 0.537338, 0.534091, 0.530844, 0.527597, 0.524351, 0.521104, 0.517857, 0.507937, 0.498016, 0.488095, 0.478175, 0.468254, 0.458333, 0.448413, 0.438492, 0.428571, 0.41369, 0.39881, 0.383929, 0.369048, 0.354167, 0.339286, 0.332143, 0.325, 0.317857, 0.310714, 0.303571, 0.309524, 0.315476, 0.321429, 0.327381, 0.333333, 0.339286, 0.349026, 0.358766, 0.368506, 0.378247, 0.387987, 0.397727, 0.407468, 0.417208, 0.426948, 0.436688, 0.446429, 0.454545, 0.462662, 0.470779, 0.478896, 0.487013, 0.49513, 0.503247, 0.511364, 0.519481, 0.527597, 0.553571, 0.553571, 0.553571, 0.553571, 0.553571, 0.549107, 0.544643, 0.540179, 0.535714, 0.53125, 0.526786, 0.522321, 0.517857, 0.513393, 0.508929, 0.504464, 0.5, 0.495536, 0.491071, 0.486607, 0.482143, 0.475446, 0.46875, 0.462054, 0.455357, 0.448661, 0.441964, 0.435268, 0.428571, 0.417857, 0.407143, 0.396429, 0.385714, 0.375, 0.397321, 0.419643, 0.441964, 0.464286, 0.473214, 0.482143, 0.491071, 0.5, 0.508929, 0.517857, 0.52381, 0.529762, 0.535714, 0.535714, 0.535714, 0.535714, 0.535714, 0.535714, 0.535714, 0.53125, 0.526786, 0.522321, 0.517857, 0.507937, 0.498016, 0.488095, 0.478175, 0.468254, 0.458333, 0.448413, 0.438492, 0.428571, 0.418367, 0.408163, 0.397959, 0.387755, 0.377551, 0.367347, 0.357143, 0.348214, 0.339286, 0.34375, 0.348214, 0.352679, 0.357143, 0.362245, 0.367347, 0.372449, 0.377551, 0.382653, 0.387755, 0.392857, 0.39881, 0.404762, 0.410714, 0.416667, 0.422619, 0.428571, 0.4375, 0.446429, 0.446429, 0.446429, 0.446429, 0.446429, 0.446429, 0.4375, 0.428571, 0.416667, 0.404762, 0.392857, 0.380952, 0.369048, 0.357143, 0.339286, 0.321429, 0.310714, 0.3, 0.289286, 0.278571, 0.267857, 0.254464, 0.241071, 0.227679, 0.196429, 0.1875, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.1875, 0.196429, 0.205357, 0.214286, 0.214286, 0.214286, 0.223214, 0.232143, 0.232143, 0.232143, 0.232143, 0.232143, 0.232143, 0.232143, 0.232143, 0.232143, 0.232143, 0.223214 ]
										}
, 										{
											"number" : 2,
											"data" : [ 257, "obj-20", "multislider", "list", 0.160714, 0.160714, 0.160714, 0.160714, 0.160714, 0.165179, 0.169643, 0.174107, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.178571, 0.1875, 0.196429, 0.197321, 0.198214, 0.199107, 0.2, 0.200893, 0.201786, 0.202679, 0.203571, 0.204464, 0.205357, 0.20625, 0.207143, 0.208036, 0.208929, 0.209821, 0.210714, 0.211607, 0.2125, 0.213393, 0.214286, 0.220238, 0.22619, 0.232143, 0.238095, 0.244048, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.258929, 0.267857, 0.267857, 0.267857, 0.271825, 0.275794, 0.279762, 0.28373, 0.287698, 0.291667, 0.295635, 0.299603, 0.303571, 0.308036, 0.3125, 0.316964, 0.321429, 0.321429, 0.321429, 0.326531, 0.331633, 0.336735, 0.341837, 0.346939, 0.352041, 0.357143, 0.366071, 0.375, 0.377976, 0.380952, 0.383929, 0.386905, 0.389881, 0.392857, 0.392857, 0.392857, 0.392857, 0.397321, 0.401786, 0.40625, 0.410714, 0.410714, 0.410714, 0.410714, 0.410714, 0.414286, 0.417857, 0.421429, 0.425, 0.428571, 0.428571, 0.428571, 0.428571, 0.428571, 0.428571, 0.428571, 0.4375, 0.446429, 0.446429, 0.446429, 0.446429, 0.446429, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.464286, 0.466837, 0.469388, 0.471939, 0.47449, 0.477041, 0.479592, 0.482143, 0.486607, 0.491071, 0.495536, 0.5, 0.501984, 0.503968, 0.505952, 0.507937, 0.509921, 0.511905, 0.513889, 0.515873, 0.517857, 0.520833, 0.52381, 0.526786, 0.529762, 0.532738, 0.535714, 0.540179, 0.544643, 0.549107, 0.553571, 0.559524, 0.565476, 0.571429, 0.580357, 0.589286, 0.598214, 0.607143, 0.613095, 0.619048, 0.625, 0.630952, 0.636905, 0.642857, 0.64881, 0.654762, 0.660714, 0.660714, 0.660714, 0.665179, 0.669643, 0.674107, 0.678571, 0.681122, 0.683673, 0.686224, 0.688776, 0.691327, 0.693878, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.696429, 0.705357, 0.714286, 0.714286, 0.714286, 0.712302, 0.710317, 0.708333, 0.706349, 0.704365, 0.702381, 0.700397, 0.698413, 0.696429, 0.69246, 0.688492, 0.684524, 0.680556, 0.676587, 0.672619, 0.668651, 0.664683, 0.660714, 0.660714, 0.660714, 0.65625, 0.651786, 0.647321, 0.642857, 0.637755, 0.632653, 0.627551, 0.622449, 0.617347, 0.612245, 0.607143, 0.607143, 0.607143, 0.604167, 0.60119, 0.598214 ]
										}
, 										{
											"number" : 3,
											"data" : [ 257, "obj-20", "multislider", "list", 0.0, 0.0, 0.0, 0.0, 0.0, 0.026786, 0.035714, 0.040179, 0.044643, 0.049107, 0.053571, 0.060714, 0.067857, 0.075, 0.082143, 0.089286, 0.098214, 0.107143, 0.116071, 0.125, 0.133929, 0.142857, 0.151786, 0.160714, 0.196429, 0.196429, 0.203571, 0.210714, 0.217857, 0.225, 0.232143, 0.235902, 0.239662, 0.243421, 0.24718, 0.25094, 0.254699, 0.258459, 0.262218, 0.265977, 0.269737, 0.273496, 0.277256, 0.281015, 0.284774, 0.288534, 0.292293, 0.296053, 0.299812, 0.303571, 0.306677, 0.309783, 0.312888, 0.315994, 0.319099, 0.322205, 0.325311, 0.328416, 0.331522, 0.334627, 0.337733, 0.340839, 0.343944, 0.34705, 0.350155, 0.353261, 0.356366, 0.359472, 0.362578, 0.365683, 0.368789, 0.371894, 0.375, 0.375, 0.375, 0.375, 0.366071, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.357143, 0.366071, 0.375, 0.379464, 0.383929, 0.388393, 0.392857, 0.397321, 0.401786, 0.40625, 0.410714, 0.428571, 0.446429, 0.464286, 0.482143, 0.5, 0.517857, 0.535714, 0.549107, 0.5625, 0.575893, 0.589286, 0.616071, 0.642857, 0.652597, 0.662338, 0.672078, 0.681818, 0.691558, 0.701299, 0.711039, 0.720779, 0.730519, 0.74026, 0.75, 0.758929, 0.767857, 0.776786, 0.785714, 0.790966, 0.796219, 0.801471, 0.806723, 0.811975, 0.817227, 0.822479, 0.827731, 0.832983, 0.838235, 0.843487, 0.84874, 0.853992, 0.859244, 0.864496, 0.869748, 0.875, 0.875, 0.875, 0.875, 0.875, 0.875, 0.875, 0.875, 0.875, 0.875, 0.830357, 0.821429, 0.785714, 0.75, 0.705357, 0.660714, 0.635204, 0.609694, 0.584184, 0.558673, 0.533163, 0.507653, 0.482143, 0.467857, 0.453571, 0.439286, 0.425, 0.410714, 0.396429, 0.382143, 0.367857, 0.353571, 0.339286, 0.332589, 0.325893, 0.319196, 0.3125, 0.305804, 0.299107, 0.292411, 0.285714, 0.279018, 0.272321, 0.265625, 0.258929, 0.252232, 0.245536, 0.238839, 0.232143, 0.225446, 0.21875, 0.212054, 0.205357, 0.198661, 0.191964, 0.185268, 0.178571, 0.17437, 0.170168, 0.165966, 0.161765, 0.157563, 0.153361, 0.14916, 0.144958, 0.140756, 0.136555, 0.132353, 0.128151, 0.12395, 0.119748, 0.115546, 0.111345, 0.107143, 0.102941, 0.098739, 0.094538, 0.090336, 0.086134, 0.081933, 0.077731, 0.073529, 0.069328, 0.065126, 0.060924, 0.056723, 0.052521, 0.048319, 0.044118, 0.039916, 0.035714, 0.035714, 0.035714 ]
										}
, 										{
											"number" : 4,
											"data" : [ 257, "obj-20", "multislider", "list", 0.160714, 0.160714, 0.982143, 0.972619, 0.963095, 0.953571, 0.944048, 0.934524, 0.925, 0.915476, 0.905952, 0.896429, 0.886905, 0.877381, 0.867857, 0.858333, 0.84881, 0.839286, 0.830745, 0.822205, 0.813665, 0.805124, 0.796584, 0.788043, 0.779503, 0.770963, 0.762422, 0.753882, 0.745342, 0.736801, 0.728261, 0.71972, 0.71118, 0.70264, 0.694099, 0.685559, 0.677019, 0.668478, 0.659938, 0.651398, 0.642857, 0.636161, 0.629464, 0.622768, 0.616071, 0.609375, 0.602679, 0.595982, 0.589286, 0.582589, 0.575893, 0.569196, 0.5625, 0.555804, 0.549107, 0.542411, 0.535714, 0.529018, 0.522321, 0.515625, 0.508929, 0.502232, 0.495536, 0.488839, 0.482143, 0.475446, 0.46875, 0.462054, 0.455357, 0.448661, 0.441964, 0.435268, 0.428571, 0.422321, 0.416071, 0.409821, 0.403571, 0.397321, 0.391071, 0.384821, 0.378571, 0.372321, 0.366071, 0.359821, 0.353571, 0.347321, 0.341071, 0.334821, 0.328571, 0.322321, 0.316071, 0.309821, 0.303571, 0.294643, 0.285714, 0.276786, 0.267857, 0.258929, 0.25, 0.241071, 0.232143, 0.223214, 0.196429, 0.185714, 0.175, 0.164286, 0.153571, 0.142857, 0.138393, 0.133929, 0.129464, 0.125, 0.116071, 0.089286, 0.084821, 0.080357, 0.075893, 0.071429, 0.071429, 0.071429, 0.071429, 0.066071, 0.060714, 0.055357, 0.05, 0.044643, 0.039286, 0.033929, 0.028571, 0.023214, 0.017857, 0.02551, 0.033163, 0.040816, 0.048469, 0.056122, 0.063776, 0.071429, 0.077731, 0.084034, 0.090336, 0.096639, 0.102941, 0.109244, 0.115546, 0.121849, 0.128151, 0.134454, 0.140756, 0.147059, 0.153361, 0.159664, 0.165966, 0.172269, 0.178571, 0.184006, 0.189441, 0.194876, 0.200311, 0.205745, 0.21118, 0.216615, 0.22205, 0.227484, 0.232919, 0.238354, 0.243789, 0.249224, 0.254658, 0.260093, 0.265528, 0.270963, 0.276398, 0.281832, 0.287267, 0.292702, 0.298137, 0.303571, 0.310714, 0.317857, 0.325, 0.332143, 0.339286, 0.346429, 0.353571, 0.360714, 0.367857, 0.375, 0.382143, 0.389286, 0.396429, 0.403571, 0.410714, 0.416667, 0.422619, 0.428571, 0.434524, 0.440476, 0.446429, 0.452381, 0.458333, 0.464286, 0.472222, 0.480159, 0.488095, 0.496032, 0.503968, 0.511905, 0.519841, 0.527778, 0.535714, 0.549107, 0.5625, 0.575893, 0.589286, 0.602679, 0.616071, 0.629464, 0.642857, 0.651786, 0.678571, 0.678571, 0.714286, 0.720238, 0.72619, 0.75, 0.758929, 0.767857, 0.776786, 0.785714, 0.794643, 0.803571, 0.821429, 0.839286, 0.848214, 0.857143, 0.862245, 0.867347, 0.872449, 0.877551, 0.882653, 0.887755, 0.892857, 0.910714, 0.928571, 0.928571, 0.946429, 0.955357, 0.964286, 0.973214, 0.982143, 0.991071, 1.0 ]
										}
, 										{
											"number" : 9,
											"data" : [ 257, "obj-20", "multislider", "list", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ]
										}
 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-17",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 49.0, 201.0, 43.0, 22.0 ],
									"style" : "default",
									"text" : "zl reg"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-18",
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 345.0, 207.0, 40.0, 22.0 ],
									"style" : "default",
									"triscale" : 0.9
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-19",
									"maxclass" : "number",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 254.0, 209.0, 41.0, 22.0 ],
									"style" : "default",
									"triscale" : 0.9
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.031373, 0.003922, 0.509804, 1.0 ],
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"contdata" : 1,
									"id" : "obj-20",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 74.0, 98.0, 169.0, 99.0 ],
									"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
									"setminmax" : [ 0.0, 1.0 ],
									"setstyle" : 1,
									"size" : 253,
									"slidercolor" : [ 0.615686, 0.776471, 1.0, 1.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-21",
									"maxclass" : "newobj",
									"numinlets" : 3,
									"numoutlets" : 2,
									"outlettype" : [ "list", "bang" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 0,
											"revision" : 4,
											"architecture" : "x64",
											"modernui" : 1
										}
,
										"rect" : [ 60.0, 79.0, 608.0, 499.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 10.0, 10.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"comment" : "done",
													"id" : "obj-1",
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 133.0, 395.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-2",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"patching_rect" : [ 137.0, 119.0, 60.0, 22.0 ],
													"style" : "default",
													"text" : "loadbang"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-3",
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 67.0, 377.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-4",
													"maxclass" : "toggle",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"parameter_enable" : 0,
													"patching_rect" : [ 184.0, 212.0, 24.0, 24.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-5",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "bang", "int" ],
													"patching_rect" : [ 133.0, 343.0, 37.0, 22.0 ],
													"style" : "default",
													"text" : "t b 0"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-6",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "int", "" ],
													"patching_rect" : [ 53.0, 129.0, 33.0, 22.0 ],
													"style" : "default",
													"text" : "t 1 l"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-7",
													"maxclass" : "message",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 113.0, 248.0, 57.0, 22.0 ],
													"style" : "default",
													"text" : "steps $1"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-8",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"patching_rect" : [ 184.0, 245.0, 56.0, 22.0 ],
													"style" : "default",
													"text" : "metro 1"
												}

											}
, 											{
												"box" : 												{
													"comment" : "time between steps (def 1 ms)",
													"id" : "obj-9",
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 221.0, 106.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"comment" : "number of steps (def 100)",
													"id" : "obj-10",
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 113.0, 71.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-11",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "list", "bang" ],
													"patching_rect" : [ 67.0, 310.0, 85.0, 22.0 ],
													"style" : "default",
													"text" : "list-interpolate"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-12",
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 53.0, 71.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-13",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "int" ],
													"patching_rect" : [ 137.0, 152.0, 37.0, 22.0 ],
													"style" : "default",
													"text" : "t 100"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-7", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-10", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-3", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-11", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-5", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-11", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-6", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-12", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-7", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-13", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-13", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-2", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-8", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-4", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-1", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-5", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-4", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"midpoints" : [ 160.5, 379.0, 299.5, 379.0, 299.5, 201.0, 193.5, 201.0 ],
													"source" : [ "obj-5", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-11", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-6", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-4", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-6", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-11", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-7", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-11", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-8", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-8", 1 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-9", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 220.0, 243.0, 101.0, 22.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"style" : "",
										"tags" : ""
									}
,
									"style" : "default",
									"text" : "p list-smoothly"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-22",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 78.0, 25.0, 83.0, 20.0 ],
									"style" : "default",
									"text" : "try these....."
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-13", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-9", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-20", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-17", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-18", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-20", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-1", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-21", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-18", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-19", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 647.0, 411.0, 68.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p prettier"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-21",
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
						"rect" : [ 60.0, 79.0, 546.0, 449.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 10.0, 10.0 ],
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
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-1",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 217.0, 151.0, 69.0, 17.0 ],
									"style" : "",
									"text" : "< move this >"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-2",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 233.0, 328.0, 82.0, 17.0 ],
									"style" : "",
									"text" : "interpolated mix"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-3",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 343.0, 134.0, 65.0, 17.0 ],
									"style" : "",
									"text" : "another thing"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-4",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 97.0, 136.0, 48.0, 17.0 ],
									"style" : "",
									"text" : "one thing"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-5",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 232.0, 134.0, 31.0, 17.0 ],
									"style" : "",
									"text" : "m i x"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 41.5, 38.0, 29.5, 22.0 ],
									"style" : "default",
									"text" : "1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 41.5, 7.0, 60.0, 22.0 ],
									"style" : "default",
									"text" : "loadbang"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"maxclass" : "preset",
									"numinlets" : 1,
									"numoutlets" : 4,
									"outlettype" : [ "preset", "int", "preset", "int" ],
									"patching_rect" : [ 41.5, 67.0, 28.0, 17.0 ],
									"preset_data" : [ 										{
											"number" : 1,
											"data" : [ 257, "obj-14", "multislider", "list", 209.454544, 210.236816, 211.019104, 212.779221, 210.008652, 207.238098, 204.467529, 201.696976, 198.926407, 196.155838, 194.20015, 192.244461, 190.288773, 188.333084, 186.37738, 184.421692, 182.466003, 180.510315, 178.554626, 176.598938, 174.643234, 172.687546, 170.731857, 168.776169, 166.82048, 164.864777, 162.909088, 161.374619, 159.840164, 158.305695, 156.771225, 155.236771, 153.702301, 152.167831, 150.633362, 149.098907, 147.564438, 146.029968, 144.495499, 142.961044, 141.426575, 139.892105, 138.357635, 136.823181, 135.288712, 133.754242, 132.219788, 130.685318, 129.150848, 127.616386, 126.081917, 124.547455, 123.012985, 121.990013, 120.967033, 119.944054, 118.921082, 117.898102, 116.875122, 115.85215, 114.82917, 113.80619, 112.783218, 111.760239, 110.737267, 109.714287, 108.051949, 106.38961, 104.727272, 103.064934, 101.402596, 99.740257, 98.077919, 96.415581, 94.75325, 93.090912, 91.428574, 89.766235, 88.103897, 86.441559, 84.779221, 83.116882, 81.454544, 79.792206, 77.978752, 76.165291, 74.35183, 72.538368, 70.724915, 68.911453, 67.097992, 65.284531, 63.471073, 61.657616, 59.844154, 59.844154, 59.844154, 53.194805, 51.532467, 49.870129, 48.20779, 46.545456, 45.437229, 44.329006, 43.220779, 45.437229, 47.653679, 49.870129, 52.086578, 54.303032, 56.519482, 58.735931, 60.952381, 63.168831, 65.385284, 67.60173, 69.818184, 72.03463, 74.251083, 76.467529, 78.683983, 80.900436, 83.116882, 85.333336, 87.549782, 89.766235, 91.982681, 94.199135, 96.415581, 98.632034, 100.848488, 103.064934, 105.281387, 107.497833, 109.714287, 111.930733, 114.147186, 116.36364, 119.272728, 122.181816, 125.090912, 128.0, 130.909088, 133.818176, 136.72728, 139.636368, 142.545456, 145.454544, 148.363632, 151.27272, 154.181824, 157.090912, 160.0, 162.909088, 164.9039, 166.898697, 168.893509, 170.888306, 172.883118, 176.207794, 173.215591, 170.223373, 167.231171, 164.238968, 161.24675, 158.254547, 155.262344, 152.270126, 149.277924, 146.285721, 143.515152, 140.744583, 137.97403, 135.203461, 132.432907, 129.662338, 126.891777, 124.121216, 121.350647, 118.580086, 115.809525, 113.038963, 108.606064, 104.173157, 99.740257, 97.302162, 94.864067, 92.425972, 89.987877, 87.549782, 85.111687, 82.673592, 80.235497, 77.797401, 75.359306, 72.921211, 70.483116, 68.045021, 65.606926, 63.168831, 59.844154, 59.844154, 59.844154, 63.168831, 66.493507, 69.818184, 73.14286, 76.467529, 79.792206, 83.116882, 86.441559, 88.103897, 89.766235, 91.428574, 93.090912, 94.75325, 96.415581, 98.077919, 99.740257, 101.402596, 103.064934, 106.38961, 106.38961, 106.38961, 106.38961, 106.38961, 106.38961, 106.864563, 107.339516, 107.814468, 108.289421, 108.764381, 109.239334, 109.714287, 109.714287, 109.714287, 109.714287, 105.97403, 102.233765, 98.493507, 94.75325, 91.012985, 87.272728, 83.532471, 79.792206, 78.129868, 76.467529, 74.805191, 73.14286, 66.493507, 59.844154, 53.194805, 39.896103, 257, "obj-15", "multislider", "list", 119.688309, 121.50177, 123.315231, 125.128693, 126.942146, 128.755615, 130.569061, 132.382523, 134.195984, 136.009445, 137.822906, 139.636368, 141.449829, 143.263275, 145.076736, 146.890198, 148.703659, 150.51712, 152.330582, 154.144043, 155.957504, 157.77095, 159.584412, 160.470993, 161.357574, 162.244156, 163.130737, 164.017319, 164.9039, 165.790482, 166.677063, 167.563629, 168.450211, 169.336792, 170.223373, 171.109955, 171.996536, 172.883118, 173.769699, 174.656281, 175.542862, 176.429443, 177.316025, 178.202591, 179.089172, 179.975754, 180.862335, 181.748917, 182.635498, 183.522079, 184.408661, 185.295242, 186.181824, 186.597397, 187.012985, 187.428574, 187.844162, 188.259735, 188.675323, 189.090912, 189.5065, 189.922073, 190.337662, 190.75325, 191.168839, 191.584412, 192.0, 192.415588, 189.5065, 187.290039, 185.073593, 182.857147, 180.640686, 178.42424, 176.207794, 173.991348, 171.774887, 169.558441, 167.341995, 165.125534, 162.909088, 160.692642, 158.476196, 156.259735, 154.043289, 151.826843, 149.610382, 147.393936, 145.17749, 142.961044, 141.219543, 139.478043, 137.736542, 135.995056, 134.253555, 132.512054, 130.770569, 129.029068, 127.287567, 125.546074, 123.804573, 122.06308, 120.321587, 118.580086, 116.838593, 115.097092, 113.355598, 111.614098, 109.872604, 108.131104, 106.38961, 105.439705, 104.489799, 103.539886, 102.589981, 101.640076, 100.69017, 99.740257, 98.790352, 97.840446, 96.890541, 95.940628, 94.990723, 94.040817, 93.090912, 93.090912, 94.642426, 96.193939, 97.745453, 99.296967, 100.848488, 102.400002, 103.951515, 105.503029, 107.054543, 108.606064, 110.157578, 111.709091, 113.260605, 114.812119, 116.36364, 116.696106, 117.028572, 117.361038, 117.693504, 118.02597, 118.358444, 118.69091, 119.023376, 119.355843, 119.688309, 119.688309, 119.688309, 119.688309, 119.688309, 119.688309, 119.688309, 119.688309, 119.688309, 119.272728, 118.85714, 118.441559, 118.02597, 117.61039, 117.194809, 116.779221, 116.36364, 115.440117, 114.516594, 113.593071, 112.669556, 111.746033, 110.82251, 109.898987, 108.975471, 108.051949, 107.128426, 106.204903, 105.281387, 104.357864, 103.434341, 102.510826, 101.587303, 100.66378, 99.740257, 99.438019, 99.135773, 98.833527, 98.531288, 98.229042, 97.926804, 97.624557, 97.322311, 97.020073, 96.717827, 96.415581, 96.415581, 96.415581, 96.415581, 96.415581, 96.927071, 97.43856, 97.95005, 98.46154, 98.97303, 99.484512, 99.996002, 100.507492, 101.018982, 101.530472, 102.041962, 102.553444, 103.064934, 103.064934, 101.018982, 98.97303, 96.927071, 94.881119, 92.835167, 90.789207, 88.743256, 86.697304, 84.651352, 82.605392, 80.559441, 78.513489, 76.467529, 73.881676, 71.295815, 68.709953, 66.1241, 63.538239, 60.952381, 58.366524, 55.780663, 53.194805, 50.978355, 48.761906, 46.545456, 44.329006, 42.112553, 36.57143, 36.57143, 36.57143, 33.246754, 32.581818, 31.916883, 31.251947, 30.587013, 29.922077, 29.922077, 29.922077, 26.597403 ]
										}
, 										{
											"number" : 2,
											"data" : [ 257, "obj-14", "multislider", "list", 246.02597, 241.038956, 236.051941, 231.064941, 226.077927, 223.861465, 221.64502, 219.428574, 217.212128, 214.995667, 212.779221, 210.562775, 208.346313, 206.129868, 202.805191, 199.480515, 196.155838, 192.831161, 189.5065, 186.181824, 180.862335, 175.542862, 170.223373, 164.9039, 159.584412, 156.259735, 152.935059, 149.610382, 146.285721, 142.961044, 141.852814, 140.744583, 139.636368, 138.306488, 136.976624, 135.646759, 134.316879, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 132.987015, 133.573715, 134.160431, 134.747131, 135.333847, 135.920547, 136.507263, 137.093964, 137.680679, 138.26738, 138.85408, 139.440796, 140.027496, 140.614212, 141.200912, 141.787628, 142.374329, 142.961044, 144.623383, 146.285721, 147.948059, 149.610382, 151.27272, 152.935059, 155.594803, 158.254547, 160.914291, 163.57402, 166.233765, 169.558441, 172.883118, 176.207794, 179.532471, 182.857147, 185.231918, 187.606674, 189.981445, 192.356216, 194.730988, 197.105759, 199.480515, 201.97403, 204.467529, 206.961044, 209.454544, 209.676193, 209.897842, 210.119476, 210.341125, 210.562775, 210.784409, 211.006058, 211.227707, 211.449356, 211.67099, 211.892639, 212.114288, 212.335938, 212.557571, 212.779221, 212.779221, 212.779221, 212.779221, 212.779221, 212.779221, 212.779221, 212.779221, 212.779221, 212.779221, 212.779221, 212.779221, 212.779221, 211.67099, 210.562775, 209.454544, 208.346313, 207.238098, 206.129868, 205.021652, 203.913422, 202.805191, 201.696976, 200.588745, 199.480515, 198.372299, 197.264069, 196.155838, 195.047623, 193.939392, 192.831161, 190.614716, 188.39827, 186.181824, 183.965363, 181.748917, 179.532471, 177.316025, 175.099564, 172.883118, 171.220779, 169.558441, 167.896103, 159.584412, 156.515488, 153.446548, 150.377625, 147.308685, 144.239761, 141.170822, 138.101898, 135.032974, 131.964035, 128.895111, 125.826172, 122.75724, 119.688309, 116.779221, 113.870132, 110.961037, 108.051949, 105.14286, 102.233765, 99.324677, 96.415581, 93.64502, 90.874458, 88.103897, 85.333336, 82.562767, 79.792206, 78.367348, 76.94249, 75.517624, 74.092766, 72.6679, 71.243042, 69.818184, 68.820778, 67.82338, 66.825974, 65.828575, 64.831169, 63.833767, 62.836365, 61.838963, 60.841557, 59.844154, 58.735931, 57.627705, 56.519482, 55.411255, 54.303032, 53.194805, 53.194805, 53.194805, 53.194805, 53.194805, 53.194805, 53.194805, 53.194805, 53.194805, 53.194805, 53.194805, 53.194805, 53.194805, 53.194805, 54.303032, 55.411255, 56.519482, 59.844154, 63.168831, 66.493507, 69.818184, 73.14286, 73.14286, 73.14286, 74.805191, 76.467529, 83.116882, 83.781815, 84.446754, 85.111687, 85.776627, 86.441559, 87.771431, 89.101295, 90.431168, 91.76104, 93.090912, 94.199135, 95.307358, 96.415581, 99.740257, 103.064934, 104.727272, 106.38961, 113.038963, 113.038963, 113.038963, 116.36364, 257, "obj-15", "multislider", "list", 0.0, 1.662338, 3.324675, 4.987013, 6.649351, 8.422511, 10.195671, 11.968831, 13.741991, 15.515152, 17.288311, 19.061472, 20.834633, 22.607792, 24.380953, 26.154112, 27.927273, 29.700434, 31.473593, 33.246754, 34.909092, 36.57143, 38.233765, 39.896103, 41.558441, 43.220779, 44.883118, 46.545456, 48.20779, 49.870129, 51.68359, 53.497047, 55.310509, 57.123966, 58.937428, 60.750885, 62.564346, 64.377808, 66.191261, 68.004723, 69.818184, 72.311691, 74.805191, 77.298698, 79.792206, 83.948051, 88.103897, 92.259743, 96.415581, 98.315399, 100.21521, 102.115028, 104.014839, 105.914658, 107.814468, 109.714287, 112.08905, 114.463821, 116.838593, 119.213356, 121.588127, 123.962891, 126.337662, 128.554108, 130.770569, 132.987015, 135.314285, 137.641556, 139.968826, 142.296097, 144.623383, 146.950653, 149.277924, 151.605194, 153.932465, 156.259735, 159.030304, 161.800873, 164.571426, 167.341995, 170.112549, 172.883118, 173.991348, 175.099564, 176.207794, 177.63266, 179.05751, 180.482376, 181.907242, 183.332092, 184.756958, 186.181824, 186.181824, 186.181824, 187.290039, 188.39827, 189.5065, 189.5065, 189.5065, 189.5065, 188.902008, 188.297516, 187.693039, 187.088547, 186.484055, 185.879578, 185.275085, 184.670609, 184.066116, 183.461624, 182.857147, 181.669754, 180.482376, 179.294998, 178.107605, 176.920227, 175.732834, 174.545456, 173.358078, 172.170685, 170.983307, 169.795914, 168.608536, 167.421158, 166.233765, 164.955048, 163.676331, 162.397598, 161.118881, 159.840164, 158.561432, 157.282715, 156.003998, 154.725281, 153.446548, 152.167831, 150.889114, 149.610382, 148.502167, 147.393936, 146.285721, 143.625977, 140.966232, 138.306488, 135.646759, 132.987015, 126.337662, 125.506493, 124.675323, 123.844154, 116.36364, 114.147186, 111.930733, 109.714287, 103.064934, 96.415581, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.090912, 93.755844, 94.420776, 95.085716, 95.750648, 96.415581, 96.415581, 96.415581, 96.415581, 96.415581, 96.415581, 96.415581, 96.415581, 96.415581, 96.415581, 96.415581, 96.415581, 94.75325, 93.090912, 90.431168, 87.771431, 85.111687, 82.45195, 79.792206, 78.683983, 77.57576, 76.467529, 75.802597, 75.137665, 74.472725, 73.807793, 73.14286, 72.03463, 70.926407, 69.818184, 67.60173, 65.385284, 63.168831, 61.691196, 60.213566, 58.735931, 57.258297, 55.780663, 54.303032, 52.825397, 51.347763, 49.870129, 48.20779, 46.545456, 44.883118, 43.220779, 41.558441, 39.896103, 38.649349, 37.402596, 36.155846, 34.909092, 33.662338, 32.415585, 31.168831, 29.922077, 28.536797, 27.151516, 25.766233, 24.380953, 22.99567, 21.61039, 20.225109, 18.839827, 17.454546, 16.069263, 14.683983, 13.298701, 11.968831, 10.638961, 9.309091, 7.979221, 6.649351, 4.987013, 3.324675, 1.662338, 0.0 ]
										}
 ],
									"style" : "default"
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"contdata" : 1,
									"id" : "obj-9",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"orientation" : 0,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 198.0, 103.0, 99.0, 78.0 ],
									"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
									"setminmax" : [ 0.0, 1.0 ],
									"slidercolor" : [ 0.584314, 0.584314, 0.584314, 1.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Impact",
									"fontsize" : 18.0,
									"id" : "obj-10",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 296.0, 79.0, 21.0, 28.0 ],
									"style" : "",
									"text" : "1."
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"format" : 6,
									"htricolor" : [ 0.87, 0.82, 0.24, 1.0 ],
									"id" : "obj-11",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 198.0, 185.0, 35.0, 19.0 ],
									"style" : "",
									"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"tricolor" : [ 0.75, 0.75, 0.75, 1.0 ],
									"triscale" : 0.9
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-12",
									"maxclass" : "newobj",
									"numinlets" : 3,
									"numoutlets" : 1,
									"outlettype" : [ "list" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 0,
											"revision" : 4,
											"architecture" : "x64",
											"modernui" : 1
										}
,
										"rect" : [ 70.0, 79.0, 424.0, 406.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 10.0, 10.0 ],
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
										"boxes" : [ 											{
												"box" : 												{
													"id" : "obj-1",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 1,
													"outlettype" : [ "float" ],
													"patching_rect" : [ 60.0, 234.0, 29.5, 22.0 ],
													"style" : "default",
													"text" : "f"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-2",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 3,
													"outlettype" : [ "bang", "bang", "" ],
													"patching_rect" : [ 155.0, 181.0, 40.0, 22.0 ],
													"style" : "default",
													"text" : "t b b l"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-3",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"patching_rect" : [ 176.0, 234.0, 40.0, 22.0 ],
													"style" : "default",
													"text" : "zl reg"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-4",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 3,
													"outlettype" : [ "bang", "", "bang" ],
													"patching_rect" : [ 276.0, 180.0, 40.0, 22.0 ],
													"style" : "default",
													"text" : "t b l b"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-5",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "", "" ],
													"patching_rect" : [ 276.0, 237.0, 40.0, 22.0 ],
													"style" : "default",
													"text" : "zl reg"
												}

											}
, 											{
												"box" : 												{
													"comment" : "input list 2",
													"id" : "obj-6",
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 276.0, 87.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"comment" : "inpute list 1",
													"id" : "obj-7",
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 155.0, 86.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"comment" : "interpolated mix",
													"id" : "obj-8",
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 176.0, 330.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"comment" : "(float) between 0. and 1., mix level",
													"id" : "obj-9",
													"maxclass" : "inlet",
													"numinlets" : 0,
													"numoutlets" : 1,
													"outlettype" : [ "" ],
													"patching_rect" : [ 29.0, 86.0, 30.0, 30.0 ],
													"style" : "default"
												}

											}
, 											{
												"box" : 												{
													"id" : "obj-10",
													"maxclass" : "newobj",
													"numinlets" : 1,
													"numoutlets" : 2,
													"outlettype" : [ "list", "bang" ],
													"patching_rect" : [ 176.0, 285.0, 85.0, 22.0 ],
													"style" : "default",
													"text" : "list-interpolate"
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-1", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-8", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-10", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-1", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-2", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-3", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-2", 2 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-5", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-2", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-3", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-1", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-4", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-3", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-4", 2 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-5", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-4", 1 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-5", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-4", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-6", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-2", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-7", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-1", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-9", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 198.0, 247.0, 69.0, 22.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"style" : "",
										"tags" : ""
									}
,
									"style" : "default",
									"text" : "p list-mixer"
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.909804, 0.85098, 0.0, 1.0 ],
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"id" : "obj-13",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 198.0, 305.0, 157.0, 79.0 ],
									"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
									"setminmax" : [ 0.0, 256.0 ],
									"size" : 253,
									"slidercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.572549, 0.713726, 0.0, 1.0 ],
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"id" : "obj-14",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 298.0, 103.0, 157.0, 79.0 ],
									"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
									"setminmax" : [ 0.0, 256.0 ],
									"size" : 253,
									"slidercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.839216, 0.392157, 0.0, 1.0 ],
									"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
									"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
									"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
									"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
									"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
									"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
									"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
									"id" : "obj-15",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 40.0, 103.0, 157.0, 79.0 ],
									"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
									"setminmax" : [ 0.0, 256.0 ],
									"size" : 253,
									"slidercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Impact",
									"fontsize" : 18.0,
									"id" : "obj-16",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 180.0, 79.0, 21.0, 28.0 ],
									"style" : "",
									"text" : "0."
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 9.0,
									"id" : "obj-17",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 199.0, 86.0, 104.0, 17.0 ],
									"style" : "",
									"text" : "................................"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
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
									"destination" : [ "obj-12", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"midpoints" : [ 49.5, 99.0, 307.5, 99.0 ],
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 647.0, 382.0, 59.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "default",
					"text" : "p pretty"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-22",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 474.0, 213.5, 51.0, 22.0 ],
					"style" : "default",
					"text" : "steps 5"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-23",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 38.0, 241.5, 61.0, 22.0 ],
					"style" : "default",
					"text" : "3. 3. 3. 3."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-24",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 39.0, 210.0, 61.0, 22.0 ],
					"style" : "default",
					"text" : "2. 2. 2. 2."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-25",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 39.0, 178.5, 61.0, 22.0 ],
					"style" : "default",
					"text" : "1. 1. 1. 1."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-26",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 39.0, 147.0, 61.0, 22.0 ],
					"style" : "default",
					"text" : "0. 0. 0. 0."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-27",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 218.5, 407.5, 24.0, 24.0 ],
					"style" : "default"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-28",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 405.0, 213.5, 57.0, 22.0 ],
					"style" : "default",
					"text" : "steps 10"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-29",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 116.0, 246.75, 81.0, 22.0 ],
					"style" : "default",
					"text" : "1. 2. 10. 100."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-30",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 117.0, 213.5, 61.0, 22.0 ],
					"style" : "default",
					"text" : "0. 0. 0. 0."
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-31",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 218.5, 194.0, 37.0, 22.0 ],
					"style" : "default",
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-32",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 427.0, 322.0, 98.0, 22.0 ],
					"style" : "default",
					"text" : "tellmeeverything"
				}

			}
, 			{
				"box" : 				{
					"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
					"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
					"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
					"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
					"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
					"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
					"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
					"contdata" : 1,
					"id" : "obj-33",
					"maxclass" : "multislider",
					"numinlets" : 1,
					"numoutlets" : 2,
					"orientation" : 0,
					"outlettype" : [ "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 218.5, 174.0, 121.0, 15.0 ],
					"setminmax" : [ 0.0, 1.0 ],
					"setstyle" : 1,
					"style" : "default"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-34",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 117.0, 180.25, 71.0, 22.0 ],
					"style" : "default",
					"text" : "5. 16. 0. -2."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-35",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 117.0, 147.0, 61.0, 22.0 ],
					"style" : "default",
					"text" : "1. 2. 3. 4."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-36",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 112.0, 404.0, 69.0, 22.0 ],
					"style" : "default",
					"text" : "print hello"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-37",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 365.5, 177.0, 25.0, 25.0 ],
					"style" : "default"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-38",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "list", "bang" ],
					"patching_rect" : [ 112.0, 364.0, 125.0, 22.0 ],
					"style" : "default",
					"text" : "list-interpolate 5 1024"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-39",
					"linecount" : 3,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 62.0, 91.0, 118.0, 47.0 ],
					"style" : "default",
					"text" : "list-interpolate remembers the last two lists you sent it."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-40",
					"linecount" : 5,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 365.5, 102.0, 211.0, 74.0 ],
					"style" : "default",
					"text" : "Each bang moves one \"step\" towards the most recently received list. Once you get there you get a bang out the right outlet and then bang does nothing until you send a new list."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-41",
					"linecount" : 4,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 218.5, 109.0, 139.0, 60.0 ],
					"style" : "default",
					"text" : "A float interpolates between the two most recently received lists. (0.-1.)"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-42",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 531.0, 322.0, 217.0, 20.0 ],
					"style" : "default",
					"text" : "Print internal state to Max window"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-43",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 272.0, 364.0, 226.0, 20.0 ],
					"style" : "default",
					"text" : "Optional integer arguments:"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-44",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 549.0, 213.5, 157.0, 20.0 ],
					"style" : "default",
					"text" : "Set the number of steps"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-22", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-23", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-24", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-26", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-28", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-30", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-33", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-34", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-35", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-27", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-38", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-36", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-38", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-4", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "list-interpolate.mxo",
				"type" : "iLaX"
			}
 ],
		"embedsnapshot" : 0
	}

}
