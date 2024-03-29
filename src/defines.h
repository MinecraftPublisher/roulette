#ifndef __ROULETTE_DEFINES_H
#define __ROULETTE_DEFINES_H

#include "styles.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG_CONSTANT 1

#define false 0
#define true  1
#define uturn true
#define aturn false

typedef char byte;
typedef byte bool;

enum ACTION { SHOOT_U, SHOOT_A, ROLL, CHECK_CHAMBER };

struct TURN {
    bool        turn;
    enum ACTION action;
    byte       *revolver;
    int         pointer;
};

// argv option variables
bool turn;
bool no_report;
bool no_log;
bool no_logo;
bool no_center;
int  round_count;

int          report_size;
int          report_pointer;
struct TURN *report_logs;
bool         report_initialized;

typedef char *string;

// excuse me for the interesting naming conventions :)
byte *revolver;
int   revolver_pointer;

enum ACTION user_choice;
bool        turn;

// ai and logic related variables
enum NEXT_CHAMBER { BLANK_ROUND, LIVE_ROUND, NO_IDEA };
int chambersSinceLastRoll = 0;
int remainder_rounds;
int live_rounds;

// turn-playing mechanisms
void TURN();
void END();
int  point();

int TIME_MODE;
int round_count;

void msleep(double ms);

void memory_error(string __FILE, int __LINE) {
    printf(
        "Memory error occured.\n"
        "This usually means the system didn't give me memory,\n"
        "Or the system doesn't even have enough memory to give it to me.\n"
        "This usually cannot be recovered from, And I would have to close the app.\n"
        "Sorry about that!\n\nTechnical details:\nFile name: %s\nLine number: %d\n",
        __FILE,
        __LINE);
    exit(EXIT_FAILURE);
}

#define E_ALLOC_LIMIT 50

void *__e_malloc(size_t size, string __FILE, int __LINE) {
    void *result = malloc(size);
    int   i      = 0;
    while (result == NULL && i < E_ALLOC_LIMIT) {
        msleep(0.01);
        result = malloc(size);
    }
    if (result == NULL) memory_error(__FILE, __LINE);

    return result;
}

void *__e_realloc(void *ptr, size_t size, string __FILE, int __LINE) {
    void *result = realloc(ptr, size);
    int   i      = 0;
    while (result == NULL && i < E_ALLOC_LIMIT) {
        msleep(0.01);
        result = realloc(ptr, size);
    }
    if (result == NULL) memory_error(__FILE, __LINE);

    return result;
}

#define e_malloc(size)       __e_malloc(size, __FILE__, __LINE__)
#define e_realloc(ptr, size) __e_realloc(ptr, size, __FILE__, __LINE__)

void anykey() {
    fflush(stdin);
    printf("\n");
    print(BLU "Press any key to continue..." reset);
    printf("%s", INDENT);
    system("/bin/stty raw");

READ_CHARACTER:;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start); // get start time

    if (getchar() < 10) {
        // exit if user abort requested
        system("/bin/stty cooked");
        END();
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end); // get end time
    uint64_t delta_us
        = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    if (delta_us < 1000) goto READ_CHARACTER; // character already existed in the buffer, read again

    system("/bin/stty cooked");
    printf(BACK512);
}

#endif