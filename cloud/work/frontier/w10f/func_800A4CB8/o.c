/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NOT A MATCH: scorer 98/102 (prologue shifted by one instruction); aligned diff 53 rows, almost all
 * one-off temp numbers (t7 for t6 ...) caused by a single allocation difference.
 * func_800A4CB8 @ 0x800A4CB8: audio list/pool init. Two List headers via the hud_speed_display
 * list_init helper (inlined, store order +1,+0,+8,+0xC,+4 proven), D_801460F4 = count, 24-byte pool
 * D_80144C48 and 32-byte x 3*count pool D_80146100 (audio_dma_sync / audio_loop_control / memset), each
 * 32-byte record pushed with func_80091FBC(&D_80146138, rec, D_80146138.head), D_80144DB8 = count,
 * D_801460C8[0..4] = 0 (peeled + unrolled), D_8011028C = 0, D_80110284 = 1.
 * Recovered: D_8011028C is `volatile` (lui/addiu + sb 0(reg), as in every other reader).
 * Residual: retail keeps `count` in a caller-saved register (move a2,a0; sw a2,48(sp) before the
 * first jal); ours leaves it in its home slot and reloads (lw $14,48($sp) pre-as1), which shifts the
 * temp ring by one for the rest of the function.
 */
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
    if (count <= 0) {
    }
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
