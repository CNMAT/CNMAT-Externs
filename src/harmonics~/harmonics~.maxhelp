{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 6,
			"minor" : 0,
			"revision" : 7
		}
,
		"rect" : [ -6.0, 44.0, 818.0, 682.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 10.0, 10.0 ],
		"gridsnaponopen" : 0,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"boxes" : [ 			{
				"box" : 				{
					"args" : [ 4069 ],
					"id" : "obj-1",
					"lockeddragscroll" : 1,
					"maxclass" : "bpatcher",
					"name" : "html_reference.maxpat",
					"numinlets" : 0,
					"numoutlets" : 0,
					"offset" : [ -68.0, -58.0 ],
					"patching_rect" : [ 457.0, 471.0, 131.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-2",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 46.0, 613.0, 48.0, 17.0 ],
					"text" : "see also:",
					"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"hidden" : 1,
					"id" : "obj-3",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 46.0, 649.0, 64.0, 17.0 ],
					"text" : "prepend help"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-4",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 46.0, 627.0, 57.0, 15.0 ],
					"text" : "sinusoids~"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"hidden" : 1,
					"id" : "obj-5",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 46.0, 670.0, 45.0, 17.0 ],
					"text" : "pcontrol"
				}

			}
, 			{
				"box" : 				{
					"handoff" : "",
					"id" : "obj-6",
					"maxclass" : "ubutton",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"patching_rect" : [ 491.0, 601.0, 268.0, 26.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-7",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 552.0, 614.0, 196.0, 17.0 ],
					"text" : "http://www.cnmat.berkeley.edu/MAX/",
					"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"hidden" : 1,
					"id" : "obj-8",
					"linecount" : 2,
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 490.0, 649.0, 294.0, 25.0 ],
					"text" : ";\rmax launch_browser http://www.cnmat.berkeley.edu/MAX/"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-9",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 488.0, 629.0, 256.0, 17.0 ],
					"text" : "Adrian Freed ©2006, UC Regents. All Rights Reserved"
				}

			}
, 			{
				"box" : 				{
					"data" : [ 593, "png", "IBkSG0fBZn....PCIgDQRA...zF....LHX....vFnS+e....DLmPIQEBHf.B7g.YHB..BfPRDEDU3wY6bEsiCCBCKLc+++xbOwItcqkVHN1oGVZZRSCWZbLgNfUp0Z01HU3K1cfMLqTJC+N8dqOJZ2kjQs0Cy7Sk2qDqaeuV6esBIWssWky6bs9Oy6qO8gH.RtmAn5O2k2Yb0SWSalKVuE2KffyYwrIBm0++DmmJZpDLx.NpFFBG8epo4IVovKR7deHaImPEMTPsfLpYfdDfKZQ31Tv8FIBwoolyvSDsKyLhCOxxcftdVDIngIZnCNLRBXk3QchHOsZQQUFHTQSsZaqzeXlvQeJ+Qdyi7ZEYBY3hlm2brpqwdXc5NMy3GDVEQOrOEQSgZay1GTHASBmlY3CFn3mQBHMQyqa1HqqofKyLxNMEd.4U.qg4kY3wHwrKfqJftn4gaajHfHfybxTzEMD3ouzORHZYq1F6GYQBQKRb2.thaMAYDsUcaG0d0csy.YDMEghtLyDSzxVsMVPJQyLeEtUbJp5xLavlUMaY5n1bndBO5e+xogbsthpsHfJ2KGdpY73hfjiQbGc6tB7lix9jflOH2DQ1XL1hVBwVzRH1hVBwVzRH1hVBws+KoX0mPXzuH.5spcV4um2ScZ0Z8mWdidNQehZP0+QFaNi6gCOVJkTtmAabiLgqEWP7O1P+6uC5G0ITBWi6mxgtnGCEMTCAzPV2bosQfXrpBzVZldtQu6oxF+i3b+CFmP7MQ+TMlEbQ3RG.....IUjSD4pPfIH" ],
					"embed" : 1,
					"id" : "obj-10",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 487.0, 515.0, 109.0, 48.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-11",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 490.0, 602.0, 263.0, 27.0 ],
					"text" : "harmonics~, and all other CNMAT Max objects, can be found at:"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-12",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 599.0, 539.0, 179.0, 27.0 ],
					"text" : "harmonics~.help version 1.1 by Adrian Freed & Michael Zbyszynski"
				}

			}
