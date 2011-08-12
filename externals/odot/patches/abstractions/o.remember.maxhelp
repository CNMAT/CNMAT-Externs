{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 25.0, 69.0, 869.0, 606.0 ],
		"bglocked" : 0,
		"defrect" : [ 25.0, 69.0, 869.0, 606.0 ],
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
					"text" : "o.expr /dx = /horizontal - /was/horizontal (/dy = /vertical - /was/vertical)",
					"outlettype" : [ "FullPacket" ],
					"fontsize" : 20.0,
					"numinlets" : 1,
					"patching_rect" : [ 100.0, 268.0, 693.0, 29.0 ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/was/horizontal 563 \n/was/vertical 300 \n/horizontal 563 \n/vertical 300 \n/dx 0 \n/dy 0 \n",
					"linecount" : 6,
					"outlettype" : [ "" ],
					"fontsize" : 20.0,
					"numinlets" : 2,
					"patching_rect" : [ 217.0, 343.0, 286.0, 144.0 ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.remember /horizontal /vertical",
					"outlettype" : [ "" ],
					"fontsize" : 20.0,
					"numinlets" : 1,
					"patching_rect" : [ 270.0, 205.0, 291.0, 29.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.io.mouse",
					"outlettype" : [ "" ],
					"fontsize" : 20.0,
					"numinlets" : 0,
					"patching_rect" : [ 272.0, 162.0, 108.0, 29.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"numoutlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
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
 ]
	}

}
