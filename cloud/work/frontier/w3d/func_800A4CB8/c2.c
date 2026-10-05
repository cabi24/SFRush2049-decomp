typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;

typedef struct Link {
    struct Link *next, *prev;
} Link;

typedef struct List {
    u8 indirect, doubly, pad[2];
    u32 count;
    Link *head, *tail;
} List;

typedef struct { u8 bytes[24]; } Rec24;
typedef struct { Link link; u8 bytes[24]; } Rec32;

extern List D_80146160;
extern List D_80146138;
extern s32 D_801460F4;
extern s32 D_80144DB8;
extern Rec24 *D_80144C48;
extern Rec32 *D_80146100;
extern s32 D_801460C8[];
extern volatile s8 D_8011028C;
extern s8 D_80110284;

void *audio_dma_sync(s32 arg0, s32 arg1);
void audio_loop_control(void *arg0, s32 arg1);
void *memset(void *, s32, u32);
void func_80091FBC(List *list, Link *object, Link *before);

void func_800A370C(List *list) {
    list->head = 0;
    list->tail = 0;
    list->count = 0;
}

static void list_init(List *list, s8 indirect, s8 doubly) {
    list->indirect = indirect;
    list->doubly = doubly;
    func_800A370C(list);
}

void func_800A4CB8(s32 count) {
    s32 i;
    s32 n;

    list_init(&D_80146160, 0, 1);
    D_801460F4 = count;
    list_init(&D_80146138, 0, 1);
    D_80144C48 = audio_dma_sync(0, D_801460F4 * sizeof(Rec24));
    audio_loop_control(D_80144C48, 0);
    memset(D_80144C48, 0, D_801460F4 * sizeof(Rec24));
    n = count * 3;
    D_80146100 = audio_dma_sync(0, n * sizeof(Rec32));
    audio_loop_control(D_80146100, 0);
    memset(D_80146100, 0, n * sizeof(Rec32));
    for (i = 0; i < n; i++) {
        func_80091FBC(&D_80146138, &D_80146100[i].link, D_80146138.head);
    }
    D_80144DB8 = count;
    for (i = 0; i < 5; i++) {
        D_801460C8[i] = 0;
    }
    D_8011028C = 0;
    D_80110284 = 1;
}
