Some info about updating externs to work in Max/MSP for OS X.

David Zicarelli, 11/12/2002


Overview

Max/MSP objects use the CFM object code format. Mach-O externs
are not supported yet.

This means you have to use Code Warrior, not Project Builder,
to make externs. We realize people really want to use
Project Builder, and we will attempt to find a way to use
Mach-O externs.

One advantage of using CFM is that many existing externals
can be used unchanged. If the externs don't make any calls
to InterfaceLib (toolbox functions) or MathLib (math functions)
they should work. 

If you try to load an external object and you see 

* error: can't fragload <objectname> missing <<InterfaceLib>>

the object will need to be updated. missing <<MathLib>> 
is the same error. Or it might be some other "lib" that
is missing.

Requirements

As mentioned above, the SDK is based on Code Warrior. All
files included were built for version 8.x of Code Warrior.
We suggest getting the latest release possible.

All of our porting work has been done with OS X 10.2 in
mind. It's possible the stuff will work on earlier versions
but we aren't going to support them.

What's in the SDK?

c74support folder

This folder contains all the headers needed for creating
Max, MSP, and Jitter external objects, plus some 
commonly used files. This folder should be placed in
your Metrowerks Code Warrior folder (also known as the
"compiler" folder) in order to compile the examples provided.

Project Templates

We have provided a series of project templates that might
make updating a bit easier. These are XML files that
work with Code Warrior 8. Refer to the directions below.

Examples

1. maximum

A simple external object that didn't need to be updated.
But you can look at the new project format. Note that it
has logical bugs in the new instance routine!

2. xgui

Provides an example of a carbonized UI external object.
Actual comments too!

3. lores~

Provides an example of a signal object that makes a
call into the math functions in CarbonLib. The classic
target uses MathLib.

Understanding Project Templates

The three examples we've provided were built on the
basis of our new project template files. First we'll
explain how our new .mcp files work to build externs
for both OS X and OS 9. This may be of help to you
in trying to maintain multiple versions of your objects.

If you open the project file hslider.mcp you will see
that it contains four targets (check the menu at the
top of the project list window), debug, release, 
debug-classic and release-classic. For OS X we are
only concerned with debug and release. The classic
targets allow you to make versions of an object that
will work with OS 9 versions Max/MSP.

Make sure the debug target is selected and look at the
list of files. You'll see that a number of these files
are not included in the debug target because they do
not have a dot in the "target" column (the icon is a
red-and-white target with an arrow pointing at it).
These files are CarbonAccessors.o, MathLib, and InterfaceLib.

However, CarbonLib is included. Switching to one of the
classic targets, you see that CarbonLib is not included,
but these other three files are. This is the important
step in building an external object for OS X -- you have
to use CarbonLib.

However, if you open the maximum.mcp file, you'll see
that there is no separate classic-debug or classic-release
target, and none of the "lib" files mentioned above is
included. That's because the maximum object doesn't need
any of these files in order to work. It is therefore
a "universal" object that should work with both OS X and
OS 9 versions.

Any object that makes direct use of the Mac OS will not
be universal, and you'll need to have separate classic
and carbon targets.

The good news is that our project template files can set
up all of this for you.

Selecting and Using Project Templates

1. Create a folder with your source (.c) files and
resource files, which should be called objectname.c
and objectname.rsrc.

1. If you are going to be attempting to build a classic
version of a user interface object with these templates,
you'll probably need to move the file MacHeadersACAF.h
into the c74support folder.

2. This list should provide a good idea of which
template file you'll want to use:

- If you have a Max (not MSP) object that doesn't call any
Mac toolbox or math functions, try max.common.mcp.xml

- If you have a Max (not MSP) object that calls math
functions but not toolbox functions, try max.mathlib.mcp.xml

- If you have a Max user-interface object, try 
max.startup.ACAF.mcp.xml

