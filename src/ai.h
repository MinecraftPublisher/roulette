#ifndef __ROULETTE_HEADER_AI_H
#define __ROULETTE_HEADER_AI_H

#include "helpers.h"
#include "styles.h"

#include <stdio.h>
#include <stdlib.h>

#define false 0
#define true  1

bool turn;

int chambersSinceLastRoll = 0;
int remainder_rounds;

void TURN();
void END();

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

    if (ai_choice != CHECK_CHAMBER) next_chamber = NO_IDEA;
    msleep(2.5);
}

#endif