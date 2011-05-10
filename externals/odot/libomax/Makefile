OMAX_BASENAMES = omax_expr 
OMAX_OBJECTS = $(foreach OBJ, $(OMAX_BASENAMES), $(OBJ).o) omax_parser.o omax_scanner.o
OMAX_CFILES = $(foreach F, $(OMAX_BASENAMES), $(F).c)
OMAX_HFILES = $(foreach F, $(OMAX_BASENAMES), $(F).h

CC = gcc
ARCH = -arch i386

I = -I../../../c74support/max-includes -I../libo -I/System/Library/Frameworks/Carbon.framework/Headers -I/System/Library/Frameworks/CoreServices.framework/Headers

all: libomax.a

libomax.a: $(OMAX_OBJECTS)
	libtool -static -o libomax.a omax_scanner.o omax_parser.o /usr/lib/libfl.a

omax_scanner.c: omax_scanner.l
	flex -o omax_scanner.c --header-file=omax_scanner.h omax_scanner.l
omax_scanner.o: omax_scanner.c omax_scanner.h
	$(CC) $(ARCH) $(I) -o omax_scanner.o -c omax_scanner.c

omax_parser.c: omax_parser.y
	bison -d -o omax_parser.c omax_parser.y
omax_parser.o: omax_parser.c omax_parser.h
	$(CC) $(ARCH) $(I) -o omax_parser.o -c omax_parser.c

%.o: %.c %.h
	$(CC) $(ARCH) $(I) -o $(notdir $(basename $@)).o -c $(notdir $(basename $@)).c

#omax_scanner.o: omax_scanner.c
#gcc -arch i386 -I../../../c74support/max-includes -I../libo -I/System/Library/Frameworks/Carbon.framework/Headers -I/System/Library/Frameworks/CoreServices.framework/Headers -o omax_scanner.o -c omax_scanner.c

#omax_parser.o: omax_parser.c
#gcc -arch i386 -I../../../c74support/max-includes -I../libo -I/System/Library/Frameworks/Carbon.framework/Headers -I/System/Library/Frameworks/CoreServices.framework/Headers -o omax_parser.o -c omax_parser.c

#omax_expr.o: omax_expr.c omax_expr.h
#gcc -arch i386 -I../../../c74support/max-includes -I../libo -I/System/Library/Frameworks/Carbon.framework/Headers -I/System/Library/Frameworks/CoreServices.framework/Headers -o omax_expr.o -c omax_expr.c

.PHONY: clean
clean:
	rm omax_scanner.[ch] omax_parser.[ch] *.o *.a
