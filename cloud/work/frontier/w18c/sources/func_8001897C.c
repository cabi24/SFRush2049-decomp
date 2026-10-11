/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001897C - lib_17dc0 (IDO 7.1). Recompute the current context's tempo increment from its
 * tempo (rate124), the output/frame rate ratio D_8004F808 / D_8004F800 and the per-context
 * scale valueFC2 (8.8): value = ((rate * 3072 / 60) * ((D_8004F808 << 16) / D_8004F800)) >> 3,
 * scaled by valueFC2 >> 8; stored as fraction118 (low 16 bits) and whole11C (high), and
 * half120 = whole11C >> 1.
 * Base: w17g lockonly/func_8001897C.c (D10 body; LOCK-exact under 7.1, TU-blocked by its private
 * Context view). w18c: same body on include/sequence_context.h (w18c/include_patch names
 * fraction118/whole11C; layout unchanged); D_8004F808 as the header declares it (unsigned int),
 * D_8004F800 as lib_1a660's locked declaration (s32). Casts are D10's. */
#include "sequence_context.h"
extern int D_8004F800;

void func_8001897C(void)
{
    unsigned int value;
    value = ((D_8004BE80->rate124 * 3072U / 60U) *
        (unsigned int)((int)((unsigned int)D_8004F808 << 16) / D_8004F800)) >> 3;
    value = (D_8004BE80->valueFC2 * value) >> 8;
    D_8004BE80->fraction118 = value & 65535;
    D_8004BE80->whole11C = value >> 16;
    D_8004BE80->half120 = D_8004BE80->whole11C >> 1;
}
