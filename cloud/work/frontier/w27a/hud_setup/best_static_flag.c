/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef void *OSMesg;
typedef struct OSMesgQueue { u8 opaque[0x18]; } OSMesgQueue;
typedef struct OSThread { u8 opaque[0x1B0]; } OSThread;

typedef struct Link {
    struct Link *next, *prev;
} Link;
typedef struct List {
    u8 indirect, doubly, pad[2];
    u32 count;
    Link *head, *tail;
} List;
typedef struct Voice {      /* 0x44 */
    Link link;
    List *owner;            /* 0x08 */
    s32 index;              /* 0x0C */
    u8 pad10[0x34];
} Voice;
typedef struct Chan {       /* 0x3C */
    Link link;
    s32 index;              /* 0x08 */
    s8 unkC;
    s8 unkD;
    u8 padE[0x2E];
} Chan;
typedef struct Slot24 { u8 pad0[3]; u8 active; u8 pad4[20]; } Slot24;
typedef struct Handler { void (*func)(void); s32 unk4; s32 unk8; } Handler;

extern s8 D_8011023C;
extern s32 D_80146200;
extern s32 D_80149410;
extern Slot24 D_80142DD8[128];
extern OSMesgQueue D_80142728;
extern OSMesg D_80142768[1];
extern OSThread D_80034F00;
extern u8 D_800339F0[];
extern s32 D_80110264;
extern Voice *D_80110244;
extern void *D_80110248;
extern s32 D_8011024C;
extern s32 D_80110250;
extern Handler D_80143F58;
extern List D_80143FF8, D_80144020, D_80144C50;
extern s32 D_801460F8;
extern s32 D_80146104;
extern Chan *D_80110270;
extern List D_80149808, D_80149860;

void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
void osCreateThread(OSThread *thread, s32 id, void (*entry)(void *), void *arg, void *sp, s32 priority);
void osStartThread(OSThread *thread);
void func_800C8918(void);
void car_shadow_render(s32 arg0, s32 arg1, s32 arg2);
void func_800992AC(void *arg);
void func_8009539C(void);
void *audio_dma_sync(s32 arg0, s32 arg1);
void audio_loop_control(void *arg0, s32 arg1);
void func_80091FBC(List *list, Link *object, Link *before);
s32 func_800C8738(s32 arg0);
void hud_speed_display(s32 numA, s32 numB, f32 arg2, f32 arg3, s32 arg4);

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

void hud_setup(u8 arg0, u8 arg1, u8 arg2, s32 numA, s32 numB, f32 arg5, f32 arg6, s32 arg7) {
    static s8 D_80110274;
    s32 i;
    s32 reinit;

    reinit = 0;
    if (D_8011023C) {
        reinit = 1;
        if (D_80146200 == arg1 && D_80149410 == arg2) {
            return;
        }
        func_800C8918();
    }
    D_80146200 = arg1;
    D_80149410 = arg2;
    for (i = 0; i < 128; i++) {
        D_80142DD8[i].active = 0;
    }
    if (!D_80110274) {
        D_80110274 = 1;
        osCreateMesgQueue(&D_80142728, D_80142768, 1);
        osSendMesg(&D_80142728, 0, 0);
    }
    car_shadow_render(arg0, arg1, arg2);
    if (!reinit) {
        osCreateThread(&D_80034F00, 10, func_800992AC, 0, D_800339F0, 10);
        osStartThread(&D_80034F00);
    }
    if (D_80146200 * 2 < 64) {
        D_80110264 = D_80146200 * 2;
    } else {
        D_80110264 = 64;
    }
    D_80110264 = (D_80146200 + numB < D_80110264) ? D_80110264 : D_80146200 + numB;
    D_80110244 = audio_dma_sync(0, D_80110264 * sizeof(Voice));
    audio_loop_control(D_80110244, 0);
    list_init(&D_80143FF8, 0, 1);
    list_init(&D_80144020, 0, 1);
    list_init(&D_80144C50, 0, 1);
    D_80110248 = audio_dma_sync(0, D_80110264 * 12);
    audio_loop_control(D_80110248, 0);
    D_80143F58.func = func_8009539C;
    D_80143F58.unk8 = 0;
    D_80143F58.unk4 = 0;
    D_8011024C = D_80110250 = 0;
    for (i = 0; i < D_80110264; i++) {
        D_80110244[i].index = i;
        func_80091FBC(&D_80143FF8, &D_80110244[i].link, D_80143FF8.head);
        D_80110244[i].owner = &D_80143FF8;
    }
    D_801460F8 = func_800C8738(D_80110264);
    if ((1 << D_801460F8) < D_80110264) {
        D_801460F8++;
    }
    D_80146104 = (1 << D_801460F8) - 1;
    D_80110270 = audio_dma_sync(0, 6 * sizeof(Chan));
    audio_loop_control(D_80110270, 0);
    list_init(&D_80149808, 0, 1);
    list_init(&D_80149860, 0, 1);
    for (i = 0; i < 6; i++) {
        D_80110270[i].index = i;
        D_80110270[i].unkC = 1;
        D_80110270[i].unkD = 0;
        func_80091FBC(&D_80149808, &D_80110270[i].link, D_80149808.head);
    }
    hud_speed_display(numA, numB, arg5, arg6, arg7);
    D_8011023C = 1;
}
