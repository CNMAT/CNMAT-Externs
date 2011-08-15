LIBO_BASENAMES = osc_match osc_timetag osc_bundle osc_bundle_s osc_bundle_u osc_bundle_iterator_s osc_bundle_iterator_u osc_error osc_mem osc_message osc_message_s osc_message_u osc_message_iterator_s osc_message_iterator_u osc_atom_s osc_atom_u osc_array osc_atom_array_s osc_atom_array_u strptime OSCObject OSCMessage SerializedOSCMessage 
LIBO_OBJECTS = $(foreach OBJ, $(LIBO_BASENAMES), $(OBJ).o) osc_scanner.o osc_parser.o
LIBO_CFILES = $(foreach F, $(LIBO_BASENAMES), $(F).c)
LIBO_HFILES = $(foreach F, $(LIBO_BASENAMES), $(F).h) osc.h

CFLAGS += -Wall -O3 -funroll-loops

MAC-CFLAGS = -arch i386 -isysroot /Developer/SDKs/MacOSX10.7.sdk -mmacosx-version-min=10.5
WIN-CFLAGS = -mno-cygwin -DWIN_VERSION

I = -I../../../c74support/max-includes -I../libo -I/System/Library/Frameworks/Carbon.framework/Headers -I/System/Library/Frameworks/CoreServices.framework/Headers

all: CFLAGS += $(MAC-CFLAGS)
all: CC = gcc
all: $(LIBO_CFILES) $(LIBO_HFILES) libo.a
all: LIBTOOL = libtool -static -o libo.a $(LIBO_OBJECTS) /usr/lib/libfl.a


win: CFLAGS += $(WIN-CFLAGS)
win: CC = gcc-3
win: $(LIBO_CFILES) $(LIBO_HFILES) libo.a test/osc_test
win: LIBTOOL = ar cru libo.a $(LIBO_OBJECTS) /usr/lib/libfl.a

libo.a: $(LIBO_OBJECTS)
	rm -f libo.a
	$(LIBTOOL)
#ar -rcs libo.a $(LIBO_OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) $(I) -c -o $(notdir $(basename $@)).o $(notdir $(basename $@)).c

osc_scanner.c: osc_scanner.l osc_parser.c
	flex -o osc_scanner.c --prefix=osc_scanner_ --header-file=osc_scanner.h osc_scanner.l
osc_scanner.o: osc_scanner.c osc_scanner.h
	$(CC) $(CFLAGS) $(I) -o osc_scanner.o -c osc_scanner.c

osc_parser.c: osc_parser.y 
	bison -p osc_parser_ -d -o osc_parser.c osc_parser.y
osc_parser.o: osc_parser.c osc_parser.h
	$(CC) $(CFLAGS) $(I) -o osc_parser.o -c osc_parser.c

.PHONY: doc
doc:
	cd doc && doxygen Doxyfile

.PHONY: test test-clean
test: 
	$(MAKE) -C test

test-clean:
	$(MAKE) -C test clean

.PHONY: clean
clean:
	rm -f *.o libo.a test/osc_test *~ osc_scanner.[ch] osc_parser.[ch]
	cd doc && rm -rf html latex man
	cd test && $(MAKE) clean
