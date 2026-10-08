/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef int s32;

static u32 swap_word(u32 w) {
    return (w & 0xFFFF0000) | ((w & 0xFF00) >> 8) | ((w & 0xFF) << 8);
}

s32 func_8008AD6C(u32 *gfx) {
    u32 top = gfx[0] & 0xFF000000;
    u32 w0 = gfx[0];
    u32 w1;
    if (top == 0x05000000) {
        gfx[0] = swap_word(w0);
        return 2;
    }
    if (top == 0x07000000 || top == 0x06000000) {
        w1 = gfx[1];
        gfx[0] = swap_word(w0);
        gfx[1] = swap_word(w1);
    }
    return 2;
}
