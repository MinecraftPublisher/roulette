#ifndef __ROULETTE_HEADER_HELPERS_H
#define __ROULETTE_HEADER_HELPERS_H

#include "defines.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int point() { return rand() % round_count; }

void msleep(double ms) { usleep((int) (ms * 1000 * TIME_MODE * 250 * DEBUG_CONSTANT)); }

#define next()                                                                                     \
    revolver_pointer++;                                                                            \
    if (revolver_pointer > round_count - 1) revolver_pointer = 0

#define back printf("\33[2K\r");
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

#endif