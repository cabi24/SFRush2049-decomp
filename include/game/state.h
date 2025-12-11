/**
 * state.h - Game state machine definitions for Rush 2049 N64
 *
 * Based on arcade game/gstate.h
 * Defines game states and transition functions
 */

#ifndef STATE_H
#define STATE_H

#include "types.h"

/**
 * Game States (Arcade-style enum)
 * Based on arcade gstate.h
 *
 * Note: N64 may use bitmask-style states internally,
 * but these map to the arcade state machine
 */
typedef enum GState {
    GS_ATTRACT = 0,     /* Demo/title screen */
    GS_TRKSEL,          /* Track selection */
    GS_CARSEL,          /* Car selection */
    GS_PREPLAY,         /* Pre-race initialization */
    GS_COUNTDOWN,       /* Race countdown (3-2-1-GO) */
    GS_PREPLAY2,        /* Final pre-race setup */
    GS_PLAYGAME,        /* Active racing */
    GS_ENDGAME,         /* Race finished, coasting */
    GS_GAMEOVER,        /* Game over screen */
    GS_HISCORE,         /* High score entry */
    GS_NUM_STATES       /* Number of states */
} GState;

/**
 * N64 State Flags (bitmask style)
 * These are the actual flags used by the N64 game loop
 */
#define GSTATE_INIT     0x40000     /* Initialization phase */
#define GSTATE_SETUP    0x80000     /* Setup phase */
#define GSTATE_MENU     0x100000    /* In menu */
#define GSTATE_CAMERA   0x200000    /* Camera active */
#define GSTATE_READY    0x400000    /* Ready to play (countdown) */
#define GSTATE_PLAY     0x800000    /* Gameplay active */
#define GSTATE_CARS     0x1000000   /* Cars active */
#define GSTATE_FINISH   0x2000000   /* Race finished */

/* State flag combinations for common states */
#define GSTATE_ATTRACT_FLAGS    (GSTATE_INIT)
#define GSTATE_MENU_FLAGS       (GSTATE_MENU | GSTATE_CAMERA)
#define GSTATE_RACE_FLAGS       (GSTATE_PLAY | GSTATE_CARS | GSTATE_CAMERA)
#define GSTATE_FINISH_FLAGS     (GSTATE_FINISH | GSTATE_CARS)

/* External state variables */
extern u8 gstate;           /* 0x801146EC - current state byte/enum */
extern u8 gstate_prev;      /* Previous game state */
extern u32 gstate_flags;    /* State flags bitmask */

/* State transition functions */
void state_init(void);                  /* Initialize state machine */
void state_set(GState new_state);       /* Set new game state */
GState state_get(void);                 /* Get current state */
u32 state_get_flags(void);              /* Get current state flags */

/* State handler prototypes */
void state_attract(void);               /* Handle attract mode */
void state_track_select(void);          /* Handle track selection */
void state_car_select(void);            /* Handle car selection */
void state_preplay(void);               /* Handle pre-race setup */
void state_countdown(void);             /* Handle countdown */
void state_playgame(void);              /* Handle active racing */
void state_endgame(void);               /* Handle race end */
void state_gameover(void);              /* Handle game over */
void state_hiscore(void);               /* Handle high score entry */

/* Countdown functions */
void countdown_init(s32 start_value);   /* Start countdown from value */
s32 countdown_update(void);             /* Update and get current value */
s32 countdown_finished(void);           /* Check if countdown done */

/* Timer functions */
void timer_set_countdown(u32 time);     /* Set countdown timer */
u32 timer_get_countdown(void);          /* Get remaining time */
u32 timer_get_elapsed(void);            /* Get elapsed game time */
s32 timer_timeout(void);                /* Check if time ran out */

/* Utility functions */
void state_enter(GState state);         /* Called on state entry */
void state_exit(GState state);          /* Called on state exit */
const char* state_get_name(GState state); /* Get state name for debug */

#endif /* STATE_H */
