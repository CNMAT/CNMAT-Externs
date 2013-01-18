SRCDIR = src
OBJECTNAMES = $(shell ls src)
BUILDDIR = build/Release

EXT = mxo
win: EXT = mxe

OBJECTS = $(foreach f, $(OBJECTNAMES), $(BUILDDIR)/$(f).$(EXT))#$(addsuffix $(EXT), $(addprefix $(BUILDDIR)/, $(OBJECTNAMES)))
CFILES = $(foreach f, $(OBJECTNAMES), $(SRCDIR)/$(f)/$(f).c)

all: $(CFILES)
	xcodebuild -target CNMAT-Externs -project CNMAT-Externs.xcodeproj -configuration Release

%:
	xcodebuild -target $* -project CNMAT-Externs.xcodeproj -configuration Releases

.PHONY: clean
clean:
	rm -rf build
	xcodebuild clean
