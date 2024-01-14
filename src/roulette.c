#include "argv.h"
#include "helpers.h"
#include "report.h"
#include "styles.h"
#include <stdarg.h>
#include <stdio.h>

byte *revolver;

int  revolver_pointer;
char choice = 0;

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
        remove("output.roulette");
        FILE *f = fopen("output.roulette", "w+");

        string report = generate();
        if (!no_log) fprintf(stderr, "%s\n", report);
        if (!no_report) fprintf(f, "%s\n", report);
    }

    exit(0);
}

int chambersSinceLastRoll = 0;
int remainder_rounds;

void TURN();

enum NEXT_CHAMBER { BLANK_ROUND, LIVE_ROUND, NO_IDEA };

void AI() {
    static enum NEXT_CHAMBER next_chamber = NO_IDEA;

    static bool         aiinit = false;
    static int          aisize = 255;
    static enum ACTION *aichoice;
    static int          aiptr;

    if (!aiinit) {
        aichoice = malloc(sizeof(enum ACTION) * aisize);
        aiinit   = true;
        aiptr    = 0;
    }

    if (aiptr == aisize - 1) {
        aisize += 255;
        aichoice = realloc(aichoice, sizeof(enum ACTION) * aisize);
    }

    msleep(2);

    // TODO: Implement better AI choice algorithm
    int         CAN_CHEAT = rand() % 100 < 10;
    enum ACTION ai_choice = revolver[ revolver_pointer ] ? SHOOT_U : SHOOT_A;

    if (!CAN_CHEAT && next_chamber == NO_IDEA) {
        // Calculate the exact probability of the next chamber containing a bullet
        float probability = 1.0f / (float) (remainder_rounds + 1);

        // Decide to shoot the user if the probability is low enough
        if (probability <= 0.2f) {
            ai_choice = SHOOT_U; // shoot the user
        } else {
            // Risk assessment based on the game state
            if (remainder_rounds > round_count / 2) {
                // More rounds left, can take a risk
                ai_choice
                    = (rand() % 100 < 40) ? ROLL : SHOOT_A; // roll the chamber or shoot itself
            } else {
                // Fewer rounds left, be cautious
                ai_choice
                    = (rand() % 100 < 70) ? SHOOT_U : ROLL; // shoot the user or roll the chamber
            }
        }
    }

    aichoice[ aiptr ] = ai_choice;
    aiptr++;

    switch (ai_choice) {
        case SHOOT_A:
            chambersSinceLastRoll++;
            remainder_rounds--;

            report(aturn, SHOOT_A);

            print("Your enemy decides to shoot themselves.");
            printf(RED "%sYour enemy points the gun at their own head.", INDENT);
            step;

            back;
            printf(MAG "%sYour enemy pulls the trigger.", INDENT);
            step;
            msleep(1);

            back;
            if (revolver[ revolver_pointer ]) {
                printf(RED "%sBANG!", INDENT);
                printf(GRN " Your enemy died. You win.\n");
                END();
            } else {
                print(RED "Your enemy did not die. The chamber moves to the next round.");
                next();
                turn = aturn;
            }

            break;
        case SHOOT_U:
            chambersSinceLastRoll++;
            remainder_rounds--;

            report(aturn, SHOOT_U);

            print("Your enemy decides to shoot you.");
            printf(RED "%sYour enemy points the gun at you.", INDENT);
            step;

            back;
            printf(MAG "%sYour enemy pulls the trigger.", INDENT);
            step;
            msleep(1);

            back;
            if (revolver[ revolver_pointer ]) {
                printf(RED "%sBANG!", INDENT);
                printf(" You died.\n");
                END();
            } else {
                print(GRN "You did not die. The chamber moves to the next round.");
                next();
                turn = uturn;
            }

            break;
        case ROLL:
            chambersSinceLastRoll = 0;
            remainder_rounds      = round_count - 1;
            report(aturn, ROLL);

            print(MAG "Your enemy decided to roll the chamber.");
            revolver_pointer = point();
            msleep(1.5);
            turn = uturn;

            break;
        case CHECK_CHAMBER:
            report(aturn, CHECK_CHAMBER);
            
            print(BLU "Your enemy decides to check the chamber.");
            print(RED "\"VERY INTERESTING.\"");
            next_chamber = revolver[ revolver_pointer ];

            turn = uturn;
            break;
    }

    if (choice != CHECK_CHAMBER) next_chamber = NO_IDEA;
    msleep(2.5);
}

void USER() {
    print(reset);
    print("1. %sShoot yourself%s", RED, reset);
    print("2. %sShoot your enemy%s", MAG, reset);
    print("3. %sRoll the chamber%s", BLU, reset);
    print("4. %sCheck the chamber%s", BLU, reset);
    print("d. Exit");
    printf("%s%s:", INDENT, GRN);

    system("/bin/stty raw");
    choice = getchar();
    system("/bin/stty cooked");
    if (choice == 'd' || choice == 'D') {
        clear;
        gun;
        print(CYN "-- ROULETTE --%s", reset);
        END();
    }

    clear;
    gun;
    print("-- ROULETTE --%s", reset);

    enum ACTION chc = choice - '1';

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
    print("Bringing out the revoler...");
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
