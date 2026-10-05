typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32;
typedef struct { u8 b0; u8 pad1[3]; s32 count; s32 size; u8 *mem; s32 f16; u8 *free; } Pool;
typedef struct { s32 w[5]; } Rec20;
typedef struct { s32 head; } List;
extern Pool D_80143FC8;
extern u8 D_8014D280[];
extern s8 D_80114650;
extern s8 D_80150DD0;
extern s32 D_80150B64;
extern u8 D_80150E28[8];
extern s32 D_80151408;
extern Rec20 D_80151528[4];
extern s32 D_8014A110;
extern void *D_80151A70;
extern Pool D_80151AA8;
extern s32 D_80150F78;
extern s32 D_80151688;
extern s32 D_801515F0;
extern s32 D_80151964;
extern s32 D_80151610;
void pool_linked_list_init(Pool *p);
void func_800BB7F4(void);
void *audio_dma_sync(s32 arg0, s32 arg1);
void func_803914A8(void);
extern void dbg(void *);
extern s32 gDebug;

static void pool_init(Pool *p, void *mem, s32 count, s32 size) {
    p->mem = mem;
    p->count = count;
    p->size = size;
    p->b0 = 0;
    pool_linked_list_init(p);
}

static void list_init(List *l) {
    l->head = 0;
}

void camera_lerp_position(s32 arg0) {
    s16 i;

    pool_init(&D_80143FC8, D_8014D280, 130, 112);
    D_80150DD0 = D_80114650;
    func_800BB7F4();
    D_80150B64 = 0;
    for (i = 0; i < 8; i++) {
        D_80150E28[i] = 0;
    }
    D_80151408 = 0;
    for (i = 0; i < 4; i++) {
        D_80151528[i].w[0] = 0;
        D_80151528[i].w[1] = 0;
        D_80151528[i].w[2] = 0;
        D_80151528[i].w[3] = 0;
        D_80151528[i].w[4] = 0;
    }
    if (D_8014A110 == 6) {
        if (D_80150DD0 == 0) {
            D_80151A70 = audio_dma_sync(0, 120);
        }
        D_80150E28[5] = 16;
        func_803914A8();
        pool_init(&D_80151AA8, D_80151A70, 15, 8);
    }
    D_80150F78 = 0;
    { s32 *p = &D_80151688; *p = 0; }
    D_801515F0 = 0;
    { s32 *p = &D_80151964; *p = 0; }
    D_80151610 = 0;
}
