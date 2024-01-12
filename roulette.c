#include "helpers.h"
#include "styles.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef char byte;
typedef byte bool;
#define false 0
#define true  1

byte *revolver;

int  pointer;
char choice = 0;

#define uturn true
#define aturn false

bool turn;

enum ACTION { SHOOT_U, SHOOT_A, ROLL };

struct TURN {
    bool        turn;
    enum ACTION action;
    byte       *revolver;
    int         pointer;
};

int          size;
int          ptr;
struct TURN *logs;

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

void report(bool turn, enum ACTION action) {
    static bool init = false;

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
    _turn->pointer     = ptr;
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

void END() {
    usleep(10000);
    printf(RED INDENT "GOOD. ");
    fflush(stdout);
    msleep(1.5);
    printf("LUCK.\n" reset);
    msleep(1.5);
    print();

    string report = generate();
    fprintf(stderr, "%s\n", report);

    exit(0);
}

int chambersSinceLastRoll = 0;

void AI() {
    static bool  aiinit = false;
    static int   aisize = 255;
    static byte *aichoice;
    static int   aiptr;

    if (!aiinit) {
        aichoice = malloc(sizeof(int) * aisize);
        aiinit   = true;
        aiptr    = 0;
    }

    if (aiptr == aisize - 1) {
        aisize += 255;
        aichoice = realloc(aichoice, sizeof(int) * aisize);
    }

    msleep(2);

    // TODO: Implement better AI choice algorithm
    int CAN_CHEAT = rand() % 100 < 5;
    int AI_CHOICE = revolver[ pointer ] ? 33 : 15;

    if (!CAN_CHEAT) {
        float probability = 1.0f / (float) (round_count - chambersSinceLastRoll);
        if (probability > 0.25f) {
            AI_CHOICE = 90; // shoot the user
        } else {
            int SHOULD_ROLL = rand() % 100;
            if (SHOULD_ROLL < 10) { // roll the chamber
                AI_CHOICE = 95;
            } else if (SHOULD_ROLL < 40) { // shoot itself
                AI_CHOICE = 30;
            } else { // shoot the user
                AI_CHOICE = 90;
            }
        }
    }

    aichoice[ aiptr ] = AI_CHOICE;
    aiptr++;

    if (AI_CHOICE < 32) { // point at self
        chambersSinceLastRoll++;
        report(false, SHOOT_A);

        print("Your enemy decides to shoot themselves.");
        printf(INDENT RED "Your enemy points the gun at their own head.");
        step;

        back;
        printf(MAG INDENT "Your enemy pulls the trigger.");
        step;
        msleep(1);

        back;
        if (revolver[ pointer ]) {
            printf(RED INDENT "BANG!");
            printf(GRN " Your enemy died. You win.\n");
            END();
        } else {
            print(RED "Your enemy has lived. The chamber moves to the next round.");
            next();
            turn = aturn;
        }
    } else if (AI_CHOICE < 94) { // point at user
        chambersSinceLastRoll++;
        report(false, SHOOT_U);

        print("Your enemy decides to shoot you.");
        printf(INDENT RED "Your enemy points the gun at you.");
        step;

        back;
        printf(MAG INDENT "Your enemy pulls the trigger.");
        step;
        msleep(1);

        back;
        if (revolver[ pointer ]) {
            printf(RED INDENT "BANG!");
            printf(" You died.\n");
            END();
        } else {
            print(GRN "You have lived. The chamber moves to the next round.");
            next();
            turn = uturn;
        }
    } else { // roll chamber
        chambersSinceLastRoll = 0;
        report(false, ROLL);

        print(MAG "Your enemy decided to roll the chamber.");
        pointer = point();
        msleep(1.5);
        turn = uturn;
    }

    msleep(2);
}

void USER() {
    print(reset);
    print("1. %sShoot yourself%s", RED, reset);
    print("2. %sShoot your enemy%s", MAG, reset);
    print("3. %sRoll the chamber%s", BLU, reset);
    print("d. Exit");
    printf("                              %s:", GRN);

    system("/bin/stty raw");
    choice = getchar();
    system("/bin/stty cooked");
    if (choice == 'd' || choice == 'D') {
        clear;
        gun;
        print("%s-- ROULETTE --%s", CYN, reset);
        END();
    }

    clear;
    gun;
    print("%s-- ROULETTE --%s", CYN, reset);
    switch (choice) {
        case '1':
            chambersSinceLastRoll++;
            report(true, SHOOT_U);

            printf(INDENT RED "You put the gun on your head.");
            step;

            back;
            printf(MAG INDENT "You pull the trigger.");
            step;
            msleep(1);

            back;
            if (revolver[ pointer ]) {
                printf(RED INDENT "BANG!");
                printf(" You died.\n");
                END();
            } else {
                print(GRN "You have lived. The chamber moves to the next round.");
                next();
                turn = uturn;
            }

            msleep(3);
            break;
        case '2':
            chambersSinceLastRoll++;
            report(true, SHOOT_A);

            printf(INDENT RED "You point the gun at your enemy.");
            step;

            back;
            printf(MAG INDENT "You pull the trigger.");
            step;
            msleep(1);

            back;
            if (revolver[ pointer ]) {
                printf(RED INDENT "BANG!");
                printf(GRN " Your enemy died. You win.\n");
                END();
            } else {
                print(RED "Your enemy has lived. The chamber moves to the next round.");
                next();
                turn = aturn;
            }

            msleep(3);
            break;
        case '3':
            chambersSinceLastRoll = 0;
            report(true, ROLL);

            print(MAG "Rolling the chamber...");
            pointer = point();
            msleep(1.5);
            turn = aturn;

            break;
    }
}

void TURN() {
    clear;
    if (turn) printf(GRN);
    else
        printf(RED);
    gun;
    print("%s-- ROULETTE --%s", CYN, reset);
    print("There is (1) bullet in the chamber.");

    if (pointer >= round_count) pointer = 0;

    if (turn) USER();
    else
        AI();
}

void mode(char **argv) {
    if (!strcmp(argv[ 2 ], "fast")) {
        TIME_MODE = 1;
    } else if (!strcmp(argv[ 2 ], "normal")) {
        TIME_MODE = 2;
    } else if (!strcmp(argv[ 2 ], "slow")) {
        TIME_MODE = 4;
    } else if (!strcmp(argv[ 2 ], "help") || !strcmp(argv[ 1 ], "?") || !strcmp(argv[ 1 ], "-h")) {
        printf("Russian roulette\n");
        printf("Play russian roulette in your CLI.\n");
        printf("Options:\n");
        printf("    %s  <rounds> <pace>\n", argv[ 0 ]);
        printf("                 fast    - Play on fast mode. Half the stops.\n");
        printf("                 normal  - Same as no arguments.\n");
        printf("                 slow    - More drama. Double the stops.\n");
        printf("        3 - 24           - Determine the number of rounds.\n");
        ;

        exit(0);
    }
}

void COUNT_RC(char **argv) {
    char *rounds = argv[ 1 ];
    round_count  = atoi(rounds);
    if (round_count < 3 || round_count > 24) {
        printf("Too many/low number of rounds. Rounds should be between 3 and 24.\n");
    }
}

int main(int argc, char **argv) {
    if (argc >= 3) {
        mode(argv);
        COUNT_RC(argv);
    } else if (argc == 2) {
        COUNT_RC(argv);
    } else {
        round_count = 6;
    }

    srand(time(NULL));

    clear;
    printf(MAG);
    gun;
    printf(reset);

    print("Playing with %i rounds.", round_count);
    print("Bringing out the revoler...");
    revolver = malloc(sizeof(bool) * round_count);
    msleep(1.5);

    printf(INDENT "Filling the chamber... (");
    msleep(0.5);
    int REV_C = point();
    for (int i = 0; i < round_count; i++) {
        if (i == REV_C) {
            printf(RED "@");
            fflush(stdout);
            revolver[ i ] = true;
            msleep(0.35);
        } else {
            printf(GRN "O");
            fflush(stdout);
            revolver[ i ] = false;
            msleep(0.2);
        }
    }
    printf(reset ")\n");
    msleep(1.5);

    print("Rolling the chamber...");
    pointer = point();
    msleep(1.5);

    turn = random() % 2;

    while (true) TURN();

    return 0;
}