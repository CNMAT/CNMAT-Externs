{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 74.0, 96.0, 1113.0, 717.0 ],
		"bglocked" : 0,
		"defrect" : [ 74.0, 96.0, 1113.0, 717.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 4.0, 4.0 ],
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
					"text" : "3 1000. 0.5",
					"linecount" : 2,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1102.0, 886.0, 52.0, 32.0 ],
					"id" : "obj-38",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
<<<<<<< .mine
					"text" : "p someFunctions",
=======
					"text" : "print out1",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 44.0, 620.0, 61.0, 20.0 ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p MakeMidi",
>>>>>>> .r3048
					"fontsize" : 12.0,
<<<<<<< .mine
					"patching_rect" : [ 378.0, 86.0, 102.0, 20.0 ],
					"id" : "obj-37",
=======
					"numinlets" : 1,
					"patching_rect" : [ 224.0, 808.0, 72.0, 20.0 ],
					"id" : "obj-84",
>>>>>>> .r3048
					"fontname" : "Arial",
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 25.0, 69.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 25.0, 69.0, 640.0, 480.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 5.0, 5.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
<<<<<<< .mine
									"maxclass" : "inlet",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 395.0, 45.0, 25.0, 25.0 ],
=======
									"maxclass" : "newobj",
									"text" : "noteout",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 1055.0, 276.0, 92.0, 20.0 ],
									"id" : "obj-17",
									"fontname" : "Arial",
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 96 100",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 1055.0, 245.0, 100.0, 20.0 ],
									"id" : "obj-18",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 930.0, 276.0, 92.0, 20.0 ],
									"id" : "obj-14",
									"fontname" : "Arial",
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 96 100",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 930.0, 245.0, 100.0, 20.0 ],
									"id" : "obj-15",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 800.0, 276.0, 92.0, 20.0 ],
									"id" : "obj-12",
									"fontname" : "Arial",
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 96 100",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 800.0, 245.0, 100.0, 20.0 ],
									"id" : "obj-13",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 680.0, 276.0, 92.0, 20.0 ],
									"id" : "obj-10",
									"fontname" : "Arial",
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 96 100",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 680.0, 245.0, 100.0, 20.0 ],
									"id" : "obj-11",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 545.0, 276.0, 92.0, 20.0 ],
									"id" : "obj-8",
									"fontname" : "Arial",
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 96 100",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 545.0, 245.0, 100.0, 20.0 ],
									"id" : "obj-9",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 430.0, 276.0, 92.0, 20.0 ],
									"id" : "obj-6",
									"fontname" : "Arial",
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 96 100",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 430.0, 245.0, 100.0, 20.0 ],
									"id" : "obj-7",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 300.0, 276.0, 92.0, 20.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 96 100",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 300.0, 245.0, 100.0, 20.0 ],
									"id" : "obj-5",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "noteout",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 175.0, 276.0, 92.0, 20.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 96 100",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 175.0, 245.0, 100.0, 20.0 ],
>>>>>>> .r3048
									"id" : "obj-3",
<<<<<<< .mine
									"numinlets" : 0,
									"comment" : ""
=======
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ]
>>>>>>> .r3048
								}

							}
, 							{
								"box" : 								{
<<<<<<< .mine
									"maxclass" : "message",
									"text" : "0 0 5, 0 125 5, 0 126 5.5, 0 250 5.5, 0 251 6, 0 375 6, 0 376 6.5, 0 625 6.5, 0 626 6., 0 750 6., 0 751 5.5, 0 875 5.5, 0 876 5., 0 1000 5., 1 0 4, 1 125 4, 1 126 3.5, 1 250 3.5, 1 251 3, 1 375 3, 1 376 2.5, 1 625 2.5, 1 626 3., 1 750 3., 1 751 3.5, 1 875 3.5, 1 876 4., 1 1000 4., 2 0 0.5, 2 125 0.5, 2 126 1, 2 250 1, 2 251 1.5, 2 375 1.5, 2 376 2, 2 625 2, 2 626 1.5, 2 750 1.5, 2 751 1, 2 875 1, 2 876 0.5, 3 1000 0.5, 3 0 9.5, 3 125 9.5, 3 126 9, 3 250 9, 3 251 8.5, 3 375 8.5, 3 376 8, 3 625 8, 3 626 8.5, 3 750 8.5, 3 751 9, 3 875 9, 3 876 9.5, 3 1000 9.5,",
									"linecount" : 16,
=======
									"maxclass" : "newobj",
									"text" : "noteout",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 50.0, 275.0, 92.0, 20.0 ],
									"id" : "obj-80",
									"fontname" : "Arial",
									"numoutlets" : 0
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "makenote 96 100",
									"fontsize" : 11.595187,
									"numinlets" : 3,
									"patching_rect" : [ 50.0, 244.0, 100.0, 20.0 ],
									"id" : "obj-79",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "float" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "route 0 1 2 3 4 5 6 7 8",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 134.0, 100.0, 140.5, 20.0 ],
									"id" : "obj-76",
									"fontname" : "Arial",
									"numoutlets" : 10,
									"outlettype" : [ "", "", "", "", "", "", "", "", "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 1054.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-65",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 20 120",
>>>>>>> .r3048
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "" ],
									"patching_rect" : [ 270.0, 185.0, 202.0, 225.0 ],
									"id" : "obj-89",
=======
									"numinlets" : 6,
									"patching_rect" : [ 1054.0, 204.0, 112.0, 20.0 ],
									"id" : "obj-66",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
=======
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 930.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-68",
									"fontname" : "Arial",
>>>>>>> .r3048
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "0 1000. 5, 0 0 5., 0 500. 5, 1 1000 5, 1 0 5, 1 500. 6, 2 1000 5., 2 0 5, 2 500. 7, 3 1000 5, 3 500. 8, 3 0 5, 4 1000 5., 4 0 5, 4 500. 9, 5 1000 5, 5 0 5, 5 500. 4, 6 1000 5., 6 0 5, 6 500. 3, 7 1000 5, 7 0 5, 7 500. 2",
									"linecount" : 10,
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "" ],
									"patching_rect" : [ 495.0, 190.0, 132.0, 142.0 ],
									"id" : "obj-35",
=======
									"numinlets" : 6,
									"patching_rect" : [ 930.0, 204.0, 112.0, 20.0 ],
									"id" : "obj-69",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
=======
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 796.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-71",
									"fontname" : "Arial",
>>>>>>> .r3048
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
<<<<<<< .mine
=======
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 20 120",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 796.0, 204.0, 112.0, 20.0 ],
									"id" : "obj-72",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 678.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-56",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 20 120",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 678.0, 204.0, 112.0, 20.0 ],
									"id" : "obj-57",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 545.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-59",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 20 120",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 545.0, 204.0, 112.0, 20.0 ],
									"id" : "obj-60",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 430.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-62",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 20 120",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 430.0, 204.0, 112.0, 20.0 ],
									"id" : "obj-63",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 298.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-34",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 20 120",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 298.0, 204.0, 112.0, 20.0 ],
									"id" : "obj-54",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 174.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-28",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 20 120",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 174.0, 204.0, 112.0, 20.0 ],
									"id" : "obj-30",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 50.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-32",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 20 120",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 50.0, 204.0, 112.0, 20.0 ],
									"id" : "obj-31",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 134.0, 40.0, 25.0, 25.0 ],
									"id" : "obj-81",
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"comment" : ""
								}

							}
