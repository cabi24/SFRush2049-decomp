/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Link {
    struct Link *next, *prev;
} Link;

typedef struct List {
    u8 indirect, doubly, pad[2];
    u32 count;
    Link *head, *tail;
} List;

typedef struct {
    /* 0x00 */ Link link;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 padA[0x32];
} RecA; /* 0x3C */

typedef struct {
    /* 0x00 */ Link link;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 padA[0x22];
} RecB; /* 0x2C */

extern List D_80146170;
extern List D_80146188;
extern List D_801461B0;
extern List D_801461E8;
extern RecA *D_8011025C;
extern RecB *D_80110260;
extern s32 D_80110268;
extern s32 D_8011026C;
extern f32 D_80139318;
extern f32 D_8013C090;
extern u8 D_801497E8;

void *audio_dma_sync(s32 arg0, s32 arg1);
void audio_loop_control(void *arg0, s32 arg1);
void func_80091FBC(List *list, Link *object, Link *before);

void func_800A44E8(List *list, u8 indirect, u8 doubly) {
    list->indirect = indirect;
    list->doubly = doubly;
    list->count = 0;
    list->tail = 0;
    list->head = 0;
}

void hud_speed_display(s32 numA, s32 numB, f32 arg2, f32 arg3, s32 arg4) {
    s32 i;

    func_800A44E8(&D_80146170, 0, 1);
    func_800A44E8(&D_80146188, 0, 1);
    func_800A44E8(&D_801461B0, 0, 1);
    func_800A44E8(&D_801461E8, 0, 1);
    if (numA > 0 && numB > 0) {
        D_80110268 = numA;
        D_8011025C = audio_dma_sync(0, D_80110268 * sizeof(RecA));
        audio_loop_control(D_8011025C, 0);
        for (i = 0; i < D_80110268; i++) {
            D_8011025C[i].unk8 = 1;
            D_8011025C[i].unk9 = 0;
            func_80091FBC(&D_80146170, &D_8011025C[i].link, D_80146170.head);
        }
        D_8011026C = numB;
        D_80110260 = audio_dma_sync(0, D_8011026C * sizeof(RecB));
        audio_loop_control(D_80110260, 0);
        for (i = 0; i < D_8011026C; i++) {
            D_80110260[i].unk8 = 1;
            D_80110260[i].unk9 = 0;
            func_80091FBC(&D_801461B0, &D_80110260[i].link, D_801461B0.head);
        }
        D_80139318 = 1100.0f * arg2;
        D_8013C090 = arg3;
        D_801497E8 = arg4;
    }
}
