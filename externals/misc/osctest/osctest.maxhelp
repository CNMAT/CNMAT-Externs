{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 485.0, 44.0, 910.0, 669.0 ],
		"bglocked" : 0,
		"defrect" : [ 485.0, 44.0, 910.0, 669.0 ],
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
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "any messages in a bundle that an object doesn't have a method to handle will be copied and sent out the right outlet",
					"linecount" : 3,
					"numinlets" : 1,
					"presentation_rect" : [ 528.0, 545.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 528.0, 544.0, 313.0, 48.0 ],
					"id" : "obj-23",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "in response to /<classname>/<parametername>/get, all objects will output /<classname>/<parametername> <value> out the right outlet",
					"linecount" : 3,
					"numinlets" : 1,
					"presentation_rect" : [ 512.0, 269.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 512.0, 268.0, 313.0, 48.0 ],
					"id" : "obj-21",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "the value of any parameter can be set with normal max messages, or with an OSC message with an address like /<classname>/<parametername>/<get|set> <value>",
					"linecount" : 3,
					"numinlets" : 1,
					"presentation_rect" : [ 512.0, 209.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 512.0, 208.0, 318.0, 48.0 ],
					"id" : "obj-20",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "osctest has one parameter: param <float>",
					"numinlets" : 1,
					"presentation_rect" : [ 511.0, 236.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 512.0, 180.0, 313.0, 20.0 ],
					"id" : "obj-19",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "in response to the probe message, all objects will output an OSC bundle with the addresses of all methods and the values of all parameters",
					"linecount" : 3,
					"numinlets" : 1,
					"presentation_rect" : [ 528.0, 362.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 528.0, 404.0, 313.0, 48.0 ],
					"id" : "obj-18",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "all objects will respond to the probe max message or the /<classname>/probe OSC message.",
					"linecount" : 2,
					"numinlets" : 1,
					"presentation_rect" : [ 529.0, 312.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 528.0, 368.0, 313.0, 34.0 ],
					"id" : "obj-17",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "all messages (in this case, just bang) can be called with a normal max message, or with an OSC message with an address like /<classname>/message",
					"linecount" : 2,
					"numinlets" : 1,
					"presentation_rect" : [ 328.0, 169.0, 0.0, 0.0 ],
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 328.0, 68.0, 577.0, 34.0 ],
					"id" : "obj-16",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "osctest has one message: bang",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 328.0, 44.0, 180.0, 20.0 ],
					"id" : "obj-15",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/osctest/probe",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 416.0, 368.0, 105.0, 20.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-13",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "probe",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 372.0, 368.0, 41.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-14",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 260.0, 508.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"id" : "obj-9"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/osctest/bang",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 184.0, 44.0, 138.0, 20.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-8",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/osctest/param/get",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 220.0, 180.0, 138.0, 20.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-3",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"patching_rect" : [ 20.0, 176.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-11",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 160.0, 44.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 128.0, 492.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"id" : "obj-7"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"patching_rect" : [ 160.0, 492.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-5",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/osctest/bang \n/osctest/param/set \n/osctest/probe \n/osctest/param/get \n/osctest/param 20.000000 ",
					"linecount" : 5,
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 160.0, 536.0, 157.0, 75.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-2",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/osctest/param/set 100. /foo/bar 199",
					"linecount" : 2,
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 368.0, 180.0, 138.0, 34.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-4",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"varname" : "osctest",
					"text" : "osctest @param 20.",
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"patching_rect" : [ 160.0, 456.0, 118.0, 20.0 ],
					"outlettype" : [ "float", "FullPacket" ],
					"id" : "obj-1",
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "param $1",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 20.0, 208.0, 59.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-10",
					"fontname" : "Arial"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-1", 0 ],
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
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-7", 0 ],
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
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-2", 1 ],
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
					"source" : [ "obj-6", 0 ],
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
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-9", 0 ],
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
 ]
	}

}
