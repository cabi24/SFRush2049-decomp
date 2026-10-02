/* GENERATED ROM-aligned TU — segment 0xefc0 (rom/lib_efc0)
 * layout map 4f3f427d418a827f79392ed5cd2e534299f40208223718dc18b06a16e0b36ecb; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* IDO intrinsic required: the same-name call denotes hardware square root. */
float sqrtf(float);
#pragma intrinsic(sqrtf)

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_efc0/sqrtf.s")
