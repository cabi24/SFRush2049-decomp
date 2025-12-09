/**
 * @file physics.c
 * @brief Car physics for Rush 2049 N64
 *
 * Based on arcade game/cars.c and game/tires.c
 * Handles vehicle dynamics, tire simulation, suspension
 *
 * Key functions:
 * - func_800B0868 (PhysicsObjectList_Update) - per-frame physics
 * - func_800B811C (Effects_UpdateEmitters) - particle/trail FX
 *
 * Arcade equivalents:
 * - Update_MDrive() - main physics update
 * - CheckCrash() - collision response
 * - CalcTireForces() - tire grip model
 */

#include "types.h"

/* External declarations */
extern u8 gstate;
extern u32 frame_counter;

/* Physics constants */
#define GRAVITY         9.8f
#define TIRE_GRIP_DRY   1.0f
#define TIRE_GRIP_WET   0.7f

/**
 * Per-frame physics/vehicle list update
 * Address: 0x800B0868
 *
 * Updates physics for all active cars.
 * Called from main game loop during PLAYGAME state.
 */
void func_800B0868(void);  /* TODO: Decompile */

/**
 * Particle/trail/FX system update
 * Address: 0x800B811C
 *
 * Updates smoke, tire marks, sparks, etc.
 */
void func_800B811C(void);  /* TODO: Decompile */

/**
 * Collision response
 *
 * Handles car-to-car and car-to-world collisions.
 * Applies impulse forces.
 */
void collision_response(void);  /* TODO: Find address and decompile */
