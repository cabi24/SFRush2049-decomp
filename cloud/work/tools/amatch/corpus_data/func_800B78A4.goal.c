/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

s32 func_800B78A4(u32 x, u8 n)
{
    s32 c = 0;

    while (n != 0 && x != 0) {
        if (x & 1) {
            c++;
        }
        n--;
        x >>= 1;
    }
    return c;
}
