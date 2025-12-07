/**
 * @file gfx.c
 * @brief Graphics display list management
 *
 * Decompiled from asm/us/5610.s
 */

#include "types.h"

/* External data - display list pointers */
extern Gfx *gDisplayListHead;  /* D_800354C4 */
extern Gfx *gDisplayListEnd;   /* D_800354CC */
extern u32 gDisplayListSize;   /* D_800354C8 */

/**
 * Initialize display list buffer
 * @param start Pointer to start of display list buffer
 * @param end Pointer to end of display list buffer
 */
void gfx_init_dl(Gfx *start, Gfx *end) {
    gDisplayListHead = start;
    gDisplayListEnd = end;
    gDisplayListSize = 0;
}

/**
 * Allocate space in display list
 * @param size Number of Gfx commands to allocate
 * @return Pointer to allocated display list space
 */
Gfx *gfx_alloc_dl(u32 size) {
    Gfx *result;

    gDisplayListSize += size;
    result = gDisplayListHead + (gDisplayListSize - size);

    return result;
}
