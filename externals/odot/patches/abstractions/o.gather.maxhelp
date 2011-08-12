{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 803.0, 114.0, 640.0, 480.0 ],
		"bglocked" : 0,
		"defrect" : [ 803.0, 114.0, 640.0, 480.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 20.0,
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
					"maxclass" : "o.message",
					"text" : "/thing 4. \n/thang 3.3 sdf \n",
					"linecount" : 2,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"id" : "obj-7",
					"outlettype" : [ "" ],
					"patching_rect" : [ 58.0, 355.0, 193.0, 52.0 ],
					"fontsize" : 20.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/thong 43 \n/bla* 9. \n",
					"linecount" : 2,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"id" : "obj-6",
					"outlettype" : [ "" ],
					"patching_rect" : [ 288.0, 332.0, 416.0, 52.0 ],
					"fontsize" : 20.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/thing 4.",
					"numoutlets" : 1,
					"fontname" : "Arial",
					"id" : "obj-5",
					"outlettype" : [ "" ],
					"patching_rect" : [ 32.0, 127.0, 98.0, 29.0 ],
					"fontsize" : 20.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/thing 4.\n/thang 3.3 sdf\n/thong 43\n/blah 9.",
					"linecount" : 4,
					"numoutlets" : 1,
					"fontname" : "Arial",
					"id" : "obj-4",
					"outlettype" : [ "" ],
					"patching_rect" : [ 196.0, 170.0, 416.0, 98.0 ],
					"fontsize" : 20.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.gather /thing /thang /bla*",
					"numoutlets" : 2,
					"fontname" : "Arial",
					"id" : "obj-8",
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 194.0, 288.0, 116.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Divide OSC message into two messages gathering the named items into the one coming out of the left outlet and the remaining out the right outlet",
					"linecount" : 3,
					"numoutlets" : 0,
					"fontname" : "Arial",
					"id" : "obj-9",
					"patching_rect" : [ 66.0, 30.0, 229.0, 38.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 1 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
