/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001E7BC - lib_1f340 (IDO 7.1). Sine lookup: PHASE & 4095 over a full turn, from the
 * quarter-wave table D_8002CC54[1024] (s16): quadrant 0 table[p], 1 table[1023 - p],
 * 2 -table[p], 3 -table[1023 - p] (p = phase & 1023).
 * Natural form: D_8002CC54[1023 - (phase & 1023)] (true=20 reloc_blind=0: relocation NAME
 * only). Relocation-label view (disclosed shaping device): IDO folds the mirrored index into
 * (D_8002CC54 + 2046)[-p], an address the reloc-aware target names by its own label
 * D_8002D452 (= &D_8002CC54[1023]); the mirrored reads are spelled (&D_8002D452[-1023])[1023 - p]
 * so the index arithmetic stays D10's. Same practice as docs/reloc_reconciliation.md.
 * Prior: D10 C11-numeric (5.3, 41 words; 7.1 true=20 reloc_blind=0). */
typedef unsigned short u16;
extern const short D_8002CC54[1024];
extern const short D_8002D452[];

short func_8001E7BC(u16 phase)
{
    phase &= 4095;
    if (phase < 1024) {
        return D_8002CC54[phase];
    }
    if (phase < 2048) {
        return (&D_8002D452[-1023])[1023 - (phase & 1023)];
    }
    if (phase < 3072) {
        return -D_8002CC54[phase & 1023];
    }
    return -(&D_8002D452[-1023])[1023 - (phase & 1023)];
}
