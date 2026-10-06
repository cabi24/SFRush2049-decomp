typedef unsigned int u32; typedef int s32;
s32 func_8008AD6C(u32 *gfx) {
    u32 w;
    u32 x;
    u32 lo;
    s32 i, n;
    w = *gfx;
    x = w & 0xFF000000;
    if (x == 0x05000000) {
        n = 1;
    } else if (x == 0x07000000 || x == 0x06000000) {
        n = 2;
    } else {
        return 2;
    }
    for (i = 0; i < n; i++) {
        w = *gfx;
        x = w & 0xFF00;
        lo = w & 0xFF;
        *gfx++ = (w & 0xFFFF0000) | (x >> 8) | (lo << 8);
    }
    return 2;
}
