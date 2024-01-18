#ifndef __ROULETTE_HEADER_ARGV_H
#define __ROULETTE_HEADER_ARGV_H

#include "defines.h"
#include "report.h"
#include <stdlib.h>

void process_argv(int argc, string *argv) {
    bool did_set_round_count = false;
    bool did_set_live_count = false;

    live_rounds = 1;

    for (int i = 1; i < argc; i++) {
        string cur = argv[ i ];

        // -------------- SPEED SETTINGS  -------------
        if (!strcmp(cur, "flash")) TIME_MODE = 1;
        else if (!strcmp(cur, "fast"))
            TIME_MODE = 2;
        else if (!strcmp(cur, "normal"))
            TIME_MODE = 4;
        else if (!strcmp(cur, "slow"))
            TIME_MODE = 8;
        // -------------- SPEED SETTINGS  -------------

        // -------------- REPORT SETTINGS -------------
        else if (!strcmp(cur, "nolog") || !strcmp(cur, "nlog"))
            no_log = true;
        else if (!strcmp(cur, "log"))
            no_log = false;
        else if (!strcmp(cur, "noreport") || !strcmp(cur, "nrep"))
            no_report = true;
        else if (!strcmp(cur, "report"))
            no_report = false;
        // -------------- REPORT SETTINGS -------------

        // --------------- STYLE SETTINGS -------------
        else if (!strcmp(cur, "nologo"))
            no_logo = true;
        else if (!strcmp(cur, "nlogo"))
            no_logo = true;
        else if (!strcmp(cur, "logo"))
            no_logo = false;

        else if (!strcmp(cur, "nocenter"))
            no_center = true;
        else if (!strcmp(cur, "ncenter"))
            no_center = true;
        else if (!strcmp(cur, "center"))
            no_center = false;
        // --------------- STYLE SETTINGS -------------

        // -------------- ROUND SETTINGS  -------------
        else if (!strncmp(cur, "r:", 2)) {
            if(did_set_round_count) {
                printf("ERROR: Cannot override round count. Already set to %i.\n", round_count);
                exit(1);
            }
            
            did_set_round_count = true;
            
            round_count = atoi(&cur[ 2 ]);
            if (round_count < 3) {
                printf(
                    "Not enough rounds. Rounds should be more than 3. Got: %i\n",
                    round_count);
            }
        }

        else if (!strncmp(cur, "l:", 2)) {
            if(did_set_live_count) {
                printf("ERROR: Cannot override live rounds. Already set to %i.\n", live_rounds);
                exit(1);
            }
            
            if(!did_set_round_count) {
                printf("ERROR: Cannot set live rounds before setting total rounds.\n");
                exit(1);
            }
            
            did_set_live_count = true;
            
            live_rounds = atoi(&cur[ 2 ]);
            if (live_rounds < 1 || live_rounds >= round_count) {
                printf(
                    "%s live rounds. Rounds should be between 1 and the number of rounds minus 1. Got: %i\n",
                    live_rounds < 1 ? "Not enough" : "Too many",
                    live_rounds);
            }
        }
        // -------------- ROUND SETTINGS  -------------

        // -------------- HELP SETTINGS  --------------
        else if (!strcmp(cur, "help") || !strcmp(cur, "?") || !strcmp(cur, "-h")) {
            printf(
                "# Russian roulette\n"
                "Play russian roulette in your CLI.\n\n"
                "## Options:\n"
                "    %s [.. argument list ..]\n\n"

                "## Arguments list:\n"
                "You can insert any of the following in any order:\n"
                "1. flash / fast / normal / slow -> Set timings, Speed increases from left to "
                "right.\n"
                "2. nolog / nlog / log -> Switch wether the game report should be printed to the "
                "console or not.\n"
                "3. nologo / nlogo / logo -> Switch wether the gun ASCII art should be printed to "
                "the console or not.\n"
                "4. nocenter / ncenter / center -> Switch wether the text should be centered or "
                "not.\n"
                "5. noreport / nrep / report -> Switch wether the game report should be written to "
                "a file named `output.roulette`.\n"
                "6. r:<round_count> -> Set the number of rounds. Should be more than 3.\n"
                "7. l:<live_rounds> -> Set the number of live rounds. Should be between 1 and the number of rounds minus 1.\n"
                "8. help / ? / -h -> Show this menu and exit.\n\n"

                "## Examples:\n"
                "    %s flash nlog nrep r:24\n"
                "    |- Play on the fastest time, Do not log and Do not report, 24 rounds.\n"
                "    %s slow r:8\n"
                "    |- Play on the slowest time, 8 rounds.\n\n",

                argv[ 0 ],
                argv[ 0 ],
                argv[ 0 ]);

            exit(0);
        }
        // -------------- HELP SETTINGS  --------------
    }
}

#endif