typedef unsigned int u32; typedef int s32; typedef unsigned short u16; typedef unsigned char u8;
s32 func_8008AD6C(u32 *gfx) {
    u32 w;
    u32 op;
    u32 lo;
    w = *gfx;
    op = w & 0xFF000000;
    if (op == 0x05000000) {
        lo = w & 0xFF; op = w & 0xFF00; *gfx = (w & 0xFFFF0000) | (op >> 8) | (lo << 8);
        return 2;
    }
    if (op == 0x07000000 || op == 0x06000000) {
        lo = w & 0xFF; op = w & 0xFF00; *gfx = (w & 0xFFFF0000) | (op >> 8) | (lo << 8);
        w = gfx[1];
        gfx++;
        lo = w & 0xFF; op = w & 0xFF00; *gfx = (w & 0xFFFF0000) | (op >> 8) | (lo << 8);
    }
    return 2;
}
