#ifndef __ROULETTE_HEADER_REPORT_H
#define __ROULETTE_HEADER_REPORT_H

#include "defines.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool no_report = false;
bool no_log    = false;
bool no_logo   = false;
bool no_center = false;

int TIME_MODE   = 4;
int round_count = 6;

string concatC(string left, string right) {
    int left_s  = strlen(left);
    int right_s = strlen(right);
    int size    = left_s + right_s;

    string new = malloc(sizeof(char) * size);
    for (int i = 0; i < left_s; i++) new[ i ] = left[ i ];
    for (int i = left_s; i < size; i++) new[ i ] = right[ i ];

    return new;
}

string concat(int count, ...) {
    va_list args;
    va_start(args, count);

    int total_length = 0;
    for (int i = 0; i < count; i++) {
        char *str = va_arg(args, char *);
        total_length += strlen(str);
    }
    va_end(args);

    string result = malloc(total_length + 1);
    if (result == NULL) { return NULL; }

    va_start(args, count);
    char *temp;
    int   pos = 0;
    for (int i = 0; i < count; i++) {
        temp = va_arg(args, char *);
        strcpy(result + pos, temp);
        pos += strlen(temp);
    }
    va_end(args);

    result[ total_length ] = '\0';

    return result;
}

int revolver_pointer;

void report(bool turn, enum ACTION action) {
    if (!report_initialized) {
        report_size = 255;
        report_logs = malloc(sizeof(struct TURN) * report_size);
        report_initialized = true;
        report_pointer  = 0;
    }

    if (report_pointer == report_size - 1) {
        report_size += 255;
        report_logs = realloc(report_logs, sizeof(struct TURN) * report_size);
    }

    struct TURN *_turn = malloc(sizeof(struct TURN));
    _turn->turn        = turn;
    _turn->action      = action;
    _turn->pointer     = revolver_pointer;
    _turn->revolver    = malloc(sizeof(byte) * round_count);
    for (int i = 0; i < round_count; i++) _turn->revolver[ i ] = revolver[ i ];

    report_logs[ report_pointer ] = *_turn;
    report_pointer++;
}

string LINE(struct TURN t) {
    string player = t.turn ? "user" : "enemy";
    string action = t.action == SHOOT_U ? (t.turn == uturn ? "shoot themselves" : "shoot the user")
                    : t.action == SHOOT_A ? (t.turn ? "shoot the enemy" : "shoot themselves")
                    : t.action == ROLL    ? "roll the chamber"
                                          : concat(
                                           3,
                                           "check the chamber, It was a ",
                                           t.revolver[ t.pointer ] ? "live" : "blank",
                                           " round");

    string revolver_out = malloc(sizeof(char) * round_count);
    for (int i = 0; i < round_count; i++) revolver_out[ i ] = t.revolver[ i ] ? '@' : 'O';
    string revolver_point = malloc(sizeof(char) * round_count);
    for (int i = 0; i < round_count; i++) revolver_point[ i ] = i == t.pointer ? '^' : ' ';
    string revolver_graph = concat(4, revolver_out, "\n", revolver_point, "\n");

    string consequence = t.revolver[ t.pointer ] ? "BANG! The round was live.\n\n"
                                                 : "*click*. The round was blank.\n\n";

    return concat(
        9,
        revolver_graph,
        "It is the ",
        player,
        "'s turn. The ",
        player,
        " decides to ",
        action,
        ". ",
        t.action > 1 ? "\n\n" : consequence);
}

string itoa(int i) {
    int    n = snprintf(NULL, 0, "%d", i) + 1;
    string s = malloc(n);

    if (s != NULL) snprintf(s, n, "%d", i);
    return s;
}

string generate() {
    time_t     timer;
    char       buffer[ 100 ];
    struct tm *tm_info;

    timer   = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    // don't hate me for adding a URL. you can always build without it.
    string output = concat(
        7,
        "-- ROULETTE --\nURL: https://github.com/MinecraftPublisher/roulette\nPlayed at ",
        buffer,
        "\nRounds: ",
        itoa(round_count),
        " (1 live round, ",
        itoa(round_count - 1),
        " blank rounds)\n\n");
    for (int i = 0; i < report_pointer; i++) { output = concat(2, output, LINE(report_logs[ i ])); }

    return output;
}

#endif