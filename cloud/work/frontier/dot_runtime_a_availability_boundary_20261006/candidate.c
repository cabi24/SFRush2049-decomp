/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * Complete research NONMATCH: image A, 0x8039D494..0x8039D6A4.
 * N64 front-end option availability predicate. No exact arcade donor known.
 * Ordinary a0/a1 inputs, but a real private caller retains t2-t5 across this
 * function. This isolated compilation violates that narrower clobber set.
 * Do not submit as matching or replace the native body without real context.
 */
typedef signed char s8;
typedef short s16;
typedef int s32;
extern s8 D_801407D0, D_80156994, D_8014978C;
extern s32 D_803AF980, D_8014A110;
extern s16 D_801164BE;
extern s8 D_803B9FD0[], D_803B3020[];
s32 func_8039D494(s32 item, s32 player)
{
    s32 available = 1;
    if ((item == 13 || item == 14) && !D_801407D0) available = 0;
    if (!D_80156994) {
        if (item == 11 && D_8014978C >= 0 && D_8014978C < 6) available = 0;
        if (item == 17 || item == 18) available = 0;
    }
    if (!D_803AF980 && (item == 16 || item == 17 || item == 18)) available = 0;
    if (item == 6 && (D_8014A110 == 2 || D_8014A110 == 6)) available = 0;
    if ((item == 7 || item == 8 || item == 9) && D_8014A110 == 6 && !D_801164BE) available = 0;
    if (item == 7 && !(D_803B3020[D_803B9FD0[player]] & 1)) available = 0;
    if (item == 8 && !(D_803B3020[D_803B9FD0[player]] & 2)) available = 0;
    if (item == 9 && !(D_803B3020[D_803B9FD0[player]] & 4)) available = 0;
    if (item == 10 && D_8014A110 != 6) available = 0;
    if ((item == 16 || item == 17 || item == 18) && D_8014A110 != 2) available = 0;
    if (item == 12 || item == 15) available = 0;
    return available;
}
