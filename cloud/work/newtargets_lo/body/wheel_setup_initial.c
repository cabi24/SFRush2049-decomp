typedef struct { u8 pad0[6]; u8 owner; u8 pad7[5]; void *h; u8 pad10[4]; } Slot;
extern Slot D_80156D38[];
void func_80096130(s32);
void wheel_setup_initial(s32 a) {
    s32 i;
    Slot *p = D_80156D38;
    s32 n = 64;
    for (i = 0; i != n; i++, p++) {
        if (p->h != 0 && p->owner == a) {
            func_80096130(i);
        }
    }
}
