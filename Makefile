SRCDIR = src
OBJECTNAMES = $(shell ls src)
JAVAOBJECTNAMES = midifile
BUILDDIR = build/Release

C74SUPPORT = ../max6-sdk/c74support
MAX_INCLUDES = $(C74SUPPORT)/max-includes
MSP_INCLUDES = $(C74SUPPORT)/msp-includes

EXT = mxe

win: EXT = mxe
win: CC = i686-w64-mingw32-gcc
win: LD = $(CC)
win: CFLAGS += -mno-cygwin -DWIN_VERSION -DWIN_EXT_VERSION -U__STRICT_ANSI__ -U__ANSI_SOURCE -std=c99 -O3 -DNO_TRANSLATION_SUPPORT
win: INCLUDES = -I$(MAX_INCLUDES) -Iinclude -I$(MSP_INCLUDES) 
win: LIBS = -L$(MAX_INCLUDES) -lMaxAPI -L$(MSP_INCLUDES) -lMaxAudio

JAVA_EXT = class

ARCHIVE_EXT = tgz
STAGING_DIR = CNMAT_Externals

OBJECTS = $(foreach f, $(OBJECTNAMES), $(BUILDDIR)/$(f).$(EXT))
CFILES = $(foreach f, $(OBJECTNAMES), $(SRCDIR)/$(f)/$(f).c)

JAVAOBJECTS = $(foreach f, $(JAVAOBJECTS), $(BUILDDIR)/$(f).$(JAVA_EXT))
JAVAFILES = $(foreach f, $(JAVAOBJECTS), $(BUILDDIR)/$(f).java)

$(BUILDDIR)/%.$(JAVA_EXT): $(SRCDIR)/%/%.java

CURRENT_VERSION_FILE = include/current_version.h

all: $(CFILES) $(CURRENT_VERSION_FILE)
	xcodebuild -target CNMAT-Externs -project CNMAT-Externs.xcodeproj -configuration Release

.PHONY: win
win: $(OBJECTS)

$(BUILDDIR)/commonsyms.o: $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/commonsyms.o $(MAX_INCLUDES)/common/commonsyms.c

$(BUILDDIR)/%.mxe: $(BUILDDIR) $(BUILDDIR)/commonsyms.o $(CURRENT_VERSION_FILE)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(BUILDDIR)/$*.o $(SRCDIR)/$*/$*.c
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/$*.mxe $(BUILDDIR)/$*.o $(BUILDDIR)/commonsyms.o $(LIBS)

$(CURRENT_VERSION_FILE):
	echo "#define CNMAT_EXT_VERSION \""`git describe --tags --long`"\"" > $(CURRENT_VERSION_FILE)
	echo "#define CNMAT_EXT_COMPILE_DATE \""`date`"\"" >> $(CURRENT_VERSION_FILE)

archive: $(STAGING_DIR).$(ARCHIVE_EXT)

$(STAGING_DIR).$(ARCHIVE_EXT):
	$(shell if test ! -d $(STAGING_DIR); then mkdir $(STAGING_DIR); fi)
	cp -r $(BUILDDIR)/*.$(EXT) $(STAGING_DIR)/
	$(shell cd $(SRCDIR) && for f in `ls`; do cp $$f/$$f.maxhelp ../$(STAGING_DIR); done)
	tar zcf $(STAGING_DIR).$(ARCHIVE_EXT) $(STAGING_DIR)

.PHONY: clean
clean:
	rm -f $(CURRENT_VERSION_FILE)
	rm -rf build
	xcodebuild clean
	rm -rf $(STAGING_DIR) $(STAGING_DIR).$(ARCHIVE_EXT)

$(BUILDDIR):
	[ -d $(BUILDDIR) ] || mkdir -p $(BUILDDIR)
