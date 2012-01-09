VERSION = 0.1
OBJDIR = $(CURDIR)/build
# o.build has to come before o.bild so that the version.[ch] will be there
OBJECTS = o.accumulate o.change o.cond o.unless o.when o.expr o.if o.mappatch o.message \
o.pack o.pak o.prepend\
o.print o.printbytes o.route o.select o.spew o.var o.union o.intersection o.difference
ODOT_CFILES = $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).c)
ODOT_HFILES = $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).h)
ODOT_MXO = $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).mxo)
ODOT_MXE = $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).mxe)

#ODOT_BUFFER_LEN = 4096

ARCHIVE = o.tar.gz
UNSTABLE = o.unstable.tar.gz
WIN-RELEASE = o.win.tar.gz

SERVER_PATH = /home/www-data/berkeley.edu-cnmat.www/maxdl/files/odot/

ARCH = -arch i386 -arch ppc

all: $(OBJDIR) $(ODOT_MXO) $(OBJDIR)/patches
win: $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).mxe) $(OBJDIR)/patches 

$(ODOT_MXO):
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
	rm -rf $(OBJDIR)
	for d in $(OBJECTS); do (cd $$d; $(MAKE) clean); done

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

win-release:
	rm -f build/$(WIN-RELEASE)
	cd build && tar -zcvf $(WIN-RELEASE) *
	@read 0p "Username: " UN;\
	scp build/$(WIN-RELEASE) $$UN@cnmat.berkeley.edu:$(SERVER_PATH)
