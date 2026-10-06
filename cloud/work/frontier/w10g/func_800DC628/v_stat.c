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


s32 func_800DC628(u32 a, u32 b)
{
    static s32 D_801170F8 = 1;
    s32 i;
    if (D_801170F8) {
        D_801170F8 = 0;
        for (i = 0; i < 32; i++) {
            D_80116FE8[D_80116FE4[i]] = i;
        }
    }
    D_801170E8 = (a + b + 7) >> 3;
    if (D_801170E8 > 32) return 0;
    if (b > 32) return 0;
    for (i = 0; i < D_801170E8; i++) {
        D_8012E618[i] = 0;
    }
    D_801170EC = a;
    D_801170F0 = b;
    D_801170F4 = 0;
    return 1;
}
