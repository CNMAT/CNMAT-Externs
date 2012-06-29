{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 57.0, 84.0, 394.0, 430.0 ],
		"bglocked" : 0,
		"defrect" : [ 57.0, 84.0, 394.0, 430.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 13.0,
		"default_fontface" : 0,
		"default_fontname" : "Helvetica Neue Light",
		"gridonopen" : 0,
		"gridsize" : [ 5.0, 5.0 ],
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
					"text" : "messages may pass a key followed by N values",
					"linecount" : 2,
					"numinlets" : 1,
					"textcolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"fontsize" : 13.0,
					"numoutlets" : 0,
					"presentation_rect" : [ 198.0, 188.0, 0.0, 0.0 ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 175.0, 195.0, 166.0, 38.0 ],
					"frgb" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"id" : "obj-17"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "messages may also use dictionary syntax",
					"linecount" : 2,
					"numinlets" : 1,
					"textcolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"fontsize" : 13.0,
					"numoutlets" : 0,
					"presentation_rect" : [ 199.0, 205.0, 0.0, 0.0 ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 175.0, 255.0, 152.0, 38.0 ],
					"frgb" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"id" : "obj-16"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "optional argument specifies the name of the dictionary",
					"linecount" : 2,
					"numinlets" : 1,
					"textcolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"fontsize" : 13.0,
					"numoutlets" : 0,
					"presentation_rect" : [ 153.0, 309.0, 0.0, 0.0 ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 110.0, 330.0, 170.0, 38.0 ],
					"frgb" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"id" : "obj-15"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dict.print",
					"numinlets" : 1,
					"fontsize" : 13.0,
					"numoutlets" : 0,
					"presentation_rect" : [ 82.0, 343.0, 0.0, 0.0 ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 35.0, 375.0, 61.0, 22.0 ],
					"id" : "obj-14"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "cat : on the mat",
					"numinlets" : 2,
					"fontsize" : 13.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 179.0, 245.0, 0.0, 0.0 ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 55.0, 295.0, 101.0, 20.0 ],
					"id" : "obj-5"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "moo :shoo flu",
					"numinlets" : 2,
					"fontsize" : 13.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 184.0, 220.0, 0.0, 0.0 ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 55.0, 270.0, 89.0, 20.0 ],
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "bar: goo wah",
					"numinlets" : 2,
					"fontsize" : 13.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 199.0, 185.0, 0.0, 0.0 ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 55.0, 245.0, 87.0, 20.0 ],
					"id" : "obj-7"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "foo man chu",
					"numinlets" : 2,
					"fontsize" : 13.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"presentation_rect" : [ 109.0, 185.0, 0.0, 0.0 ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 55.0, 205.0, 83.0, 20.0 ],
					"id" : "obj-9"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"presentation_rect" : [ 84.0, 185.0, 0.0, 0.0 ],
					"patching_rect" : [ 35.0, 185.0, 20.0, 20.0 ],
					"id" : "obj-12"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dict.group",
					"numinlets" : 1,
					"fontsize" : 13.0,
					"numoutlets" : 1,
					"outlettype" : [ "dictionary" ],
					"presentation_rect" : [ 84.0, 215.0, 0.0, 0.0 ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 35.0, 340.0, 69.0, 22.0 ],
					"id" : "obj-13"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "dict.group",
					"numinlets" : 1,
					"fontsize" : 48.0,
					"numoutlets" : 0,
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 30.0, 25.0, 239.0, 65.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-40"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "send a bang to pass the completed dictionary and start the process over again.",
					"linecount" : 2,
					"numinlets" : 1,
					"textcolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"fontsize" : 13.0,
					"numoutlets" : 0,
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 30.0, 130.0, 293.0, 38.0 ],
					"frgb" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"id" : "obj-3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "collect key/value pairs from the input to build a dictionary",
					"numinlets" : 1,
					"fontsize" : 13.0,
					"numoutlets" : 0,
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 30.0, 110.0, 336.0, 22.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-4"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 64.5, 292.5, 44.5, 292.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 64.5, 318.0, 44.5, 318.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 64.5, 232.5, 44.5, 232.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 64.5, 267.0, 44.5, 267.0 ]
				}

			}
 ]
	}

}
