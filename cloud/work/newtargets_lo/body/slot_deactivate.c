typedef struct { u8 pad0[2]; u8 active; u8 pad3[9]; void *h; u8 pad10[4]; } Slot;
extern Slot D_80156D38[];
void func_80096288(s32, s32, s32);
void dma_request(void *, s32);
void slot_deactivate(s32 i) {
    func_80096288(i, 0, 0);
    dma_request(D_80156D38[i].h, 1);
    D_80156D38[i].active = 0;
}