, 			{
				"box" : 				{
					"color" : [ 0.4, 0.4, 0.8, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"id" : "obj-13",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 6,
							"minor" : 0,
							"revision" : 7
						}
,
						"rect" : [ 10.0, 59.0, 692.0, 323.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 10.0, 10.0 ],
						"gridsnaponopen" : 0,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"boxes" : [ 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-1",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 540.0, 251.0, 100.0, 20.0 ],
									"text" : "triangle",
									"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-2",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 300.0, 251.0, 100.0, 20.0 ],
									"text" : "sawtooth",
									"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"data" : [ 1507, "png", "IBkSG0fBZn....PCIgDQRA...TL...P3HX.....dEDGz....DLmPIQEBHf.B7g.YHB..EnYRDEDU3wY6bGsaipDDD.EVk++eYtObEY8VxNF7LDZXNGoHqMOfKiooXvdy7xxxxDv295rCvUz77bW2dsddopkmqt+b1A.plCooXdddHNaiyveO08lhd+FM7aqqCEiRCA2accnv.A2AVnMDLT.ACEP3OSStiQvizT.ACEP3PFJbqY4JSSADLT.ACEPnoukrexsx05Mn55RSw5A5u6Q3Jn4++Trrr782N1s73Y5re9ekplqQUyMEWkABXqFplhpcYbUKO7+zT.glGJ1SSAbEno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BZJfflBHno.BCWSQUxA00P0TXffsXXZJpxfI02vzTb1O+bcLLMEvVMLMEvVoo.BZJfflBHno.BCWSggSdmuZcCrmlhyZvXqOuFXXZpCCEUefXZxA6rOVSADFhlhGy5JCo7JCSSwZFV+oBCoTSCyceJGJqPlnlFllhTEyD0vPslhoo+ttBCD7JCWSg0Tv6LLqoHUwLQMLbMEv6LLME4yuAUdkg369zi47w+M7LC0cexf.ag0T.ggYMEWE1Oc9zTTHFHpAMEEgSbTG2569zm77cVGXZfnNFhlh0C3d2ivzz.rlhpmOpmaeSQ0yG0ilBMEDzTnofflBMEDzTTrlBCmmua8mSwUHeSS9KXX0b6+VxV87MM4f8pwZJrlhcoJWl4QRSQAZJVy4yTog0ydezuEqonPCFUZ.HUk8Q+Fb2mFn2raQkGX6MqonPqoXdd96e37nonPMEKK9C.cq5w9MMEEooHe9MXre8Z+klhB0TvmqmmXSSQQZJnM878OMEEooHe9MnddzTTjlh0br9yYmmWop4pm7IZWjlhoo5d.2V22T07uW9DsK1fQEcWNXeqrlBCDDzTXvXSd09l6XKh0TXfXSd1A+UbeVOFR0Tbh46Sd9timYtUaY+3d1uoon.MEUOeOSktsw8NG9bJjOBt6Sx2tU4gz441+p2qoP9tMV2O05IQzTHe6RUGPyb0x9LMExGAMExGAMEx2tTkbjxSbXMExGS+6W+9VFd0THe2Jq28oV3SzV99wrsWm4v6im7nkSjnoP9da91yimodc4SZJjuKc9R8X3TSg7coy2QvceR9tz46HnoP9tz46HXMEx2kMeexy4VFd0THeW97smG2BqoP9juflB4S9BZJjO4KnoP9juflB4S9BZJjO4KnoP9juflB4S9BZJjO4KnoP9juflB4S9BWllhOcmt7Iekno3m98+1jO4qDME+zu+2l7IeZJjO4qw7ooP9juflB4S9BZJjO4KnoP9jubatrrrbVuffp3wljuxeAL5Z9xmf6FCEPn4+Xn8J87NR73ZdZYa9r0N0yKcrGulOpLttcq39ud89au1VGxPQOW3ddfVKG30qsyyzyWy8dMd850ZtM5wq4d99au1VGxkO0y2TOhaBvQLPT0aVQkyVUcXW9zQnmWdRuNX4HNnqmWNwQr8V2l8p84HxWKtLCEGwk.z517HFHd1knz5ywQdYisp2WdbOtjtKwPQ0di7Q4aBUNq8RkeMlCXehxeKYOp6hUOrrr7O+r96ZgucA8QKG2bYZJdTupWq3Y65cFq9q4dmutbqmWp1dI3jU9KeB9s8e.rwPlFnMNeMB....PRE4DQtJDXBB" ],
									"embed" : 1,
									"id" : "obj-3",
									"maxclass" : "fpic",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 7.0, 22.0, 197.0, 225.0 ]
								}

							}
