VERSION = 0.1
OBJDIR = $(CURDIR)/build
LIBODIR = $(CURDIR)/libo
OBJECTS = o.accumulate o.bild o.build o.if o.io o.mappatch o.message o.prepend o.print o.printbytes\
o.route o.select o.spew o.var 
LIBO_FILES = osc_match osc_timetag osc_util osc_info osc_error osc_mem
ODOT_BUFFER_LEN = 4096

ARCH = -arch i386 -arch ppc

all: $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).mxo) $(OBJDIR)/patches $(LIBODIR)/libo.a
win: $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).mxe) $(OBJDIR)/patches $(LIBODIR)/libo.a
libo: $(LIBODIR)/libo.a $(foreach F, $(LIBO_FILES), $(LIBODIR)/$(F).c)

$(OBJDIR)/%.mxo: $(LIBODIR)/libo.a
	+cd $(notdir $(basename $@)) && $(MAKE) -f Makefile -k
	+cd $(notdir $(basename $@))/build/Deployment && cp -r $(notdir $(basename $@)).mxo $(OBJDIR)
	+cd $(notdir $(basename $@)) && cp -r $(notdir $(basename $@)).maxhelp $(OBJDIR)

$(OBJDIR)/%.mxe: $(LIBODIR)/libo.a
	+cd $(notdir $(basename $@)) && $(MAKE) win -f Makefile -k
	+cd $(notdir $(basename $@))/build-gcc && cp -r $(notdir $(basename $@)).mxe $(OBJDIR)
	+cd $(notdir $(basename $@)) && cp -r $(notdir $(basename $@)).maxhelp $(OBJDIR)

$(OBJDIR):
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)

$(LIBODIR)/libo.a: $(foreach OBJ, $(LIBO_FILES), $(LIBODIR)/$(OBJ).o) $(foreach F, $(LIBO_FILES), $(LIBODIR)/$(F).c)
	+cd $(LIBODIR) && ar -rcs libo.a $(foreach OBJ, $(LIBO_FILES), $(LIBODIR)/$(OBJ).o)

$(LIBODIR)/%.o:
	+cd $(LIBODIR) && gcc $(ARCH) -c -o $(notdir $(basename $@)).o $(notdir $(basename $@)).c

$(OBJDIR)/patches:
	cp -r patches $(OBJDIR)

.PHONY: clean
clean: clean-libo
	rm -f o.tgz
	for d in $(OBJECTS); do (cd $$d; $(MAKE) clean); done
	rm -rf $(OBJDIR)

.PHONY: clean-libo
clean-libo:
	cd $(LIBODIR) && rm -f *.o libo.a

.PHONY: install
install:
	@for d in $(OBJECTS); do (cd $$d; $(MAKE) install); done
#@cd patches/abstractions && $(MAKE) install

release:
	rm -f o.tar.gz
	tar -zcf o.tar.gz build/*
	@read -p "Username: " UN; \
	scp o.tar.gz $$UN@cnmat.berkeley.edu:/home/www-data/berkeley.edu-cnmat.www/maxdl/files/odot/
