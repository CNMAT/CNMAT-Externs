{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 317.0, 44.0, 1081.0, 572.0 ],
		"bglocked" : 0,
		"defrect" : [ 317.0, 44.0, 1081.0, 572.0 ],
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
					"maxclass" : "comment",
					"text" : "with no arguments, o.spew simply unpacks the bundle and outputs each message as a Max message.",
					"linecount" : 3,
					"patching_rect" : [ 765.0, 285.0, 217.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-43"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"patching_rect" : [ 765.0, 435.0, 34.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-42"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/2/freq 440.\n/mysynth/2/gain .5\n/nada rien",
					"linecount" : 3,
					"outlettype" : [ "" ],
					"patching_rect" : [ 765.0, 345.0, 121.0, 48.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-41"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.spew",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 765.0, 405.0, 49.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-40"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.spew is like o.select except that it outputs Max messages",
					"linecount" : 2,
					"patching_rect" : [ 765.0, 165.0, 217.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-39"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/mysynth/2/gain 0.5",
					"outlettype" : [ "" ],
					"patching_rect" : [ 895.0, 245.0, 115.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-38"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "/mysynth/2/freq 160.",
					"outlettype" : [ "" ],
					"patching_rect" : [ 765.0, 245.0, 119.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-37"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.spew /mysynth/2/freq /mysynth/2/gain",
					"outlettype" : [ "", "", "FullPacket" ],
					"patching_rect" : [ 765.0, 215.0, 221.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 3,
					"id" : "obj-34"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.select is the same as o.route except that it leaves the address in tact",
					"linecount" : 2,
					"patching_rect" : [ 485.0, 165.0, 217.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-32"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/2/gain 0.5 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 605.0, 245.0, 119.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-31"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/2/freq 160 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 485.0, 245.0, 121.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-30"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.select /mysynth/2/freq /mysynth/2/gain",
					"outlettype" : [ "", "", "FullPacket" ],
					"patching_rect" : [ 485.0, 215.0, 225.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 3,
					"id" : "obj-28"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/freq 160 \n/gain 0.5 \n",
					"linecount" : 2,
					"outlettype" : [ "" ],
					"patching_rect" : [ 395.0, 325.0, 83.0, 34.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-27"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/freq 29 \n/gain 0.5 \n",
					"linecount" : 2,
					"outlettype" : [ "" ],
					"patching_rect" : [ 85.0, 325.0, 83.0, 34.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-26"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/2/freq 160 \n/2/gain 0.5 \n",
					"linecount" : 2,
					"outlettype" : [ "" ],
					"patching_rect" : [ 85.0, 255.0, 83.0, 34.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-24"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.route strips off the portion of the address that matched",
					"linecount" : 2,
					"patching_rect" : [ 285.0, 215.0, 161.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-23"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 295.0, 125.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-17"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/*/freq $1\n/mysynth/*/gain .5\n/nada rien",
					"linecount" : 3,
					"outlettype" : [ "" ],
					"patching_rect" : [ 295.0, 155.0, 111.0, 48.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-18"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 335.0, 355.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-15"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 285.0, 355.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-16"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 225.0, 355.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-14"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 175.0, 355.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-13"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<--unmatched (delegation)",
					"patching_rect" : [ 285.0, 265.0, 150.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-11"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/nada rien \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 205.0, 265.0, 69.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-9"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /freq /gain",
					"outlettype" : [ "", "", "FullPacket" ],
					"patching_rect" : [ 285.0, 325.0, 105.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 3,
					"id" : "obj-8"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 175.0, 125.0, 50.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-7"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/mysynth/2/freq $1\n/mysynth/2/gain .5\n/nada rien",
					"linecount" : 3,
					"outlettype" : [ "" ],
					"patching_rect" : [ 175.0, 155.0, 111.0, 48.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-5"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /freq /gain",
					"outlettype" : [ "", "", "FullPacket" ],
					"patching_rect" : [ 175.0, 325.0, 105.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 3,
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /1 /2",
					"outlettype" : [ "", "", "FullPacket" ],
					"patching_rect" : [ 175.0, 295.0, 239.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 3,
					"id" : "obj-3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.route /mysynth",
					"outlettype" : [ "", "FullPacket" ],
					"patching_rect" : [ 175.0, 215.0, 99.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 2,
					"id" : "obj-2"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"outlettype" : [ "" ],
					"patching_rect" : [ 1016.0, 61.0, 50.0, 18.0 ],
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
					"patching_rect" : [ 766.0, 44.0, 50.0, 18.0 ],
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
					"text" : "o.route/select/spew",
					"fontface" : 1,
					"patching_rect" : [ 769.0, 44.0, 186.0, 23.0 ],
					"fontsize" : 14.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "fpic",
					"patching_rect" : [ 40.0, 36.0, 84.020836, 37.0 ],
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
					"patching_rect" : [ 40.0, 36.0, 986.0, 497.0 ],
					"rounded" : 0,
					"border" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"id" : "obj-36"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "See the OSC-route help file for an overview of the OSC pattern matching syntax",
					"linecount" : 2,
					"patching_rect" : [ 95.0, 425.0, 315.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-22"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p who_cares?",
					"patching_rect" : [ 655.0, 475.0, 85.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 0,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-20",
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 341.0, 44.0, 532.0, 577.0 ],
						"bglocked" : 0,
						"defrect" : [ 341.0, 44.0, 532.0, 577.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "button",
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 64.0, 328.0, 20.0, 20.0 ],
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-13"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "Here, since o.route received all of the messages bundled together, it can dispatch them through the outlets in right to left order observing normal Max convention.",
									"linecount" : 4,
									"patching_rect" : [ 192.0, 400.0, 252.0, 62.0 ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"fontname" : "Arial",
									"numoutlets" : 0,
									"id" : "obj-14"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "/gain 1.",
									"outlettype" : [ "" ],
									"patching_rect" : [ 304.0, 344.0, 50.0, 18.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-15"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "/freq 440.",
									"outlettype" : [ "" ],
									"patching_rect" : [ 280.0, 320.0, 61.0, 18.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-16"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "*~ 0.",
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 64.0, 488.0, 36.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-17"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 440.",
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 64.0, 456.0, 72.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-18"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.route /freq /gain",
									"outlettype" : [ "", "", "FullPacket" ],
									"patching_rect" : [ 64.0, 400.0, 105.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"fontname" : "Arial",
									"numoutlets" : 3,
									"id" : "obj-19"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "o.packet @clearonbang 1 @accumulate 1",
									"outlettype" : [ "" ],
									"patching_rect" : [ 64.0, 368.0, 234.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-21"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "button",
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 184.0, 64.0, 20.0, 20.0 ],
									"numinlets" : 1,
									"numoutlets" : 1,
									"id" : "obj-12"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "comment",
									"text" : "In this case, the order that OSC-route will dispatch its messages will be determined by the order in which the messages were added to the OSC bundle.  In many cases this is not a problem, however, it is not always possible to ensure that the contents of the bundle will be in the order you want them in.  ",
									"linecount" : 7,
									"patching_rect" : [ 208.0, 48.0, 252.0, 103.0 ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"fontname" : "Arial",
									"numoutlets" : 0,
									"id" : "obj-10"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "/gain 1.",
									"outlettype" : [ "" ],
									"patching_rect" : [ 128.0, 64.0, 50.0, 18.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-9"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "/freq 440.",
									"outlettype" : [ "" ],
									"patching_rect" : [ 64.0, 64.0, 61.0, 18.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-8"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "*~ 0.",
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 112.0, 248.0, 36.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-6"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~ 440.",
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 112.0, 216.0, 72.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 2,
									"fontname" : "Arial",
									"numoutlets" : 1,
									"id" : "obj-5"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OSC-route /freq /gain",
									"outlettype" : [ "", "", "" ],
									"patching_rect" : [ 112.0, 160.0, 125.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"fontname" : "Arial",
									"numoutlets" : 3,
									"id" : "obj-4"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OpenSoundControl",
									"outlettype" : [ "", "", "OSCTimeTag" ],
									"patching_rect" : [ 64.0, 128.0, 113.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"fontname" : "Arial",
									"numoutlets" : 3,
									"id" : "obj-3"
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "OpenSoundControl",
									"outlettype" : [ "", "", "OSCTimeTag" ],
									"patching_rect" : [ 64.0, 96.0, 113.0, 20.0 ],
									"fontsize" : 12.0,
									"numinlets" : 1,
									"fontname" : "Arial",
									"numoutlets" : 3,
									"id" : "obj-1"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-19", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-19", 1 ],
									"destination" : [ "obj-17", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-18", 0 ],
									"destination" : [ "obj-17", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-1", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 1 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 1 ],
									"destination" : [ "obj-6", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-21", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-21", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-21", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-21", 0 ],
									"destination" : [ "obj-19", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"fontface" : 0,
						"fontsize" : 12.0,
						"globalpatchername" : "",
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"fontname" : "Arial",
						"default_fontsize" : 12.0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.route behaves like OSC-route except that it has the ability to operate on the contents of OSC bundles.",
					"patching_rect" : [ 95.0, 475.0, 561.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-19"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-37", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 1 ],
					"destination" : [ "obj-38", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-30", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 1 ],
					"destination" : [ "obj-31", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 1 ],
					"destination" : [ "obj-27", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-26", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-24", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 1 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 1 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 1 ],
					"destination" : [ "obj-9", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 1 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
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
 ]
	}

}