, 							{
								"box" : 								{
									"data" : [ 1858, "png", "IBkSG0fBZn....PCIgDQRA...XL...v3HX....f25uF1....DLmPIQEBHf.B7g.YHB..FjeRDEDU3wY6c2taihDDE.EVMu+uxr+YICKphswTcS+w4HEEkQiuF6P4a2DOYV2111V.9e9mm9..ZQFLZ.qqqKqqqMYdYer0KLX7vx9jtrGvlUFLdPFJZWEavX1eh8cLTz1JxfgmXesiO+jwEELy7x9XqWk9fw555T+D56XnnOj9fwr+D5qXnneXy2Uhgh9xOCF1WP4Xnn+nwnvLTzmLXTPFJ5WFLJDCE8MCFEfgh9WwFLl0mzMTLFzXjHCEiCCFIwPwX4O2Mfu4m+wn8MGCEimzZL1eR+cedzXnXLkxfw111Ou4Ae2mGIs7PwQi1y60PJCFe5PwH81NojuZrghmmFiufghw2s278xxb1XrrLGCEy5vUJCFWowXDFNZ4AhRj2LxdLf.1iADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADPiADnqZLLXQszMMFFJnl5hFC6OgZqKZLLTPs0EMFPs0EMFPsow.Bnw.Bnw.Bnw.BzUMFZbnV9SFgTxFiO81XngLkxfwUZLt5vgS34IXOFPftZOFqqqFtnJ5lFi8Lz7PMzEMFmusFNnz5lFCnl5hFiy2N+Tzoz5tFCCETCcQiwNCETKcSiggBpotnwvPA0Vy+dk538wYFVnTZ92qT64C0T2rGCnl5h8X.0VysGCK0hVPS1Xr+26S+LjslbOF1yBOslswvdV3Iow.BLsMFFp3UlxFCCE7NSWig8lvmX5ZLLTvmX5ZLfOwz0X.eBMFP.MFP.MFP.MFPfoswPaCuRy8uGiRj2wb+DFZnI+27c14cLW3SXOFPfobOFqqq+7ADYp1iwwr+suFVVlvFiy2FKIiHS+dLxpwvv0XY5ZL1suGCCEDYZaL11x4+O+rGkwzz1Xb995N2dFOSaiA7JSWiw4gIKEhHS2OGiy2NCEDY5ZL1ye+CHh8X.AlxFC3cF98X7s6CoF2lWwf+yZJZL1+68oe9Ss+SO+cebUVh3ya3aLJQd6Jwqna+SsgoownW1yRKbLvjbUop4U4xqzOFzXjXigghwgFijZLZkkhQNzXjPigghwiFia1XzKCEVl20nw3FMFFJFW94XjPiwuc+cGYMvk4UbalLE+lHL67Nl6Y2cuJe68aF+c4uzXjzUkJC8zIw8xxH+V1iQBWUpYyS+BH0fFijaLt6v0ucLzJCssR6ZoYOFM1xo1OdaUs7wVlzXbw79lAmY4jo6n0VJq8X7k4ckOeUqq4+ai8V5jtyZgV5yzXz.4c1wShuyqj9oKy7I0RKg8H6wnAx6b1+180U8zmz+IJ0w3cWZl2qTMPdjqLdd2dLZf7JsYZOKY87t8Xz.4Ek8u80OYd8vdVx592dLZf7hx93Wmo6j2SeReMownAx6rrOArjCZiJ6wnAxqz1196uqdu69LNtekL22Rq7b0NMFObdeyIVO4IQmuuuyfQKumEMFMRdW4yihiMY+1GOE+bLFv7pkVZ4hmc2iKWUpALunrO90slrN9d2yMWIa6wX.y6rVbXX24Vm6zBk4iS6wX.yqFZsimrowX.ya1kwRyrGiAKuuY34Iek+r2CTVKMyUkZ.yaYo7W52rFl1ers+QqzJZOFxi.ZLjGArGC48xr9lgmqzDc9XHik3kw.uFC48Q1Og8ce9arsk6aAjLxydLjW0yqjxZ4g94XHupmWIk0RyrGC4U079lgmqdxcFMW1ig7pddKKs+awdMFxqqyqTMPZLjW2m2xR9MPtpTxSdAzXHO4EPig7jW.MFxSdAzXHO4EPig7jW.MFxSdAzXHO4EPig7jW.MFxSdAzXHO4EPig7jW.MFxSdAJViwq9ykm7Z47VVJXiwq9ykm7Z47VVzXHO4ERig7jW.MFxSdAzXHO4EPig7jWTla+2XzUtQvn6me84bkZFXzkxRofQiACHPJ+lH72b0qDv6xZ2cxLZuTYtLxLdLWpiw8bawm+x56uYkUwFLxby7mOY6Nm7kUNQx7wb164KqGqmyHiGyY982rxpXKkJyuwVhKLPIFJZ0KfQKer0pJ5RoJgLWpRVmvThS7xboEkHu8LypEpDGe2QWMXThkCb2LKwPQzxUt68QIWB4ck8RkyX4ccyfQq8MyiN+MhV9XMKs7iwyCYeit3x0VpqtUF1119eer+mcGdWHji6bdSW0XbTVUss3q5k8wXq+XN6iuTtrzas1yRPCnKVJETaFLf.+aXVh5EWabt5.....PRE4DQtJDXBB" ],
									"embed" : 1,
									"id" : "obj-4",
									"maxclass" : "fpic",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 230.0, 22.0, 198.0, 227.0 ]
								}

							}
