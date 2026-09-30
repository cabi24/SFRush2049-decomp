typedef signed char s8;
typedef signed int s32;
s8 D_801147C0;
extern s32 D_801461D0;
extern s32 D_801461FC;
extern s8 D_80149DA0;
void osCreateMesgQueue(void *, void *, s32);
void osJamMesg(void *, s32, s32);
void sync_init_conditional(void) {
    if (D_801147C0 == 0) {
        D_801147C0 = 1;
        osCreateMesgQueue(&D_801461D0, &D_801461FC, 1);
        osJamMesg(&D_801461D0, 0, 0);
    }
    D_80149DA0 = -1;
}
