/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800958B8: mark every active entry of the 24-byte table at *D_80144C48
 * (count D_801460F4) for refresh (byte +4 = 1), then raise the global flag D_8011028C.
 * Arcade ancestor: not identified.
 * Shaping quirk: D_8011028C is volatile -- retail stores it through `la t8; sb a3,0(t8)`
 * (as1 hoists the `la` into the blez delay slot); a plain u8 gives `lui at; sb ...,%lo(at)`.
 * The table base is a pointer global (reloaded each iteration because the byte store may alias it),
 * and the count is re-read after each store for the same reason.
 * Also MATCH at -O2. Whole-program unit: EQUAL.
 */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;

typedef struct {
    s8 active;      /* 0 */
    u8 pad1[3];
    u8 flag;        /* 4 */
    u8 pad5[19];
} Entry;            /* 24 */

extern Entry *D_80144C48;
extern s32 D_801460F4;
extern volatile u8 D_8011028C;

void func_800958B8(void)
{
    s32 i;

    for (i = 0; i < D_801460F4; i++) {
        if (D_80144C48[i].active) {
            D_80144C48[i].flag = 1;
        }
    }
    D_8011028C = 1;
}
