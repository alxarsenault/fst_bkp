all: example

.PHONY: install
install:
	cp -r include/* /usr/local/include/

.PHONY: format
format:
	find ./ -iname *.hpp -o -iname *.h -o -iname *.cpp | xargs clang-format -i

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
