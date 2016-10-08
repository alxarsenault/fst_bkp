all: example

.PHONY: install
install:
	cp -r include/fst /usr/local/include/

.PHONY: format
format:
	clang-format -i include/fst/print

.PHONY: xcode
xcode:
	@mkdir -p cmake-xcode
	cd cmake-xcode && cmake -GXcode .. && open fst.xcodeproj

.PHONY: example
example: 
	@mkdir -p build
	clang++ -std=c++11 example/print/main.cpp -I include/ -o build/print

clean:
	rm -rf build
	rm -rf cmake-build
	rm -rf cmake-xcode