, 							{
								"box" : 								{
									"data" : [ 1708, "png", "IBkSG0fBZn....PCIgDQRA...bL...v2HX....fP.dLL....DLmPIQEBHf.B7g.YHB..FLVRDEDU3wY6bGsjhhDED.sci4++Wl8gMXBZVSEkpfKTmyKcL1SfoXcMAjYdLMMM8Cv+y+zqM7iGO50lFND+oGaTCFemk62pVg9b1pVt5ol2b73wigZGXqr9CTpzGvrLKUJW8VyGNLX74RMFUXg3yxPEx0QnamyAayyFLpx.x5rUkbcTLbbhd04Xb1KDSY6ry0Q5WCG28WrUxVN46yZg36x1nLfn43D7IWUpidg3Vy1HLfX33f8MWt1iZg3mls69.hgiCzd9dL58BwuMa24AjtMb3R59as3K3qWKD2a1tqCHZNN.s7a9t0KDaU1tiCHFN5rdbKgzpEhsNa2sADCGcTOuWo16Bwdks6z.xik2x5e68E02rS3teNIG0MQ3277bDYqx2DkaUSaNVe6Oj94c2Qtv3S+j5iJa2gFjlMbLMM82lm28y6ry3SL25BwiNaW8AjlMbr0Aiq3Nos5LOTh2sP7rx1Ud.QyQiTgiwNsP7ry1Uc.QyQCb1K9VZ8Bwpjsq3.hlicpJK9V5c2Ismkq1.hliFoBK9VJ8uAiyVkxx6zr+CV3SZNtSCHU9M6plsplq0zb.ANmCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.Bzb.AZNf.MGPflCHPyADn4.BF5liJkEpmgs4vfAuyP1bTogTpqgr4nBYf5aHaNfsXHaNfsPyADn4.Bzb.AZNffgt4vfJuxeZ0F5SZNNyAjs9bavglMbbEFL94GK5Y6bNGE1Y+AIitg9bNd73QoxyRUMWypb9ZU1F1li4rTsA1e9o92XjUa+0RsLaCYyw5EeUIWyp9fQUyWqy1v1bv2oxu+05rMjMGvVLjMGqGRMvxyLrMGyYoJCrTOCYywr4qVE7LCayw5CqxPBqMrMGWgCqpp4ZTLb2aUKUgEesZ+wQ+ZoB66RZU1Ft6J2poxKxp7faKx16x0P2bvqU4A2iHaZNJj48KqeiOs+pxKduCFhliu447nW3s7BC7rKRPUFDRW.izf8QjmkRY6ax0vb0pled25OORU+FgbVJSm0c37xm2zy+dxyv78bT878NyW14yd+26d7JsObueX7P0bT078ryw3YGV0Y7oyo7bEr2LODmywUHeqy4RO6OeTKXq7fwyd+pkYUyQQx2O+TuEhaIOqWfVsy4XOzbTr7UMOa+wqNOiJz91JCy2yQky26FLV+6OpAoO4xhVwA68RyQgx2ZKu7xK+8Uag3xAlp1.9MzbTf7skESUdA25Cq5LddW9Xshlihjuqhzhuy7Bszh+NOiqVUQxG0iug7hjOpmKUywdV3V87Y6UusWWOmiW83GsJmuW8bl9cN7u9qqMGu5wOZWg78I+j9qqmywqd7il7wmx2yg78Vi5g8o4P91rzg1cWOjOMGx2kOe8pYSyg7cKxWOtfFOlV721I6A+moooeeXU2siYD1ilcXUvcigCHnYWspjV9sNu7bhZ88.UKOjxV7ZtWYbd6Vw8es582Vss55vQKOA+0K31yBvVscdlV9Zt0mCXqdstdazhWys782Vss55gU0x2b6wEKnGCFU8hZT4rUUc+vp5gVdXKsZQSOV70xCynGau4sYqZi5Q91iK2vQONzf8tM6wfwyNzk89bzyCmbuZ8gM2hC06RMbTs2PWZ8aFUNqsRkeMtdP6abYtTt85pd0BSS+9+08ler8vcqParm0MWtlikZUsaE+zuVmwp+Zt04qIWx5opsWBJhKygUAGs+Ev4QLjlbDs6h.....PRE4DQtJDXBB" ],
									"embed" : 1,
									"id" : "obj-5",
									"maxclass" : "fpic",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 459.0, 22.0, 199.0, 223.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-6",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 77.0, 251.0, 100.0, 20.0 ],
									"text" : "square",
									"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ]
								}

							}
 ],
						"lines" : [  ]
					}
