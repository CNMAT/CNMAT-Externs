CNMAT-Externs
=============

Details on building the CNMAT-Externs repository.

## Folder structure

In general, the build settings expect all the repositories to be in the same root folder.

# MAC

### 1. GitHub repositories

Clone the following repositories:
```
git clone https://github.com/CNMAT/CNMAT-Externs
git clone https://github.com/CNMAT/libo
git clone https://github.com/CNMAT/libomax
git clone https://github.com/CNMAT/CNMAT-SDIF
git clone https://github.com/CNMAT/CNMAT-OSC
git clone https://github.com/Cycling74/max-sdk
```

### 2. FFTW

Since we are still supporting i386 processors, we need to configure `fftw` to build a universal version. To configure for i386 and x86_64:

1. `./configure CC="gcc -arch i386 -arch x86_64" CXX="g++ -arch i386 -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E"`

2. Then `sudo make` to compile the `libfftw3.a` in the `../fftw/.libs` directory.  Note that we don't invoke `make install` as we need the xcode project to refer to this specific location.
This is the (default) double precision version. However, there are some files that refer to the floating point version. To add the floating point version you need to repeat the above steps, but with the single precision flag `--enable-single`:

3. `./configure CC="gcc -arch i386 -arch x86_64" CXX="g++ -arch i386 -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E" --enable-single`

4. Then `sudo make` to compile the `libfftw3f.a` in the `../fftw/.libs` directory.

### 3. GSL

Same situation with GSL as fftw:  configure for both i386 and x86_64:

`./configure CC="gcc -arch i386 -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E"`

`sudo make`

### 4. libo / libomax

libo requires GNU [flex](https://github.com/westes/flex) and [bison](https://www.gnu.org/software/bison/) to be installed and findable by your bash shell.

Build `libo` first, and then `libomax`, using the `make`.

More details can be found [here](https://github.com/CNMAT/libo/blob/master/doc/libo-documentation/odot-build-setup.txt).

### 5. build CNMAT-Externs

Finally we are ready to build the CNMAT-Externals!

To build for Mac, enter the `CNMAT-Externs` directory and do:
```
make
```

The built objects will be in the `/build/Release` folder.

To create the CNMAT-Externals release package, run the release script:
`./make-release-mac.sh`.

### 6. footnote: Java Dev Kit

If you get an error message about Java not being installed, you may need to install the Java Dev Kit. You should be able to type `which javac` and get a report about its location. This is the application we use to compile .java file into a .class file that can be used by Max.


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
* `zip` : Info-ZIP compression utility (for making the CNMAT-Externals release)

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
git clone https://github.com/CNMAT/CNMAT-SDIF
git clone https://github.com/CNMAT/CNMAT-OSC
git clone https://github.com/Cycling74/max-sdk
```

### 5. libo and libomax

#### libo
To build for Windows 64bit, enter the `libo` directory and do:
```
make win64
```
This will compile the library with the win64 tool chain, and create a `libo.a` file in the `/libo/libs/x86_64` folder.

Then to build the Windows 32bit version of the library do:
```
make clean
make win
```
This will  compile the library with the win64 tool chain, and create a `libo.a` file in the `/libo/libs/i686` folder.

Note we need to `make clean` before building the new version, otherwise the library will be created with the already complied `.o` files.

#### libomax

Then do the same for the `libomax` library: enter the `libomax` directory and do:
```
make win64
```
This will  compile the library with the win64 tool chain, and create a `libomax.a` file in the `/libomax/libs/x86_64` folder.

Then to build the Windows 32bit version of the library do:
```
make clean
make win
```
This will  compile the library with the win64 tool chain, and create a `libomax.a` file in the `/libomax/libs/i686` folder.

### 7. Install the Java Dev Kit

Install the suggested version of the JDK for your version of Windows, at this time of writing the Orcale website told me the suggested version was 8 update 151.

Once this is installed, we need to tell Cygwin where to find it. To do this we need to add a folder to our PATH variable in the file `.bashrc` in your Cygwin user home folder. For example, for me this is at: `/cygwin64/home/rama`.

Add this line, but make sure to use the correct folder location for your JDK install:
```
PATH=$PATH:/cygdrive/c/Program\ Files/Java/jdk1.8.0_151/bin/
```

After adding this line, quite Cygwin and restart, then type `which javac` and it should print the folder that we just added.

### 7. build CNMAT-Externs

Finally we are ready to build the CNMAT-Externals for Windows 64 and 32 bit!

To build for Windows 64bit, enter the `CNMAT-Externs` directory and do:
```
make clean
make win64
```

To build for Windows 32bit, while in the `CNMAT-Externs` directory do:
```
make win
```

The built objects will be in the `/build/Release` folder.

Note that we don't need/want to `make clean` this time, since the Windows makefile routine cleans out the `.o` files but leaves the other platform Max object files (i.e. when building `.mxe64` the script leaves the `.mxe` objects in place). For the final step of making the release, it's simpler to have both platforms pre-built.

There are two scripts for creating the release for Windows 32 and 64 bit:
`./make-release-win.sh` and `./make-release-win64.sh`.

Update: there is also now a `./make-release-win32-64.sh` which bundles both bit flavors together.

*-- Rama Gottfried / Jeff Lubow*, 11/19/14, updated Sept 2017
