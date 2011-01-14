{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 521.0, 44.0, 435.0, 258.0 ],
		"bglocked" : 0,
		"defrect" : [ 521.0, 44.0, 435.0, 258.0 ],
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
					"patching_rect" : [ 40.0, 190.0, 223.0, 21.0 ],
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
					"patching_rect" : [ 50.0, 200.0, 312.0, 32.0 ],
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
					"patching_rect" : [ 50.0, 140.0, 109.0, 48.0 ],
					"numinlets" : 1,
					"embed" : 1,
					"numoutlets" : 0,
					"data" : [ 593, "", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Post the contents of an OSC bundle to the Max window as a list of bytes (useful for debugging...)",
					"linecount" : 4,
					"id" : "obj-3",
					"fontname" : "Arial",
					"patching_rect" : [ 230.0, 70.0, 150.0, 62.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.printbytes",
					"id" : "obj-2",
					"fontname" : "Arial",
					"patching_rect" : [ 50.0, 80.0, 73.0, 20.0 ],
					"numinlets" : 1,
					"fontsize" : 12.0,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 15 bar 89.333 /bar 25",
					"outlettype" : [ "" ],
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"patching_rect" : [ 50.0, 30.0, 164.0, 20.0 ],
					"numinlets" : 2,
					"fontsize" : 12.0,
					"numoutlets" : 1
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
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
