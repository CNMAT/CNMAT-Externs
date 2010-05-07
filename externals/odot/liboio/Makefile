all: lib test

lib:
	xcodebuild -project oio.xcodeproj -configuration Release
test: lib
	cd test && gcc -framework CoreFoundation -framework IOKit -framework Carbon -I.. -L../build/Release -loio -o oio_test oio_test.c

.PHONY: clean
clean:
	rm -rf build
	cd test && rm -rf oio_test