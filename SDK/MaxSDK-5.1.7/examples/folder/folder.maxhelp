{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 21.0, 59.0, 664.0, 463.0 ],
		"bglocked" : 0,
		"defrect" : [ 21.0, 59.0, 664.0, 463.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 11.595187,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
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
					"text" : "(click on \"types iLaX\" to list external objects)",
					"linecount" : 2,
					"presentation_rect" : [ 528.0, 246.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 526.0, 241.0, 127.0, 33.0 ],
					"id" : "obj-32",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "movies",
					"presentation_rect" : [ 586.0, 319.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 556.0, 319.0, 47.0, 20.0 ],
					"id" : "obj-28",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "types MooV",
					"presentation_rect" : [ 573.0, 337.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 544.0, 337.0, 72.0, 18.0 ],
					"id" : "obj-31",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "drop a folder here!",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 118.0, 188.0, 150.0, 20.0 ],
					"id" : "obj-24",
					"fontname" : "Arial",
					"textcolor" : [ 0.541176, 0.541176, 0.541176, 1.0 ],
					"frgb" : [ 0.541176, 0.541176, 0.541176, 1.0 ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "dropfile",
					"numinlets" : 1,
					"types" : [  ],
					"numoutlets" : 2,
					"patching_rect" : [ 108.0, 181.0, 118.0, 33.0 ],
					"id" : "obj-15",
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sound files",
					"presentation_rect" : [ 463.0, 252.0, 0.0, 0.0 ],
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 444.0, 319.0, 66.0, 20.0 ],
					"id" : "obj-14",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "types AIFF WAVE",
					"presentation_rect" : [ 457.0, 270.0, 0.0, 0.0 ],
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 431.0, 337.0, 104.0, 18.0 ],
					"id" : "obj-13",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "autohelp",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 511.0, 402.0, 55.0, 20.0 ],
					"id" : "obj-7",
					"fontname" : "Arial",
					"hidden" : 1,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<-- Macintosh",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 445.0, 258.0, 80.0, 20.0 ],
					"id" : "obj-9",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"items" : [ "abs.maxhelp", ",", "absolutepath.maxhelp", ",", "accum.maxhelp", ",", "acos.maxhelp", ",", "acosh.maxhelp", ",", "active.maxhelp", ",", "anal.maxhelp", ",", "append.maxhelp", ",", "asin.maxhelp", ",", "asinh.maxhelp", ",", "atan.maxhelp", ",", "atan2.maxhelp", ",", "atanh.maxhelp", ",", "atodb.maxhelp", ",", "atoi.maxhelp", ",", "autopattr.maxhelp", ",", "bag.maxhelp", ",", "bangbang.maxhelp", ",", "bendin.maxhelp", ",", "bendout.maxhelp", ",", "bgcolor.maxhelp", ",", "bitand.maxhelp", ",", "bitor.maxhelp", ",", "bline.maxhelp", ",", "bogus.maxhelp", ",", "bondo.maxhelp", ",", "borax.maxhelp", ",", "bpatcher.maxhelp", ",", "BPflip.maxpat", ",", "BPswitch.maxpat", ",", "bucket.maxhelp", ",", "buddy.maxhelp", ",", "button.maxhelp", ",", "capture.maxhelp", ",", "cartopol.maxhelp", ",", "change.maxhelp", ",", "clip.maxhelp", ",", "clocker.maxhelp", ",", "closebang.maxhelp", ",", "coll.maxhelp", ",", "colorpicker.maxhelp", ",", "combine.maxhelp", ",", "comment.maxhelp", ",", "conformpath.maxhelp", ",", "cos.maxhelp", ",", "cosh.maxhelp", ",", "counter.maxhelp", ",", "cpuclock.maxhelp", ",", "ctlin.maxhelp", ",", "ctlout.maxhelp", ",", "cycle.maxhelp", ",", "date.maxhelp", ",", "dbtoa.maxhelp", ",", "decide.maxhelp", ",", "decode.maxhelp", ",", "defer.maxhelp", ",", "deferlow.maxhelp", ",", "delay.maxhelp", ",", "detonate.maxhelp", ",", "dial.maxhelp", ",", "dialog.maxhelp", ",", "div.maxhelp", ",", "dropfile.maxhelp", ",", "drunk.maxhelp", ",", "equals.maxhelp", ",", "error.maxhelp", ",", "example.maxpresets", ",", "expr.maxhelp", ",", "filedate.maxhelp", ",", "filein.maxhelp", ",", "filepath.maxhelp", ",", "filewatch.maxhelp", ",", "float.maxhelp", ",", "flonum.maxhelp", ",", "flush.maxhelp", ",", "folder.maxhelp", ",", "follow.maxhelp", ",", "fontlist.maxhelp", ",", "forward.maxhelp", ",", "fpic.maxhelp", ",", "freebang.maxhelp", ",", "fromsymbol.maxhelp", ",", "fswap.maxhelp", ",", "ftom.maxhelp", ",", "funbuff.maxhelp", ",", "function.maxhelp", ",", "funnel.maxhelp", ",", "gate.maxhelp", ",", "gestalt.maxhelp", ",", "grab.maxhelp", ",", "greaterthan.maxhelp", ",", "greaterthaneq.maxhelp", ",", "gswitch.maxhelp", ",", "gswitch2.maxhelp", ",", "hi.maxhelp", ",", "hint.maxhelp", ",", "histo.maxhelp", ",", "hover-insider.maxpat", ",", "hover.maxhelp", ",", "if.maxhelp", ",", "imovie.maxhelp", ",", "in.maxhelp", ",", "incdec.maxhelp", ",", "inlet.maxhelp", ",", "inport.maxhelp", ",", "int.maxhelp", ",", "itable.maxhelp", ",", "iter.maxhelp", ",", "itoa.maxhelp", ",", "jit.cellblock.maxhelp", ",", "js.maxhelp", ",", "jstrigger.maxhelp", ",", "jsui.maxhelp", ",", "jweb.maxhelp", ",", "key.maxhelp", ",", "keyup.maxhelp", ",", "kslider.maxhelp", ",", "lcd.maxhelp", ",", "led.maxhelp", ",", "lessthan.maxhelp", ",", "lessthaneq.maxhelp", ",", "line.maxhelp", ",", "linedrive.maxhelp", ",", "listfunnel.maxhelp", ",", "loadbang.maxhelp", ",", "loadmess.maxhelp", ",", "logand.maxhelp", ",", "logor.maxhelp", ",", "makenote.maxhelp", ",", "match.maxhelp", ",", "matrixctrl.maxhelp", ",", "maximum.maxhelp", ",", "mean.maxhelp", ",", "menubar.maxhelp", ",", "message.maxhelp", ",", "metro.maxhelp", ",", "midiflush.maxhelp", ",", "midiformat.maxhelp", ",", "midiin.maxhelp", ",", "midiinfo.maxhelp", ",", "midiout.maxhelp", ",", "midiparse.maxhelp", ",", "minimum.maxhelp", ",", "minus.maxhelp", ",", "modifiers.maxhelp", ",", "modulo.maxhelp", ",", "mousefilter.maxhelp", ",", "mousestate.maxhelp", ",", "movie.maxhelp", ",", "mtof.maxhelp", ",", "mtr.maxhelp", ",", "multislider.maxhelp", ",", "next.maxhelp", ",", "notein.maxhelp", ",", "noteout.maxhelp", ",", "notequals.maxhelp", ",", "nslider.maxhelp", ",", "number.maxhelp", ",", "numkey.maxhelp", ",", "offer.maxhelp", ",", "onebang.maxhelp", ",", "onecopy.maxhelp", ",", "opendialog.maxhelp", ",", "out.maxhelp", ",", "outlet.maxhelp", ",", "outport.maxhelp", ",", "pack.maxhelp", ",", "pak.maxhelp", ",", "panel.maxhelp", ",", "past.maxhelp", ",", "patcher.maxhelp", ",", "patcherargs.maxhelp", ",", "pattr.maxhelp", ",", "pattrforward.maxhelp", ",", "pattrhub.maxhelp", ",", "pattrmarker.maxhelp", ",", "pattrmarker2.maxpat", ",", "pattrstorage.maxhelp", ",", "pcontrol.maxhelp", ",", "pcontrol_ExamplePatch.maxpat", ",", "peak.maxhelp", ",", "pgmin.maxhelp", ",", "pgmout.maxhelp", ",", "pictctrl.maxhelp", ",", "pictslider.maxhelp", ",", "pipe.maxhelp", ",", "playbar.maxhelp", ",", "plugconfig.maxhelp", ",", "plugmidiin.maxhelp", ",", "plugmidiout.maxhelp", ",", "plugmod.maxhelp", ",", "plugmorph.maxhelp", ",", "plugmultiparam.maxhelp", ",", "plugstore.maxhelp", ",", "plus.maxhelp", ",", "poltocar.maxhelp", ",", "poly.maxhelp", ",", "polyin.maxhelp", ",", "polyout.maxhelp", ",", "pow.maxhelp", ",", "pp.maxhelp", ",", "pptempo.maxhelp", ",", "pptime.maxhelp", ",", "prepend.maxhelp", ",", "preset.maxhelp", ",", "print.maxhelp", ",", "prob.maxhelp", ",", "pv.maxhelp", ",", "pvar.maxhelp", ",", "qlim.maxhelp", ",", "qlist.maxhelp", ",", "qmetro.maxhelp", ",", "quickthresh.maxhelp", ",", "radiogroup.maxhelp", ",", "random.maxhelp", ",", "rdiv.maxhelp", ",", "receive.maxhelp", ",", "regexp.maxhelp", ",", "relativepath.maxhelp", ",", "rminus.maxhelp", ",", "round.maxhelp", ",", "route.maxhelp", ",", "router.maxhelp", ",", "rslider.maxhelp", ",", "rtin.maxhelp", ",", "savedialog.maxhelp", ",", "scale.maxhelp", ",", "screensize.maxhelp", ",", "scrollbar.maxhelp", ",", "select.maxhelp", ",", "send.maxhelp", ",", "seq.maxhelp", ",", "serial.maxhelp", ",", "setclock.maxhelp", ",", "shiftleft.maxhelp", ",", "shiftright.maxhelp", ",", "sin.maxhelp", ",", "sinh.maxhelp", ",", "slide.maxhelp", ",", "slider.maxhelp", ",", "speedlim.maxhelp", ",", "spell.maxhelp", ",", "split.maxhelp", ",", "spray.maxhelp", ",", "sprintf.maxhelp", ",", "sqrt.maxhelp", ",", "standalone.maxhelp", ",", "stripnote.maxhelp", ",", "strippath.maxhelp", ",", "substitute.maxhelp", ",", "suckah.maxhelp", ",", "suspend.maxhelp", ",", "sustain.maxhelp", ",", "swap.maxhelp", ",", "swatch.maxhelp", ",", "switch.maxhelp", ",", "sxformat.maxhelp", ",", "sysexin.maxhelp", ",", "tab.maxhelp", ",", "table.maxhelp", ",", "tan.maxhelp", ",", "tanh.maxhelp", ",", "tempo.maxhelp", ",", "text.maxhelp", ",", "textbutton.maxhelp", ",", "textedit.maxhelp", ",", "thispatcher.maxhelp", ",", "thresh.maxhelp", ",", "timepoint.maxhelp", ",", "timer.maxhelp", ",", "times.maxhelp", ",", "togedge.maxhelp", ",", "toggle.maxhelp", ",", "tosymbol.maxhelp", ",", "touchin.maxhelp", ",", "touchout.maxhelp", ",", "translate.maxhelp", ",", "transport.maxhelp", ",", "trigger.maxhelp", ",", "trough.maxhelp", ",", "ubutton.maxhelp", ",", "udpreceive.maxhelp", ",", "udpsend.maxhelp", ",", "umenu.maxhelp", ",", "universal.maxhelp", ",", "unpack.maxhelp", ",", "urn.maxhelp", ",", "uzi.maxhelp", ",", "value.maxhelp", ",", "vdp.maxhelp", ",", "vexpr.maxhelp", ",", "when.maxhelp", ",", "xbendin.maxhelp", ",", "xbendout.maxhelp", ",", "xnotein.maxhelp", ",", "xnoteout.maxhelp", ",", "zl.maxhelp", ",", "zmap.maxhelp" ],
					"numinlets" : 1,
					"types" : [  ],
					"numoutlets" : 3,
					"patching_rect" : [ 90.0, 404.0, 110.0, 20.0 ],
					"id" : "obj-10",
					"fontname" : "Arial",
					"outlettype" : [ "int", "", "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "\"/Program Files/Common Files/Cycling '74/max-externals\"",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 135.0, 236.0, 308.0, 18.0 ],
					"id" : "obj-11",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"patching_rect" : [ 90.0, 295.0, 20.0, 20.0 ],
					"id" : "obj-17",
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "folder \"./Cycling '74/max-help\"",
					"numinlets" : 1,
					"numoutlets" : 2,
					"patching_rect" : [ 90.0, 371.0, 163.0, 20.0 ],
					"id" : "obj-18",
					"fontname" : "Arial",
					"outlettype" : [ "", "int" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "types iLaX",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 343.0, 337.0, 63.0, 18.0 ],
					"id" : "obj-19",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "types JSON",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 252.0, 337.0, 70.0, 18.0 ],
					"id" : "obj-20",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "types TEXT",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 173.0, 337.0, 69.0, 18.0 ],
					"id" : "obj-21",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "text files",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 182.0, 319.0, 51.0, 20.0 ],
					"id" : "obj-22",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Max patches",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 252.0, 319.0, 76.0, 20.0 ],
					"id" : "obj-23",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "external objects",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 337.0, 319.0, 92.0, 20.0 ],
					"id" : "obj-25",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "\"/Applications/Max5/Cycling '74/max-externals\"",
					"numinlets" : 2,
					"numoutlets" : 1,
					"patching_rect" : [ 155.0, 258.0, 252.0, 18.0 ],
					"id" : "obj-26",
					"fontname" : "Arial",
					"outlettype" : [ "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The types message allows you to list only one type of file. Files have a four-character file type (case sensitive).",
					"linecount" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 202.0, 289.0, 307.0, 33.0 ],
					"id" : "obj-27",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "bang makes the list",
					"linecount" : 2,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 32.0, 278.0, 73.0, 33.0 ],
					"id" : "obj-29",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "<-- Windows",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 445.0, 236.0, 80.0, 20.0 ],
					"id" : "obj-30",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "folder",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 8.0, 485.0, 30.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontface" : 3,
					"fontsize" : 20.871338
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "List the files in a specific folder",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 36.0, 485.0, 21.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"textcolor" : [ 0.93, 0.93, 0.97, 1.0 ],
					"frgb" : [ 0.93, 0.93, 0.97, 1.0 ],
					"fontsize" : 12.754705
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "textbutton",
					"varname" : "autohelp_top_ref",
					"spacing_x" : 0.0,
					"text" : "open folder reference",
					"bgoncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"presentation_rect" : [ 0.0, 0.0, 138.418106, 14.666666 ],
					"numinlets" : 1,
					"bordercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"spacing_y" : 0.0,
					"textoncolor" : [ 0.27, 0.35, 0.47, 1.0 ],
					"numoutlets" : 3,
					"bgcolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"patching_rect" : [ 361.581909, 22.0, 138.418106, 14.666666 ],
					"fontlink" : 1,
					"textovercolor" : [ 0.4, 0.5, 0.65, 1.0 ],
					"id" : "obj-4",
					"fontname" : "Arial",
					"underline" : 1,
					"textcolor" : [ 0.166, 0.23, 0.326, 1.0 ],
					"outlettype" : [ "", "", "int" ],
					"bgovercolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"bgoveroncolor" : [ 0.0, 0.0, 0.0, 0.0 ],
					"fontface" : 3,
					"fontsize" : 12.754705
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_see_title",
					"text" : "See Also:",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 516.0, 406.0, 100.0, 20.0 ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"fontface" : 1,
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "umenu",
					"varname" : "autohelp_see_menu",
					"items" : [ "(Objects:)", ",", "conformpath", ",", "filein", ",", "filepath", ",", "opendialog", ",", "pcontrol", ",", "<separator>" ],
					"numinlets" : 1,
					"types" : [  ],
					"numoutlets" : 3,
					"patching_rect" : [ 516.0, 426.0, 130.0, 20.0 ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"outlettype" : [ "int", "", "" ],
					"fontsize" : 11.595187
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel",
					"numinlets" : 1,
					"background" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 5.0, 5.0, 495.0, 52.0 ],
					"id" : "obj-5",
					"grad1" : [ 0.27, 0.35, 0.47, 1.0 ],
					"mode" : 1,
					"grad2" : [ 0.85, 0.85, 0.85, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_see_panel",
					"numinlets" : 1,
					"bordercolor" : [ 0.5, 0.5, 0.5, 0.75 ],
					"background" : 1,
					"numoutlets" : 0,
					"bgcolor" : [ 0.85, 0.85, 0.85, 0.75 ],
					"patching_rect" : [ 511.0, 402.0, 140.0, 50.0 ],
					"id" : "obj-12",
					"border" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_description",
					"text" : "The folder object can fill a menu object with the names of files in a folder. You can specify the folder as an argument or by sending a symbol with the name in it. The forward-slash character ( / ) is used as a path delineator. An initial slash at the beginning of a path indicates the boot volume. The Max application folder is referred to by using period-slash (e.g.: \"./Cycling '74/max-help\"), and the Cycling '74 folder may be refered to by using C74. A path on Macintosh systems is preceded by the name of the machine and a colon:\n\"Macintosh HD:/Users/Documents/max stuff/Desktop/phase_vocoder.pat\"\nOn Windows systems, a path is preceded by a drive letter \n\"C:/Documents/max stuff/Desktop/phase_vocoder.pat\"",
					"linecount" : 8,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.0, 57.0, 605.0, 113.0 ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"fontsize" : 11.595187
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 553.5, 363.0, 99.5, 363.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 117.5, 345.0, 99.5, 345.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 440.5, 363.0, 99.5, 363.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 144.5, 345.5, 99.5, 345.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 352.5, 363.0, 99.5, 363.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 164.5, 345.0, 99.5, 345.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 261.5, 363.0, 99.5, 363.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 182.5, 363.0, 99.5, 363.0 ]
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
 ]
	}

}
