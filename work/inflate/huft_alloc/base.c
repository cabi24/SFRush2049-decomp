/*
 * Function: huft_alloc
 * Address:  0x80004D6C
 * Category: inflate
 * Status:   WIP
 *
 * Bump allocator for Huffman table memory.
 * Increments heap pointer and returns previous position.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern u32 D_800354C8;  /* Current heap offset */
extern u8 *D_800354C4;  /* Heap base pointer */

void *huft_alloc(u32 size) {
    u32 old_offset;

    D_800354C8 += size;
    old_offset = D_800354C8 - size;

    return D_800354C4 + old_offset;
}
