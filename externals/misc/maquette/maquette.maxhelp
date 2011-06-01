{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 276.0, 44.0, 1385.0, 930.0 ],
		"bglocked" : 0,
		"defrect" : [ 276.0, 44.0, 1385.0, 930.0 ],
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
					"maxclass" : "message",
					"text" : "deleteselected",
					"fontname" : "Arial",
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"patching_rect" : [ 32.0, 24.0, 88.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-9"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "tempo_editor",
					"textcolor" : [  ],
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"name" : "",
					"freqmax" : 8.77035,
					"timemin" : 4.472844,
					"fontname" : "Arial",
					"timemax" : 20.607029,
					"freqmin" : 6.186047,
					"numinlets" : 1,
					"functions" : [ 2, 0.0, 8.77035, 8.77035, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 20.607029, 6.186047, 6.186047, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
					"show_xaxis" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"show_yaxis" : 1,
					"patching_rect" : [ 128.0, 688.0, 381.0, 153.0 ],
					"id" : "obj-4",
					"colors" : [ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "maquette",
					"textcolor" : [  ],
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"ymax" : 7.0,
					"fontname" : "Arial",
					"xmax" : 50.0,
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"patching_rect" : [ 32.0, 56.0, 626.0, 344.0 ],
					"outlettype" : [ "", "" ],
					"id" : "obj-1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "tempo_editor",
					"textcolor" : [  ],
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"name" : "",
					"freqmax" : 8.77035,
					"timemin" : 4.472844,
					"fontname" : "Arial",
					"timemax" : 20.607029,
					"freqmin" : 6.186047,
					"numinlets" : 1,
					"functions" : [ 2, 0.0, 8.77035, 8.77035, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0, 20.607029, 6.186047, 6.186047, 0.0, 0.0, 0.1, 0.9, 1.0, 1.0, 2.0, 2.0, 0 ],
					"show_xaxis" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"show_yaxis" : 1,
					"patching_rect" : [ 128.0, 688.0, 381.0, 153.0 ],
					"id" : "obj-4",
					"colors" : [ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
