#ifndef __ROULETTE_DEFINES_H
#define __ROULETTE_DEFINES_H

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

#endif