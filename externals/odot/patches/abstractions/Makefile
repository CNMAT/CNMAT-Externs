ifndef CNMAT_MAX_INSTALL_DIR
	INSTALL_DIR = $(CURDIR)/build
	export INSTALL_DIR
else
	INSTALL_DIR = $(CNMAT_MAX_INSTALL_DIR)
	export INSTALL_DIR
endif

#install: $(INSTALL_DIR)
#	$(foreach DIR, $(shell ls -p | egrep '.*/'), \
#		$(shell cp $(DIR)/*.max* $(INSTALL_DIR)))

install: $(INSTALL_DIR)
	cp *.max* $(INSTALL_DIR)/

$(INSTALL_DIR):
	@[ -d $(INSTALL_DIR) ] || mkdir -p $(INSTALL_DIR)
