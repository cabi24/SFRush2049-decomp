typedef unsigned int u32; typedef int s32;
static void swap16(u32 *p) {
    u32 w = *p;
    u32 hi = w & 0xFF00;
    u32 lo = w & 0xFF;
    *p = (w & 0xFFFF0000) | (hi >> 8) | (lo << 8);
}
s32 func_8008AD6C(u32 *gfx) {
    u32 op = *gfx & 0xFF000000;
    if (op == 0x05000000) {
        swap16(gfx);
        return 2;
    }
    if (op == 0x07000000 || op == 0x06000000) {
        swap16(gfx);
        gfx++;
        swap16(gfx);
    }
    return 2;
}
