#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef char byte;
typedef byte bool;
#define false 0
#define true  1

enum ACTION { SHOOT_U, SHOOT_A, ROLL };

struct TURN {
    bool        turn;
    enum ACTION action;
    byte       *revolver;
    int         pointer;
};

int   round_count;
byte *revolver;

int          size;
int          ptr;
struct TURN *logs;
bool         init;

typedef char *string;

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

int pointer;

void report(bool turn, enum ACTION action) {
    if (!init) {
        size = 255;
        logs = malloc(sizeof(struct TURN) * size);
        init = true;
        ptr  = 0;
    }

    if (ptr == size - 1) {
        size += 255;
        logs = realloc(logs, sizeof(struct TURN) * size);
    }

    struct TURN *_turn = malloc(sizeof(struct TURN));
    _turn->turn        = turn;
    _turn->action      = action;
    _turn->pointer     = pointer;
    _turn->revolver    = malloc(sizeof(byte) * round_count);
    for (int i = 0; i < round_count; i++) _turn->revolver[ i ] = revolver[ i ];

    logs[ ptr ] = *_turn;
    ptr++;
}

string LINE(struct TURN t) {
    string player = t.turn ? "user" : "ai";
    string action = t.action == 0   ? (t.turn ? "shoot themselves" : "shoot the user")
                    : t.action == 1 ? (t.turn ? "shoot the enemy" : "shoot themselves")
                                    : "roll the chamber";

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
        consequence);
}

string generate() {
    string output = LINE(logs[ 0 ]);
    for (int i = 1; i < ptr; i++) { output = concat(2, output, LINE(logs[ i ])); }

    return output;
}