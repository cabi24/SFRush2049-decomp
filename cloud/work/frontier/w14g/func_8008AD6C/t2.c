/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef int s32;
typedef unsigned char u8;

s32 func_8008AD6C(u32 *gfx) {
    /*@{ty*/u32 x; u32 hi; u32 lo; u32 w;/*@| s32 x; s32 hi; s32 lo; s32 w; @| u32 x; s32 hi; s32 lo; s32 w; @| s32 x; u32 hi; u32 lo; u32 w; @| u32 x; u32 w; u32 lo; u32 hi; @| u32 w; u32 hi; u32 lo; u32 x; @}*/
    /*@{ws*/w = *gfx;/*@| w = gfx[0]; @| w = *(u32 *) gfx; @}*/
    /*@{xs*/x = w & 0xFF000000;/*@| x = w & 0xFF000000u; @| x = (u32) (w & 0xFF000000); @| x = w; x &= 0xFF000000; @| x = w; x = x & 0xFF000000; @}*/
    if (x == 0x05000000) {
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