,
					"patching_rect" : [ 564.0, 151.0, 99.0, 22.0 ],
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"description" : "",
						"digest" : "",
						"fontface" : 0,
						"fontname" : "Arial",
						"fontsize" : 12.0,
						"globalpatchername" : "",
						"tags" : ""
					}
,
					"text" : "p waveforms"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"frgb" : 0.0,
					"id" : "obj-14",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 59.0, 133.0, 219.0, 22.0 ],
					"text" : "Specify amplitudes of partials:",
					"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-15",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 417.0, 415.0, 206.0, 17.0 ],
					"text" : "arguments: waveform, number of sinusoids"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-16",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 71.0, 346.0, 230.0, 17.0 ],
					"text" : "arguments: frequency, amplitude1, amplitude2..."
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-17",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 58.0, 209.0, 31.0, 17.0 ],
					"text" : "mtof"
				}

			}
, 			{
				"box" : 				{
					"hkeycolor" : [ 0.501961, 0.501961, 0.501961, 1.0 ],
					"id" : "obj-18",
					"maxclass" : "kslider",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "int", "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 58.0, 166.0, 196.0, 35.0 ],
					"presentation_rect" : [ 0.0, 0.0, 196.0, 35.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-19",
					"linecount" : 3,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 551.0, 286.0, 100.0, 37.0 ],
					"text" : "Independantly control the amplitude of the first partial"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"frgb" : 0.0,
					"id" : "obj-20",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 453.0, 29.0, 148.0, 22.0 ],
					"text" : "Specify waveshape:",
					"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"hidden" : 1,
					"id" : "obj-21",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 163.0, 402.0, 47.0, 17.0 ],
					"text" : "loadbang"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"hidden" : 1,
					"id" : "obj-22",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 163.0, 421.0, 16.0, 15.0 ],
					"text" : "1"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"id" : "obj-23",
					"items" : [ "Nothing", ",", "partial", "amplitudes", ",", "waveform" ],
					"labelclick" : 1,
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 163.0, 439.0, 187.0, 22.0 ],
					"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-24",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 37.0, 61.0, 192.0, 17.0 ],
					"text" : "Bank of harmonic, sinusoidal oscillators"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 18.0,
					"frgb" : 0.0,
					"id" : "obj-25",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 37.0, 33.0, 185.0, 27.0 ],
					"text" : "harmonics~",
					"textcolor" : [ 0.4, 0.4, 0.8, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.87451, 0.933333, 1.0, 1.0 ],
					"border" : 5,
					"bordercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"id" : "obj-26",
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 9.0, 22.0, 343.0, 68.0 ],
					"rounded" : 70
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-27",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 141.0, 546.0, 27.0, 15.0 ],
					"text" : "stop"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-28",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 72.0, 546.0, 65.0, 15.0 ],
					"text" : "startwindow"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-29",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 91.0, 532.0, 69.0, 17.0 ],
					"text" : "• start audio"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-30",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 199.0, 537.0, 39.0, 17.0 ],
					"text" : "*~ 0.2"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-31",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 199.0, 496.0, 60.0, 17.0 ],
					"text" : "selector~ 2"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-32",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 199.0, 575.0, 41.0, 17.0 ],
					"text" : "dac~"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"border" : 2,
					"bordercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"id" : "obj-33",
					"maxclass" : "panel",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 64.0, 525.0, 123.0, 43.0 ],
					"rounded" : 0
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-34",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 358.0, 306.0, 32.0, 15.0 ],
					"text" : "clear"
				}

			}
