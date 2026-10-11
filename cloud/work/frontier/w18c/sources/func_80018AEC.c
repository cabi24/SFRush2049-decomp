/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80018AEC - lib_17dc0 (IDO 7.1). Sequence clock tick for the current context
 * D_8004BE80 (when its timing list is active): add the fixed-point tempo increment
 * (fraction118 . whole11C, 16.16) to the context clock (lowF70 . highF74): the 16-bit
 * fraction wraps into lowF70 and its carry plus the whole part go to highF74.
 * Field names fraction118/whole11C follow w17g lockonly func_8001897C (which sets them:
 * fraction118 = value & 0xFFFF, whole11C = value >> 16).
 * NEEDS include/sequence_context.h: `unsigned char unknown118[8];` -> `unsigned int
 * fraction118; int whole11C;` (same layout; patch in w18c/include_patch/). With a byte-array
 * view (`((u32 *)ctx->unknown118)[0]`) uopt reorders the last sum (true=45).
 * Prior: D10 BT03-high-medium (u32 array view, 7.1 true=35). No shaping devices. */
#include "sequence_context.h"

void func_80018AEC(void)
{
    unsigned int value;
    if (D_8004BE80->timingStartF68) {
        value = D_8004BE80->lowF70 + D_8004BE80->fraction118;
        D_8004BE80->lowF70 = value & 0xFFFF;
        value >>= 16;
        D_8004BE80->highF74 = D_8004BE80->highF74 + value + D_8004BE80->whole11C;
    }
}
