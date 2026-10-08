/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef int s32;

s32 func_8008AD6C(u32 *gfx) {
    /*@{dd*/u32 x; u32 hi; u32 lo; u32 w;/*@| u32 w; u32 lo; u32 hi; u32 x; @| u32 x, w, hi, lo; @| u32 w; u32 x; u32 lo; u32 hi; @}*/
    /*@{ws*/w = *gfx;/*@| w = gfx[0]; @| w = *(u32 *) gfx; @}*/
    x = /*@{xm*/w & 0xFF000000/*@| w & 0xFF000000u @| 0xFF000000 & w @}*/;
    if (x == /*@{cm*/0x05000000/*@| 0x05000000u @}*/) {
        /*@{ho*/hi = w & 0xFF00;
        lo = w & 0xFF;/*@| lo = w & 0xFF;
        hi = w & 0xFF00; @}*/
        *gfx = (w & /*@{mk*/0xFFFF0000/*@| 0xFFFF0000u @| ~0xFFFF @}*/) | (hi >> 8) | (lo << 8);
        return 2;
    }
    if (x == 0x07000000 || x == 0x06000000) {
        /*@{ho2*/hi = w & 0xFF00;
        lo = w & 0xFF;/*@| lo = w & 0xFF;
        hi = w & 0xFF00; @}*/
        *gfx = (w & /*@{mk2*/0xFFFF0000/*@| 0xFFFF0000u @| ~0xFFFF @}*/) | (hi >> 8) | (lo << 8);
        w = gfx[1];
        hi = w & 0xFF00;
        lo = w & 0xFF;
        gfx++;
        *gfx = (w & 0xFFFF0000) | (hi >> 8) | (lo << 8);
    }
    return 2;
}
