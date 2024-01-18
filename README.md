# roulette.c
[![Build roulette](https://github.com/MinecraftPublisher/roulette/actions/workflows/build.yml/badge.svg)](https://github.com/MinecraftPublisher/roulette/actions/workflows/build.yml)

Harmless and fun russian roulette written in C. AI is intermediate. Useful for killing time.

## Usage
    roulette [.. argument list ..]

## Arguments list
You can insert any of the following in any order:
1. flash / fast / normal / slow → Set timings, Speed increases from left to right.
2. nolog / nlog / log → Switch wether the game report should be printed to the console or not.
3. nologo / nlogo / logo → Switch wether the gun ASCII art should be printed to the console or not.
4. nocenter / ncenter / center → Switch wether the text should be centered or not.
5. noreport / nrep / report → Switch wether the game report should be written to a file named `output.roulette`.
6. r:<round_count> → Set the number of rounds in the revolver. Should be more than 3.
7. l:<live_rounds> → Set the number of rounds. Should be between 1 and <round_count> - 1.
8. help / ? / -h → Show this menu and exit.

## Examples
    roulette flash nlog nrep r:24
    |- Play on the fastest time, Do not log and Do not report, 24 rounds.
    roulette slow r:8
    |- Play on the slowest time, 8 rounds.
    roulette flash r:15 l:9
    |- Play with 9 live bullets and 6 blanks. Fastest time.

## Building & Installation
- Build with `make build`.
- Run with `make run`.
- Install to `/usr/local/bin` with `make install`.