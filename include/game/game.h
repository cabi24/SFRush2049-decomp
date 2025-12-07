/**
 * @file game.h
 * @brief Game state definitions
 *
 * Based on reference/repos/rushtherock/game/game.h
 * Defines the game state machine states and related types.
 */

#ifndef _GAME_H
#define _GAME_H

#include "types.h"

/**
 * Game command function types
 * Used for state machine transitions
 */
typedef enum {
    CMD_INITIALIZE,
    CMD_ALLOCATE,
    CMD_DO_IT,
    CMD_UNDO_IT,
    CMD_DEALLOCATE,
    CMD_CLEANUP,
    NUM_COMMANDS
} CmdFunc;

/**
 * Game state enumeration
 * Defines the main game flow states
 */
typedef enum GState {
    ATTRACT,        /* Attract mode - demo loop */
    TRKSEL,         /* Track selection screen */
    CARSEL,         /* Car selection screen */
    PLAYGAME,       /* Active gameplay */
    ENDGAME,        /* End of game sequence */
    GAMEOVER,       /* Game over screen */
    HISCORE,        /* High score entry */
    PREPLAY,        /* Pre-play setup 1 */
    PREPLAY2,       /* Pre-play setup 2 */
    COUNTDOWN,      /* Race countdown */
    NUM_GAME_STATES
} GState;

/* Entity types */
#define DRONE   1   /* AI-controlled car */
#define HUMAN   2   /* Player-controlled car */

/* Boolean values */
#ifndef true
#define true    1
#endif
#ifndef false
#define false   0
#endif

/* Time constants (assuming 60 fps) */
#define ONE_SEC         60
#define GAMEOVER_TIME   (4 * ONE_SEC)

/* Global game state variable (defined in game code) */
extern GState gstate;

#endif /* _GAME_H */
