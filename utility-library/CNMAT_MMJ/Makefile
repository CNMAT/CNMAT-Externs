BUILD_DIR = ./build
INSTALL_DIR = ./lib

all:  dir CNMAT_MMJ_common.o CNMAT_MMJ_SDIF.o macho-prefix.h.gch doc
	cd $(BUILD_DIR) && ar rcs libCNMAT_MMJ_ppc.a *_ppc.o
	cd $(BUILD_DIR) && ar rcs libCNMAT_MMJ_i386.a *_i386.o
	cd $(BUILD_DIR) && lipo -create libCNMAT_MMJ_ppc.a libCNMAT_MMJ_i386.a -output libCNMAT_MMJ.a
	cd $(BUILD_DIR) && ranlib libCNMAT_MMJ.a
	mv $(BUILD_DIR)/libCNMAT_MMJ.a $(INSTALL_DIR)
	cd $(BUILD_DIR) && rm *.a

CNMAT_MMJ_SDIF.o: macho-prefix.h.gch
	gcc -arch i386 -F/System/Library/Frameworks/Carbon.framework -F../../SDK/UB-SDK -I../search-path -I../../c74support/max-includes -I../../../sdif/lib -I../../externals/SDIF/SDIF-buffer -Iinclude -include macho-prefix.h -o $(BUILD_DIR)/CNMAT_MMJ_SDIF_i386.o -c src/CNMAT_MMJ_SDIF.c
	gcc -arch ppc -F/System/Library/Frameworks/Carbon.framework -F../../SDK/UB-SDK -I../search-path -I../../c74support/max-includes -I../../../sdif/lib -I../../externals/SDIF/SDIF-buffer -Iinclude -include macho-prefix.h -o $(BUILD_DIR)/CNMAT_MMJ_SDIF_ppc.o -c src/CNMAT_MMJ_SDIF.c

CNMAT_MMJ_common.o: macho-prefix.h.gch
	gcc -arch i386 -F/System/Library/Frameworks/Carbon.framework -F../../SDK/UB-SDK -I../search-path -I../../c74support/max-includes -I../../../OSC/libOSC -Iinclude -include macho-prefix.h -o $(BUILD_DIR)/CNMAT_MMJ_common_i386.o -c src/CNMAT_MMJ_common.c
	gcc -arch ppc -F/System/Library/Frameworks/Carbon.framework -F../../SDK/UB-SDK -I../search-path -I../../c74support/max-includes -I../../../OSC/libOSC -Iinclude -include macho-prefix.h -o $(BUILD_DIR)/CNMAT_MMJ_common_ppc.o -c src/CNMAT_MMJ_common.c

macho-prefix.h.gch:
	/Developer/usr/bin/gcc-4.0 -x c-header -arch i386 -fmessage-length=0 -pipe -Wno-trigraphs -fpascal-strings -fasm-blocks -Os -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4 -Wmost -Wno-four-char-constants -Wno-unknown-pragmas -F../../SDK/UB-SDK -I../../../sdif/lib -I../../utility-library/search-path -I../../c74support/max-includes  -c ../../c74support/max-includes/macho-prefix.h -o $(BUILD_DIR)/macho-prefix_i386.h.gch
	/Developer/usr/bin/gcc-4.0 -x c-header -arch ppc -fmessage-length=0 -pipe -Wno-trigraphs -fpascal-strings -fasm-blocks -Os -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4 -Wmost -Wno-four-char-constants -Wno-unknown-pragmas -F../../SDK/UB-SDK -I../../../sdif/lib -I../../utility-library/search-path -I../../c74support/max-includes  -c ../../c74support/max-includes/macho-prefix.h -o $(BUILD_DIR)/macho-prefix_ppc.h.gch

doc:
	doxygen Doxyfile

dir:
	touch $(BUILD_DIR)/exists
	touch $(INSTALL_DIR)/exists

.PHONY: clean
clean:
	rm $(BUILD_DIR)/* lib/*.a