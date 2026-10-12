/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_8008AD6C (w25e): flip the winding of one F3DEX2 triangle command in a display list. For opcode 0x05
 * (G_TRI1) it swaps the last two vertex-index bytes of word 0; for 0x06 (G_TRI2) and 0x07 (G_QUAD) it swaps them in
 * both words. Returns 2 (the command's word count). N64-only, no arcade ancestor.
 * Shape: every read goes through `*gfx` (no word local) and the opcode is tested as three separate
 * `(*gfx & 0xFF000000)` expressions. A `w = *gfx` local or an `op` variable makes uopt propagate the byte
 * pieces into ugen temps (w21g/w24f, 29 words); reading memory keeps `mid`/`low` as coloured webs (v1/a1) and
 * the load in v0 as in retail. No shaping devices, no unused locals. */
typedef unsigned int u32;
typedef int s32;

s32 func_8008AD6C(u32 *gfx) {
    u32 mid;
    u32 low;

    if ((*gfx & 0xFF000000) == 0x05000000) {
        mid = *gfx & 0xFF00;
        low = *gfx & 0xFF;
        *gfx = (*gfx & 0xFFFF0000) | (mid >> 8) | (low << 8);
    } else if ((*gfx & 0xFF000000) == 0x07000000 || (*gfx & 0xFF000000) == 0x06000000) {
        mid = *gfx & 0xFF00;
        low = *gfx & 0xFF;
        *gfx = (*gfx & 0xFFFF0000) | (mid >> 8) | (low << 8);
        gfx++;
        mid = *gfx & 0xFF00;
        low = *gfx & 0xFF;
        *gfx = (*gfx & 0xFFFF0000) | (mid >> 8) | (low << 8);
    }
    return 2;
}
