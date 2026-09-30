typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u8 active;
    /* 0x03 */ u8 pad3[9];
    /* 0x0C */ void *handle;
    /* 0x10 */ u8 pad10[4];
} Slot; /* 0x14 */

extern Slot D_80156D38[];
void dma_request(void *h, s32 mode);

void func_80096288(s32 a, s32 b, s32 c)
{
    if (c != 0) {
        if (0) {
            switch (c) { case 0: c = 1; break; case 1: c = 2; break; case 2: c = 3; break; case 3: c = 5; break; }
        }
    }
}

void slot_deactivate(s32 i)
{
    func_80096288(i, 0, 0);
    dma_request(D_80156D38[i].handle, 1);
    D_80156D38[i].active = 0;
}

void display_list_alloc(s32 i)
{
    func_80096288(i, 0, 0);
    dma_request(D_80156D38[i].handle, 0);
    D_80156D38[i].active = 1;
}

void __standin_a(void) { slot_deactivate(0); display_list_alloc(0); func_80096288(1, 1, 1); }
