/**
 * @file ai.c
 * @brief AI/Drone car logic for Rush 2049 N64
 *
 * Based on arcade game/drones.c (~24K lines)
 * Handles computer-controlled opponent behavior
 *
 * Arcade key functions:
 * - DoDrones() - main AI update loop
 * - InitDrones() - initialize AI state
 * - UpdateDronePhysics() - AI car physics
 * - ChoosePath() - path selection at branches
 */

#include "types.h"

/* External declarations */
extern u8 gstate;
extern u32 frame_counter;

/* AI difficulty levels */
#define AI_EASY     0
#define AI_MEDIUM   1
#define AI_HARD     2
#define AI_INSANE   3

/**
 * Main AI update - called per frame for each AI car
 * Updates steering, throttle, and path following
 */
void ai_update(void);  /* TODO: Decompile - find address */

/**
 * Initialize AI state for a race
 * Sets up path data, difficulty, rubber-banding
 */
void ai_init(void);  /* TODO: Decompile - find address */

/**
 * AI path selection at track branches
 * Uses maxpath.c data structures
 */
void ai_choose_path(void);  /* TODO: Decompile - find address */
