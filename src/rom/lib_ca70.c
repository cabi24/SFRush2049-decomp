/* GENERATED ROM-aligned TU — segment 0xca70 (rom/lib_ca70)
 * layout map ea557f1d62ec891152dea96bea0586c4272de97209ee1945ee7814bfc3c2f8e8; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_ca70/osAiSetNextBuffer.s")
/* PROMOTED 2026-10-01 — osAiSetFrequency
 * Source:   cloud/work/static_C6/osAiSetFrequency.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C6/osAiSetFrequency.c:osAiSetFrequency (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osAiSetFrequency(u32 frequency) {
    register unsigned int dacRate;
    register unsigned char bitRate;
    register float f;


    
    f = gAudioDmaBufferPtr / (float)frequency + .5f;
    dacRate = f;

    
    
    if (dacRate < AI_MIN_DAC_RATE) {
        return -1;
    }

    
    
    bitRate = dacRate / 66;
    
    if (bitRate > AI_MAX_BIT_RATE) {
        bitRate = AI_MAX_BIT_RATE;
    }

    IO_WRITE(AI_DACRATE_REG, dacRate - 1);
    IO_WRITE(AI_BITRATE_REG, bitRate - 1);
    
    
    return gAudioDmaBufferPtr / (s32)dacRate;
}

