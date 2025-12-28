/**
 * @file render.c
 * @brief 3D rendering for Rush 2049 N64
 *
 * Based on arcade game/visuals.c
 * Handles model rendering, display list construction
 *
 * Key functions:
 * - func_80099BFC (render_object) - 10KB, main model render
 * - func_80087A08 (render_large) - 10KB, large render function
 * - func_800B65B4 (render_helper) - rendering utility
 *
 * Uses F3DEX2 microcode:
 * - gSPMatrix, gSPVertex, gSP2Triangles
 * - gSPDisplayList for nested display lists
 */

#include "types.h"

/* External declarations */
extern u8 gstate;
extern u32 frame_counter;

/* Display list pointer - typically passed around */
/* Gfx *glistp; */

/**
 * Main 3D object rendering
 * Address: 0x80099BFC
 * Size: 10236 bytes
 *
 * Renders car bodies, wheels, track objects.
 * Uses G_DL commands for display lists.
 */
void render_objects_3d(void);  /* TODO: Decompile */

/**
 * Large rendering function
 * Address: 0x80087A08
 * Size: 10048 bytes
 *
 * Possibly scene/track rendering.
 */
void render_scene_large(void);  /* TODO: Decompile */

/**
 * Rendering utility helper
 * Address: 0x800B65B4
 * Called 65 times
 *
 * Common rendering operations.
 */
void render_util_common(void);  /* TODO: Decompile */
