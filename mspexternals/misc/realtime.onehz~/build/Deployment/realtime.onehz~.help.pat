{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 1615.0, -369.0, 928.0, 652.0 ],
		"bglocked" : 0,
		"defrect" : [ 1615.0, -369.0, 928.0, 652.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "realtime.onehz~",
					"id" : "obj-27",
					"fontname" : "Arial",
					"patching_rect" : [ 40.0, 20.0, 150.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "realtime.onehz~ written by Andy Schmeder <andy@cnmat.berkeley.edu>\n\nthe implementation of the realtime clock is based on on the paper \"Using a DLL to filter time\" by Fons Adriaensen.",
					"linecount" : 3,
					"id" : "obj-26",
					"fontname" : "Arial",
					"patching_rect" : [ 50.0, 560.0, 687.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"presentation_rect" : [ 483.0, 546.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "why bother?\n\nphasor~ assumes that the nominal sampling frequency is the actual sampling frequency, which isn't actually true.\n\nphasor~ has no \"ground truth\" for phase. there is no way to determine when the waveform should start.",
					"linecount" : 11,
					"id" : "obj-25",
					"fontname" : "Arial",
					"patching_rect" : [ 660.0, 60.0, 178.0, 158.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "falling edge at the start of each realtime second ->",
					"linecount" : 3,
					"id" : "obj-24",
					"fontname" : "Arial",
					"patching_rect" : [ 40.0, 140.0, 125.0, 48.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"presentation_rect" : [ 40.0, 173.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "is not the same as ->",
					"id" : "obj-23",
					"fontname" : "Arial",
					"patching_rect" : [ 290.0, 60.0, 125.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "realtime clock notes:\n\nthe behavior of realtime.onehz~ is less accurate for the first few seconds after DSP is started or restarted.\n\nit is also less accurate every time the DSP has an under-flow (signal deadline miss), e.g. as caused by excessive CPU usage or certain hardware system events that lock interrupts for too long.\n\nit is also temporarily less accurate if the system clock is changed suddenly by more than a few milliseconds.\n\nit does not work properly when io-vector-size is not equal to signal-vector-size (see MaxMSP DSP settings).\n\nin general it works fine while NTP is running, and has some ability to recover quickly when a large timing error occurs.",
					"linecount" : 17,
					"id" : "obj-22",
					"fontname" : "Arial",
					"patching_rect" : [ 480.0, 300.0, 412.0, 241.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0,
					"presentation_rect" : [ 417.0, 319.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "realtime.onehz~\n\na signal phasor that operates at exactly one hertz, regardless of sampling rate error, and is always in-phase with the realtime clock.\n\nthis signal has a falling edge on the start of each actual second.  if your system is synchronized to universal time, this signal should be approximately the same everywhere.\n\nthe output of onehz~ is the fractional part of the seconds of the clock; it is strictly less than one, but does not necessarily reach zero after the falling edge.",
					"linecount" : 14,
					"id" : "obj-21",
					"fontname" : "Arial",
					"patching_rect" : [ 170.0, 300.0, 293.0, 200.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"id" : "obj-8",
					"patching_rect" : [ 400.0, 130.0, 198.0, 115.0 ],
					"calccount" : 64,
					"numinlets" : 2,
					"bufsize" : 16,
					"numoutlets" : 0,
					"presentation_rect" : [ 358.0, 148.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "-~",
					"id" : "obj-7",
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 400.0, 100.0, 32.5, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "phasor~ 1.",
					"id" : "obj-6",
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 440.0, 60.0, 68.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 2,
					"numoutlets" : 1,
					"presentation_rect" : [ 395.0, 81.0, 0.0, 0.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ezdac~",
					"id" : "obj-3",
					"patching_rect" : [ 90.0, 260.0, 45.0, 45.0 ],
					"numinlets" : 2,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "scope~",
					"id" : "obj-2",
					"patching_rect" : [ 170.0, 130.0, 198.0, 115.0 ],
					"calccount" : 64,
					"numinlets" : 2,
					"bufsize" : 16,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "realtime.onehz~",
					"id" : "obj-1",
					"fontname" : "Arial",
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 170.0, 60.0, 96.0, 20.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
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
