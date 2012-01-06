{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 162.0, 176.0, 869.0, 606.0 ],
		"bglocked" : 0,
		"defrect" : [ 162.0, 176.0, 869.0, 606.0 ],
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
					"maxclass" : "newobj",
					"text" : "o.expr (/dx = /horizontal - /was/horizontal) \\; (/dy = /vertical - /was/vertical)",
					"linecount" : 2,
					"numoutlets" : 1,
					"fontsize" : 20.0,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 77.0, 170.0, 543.0, 52.0 ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/was/horizontal 781 \n/was/vertical 499 \n/horizontal 781 \n/vertical 499 \n/shift up \n/capslock up \n/option up \n/control up \n/command down \n/button/is up \n/dx 0 \n/dy 0 \n",
					"linecount" : 12,
					"numoutlets" : 1,
					"fontsize" : 20.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 85.0, 253.0, 286.0, 282.0 ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.remember /horizontal /vertical",
					"numoutlets" : 1,
					"fontsize" : 20.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 247.0, 108.0, 291.0, 29.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.io.mouse",
					"numoutlets" : 1,
					"fontsize" : 20.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 249.0, 65.0, 108.0, 29.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"numinlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-5", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
