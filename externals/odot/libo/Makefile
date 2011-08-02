LIBO_BASENAMES = osc_match osc_timetag osc_bundle osc_error osc_mem osc_message strptime
LIBO_OBJECTS = $(foreach OBJ, $(LIBO_BASENAMES), $(OBJ).o) osc_scanner.o osc_parser.o
LIBO_CFILES = $(foreach F, $(LIBO_BASENAMES), $(F).c)
LIBO_HFILES = $(foreach F, $(LIBO_BASENAMES), $(F).h) osc.h

MAC-CFLAGS = -arch i386 -O3 -funroll-loops
WIN-CFLAGS = -mno-cygwin -O3 -funroll-loops -DWIN_VERSION

I = -I../../../c74support/max-includes -I../libo -I/System/Library/Frameworks/Carbon.framework/Headers -I/System/Library/Frameworks/CoreServices.framework/Headers

all: CFLAGS += $(MAC-CFLAGS)
all: CC = gcc
all: $(LIBO_CFILES) $(LIBO_HFILES) libo.a test/osc_test
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

test/osc_test: test/osc_test.c libo.a
	+cd test && $(CC) $(CFLAGS) -framework CoreServices -L.. -lo -lfl -I.. -o osc_test osc_test.c

.PHONY: clean
clean:
	rm -f *.o libo.a test/osc_test *~ osc_scanner.[ch] osc_parser.[ch]
	cd doc && rm -rf html latex man
