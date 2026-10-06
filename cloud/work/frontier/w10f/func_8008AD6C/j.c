typedef unsigned int u32; typedef int s32; typedef unsigned short u16; typedef unsigned char u8;
s32 func_8008AD6C(u32 *gfx) {
    u32 w;
    u32 op;
    u32 lo, hi;
    w = *gfx;
    op = w & 0xFF000000;
    if (op == 0x05000000) {
        lo = w & 0xFF; hi = w & 0xFF00; w &= 0xFFFF0000; w |= hi >> 8; w |= lo << 8; *gfx = w;
        return 2;
    }
    if (op == 0x07000000 || op == 0x06000000) {
        lo = w & 0xFF; hi = w & 0xFF00; w &= 0xFFFF0000; w |= hi >> 8; w |= lo << 8; *gfx = w;
        w = gfx[1];
        gfx++;
        lo = w & 0xFF; hi = w & 0xFF00; w &= 0xFFFF0000; w |= hi >> 8; w |= lo << 8; *gfx = w;
    }
    return 2;
}
