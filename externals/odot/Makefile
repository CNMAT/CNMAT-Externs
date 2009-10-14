OBJDIR = $(CURDIR)/build
OBJECTS = o.compliment o.delay o.explode o.filter o.insert o.intersection o.list-accum o.apply o.merge o.message o.op o.pack o.packet o.pak o.prepend o.rename o.route o.split o.test o.timetag o.union
ODOT_BUFFER_LEN = 4096

all: $(OBJDIR) $(foreach OBJ, $(OBJECTS), $(OBJDIR)/$(OBJ).mxo) $(OBJDIR)/patches

$(OBJDIR)/%.mxo: 
	+cd $(notdir $(basename $@)) && $(MAKE) -f Makefile -k
	+cd $(notdir $(basename $@))/build/Deployment && cp -r $(notdir $(basename $@)).mxo $(OBJDIR)
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