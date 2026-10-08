/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef int s32;

s32 func_8008AD6C(u32 *gfx) {
    u32 w;
    u32 x;
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
    u32 f;

    w = gfx[0];
    x = w & 0xFF000000;
    if (x == 0x05000000) {
        gfx[0] = (w & 0xFFFF0000) | ((w & 0xFF00) >> 8) | ((w & 0xFF) << 8);
        return 2;
    }
    if (x == 0x07000000 || x == 0x06000000) {
        a = w & 0xFFFF0000;
        b = (w & 0xFF00) >> 8;
        c = (w & 0xFF) << 8;
        w = gfx[1];
        d = w & 0xFFFF0000;
        e = (w & 0xFF00) >> 8;
        f = (w & 0xFF) << 8;
        gfx[0] = (a | b) | c;
        gfx[1] = (d | e) | f;
    }
    return 2;
}
