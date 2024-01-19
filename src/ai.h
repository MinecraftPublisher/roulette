#ifndef __ROULETTE_HEADER_AI_H
#define __ROULETTE_HEADER_AI_H

#include "defines.h"
#include "helpers.h"
#include "report.h"
#include "styles.h"
#include <stdio.h>
#include <stdlib.h>

void AI() {
    static enum NEXT_CHAMBER next_chamber = NO_IDEA;

    static bool         ai_init = false;
    static int          ai_size = 255;
    static enum ACTION *ai_choice_history;
    static int          ai_pointer;
    static enum ACTION  ai_choice = -1;

    if (!ai_init) {
        ai_choice_history = e_malloc(sizeof(enum ACTION) * ai_size);
        ai_init           = true;
        ai_pointer        = 0;
    }

    if (ai_pointer == ai_size - 1) {
        ai_size += 255;
        ai_choice_history = e_realloc(ai_choice_history, sizeof(enum ACTION) * ai_size);
    }

    msleep(2);

    if (ai_choice != CHECK_CHAMBER) next_chamber = NO_IDEA;

    // TODO: Implement better AI choice algorithm
    int CAN_CHEAT = rand() % 100 < 5;
    ai_choice     = revolver[ revolver_pointer ] ? SHOOT_U : SHOOT_A;

    // If cheating isn't allowed and the AI doesn't know what the next chamber is
    if (!CAN_CHEAT && next_chamber == NO_IDEA) {
        // Calculate the exact probability of the next chamber containing a bullet
        float probability = (float) live_rounds / (float) round_count;

        // Decide to shoot the user if the probability is high enough
        if (probability >= 0.5f) {
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

    ai_choice_history[ ai_pointer ] = ai_choice;
    ai_pointer++;

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
            remainder_rounds      = round_count - live_rounds;
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

    msleep(2.5);
}

#endif