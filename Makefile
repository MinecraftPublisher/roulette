clean:
	rm -rf ./build
	mkdir ./build/

build: clean
	clang src/roulette.c -o build/roulette

build_g: clean
	clang -g src/roulette.c -o build/roulette

build_r: clean
	clang src/roulette.c -O3 -o build/roulette

run: build
	./build/roulette r:3 fast ncenter nlogo

debug: build_g
	lldb ./build/roulette r:3 fast ncenter nlogo

install: build_r
	cp ./build/roulette /usr/local/bin/roulette