clean:
	rm -rf ./roulette

build:
	clang roulette.c -o roulette

run: build
	./roulette

install: build
	cp ./roulette /usr/local/bin/roulette