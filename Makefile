clean:
	rm -rf ./build
	mkdir ./build/

build: clean
	clang roulette.c -o build/roulette

build_g: clean
	clang -g roulette.c -o build/roulette

run: build
	./build/roulette 3 fast

debug: build_g
	lldb ./build/roulette 3 fast

install: build
	cp ./build/roulette /usr/local/bin/roulette