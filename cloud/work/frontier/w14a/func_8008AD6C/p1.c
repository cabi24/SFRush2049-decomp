/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef int s32;

s32 func_8008AD6C(u32 *gfx) {
    u32 w0;
    u32 w1;
    u32 n0;
    u32 n1;

    w0 = gfx[0];
    if ((w0 & 0xFF000000) == 0x05000000) {
        n0 = (w0 & 0xFFFF0000) | ((w0 & 0xFF00) >> 8) | ((w0 & 0xFF) << 8);
        gfx[0] = n0;
        return 2;
    }
    if ((w0 & 0xFF000000) == 0x07000000 || (w0 & 0xFF000000) == 0x06000000) {
        w1 = gfx[1];
        n0 = (w0 & 0xFFFF0000) | ((w0 & 0xFF00) >> 8) | ((w0 & 0xFF) << 8);
        n1 = (w1 & 0xFFFF0000) | ((w1 & 0xFF00) >> 8) | ((w1 & 0xFF) << 8);
        gfx[0] = n0;
        gfx[1] = n1;
    }
    return 2;
}
