/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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

static void slot_free(Slot20 *s)
{
    dma_wait();
    func_800960CC(s->allocation);
    if (s) {}
    s->allocation = 0;
}

void func_80096130(s32 index)
{
    Slot20 *slot = &D_80156D38[index];

    if (slot->allocation != 0) {
        slot_free(slot);
        memset(D_801161F4 + index * 8, 0, 8);
        memset(D_80151AE8 + index * 8, 0, 8);
        memset(D_80138670 + index * 8, 0, 8);
    }
}
