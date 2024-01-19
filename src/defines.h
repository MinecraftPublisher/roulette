#ifndef __ROULETTE_DEFINES_H
#define __ROULETTE_DEFINES_H

#include <stdio.h>
#include <stdlib.h>

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
int   round_count;

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

void memory_error() {
    printf("Memory error occured.\n"
        "This usually means the system didn't give me memory,\n"
        "Or the system doesn't even have enough memory to give it to me.\n"
        "This usually cannot be recovered from, And I would have to close the app.\n"
        "Sorry about that!\n");
    exit(EXIT_FAILURE);
}

#define E_ALLOC_LIMIT 50

void *e_malloc(size_t size) {
    void *result = malloc(size);
    int i = 0;
    while(result == NULL && i < E_ALLOC_LIMIT) {
        msleep(0.01);
        result = malloc(size);
    }
    if(result == NULL) memory_error();

    return result;
}

void *e_realloc(void *ptr, size_t size) {
    void *result = realloc(ptr, size);
    int i = 0;
    while(result == NULL && i < E_ALLOC_LIMIT) {
        msleep(0.01);
        result = realloc(ptr, size);
    }
    if(result == NULL) memory_error();
    
    return result;
}

#endif