- If you have a Max object that makes toolbox graphics
calls (but isn't a UI object), try max.interfacelib.ACAF.mcp.xml

- If you have a Max object that makes toolbox calls
but doesn't deal with GrafPorts, Windows, or Dialogs,
try max.interfacelib.mcp.xml

The "ACAF" project templates include CarbonAccessors.o,
plus they use the MacHeadersACAF.h, which defines 
ACCESSOR_CALLS_ARE_FUNCTIONS to allow Carbon Accessor
functions to be used in classic targets. In many cases
this allows you to write Carbonized code that works on
both platforms.

- If you have an MSP object that doesn't call any Mac
toolbox or math functions, try msp.common.mcp.xml

- If you have an MSP object that calls math functions
but not toolbox functions, try msp.mathlib.mcp.xml

- If you have an MSP user-interface object, try 
msp.startup.ACAF.mcp.xml

- If you have an MSP object that makes toolbox graphics
calls (but isn't a UI object), try msp.interfacelib.ACAF.mcp.xml

- If you have an MSP object that makes toolbox calls but
doesn't deal with GrafPorts, Windows, or Dialogs,
try msp.interfacelib.mcp.xml


3. After opening the desired template file, save it in a
folder containing your .c and .rsrc files. Name the
file objectname.xml

4. Choose Find and Replace... from the Search menu.
Enter C74OBJECTNAME as the find string, and the name
of your object as the replace string. Click Replace All.

5. Save your .xml file and close its window.

6. Choose Import Project... from the File menu. Save the
file as objectname.mcp in the same folder as your .c and
.rsrc files.

7. The new project will open. It will include your .c and
.rsrc file. By default, the debug target will be selected.
Time to start working on getting it to compile.

8. Assuming you can get your object to compile, it will be
built (along with a .xSYM file -- see below) in a folder
called build-mac at the same level as your project folder.

Debugging under OS X

The Code Warrior debugger uses GDB, which is installed by
the Mac OS X Developer Tools CD. If you don't have this
CD, you won't be able to debug.

And even if you do, getting the Code Warrior debugger to
land in your code seems currently to be a hit-or-miss
proposition. It works for some people but not others.

There are a couple of general points we can make:

1. Click the "debug" column for your .c file in the
project. This isn't enabled by default.

2. Objects are built in build-mac directory at the same
level as the directory containing your source. (Startup
UI objects are built in build-startup-mac.) Also included
is the .xSYM file for each object. You'll need to copy the
.xSYM file to wherever you copy the object (if you in
fact copy it at all). Double-click on the .xSYM file
to load it into the debugger, and try setting a breakpoint.
As long as Max/MSP loads the copy of the extern in the same
folder as the .xSYM file, it might just work.

You can also try setting the Host Application in the 
Runtime Settings panel of the target settings (it'll say
debug Settings... in the Edit menu for the debug target).
Choose your copy of Max/MSP.

Another useful debugging resource is the Crash Reporter
log. It's in your user home directory's Library folder,
inside the Logs folder, then inside Crash Reporter. Each
application gets its own file, and each time your object
crashes, it's ultimately Max/MSP that crashes so look 
in the Max/MSP file for details of what was on the stack
when your object crashed. It might reveal something. 
However, note that Crash Reporter appends crash information
to any existing file for Max/MSP, so read from the bottom
rather than the top.

You'll get the Crash Reporter log whether or not you are
debugging with the Metrowerks debugger.

Carbonizing Hints

A complete tutorial on carbonizing your object is beyond the
scope of this document. But in general, you'll work by
trying to compile your object and then fixing the compiler
errors. Almost all crashes resulting from incomplete
carbonization are detected by the compiler. For instance,
a GrafPtr is no longer the same as a WindowPtr or DialogPtr.

If you want to call SetPort with a WindowPtr you have to do
it this way:

SetPort(GetWindowPort(grafPtr));

GetWindowPort is defined in CarbonAccessors -- it does
nothing under classic.

You can no longer access the fields of a GrafPort directly.
But there are accessor functions available, such as
GetPortTextFont(grafPtr) that replaces grafPtr->txFont.

Using these functions in conjunction with MacHeadersACAF
and CarbonAccessors.o allows you to "carbonize" your
source while maintaining backward compatibility with
classic Max/MSP.