>>>>>>> .r3048
 ],
<<<<<<< .mine
						"lines" : [  ]
=======
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-81", 0 ],
									"destination" : [ "obj-76", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 0 ],
									"destination" : [ "obj-32", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 1 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 2 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 3 ],
									"destination" : [ "obj-62", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 4 ],
									"destination" : [ "obj-59", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 5 ],
									"destination" : [ "obj-56", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 6 ],
									"destination" : [ "obj-71", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 7 ],
									"destination" : [ "obj-68", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 8 ],
									"destination" : [ "obj-65", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-79", 0 ],
									"destination" : [ "obj-80", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-79", 1 ],
									"destination" : [ "obj-80", 1 ],
									"hidden" : 0,
									"midpoints" : [ 140.5, 270.0, 96.0, 270.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-31", 0 ],
									"destination" : [ "obj-79", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-32", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 1 ],
									"destination" : [ "obj-2", 1 ],
									"hidden" : 0,
									"midpoints" : [ 265.5, 271.0, 221.0, 271.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-2", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-4", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 1 ],
									"destination" : [ "obj-4", 1 ],
									"hidden" : 0,
									"midpoints" : [ 390.5, 271.0, 346.0, 271.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 1 ],
									"destination" : [ "obj-6", 1 ],
									"hidden" : 0,
									"midpoints" : [ 520.5, 271.0, 476.0, 271.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-6", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-8", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-9", 1 ],
									"destination" : [ "obj-8", 1 ],
									"hidden" : 0,
									"midpoints" : [ 635.5, 271.0, 591.0, 271.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 1 ],
									"destination" : [ "obj-10", 1 ],
									"hidden" : 0,
									"midpoints" : [ 770.5, 271.0, 726.0, 271.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-12", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-13", 1 ],
									"destination" : [ "obj-12", 1 ],
									"hidden" : 0,
									"midpoints" : [ 890.5, 271.0, 846.0, 271.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 1 ],
									"destination" : [ "obj-14", 1 ],
									"hidden" : 0,
									"midpoints" : [ 1020.5, 271.0, 976.0, 271.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-14", 0 ],
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
									"source" : [ "obj-18", 1 ],
									"destination" : [ "obj-17", 1 ],
									"hidden" : 0,
									"midpoints" : [ 1145.5, 271.0, 1101.0, 271.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-30", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-54", 0 ],
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-54", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-63", 0 ],
									"destination" : [ "obj-7", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-62", 0 ],
									"destination" : [ "obj-63", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-60", 0 ],
									"destination" : [ "obj-9", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-59", 0 ],
									"destination" : [ "obj-60", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-57", 0 ],
									"destination" : [ "obj-11", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-56", 0 ],
									"destination" : [ "obj-57", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-72", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-71", 0 ],
									"destination" : [ "obj-72", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-69", 0 ],
									"destination" : [ "obj-15", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-68", 0 ],
									"destination" : [ "obj-69", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-66", 0 ],
									"destination" : [ "obj-18", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-65", 0 ],
									"destination" : [ "obj-66", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
 ]
>>>>>>> .r3048
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "insert 0 clear",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1158.0, 666.0, 79.0, 18.0 ],
					"id" : "obj-33",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1122.0, 738.0, 35.0, 18.0 ],
					"id" : "obj-32",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setFunction 0, 1000. 9, 0 5., setFunctionName funct1, setFunction 1, 1000 8, 0 5, setFunctionName pitch2, setFunction 2, 1000 7., 0 5, setFunctionName pitch3, setFunction 3, 1000 6, 0 5, setFunctionName pitch4, setFunction 4, 1000 5., 0 5, setFunctionName pitch5, setFunction 5, 1000 4, 0 5, setFunctionName pitch6, setFunction 6, 1000 3., 0 5, setFunctionName pitch7, setFunction 7, 1000 2, 0 5, setFunctionName pitch8,",
					"linecount" : 4,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 474.0, 190.0, 581.0, 60.0 ],
					"id" : "obj-31",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "2 0 0.5, 2 125 0.5, 2 126 1, 2 250 1, 2 251 1.5, 2 375 1.5, 2 376 2, 2 625 2, 2 626 1.5, 2 750 1.5, 2 751 1, 2 875 1, 2 876 0.5, 3 1000 0.5, 3 0 9.5, 3 125 9.5, 3 126 9, 3 250 9, 3 251 8.5, 3 375 8.5, 3 376 8, 3 625 8, 3 626 8.5, 3 750 8.5, 3 751 9, 3 875 9, 3 876 9.5, 3 1000 9.5,",
					"linecount" : 5,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1214.0, 302.0, 332.0, 73.0 ],
					"id" : "obj-20",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0 0 5, 0 125 5, 0 126 5.5, 0 250 5.5, 0 251 6, 0 375 6, 0 376 6.5, 0 625 6.5, 0 626 6., 0 750 6., 0 751 5.5, 0 875 5.5, 0 876 5., 0 1000 5., 1 0 4, 1 125 4, 1 126 3.5, 1 250 3.5, 1 251 3, 1 375 3, 1 376 2.5, 1 625 2.5, 1 626 3., 1 750 3., 1 751 3.5, 1 875 3.5, 1 876 4., 1 1000 4.,",
					"linecount" : 5,
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1214.0, 190.0, 332.0, 73.0 ],
					"id" : "obj-89",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print out1",
					"numoutlets" : 0,
					"fontsize" : 12.0,
					"patching_rect" : [ 122.0, 846.0, 61.0, 20.0 ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p MakePitches",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 182.0, 1178.0, 89.0, 20.0 ],
					"id" : "obj-83",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 93.0, 141.0, 1275.0, 657.0 ],
						"bglocked" : 0,
						"defrect" : [ 93.0, 141.0, 1275.0, 657.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 5.0, 5.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
<<<<<<< .mine
=======
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"visible" : 1,
>>>>>>> .r3048
						"boxes" : [ 							{
								"box" : 								{
<<<<<<< .mine
									"maxclass" : "newobj",
									"text" : "*~ 0.5",
									"numoutlets" : 1,
=======
									"maxclass" : "number~",
>>>>>>> .r3048
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 930.0, 265.0, 42.0, 20.0 ],
									"id" : "obj-13",
=======
									"mode" : 2,
									"sig" : 0.0,
									"numinlets" : 2,
									"patching_rect" : [ 950.0, 325.0, 56.0, 20.0 ],
									"id" : "obj-20",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
									"numinlets" : 2
=======
									"numoutlets" : 2,
									"outlettype" : [ "signal", "float" ]
>>>>>>> .r3048
								}

							}
, 							{
								"box" : 								{
<<<<<<< .mine
									"maxclass" : "newobj",
									"text" : "*~ 0.5",
									"numoutlets" : 1,
=======
									"maxclass" : "number~",
>>>>>>> .r3048
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 795.0, 265.0, 42.0, 20.0 ],
									"id" : "obj-12",
=======
									"mode" : 2,
									"sig" : 0.0,
									"numinlets" : 2,
									"patching_rect" : [ 705.0, 315.0, 56.0, 20.0 ],
									"id" : "obj-19",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
									"numinlets" : 2
=======
									"numoutlets" : 2,
									"outlettype" : [ "signal", "float" ]
>>>>>>> .r3048
								}

							}
, 							{
								"box" : 								{
<<<<<<< .mine
									"maxclass" : "newobj",
									"text" : "*~ 0.5",
									"numoutlets" : 1,
=======
									"maxclass" : "number~",
>>>>>>> .r3048
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 680.0, 265.0, 42.0, 20.0 ],
									"id" : "obj-11",
=======
									"mode" : 2,
									"sig" : 0.0,
									"numinlets" : 2,
									"patching_rect" : [ 450.0, 330.0, 56.0, 20.0 ],
									"id" : "obj-18",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
									"numinlets" : 2
=======
									"numoutlets" : 2,
									"outlettype" : [ "signal", "float" ]
>>>>>>> .r3048
								}

							}
, 							{
								"box" : 								{
<<<<<<< .mine
									"maxclass" : "newobj",
									"text" : "*~ 0.5",
									"numoutlets" : 1,
=======
									"maxclass" : "number~",
>>>>>>> .r3048
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 545.0, 265.0, 42.0, 20.0 ],
									"id" : "obj-10",
=======
									"mode" : 2,
									"sig" : 0.0,
									"numinlets" : 2,
									"patching_rect" : [ 1055.0, 400.0, 56.0, 20.0 ],
									"id" : "obj-17",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
									"numinlets" : 2
=======
									"numoutlets" : 2,
									"outlettype" : [ "signal", "float" ]
>>>>>>> .r3048
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
<<<<<<< .mine
									"text" : "*~ 0.5",
									"numoutlets" : 1,
=======
									"text" : "t f 5",
>>>>>>> .r3048
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 430.0, 265.0, 42.0, 20.0 ],
									"id" : "obj-9",
=======
									"numinlets" : 1,
									"patching_rect" : [ 1055.0, 305.0, 32.5, 20.0 ],
									"id" : "obj-15",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
									"numinlets" : 2
=======
									"numoutlets" : 2,
									"outlettype" : [ "float", "int" ]
>>>>>>> .r3048
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
<<<<<<< .mine
									"text" : "*~ 0.5",
									"numoutlets" : 1,
=======
									"text" : "line~",
>>>>>>> .r3048
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 295.0, 265.0, 42.0, 20.0 ],
									"id" : "obj-8",
=======
									"numinlets" : 2,
									"patching_rect" : [ 1055.0, 340.0, 36.0, 20.0 ],
									"id" : "obj-16",
>>>>>>> .r3048
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "signal", "bang" ]
								}

							}
, 							{
								"box" : 								{
<<<<<<< .mine
									"maxclass" : "newobj",
									"text" : "*~ 0.5",
									"numoutlets" : 1,
=======
									"maxclass" : "number~",
>>>>>>> .r3048
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 175.0, 265.0, 42.0, 20.0 ],
									"id" : "obj-7",
=======
									"mode" : 2,
									"sig" : 0.0,
									"numinlets" : 2,
									"patching_rect" : [ 210.0, 355.0, 56.0, 20.0 ],
									"id" : "obj-14",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
									"numinlets" : 2
=======
									"numoutlets" : 2,
									"outlettype" : [ "signal", "float" ]
>>>>>>> .r3048
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "*~ 0.5",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 50.0, 265.0, 42.0, 20.0 ],
									"id" : "obj-6",
=======
									"numinlets" : 2,
									"patching_rect" : [ 840.0, 330.0, 36.0, 20.0 ],
									"id" : "obj-11",
>>>>>>> .r3048
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
<<<<<<< .mine
									"text" : "cycle~",
									"numoutlets" : 1,
=======
									"text" : "t f 5",
>>>>>>> .r3048
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 930.0, 235.0, 45.0, 20.0 ],
									"id" : "obj-5",
=======
									"numinlets" : 1,
									"patching_rect" : [ 930.0, 235.0, 32.5, 20.0 ],
									"id" : "obj-12",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
=======
									"numoutlets" : 2,
									"outlettype" : [ "float", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "line~",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 930.0, 270.0, 36.0, 20.0 ],
									"id" : "obj-13",
									"fontname" : "Arial",
>>>>>>> .r3048
									"numoutlets" : 2,
									"outlettype" : [ "signal", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 680.0, 235.0, 45.0, 20.0 ],
									"id" : "obj-4",
=======
									"numinlets" : 2,
									"patching_rect" : [ 585.0, 330.0, 36.0, 20.0 ],
									"id" : "obj-8",
>>>>>>> .r3048
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
<<<<<<< .mine
									"text" : "cycle~",
									"numoutlets" : 1,
=======
									"text" : "t f 5",
>>>>>>> .r3048
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 430.0, 235.0, 45.0, 20.0 ],
									"id" : "obj-3",
=======
									"numinlets" : 1,
									"patching_rect" : [ 675.0, 235.0, 32.5, 20.0 ],
									"id" : "obj-9",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
=======
									"numoutlets" : 2,
									"outlettype" : [ "float", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "line~",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 675.0, 270.0, 36.0, 20.0 ],
									"id" : "obj-10",
									"fontname" : "Arial",
>>>>>>> .r3048
									"numoutlets" : 2,
									"outlettype" : [ "signal", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 175.0, 235.0, 45.0, 20.0 ],
									"id" : "obj-2",
=======
									"numinlets" : 2,
									"patching_rect" : [ 340.0, 330.0, 36.0, 20.0 ],
									"id" : "obj-5",
>>>>>>> .r3048
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "number~",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal", "float" ],
									"mode" : 2,
									"sig" : 0.0,
									"patching_rect" : [ 1055.0, 400.0, 56.0, 20.0 ],
									"id" : "obj-17",
=======
									"numinlets" : 1,
									"patching_rect" : [ 430.0, 235.0, 32.5, 20.0 ],
									"id" : "obj-6",
>>>>>>> .r3048
									"fontname" : "Arial",
<<<<<<< .mine
=======
									"numoutlets" : 2,
									"outlettype" : [ "float", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "line~",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 430.0, 270.0, 36.0, 20.0 ],
									"id" : "obj-7",
									"fontname" : "Arial",
>>>>>>> .r3048
									"numoutlets" : 2,
									"outlettype" : [ "signal", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
<<<<<<< .mine
=======
									"text" : "*~ 0.",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 85.0, 330.0, 36.0, 20.0 ],
									"id" : "obj-4",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
>>>>>>> .r3048
									"text" : "t f 5",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 1055.0, 305.0, 32.5, 20.0 ],
									"id" : "obj-15",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "line~",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 1055.0, 340.0, 36.0, 20.0 ],
									"id" : "obj-16",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "signal", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "signal" ],
