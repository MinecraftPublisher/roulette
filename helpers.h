#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int round_count;

int point() { return rand() % round_count; }

int TIME_MODE = 2;
#define DEBUG_CONSTANT 1
void msleep(double ms) { usleep((int) (ms * 1000 * TIME_MODE * 500 * DEBUG_CONSTANT)); }

#define next()                                                                                     \
    pointer++;                                                                                     \
    if (pointer > round_count - 1) pointer = 0

#define back printf(BACK512 INDENT INDENT INDENT BACK512 BACK512)
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