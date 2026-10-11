/* GENERATED ROM-aligned TU — segment 0xca70 (rom/lib_ca70)
 * layout map ea557f1d62ec891152dea96bea0586c4272de97209ee1945ee7814bfc3c2f8e8; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — osAiSetNextBuffer
 * Source:   cloud/work/frontier/w16g/sources/osAiSetNextBuffer.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/osAiSetNextBuffer.c:osAiSetNextBuffer (score0); rom_tu.h context: cloud/work/frontier/w19c/ctx/ctx_w19c.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#define TRUE 1
#define FALSE 0
#define IO_WRITE(addr,value) (*(volatile u32 *)(addr)=(value))
#define AI_DRAM_ADDR_REG 0xA4500000
#define AI_LEN_REG 0xA4500004
extern s32 __osAiDeviceBusy(void);
extern u32 osVirtualToPhysical(void *vaddr);
s32 osAiSetNextBuffer(void* bufPtr, u32 size) {
    static u8 hdwrBugFlag = FALSE;
    char* bptr;

    if (__osAiDeviceBusy()) {
        return -1;
    }

    bptr = bufPtr;

    if (hdwrBugFlag) {
        bptr = (u8*)bufPtr - 0x2000;
    }

    if ((((u32)bufPtr + size) & 0x1fff) == 0) {
        hdwrBugFlag = TRUE;
    } else {
        hdwrBugFlag = FALSE;
    }

    IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(bptr));
    IO_WRITE(AI_LEN_REG, size);
    return 0;
}

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

