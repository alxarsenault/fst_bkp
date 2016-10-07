all: example

.PHONY: install
install:
	cp -r include/fst /usr/local/include/

.PHONY: format
format:
	clang-format -i include/fst/print

.PHONY: example
example: 
	@mkdir -p build
	clang++ -std=c++11 example/main.cpp -I include/ -o build/example

clean:
	rm -rf build/
