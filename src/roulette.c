#include "argv.h"
#include "helpers.h"
#include "report.h"
#include "styles.h"
#include "ai.h"
#include <stdio.h>

// excuse me for the interesting naming conventions :)
byte *revolver;

int  revolver_pointer;
char user_choice = 0;

#define uturn true
#define aturn false

bool turn;

void END() {
    usleep(10000);
    printf(RED "%sGOOD. ", INDENT);
    fflush(stdout);
    msleep(1.5);
    printf("LUCK.\n" reset);
    msleep(1.5);
    print();

    if (init) {
        string report = generate();
        if (!no_log) fprintf(stderr, "%s\n", report);
        if (!no_report) {
            remove("output.roulette");
            FILE *f = fopen("output.roulette", "w+");

            fprintf(f, "%s\n", report);
            fclose(f);
        }
    }

    exit(0);
}

void USER() {
    print(reset);
    print("1. %sShoot yourself%s", RED, reset);
    print("2. %sShoot your enemy%s", MAG, reset);
    print("3. %sRoll the chamber%s", BLU, reset);
    print("4. %sCheck the chamber%s", BLU, reset);
    print("d. Exit");
    printf("%s%s:", INDENT, GRN);

    system("/bin/stty raw"); // setting terminal output to raw in order to get a single character as input
    
    // repeat until we get a valid character
    _getchar:
    user_choice = getchar();
    while(!(user_choice == 'd' || user_choice == 'D' || (user_choice >= '1' && user_choice <= '4'))) goto _getchar;
    
    system("/bin/stty cooked");
    
    if (user_choice == 'd' || user_choice == 'D') {
        clear;
        gun;
        print(CYN "-- ROULETTE --%s", reset);
        END();
    }

    clear;
    gun;
    print("-- ROULETTE --%s", reset);

    enum ACTION chc = user_choice - '1';

    switch (chc) {
        case SHOOT_U:
            chambersSinceLastRoll++;
            remainder_rounds--;

            report(uturn, SHOOT_U);

            printf(RED "%sYou put the gun on your head.", INDENT);
            step;

            back;
            printf(MAG "%sYou pull the trigger.", INDENT);
            step;
            msleep(1);

            back;
            if (revolver[ revolver_pointer ]) {
                printf(RED "%sBANG!", INDENT);
                printf(" You died.\n");
                END();
            } else {
                print(GRN "You have lived. The chamber moves to the next round.");
                next();
                turn = uturn;
            }

            msleep(3);
            break;
        case SHOOT_A:
            chambersSinceLastRoll++;
            remainder_rounds--;

            report(uturn, SHOOT_A);

            printf(RED "%sYou point the gun at your enemy.", INDENT);
            step;

            back;
            printf(MAG "%sYou pull the trigger.", INDENT);
            step;
            msleep(1);

            back;
            if (revolver[ revolver_pointer ]) {
                printf(RED "%sBANG!", INDENT);
                printf(GRN " Your enemy died. You win. \n");
                END();
            } else {
                print(RED "Your enemy has lived. The chamber moves to the next round.");
                next();
                turn = aturn;
            }

            msleep(3);
            break;
        case ROLL:
            chambersSinceLastRoll = 0;
            remainder_rounds      = round_count - 1;

            report(uturn, ROLL);

            print(MAG "Rolling the chamber...");
            revolver_pointer = point();
            msleep(1.5);
            turn = aturn;

            break;
        case CHECK_CHAMBER:
            report(uturn, CHECK_CHAMBER);

            printf(BLU "%sYou decide to check the chamber.", INDENT);
            step;

            back;
            print("It is a %s round. Good luck.", revolver[ revolver_pointer ] ? "live" : "blank");

            msleep(5);
            turn = aturn;
            break;
        default:
            // if the choice is invalid, simply return to the turn screen. how did the user even get here???
            TURN();
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
    print("There is (1) live round and (%i) blanks in the chamber.", remainder_rounds);

    if (revolver_pointer >= round_count) revolver_pointer = 0;

    if (turn) USER();
    else { AI(); }
}

int main(int argc, string *argv) {
    process_argv(argc, argv);

    remainder_rounds = round_count - 1;

    srand(time(NULL));

    clear;
    printf(MAG);
    gun;
    printf(reset);

    print("Playing with %i rounds.", round_count);
    print("Bringing out the revolver...");
    revolver = malloc(sizeof(bool) * round_count);
    msleep(1.5);

    printf("%sFilling the chamber... (", INDENT);
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
    msleep(2.5);

    print("Rolling the chamber...");
    revolver_pointer = point();
    msleep(1.5);

    turn = random() % 2;

    while (true) TURN();

    return 0;
}
