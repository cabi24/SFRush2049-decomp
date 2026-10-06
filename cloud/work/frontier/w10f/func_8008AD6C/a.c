typedef unsigned int u32; typedef int s32;
s32 func_8008AD6C(u32 *gfx) {
    u32 w = gfx[0];
    u32 op = w & 0xFF000000;
    u32 hi, lo;
    if (op == 0x05000000) {
        hi = w & 0xFF00;
        lo = w & 0xFF;
        gfx[0] = (w & 0xFFFF0000) | (hi >> 8) | (lo << 8);
        return 2;
    }
    if (op == 0x07000000 || op == 0x06000000) {
        hi = w & 0xFF00;
        lo = w & 0xFF;
        gfx[0] = (w & 0xFFFF0000) | (hi >> 8) | (lo << 8);
        w = gfx[1];
        hi = w & 0xFF00;
        lo = w & 0xFF;
        gfx++;
        gfx[0] = (w & 0xFFFF0000) | (hi >> 8) | (lo << 8);
    }
    return 2;
}
