{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 51.0, 44.0, 802.0, 367.0 ],
		"bglocked" : 0,
		"defrect" : [ 51.0, 44.0, 802.0, 367.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 10.0, 10.0 ],
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
					"outlettype" : [ "" ],
					"patching_rect" : [ 752.0, 56.0, 50.0, 18.0 ],
					"fontsize" : 12.0,
					"hidden" : 1,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-25"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"outlettype" : [ "" ],
					"patching_rect" : [ 501.0, 39.0, 50.0, 18.0 ],
					"fontsize" : 12.0,
					"hidden" : 1,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-35"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.prepend",
					"fontface" : 1,
					"patching_rect" : [ 504.0, 39.0, 186.0, 23.0 ],
					"fontsize" : 14.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-20"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "fpic",
					"patching_rect" : [ 45.0, 31.0, 84.020836, 37.0 ],
					"autofit" : 1,
					"embed" : 1,
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-36",
					"data" : [ 593, "", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"patching_rect" : [ 45.0, 31.0, 719.0, 297.0 ],
					"rounded" : 0,
					"border" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-37"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend /foo",
					"outlettype" : [ "" ],
					"patching_rect" : [ 640.0, 170.0, 78.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-34"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/bar 7 8 9 ",
					"outlettype" : [ "" ],
					"patching_rect" : [ 640.0, 140.0, 91.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 699.0, 172.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-33"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo/bar 7 8 9 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 640.0, 230.0, 91.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 702.0, 238.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-31"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.prepend",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 640.0, 200.0, 65.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"presentation_rect" : [ 702.0, 208.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-32"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "prepend /foo to /bar",
					"patching_rect" : [ 450.0, 180.0, 170.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"presentation_rect" : [ 529.0, 209.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"id" : "obj-30"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo/bar 7 8 9 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 370.0, 210.0, 91.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 427.0, 240.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-29"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/foo /bar 7 8 9",
					"outlettype" : [ "" ],
					"patching_rect" : [ 370.0, 140.0, 85.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 433.0, 172.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-28"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.prepend",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 370.0, 180.0, 65.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-27"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/bar 7 8 9",
					"outlettype" : [ "" ],
					"patching_rect" : [ 70.0, 140.0, 61.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 538.0, 162.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-26"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "prepend /foo to every address",
					"patching_rect" : [ 160.0, 180.0, 170.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"presentation_rect" : [ 557.0, 208.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"id" : "obj-24"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo/bar 7 8 9 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 70.0, 210.0, 91.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 470.0, 243.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-23"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/bar 1 2 3\n/bloo 4 5 6",
					"linecount" : 2,
					"outlettype" : [ "" ],
					"patching_rect" : [ 140.0, 140.0, 64.0, 34.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-22"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.prepend /foo",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 70.0, 180.0, 88.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-21"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-25", 0 ],
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
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-31", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-29", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-23", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
