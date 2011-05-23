OMAX_BASENAMES = omax_expr omax_util
OMAX_OBJECTS = $(foreach OBJ, $(OMAX_BASENAMES), $(OBJ).o) omax_parser.o omax_scanner.o
OMAX_CFILES = $(foreach F, $(OMAX_BASENAMES), $(F).c)
OMAX_HFILES = $(foreach F, $(OMAX_BASENAMES), $(F).h

CC = gcc
ARCH = -arch i386

I = -I../../../c74support/max-includes -I../libo -I/System/Library/Frameworks/Carbon.framework/Headers -I/System/Library/Frameworks/CoreServices.framework/Headers

all: libomax.a

libomax.a: $(OMAX_OBJECTS)
	libtool -static -o libomax.a $(OMAX_OBJECTS) /usr/lib/libfl.a

omax_scanner.c: omax_scanner.l
	flex -o omax_scanner.c --header-file=omax_scanner.h omax_scanner.l
omax_scanner.o: omax_scanner.c omax_scanner.h
	$(CC) $(ARCH) $(I) -o omax_scanner.o -c omax_scanner.c

omax_parser.c: omax_parser.y
	bison -d -o omax_parser.c omax_parser.y
omax_parser.o: omax_parser.c omax_parser.h
	$(CC) $(ARCH) $(I) -o omax_parser.o -c omax_parser.c

%.o: %.c %.h omax_parser.c omax_scanner.c omax_parser.y omax_scanner.l
	$(CC) $(ARCH) $(I) -o $(notdir $(basename $@)).o -c $(notdir $(basename $@)).c


.PHONY: clean
clean:
	rm omax_scanner.[ch] omax_parser.[ch] *.o *.a
