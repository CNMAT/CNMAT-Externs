ifeq ($(MAKECMDGOALS),win)
EXT = mxe
else
EXT = mxo
endif

SRCDIR = src
JAVAOBJECTNAMES = midifile
BUILDDIR = build/Release

C74SUPPORT = ../max6-sdk/c74support
MAX_INCLUDES = $(C74SUPPORT)/max-includes
MSP_INCLUDES = $(C74SUPPORT)/msp-includes
JIT_INCLUDES = $(C74SUPPORT)/jit-includes

win: CC = i686-w64-mingw32-gcc
win: LD = $(CC)
win: CFLAGS += -mno-cygwin -DWIN_VERSION -DWIN_EXT_VERSION -U__STRICT_ANSI__ -U__ANSI_SOURCE -std=c99 -O3 -DNO_TRANSLATION_SUPPORT -msse3
win: LDFLAGS = -mno-cygwin -shared #-static-libgcc
win: INCLUDES = -I/usr/i686-w64-mingw32/sys-root/mingw/include -I$(MAX_INCLUDES) -Iinclude -I$(MSP_INCLUDES) -Ilib -Ilib/Jehan-lib -I../gsl -I$(JIT_INCLUDES) -I../CNMAT-OSC/OSC-Kit -I../CNMAT-OSC/libOSC -I../fftw -I../fftw/api -I../CNMAT-SDIF/lib -Isrc/SDIF-Buffer -Iutility-library/search-path -I../libo -I../libomax
win: LIBS = -L$(MAX_INCLUDES) -lMaxAPI -L$(MSP_INCLUDES) -lMaxAudio -L$(JIT_INCLUDES) -ljitlib -lm

JAVA_EXT = class

ARCHIVE_EXT = tgz
STAGING_DIR = CNMAT_Externals

#OBJECTS = $(foreach f, $(OBJECTNAMES), $(BUILDDIR)/$(f).$(EXT))
#CFILES = $(foreach f, $(OBJECTNAMES), $(SRCDIR)/$(f)/$(f).c)

JAVAOBJECTS = $(foreach f, $(JAVAOBJECTNAMES), $(BUILDDIR)/$(f).class)
JAVAFILES = $(foreach f, $(JAVAOBJECTNAMES), $(BUILDDIR)/$(f).java)

$(BUILDDIR)/%.class: #$(SRCDIR)/%/%.java
	@echo java

CURRENT_VERSION_FILE = include/current_version.h

all: MACOBJECTS $(JAVAOBJECTS)

.PHONY: MACOBJECTS
MACOBJECTS: $(CURRENT_VERSION_FILE) 
	xcodebuild -target CNMAT-Externs -project CNMAT-Externs.xcodeproj -configuration Release

SIMPLEOBJECTNAMES = 2threshattack~ accumulate~ bpf decaying-sinusoids~ deinterleave firbank~ gridpanel interleave lcm list-accum list-interpolate migrator oscillators~ peqbank~ poly.bus~ poly.send~ rbfi res-transform resdisplay resonators~ sinusoids~ slipOSC threefates trampoline trend-report vsnapshot~ whichthread xydisplay cnmatrix~ waveguide~
SIMPLEOBJECTS = $(foreach f, $(SIMPLEOBJECTNAMES), $(BUILDDIR)/$(f).$(EXT))

MULTIPLEFILEOBJECTNAMES = harmonics~ randdist
MULTIPLEFILEOBJECTS = $(foreach f, $(MULTIPLEFILEOBJECTNAMES), $(BUILDDIR)/$(f).$(EXT))

GSLOBJECTNAMES = bdist bessel
GSLOBJECTS = $(foreach f, $(GSLOBJECTNAMES), $(BUILDDIR)/$(f).$(EXT))

FFTWOBJECTNAMES = firbank~ 
FFTWOBJECTS = $(foreach f, $(FFTWOBJECTNAMES), $(BUILDDIR)/$(f).$(EXT))

OSCOBJECTNAMES = OSC-route OSC-schedule OSC-timetag OpenSoundControl printit
OSCOBJECTS = $(foreach f, $(OSCOBJECTNAMES), $(BUILDDIR)/$(f).$(EXT))

JEHANOBJECTNAMES = analyzer~ #brightness~ loudness~ pitch~ 
JEHANOBJECTS = $(foreach f, $(JEHANOBJECTNAMES), $(BUILDDIR)/$(f).$(EXT))
JEHANDEPSNAMES = fft fftnobitrev
JEHANDEPS = $(foreach f, $(JEHANDEPSNAMES), $(BUILDDIR)/$(f).o)

SDIFOBJECTNAMES = roughness SDIF-buffer SDIF-fileinfo SDIF-info SDIF-listpoke SDIF-ranges SDIF-tuples
SDIFOBJECTS = $(foreach f, $(SDIFOBJECTNAMES), $(BUILDDIR)/$(f).$(EXT))
SDIFDEPSNAMES = sdif-buf sdif-mem sdif-sinusoids sdif-types sdif-util sdif sdif-interp-implem sdif-interp
SDIFDEPS = $(foreach f, $(SDIFDEPSNAMES), $(BUILDDIR)/$(f).o)

win: $(SIMPLEOBJECTS) $(MULTIPLEFILEOBJECTS) $(GSLOBJECTS) $(FFTWOBJECTS) $(OSCOBJECTS) $(SDIFOBJECTS) $(JEHANOBJECTS)

# Single file dependencies--just compile and stick the .o files in the build dir
$(BUILDDIR)/commonsyms.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/commonsyms.o $(MAX_INCLUDES)/common/commonsyms.c

$(JEHANDEPS): $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ lib/Jehan-lib$(subst $(BUILDDIR),,$(subst .o,,$@)).c

