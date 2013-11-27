.PHONY: all install install-dbg clean test

all: debug release-build

check: debug
	LD_LIBRARY_PATH=build/debug build/debug/test

debug: build/debug/Makefile
	make -C build/debug

release-build: build/release/Makefile
	make -C build/release

install: release-build
	make -C build/release install

clean:
	rm -rf build test

build/debug/Makefile: | build/debug
	cd build/debug; \
	  cmake -DCMAKE_BUILD_TYPE=Debug ../..

build/release/Makefile: | build/release
	cd build/release; \
	  cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ../..

build/debug:
	mkdir -p $@

build/release:
	mkdir -p $@
