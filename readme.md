CNMAT-Externs
=============

The CNMAT externals xcode project has various dependencies that need to be configured and built on your own platform for you to compile current/new externals on your own.

We have removed the built externals release as we have learned that GNU licensing for FFTW and GSL is incompatible with UCB licensing

# MAC

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

The Makefile expects it in the `../OSC-kit` directory for building legacy OSC objects.
The OSC kit is in a different CNMAT repo: https://github.com/CNMAT/CNMAT-OSC

### 3. SDIF


# WINDOWS

For building on Windows, we are working on Windows 10, using the MinGW-w64 complier on Cygwin64, with separate tool chains for 32 and 64 bit externals.

### 1. Cygwin64
Down and install Cygwin64, available [here](http://www.cygwin.com/install.html).

The Cygwin installation/setup program allows you to select install packages.

Once you arrive at the `Select Packages` page, set `View` to `Full`, and then search for the following packages. You will need to scroll down to find them sometimes.

select and install the following:
* `make` : The GNU Version of the make utility.
* `gcc` : GNU complier collection
* `mingw64-x86_64-gcc-core` : GGC for Win64 toolchain
* `mingw64-i686-gcc-core` : GGC for Win32 toolchain
* `git` : Distributed version control system
* `wget` : Utility to retrieve files from the WWW via HTTP and FTP
* `flex` : A fast lexical analyzer generator
* `bison` : GNU yacc-compatible parser generator

(to select for installation, click the `Skip` icon which should then change to the version number)

On the following page, make sure `select required packages` is enabled.

**Note:**  although very tempting, ***do not install the GSL of FFTW packages through Cygwin***, we need to build these manually to work correctly (details below).


### 2. GSL
Download the GSL library from the GNU site: [link](ftp://ftp.gnu.org/gnu/gsl/) (currently using v2.4).

In Cygwin, we can do:
```
wget ftp://ftp.gnu.org/gnu/gsl/gsl-2.4.tar.gz
tar xf gsl-2.4.tar.gz
cd gsl-2.4
```

We need to build and install GSL as a static library, as separate versions for win32 and 64.

For 64bit:
```
./configure CC=x86_64-w64-mingw32-gcc --prefix=/usr/x86_64-w64-mingw32/sys-root/mingw --enable-static --disable-shared
make
make install
```
and then again for 32 bit:
```
make clean
./configure CC=i686-w64-mingw32-gcc --prefix=/usr/i686-w64-mingw32/sys-root/mingw --enable-static --disable-shared
make
make install
```

### 3. FFTW

FFTW installation is similar to GSL, except that we have to do it an additional two times, for single precision mode.

In the home directory:
```
wget ftp://ftp.fftw.org/pub/fftw/fftw-3.3.6-pl2.tar.gz
tar xf fftw-3.3.6-pl2.tar.gz
cd fftw-3.3.6-pl2
```

n.b. FFTW can take a while to build, so if your computer has multiple cores you might want to use `make -j4` instead of the usual `make` below.

For 64bit :
```
./configure CC=x86_64-w64-mingw32-gcc --prefix=/usr/x86_64-w64-mingw32/sys-root/mingw --with-our-malloc16 --with-windows-f77-mangling --enable-static --disable-shared --enable-threads --with-combined-threads --enable-portable-binary --enable-sse2 --with-incoming-stack-boundary=2
make
make install
```

and then again for 64 bit single precision:
```
make clean
./configure CC=x86_64-w64-mingw32-gcc --prefix=/usr/x86_64-w64-mingw32/sys-root/mingw --with-our-malloc16 --with-windows-f77-mangling --enable-static --disable-shared --enable-threads --with-combined-threads --enable-portable-binary --enable-sse2 --with-incoming-stack-boundary=2 --enable-single
make
make install
```

For 32 bit :
```
make clean
./configure CC=i686-w64-mingw32-gcc --prefix=/usr/i686-w64-mingw32/sys-root/mingw --with-our-malloc16 --with-windows-f77-mangling --enable-static --disable-shared --enable-threads --with-combined-threads --enable-portable-binary --enable-sse2 --with-incoming-stack-boundary=2
make
make install
```

and then again for 64 bit single precision:
```
make clean
./configure CC=i686-w64-mingw32-gcc --prefix=/usr/i686-w64-mingw32/sys-root/mingw --with-our-malloc16 --with-windows-f77-mangling --enable-static --disable-shared --enable-threads --with-combined-threads --enable-portable-binary --enable-sse2 --with-incoming-stack-boundary=2 --enable-single
make
make install
```

then we can clean up a bit and go back to our main folder:
```
rm *.gz
cd ..
```

### 4. GitHub repositories

```
git clone https://github.com/CNMAT/CNMAT-Externs
git clone https://github.com/CNMAT/libo
git clone https://github.com/CNMAT/libomax
git clone https://github.com/CNMAT/CNMAT-SDIF (private)
git clone https://github.com/CNMAT/CNMAT-OSC (private)
git clone https://github.com/Cycling74/max-sdk
```

### 5. libo and libomax

build libo and libomax first, then finally:

### 6. build CNMAT-Externs



*-- Rama Gottfried / Jeff Lubow*, 11/19/14, updated Sept 2017
