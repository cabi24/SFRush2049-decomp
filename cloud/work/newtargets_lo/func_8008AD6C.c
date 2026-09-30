typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
s32 func_8008AD6C(u32 *p) {
    u32 w = p[0];
    u32 op = w & 0xFF000000;
    u32 l, m;
    if (op == 0x05000000) {
        l = w & 0xFF;
        m = w & 0xFF00;
        p[0] = (w & 0xFFFF0000) | (m >> 8) | (l << 8);
    } else if (op == 0x07000000 || op == 0x06000000) {
        u32 w1, l1, m1;
        l = w & 0xFF;
        m = w & 0xFF00;
        w1 = p[1];
        l1 = w1 & 0xFF;
        m1 = w1 & 0xFF00;
        p[0] = (w & 0xFFFF0000) | (m >> 8) | (l << 8);
        p++;
        *p = (w1 & 0xFFFF0000) | (m1 >> 8) | (l1 << 8);
    }
    return 2;
}
