{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 618.0, 44.0, 640.0, 506.0 ],
		"bglocked" : 0,
		"defrect" : [ 618.0, 44.0, 640.0, 506.0 ],
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
		"metadata" : [  ],
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read-stream-number bone.sdif 0",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 331.0, 75.0, 151.0, 17.0 ],
					"id" : "obj-17",
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "SDIF-buffer foo",
					"outlettype" : [ "bang" ],
					"fontname" : "Arial",
					"patching_rect" : [ 356.0, 107.0, 92.0, 20.0 ],
					"id" : "obj-2",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "getMatrixHeader foo",
					"outlettype" : [ "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 34.0, 61.0, 119.0, 18.0 ],
					"id" : "obj-3",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mxj SDIFJava_test",
					"outlettype" : [ "", "" ],
					"fontname" : "Arial",
					"patching_rect" : [ 34.0, 88.0, 111.0, 20.0 ],
					"id" : "obj-1",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-2", 0 ],
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
 ]
	}

}
