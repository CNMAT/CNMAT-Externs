TOP_LEVEL = ../../

all: java jnilib jar doc

java: 
	cd $(TOP_LEVEL) && javac cnmat/sdif/SDIFTypes.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/SDIFBadTypeException.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/SDIFHeader.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/SDIFGlobalHeader.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/SDIFFrameHeader.java
	cd $(TOP_LEVEL) && javac cnmat/sdif/SDIFMatrixHeader.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIFMatrix.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIFMatrix1TRC.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIFMatrix1RES.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIFMatrix1PIC.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIFMatrix1HRM.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIFMatrix1HRE.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIFMatrix1FQ0.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIFBuffer.java

SDIFDuffer.class: SDIFBuffer.java
	cd $(TOP_LEVEL) && javac -cp .:lib/max.jar cnmat/sdif/SDIFBuffer.java

SDIFBuffer_native.h: SDIFBuffer.class
	cd $(TOP_LEVEL) && javah -jni -o cnmat/sdif/native/SDIFBuffer_native.h cnmat.sdif.SDIFBuffer

jnilib: libSDIFBuffer_native.o
	cd native && gcc -dynamiclib -F../../../../SDK/UB-SDK -L../../../../utility-library/CNMAT_MMJ/lib -filelist SDIFBuffer_native.LinkFileList -framework MaxAPI -lCNMAT_MMJ -o libSDIFBuffer_native.dylib libSDIFBuffer_native.o

libSDIFBuffer_native.o: macho-prefix.h.gch SDIFBuffer_native.h
	cd native && gcc -F/System/Library/Frameworks/Carbon.framework -F../../../../SDK/UB-SDK -I../../../../utility-library/search-path -I/System/Library/Frameworks/JavaVM.framework/Headers -I../../../../c74support/max-includes -I../../../../../sdif/lib -I../../../../externals/SDIF/SDIF-buffer -I../../../../utility-library/CNMAT_MMJ/include -include macho-prefix.h -o libSDIFBuffer_native.o -c SDIFBuffer_native.c

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
	rm -f native/*.o native/*.dylib *.class native/*.gch 
	rm -rf ./doc/*