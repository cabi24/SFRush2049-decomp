/**
 * @file camera.c
 * @brief Camera system for Rush 2049 N64
 *
 * Based on arcade game/camera.c
 * Handles chase cam, cockpit view, replay cameras
 *
 * Key functions:
 * - func_800A04C4 (render_scene) - viewport/camera setup, 2720 bytes
 * - func_800A0BAC - camera state update
 * - func_800A0F74 - camera initialization
 * - func_800A0FDC - camera mode switching
 */

#include "types.h"

/* External declarations */
extern u8 gstate;
extern u32 frame_counter;

/* Global data addresses (from symbol analysis) */
#define CAMERA_STATE_ADDR    0x80175A78
#define VIEWPORT_MODE_ADDR   0x80140618

/**
 * render_scene - Setup viewport and camera for rendering
 * Address: 0x800A04C4
 * Size: 2720 bytes
 *
 * Sets up the viewport based on current camera mode.
 * Uses G_SETGEOMETRYMODE RDP commands.
 *
 * Arcade equivalent: camera.c:UpdateCamera() or similar
 */
void func_800A04C4(void);  /* TODO: Decompile */

/**
 * Camera state update
 * Address: 0x800A0BAC
 *
 * Updates camera position based on target car.
 */
void func_800A0BAC(void);  /* TODO: Decompile */

/**
 * Camera initialization
 * Address: 0x800A0F74
 *
 * Called when entering gameplay to reset camera state.
 */
void func_800A0F74(u32 param);  /* TODO: Decompile */

/**
 * Camera mode switch
 * Address: 0x800A0FDC
 *
 * Switches between chase cam, cockpit, etc.
 */
void func_800A0FDC(u32 a0, u32 a1);  /* TODO: Decompile */
