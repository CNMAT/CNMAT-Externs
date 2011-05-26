VERSION = 0.1
OBJDIR = $(CURDIR)/build
OBJECTS = o.accumulate o.bild o.build o.expr o.if o.mappatch o.message o.prepend o.print o.printbytes\
o.route o.select o.spew o.var
ODOT_BUFFER_LEN = 4096

ARCHIVE = o.tar.gz
UNSTABLE = o.unstable.tar.gz

SERVER_PATH = /home/www-data/berkeley.edu-cnmat.www/maxdl/files/odot/

ARCH = -arch i386 -arch ppc

all: $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).mxo) $(OBJDIR)/patches
win: $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).mxe) $(OBJDIR)/patches 

$(OBJDIR)/%.mxo: 
	+cd $(notdir $(basename $@)) && $(MAKE) -f Makefile -k
	+cd $(notdir $(basename $@)) && $(MAKE) -f Makefile -k install
	+cd $(notdir $(basename $@))/build/Deployment && cp -r $(notdir $(basename $@)).mxo $(OBJDIR)
	+cd $(notdir $(basename $@)) && cp -r $(notdir $(basename $@)).maxhelp $(OBJDIR)

$(OBJDIR)/%.mxe:
	+cd $(notdir $(basename $@)) && $(MAKE) win -f Makefile -k
	+cd $(notdir $(basename $@))/build-gcc && cp -r $(notdir $(basename $@)).mxe $(OBJDIR)
	+cd $(notdir $(basename $@)) && cp -r $(notdir $(basename $@)).maxhelp $(OBJDIR)

$(OBJDIR):
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)

$(OBJDIR)/patches:
	cp -r patches $(OBJDIR)

.PHONY: clean
clean: 
	for d in $(OBJECTS); do (cd $$d; $(MAKE) clean); done
	rm -rf $(OBJDIR)

.PHONY: install
install:
	@for d in $(OBJECTS); do (cd $$d; $(MAKE) install); done

release:
	rm -f build/$(ARCHIVE)
	cd build && tar -zcf $(ARCHIVE) *
	@read -p "Username: " UN; \
	scp build/$(ARCHIVE) $$UN@cnmat.berkeley.edu:$(SERVER_PATH)

unstable:
	rm -f build/$(UNSTABLE)
	cd build && tar -zcvf $(UNSTABLE) *
	@read -p "Username: " UN;\
	scp build/$(UNSTABLE) $$UN@cnmat.berkeley.edu:$(SERVER_PATH)