=======
									"numinlets" : 2,
>>>>>>> .r3048
									"patching_rect" : [ 545.0, 235.0, 45.0, 20.0 ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "route 0 1 2 3 4 5 6 7 8",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 134.0, 100.0, 140.5, 20.0 ],
									"id" : "obj-76",
									"fontname" : "Arial",
									"numoutlets" : 10,
									"outlettype" : [ "", "", "", "", "", "", "", "", "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "flonum",
									"fontsize" : 12.0,
									"maximum" : 1.0,
									"minimum" : 0.0,
									"numinlets" : 1,
									"patching_rect" : [ 238.0, 457.0, 50.0, 20.0 ],
									"id" : "obj-75",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "float", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 1054.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-65",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 0. 1.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 1054.0, 204.0, 99.0, 20.0 ],
									"id" : "obj-66",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 930.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-68",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 50. 1000.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 930.0, 204.0, 125.0, 20.0 ],
									"id" : "obj-69",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 796.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-71",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 50. 1000.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 796.0, 204.0, 125.0, 20.0 ],
									"id" : "obj-72",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 796.0, 232.0, 45.0, 20.0 ],
									"id" : "obj-73",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 678.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-56",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 50. 1000.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 678.0, 204.0, 125.0, 20.0 ],
									"id" : "obj-57",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 545.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-59",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 50. 1000.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 545.0, 204.0, 125.0, 20.0 ],
									"id" : "obj-60",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 430.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-62",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 50. 1000.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 430.0, 204.0, 125.0, 20.0 ],
									"id" : "obj-63",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 298.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-34",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 50. 1000.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 298.0, 204.0, 125.0, 20.0 ],
									"id" : "obj-54",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 298.0, 232.0, 45.0, 20.0 ],
									"id" : "obj-55",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 174.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-28",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 50. 1000.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 174.0, 204.0, 125.0, 20.0 ],
									"id" : "obj-30",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "zl nth 2",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 50.0, 172.0, 49.0, 20.0 ],
									"id" : "obj-32",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "", "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0. 10. 50. 1000.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 50.0, 204.0, 125.0, 20.0 ],
									"id" : "obj-31",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "*~ 0.",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 102.0, 461.0, 103.0, 20.0 ],
									"id" : "obj-29",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "cycle~",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 50.0, 232.0, 45.0, 20.0 ],
									"id" : "obj-26",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "signal" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
