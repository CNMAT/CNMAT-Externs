{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 38.0, 59.0, 897.0, 732.0 ],
		"bglocked" : 0,
		"defrect" : [ 38.0, 59.0, 897.0, 732.0 ],
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
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "I prefer this over  the mxj to avoid garbage collection",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-3",
					"fontname" : "Arial",
					"patching_rect" : [ 230.0, 96.0, 150.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "date",
					"numoutlets" : 3,
					"id" : "obj-1",
					"fontname" : "Arial",
					"patching_rect" : [ 376.0, 117.0, 46.0, 20.0 ],
					"outlettype" : [ "list", "list", "int" ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/month $1 \n",
					"numoutlets" : 1,
					"id" : "obj-46",
					"fontname" : "Arial",
					"patching_rect" : [ 504.370422, 660.578979, 158.0, 20.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/month/name $1",
					"numoutlets" : 1,
					"id" : "obj-45",
					"fontname" : "Arial",
					"patching_rect" : [ 208.833344, 682.315796, 150.0, 20.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-34",
					"patching_rect" : [ 17.5, 827.0, 25.0, 25.0 ],
					"numinlets" : 1,
					"comment" : ""
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/month 1 \n",
					"numoutlets" : 1,
					"id" : "obj-35",
					"fontname" : "Arial",
					"patching_rect" : [ 171.0, 864.0, 158.0, 20.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.accumulate",
					"numoutlets" : 1,
					"id" : "obj-37",
					"fontname" : "Arial",
					"patching_rect" : [ 17.5, 778.0, 82.0, 20.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "+ 1",
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-87",
					"fontname" : "Arial",
					"patching_rect" : [ 161.916672, 673.964905, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/day $1",
					"numoutlets" : 1,
					"id" : "obj-85",
					"fontname" : "Arial",
					"patching_rect" : [ 208.833344, 566.315796, 150.0, 20.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/year $1",
					"numoutlets" : 1,
					"id" : "obj-84",
					"fontname" : "Arial",
					"patching_rect" : [ 302.666656, 626.578979, 150.0, 20.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/month $1",
					"numoutlets" : 1,
					"id" : "obj-83",
					"fontname" : "Arial",
					"patching_rect" : [ 161.916672, 718.315796, 150.0, 20.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "07:24:04:938",
					"numoutlets" : 1,
					"id" : "obj-82",
					"fontname" : "Arial",
					"patching_rect" : [ 507.61438, 626.578979, 151.512085, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sprintf %.2d:%.2d:%.2d:%.3d",
					"numoutlets" : 1,
					"id" : "obj-80",
					"fontname" : "Arial",
					"patching_rect" : [ 509.126465, 570.578979, 166.0, 20.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"numinlets" : 4
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"numoutlets" : 3,
					"id" : "obj-54",
					"fontname" : "Arial",
					"types" : [  ],
					"patching_rect" : [ 540.512085, 291.333313, 28.0, 20.0 ],
					"outlettype" : [ "int", "", "" ],
					"fontsize" : 12.0,
					"items" : [ "AM", ",", "PM" ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"numoutlets" : 3,
					"id" : "obj-55",
					"fontname" : "Arial",
					"types" : [  ],
					"framecolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"patching_rect" : [ 186.583282, 652.684265, 66.0, 20.0 ],
					"outlettype" : [ "int", "", "" ],
					"arrow" : 0,
					"discolor" : [ 0.439216, 0.439216, 0.439216, 1.0 ],
					"textcolor" : [ 0.121569, 0.121569, 0.121569, 1.0 ],
					"fontsize" : 12.0,
					"togcolor" : [ 0.552941, 0.552941, 0.552941, 1.0 ],
					"bgcolor" : [ 0.733333, 0.733333, 0.733333, 1.0 ],
					"items" : [ "January", ",", "February", ",", "March", ",", "April", ",", "May", ",", "June", ",", "July", ",", "August", ",", "September", ",", "October", ",", "November", ",", "December" ],
					"hltcolor" : [ 0.733333, 0.733333, 0.733333, 1.0 ],
					"textcolor2" : [ 0.121569, 0.121569, 0.121569, 1.0 ],
					"numinlets" : 1,
					"bgcolor2" : [ 0.917647, 0.917647, 0.917647, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "- 1",
					"hidden" : 1,
					"numoutlets" : 1,
					"id" : "obj-56",
					"fontname" : "Arial",
					"patching_rect" : [ 115.0, 506.964905, 32.5, 20.0 ],
					"outlettype" : [ "int" ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"numoutlets" : 3,
					"id" : "obj-57",
					"fontname" : "Arial",
					"types" : [  ],
					"framecolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"patching_rect" : [ 115.0, 552.578979, 66.0, 20.0 ],
					"outlettype" : [ "int", "", "" ],
					"arrow" : 0,
					"discolor" : [ 0.439216, 0.439216, 0.439216, 1.0 ],
					"textcolor" : [ 0.121569, 0.121569, 0.121569, 1.0 ],
					"fontsize" : 12.0,
					"togcolor" : [ 0.552941, 0.552941, 0.552941, 1.0 ],
					"bgcolor" : [ 0.733333, 0.733333, 0.733333, 1.0 ],
					"items" : [ "Sunday", ",", "Monday", ",", "Tuesday", ",", "Wednesday", ",", "Thursday", ",", "Friday", ",", "Saturday" ],
					"hltcolor" : [ 0.733333, 0.733333, 0.733333, 1.0 ],
					"textcolor2" : [ 0.121569, 0.121569, 0.121569, 1.0 ],
					"numinlets" : 1,
					"bgcolor2" : [ 0.917647, 0.917647, 0.917647, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b b",
					"numoutlets" : 2,
					"id" : "obj-58",
					"fontname" : "Arial",
					"patching_rect" : [ 100.0, 151.052643, 34.0, 20.0 ],
					"outlettype" : [ "bang", "bang" ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "week of month",
					"numoutlets" : 0,
					"id" : "obj-59",
					"fontname" : "Arial",
					"patching_rect" : [ 668.105347, 210.736816, 89.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "week of year",
					"numoutlets" : 0,
					"id" : "obj-60",
					"fontname" : "Arial",
					"patching_rect" : [ 621.241028, 238.78949, 79.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "day of year",
					"numoutlets" : 0,
					"id" : "obj-61",
					"fontname" : "Arial",
					"patching_rect" : [ 574.376465, 262.842102, 70.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "am / pm",
					"numoutlets" : 0,
					"id" : "obj-62",
					"fontname" : "Arial",
					"patching_rect" : [ 566.801208, 290.614014, 54.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "millisecond",
					"numoutlets" : 0,
					"id" : "obj-63",
					"fontname" : "Arial",
					"patching_rect" : [ 479.647583, 324.719299, 70.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "second",
					"numoutlets" : 0,
					"id" : "obj-64",
					"fontname" : "Arial",
					"patching_rect" : [ 433.783142, 369.263153, 49.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "minute",
					"numoutlets" : 0,
					"id" : "obj-65",
					"fontname" : "Arial",
					"patching_rect" : [ 386.918732, 414.368378, 47.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "hour",
					"numoutlets" : 0,
					"id" : "obj-14",
					"fontname" : "Arial",
					"patching_rect" : [ 340.05423, 457.912292, 35.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "year",
					"numoutlets" : 0,
					"id" : "obj-66",
					"fontname" : "Arial",
					"patching_rect" : [ 300.189758, 493.315796, 34.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "day",
					"numoutlets" : 0,
					"id" : "obj-67",
					"fontname" : "Arial",
					"patching_rect" : [ 246.325348, 535.087708, 30.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "month",
					"numoutlets" : 0,
					"id" : "obj-68",
					"fontname" : "Arial",
					"patching_rect" : [ 197.583282, 611.964905, 44.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"id" : "obj-18",
					"patching_rect" : [ 100.0, 100.0, 20.0, 20.0 ],
					"outlettype" : [ "int" ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "qmetro 50",
					"numoutlets" : 1,
					"id" : "obj-19",
					"fontname" : "Arial",
					"patching_rect" : [ 100.0, 123.052643, 65.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-20",
					"fontname" : "Arial",
					"patching_rect" : [ 631.083313, 211.736816, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-69",
					"fontname" : "Arial",
					"patching_rect" : [ 584.166687, 238.508789, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-22",
					"fontname" : "Arial",
					"patching_rect" : [ 537.25, 262.561401, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-23",
					"fontname" : "Arial",
					"patching_rect" : [ 490.333313, 291.333313, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-70",
					"fontname" : "Arial",
					"patching_rect" : [ 443.416687, 324.438599, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-71",
					"fontname" : "Arial",
					"patching_rect" : [ 396.5, 367.982452, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-72",
					"fontname" : "Arial",
					"patching_rect" : [ 349.583344, 414.087708, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-73",
					"fontname" : "Arial",
					"patching_rect" : [ 302.666656, 457.631592, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-28",
					"fontname" : "Arial",
					"patching_rect" : [ 255.75, 493.035095, 46.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-74",
					"fontname" : "Arial",
					"patching_rect" : [ 208.833344, 534.807007, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-75",
					"fontname" : "Arial",
					"patching_rect" : [ 161.916672, 611.964905, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"id" : "obj-76",
					"fontname" : "Arial",
					"patching_rect" : [ 115.0, 482.631592, 37.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"triscale" : 0.9,
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mxj now",
					"numoutlets" : 13,
					"id" : "obj-77",
					"fontname" : "Arial",
					"patching_rect" : [ 115.0, 179.719299, 582.0, 20.0 ],
					"outlettype" : [ "int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "" ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "day of week",
					"numoutlets" : 0,
					"id" : "obj-33",
					"fontname" : "Arial",
					"patching_rect" : [ 40.0, 482.631592, 75.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"id" : "obj-17",
					"patching_rect" : [ 97.5, 11.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"numoutlets" : 1,
					"id" : "obj-15",
					"fontname" : "Arial",
					"patching_rect" : [ 97.5, 74.0, 32.5, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"numoutlets" : 1,
					"id" : "obj-12",
					"fontname" : "Arial",
					"patching_rect" : [ 97.5, 39.0, 60.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-87", 0 ],
					"destination" : [ "obj-83", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-80", 0 ],
					"destination" : [ "obj-82", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 0 ],
					"destination" : [ "obj-76", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 1 ],
					"destination" : [ "obj-75", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 2 ],
					"destination" : [ "obj-74", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 4 ],
					"destination" : [ "obj-73", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 5 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 6 ],
					"destination" : [ "obj-71", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 7 ],
					"destination" : [ "obj-70", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 10 ],
					"destination" : [ "obj-69", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 3 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 8 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 9 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-77", 11 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-76", 0 ],
					"destination" : [ "obj-56", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-75", 0 ],
					"destination" : [ "obj-87", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-75", 0 ],
					"destination" : [ "obj-55", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-74", 0 ],
					"destination" : [ "obj-85", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-73", 0 ],
					"destination" : [ "obj-80", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-72", 0 ],
					"destination" : [ "obj-80", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-71", 0 ],
					"destination" : [ "obj-80", 2 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-70", 0 ],
					"destination" : [ "obj-80", 3 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-58", 1 ],
					"destination" : [ "obj-77", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-58", 0 ],
					"destination" : [ "obj-37", 0 ],
					"hidden" : 0,
					"midpoints" : [ 109.5, 194.0, 27.0, 194.0, 27.0, 771.0, 27.0, 771.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-56", 0 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-35", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-54", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-58", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
