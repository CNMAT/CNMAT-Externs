{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 691.0, 139.0, 1112.0, 603.0 ],
		"bglocked" : 0,
		"defrect" : [ 691.0, 139.0, 1112.0, 603.0 ],
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
					"maxclass" : "o.message",
					"text" : "/button/is up \n/horizontal 568 \n/vertical 116 \n",
					"linecount" : 3,
					"id" : "obj-7",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 114.0, 80.0, 339.0, 48.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.io.mouse",
					"id" : "obj-2",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 166.0, 48.0, 55.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "This is o.show embedded with a bpatcher",
					"linecount" : 3,
					"id" : "obj-3",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"patching_rect" : [ 776.0, 98.0, 100.0, 38.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "screen coordinates",
					"id" : "obj-4",
					"fontname" : "Arial",
					"numoutlets" : 0,
					"patching_rect" : [ 522.0, 107.0, 100.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
