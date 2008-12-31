INSTALLATION INSTRUCTIONS:

To build the library, you will need a copy of cmake (www.cmake.org).  Once
you have installed cmake, you can make the Makefiles with

	cmake .

or

	cmake -DCMAKE_INSTALL_PREFIX=/install/prefix .

to change the location where the library and .h files will be installed. 
The defaults are /usr/local/lib and /usr/local/include.

Then build the project using

	make sudo make install


OUT OF SOURCE BUILDS:

Out of source builds can be made by creating a build directory and starting
the cmake process from there.  For example

	mkdir build cd build cmake .. make make install


XCODE:

To have cmake generate an xcode project, use the following command

	cmake -G Xcode .


DOCUMENTATION:

To build the documentation, you will need a copy of doxygen
(www.doxygen.org).  Then the documentation can be built by typing

	make doc

or

	doxygen Doxyfile

PDF documentation can be built with

	cd doc/latex make pdf

Platform specific instructions:

Windows:

Nothing has been tested on Windows yet, but I'd be surprised if anything
worked...
