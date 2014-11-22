CNMAT-Externs
=============

The CNMAT externals xcode project has various dependencies that need to be configured and built on your own platform for you to compile current/new externals on your own.

## Dependencies

### 1. FFTW

Since we are still supporting i386 processors, we need to configure `fftw` to build a universal version. To configure for i386 and x86_64: 

1. `./configure CC="gcc -arch i386 -arch x86_64" CXX="g++ -arch i386 -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E"`

2. Then `sudo make` to compile the `libfftw3.a` in the `../fftw/.libs` directory.  Note that we don't invoke `make install` as we need the xcode project to refer to this specific location.	
This is the (default) double precision version. However, there are some files that refer to the floating point version. To add the floating point version you need to repeat the above steps, but with the single precision flag `--enable-single`:

3. `./configure CC="gcc -arch i386 -arch x86_64" CXX="g++ -arch i386 -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E" --enable-single`

4. Then `sudo make` to compile the `libfftw3f.a` in the `../fftw/.libs` directory.

### 2. GSL

Same situation with GSL as fftw:  configure for both i386 and x86_64:

`./configure CC="gcc -arch i386 -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E"`

`sudo make`

### 3. libo / libomax

The instructions for configuring and making these libraries can be found [here](https://github.com/CNMAT/libo/blob/master/doc/libo-documentation/odot-build-setup.txt).

### 4. Max6-SDK

Cycling's SDK can be found [here](https://github.com/Cycling74/max6-sdk)

`git clone git@github.com:Cycling74/max6-sdk.git`
...into the local CNMAT building directory, one level up from the xcode project.

### 2. OSC-kit

I don't see this in the repository, and the Makefile expects it in the `../OSC-kit` directory for building legacy OSC objects.  Does anyone have a copy they could post in the root CNMAT repository folder?

### 3. SDIF





<<<<<<< Updated upstream
*-- Rama Gottfried / Jeff Lubow*, 11/19/14
=======
*-- Rama Gottfried + Jeff Lubow*, 10/10/14
>>>>>>> Stashed changes
