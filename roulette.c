#include "styles.h"
#include "helpers.h"

#include <stdio.h>
#include <string.h>

typedef char byte;
typedef byte bool;
#define false 0
#define true  1

bool *revolver;

int  pointer;
char choice = 0;

#define uturn true
#define aturn false

bool turn;

void END() {
    usleep(10000);
    printf(RED INDENT "GOOD. ");
    fflush(stdout);
    msleep(1.5);
    printf("LUCK.\n" reset);
    msleep(1.5);
    print();

    exit(0);
}

void AI() {
    msleep(2);

    int AI_CHOICE = rand() % 100;
    if (AI_CHOICE < 32) { // point at self
        print("Your enemy wants to shoot themselves.");
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
        print("Your enemy wants to shoot you.");
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
        case 0: print("There is (1) bullet in the chamber."); break;

        case '1':
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

            break;
        case '2':
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
            print(MAG "Rolling the chamber...");
            pointer = point();
            msleep(1.5);
            turn = aturn;

            break;
    }
}

void TURN() {
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

int main(int argc, char **argv) {
    if (argc == 2) {
        mode(argv);
        char *rounds = argv[ 1 ];
        round_count  = atoi(rounds);
        if (round_count < 3 || round_count > 24) {
            printf("Too many/low number of rounds. Rounds should be between 3 and 24.\n");
        }
    } else if (argc == 3) {
        mode(argv);
        round_count = 6;
    } else {
        round_count = 6;
    }

    srand(time(NULL));

    clear;
    printf(RED);
    gun;
    printf(reset);

    print("Playing with %i rounds.", round_count);
    print("Bringing out the revoler...");
    revolver = malloc(sizeof(bool) * round_count);
    msleep(1.5);

    printf(INDENT "Filling the chamber... (");
    msleep(0.5);
    for (int i = 0; i < round_count; i++) {
        if(i == 0) {
            printf(RED "O");
            fflush(stdout);
            revolver[ i ] = true;
            msleep(0.35);
        }
        else {
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

    while (true) {
        clear;
        gun;
        print("%s-- ROULETTE --%s", CYN, reset);

        TURN();
    }

    return 0;
}