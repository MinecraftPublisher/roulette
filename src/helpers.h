#pragma once

#include "argv.h"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int point() { return rand() % round_count; }

#define DEBUG_CONSTANT 1
void msleep(double ms) { usleep((int) (ms * 1000 * TIME_MODE * 250 * DEBUG_CONSTANT)); }

#define next()                                                                                     \
    pointer++;                                                                                     \
    if (pointer > round_count - 1) pointer = 0

#define back printf(BACK512 BACK512 BACK512 "%s%s%s", INDENT, INDENT, INDENT)
#define step                                                                                       \
    fflush(stdout);                                                                                \
    msleep(0.5);                                                                                   \
                                                                                                   \
    printf(".");                                                                                   \
    fflush(stdout);                                                                                \
    msleep(1);                                                                                     \
                                                                                                   \
    printf(".");                                                                                   \
    fflush(stdout);                                                                                \
    msleep(1.5)