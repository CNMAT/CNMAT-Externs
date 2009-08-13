all:	build

build:
	cd o.op; make -i
	cd o.rename; make -i
	cd o.pack; make -i
	cd o.pak; make -i
	cd o.route; make -i

install: build
	cd o.op; make install
	cd o.rename; make install
	cd o.pack; make install
	cd o.pak; make install
	cd o.route; make install

clean:
	cd o.op; make clean
	cd o.rename; make clean
	cd o.pack; make clean
	cd o.pak; make clean
	cd o.route; make clean