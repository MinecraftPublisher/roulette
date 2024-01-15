#!/bin/bash

# the anykey program is required for this to work. this script is macos only.
# source code of the anykey program provided below:
# simply exits with the pressed key as the return value, optionally prints a prompt

##include <stdio.h>
##include <stdlib.h>
#int main(int argc, char **argv) {
#	if(argc >= 2) printf("%s\n", argv[1]);
#	system("/bin/stty raw");
#	char x = getchar();
#	system("/bin/stty cooked");
#	return x;
#}

echo -e "tell application \"iTerm\"\ncreate window with default profile\ntell current session of current window\ndelay 1\nwrite text \"cd $PWD\"\nwrite text \"build/roulette r:4 flash ncenter nlogo nrep; anykey \\\"Press any key to continue...\\\"\"\nend tell\nend tell\nEOF" | osascript