$(BUILDDIR)/libranddist.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/libranddist.o src/randdist/libranddist.c

$(BUILDDIR)/noise-table.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/noise-table.o src/harmonics~/noise-table.c

$(BUILDDIR)/OSC-pattern-match.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/OSC-pattern-match.o ../CNMAT-OSC/OSC-Kit/OSC-pattern-match.c

$(BUILDDIR)/OSC-client.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/OSC-client.o ../CNMAT-OSC/libOSC/OSC-client.c

$(BUILDDIR)/OSC-timetag-libOSC.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/OSC-timetag-libOSC.o ../CNMAT-OSC/libOSC/OSC-timetag.c

$(BUILDDIR)/OSC-timetag-ops.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/OSC-timetag-ops.o src/OSC-timetag/OSC-timetag-ops.c

$(BUILDDIR)/pqops.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/pqops.o src/OSC-schedule/pqops.c

$(BUILDDIR)/strptime.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/strptime.o ../libo/contrib/strptime.c

$(BUILDDIR)/myPrintOSCpacket.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/myPrintOSCPacket.o src/printit/myPrintOSCpacket.c

$(SDIFDEPS): $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ ../CNMAT-SDIF/lib$(subst $(BUILDDIR),,$(subst .o,,$@)).c

$(BUILDDIR)/open-sdif-file.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/open-sdif-file.o utility-library/search-path/open-sdif-file.c

# simple objects that have no dependencies
$(SIMPLEOBJECTS): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(LIBS)

# objects that need to link against the gsl
$(GSLOBJECTS): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(LIBS) -L../gsl/.libs -lgsl

# objects that need to link against fftw
$(FFTWOBJECTS): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(LIBS) -L../fftw -lfftw3f_i386

# objects that rely on one or more files scattered around the repo
$(BUILDDIR)/harmonics~.$(EXT): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(BUILDDIR)/noise-table.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(BUILDDIR)/noise-table.o $(LIBS)

# links against the gsl and libranddist.o
$(BUILDDIR)/randdist.$(EXT): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(BUILDDIR)/libranddist.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(BUILDDIR)/libranddist.o $(LIBS) -L../gsl/.libs -lgsl

$(JEHANOBJECTS): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(CURRENT_VERSION_FILE) #$(JEHANDEPS)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(LIBS) -L../libo -lo -L../libomax -lomax -L../fftw -lfftw3_i386

$(SDIFOBJECTS): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(SDIFDEPS) $(BUILDDIR)/open-sdif-file.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(SDIFDEPS) $(BUILDDIR)/open-sdif-file.o $(LIBS)

$(BUILDDIR)/OSC-route.$(EXT): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(BUILDDIR)/OSC-pattern-match.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(BUILDDIR)/OSC-pattern-match.o $(LIBS)

$(BUILDDIR)/OSC-timetag.$(EXT) $(BUILDDIR)/OSC-schedule.$(EXT): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(BUILDDIR)/OSC-timetag-ops.o $(BUILDDIR)/pqops.o $(BUILDDIR)/strptime.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c -I../libo/contrib
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(BUILDDIR)/OSC-timetag-ops.o $(BUILDDIR)/pqops.o $(BUILDDIR)/strptime.o $(LIBS) -lws2_32

$(BUILDDIR)/OpenSoundControl.$(EXT): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(BUILDDIR)/OSC-timetag-libOSC.o $(BUILDDIR)/OSC-client.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(BUILDDIR)/OSC-client.o $(BUILDDIR)/OSC-timetag-libOSC.o $(LIBS) -lws2_32

$(BUILDDIR)/printit.$(EXT): $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(BUILDDIR)/myPrintOSCpacket.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(subst $(EXT),,$@)o $(SRCDIR)$(subst $(BUILDDIR),,$(subst .$(EXT),,$@))$(subst $(BUILDDIR),,$(subst .$(EXT),,$@)).c
	$(LD) $(LDFLAGS) -o $(subst $(EXT),,$@)mxe $(subst $(EXT),,$@)o $(BUILDDIR)/commonsyms.o $(BUILDDIR)/myPrintOSCpacket.o $(LIBS) -lws2_32

.PHONY: $(CURRENT_VERSION_FILE)
$(CURRENT_VERSION_FILE):
	echo "#define CNMAT_EXT_VERSION \""`git describe --tags --long`"\"" > $(CURRENT_VERSION_FILE)
	echo "#define CNMAT_EXT_COMPILE_DATE \""`date`"\"" >> $(CURRENT_VERSION_FILE)

archive: $(STAGING_DIR).$(ARCHIVE_EXT)

$(STAGING_DIR).$(ARCHIVE_EXT):
	$(shell if test ! -d $(STAGING_DIR); then mkdir $(STAGING_DIR); fi)
	cp -r $(BUILDDIR)/*.$(EXT) $(STAGING_DIR)/
	$(shell cd $(SRCDIR) && for f in `ls`; do cp $$f/$$f.maxhelp ../$(STAGING_DIR); done)
	tar zcf $(STAGING_DIR).$(ARCHIVE_EXT) $(STAGING_DIR)

.PHONY: clean win-clean
clean:
	rm -f $(CURRENT_VERSION_FILE)
	rm -rf build
	xcodebuild clean
	rm -rf $(STAGING_DIR) $(STAGING_DIR).$(ARCHIVE_EXT)

win-clean:
	rm -f $(CURRENT_VERSION_FILE)
	rm -rf build
	rm -rf $(STAGING_DIR) $(STAGING_DIR).$(ARCHIVE_EXT)

$(BUILDDIR):
	[ -d $(BUILDDIR) ] || mkdir -p $(BUILDDIR)
