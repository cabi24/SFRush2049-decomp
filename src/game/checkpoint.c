/**
 * @file checkpoint.c
 * @brief Checkpoint and lap counting for Rush 2049 N64
 *
 * Based on arcade game/checkpoint.c
 * Handles checkpoint detection, lap counting, race positions
 *
 * Key functions:
 * - func_800C00E0 - checkpoint collision detection
 * - func_800C02A0 - checkpoint state update
 * - func_800C04CC - lap completion handler
 * - func_800C085C - checkpoint initialization
 * - func_800C0AC0 - race position calculation
 */

#include "types.h"

/* External declarations */
extern u8 gstate;
extern u32 frame_counter;

/* Game state flags */
#define GSTATE_INIT     0x40000
#define GSTATE_SETUP    0x80000
#define GSTATE_MENU     0x100000
#define GSTATE_CAMERA   0x200000
#define GSTATE_READY    0x400000
#define GSTATE_PLAY     0x800000
#define GSTATE_CARS     0x1000000
#define GSTATE_FINISH   0x2000000

/* Global data addresses (from symbol analysis) */
#define CAR_ARRAY_ADDR      0x80152818
#define GAME_STRUCT_ADDR    0x801461D0
#define GSTATE_ADDR         0x801146EC

/**
 * Checkpoint collision detection
 * Address: 0x800C00E0
 *
 * Checks if a car has passed through a checkpoint trigger.
 * Updates car's checkpoint counter and validates sequence.
 *
 * Arcade equivalent: checkpoint.c:CheckCPs()
 */
void func_800C00E0(void);  /* TODO: Decompile */

/**
 * Checkpoint state update
 * Address: 0x800C02A0
 *
 * Updates checkpoint state for a specific car.
 * Called after collision detection confirms checkpoint hit.
 */
void func_800C02A0(s32 arg0, s32 arg1);  /* TODO: Decompile */

/**
 * Lap completion handler
 * Address: 0x800C04CC
 *
 * Called when a car completes a lap.
 * Updates lap counter, records lap time, checks for race finish.
 *
 * Arcade equivalent: checkpoint.c:LapComplete()
 */
void func_800C04CC(u32 car_index);  /* TODO: Decompile */

/**
 * Checkpoint initialization
 * Address: 0x800C085C
 *
 * Initializes checkpoint system for a race.
 * Sets up checkpoint triggers and resets counters.
 *
 * Arcade equivalent: checkpoint.c:InitCPS()
 */
void func_800C085C(u16 arg0);  /* TODO: Decompile */

/**
 * Race position calculation
 * Address: 0x800C0AC0
 *
 * Calculates current race positions based on:
 * - Lap count
 * - Checkpoint progress
 * - Distance to next checkpoint
 *
 * Arcade equivalent: checkpoint.c:CalcPositions()
 */
void func_800C0AC0(u16 arg0);  /* TODO: Decompile */
