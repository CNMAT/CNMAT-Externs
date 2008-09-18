TOP_LEVEL = ../../

all: java jnilib jar doc

java: SDIFDataTypes.java SDIFBadTypeException.java Header.java GlobalHeader.java FrameHeader.java MatrixHeader.java SDIF_buffer.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/SDIFDataTypes.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/SDIFBadTypeException.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/Header.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/GlobalHeader.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/FrameHeader.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/MatrixHeader.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIF_buffer.java

SDIF_buffer.class: SDIF_buffer.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIF_buffer.java

SDIF_buffer_native.h: SDIF_buffer.class
	cd $(TOP_LEVEL) && javah -jni -o cnmat/sdif/native/SDIF_buffer_native.h cnmat.sdif.SDIF_buffer

jnilib: libSDIF_buffer_native.o
	cd native && gcc -dynamiclib -F../../../../SDK/UB-SDK -filelist SDIF_Buffer_native.LinkFileList -framework MaxAPI -o libSDIF_buffer_native.dylib libSDIF_buffer_native.o

libSDIF_buffer_native.o: macho-prefix.h.gch SDIF_buffer_native.h
	cd native && gcc -F/System/Library/Frameworks/Carbon.framework -F../../../../SDK/UB-SDK -I../../../../utility-library/search-path -I/System/Library/Frameworks/JavaVM.framework/Headers -I../../../../c74support/max-includes -I../../../../../sdif/lib -I../../../../externals/SDIF/SDIF-buffer -include macho-prefix.h -o libSDIF_buffer_native.o -c SDIF_buffer_native.c

macho-prefix.h.gch:
	cd native && /Developer/usr/bin/gcc-4.0 -x c-header -arch i386 -fmessage-length=0 -pipe -Wno-trigraphs -fpascal-strings -fasm-blocks -Os -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4 -Wmost -Wno-four-char-constants -Wno-unknown-pragmas -F../../../../SDK/UB-SDK -I../../../../../sdif/lib -I../../../../utility-library/search-path -I../../../../c74support/max-includes  -c ../../../../c74support/max-includes/macho-prefix.h -o macho-prefix.h.gch

jar: 
	cd $(TOP_LEVEL) && jar cvf cnmat/cnmat.jar cnmat/sdif/*.class cnmat/sdif/native/*.dylib

install: jar
	cd $(TOP_LEVEL) && cp cnmat/cnmat.jar lib/cnmat.jar
	cd $(TOP_LEVEL) && rm cnmat/cnmat.jar
doc: java
	doxygen Doxyfile

.PHONY: clean
clean:
	rm native/*.o native/*.dylib *.class native/*.gch