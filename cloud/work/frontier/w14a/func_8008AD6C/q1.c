/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef int s32;

s32 func_8008AD6C(u32 *gfx) {
    u32 x;
    u32 hi;
    u32 lo;
    u32 w;

    w = *gfx;
    x = w & 0xFF000000;
    if (x == 0x05000000) {
        hi = w & 0xFF00;
        lo = w & 0xFF;
        *gfx = (w & 0xFFFF0000) | (hi >> 8) | (lo << 8);
        return 2;
    }
    if (x == 0x07000000 || x == 0x06000000) {
        hi = w & 0xFF00;
        lo = w & 0xFF;
        *gfx = (w & 0xFFFF0000) | (hi >> 8) | (lo << 8);
        w = gfx[1];
        hi = w & 0xFF00;
        lo = w & 0xFF;
        gfx++;
        *gfx = (w & 0xFFFF0000) | (hi >> 8) | (lo << 8);
    }
    return 2;
}
