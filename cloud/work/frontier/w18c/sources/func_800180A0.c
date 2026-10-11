/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800180A0 - lib_17dc0 (IDO 7.1). Advance the 16.16 clock of every active track of the
 * current sequence context D_8004BE80 by the context's tempo increment (fraction118 .
 * whole11C): the 16-bit fraction wraps and carries into the whole part. A track is active
 * when any of its three channel words (0x0C/0x10/0x14) is nonzero. Sibling of func_80018AEC.
 * Base: w17g lockonly/func_800180A0.c (D10 body; LOCK-exact under 7.1, TU-blocked by its
 * private Context view). w18c: same body on include/sequence_context.h with the fields named
 * (w18c/include_patch: fraction118, whole11C, SequenceTrack tracks568[64]; layout unchanged).
 * The (u32)/(s32) casts are D10's (unsigned 32-bit wrap arithmetic, signed >> 16). */
#include "sequence_context.h"

void func_800180A0(void)
{
    int i;
    int sum;
    for (i = 0; i < 64; i++) {
        if (D_8004BE80->tracks568[i].active0C || D_8004BE80->tracks568[i].active10 ||
            D_8004BE80->tracks568[i].active14) {
            sum = (int)(D_8004BE80->tracks568[i].fraction + D_8004BE80->fraction118);
            D_8004BE80->tracks568[i].fraction = (unsigned int)sum & 65535;
            sum >>= 16;
            D_8004BE80->tracks568[i].whole = (int)((unsigned int)D_8004BE80->tracks568[i].whole +
                (unsigned int)sum + (unsigned int)D_8004BE80->whole11C);
        }
    }
}
