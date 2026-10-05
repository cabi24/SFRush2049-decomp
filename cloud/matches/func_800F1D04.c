/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Name-cache slot allocator: 20 slots of 13-byte names at 0x80151968 with a
 * u16 age per slot at 0x80151A78.  Ages every slot, then returns the slot
 * that already holds `name` (func_8008AD04 is strcmp), else the first empty
 * slot, else the first slot not referenced by the 12x3x5 table of slot
 * numbers at 0x80151690, else the oldest slot (every reference to it in the
 * table is reset to -1 first).  The chosen slot gets the name
 * (func_800A473C is strcpy) and age 0.  No arcade ancestor identified.
 * Also matches at -O2 and as a group with both real callees kept.
 *
 * What the match depends on:
 *  - `victim` is an unsigned copy of `best`: the unsigned compare makes uopt
 *    hoist the conversion as its own register (`move a1,t2`), and the second
 *    s32-sized local after `slot` gives the 120-byte frame.  `== best`
 *    directly, or an s32/s16 copy, puts `best` in a2 and gives a 112-byte
 *    frame (103 words differ).
 *  - Declaration order sets the homes: i/j/k above references[], then
 *    oldest, best, slot, victim.
 *  - `best` is read uninitialised when no age is above 0, as in retail
 *    (`lh t2,68(sp)` before the scan).
 */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

extern u8 D_80151968[20][13];
extern u16 D_80151A78[20];
extern s32 D_80151690[12][3][5];
extern s32 func_8008AD04(u8 *, u8 *);
extern u8 *func_800A473C(u8 *, u8 *);

s16 func_800F1D04(u8 *name)
{
    s16 i, j, k;
    s16 references[20];
    s16 oldest, best;
    s32 slot;
    u32 victim;

    for (i = 0; i < 20; i++) {
        D_80151A78[i]++;
    }
    for (i = 0; i < 20; i++) {
        if (func_8008AD04(D_80151968[i], name) == 0) {
            D_80151A78[i] = 0;
            return i;
        }
    }
    for (i = 0; i < 20; i++) {
        if (D_80151968[i][0] == 0) {
            func_800A473C(D_80151968[i], name);
            D_80151A78[i] = 0;
            return i;
        }
    }
    for (i = 0; i < 20; i++) {
        references[i] = 0;
    }
    for (j = 0; j < 12; j++) {
        for (k = 0; k < 3; k++) {
            for (i = 0; i < 5; i++) {
                slot = D_80151690[j][k][i];
                if (slot >= 0 && slot < 20) {
                    references[slot]++;
                }
            }
        }
    }
    for (i = 0; i < 20; i++) {
        if (references[i] == 0) {
            func_800A473C(D_80151968[i], name);
            D_80151A78[i] = 0;
            return i;
        }
    }
    oldest = 0;
    for (i = 0; i < 20; i++) {
        if (oldest < D_80151A78[i]) {
            oldest = D_80151A78[i];
            best = i;
        }
    }
    victim = best;
    for (j = 0; j < 12; j++) {
        for (k = 0; k < 3; k++) {
            for (i = 0; i < 5; i++) {
                if (D_80151690[j][k][i] == victim) {
                    D_80151690[j][k][i] = -1;
                }
            }
        }
    }
    func_800A473C(D_80151968[best], name);
    D_80151A78[best] = 0;
    return best;
}

