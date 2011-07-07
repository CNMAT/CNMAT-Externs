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
					"text" : "snap_to_grid 1",
					"patching_rect" : [ 64.0, 256.0, 90.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-38",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/maquette/grid/major/x 10.\n/maquette/grid/minor/* 2.\n/maquette/grid/major/y 4.",
					"linecount" : 3,
					"patching_rect" : [ 8.0, 184.0, 178.0, 48.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-36",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr range(0\\,100\\,10) @as /maquette/grid/major/y",
					"patching_rect" : [ 424.0, 48.0, 290.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-35",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.print",
					"patching_rect" : [ 440.0, 128.0, 44.0, 20.0 ],
					"id" : "obj-32",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/maquette/grid/major/x 0.000000 10.000000 20.000000 30.000000 40.000000 50.000000 60.000000 70.000000 80.000000 90.000000 \n",
					"linecount" : 2,
					"patching_rect" : [ 424.0, 80.0, 478.0, 34.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-34",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 424.0, 0.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"id" : "obj-33",
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr range(0\\,100\\,10) @as /maquette/grid/major/x",
					"patching_rect" : [ 424.0, 24.0, 290.0, 20.0 ],
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-31",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"patching_rect" : [ 1184.0, 2008.0, 34.0, 20.0 ],
					"id" : "obj-30",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/0/hztrans 0 135.488 \n/0/microtrans 0 7380.73 \n/0/rev 0.25 \n/0/steps 50 \n/0/interp_incr 147.615 \n/0/dur 180.828 \n/0/direction 1 \n/1/hztrans 135.488 398.415 \n/1/microtrans 7380.73 2509.95 \n/1/rev 0.0714286 \n/1/steps 14 \n/1/interp_incr -347.913 \n/1/dur 71.6701 \n/1/direction 1 \n/2/hztrans 398.415 398.415 \n/2/microtrans 2509.95 2509.95 \n/2/rev 0.857143 \n/2/steps 171 \n/2/interp_incr 0 \n/2/dur 429.201 \n/2/direction 1 \n/3/hztrans 398.415 0 \n/3/microtrans 2509.95 0 \n/3/rev 1.90654 \n/3/steps 381 \n/3/interp_incr -6.58779 \n/3/dur 479.4 \n/3/direction 1 \n/4/hztrans 0 408.533 \n/4/microtrans 0 2447.78 \n/4/rev 0.0531912 \n/4/steps 11 \n/4/interp_incr 222.526 \n/4/dur 12.2389 \n/4/direction 1 \n/5/hztrans 408.533 402 \n/5/microtrans 2447.78 2487.56 \n/5/rev 0.478723 \n/5/steps 96 \n/5/interp_incr 0.41438 \n/5/dur 236.877 \n/5/direction 1 \n/6/hztrans 402 0 \n/6/microtrans 2487.56 0 \n/6/rev 0.319149 \n/6/steps 64 \n/6/interp_incr -38.8682 \n/6/dur 80.8458 \n/6/direction 1 \n/7/hztrans 0 0 \n/7/microtrans 0 0 \n/7/rev 0.478723 \n/7/steps 96 \n/7/interp_incr 0 \n/7/dur 0 \n/7/direction 1 \n/8/hztrans 0 402 \n/8/microtrans 0 2487.56 \n/8/rev 0 \n/8/steps 0 \n/8/interp_incr 0 \n/8/dur 0 \n/8/direction 1 \n/9/hztrans 402 0 \n/9/microtrans 2487.56 0 \n/9/rev 0.106383 \n/9/steps 21 \n/9/interp_incr -118.455 \n/9/dur 27.3632 \n/9/direction 1 \n/10/hztrans 0 0 \n/10/microtrans 0 0 \n/10/rev 0.797873 \n/10/steps 160 \n/10/interp_incr 0 \n/10/dur 0 \n/10/direction 1 \n/11/hztrans 0 415.067 \n/11/microtrans 0 2409.25 \n/11/rev 0 \n/11/steps 0 \n/11/interp_incr 0 \n/11/dur 0 \n/11/direction 1 \n/12/hztrans 415.067 0 \n/12/microtrans 2409.25 0 \n/12/rev 0.106382 \n/12/steps 21 \n/12/interp_incr -114.726 \n/12/dur 26.5018 \n/12/direction 1 \n/13/hztrans 0 0 \n/13/microtrans 0 0 \n/13/rev 0.425532 \n/13/steps 85 \n/13/interp_incr 0 \n/13/dur 0 \n/13/direction 1 \n/14/hztrans 0 408.533 \n/14/microtrans 0 2447.78 \n/14/rev 0.319149 \n/14/steps 64 \n/14/interp_incr 38.2466 \n/14/dur 77.1051 \n/14/direction 1 \n/15/hztrans 408.533 0 \n/15/microtrans 2447.78 0 \n/15/rev 0.106382 \n/15/steps 21 \n/15/interp_incr -116.561 \n/15/dur 26.9256 \n/15/direction 1 \n/16/hztrans 0 0 \n/16/microtrans 0 0 \n/16/rev 0.851064 \n/16/steps 170 \n/16/interp_incr 0 \n/16/dur 0 \n/16/direction 1 \n/17/hztrans 0 264.8 \n/17/microtrans 0 3776.44 \n/17/rev 0.372341 \n/17/steps 74 \n/17/interp_incr 51.0329 \n/17/dur 137.84 \n/17/direction 1 \n/18/hztrans 264.8 264.8 \n/18/microtrans 3776.44 3776.44 \n/18/rev 0.744681 \n/18/steps 149 \n/18/interp_incr 0 \n/18/dur 562.689 \n/18/direction 1 \n/19/hztrans 264.8 134.133 \n/19/microtrans 3776.44 7455.28 \n/19/rev 0.957447 \n/19/steps 191 \n/19/interp_incr 19.261 \n/19/dur 1070.79 \n/19/direction 1 \n/20/hztrans 134.133 0 \n/20/microtrans 7455.28 0 \n/20/rev 0.797872 \n/20/steps 160 \n/20/interp_incr -46.5955 \n/20/dur 600.15 \n/20/direction 1 \n",
					"linecount" : 147,
					"patching_rect" : [ 1312.0, 24.0, 185.0, 2035.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "maquette",
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"patching_rect" : [ 72.0, 56.0, 300.0, 100.0 ],
					"outlettype" : [ "", "" ],
					"textcolor" : [  ],
					"id" : "obj-28",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"xmax" : 200.0,
					"bordercolor" : [ 0.0, 0.0, 0.1, 1.0 ],
					"numoutlets" : 2,
					"ymax" : 20.0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/maquette/x/left 36.3047 \n/maquette/x/right 44.4084 \n/maquette/y/bottom 16.1789 \n/maquette/y/top 17.3984 \n",
					"linecount" : 4,
					"patching_rect" : [ 904.0, 32.0, 165.0, 62.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "remove 0",
					"patching_rect" : [ 760.0, 136.0, 61.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-27",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "name maquette",
					"patching_rect" : [ 736.0, 248.0, 94.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-9",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "printtree",
					"patching_rect" : [ 464.0, 152.0, 55.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"patching_rect" : [ 544.0, 192.0, 20.0, 20.0 ],
					"outlettype" : [ "int" ],
					"id" : "obj-15",
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "drawrtreenodes $1",
					"patching_rect" : [ 544.0, 224.0, 111.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"patching_rect" : [ 1152.0, 832.0, 34.0, 20.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/x/left 7.13128 \n/x/right 9.15721 \n/y/top 6.17886 \n/y/bottom 6.78862 \n/maquette/x/left 30.389 \n/maquette/x/right 32.4149 \n/maquette/y/bottom 3.98374 \n/maquette/y/top 4.5935 \n",
					"linecount" : 8,
					"patching_rect" : [ 1104.0, 152.0, 205.0, 117.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/x/left 7.13128 \n/x/right 9.15721 \n/y/top 6.17886 \n/y/bottom 6.78862 \n",
					"linecount" : 4,
					"patching_rect" : [ 616.0, 128.0, 111.0, 62.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-21",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "name foo",
					"patching_rect" : [ 664.0, 248.0, 61.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-18",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"patching_rect" : [ 992.0, 176.0, 34.0, 20.0 ],
					"id" : "obj-23",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "tellmeeverything",
					"patching_rect" : [ 984.0, 136.0, 98.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/maquette/x/left 36.3047 \n/maquette/x/right 44.4084 \n/maquette/y/bottom 16.1789 \n/maquette/y/top 17.3984 \n",
					"linecount" : 4,
					"patching_rect" : [ 856.0, 208.0, 165.0, 62.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-20",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"patching_rect" : [ 856.0, 128.0, 50.0, 20.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-19",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.table maquette",
					"patching_rect" : [ 856.0, 168.0, 100.0, 20.0 ],
					"outlettype" : [ "", "" ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "delete",
					"patching_rect" : [ 664.0, 224.0, 43.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear",
					"patching_rect" : [ 504.0, 224.0, 37.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/maquette/x/left 72.285248 \n/maquette/x/right 92.000000 \n/maquette/y/bottom 5.000000 \n/maquette/y/top 6.869919 \n",
					"linecount" : 4,
					"patching_rect" : [ 48.0, 808.0, 205.0, 62.0 ],
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"patching_rect" : [ 384.0, 184.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ymax $1",
					"patching_rect" : [ 384.0, 216.0, 56.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-12",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"patching_rect" : [ 320.0, 184.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "ymin $1",
					"patching_rect" : [ 320.0, 216.0, 53.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"patching_rect" : [ 256.0, 184.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "xmax $1",
					"patching_rect" : [ 256.0, 216.0, 56.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-7",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"patching_rect" : [ 192.0, 184.0, 50.0, 20.0 ],
					"outlettype" : [ "float", "bang" ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "xmin $1",
					"patching_rect" : [ 192.0, 216.0, 53.0, 18.0 ],
					"outlettype" : [ "" ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "maquette",
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"patching_rect" : [ 48.0, 288.0, 1234.0, 492.0 ],
					"outlettype" : [ "", "" ],
					"textcolor" : [  ],
					"id" : "obj-26",
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"xmax" : 200.0,
					"bordercolor" : [ 0.0, 0.0, 0.1, 1.0 ],
					"numoutlets" : 2,
					"ymax" : 10.0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-13", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 1 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-20", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 1 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-34", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