, 			{
				"box" : 				{
					"bordercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
					"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
					"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
					"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
					"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
					"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
					"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
					"compatibility" : 1,
					"contdata" : 1,
					"id" : "obj-35",
					"maxclass" : "multislider",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 429.0, 215.0, 12.0, 82.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
					"setminmax" : [ 0.0, 1.0 ],
					"setstyle" : 1,
					"slidercolor" : [ 0.4, 0.4, 0.8, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-36",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 429.0, 306.0, 65.0, 15.0 ],
					"text" : "noisiness $1"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.611765, 0.701961, 1.0, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-37",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 427.0, 124.0, 98.0, 15.0 ],
					"text" : "wave sawtooth 200"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.611765, 0.701961, 1.0, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-38",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 427.0, 179.0, 92.0, 15.0 ],
					"text" : "wave triangle 400"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.611765, 0.701961, 1.0, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-39",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 427.0, 72.0, 105.0, 15.0 ],
					"text" : "wave pulse-train 200"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.611765, 0.701961, 1.0, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-40",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 427.0, 149.0, 81.0, 15.0 ],
					"text" : "wave square 20"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.611765, 0.701961, 1.0, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-41",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 427.0, 99.0, 63.0, 15.0 ],
					"text" : "wave sine 1"
				}

			}
, 			{
				"box" : 				{
					"bordercolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"candicane2" : [ 0.145098, 0.203922, 0.356863, 1.0 ],
					"candicane3" : [ 0.290196, 0.411765, 0.713726, 1.0 ],
					"candicane4" : [ 0.439216, 0.619608, 0.070588, 1.0 ],
					"candicane5" : [ 0.584314, 0.827451, 0.431373, 1.0 ],
					"candicane6" : [ 0.733333, 0.035294, 0.788235, 1.0 ],
					"candicane7" : [ 0.878431, 0.243137, 0.145098, 1.0 ],
					"candicane8" : [ 0.027451, 0.447059, 0.501961, 1.0 ],
					"compatibility" : 1,
					"contdata" : 1,
					"id" : "obj-42",
					"maxclass" : "multislider",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 534.0, 271.0, 12.0, 82.0 ],
					"peakcolor" : [ 0.498039, 0.498039, 0.498039, 1.0 ],
					"setminmax" : [ 0.0, 5.0 ],
					"slidercolor" : [ 0.4, 0.4, 0.8, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.4, 0.4, 0.8, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-43",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 534.0, 358.0, 93.0, 15.0 ],
					"text" : "first-amplitude $1"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"id" : "obj-44",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 58.0, 237.0, 35.0, 17.0 ],
					"triscale" : 0.9
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.529412, 0.529412, 0.529412, 1.0 ],
					"bufsize" : 100,
					"calccount" : 2,
					"fgcolor" : [ 0.419608, 1.0, 0.988235, 1.0 ],
					"id" : "obj-45",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 251.0, 518.0, 225.0, 125.0 ],
					"rounded" : 0,
					"trigger" : 1
				}

			}
