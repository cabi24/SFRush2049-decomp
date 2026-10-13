/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_80096130 (w17i): release audio slot `index`. Waits for DMA idle (inlined static dma_wait), frees the slot's
 * heap block through the inlined heap-free helper func_800960CC (lock / audio_reverb_update / unlock; its real
 * definition is wheel_params_set.c's, the prefer_definition override of the locked stub), clears the allocation and
 * the slot's three 8-byte per-slot records. No arcade ancestor (N64 audio heap code).
 * Shaping (disclosed): one compiled-out read `if (slot) {}` before the store. It is a code-free extra use that keeps
 * the slot address web unsplit (totalsave 4 > caller-save cost 3), so it is coloured v1 ahead of the allocation
 * (a3) as in retail. The early `return` (instead of w16e's nested if) is the source lever for the tail: with the
 * return arm as block 0's fall-through, ugen closes v1's `.noalias $3,$sp` region there, so as1 keeps
 * `sw zero,12(v1)` ahead of the `lw v0,72(sp)` index reload (retail order). No unused locals, no pad, no stand-ins. */
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Slot20 { u8 pad0[6]; u8 resource; u8 pad7; u32 word8, allocation, word16; } Slot20;
extern Slot20 gWaveInstance[64];
extern volatile s16 D_8002EB70;
extern u8 gRomObjDefPtrs[], gTexDefPtrs[], gPalDefPtrs[];
void *memset(void *, int, u32);
void func_800960CC(u32 address);

static void dma_wait(void)
{
    while (D_8002EB70 != 0) {
    }
}

void func_80096130(s32 index)
{
    Slot20 *slot = &gWaveInstance[index];

    if (slot->allocation == 0) {
        return;
    }
    dma_wait();
    func_800960CC(slot->allocation);
    if (slot) {}
    slot->allocation = 0;
    memset(gRomObjDefPtrs + index * 8, 0, 8);
    memset(gTexDefPtrs + index * 8, 0, 8);
    memset(gPalDefPtrs + index * 8, 0, 8);
}
