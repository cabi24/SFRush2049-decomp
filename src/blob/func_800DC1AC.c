/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 D_8012E618[];
extern u16 D_801170E8;
extern u16 D_801170EC;
extern u16 D_801170F0;
extern u16 D_801170F4;
extern u8 D_80116FE8[];
extern char *D_80116FE4;


s32 func_800DC1AC(u32 value, u32 nbits)
{
    s32 i;
    if (nbits > 32) return 0;
    if (D_801170EC < D_801170F4 + nbits) return 0;
    for (i = 0; i < nbits; D_801170F4++, value >>= 1, i++) {
        D_8012E618[D_801170F4 >> 3] |= (value & 1) << (D_801170F4 & 7);

    }
    return 1;
}

