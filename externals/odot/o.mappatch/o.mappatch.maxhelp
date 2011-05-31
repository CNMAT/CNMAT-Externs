{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 339.0, 44.0, 1002.0, 491.0 ],
		"bglocked" : 0,
		"defrect" : [ 339.0, 44.0, 1002.0, 491.0 ],
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
					"maxclass" : "newobj",
					"text" : "o.print left-outlet",
					"patching_rect" : [ 140.0, 280.0, 97.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-8"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t l l",
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 126.0, 250.0, 32.5, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"presentation_rect" : [ 124.0, 250.0, 0.0, 0.0 ],
					"numoutlets" : 2,
					"id" : "obj-7"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print right-outlet",
					"patching_rect" : [ 280.0, 280.0, 94.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-5"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t l l",
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 260.0, 250.0, 32.5, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "anything we don't want to change can be sent back in unaltered.  any messages not returned into the right inlet will be removed from the bundle",
					"linecount" : 3,
					"patching_rect" : [ 670.0, 300.0, 282.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "make a change to the data, but keep the old address",
					"linecount" : 2,
					"patching_rect" : [ 420.0, 400.0, 160.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"presentation_rect" : [ 408.0, 318.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"id" : "obj-2"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "make a change to the data and bind it to a new address",
					"linecount" : 2,
					"patching_rect" : [ 240.0, 400.0, 160.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"presentation_rect" : [ 257.0, 322.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"id" : "obj-1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.mappatch outputs the contents of a bundle as a sequence of Max messages and constructs a new bundle from the data received in the right inlet",
					"linecount" : 2,
					"patching_rect" : [ 230.0, 150.0, 431.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-104"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend /midi",
					"outlettype" : [ "" ],
					"patching_rect" : [ 260.0, 370.0, 83.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-101"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 10.",
					"outlettype" : [ "float" ],
					"patching_rect" : [ 430.0, 340.0, 34.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-100"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/midi 69 \n/gain 0.1 \n/nada 0 \n",
					"linecount" : 3,
					"outlettype" : [ "" ],
					"patching_rect" : [ 84.0, 310.0, 61.0, 48.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-97"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "ftom 0.",
					"outlettype" : [ "" ],
					"patching_rect" : [ 260.0, 340.0, 47.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-96"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /freq /gain",
					"outlettype" : [ "", "", "FullPacket" ],
					"patching_rect" : [ 260.0, 310.0, 359.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 3,
					"id" : "obj-84"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/freq 440.\n/gain 1.\n/nada 0.",
					"linecount" : 3,
					"outlettype" : [ "" ],
					"patching_rect" : [ 126.0, 148.0, 64.0, 48.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-81"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.mappatch",
					"outlettype" : [ "FullPacket", "" ],
					"patching_rect" : [ 126.0, 220.0, 153.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"outlettype" : [ "" ],
					"patching_rect" : [ 953.0, 56.0, 50.0, 18.0 ],
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
					"patching_rect" : [ 697.0, 39.0, 50.0, 18.0 ],
					"fontsize" : 12.0,
					"hidden" : 1,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-29"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.mappatch",
					"fontface" : 1,
					"patching_rect" : [ 700.0, 39.0, 186.0, 23.0 ],
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
					"patching_rect" : [ 55.0, 31.0, 84.020836, 37.0 ],
					"autofit" : 1,
					"embed" : 1,
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-33",
					"data" : [ 593, "", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"bgcolor" : [ 0.666667, 0.666667, 0.666667, 0.0 ],
					"patching_rect" : [ 55.0, 31.0, 909.0, 421.0 ],
					"rounded" : 0,
					"border" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-36"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-7", 1 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-97", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 1 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 1 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-84", 2 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [ 609.5, 337.0, 639.0, 337.0, 639.0, 214.0, 269.5, 214.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 0 ],
					"destination" : [ "obj-101", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-100", 0 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [ 439.5, 364.0, 651.0, 364.0, 651.0, 208.0, 269.5, 208.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-101", 0 ],
					"destination" : [ "obj-6", 1 ],
					"hidden" : 0,
					"midpoints" : [ 269.5, 395.0, 662.0, 395.0, 662.0, 203.0, 269.5, 203.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-84", 1 ],
					"destination" : [ "obj-100", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-84", 0 ],
					"destination" : [ "obj-96", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-81", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
