/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Record a lap split for a player (N64 race code; no arcade ancestor found).
 * Unless the debug flag bit 3 of D_801174B4 is set or the mode word
 * D_8014A110 is 1, and only for player < D_8014A108 (active count):
 * the per-player lap counter D_80144018[player] (u8) selects the next slot of
 * the eight-float row D_80149A78[player]; the cumulative time is stored there
 * and every previous lap time is subtracted from it, giving the lap time.
 * The best-lap table D_80144DA8[player] is lowered when beaten and the lap
 * counter incremented.
 *
 * Shaping quirk: every access spells the counter as D_80144018[player] (no
 * local copy, no pointer to the slot). With a local count or slot pointer uopt
 * unrolls the subtraction loop by 4 (76 words); with the global spelling the
 * float stores into D_80149A78 provably miss D_80144018, so the bound is
 * hoisted after the unrolling decision and the loop stays rolled.
 */
typedef float f32;
typedef short s16;
typedef unsigned char u8;

extern int D_801174B4;
extern int D_8014A110;
extern s16 D_8014A108;
extern f32 D_80149A78[][8];
extern u8 D_80144018[];
extern f32 D_80144DA8[];

void func_800D2054(int player, f32 time)
{
    int i;

    if (!(D_801174B4 & 8) && D_8014A110 != 1 && player < D_8014A108) {
        D_80149A78[player][D_80144018[player]] = time;
        for (i = 0; i < D_80144018[player]; i++) {
            D_80149A78[player][D_80144018[player]] -= D_80149A78[player][i];
        }
        if (D_80149A78[player][D_80144018[player]] < D_80144DA8[player]) {
            D_80144DA8[player] = D_80149A78[player][D_80144018[player]];
        }
        D_80144018[player]++;
    }
}
