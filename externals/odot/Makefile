VERSION = 0.1
OBJDIR = $(CURDIR)/build
OBJECTS = o.accumulate o.bild o.build o.if o.mappatch o.message o.prepend o.print o.printbytes\
o.route o.select o.spew o.var
ODOT_BUFFER_LEN = 4096

all: $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).mxo) $(OBJDIR)/patches

$(OBJDIR)/%.mxo: 
	+cd $(notdir $(basename $@)) && $(MAKE) -f Makefile -k
	+cd $(notdir $(basename $@))/build/Deployment && cp -r $(notdir $(basename $@)).mxo $(OBJDIR)
	+cd $(notdir $(basename $@)) && cp -r $(notdir $(basename $@)).maxhelp $(OBJDIR)


win: $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).mxe) $(OBJDIR)/patches

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
	rm o.tgz
	for d in $(OBJECTS); do (cd $$d; $(MAKE) clean); done
	rm -rf $(OBJDIR)

.PHONY: install
install:
	@for d in $(OBJECTS); do (cd $$d; $(MAKE) install); done

release:
	cd build && rm -f *.tgz
	cd build && tar -zcf o.$(VERSION).tgz *
	@read -p "Username: " UN; \
	scp build/o.tgz $$UN@cnmat.berkeley.edu:/home/www-data/berkeley.edu-cnmat.www/maxdl/files/odot/
