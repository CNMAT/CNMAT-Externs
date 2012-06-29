{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 159.0, 82.0, 631.0, 425.0 ],
		"bglocked" : 0,
		"defrect" : [ 159.0, 82.0, 631.0, 425.0 ],
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
					"maxclass" : "newobj",
					"text" : "loadmess set",
					"numinlets" : 1,
					"fontsize" : 13.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Helvetica Neue Light",
					"hidden" : 1,
					"patching_rect" : [ 225.0, 290.0, 87.0, 22.0 ],
					"id" : "obj-36"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "dict.route",
					"numinlets" : 1,
					"fontsize" : 48.0,
					"numoutlets" : 0,
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 25.0, 20.0, 239.0, 65.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "use dict.route to see if the contents of a dictionary meet your expectations or to selectively pass dictionaries through one outlet or the other.",
					"linecount" : 2,
					"numinlets" : 1,
					"textcolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"fontsize" : 13.0,
					"numoutlets" : 0,
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 25.0, 125.0, 435.0, 38.0 ],
					"frgb" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "dict.route directs dictionaries based on their content",
					"numinlets" : 1,
					"fontsize" : 13.0,
					"numoutlets" : 0,
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 25.0, 105.0, 309.0, 22.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-5"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "dict.route will pass a dict through unchaged any time the dict matches the specified key and value.  \n\nIn this example a dict will be passed when it has a 'frog' key with a value of 4",
					"linecount" : 6,
					"numinlets" : 1,
					"textcolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"fontsize" : 13.0,
					"numoutlets" : 0,
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 340.0, 285.0, 252.0, 101.0 ],
					"frgb" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"id" : "obj-25"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "arguments",
					"numinlets" : 1,
					"fontsize" : 13.0,
					"numoutlets" : 0,
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 340.0, 265.0, 178.0, 22.0 ],
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-27"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"numinlets" : 2,
					"fontsize" : 13.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 215.0, 330.0, 107.0, 20.0 ],
					"id" : "obj-74"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 190.0, 340.0, 20.0, 20.0 ],
					"id" : "obj-75"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"numinlets" : 2,
					"fontsize" : 13.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 55.0, 330.0, 107.0, 20.0 ],
					"id" : "obj-26"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 30.0, 340.0, 20.0, 20.0 ],
					"id" : "obj-24"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dict.route frog:4",
					"numinlets" : 2,
					"fontsize" : 13.0,
					"numoutlets" : 2,
					"color" : [ 0.533333, 0.74902, 0.533333, 1.0 ],
					"outlettype" : [ "dictionary", "dictionary" ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 30.0, 290.0, 179.0, 22.0 ],
					"id" : "obj-8"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numinlets" : 1,
					"fontsize" : 13.0,
					"numoutlets" : 2,
					"outlettype" : [ "int", "bang" ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 30.0, 180.0, 50.0, 22.0 ],
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dict.pack frog: pig: horse: duck: \"quack quack\" goose: @triggers 0 1 2 3 4",
					"linecount" : 2,
					"numinlets" : 5,
					"fontsize" : 13.0,
					"numoutlets" : 1,
					"outlettype" : [ "dictionary" ],
					"fontname" : "Helvetica Neue Light",
					"patching_rect" : [ 30.0, 210.0, 326.0, 38.0 ],
					"id" : "obj-2"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-74", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 1 ],
					"destination" : [ "obj-75", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 1 ],
					"destination" : [ "obj-74", 1 ],
					"hidden" : 0,
					"midpoints" : [ 199.5, 321.0, 312.5, 321.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-26", 1 ],
					"hidden" : 0,
					"midpoints" : [ 39.5, 320.0, 152.5, 320.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
