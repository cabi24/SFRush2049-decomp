/*
 * Function: get_viewport_pos
 * Address:  0x80001E84
 * Category: boot
 * Status:   WIP
 *
 * Get current viewport position from offset tables.
 * Returns X and Y offsets based on gViewportX index.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern s16 gViewportX;
extern s16 gViewportOffsetX[];  /* Indexed by gViewportX * 4 */
extern s16 gViewportOffsetY[];  /* Indexed by gViewportX * 4 */

void get_viewport_pos(s32 *x, s32 *y) {
    s32 index = gViewportX << 3;  /* *8 for array stride */

    *x = gViewportOffsetX[index / 2];  /* lh at offset */
    *y = gViewportOffsetY[index / 2];
}
