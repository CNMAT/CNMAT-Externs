CNMAT-Externs
=============

Details on building the CNMAT-Externs repository.

## Folder structure

In general, the build settings expect all the repositories to be in the same root folder.
Identify or create this folder and proceed.

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

Download FFTW [here](http://fftw.org/download.html), and extract the package into our folder.

Then, cd into the fftw directory.  Since we are still supporting i386 processors, you need to configure `fftw` to build a universal version. To configure for i386 and x86_64:

1. `./configure CC="gcc -arch arm64 -arch x86_64" CXX="g++ -arch arm64 -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E"`

2. Then `sudo make` to compile the `libfftw3.a` in the `../fftw/.libs` directory.  Note that you don't invoke `make install` as you need the xcode project to refer to this specific location.
This is the (default) double precision version. However, there are some files that refer to the floating point version. To add the floating point version you need to repeat the above steps, but with the single precision flag `--enable-single`:

3. `./configure CC="gcc -arch arm64 -arch x86_64" CXX="g++ -arch arm64 -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E" --enable-single`

4. Then `sudo make` to compile the `libfftw3f.a` in the `../fftw/.libs` directory.

5. Finally, rename the folder to `fftw`

*Note: starting with Xcode 10, the standard Mac command-line tools no longer support 32bit (i386) processors, so you may need to remove  `-arch i386` from the configuration flags above if you want to build a 64bit only version with Xcode 10.*

### 3. GSL

Download GSL [here](http://gnu.mirror.constant.com/gsl/).  Find the package you're interested in (latest stable release), and extract the package into our folder.

Configuring GSL is the same situation as FFTW:  configure for both i386 and x86_64:

1. `./configure CC="gcc -arch arm64 -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E"`

2. `sudo make`

3. Finally, rename the folder to `gsl`

*Note: starting with Xcode 10, the standard Mac command-line tools no longer support 32bit (i386) processors, so you may need to remove  `-arch i386` from the configuration flags above if you want to build a 64bit only version with Xcode 10.*

*Note also: it is possible that the `libgsl.a` file is now compiled into the `/gsl/.libs` folder. Currently, you may need to copy this file into the main `/gsl` folder for the Makefile script to build correctly. The default project path settings may need to be updated in the near future.*


### 4. libo / libomax

libo requires GNU [flex](https://github.com/westes/flex) and [bison](https://www.gnu.org/software/bison/) to be installed and findable by your bash shell.

Build `libo` first, and then `libomax`, using the `make`.

More details can be found [here](https://github.com/CNMAT/libo/blob/master/doc/libo-documentation/odot-build-setup.txt).

### 5. build CNMAT-Externs

Finally you are ready to build the CNMAT-Externals!

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

For building on Windows, we are working on Windows 10, 8, and 7, using the MinGW-w64 complier on Cygwin64, with separate tool chains for 32 and 64 bit externals.

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

### 2. on running Cygwin, privileges, and whitespace
You will need to run Cygwin as an administrator when you run the commands outlined below, otherwise portions of the build process can fail.  If you've created a Cygwin terminal icon on the desktop, you can right-click on it and choose "Run as Administrator" or check the relevant box after going into the `Compatibility` tab of the properties window for this icon.  As of this writing some of the following commands will fail if there is white-space in the user's account name. For example, a user account path such as '/cygdrive/c/cygwin64/home/John\ Doe' will be truncated to '/cygdrive/c/cygwin64/home/John', causing a fatal error in some of the builds.  If this happens, you can rename your account name without spaces, e.g. '/cygdrive/c/cygwin64/home/John_Doe'.

### 2. GSL
Download the GSL library from the GNU site: [link](ftp://ftp.gnu.org/gnu/gsl/) (currently using v2.4).

In Cygwin, type:
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

then you can go back to our root folder and clean up a bit:
```
cd ..
rm *.gz
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

Note that you need to `make clean` before building the new version, otherwise the library will be created with the already complied `.o` files.

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

Install the suggested version of the JDK for your version of Windows, including the public JRE (if you want to run the Java classes we're building).

Once this is installed, you need to tell Cygwin where to find it. To do this, you need to add a folder to our `PATH` variable in the file `.bash_profile` in your Cygwin user home folder, found at: `C:/cygwin64/home/<your_username>/.bash_profile`.

Open `.bash_profile` and add this line, making sure to use the correct folder location for your JDK install.  Here is an example:
```
PATH=$PATH:/cygdrive/c/Program\ Files/Java/jdk1.8.0_161/bin/
```

After adding this line, quit Cygwin and restart, then type `which javac` and it should print the location that you just added.

### 7. build CNMAT-Externs

Finally, you are ready to build the CNMAT-Externals for Windows 64 and 32 bit!
(Just a reminder:  Run Cygwin as administrator)

To build for Windows 64bit, enter the `CNMAT-Externs` directory and do:
```
make win-clean
make win64
```

To build for Windows 32bit, while in the `CNMAT-Externs` directory do:
```
make win
```

The built objects will be in the `/build/Release` folder.

Note that if you are planning to use the release script, you don't need to `make win-clean` at this point, although you may in fact need to run it later if you have modified something.

To bundle the release, run 

```
./make-release-win32-64.sh
```
which bundles both 32 and 64 bit objects together.

*-- Rama Gottfried / Jeff Lubow*, 11/19/14, updated Jan 2018
