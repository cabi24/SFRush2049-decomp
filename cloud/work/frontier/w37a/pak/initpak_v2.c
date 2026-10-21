
/* ---- PS_InitPakSys ---- */
extern PakHandle D_80144C60[];          /* the 64 tPS_File handles */
extern tPS_File D_80144DC0[];           /* the 64 tPS_File records (76 bytes) */
extern tPS_File D_801460C0[];           /* end of the records */
extern u8 D_800349F0[];                 /* pak thread */
extern u8 D_800329F0[];                 /* pak thread stack top */
extern void audio_queue_process(void *);    /* rumble/pak thread entry */
extern void osCreateThread(void *, s32, void (*)(void *), void *, void *, s32);
extern void osStartThread(void *);
extern void func_800A43FC(void);        /* poll pak presence (_CompleteCallback slot) */
extern void func_800A44E8(tDLList *, u8, u8);   /* InitLL (MAT3 char = unsigned) */

/* 0x800A4508: PS_InitPakSys */
void car_lod_select(PakErrorFunc errorFunc)
{
    s32 i;
    PakHandle *fileh;
    tPS_File *psfile;

    D_80144008 = errorFunc;
    func_800A44E8(&D_801460E0, 1, 1);
    fileh = D_80144C60;
    psfile = D_80144DC0;
    do {
        fileh->file = psfile;
        func_80091FBC(&D_801460E0, fileh, D_801460E0.head);
        psfile++;
        fileh++;
    } while (psfile < D_801460C0);
    for (i = 0; i < 4; i++) {
        func_800A44E8(&D_80144D60[i], 1, 1);
        D_80144030[i].damaged = 0;
        func_800A3724(i);
    }
    osCreateThread(D_800349F0, 2, audio_queue_process, 0, D_800329F0, 6);
    osStartThread(D_800349F0);
    D_8011EAE0 = 1;
    func_800A43FC();
}
