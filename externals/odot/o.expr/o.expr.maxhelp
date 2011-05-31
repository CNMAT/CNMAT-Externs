{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 416.0, 44.0, 917.0, 849.0 ],
		"bglocked" : 0,
		"defrect" : [ 416.0, 44.0, 917.0, 849.0 ],
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
					"outlettype" : [ "" ],
					"patching_rect" : [ 528.0, 624.0, 50.0, 18.0 ],
					"fontsize" : 12.0,
					"hidden" : 1,
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 801.0, 390.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-50"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"outlettype" : [ "" ],
					"patching_rect" : [ 528.0, 376.0, 50.0, 18.0 ],
					"fontsize" : 12.0,
					"hidden" : 1,
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 550.0, 373.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-51"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Functions and constants",
					"patching_rect" : [ 64.0, 544.0, 192.0, 20.0 ],
					"fontsize" : 12.0,
					"underline" : 1,
					"numinlets" : 1,
					"fontname" : "Arial",
					"presentation_rect" : [ 47.0, 538.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"id" : "obj-49"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Index lookup",
					"patching_rect" : [ 64.0, 384.0, 192.0, 20.0 ],
					"fontsize" : 12.0,
					"underline" : 1,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-48"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "o.expr understands certain constants such as PI and E",
					"linecount" : 2,
					"patching_rect" : [ 552.0, 264.0, 210.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"presentation_rect" : [ 537.0, 264.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"id" : "obj-47"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "OSC addresses can be used as parameters to the expression",
					"linecount" : 2,
					"patching_rect" : [ 552.0, 176.0, 210.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"presentation_rect" : [ 535.0, 176.0, 0.0, 0.0 ],
					"numoutlets" : 0,
					"id" : "obj-46"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 4 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 408.0, 248.0, 64.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 389.0, 361.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-43"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 7.14159 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 408.0, 296.0, 82.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 389.0, 409.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-44"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo + PI @as /foo",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 408.0, 272.0, 144.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-45"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 4 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 408.0, 160.0, 64.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 238.0, 357.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-38"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 6 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 408.0, 208.0, 64.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"presentation_rect" : [ 236.0, 357.0, 0.0, 0.0 ],
					"numoutlets" : 1,
					"id" : "obj-4"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo + 2 @as /foo",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 408.0, 184.0, 139.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-30"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 1 2 3 4 5 6 7 8 9 10 \n/result 5 6 7 \n",
					"linecount" : 2,
					"outlettype" : [ "" ],
					"patching_rect" : [ 160.0, 472.0, 142.0, 34.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-42"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 1 2 3 4 5 6 7 8 9 10",
					"outlettype" : [ "" ],
					"patching_rect" : [ 160.0, 424.0, 135.0, 20.0 ],
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
					"text" : "o.expr /foo[[4\\,5\\,6]]",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 160.0, 448.0, 114.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-40"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 616.0, 392.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-32"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr range(1\\, length(/foo)\\, 2) @as /oddi",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 616.0, 464.0, 237.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-28"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr range(0\\, length(/foo)\\, 2) @as /eveni",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 616.0, 440.0, 243.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-26"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 0 1 2 3 4 5 6 7 8 9 \n/eveni 0 2 4 6 8 \n/oddi 1 3 5 7 \n/even 0 2 4 6 8 \n/odd 1 3 5 7 \n",
					"linecount" : 5,
					"outlettype" : [ "" ],
					"patching_rect" : [ 616.0, 544.0, 135.0, 75.0 ],
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
					"maxclass" : "newobj",
					"text" : "o.expr range(0\\, 10) @as /foo",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 616.0, 416.0, 167.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-27"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo[[/oddi]] @as /odd",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 616.0, 512.0, 161.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-24"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo[[/eveni]] @as /even",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 616.0, 488.0, 173.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-23"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "[[ ]] can be used to extract the nth (counting from 0) item from a list",
					"linecount" : 2,
					"patching_rect" : [ 312.0, 440.0, 190.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-22"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 4 5 6 \n/result 5 \n",
					"linecount" : 2,
					"outlettype" : [ "" ],
					"patching_rect" : [ 64.0, 472.0, 65.0, 34.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-21"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 4 5 6",
					"outlettype" : [ "" ],
					"patching_rect" : [ 64.0, 424.0, 64.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-19"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr /foo[[1]]",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 64.0, 448.0, 87.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-18"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "@as changes the address to which the results are bound",
					"linecount" : 2,
					"patching_rect" : [ 192.0, 264.0, 173.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-12"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/foo 4 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 64.0, 296.0, 64.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-15"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 64.0, 248.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-16"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr 2 + 2 @as /foo",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 64.0, 272.0, 126.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-17"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "results of the computation are bound to the address \"/result\" by default",
					"linecount" : 2,
					"patching_rect" : [ 144.0, 176.0, 210.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-6"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "o.message",
					"text" : "/result 4 \n",
					"outlettype" : [ "" ],
					"patching_rect" : [ 64.0, 208.0, 64.0, 20.0 ],
					"fontsize" : 12.0,
					"textcolor" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-14"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 64.0, 160.0, 20.0, 20.0 ],
					"numinlets" : 1,
					"numoutlets" : 1,
					"id" : "obj-13"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr 2 + 2",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 64.0, 184.0, 74.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-11"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "post the function graph and output of the tokenizer to the Max window",
					"linecount" : 3,
					"patching_rect" : [ 184.0, 728.0, 181.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-10"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-argvlex",
					"outlettype" : [ "" ],
					"patching_rect" : [ 80.0, 752.0, 76.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-9"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-functiongraph",
					"outlettype" : [ "" ],
					"patching_rect" : [ 64.0, 728.0, 110.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-8"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr sum((/yy*(0.5*cos((PI*(/x-/xx))/ /w)+.5)) * (/x >= (/xx - /w) && /x <= (/xx + /w))) @as /y",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 64.0, 784.0, 496.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-7"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "post a list of the functions and constants that are available in o.expr to the Max window",
					"linecount" : 3,
					"patching_rect" : [ 176.0, 584.0, 181.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-5"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-constants",
					"outlettype" : [ "" ],
					"patching_rect" : [ 80.0, 608.0, 89.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-3"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "post-functions",
					"outlettype" : [ "" ],
					"patching_rect" : [ 64.0, 584.0, 85.0, 18.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-2"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "o.expr",
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 64.0, 640.0, 44.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 1,
					"id" : "obj-1"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"outlettype" : [ "" ],
					"patching_rect" : [ 866.0, 57.0, 50.0, 18.0 ],
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
					"patching_rect" : [ 616.0, 40.0, 50.0, 18.0 ],
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
					"maxclass" : "fpic",
					"patching_rect" : [ 48.0, 32.0, 84.020836, 37.0 ],
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
					"patching_rect" : [ 48.0, 32.0, 828.0, 795.0 ],
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
					"text" : "o.expr",
					"fontface" : 1,
					"patching_rect" : [ 619.0, 40.0, 186.0, 23.0 ],
					"fontsize" : 14.0,
					"numinlets" : 1,
					"fontname" : "Arial",
					"numoutlets" : 0,
					"id" : "obj-20"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-44", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-4", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-14", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-7", 0 ],
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
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
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
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-15", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-21", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-31", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-27", 0 ],
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
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-42", 1 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
