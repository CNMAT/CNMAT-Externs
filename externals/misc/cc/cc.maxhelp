{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 233.0, 44.0, 1024.0, 629.0 ],
		"bglocked" : 0,
		"defrect" : [ 233.0, 44.0, 1024.0, 629.0 ],
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
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "build_path /Users/johnmac/foo/",
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"presentation_rect" : [ 163.0, 54.0, 0.0, 0.0 ],
					"outlettype" : [ "" ],
					"patching_rect" : [ 164.0, 56.0, 176.0, 18.0 ],
					"id" : "obj-2",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Arguments:\no: A pointer to the cc struct.  This is necessary in case you need to pass it to an API function that needs it.\nx: A byte array (65535 bytes) that can be used to store any instance data.\ninlet: The inlet number that the message was passed to.  The leftmost inlet is 0.\nargc/argv: An atom array containing any arguments.\nnumoutlets: The number of outlets your object has.\noutlets: An array of pointers to your object's outlets.",
					"linecount" : 8,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 16.0, 308.0, 510.0, 117.0 ],
					"id" : "obj-8",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The function can have any name you like--once you have compiled and loaded your code, the cc object will respond to a message that is the same as your function name.  In the above case the cc object will respond to the message \"foo\".",
					"linecount" : 3,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 16.0, 252.0, 510.0, 48.0 ],
					"id" : "obj-7",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Defining functions:\nWith the exception of two functions (see the section on the new() and free() functions below) all functions you define must have the following signature:\nint foo(t_object *o, char *x, int inlet, int argc, t_atom *argv, int numoutlets, void **outlets)",
					"linecount" : 4,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 16.0, 180.0, 570.0, 62.0 ],
					"id" : "obj-6",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Attributes:\n@inlets: The number of inlets your object will have.\n@outlets: The number of outlets your object will have.\n@cfile: The path to your cfile.  If not specified, a temp file will be created in /private/var/tmp\n@build_path: The folder where the build will take place.  Default: /private/var/tmp\n@maxsdk: The path to the Max SDK (the folder that contains the c74support folder)",
					"linecount" : 9,
					"fontsize" : 12.0,
					"numoutlets" : 0,
					"patching_rect" : [ 600.0, 24.0, 402.0, 131.0 ],
					"id" : "obj-5",
					"fontname" : "Arial",
					"frgb" : [ 0.0, 0.0, 0.0, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "compile_and_load",
					"fontsize" : 12.0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 44.0, 56.0, 108.0, 18.0 ],
					"id" : "obj-3",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "cc @inlets 2 @outlets 2 @maxsdk /Users/johnmac/Development/cnmat/trunk/max/SDK/MaxSDK-5",
					"fontsize" : 12.0,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 44.0, 100.0, 536.0, 20.0 ],
					"id" : "obj-1",
					"fontname" : "Arial",
					"numinlets" : 2
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
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
 ]
	}

}
