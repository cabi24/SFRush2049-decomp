typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;

typedef struct List {
    u8 f0;
    u8 f1;
    s32 f4;
    s32 f8;
    s32 fC;
} List;

extern List D_80146160;
extern List D_80146138;
extern s32 D_801460F4;
extern void *D_80144C48;
extern char *D_80146100;
extern s32 D_80144DB8;
extern s32 D_801460C8[];
extern s8 D_8011028C;
extern s8 D_80110284;

extern void *audio_dma_sync(s32, s32);
extern void dma_request(void *, s32);
extern void *memset(void *, s32, s32);
extern void func_80091FBC(void *, void *, s32);

void func_800A4CB8(s32 arg0) {
    s32 i;
    s32 n;
    List *l;

    l = &D_80146160;
    l->f0 = 0;
    l->f1 = 1;
    l->f4 = 0;
    l->f8 = 0;
    l->fC = 0;
    D_801460F4 = arg0;
    l = &D_80146138;
    l->f0 = 0;
    l->f1 = 1;
    l->f4 = 0;
    l->f8 = 0;
    l->fC = 0;
    D_80144C48 = audio_dma_sync(0, D_801460F4 * 24);
    dma_request(D_80144C48, 0);
    memset(D_80144C48, 0, D_801460F4 * 24);
    n = arg0 * 3;
    D_80146100 = audio_dma_sync(0, n * 32);
    dma_request(D_80146100, 0);
    memset(D_80146100, 0, n * 32);
    for (i = 0; i < n; i++) {
        func_80091FBC(&D_80146138, D_80146100 + i * 32, D_80146138.f8);
    }
    D_80144DB8 = arg0;
    for (i = 0; i < 5; i++) {
        D_801460C8[i] = 0;
    }
    for (i = 0; i < 1; i++) {
        (&D_8011028C)[i] = 0;
    }
    D_80110284 = 1;
}
