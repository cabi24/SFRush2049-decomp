/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NOT A MATCH (w16e): 3 of 66 words in the whole-program unit -- only the index*8 spill home (36(sp) vs
 * retail 32(sp)). Release audio slot `index`: wait for DMA idle, free its heap block through the inlined
 * heap-free helper func_800960CC (lock / audio_reverb_update / unlock; real definition comes from the
 * prefer_definition override in codex_heap_release_a25/wheel_params_set.c), clear the slot's allocation and
 * three 8-byte per-slot records.
 * Shaping: one compiled-out read `if (slot) {}` right after the store (keeps the slot web unsplit, v1).
 * Static helper dma_wait (inlined) gives the 8 bytes of frame (72). */
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Slot20 { u8 pad0[6]; u8 resource; u8 pad7; u32 word8, allocation, word16; } Slot20;
extern Slot20 D_80156D38[64];
extern volatile s16 D_8002EB70;
extern u8 D_801161F4[], D_80151AE8[], D_80138670[];
void *memset(void *, int, u32);
void func_800960CC(u32 address);

static void dma_wait(void)
{
    while (D_8002EB70 != 0) {
    }
}
void func_80096130(s32 index)
{
    Slot20 *slot = &D_80156D38[index];

    if (slot->allocation != 0) {
        dma_wait();
        func_800960CC(slot->allocation);
        slot->allocation = 0;
        if (slot) {}
        memset(D_801161F4 + index * 8, 0, 8);
        memset(D_80151AE8 + index * 8, 0, 8);
        memset(D_80138670 + index * 8, 0, 8);
    }
}
