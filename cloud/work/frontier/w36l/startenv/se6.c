/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;

typedef union tRGBA { unsigned char c[4]; unsigned int rgba; } tRGBA;
typedef struct tVisual {
    struct tVisual *next;       /* 0x00 */
    s16 index;                  /* 0x04 */
    s16 objnum;                 /* 0x06 */
    s16 slot;                   /* 0x08 */
    u8 pad0A[2];
    void *data;                 /* 0x0C */
    float timeStamp;            /* 0x10 */
    void (*func)(struct tVisual *, s16);   /* 0x14 */
} tVisual;
typedef struct tCarData { u8 pad0[0xF8]; tVisual visuals[22]; u8 pad308[952 - 0x308]; } tCarData;
typedef struct tModelData { u8 pad0[0x7C8]; s16 in_game; u8 pad7CA[0x808 - 0x7CA]; } tModelData;
typedef struct tPoly { s16 nverts; u16 flags; u16 texidx; s16 z; u8 verts[80]; } tPoly;
typedef struct tLink { u8 pad0; u8 car; u8 pad2[5]; u8 owner; } tLink;

extern tCarData player_array[];         /* gGameCar */
extern tModelData D_8014A250[];         /* model */
extern f32 D_801543CC;                  /* gModelTime */
extern tRGBA D_8011AD8C;                /* gShadowColor */
extern f32 D_8011AD90[4][3];            /* gDummyVtx */
extern u16 D_80161368[];                /* gTexList */
extern tPoly D_8015B268[];              /* PolyInstanceList */
extern u32 D_801174B4;                  /* gGameState */
extern tRGBA D_8011B558[5];             /* arrowColors */
extern s8 D_8012E67C[];                 /* gCarTeam */
extern s32 D_8014A110;                  /* gRace */
extern s16 D_80151AD0;                  /* gNumWindows */
extern s16 active_player_count;         /* gNumPlayers */
extern tLink D_80153E88[];              /* gLink */

void music_seq_load(s16 slot, s16 camlink, int for_game);          /* CreateCar */
void audio_frame_update(s16 slot);                                  /* StartCar */
tPoly *func_800A78BC(int nverts, f32 (*xyz)[3], u16 texidx, tRGBA *color, u16 flags, int useTidx); /* ZOID_NewPoly */
void entity_process_main(tVisual *v, s16 op);                       /* AnimateShadow */
void entity_render_setup(tVisual *v, s16 op);                       /* AnimateArrow */

static void ZOID_SetPolyZ(tPoly *poly, s16 z)
{
    poly->z = z;
}

static void StartShadow(s16 slot)
{
    tVisual *v;
    int i;
    void *vv;
    void *shadow1;
    void *shadow2;
    void *shadow3;
    int u0, u1;
    v = &player_array[slot].visuals[11];
    v->func = entity_process_main;
    v->slot = slot;
    v->timeStamp = D_801543CC;
    D_8011AD8C.c[3] = 192;
    v->objnum = func_800A78BC(4, D_8011AD90, D_80161368[6], &D_8011AD8C, ((D_801174B4 & 0x100) ? 1 : 15) | 0x200 | 0x8000, 1) - D_8015B268;
}

static void StartArrow(s16 slot)
{
    tVisual *v = &player_array[slot].visuals[12];
    int i;
    int u2;
    tRGBA clr = { { 255, 255, 255, 255 } };
    tPoly *poly;
    if (D_8014A110 == 6) {
        clr.c[0] = D_8011B558[D_8012E67C[slot]].c[0];
        clr.c[1] = D_8011B558[D_8012E67C[slot]].c[1];
        clr.c[2] = D_8011B558[D_8012E67C[slot]].c[2];
        clr.c[3] = D_8011B558[D_8012E67C[slot]].c[3];
    } else {
        clr.c[0] = D_8011B558[slot].c[0];
        clr.c[1] = D_8011B558[slot].c[1];
        clr.c[2] = D_8011B558[slot].c[2];
        clr.c[3] = D_8011B558[slot].c[3];
    }
    for (i = 0; i < D_80151AD0; i++) {
        if (i == slot) continue;
        {
            v->func = entity_render_setup;
            v->slot = slot;
            v->index = i;
            if (D_8014A110 == 6) poly = func_800A78BC(4, D_8011AD90, D_80161368[7], &clr, (1 << i) | 0x3200, 1);
            else poly = func_800A78BC(4, D_8011AD90, D_80161368[7], &clr, (1 << i) | 0x1200, 1);
            ZOID_SetPolyZ(poly, 1);
            v->objnum = poly - D_8015B268;
            v++;
        }
    }
    while (v < &player_array[slot].visuals[14]) {
        v->func = 0;
        v->objnum = -1;
        v++;
    }
}

void music_control(void)
{
    s16 i;
    for (i = 0; i < 6; i++) {
        if (D_8014A250[i].in_game) {
            music_seq_load(i, 15, 1);
            audio_frame_update(i);
        }
    }
    for (i = 0; i < 6; i++) {
        if (D_8014A250[i].in_game) {
            StartShadow(i);
            if (active_player_count > 1 && D_80153E88[i].owner == 6) StartArrow(i);
        }
    }
}
