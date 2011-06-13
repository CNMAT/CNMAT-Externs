LIBO_BASENAMES = osc_match osc_timetag osc_bundle osc_error osc_mem osc_message osc_bundle
LIBO_OBJECTS = $(foreach OBJ, $(LIBO_BASENAMES), $(OBJ).o)
LIBO_CFILES = $(foreach F, $(LIBO_BASENAMES), $(F).c)
LIBO_HFILES = $(foreach F, $(LIBO_BASENAMES), $(F).h) osc.h

CFLAGS += -arch i386 -O3 -funroll-loops
WIN-CFLAGS += -mno-cygwin -O3 -funroll-loops
#ARCH = -arch i386 #-arch ppc
CC = gcc
#OPTIMIZATION = -O3 -funroll-loops

all: $(LIBO_CFILES) $(LIBO_HFILES)
	$(MAKE) libo.a
	$(MAKE) test/osc_test

win: $(LIBO_CFILES) $(LIBO_HFILES)
	echo cflags $(WIN-CFLAGS)
	$(MAKE) libo.a
	$(MAKE) test/osc_test

libo.a: $(LIBO_OBJECTS)
	rm -f libo.a
	ar -rcs libo.a $(LIBO_OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $(notdir $(basename $@)).o $(notdir $(basename $@)).c

.PHONY: doc
doc:
	cd doc && doxygen Doxyfile

test/osc_test: test/osc_test.c libo.a
	+cd test && $(CC) $(CFLAGS) -L.. -lo -I.. -o osc_test osc_test.c

.PHONY: clean
clean:
	rm -f *.o libo.a test/osc_test *~
	cd doc && rm -rf html latex man
