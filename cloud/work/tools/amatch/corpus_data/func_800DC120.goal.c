s32 func_800DC120(void)
{
    u32 bit;
    u32 i;
    s32 sum;

    if (D_801170F0 == 0) {
        return 0;
    }
    i = 0;
    sum = 0;
    while (i < D_801170EC) {
        bit = D_8012E618[i >> 3] & (1 << (i & 7));
        i++;
        while (bit >= (1 << D_801170F0)) {
            bit >>= D_801170F0;
        }
        sum += bit;
    }
    return sum;
}