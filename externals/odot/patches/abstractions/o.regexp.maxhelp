{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 264.0, 44.0, 640.0, 480.0 ],
		"bglocked" : 0,
		"defrect" : [ 264.0, 44.0, 640.0, 480.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 8.0, 8.0 ],
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
					"text" : "/foo/rock/bar/triple 12 beers ",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"presentation_rect" : [ 181.0, 211.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 184.0, 232.0, 158.0, 20.0 ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/triple/rock 12 beers",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 184.0, 112.0, 114.0, 20.0 ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.regexp /(\\\\w+)/(\\\\w+) @substitute /foo/%2/bar/%1",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 184.0, 176.0, 280.0, 20.0 ],
					"id" : "obj-1",
					"fontname" : "Arial"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-5", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