, 			{
				"box" : 				{
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-46",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 402.0, 389.0, 125.0, 17.0 ],
					"text" : "harmonics~ sawtooth 300"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-47",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 216.0, 292.0, 78.0, 15.0 ],
					"text" : "1.1 0.2 0.3 0.4"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-48",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 212.0, 275.0, 131.0, 17.0 ],
					"text" : "amplitudes"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-49",
					"linecount" : 3,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 489.0, 564.0, 266.0, 37.0 ],
					"text" : "Wave harmonic images courtesy of Dr. Rod Nave of the Department of Physics and Astronomy, Georgia State University RodNave@gsu.edu"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-50",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 17.0, 300.0, 31.0, 15.0 ],
					"text" : "clear"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-51",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 75.0, 295.0, 102.0, 15.0 ],
					"text" : "420. 0.1 0.2 0.3 0.4"
				}

			}
, 			{
				"box" : 				{
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-52",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 58.0, 321.0, 125.0, 17.0 ],
					"text" : "harmonics~ 340. 0.12 0.5"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-53",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 73.0, 275.0, 131.0, 17.0 ],
					"text" : "frequency, amplitudes"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-54",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 93.0, 238.0, 100.0, 17.0 ],
					"text" : "frequency"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-55",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 531.0, 121.0, 139.0, 27.0 ],
					"text" : "waveshape and maximum number of sinusoids to use."
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-56",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 432.0, 54.0, 94.0, 17.0 ],
					"text" : "change waveforms"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-57",
					"linecount" : 4,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 447.0, 215.0, 100.0, 47.0 ],
					"text" : "between 0. and 1. use bwe sinusoids for per partial control of noisiness"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-44", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-18", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-23", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-22", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 172.5, 478.0, 208.5, 478.0 ],
					"source" : [ "obj-23", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 150.5, 573.0, 208.5, 573.0 ],
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 81.5, 573.0, 208.5, 573.0 ],
					"source" : [ "obj-28", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-30", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-30", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-30", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-45", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 367.5, 333.0, 411.5, 333.0 ],
					"source" : [ "obj-34", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-36", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-35", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 438.5, 342.0, 411.5, 342.0 ],
					"source" : [ "obj-36", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 436.5, 143.0, 411.5, 143.0 ],
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 436.5, 201.0, 411.5, 201.0 ],
					"source" : [ "obj-38", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 436.5, 94.0, 411.5, 94.0 ],
					"source" : [ "obj-39", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"midpoints" : [ 55.5, 645.0, 55.5, 645.0 ],
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 436.5, 172.0, 411.5, 172.0 ],
					"source" : [ "obj-40", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 436.5, 121.0, 411.5, 121.0 ],
					"source" : [ "obj-41", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-42", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 543.5, 379.0, 411.5, 379.0 ],
					"source" : [ "obj-43", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-44", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 2 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 411.5, 471.0, 249.5, 471.0 ],
					"source" : [ "obj-46", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-47", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-50", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-51", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"midpoints" : [ 67.5, 471.0, 229.0, 471.0 ],
					"source" : [ "obj-52", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"disabled" : 0,
					"hidden" : 1,
					"source" : [ "obj-6", 1 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "html_reference.maxpat",
				"bootpath" : "/Users/john/Development/cnmat/trunk/max/patches/MMJ-DEPOT/CNMAT_MMJ-Depot/modules/Depot_Support",
				"patcherrelativepath" : "../../../cnmat/trunk/max/patches/MMJ-DEPOT/CNMAT_MMJ-Depot/modules/Depot_Support",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "harmonics~.mxo",
				"type" : "iLaX"
			}
 ]
	}

}
