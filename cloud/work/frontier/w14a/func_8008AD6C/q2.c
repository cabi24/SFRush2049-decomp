/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef int s32;

s32 func_8008AD6C(u32 *gfx) {
    u32 w;
    u32 x;

    w = gfx[0];
    x = w & 0xFF000000;
    if (x == 0x05000000) {
        gfx[0] = (w & 0xFFFF0000) | ((w & 0xFF00) >> 8) | ((w & 0xFF) << 8);
        return 2;
    }
    if (x == 0x07000000 || x == 0x06000000) {
        u32 w1 = gfx[1];
        gfx[0] = (w & 0xFFFF0000) | ((w & 0xFF00) >> 8) | ((w & 0xFF) << 8);
        gfx[1] = (w1 & 0xFFFF0000) | ((w1 & 0xFF00) >> 8) | ((w1 & 0xFF) << 8);
    }
    return 2;
}
