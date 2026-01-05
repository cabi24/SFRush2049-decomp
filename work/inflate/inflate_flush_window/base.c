/*
 * Function: inflate_flush_window
 * Address:  0x80004D50
 * Category: inflate
 * Status:   WIP
 *
 * Initialize output window buffer for inflate.
 * Sets buffer pointer, size, and resets offset.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern u8 *gInflateWindowBuffer;
extern u32 gInflateWindowSize;
extern u32 gInflateWindowOffset;

void inflate_flush_window(u8 *buffer, u32 size) {
    gInflateWindowBuffer = buffer;
    gInflateWindowSize = size;
    gInflateWindowOffset = 0;
}
