/**
 * @file state.c
 * @brief Game state management for Rush 2049 N64
 *
 * Handles state transitions between:
 * - ATTRACT (demo/title)
 * - TRKSEL (track select)
 * - CARSEL (car select)
 * - PREPLAY (pre-race setup)
 * - COUNTDOWN
 * - PLAYGAME (active racing)
 * - ENDGAME (race end)
 * - GAMEOVER
 * - HISCORE
 *
 * Key functions:
 * - func_800DB81C (RaceStateMachine_Update) - 2148 bytes
 * - func_800FBF88 (state_transition) - conditional transitions
 */

#include "types.h"

/* External declarations */
extern u8 gstate;               /* 0x801146EC */
extern u8 gstate_prev;
extern u32 frame_counter;

/* Game state flags (N64 uses bitmask, not enum) */
#define GSTATE_INIT     0x40000
#define GSTATE_SETUP    0x80000
#define GSTATE_MENU     0x100000
#define GSTATE_CAMERA   0x200000
#define GSTATE_READY    0x400000
#define GSTATE_PLAY     0x800000
#define GSTATE_CARS     0x1000000
#define GSTATE_FINISH   0x2000000

/**
 * Race state machine update
 * Address: 0x800DB81C
 * Size: 2148 bytes
 *
 * Controls race progression, checkpoint handling,
 * race finalization.
 */
void func_800DB81C(void);  /* TODO: Decompile */

/**
 * Conditional state transition
 * Address: 0x800FBF88
 *
 * Handles transitions between game states.
 */
void func_800FBF88(void);  /* TODO: Decompile */

/**
 * State change notification
 *
 * Called when gstate changes.
 * Performs cleanup/setup for state transitions.
 */
void state_change_notify(void);  /* TODO: Find address and decompile */
