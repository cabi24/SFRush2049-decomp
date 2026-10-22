/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef unsigned short u16;

s32 func_800B0EA0(s32 t, s32 a, s32 b)
{
    s32 inv;
    s32 c;

    if (t == 0) {
        return a;
    }
    if (t >= 255) {
        return b;
    }
    inv = 255 - t;
    c = ((((a & 0xF800) * inv + (b & 0xF800) * t) >> 8) & 0xF800)
      | ((((a & 0x7C0) * inv + (b & 0x7C0) * t) >> 8) & 0x7C0)
      | ((((a & 0x3E) * inv + (b & 0x3E) * t) >> 8) & 0x3E);
    c |= 1;
    return c;
}
