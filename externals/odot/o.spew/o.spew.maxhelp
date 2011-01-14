{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 317.0, 44.0, 584.0, 428.0 ],
		"bglocked" : 0,
		"defrect" : [ 317.0, 44.0, 584.0, 428.0 ],
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
					"maxclass" : "textbutton",
					"textoncolor" : [ 0.0, 0.0, 1.0, 1.0 ],
					"border" : 0,
					"outlettype" : [ "", "", "int" ],
					"textcolor" : [ 0.0, 0.0, 1.0, 1.0 ],
					"text" : "http://cnmat.berkeley.edu/downloads",
					"textovercolor" : [ 0.0, 0.0, 1.0, 1.0 ],
					"id" : "obj-17",
					"fontname" : "Arial",
					"bgovercolor" : [ 0.698039, 0.698039, 0.698039, 0.0 ],
					"textoveroncolor" : [ 0.0, 0.0, 1.0, 1.0 ],
					"bgcolor" : [ 0.74902, 0.74902, 0.74902, 0.0 ],
					"bgoveroncolor" : [ 0.501961, 0.501961, 0.501961, 0.0 ],
					"patching_rect" : [ 50.0, 380.0, 223.0, 21.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"bgoncolor" : [ 0.54902, 0.54902, 0.54902, 0.0 ],
					"numoutlets" : 3
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\rmax launchbrowser http://cnmat.berkeley.edu/downloads",
					"linecount" : 2,
					"outlettype" : [ "" ],
					"id" : "obj-15",
					"fontname" : "Arial",
					"patching_rect" : [ 60.0, 390.0, 312.0, 32.0 ],
					"numinlets" : 2,
					"fontsize" : 12.0,
					"hidden" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "fpic",
					"id" : "obj-36",
					"patching_rect" : [ 60.0, 330.0, 109.0, 48.0 ],
					"numinlets" : 1,
					"embed" : 1,
					"numoutlets" : 0,
					"data" : [ 593, "", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "This behavior is like o.route, except that the addresses remain in tact.",
					"linecount" : 3,
					"id" : "obj-12",
					"presentation_rect" : [ 278.0, 248.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 280.0, 250.0, 177.0, 48.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print unmatched",
					"id" : "obj-11",
					"fontname" : "Arial",
					"patching_rect" : [ 160.0, 250.0, 97.0, 20.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Arguments specify which outlet a message should come out.  All unmatched messages will come out the right outlet.",
					"linecount" : 4,
					"id" : "obj-9",
					"presentation_rect" : [ 196.0, 192.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 280.0, 190.0, 177.0, 62.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"id" : "obj-8",
					"fontname" : "Arial",
					"patching_rect" : [ 60.0, 120.0, 34.0, 20.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo bar /foo/bar 666",
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-7",
					"presentation_rect" : [ 58.0, 41.0, 0.0, 0.0 ],
					"fontname" : "Arial",
					"patching_rect" : [ 60.0, 60.0, 124.0, 20.0 ],
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "With no arguments, o.spew behaves like the left outlet of the OpenSoundControl object.  A bundle comes in and max messages come out",
					"linecount" : 5,
					"id" : "obj-6",
					"fontname" : "Arial",
					"patching_rect" : [ 200.0, 60.0, 170.0, 75.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.spew",
					"outlettype" : [ "FullPacket" ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"patching_rect" : [ 60.0, 90.0, 49.0, 20.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print 2",
					"id" : "obj-3",
					"fontname" : "Arial",
					"patching_rect" : [ 110.0, 250.0, 44.0, 20.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo bar /foo/bar 666 /nada 0.",
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"patching_rect" : [ 60.0, 190.0, 163.0, 20.0 ],
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print 1",
					"id" : "obj-10",
					"fontname" : "Arial",
					"patching_rect" : [ 60.0, 250.0, 44.0, 20.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.spew /foo/bar /foo",
					"outlettype" : [ "", "", "FullPacket" ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"patching_rect" : [ 60.0, 220.0, 116.0, 20.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 3
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 2 ],
					"destination" : [ "obj-11", 0 ],
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
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
