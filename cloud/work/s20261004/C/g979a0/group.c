typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct ResSlot {         /* D_80156D38[], 0x14 bytes */
    s8 f0;
    s8 loaded;
    s8 f2;
    s8 f3;
    s8 f4;
    s8 f5;
    u8 type;
    u8 pad07;
    s32 pad08;
    s32 p0C;
    u8 *w10;
} ResSlot;

typedef struct SeqEntry {        /* D_8011F070[], 4 bytes */
    u16 h0;
    u16 h2;
} SeqEntry;

extern u8 D_80151960;
extern s32 D_8011EAA0;
extern s32 D_80151A6C;
extern s32 D_80151AD4;
extern s32 D_80151ADC;
extern ResSlot D_80156D38[];
extern SeqEntry D_8011F070[];
extern s32 D_80152464;
extern s32 D_801525FC;
extern s32 D_80152690;
extern s32 D_801526D8;

extern s32 audio_frame_sync(s32, s32, s32, s32, s32);
extern void func_8001536C(s32, u16, s32, s32, s32);
extern s32 func_800156E8(u16, u16, s32, s32);


void func_80096288(s32 arg0, s32 arg1, s32 arg2) {
    s32 t;
    if (arg0) {}
    if (arg1) {}
    t = !arg2;
    if (arg2 != 0)
    {
        if (t) {}
        if (t) {}
    }
    if (t) {}
}

extern void dma_request(s32, s32);

void display_list_alloc(s32 slot) {
    ResSlot *r;

    func_80096288(slot, 0, 0);
    r = &D_80156D38[slot];
    dma_request(r->p0C, 0);
    r->loaded = 1;
}

s32 slot_value_get(s32 arg) {
    func_80096288(arg, 0, 0);
    return D_80156D38[arg].p0C;
}

void func_800979A0(s32 arg0, s32 arg1) {
    SeqEntry *e;

    if (D_80151960 == 0) {
        if (D_8011EAA0 == -1 || arg0 != D_80151A6C) {
            D_80151AD4 = audio_frame_sync(arg0 + 10, 0, 0, 0, 0);
            display_list_alloc(D_80151AD4);
            D_80151ADC = slot_value_get(D_80151AD4);
            e = &D_8011F070[arg0];
            func_8001536C(D_80152464, e->h2, D_801525FC, D_80152690, D_801526D8);
            D_8011EAA0 = func_800156E8(e->h2, arg0, D_80151ADC, 0);
            D_80151A6C = arg0;
        }
    }
}
