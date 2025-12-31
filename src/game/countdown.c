/**
 * countdown.c - Countdown timer state handler
 * Decompiled from 0x800FBC30 with Ollama assistance
 */

#include "types.h"

/* Game state values (from arcade game.h) */
#define COUNTDOWN 5
#define PREPLAY2  6

/* External declarations */
extern s8 gstate;  /* 0x801146EC */
extern s32 countdown_check_complete(void);  /* 0x800FB908 - Check if countdown complete */

/**
 * CountDown - Handle countdown state (3, 2, 1, GO!)
 * Transitions to PREPLAY2 when countdown finishes
 */
void CountDown(void) {
    if (gstate == COUNTDOWN) {
        if (countdown_check_complete() != 0) {
            gstate = PREPLAY2;
        }
    }
}
