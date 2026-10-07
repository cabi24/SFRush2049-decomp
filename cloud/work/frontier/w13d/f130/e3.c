/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NOT A MATCH (w12e): 3 of 66 words in the unit (was 21). `if (slot) {}` before the store fixes the colouring
 * (slot v1, allocation a3); `if (0) {}` after the store places ugen's `.alias $3,$sp` so as1 keeps the store before
 * the memset code; residual: the index*8 spill home is 36(sp) (reused from the slot web), retail 32(sp).
 * `off` is a named local replacing w11h's dma_wait static (frame 72). See ../RESULTS.md. */
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

static void clear3(s32 off)
{
    memset(D_801161F4 + off, 0, 8);
    memset(D_80151AE8 + off, 0, 8);
    memset(D_80138670 + off, 0, 8);
}

static void clear3i(s32 index)
{
    memset(D_801161F4 + index * 8, 0, 8);
    memset(D_80151AE8 + index * 8, 0, 8);
    memset(D_80138670 + index * 8, 0, 8);
}

void func_80096130(s32 index)
{
    Slot20 *slot = &D_80156D38[index];
    s32 off;

    if (slot->allocation != 0) {
        while (D_8002EB70 != 0) {
        }
        func_800960CC(slot->allocation);
        if (slot) {}
        slot->allocation = 0;
        clear3(index * 8);
    }
}