<<<<<<< .mine
									"numoutlets" : 1,
									"outlettype" : [ "" ],
=======
									"numinlets" : 0,
>>>>>>> .r3048
									"patching_rect" : [ 134.0, 40.0, 25.0, 25.0 ],
									"id" : "obj-81",
									"numoutlets" : 1,
									"outlettype" : [ "list" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"patching_rect" : [ 102.0, 541.0, 25.0, 25.0 ],
									"id" : "obj-82",
									"numoutlets" : 0,
									"comment" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-82", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-32", 0 ],
									"destination" : [ "obj-31", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-31", 0 ],
									"destination" : [ "obj-26", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-15", 1 ],
									"destination" : [ "obj-16", 1 ],
=======
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-54", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-16", 0 ],
=======
									"source" : [ "obj-54", 0 ],
									"destination" : [ "obj-55", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-17", 0 ],
=======
									"source" : [ "obj-56", 0 ],
									"destination" : [ "obj-57", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-29", 1 ],
=======
									"source" : [ "obj-65", 0 ],
									"destination" : [ "obj-66", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-30", 0 ],
=======
									"source" : [ "obj-68", 0 ],
									"destination" : [ "obj-69", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-54", 0 ],
									"destination" : [ "obj-55", 0 ],
=======
									"source" : [ "obj-28", 0 ],
									"destination" : [ "obj-30", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-54", 0 ],
=======
									"source" : [ "obj-62", 0 ],
									"destination" : [ "obj-63", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-62", 0 ],
									"destination" : [ "obj-63", 0 ],
=======
									"source" : [ "obj-59", 0 ],
									"destination" : [ "obj-60", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-60", 0 ],
									"destination" : [ "obj-1", 0 ],
=======
									"source" : [ "obj-71", 0 ],
									"destination" : [ "obj-72", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-59", 0 ],
									"destination" : [ "obj-60", 0 ],
=======
									"source" : [ "obj-72", 0 ],
									"destination" : [ "obj-73", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-56", 0 ],
									"destination" : [ "obj-57", 0 ],
=======
									"source" : [ "obj-60", 0 ],
									"destination" : [ "obj-1", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-72", 0 ],
									"destination" : [ "obj-73", 0 ],
=======
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-5", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-71", 0 ],
									"destination" : [ "obj-72", 0 ],
=======
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-7", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-68", 0 ],
									"destination" : [ "obj-69", 0 ],
=======
									"source" : [ "obj-6", 1 ],
									"destination" : [ "obj-7", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-2", 0 ],
=======
									"source" : [ "obj-9", 1 ],
									"destination" : [ "obj-10", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-63", 0 ],
									"destination" : [ "obj-3", 0 ],
=======
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-10", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-8", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-57", 0 ],
									"destination" : [ "obj-4", 0 ],
=======
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-11", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-69", 0 ],
									"destination" : [ "obj-5", 0 ],
=======
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-13", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-26", 0 ],
									"destination" : [ "obj-6", 0 ],
=======
									"source" : [ "obj-12", 1 ],
									"destination" : [ "obj-13", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-13", 0 ],
=======
									"source" : [ "obj-63", 0 ],
									"destination" : [ "obj-6", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-73", 0 ],
									"destination" : [ "obj-12", 0 ],
=======
									"source" : [ "obj-57", 0 ],
									"destination" : [ "obj-9", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-11", 0 ],
=======
									"source" : [ "obj-69", 0 ],
									"destination" : [ "obj-12", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-10", 0 ],
=======
									"source" : [ "obj-73", 0 ],
									"destination" : [ "obj-11", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-9", 0 ],
=======
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-8", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-55", 0 ],
<<<<<<< .mine
									"destination" : [ "obj-8", 0 ],
=======
									"destination" : [ "obj-5", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-5", 0 ],
									"destination" : [ "obj-29", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-7", 0 ],
=======
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-29", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-6", 0 ],
									"destination" : [ "obj-29", 0 ],
=======
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-29", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-29", 0 ],
=======
									"source" : [ "obj-15", 1 ],
									"destination" : [ "obj-16", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-8", 0 ],
									"destination" : [ "obj-29", 0 ],
=======
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-16", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-9", 0 ],
									"destination" : [ "obj-29", 0 ],
=======
									"source" : [ "obj-66", 0 ],
									"destination" : [ "obj-15", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-29", 0 ],
=======
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-14", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-11", 0 ],
									"destination" : [ "obj-29", 0 ],
=======
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-29", 0 ],
=======
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-3", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-29", 0 ],
=======
									"source" : [ "obj-3", 1 ],
									"destination" : [ "obj-2", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-76", 8 ],
									"destination" : [ "obj-65", 0 ],
=======
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-2", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-76", 7 ],
									"destination" : [ "obj-68", 0 ],
=======
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-4", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-76", 6 ],
									"destination" : [ "obj-71", 0 ],
=======
									"source" : [ "obj-26", 0 ],
									"destination" : [ "obj-4", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-76", 5 ],
									"destination" : [ "obj-56", 0 ],
=======
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-29", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-76", 4 ],
									"destination" : [ "obj-59", 0 ],
=======
									"source" : [ "obj-7", 0 ],
									"destination" : [ "obj-18", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-76", 3 ],
									"destination" : [ "obj-62", 0 ],
=======
									"source" : [ "obj-10", 0 ],
									"destination" : [ "obj-19", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-76", 2 ],
									"destination" : [ "obj-34", 0 ],
=======
									"source" : [ "obj-13", 0 ],
									"destination" : [ "obj-20", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-76", 1 ],
									"destination" : [ "obj-28", 0 ],
=======
									"source" : [ "obj-16", 0 ],
									"destination" : [ "obj-29", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-76", 0 ],
									"destination" : [ "obj-32", 0 ],
=======
									"source" : [ "obj-81", 0 ],
									"destination" : [ "obj-76", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-81", 0 ],
									"destination" : [ "obj-76", 0 ],
=======
									"source" : [ "obj-76", 0 ],
									"destination" : [ "obj-32", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
=======
									"source" : [ "obj-76", 1 ],
									"destination" : [ "obj-28", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
>>>>>>> .r3048
									"source" : [ "obj-76", 2 ],
									"destination" : [ "obj-34", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-66", 0 ],
									"destination" : [ "obj-15", 0 ],
=======
									"source" : [ "obj-76", 3 ],
									"destination" : [ "obj-62", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
<<<<<<< .mine
=======
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 4 ],
									"destination" : [ "obj-59", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 5 ],
									"destination" : [ "obj-56", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 6 ],
									"destination" : [ "obj-71", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 7 ],
									"destination" : [ "obj-68", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-76", 8 ],
									"destination" : [ "obj-65", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
>>>>>>> .r3048
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"patching_rect" : [ 206.0, 154.0, 20.0, 20.0 ],
					"id" : "obj-79",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "p makeRandom",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 206.0, 186.0, 95.0, 20.0 ],
					"id" : "obj-77",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patcher" : 					{
						"fileversion" : 1,
						"rect" : [ 71.0, 73.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"defrect" : [ 71.0, 73.0, 640.0, 480.0 ],
						"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 5.0, 5.0 ],
						"gridsnaponopen" : 0,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"boxes" : [ 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "bang", "bang" ],
									"patching_rect" : [ 155.0, 115.0, 34.0, 20.0 ],
									"id" : "obj-4",
=======
									"numinlets" : 1,
									"patching_rect" : [ 70.0, 280.0, 50.0, 20.0 ],
									"id" : "obj-94",
>>>>>>> .r3048
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "int", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "clear",
									"numoutlets" : 1,
									"fontsize" : 12.0,
									"outlettype" : [ "" ],
									"patching_rect" : [ 100.0, 475.0, 37.0, 18.0 ],
									"id" : "obj-3",
									"fontname" : "Arial",
									"numinlets" : 2
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b i",
									"numoutlets" : 2,
									"fontsize" : 12.0,
									"outlettype" : [ "bang", "int" ],
									"patching_rect" : [ 155.0, 245.0, 32.5, 20.0 ],
									"id" : "obj-2",
									"fontname" : "Arial",
									"numinlets" : 1
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0 100 0. 10.",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "float" ],
=======
									"numinlets" : 6,
>>>>>>> .r3048
									"patching_rect" : [ 885.0, 390.0, 105.0, 20.0 ],
									"id" : "obj-88",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 100",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "int" ],
=======
									"numinlets" : 2,
>>>>>>> .r3048
									"patching_rect" : [ 885.0, 365.0, 75.0, 20.0 ],
									"id" : "obj-89",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "bang", "bang" ],
=======
									"numinlets" : 1,
>>>>>>> .r3048
									"patching_rect" : [ 885.0, 315.0, 34.0, 20.0 ],
									"id" : "obj-90",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "bang", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 1000",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "int" ],
									"patching_rect" : [ 865.0, 340.0, 81.0, 20.0 ],
=======
									"numinlets" : 2,
									"patching_rect" : [ 865.0, 340.0, 88.0, 20.0 ],
>>>>>>> .r3048
									"id" : "obj-91",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0.",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "" ],
=======
									"numinlets" : 2,
>>>>>>> .r3048
									"patching_rect" : [ 865.0, 415.0, 64.0, 20.0 ],
									"id" : "obj-92",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0 100 0. 10.",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "float" ],
=======
									"numinlets" : 6,
>>>>>>> .r3048
									"patching_rect" : [ 755.0, 390.0, 105.0, 20.0 ],
									"id" : "obj-83",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 100",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "int" ],
=======
									"numinlets" : 2,
>>>>>>> .r3048
									"patching_rect" : [ 755.0, 365.0, 75.0, 20.0 ],
									"id" : "obj-84",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "bang", "bang" ],
=======
									"numinlets" : 1,
>>>>>>> .r3048
									"patching_rect" : [ 755.0, 315.0, 34.0, 20.0 ],
									"id" : "obj-85",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "bang", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 1000",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "int" ],
									"patching_rect" : [ 735.0, 340.0, 81.0, 20.0 ],
=======
									"numinlets" : 2,
									"patching_rect" : [ 735.0, 340.0, 88.0, 20.0 ],
>>>>>>> .r3048
									"id" : "obj-86",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0.",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "" ],
=======
									"numinlets" : 2,
>>>>>>> .r3048
									"patching_rect" : [ 735.0, 415.0, 64.0, 20.0 ],
									"id" : "obj-87",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0 100 0. 10.",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "float" ],
=======
									"numinlets" : 6,
>>>>>>> .r3048
									"patching_rect" : [ 630.0, 390.0, 105.0, 20.0 ],
									"id" : "obj-78",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 100",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "int" ],
=======
									"numinlets" : 2,
>>>>>>> .r3048
									"patching_rect" : [ 630.0, 365.0, 75.0, 20.0 ],
									"id" : "obj-79",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "bang", "bang" ],
=======
									"numinlets" : 1,
>>>>>>> .r3048
									"patching_rect" : [ 630.0, 315.0, 34.0, 20.0 ],
									"id" : "obj-80",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "bang", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 1000",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "int" ],
									"patching_rect" : [ 610.0, 340.0, 81.0, 20.0 ],
=======
									"numinlets" : 2,
									"patching_rect" : [ 610.0, 340.0, 88.0, 20.0 ],
>>>>>>> .r3048
									"id" : "obj-81",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0.",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "" ],
=======
									"numinlets" : 2,
>>>>>>> .r3048
									"patching_rect" : [ 610.0, 415.0, 64.0, 20.0 ],
									"id" : "obj-82",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0 100 0. 10.",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "float" ],
=======
									"numinlets" : 6,
>>>>>>> .r3048
									"patching_rect" : [ 500.0, 385.0, 105.0, 20.0 ],
									"id" : "obj-76",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 100",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "int" ],
=======
									"numinlets" : 2,
>>>>>>> .r3048
									"patching_rect" : [ 500.0, 360.0, 75.0, 20.0 ],
									"id" : "obj-77",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0 100 0. 10.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 370.0, 340.0, 105.0, 20.0 ],
									"id" : "obj-75",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "scale 0 100 0. 10.",
									"fontsize" : 12.0,
									"numinlets" : 6,
									"patching_rect" : [ 245.0, 350.0, 105.0, 20.0 ],
									"id" : "obj-74",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b b b b b",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 245.0, 275.0, 657.5, 20.0 ],
									"id" : "obj-65",
									"fontname" : "Arial",
									"numoutlets" : 6,
									"outlettype" : [ "bang", "bang", "bang", "bang", "bang", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "t b b",
									"fontsize" : 12.0,
									"numinlets" : 1,
									"patching_rect" : [ 500.0, 310.0, 34.0, 20.0 ],
									"id" : "obj-52",
									"fontname" : "Arial",
									"numoutlets" : 2,
									"outlettype" : [ "bang", "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 1000",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 480.0, 335.0, 81.0, 20.0 ],
									"id" : "obj-51",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "pack 0. 0.",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 480.0, 410.0, 64.0, 20.0 ],
									"id" : "obj-50",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 100",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 245.0, 325.0, 75.0, 20.0 ],
									"id" : "obj-37",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "random 100",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 370.0, 315.0, 75.0, 20.0 ],
									"id" : "obj-35",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "int" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "10000 $1",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "" ],
=======
									"numinlets" : 2,
>>>>>>> .r3048
									"patching_rect" : [ 370.0, 375.0, 61.0, 18.0 ],
									"id" : "obj-34",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "0 $1",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 245.0, 375.0, 34.0, 18.0 ],
									"id" : "obj-33",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "0",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "" ],
=======
									"numinlets" : 2,
>>>>>>> .r3048
									"patching_rect" : [ 195.0, 150.0, 32.5, 18.0 ],
									"id" : "obj-30",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "metro 100",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 155.0, 185.0, 65.0, 20.0 ],
									"id" : "obj-29",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "1",
									"fontsize" : 12.0,
<<<<<<< .mine
									"outlettype" : [ "" ],
									"patching_rect" : [ 155.0, 150.0, 32.5, 18.0 ],
=======
									"numinlets" : 2,
									"patching_rect" : [ 155.0, 115.0, 32.5, 18.0 ],
>>>>>>> .r3048
									"id" : "obj-15",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "inlet",
									"numinlets" : 0,
									"patching_rect" : [ 155.0, 75.0, 25.0, 25.0 ],
									"id" : "obj-14",
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "outlet",
									"numinlets" : 1,
									"patching_rect" : [ 615.0, 535.0, 25.0, 25.0 ],
									"id" : "obj-13",
									"numoutlets" : 0,
									"comment" : ""
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "message",
									"text" : "setFunction $1",
									"fontsize" : 12.0,
									"numinlets" : 2,
									"patching_rect" : [ 155.0, 410.0, 89.0, 18.0 ],
									"id" : "obj-12",
									"fontname" : "Arial",
									"numoutlets" : 1,
									"outlettype" : [ "" ]
								}

							}
, 							{
								"box" : 								{
									"maxclass" : "newobj",
									"text" : "counter 0 7",
									"fontsize" : 12.0,
									"numinlets" : 5,
									"patching_rect" : [ 155.0, 215.0, 73.0, 20.0 ],
									"id" : "obj-1",
									"fontname" : "Arial",
									"numoutlets" : 4,
									"outlettype" : [ "int", "", "", "int" ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"source" : [ "obj-12", 0 ],
									"destination" : [ "obj-13", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-33", 0 ],
									"destination" : [ "obj-13", 0 ],
=======
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-15", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-13", 0 ],
=======
									"source" : [ "obj-33", 0 ],
									"destination" : [ "obj-13", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-50", 0 ],
									"destination" : [ "obj-13", 0 ],
=======
									"source" : [ "obj-34", 0 ],
									"destination" : [ "obj-13", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-29", 0 ],
=======
									"source" : [ "obj-51", 0 ],
									"destination" : [ "obj-50", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-29", 0 ],
=======
									"source" : [ "obj-52", 0 ],
									"destination" : [ "obj-51", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-37", 0 ],
									"destination" : [ "obj-74", 0 ],
=======
									"source" : [ "obj-50", 0 ],
									"destination" : [ "obj-13", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-74", 0 ],
									"destination" : [ "obj-33", 0 ],
=======
									"source" : [ "obj-15", 0 ],
									"destination" : [ "obj-29", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-75", 0 ],
									"destination" : [ "obj-34", 0 ],
=======
									"source" : [ "obj-30", 0 ],
									"destination" : [ "obj-29", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-65", 1 ],
									"destination" : [ "obj-35", 0 ],
=======
									"source" : [ "obj-37", 0 ],
									"destination" : [ "obj-74", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-65", 0 ],
									"destination" : [ "obj-37", 0 ],
=======
									"source" : [ "obj-74", 0 ],
									"destination" : [ "obj-33", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-65", 2 ],
									"destination" : [ "obj-52", 0 ],
=======
									"source" : [ "obj-75", 0 ],
									"destination" : [ "obj-34", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-35", 0 ],
									"destination" : [ "obj-75", 0 ],
=======
									"source" : [ "obj-65", 1 ],
									"destination" : [ "obj-35", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-77", 0 ],
									"destination" : [ "obj-76", 0 ],
=======
									"source" : [ "obj-65", 0 ],
									"destination" : [ "obj-37", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-76", 0 ],
									"destination" : [ "obj-50", 1 ],
=======
									"source" : [ "obj-65", 2 ],
									"destination" : [ "obj-52", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-52", 1 ],
									"destination" : [ "obj-77", 0 ],
=======
									"source" : [ "obj-35", 0 ],
									"destination" : [ "obj-75", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-80", 1 ],
									"destination" : [ "obj-79", 0 ],
=======
									"source" : [ "obj-77", 0 ],
									"destination" : [ "obj-76", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-78", 0 ],
									"destination" : [ "obj-82", 1 ],
=======
									"source" : [ "obj-76", 0 ],
									"destination" : [ "obj-50", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-79", 0 ],
									"destination" : [ "obj-78", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-84", 0 ],
									"destination" : [ "obj-83", 0 ],
=======
									"source" : [ "obj-52", 1 ],
									"destination" : [ "obj-77", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-83", 0 ],
									"destination" : [ "obj-87", 1 ],
=======
									"source" : [ "obj-80", 1 ],
									"destination" : [ "obj-79", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-85", 1 ],
									"destination" : [ "obj-84", 0 ],
=======
									"source" : [ "obj-78", 0 ],
									"destination" : [ "obj-82", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-90", 1 ],
									"destination" : [ "obj-89", 0 ],
=======
									"source" : [ "obj-79", 0 ],
									"destination" : [ "obj-78", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-88", 0 ],
									"destination" : [ "obj-92", 1 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-89", 0 ],
									"destination" : [ "obj-88", 0 ],
=======
									"source" : [ "obj-81", 0 ],
									"destination" : [ "obj-82", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-65", 5 ],
									"destination" : [ "obj-90", 0 ],
=======
									"source" : [ "obj-86", 0 ],
									"destination" : [ "obj-87", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-65", 4 ],
									"destination" : [ "obj-85", 0 ],
=======
									"source" : [ "obj-85", 0 ],
									"destination" : [ "obj-86", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-65", 3 ],
									"destination" : [ "obj-80", 0 ],
=======
									"source" : [ "obj-84", 0 ],
									"destination" : [ "obj-83", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-82", 0 ],
									"destination" : [ "obj-13", 0 ],
=======
									"source" : [ "obj-83", 0 ],
									"destination" : [ "obj-87", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-87", 0 ],
									"destination" : [ "obj-13", 0 ],
=======
									"source" : [ "obj-85", 1 ],
									"destination" : [ "obj-84", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-92", 0 ],
									"destination" : [ "obj-13", 0 ],
=======
									"source" : [ "obj-90", 1 ],
									"destination" : [ "obj-89", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-1", 0 ],
=======
									"source" : [ "obj-88", 0 ],
									"destination" : [ "obj-92", 1 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-1", 3 ],
									"destination" : [ "obj-30", 0 ],
=======
									"source" : [ "obj-89", 0 ],
									"destination" : [ "obj-88", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [ 230.0, 256.0, 278.0, 123.0, 204.5, 135.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-52", 0 ],
									"destination" : [ "obj-51", 0 ],
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-51", 0 ],
									"destination" : [ "obj-50", 0 ],
=======
									"source" : [ "obj-90", 0 ],
									"destination" : [ "obj-91", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-81", 0 ],
									"destination" : [ "obj-82", 0 ],
=======
									"source" : [ "obj-91", 0 ],
									"destination" : [ "obj-92", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-80", 0 ],
									"destination" : [ "obj-81", 0 ],
=======
									"source" : [ "obj-65", 5 ],
									"destination" : [ "obj-90", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-85", 0 ],
									"destination" : [ "obj-86", 0 ],
=======
									"source" : [ "obj-65", 4 ],
									"destination" : [ "obj-85", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-86", 0 ],
									"destination" : [ "obj-87", 0 ],
=======
									"source" : [ "obj-65", 3 ],
									"destination" : [ "obj-80", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-91", 0 ],
									"destination" : [ "obj-92", 0 ],
=======
									"source" : [ "obj-82", 0 ],
									"destination" : [ "obj-13", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-90", 0 ],
									"destination" : [ "obj-91", 0 ],
=======
									"source" : [ "obj-87", 0 ],
									"destination" : [ "obj-13", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-2", 0 ],
=======
									"source" : [ "obj-92", 0 ],
									"destination" : [ "obj-13", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-2", 1 ],
									"destination" : [ "obj-12", 0 ],
=======
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-94", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-2", 0 ],
									"destination" : [ "obj-65", 0 ],
=======
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-65", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-3", 0 ],
									"destination" : [ "obj-13", 0 ],
=======
									"source" : [ "obj-29", 0 ],
									"destination" : [ "obj-1", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-14", 0 ],
									"destination" : [ "obj-4", 0 ],
=======
									"source" : [ "obj-1", 0 ],
									"destination" : [ "obj-12", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [  ]
								}

							}
, 							{
								"patchline" : 								{
<<<<<<< .mine
									"source" : [ "obj-4", 0 ],
									"destination" : [ "obj-15", 0 ],
=======
									"source" : [ "obj-1", 3 ],
									"destination" : [ "obj-30", 0 ],
>>>>>>> .r3048
									"hidden" : 0,
									"midpoints" : [ 230.0, 256.0, 278.0, 123.0, 204.5, 135.0 ]
								}

							}
, 							{
								"patchline" : 								{
									"source" : [ "obj-4", 1 ],
									"destination" : [ "obj-3", 0 ],
									"hidden" : 0,
									"midpoints" : [ 55.0, 192.0 ]
								}

							}
 ]
					}
,
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"fontface" : 0,
						"fontsize" : 12.0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"fontname" : "Arial",
						"globalpatchername" : ""
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setFunction 0, 1000. 5, 0 5., 500. 5, setFunctionName funct1, setFunction 1, 1000 5, 0 5, 500. 6, setFunctionName pitch2, setFunction 2, 1000 5., 0 5, 500. 7, setFunctionName pitch3, setFunction 3, 1000 5, 500. 8, 0 5, setFunctionName pitch4, setFunction 4, 1000 5., 0 5, 500. 9, setFunctionName pitch5, setFunction 5, 1000 5, 0 5, 500. 4, setFunctionName pitch6, setFunction 6, 1000 5., 0 5, 500. 3, setFunctionName pitch7, setFunction 7, 1000 5, 0 5, 500. 2, setFunctionName pitch8,",
					"linecount" : 5,
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 478.0, 78.0, 600.0, 73.0 ],
					"id" : "obj-35",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
<<<<<<< .mine
=======
					"text" : "255. 5., 300 5",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 412.0, 96.0, 84.0, 18.0 ],
					"id" : "obj-27",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
>>>>>>> .r3048
					"text" : "setFunction 8",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 234.0, 238.0, 83.0, 18.0 ],
					"id" : "obj-40",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
<<<<<<< .mine
=======
					"maxclass" : "newobj",
					"text" : "line 1.",
					"fontsize" : 12.0,
					"numinlets" : 3,
					"patching_rect" : [ 116.0, 76.0, 46.0, 20.0 ],
					"id" : "obj-39",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
>>>>>>> .r3048
					"maxclass" : "message",
<<<<<<< .mine
=======
					"text" : "10000., 0. 10000",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 108.0, 48.0, 101.0, 18.0 ],
					"id" : "obj-38",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0., 10000. 10000",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 36.0, 16.0, 101.0, 18.0 ],
					"id" : "obj-37",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "line 1.",
					"fontsize" : 12.0,
					"numinlets" : 3,
					"patching_rect" : [ 44.0, 76.0, 46.0, 20.0 ],
					"id" : "obj-36",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
>>>>>>> .r3048
					"text" : "stop",
					"fontsize" : 9.0,
					"numinlets" : 2,
					"patching_rect" : [ 331.0, 1292.0, 27.0, 15.0 ],
					"id" : "obj-42",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "startwindow",
					"fontsize" : 9.0,
					"numinlets" : 2,
					"patching_rect" : [ 262.0, 1292.0, 65.0, 15.0 ],
					"id" : "obj-43",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• start audio",
					"fontsize" : 9.0,
<<<<<<< .mine
					"patching_rect" : [ 286.0, 1274.0, 69.0, 17.0 ],
=======
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 196.0, 1084.0, 69.0, 17.0 ],
>>>>>>> .r3048
					"id" : "obj-44",
					"fontname" : "Arial",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"border" : 2,
<<<<<<< .mine
					"patching_rect" : [ 254.0, 1271.0, 123.0, 43.0 ],
=======
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 164.0, 1081.0, 123.0, 43.0 ],
>>>>>>> .r3048
					"id" : "obj-45",
					"numoutlets" : 0,
					"rounded" : 0,
					"bordercolor" : [ 0.4, 0.4, 0.8, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "dac~",
					"fontsize" : 9.0,
<<<<<<< .mine
					"patching_rect" : [ 178.0, 1342.0, 36.0, 17.0 ],
=======
					"numinlets" : 2,
					"patching_rect" : [ 88.0, 1152.0, 36.0, 17.0 ],
>>>>>>> .r3048
					"id" : "obj-47",
					"fontname" : "Arial",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "addFunction",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 258.0, 310.0, 77.0, 18.0 ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "lineColor 0. 1. 0. 1.",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 954.0, 318.0, 111.0, 18.0 ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print out2",
					"fontsize" : 12.0,
<<<<<<< .mine
					"patching_rect" : [ 502.0, 846.0, 61.0, 20.0 ],
=======
					"numinlets" : 1,
					"patching_rect" : [ 412.0, 656.0, 61.0, 20.0 ],
>>>>>>> .r3048
					"id" : "obj-19",
					"fontname" : "Arial",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print out4",
					"fontsize" : 12.0,
<<<<<<< .mine
					"patching_rect" : [ 990.0, 842.0, 61.0, 20.0 ],
=======
					"numinlets" : 1,
					"patching_rect" : [ 900.0, 652.0, 61.0, 20.0 ],
>>>>>>> .r3048
					"id" : "obj-5",
					"fontname" : "Arial",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"minimum" : 0.0,
					"fontsize" : 12.0,
					"numinlets" : 1,
<<<<<<< .mine
					"patching_rect" : [ 98.0, 298.0, 65.0, 20.0 ],
=======
					"patching_rect" : [ 220.0, 756.0, 50.0, 20.0 ],
					"id" : "obj-20",
					"fontname" : "Arial",
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "flonum",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 40.0, 160.0, 65.0, 20.0 ],
>>>>>>> .r3048
					"id" : "obj-11",
					"fontname" : "Arial",
<<<<<<< .mine
					"numinlets" : 1,
					"maximum" : 1000.0
=======
					"numoutlets" : 2,
					"outlettype" : [ "float", "bang" ]
>>>>>>> .r3048
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 1194.0, 758.0, 41.0, 18.0 ],
					"id" : "obj-16",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setFunctionName envelope",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 826.0, 290.0, 157.0, 18.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "xmin 0, xmax 1000, ymin 0, ymax 10",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 602.0, 318.0, 205.0, 18.0 ],
					"id" : "obj-18",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "bpf",
<<<<<<< .mine
					"ymax" : 10.0,
					"numoutlets" : 4,
=======
					"lineColor" : [ 0.0, 1.0, 0.0, 1.0 ],
>>>>>>> .r3048
					"xmax" : 1000.0,
					"fontsize" : 12.0,
					"numinlets" : 1,
<<<<<<< .mine
					"patching_rect" : [ 206.0, 354.0, 900.0, 455.0 ],
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
=======
					"ymax" : 10.0,
					"patching_rect" : [ 116.0, 168.0, 900.0, 455.0 ],
>>>>>>> .r3048
					"id" : "obj-17",
					"fontname" : "Arial",
<<<<<<< .mine
					"lineColor" : [ 0.0, 1.0, 0.0, 1.0 ],
					"numinlets" : 1
=======
					"numoutlets" : 4,
					"color" : [ 1.0, 1.0, 1.0, 1.0 ],
					"textcolor" : [  ],
					"xmax" : 10000.0,
					"outlettype" : [ "list", "list", "bang", "list" ]
>>>>>>> .r3048
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "write",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 1150.0, 758.0, 36.0, 18.0 ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "coll",
					"fontsize" : 12.0,
					"numinlets" : 1,
					"patching_rect" : [ 1090.0, 826.0, 59.5, 20.0 ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"saved_object_attributes" : 					{
						"embed" : 0
					}

				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print out3",
					"fontsize" : 12.0,
<<<<<<< .mine
					"patching_rect" : [ 818.0, 830.0, 61.0, 20.0 ],
=======
					"numinlets" : 1,
					"patching_rect" : [ 728.0, 640.0, 61.0, 20.0 ],
>>>>>>> .r3048
					"id" : "obj-8",
					"fontname" : "Arial",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dump",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 382.0, 238.0, 41.0, 18.0 ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
<<<<<<< .mine
=======
					"text" : "251. 5.",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 378.0, 128.0, 47.0, 18.0 ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
>>>>>>> .r3048
					"text" : "clear",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 386.0, 210.0, 37.0, 18.0 ],
					"id" : "obj-9",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "multislider",
<<<<<<< .mine
					"slidercolor" : [ 0.984314, 0.007843, 0.007843, 1.0 ],
					"setminmax" : [ 0.0, 1000.0 ],
					"patching_rect" : [ 206.0, 850.0, 900.0, 15.0 ],
=======
					"setminmax" : [ 0.0, 10000.0 ],
					"numinlets" : 1,
					"patching_rect" : [ 116.0, 648.0, 900.0, 15.0 ],
>>>>>>> .r3048
					"id" : "obj-7",
					"numoutlets" : 2,
					"orientation" : 0,
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "setFunction 0",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 254.0, 286.0, 83.0, 18.0 ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "addFunction pitch",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 222.0, 214.0, 105.0, 18.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "version",
					"fontsize" : 12.0,
					"numinlets" : 2,
					"patching_rect" : [ 246.0, 262.0, 50.0, 18.0 ],
					"id" : "obj-23",
					"fontname" : "Arial",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-39", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-36", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-11", 0 ],
=======
					"source" : [ "obj-9", 0 ],
>>>>>>> .r3048
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-7", 0 ],
=======
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-17", 1 ],
					"destination" : [ "obj-19", 0 ],
=======
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-17", 2 ],
					"destination" : [ "obj-8", 0 ],
=======
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-17", 3 ],
					"destination" : [ "obj-5", 0 ],
=======
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-7", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-17", 1 ],
					"destination" : [ "obj-19", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-17", 2 ],
					"destination" : [ "obj-8", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-47", 0 ],
=======
					"source" : [ "obj-17", 3 ],
					"destination" : [ "obj-5", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [ 271.5, 1320.0, 187.5, 1320.0 ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-47", 0 ],
=======
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [ 340.5, 1320.0, 187.5, 1320.0 ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-77", 0 ],
=======
					"source" : [ "obj-25", 0 ],
>>>>>>> .r3048
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-79", 0 ],
					"destination" : [ "obj-77", 0 ],
=======
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-47", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [ 181.5, 1130.0, 97.5, 1130.0 ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-83", 0 ],
					"destination" : [ "obj-47", 0 ],
=======
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [ 250.5, 1130.0, 97.5, 1130.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-17", 3 ],
					"destination" : [ "obj-13", 0 ],
=======
					"source" : [ "obj-77", 0 ],
					"destination" : [ "obj-17", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-13", 0 ],
=======
					"source" : [ "obj-79", 0 ],
					"destination" : [ "obj-77", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-13", 0 ],
=======
					"source" : [ "obj-83", 0 ],
					"destination" : [ "obj-47", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-83", 0 ],
					"destination" : [ "obj-47", 1 ],
=======
					"source" : [ "obj-17", 3 ],
					"destination" : [ "obj-13", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-89", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-13", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-17", 0 ],
=======
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-13", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-13", 0 ],
=======
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-83", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-13", 0 ],
=======
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-11", 0 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
<<<<<<< .mine
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-38", 1 ],
=======
					"source" : [ "obj-83", 0 ],
					"destination" : [ "obj-47", 1 ],
>>>>>>> .r3048
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
