#include "argv.h"
#include "helpers.h"
#include "report.h"
#include "styles.h"
#include <stdarg.h>
#include <stdio.h>

byte *revolver;

int  pointer;
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
    int CAN_CHEAT = rand() % 100 < 10;
    int AI_CHOICE = revolver[ pointer ] ? 33 : 15;

    if (!CAN_CHEAT) {
        // Calculate the exact probability of the next chamber containing a bullet
        float probability = 1.0f / (float) (remainder_rounds + 1);

        // Decide to shoot the user if the probability is low enough
        if (probability <= 0.2f) {
            AI_CHOICE = 90; // shoot the user
        } else {
            // Risk assessment based on the game state
            if (remainder_rounds > round_count / 2) {
                // More rounds left, can take a risk
                AI_CHOICE = (rand() % 100 < 30) ? 95 : 30; // roll the chamber or shoot itself
            } else {
                // Fewer rounds left, be cautious
                AI_CHOICE = (rand() % 100 < 70) ? 90 : 95; // shoot the user or roll the chamber
            }
        }
    }

    aichoice[ aiptr ] = AI_CHOICE;
    aiptr++;

    if (AI_CHOICE < 32) { // point at self
        chambersSinceLastRoll++;
        remainder_rounds--;

        report(false, SHOOT_A);

        print("Your enemy decides to shoot themselves.");
        printf(RED "%sYour enemy points the gun at their own head.", INDENT);
        step;

        back;
        printf(MAG "%sYour enemy pulls the trigger.", INDENT);
        step;
        msleep(1);

        back;
        if (revolver[ pointer ]) {
            printf(RED "%sBANG!", INDENT);
            printf(GRN " Your enemy died. You win.\n");
            END();
        } else {
            print(RED "Your enemy has lived. The chamber moves to the next round.");
            next();
            turn = aturn;
        }
    } else if (AI_CHOICE < 94) { // point at user
        chambersSinceLastRoll++;
        remainder_rounds--;

        report(false, SHOOT_U);

        print("Your enemy decides to shoot you.");
        printf(RED "%sYour enemy points the gun at you.", INDENT);
        step;

        back;
        printf(MAG "%sYour enemy pulls the trigger.", INDENT);
        step;
        msleep(1);

        back;
        if (revolver[ pointer ]) {
            printf(RED "%sBANG!", INDENT);
            printf(" You died.\n");
            END();
        } else {
            print(GRN "You have lived. The chamber moves to the next round.");
            next();
            turn = uturn;
        }
    } else { // roll chamber
        chambersSinceLastRoll = 0;
        remainder_rounds      = round_count - 1;
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
    printf("%s%s:", INDENT, GRN);

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
            remainder_rounds--;

            report(true, SHOOT_U);

            printf(RED "%sYou put the gun on your head.", INDENT);
            step;

            back;
            printf(MAG "%sYou pull the trigger.", INDENT);
            step;
            msleep(1);

            back;
            if (revolver[ pointer ]) {
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
        case '2':
            chambersSinceLastRoll++;
            remainder_rounds--;

            report(true, SHOOT_A);

            printf(RED "%sYou point the gun at your enemy.", INDENT);
            step;

            back;
            printf(MAG "%sYou pull the trigger.", INDENT);
            step;
            msleep(1);

            back;
            if (revolver[ pointer ]) {
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
        case '3':
            chambersSinceLastRoll = 0;
            remainder_rounds      = round_count - 1;

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
    print("There is (1) live round and (%i) blanks in the chamber.", remainder_rounds);

    if (pointer >= round_count) pointer = 0;

    if (turn) USER();
    else
        AI();
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
    msleep(1.5);

    print("Rolling the chamber...");
    pointer = point();
    msleep(1.5);

    turn = random() % 2;

    while (true) TURN();

    return 0;
}
