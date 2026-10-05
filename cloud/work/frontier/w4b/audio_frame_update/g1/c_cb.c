/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct SndSlot {         /* 0x18 bytes; player_array[p].snd[] starts at 0x110 */
    u8 pad00[4];
    s16 f4;                      /* 0x04 */
    s16 handle;                  /* 0x06 */
    s16 slot;                    /* 0x08 */
    s16 pad0A;
    s32 idx;                     /* 0x0C */
    f32 t;                       /* 0x10 */
    void *cb;                    /* 0x14 */
} SndSlot;
typedef struct CarS {            /* player_array[p], 0x3B8 bytes */
    u8 pad000[0x110];
    SndSlot snd[20];             /* 0x110 */
    u8 pad2F0[0x3B8 - 0x2F0];
} CarS;
typedef struct SndCtl {          /* D_80139320[player], 0x40 bytes */
    s32 first;
    u8 pad04[0x18];
    s32 vals[4];                 /* 0x1C */
    u8 pad2C[0x14];
} SndCtl;
typedef struct Rec18 {
    u8 *ptr;
    u8 pad[0x14];
} Rec18;

extern s32 state_word_a;
extern s32 gameplay_mode;
extern s8 D_80156994;
extern s8 D_8014978C;
extern f32 D_801543CC;
extern CarS player_array[];
extern SndCtl D_80139320[];
extern Rec18 D_8013FEF4[];
extern u16 D_8011B58C[];
extern u16 D_8011B5AC[];
extern s32 D_8008BEA4;

void save_load_data(s16 arg0);
void func_800AC8D4(s16 arg0);
void func_800B08FC(s16 slot, s16 idx);
void func_800B0A88(s16 slot, s16 idx);
void func_8038c910(s16);
void anim_state_update(void *arg0, s16 arg1);

void audio_frame_update(s16 slot) {
    u8 *dst;
    CarS *car;
    SndSlot *m;
    s16 i;
    SndCtl *ctl;
    s32 t;
    f32 ft;

    save_load_data(slot);
    func_800AC8D4(slot);
    t = state_word_a & 8;
    if (t == 0 || (t != 0 && D_80156994 != 0)) {
        func_800B0A88(slot, 0);
        func_800B0A88(slot, 1);
        func_800B08FC(slot, 0);
        func_800B08FC(slot, 1);
    }
    if (gameplay_mode == 6) {
        func_8038c910(slot);
    }
    dst = D_8013FEF4[(u32) slot].ptr;
    if (D_8014978C == 3 || D_8014978C == 5) {
        ((u16 *) (dst + 0x140))[0] = D_8011B58C[0];
        ((u16 *) (dst + 0x140))[1] = D_8011B58C[1];
        ((u16 *) (dst + 0x140))[2] = D_8011B58C[2];
        ((u16 *) (dst + 0x140))[3] = D_8011B58C[3];
        ((u16 *) (dst + 0x140))[4] = D_8011B58C[4];
        ((u16 *) (dst + 0x140))[5] = D_8011B58C[5];
        ((u16 *) (dst + 0x140))[6] = D_8011B58C[6];
        ((u16 *) (dst + 0x140))[7] = D_8011B58C[7];
        ((u16 *) (dst + 0x150))[0] = D_8011B5AC[0];
        ((u16 *) (dst + 0x150))[1] = D_8011B5AC[1];
        ((u16 *) (dst + 0x150))[2] = D_8011B5AC[2];
        ((u16 *) (dst + 0x150))[3] = D_8011B5AC[3];
        ((u16 *) (dst + 0x150))[4] = D_8011B5AC[4];
        ((u16 *) (dst + 0x150))[5] = D_8011B5AC[5];
        ((u16 *) (dst + 0x150))[6] = D_8011B5AC[6];
        ((u16 *) (dst + 0x150))[7] = D_8011B5AC[7];
    }
    car = &player_array[slot];
    m = (SndSlot *) ((u32) ((u8 *) car + 0x110));
    ctl = &D_80139320[slot];
    m->cb = (void *) &D_8008BEA4;
    m->slot = slot;
    ft = D_801543CC;
    i = 0;
    do {
        car->snd[6 + i].handle = ctl->vals[i];
        car->snd[6 + i].f4 = i;
        car->snd[6 + i].cb = (void *) anim_state_update;
        car->snd[6 + i].slot = slot;
        car->snd[6 + i].idx = 0;
        car->snd[6 + i].t = ft;
        i++;
    } while (i < 4);
}
