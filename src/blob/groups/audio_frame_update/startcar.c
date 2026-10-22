/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * audio_frame_update (0x800B0C48, 600 B) = MAT3 StartCar@visuals.c (GC/DC names; w36n map row), N64 version:
 *   StartFrame (save_load_data), StartSparks (func_800AC8D4), StartWing x2 (func_800B0A88, IPA s1/s2) and
 *   StartWingFlame x2 (func_800B08FC, IPA t0/t3) unless demo flag 8 is set without D_80156994, StartWeapon
 *   (image B 0x8038C910) when gRace == 6, then the inlined statics StartHeadLights (N64: copy the two 8 x u16
 *   head-light UV blocks D_8011B58C / D_8011B5AC into the car model data at +0x140 / +0x150 on tracks 3 and 5),
 *   StartBrakeLights (visuals[0]: AnimateBrakeLights func_8008BEA4, slot) and the four StartTire calls
 *   (visuals[tire + 6]: AnimateTire anim_state_update, slot, data 0, index, gModelTime, gCarParts[slot].tires[tire]).
 * Levers (w38a, from the w17i/w26e draft at 25-27 words): MAT3 structure with static helpers; the helpers are
 *   defined AFTER StartCar (MAT3 file order: StartCar 615 < StartTire 1706 < StartBrakeLights 2323): as1 breaks the
 *   ready-list tie between the brake-light stores and the hoisted tire-loop invariants (which carry the for-loop
 *   line) by source line (6 words with the helpers first).  AnimateTire / AnimateBrakeLights / D_8011B5AC are
 *   referenced only here (no out-of-line copies), so the helpers are deleted statics; their stubs land before
 *   func_800B08FC in the group object, as retail's four stubs func_800B08DC..F4 do (the fourth is another static).
 * DISCLOSED SHAPING: StartBrakeLights computes its visual as `(tVisual *)((u32)&player_array[slot] + 0x110)`: retail
 *   shares `&gGameCar[slot]` (a2) with the tire loop and keeps visuals[0] as a2 + 272 in v0; `&gGameCar[slot].visuals[0]`,
 *   `.visuals`, a car pointer local, a (u8 *) offset or an inline getter all fold the 272 into the store offsets and
 *   recompute slot * 952 (26-32 words).  The integer form is the only one found.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct tVisual {         /* 0x18 bytes; gGameCar[slot].visuals[] at 0x110 */
    u8 pad00[4];
    s16 index;                   /* 0x04 */
    s16 objnum;                  /* 0x06 */
    s16 slot;                    /* 0x08 */
    s16 pad0A;
    s32 data;                    /* 0x0C */
    f32 timeStamp;               /* 0x10 */
    void (*func)(struct tVisual *, s16);   /* 0x14 */
} tVisual;
typedef struct tCarData {        /* gGameCar[slot], 0x3B8 bytes */
    u8 pad000[0x110];
    tVisual visuals[20];         /* 0x110 */
    u8 pad2F0[0x3B8 - 0x2F0];
} tCarData;
typedef struct tCarParts {       /* gCarParts[slot], 0x40 bytes */
    s32 frame;
    u8 pad04[0x18];
    s32 tires[4];                /* 0x1C */
    u8 pad2C[0x14];
} tCarParts;
typedef struct { u16 uv[8]; } tUVPair;
typedef struct { u8 *data; u8 pad[0x14]; } tCarModel;

extern s32 state_word_a;
extern s32 gameplay_mode;            /* gRace */
extern s8 D_80156994;
extern s8 D_8014978C;            /* gTrackNum */
extern f32 D_801543CC;           /* gModelTime */
extern tCarData player_array[];    /* gGameCar */
extern tCarParts D_80139320[];   /* gCarParts */
extern tCarModel D_8013FEF4[];
extern tUVPair D_8011B58C, D_8011B5AC;

void save_load_data(s16 slot);   /* StartFrame */
void func_800AC8D4(s16 slot);    /* StartSparks */
void func_800B08FC(s16 slot, s16 side);   /* StartWingFlame */
void func_800B0A88(s16 slot, s16 side);   /* StartWing */
void func_8038c910(s16 slot);    /* StartWeapon (image B) */
void func_8008BEA4(tVisual *v, s16 op);   /* AnimateBrakeLights */
void anim_state_update(tVisual *v, s16 op);   /* AnimateTire */

static void StartHeadLights(s16 slot);
static void StartTire(s16 slot, u32 tire);
static void StartBrakeLights(s16 slot);

void audio_frame_update(s16 slot)
{
    s16 j;
    s32 t;

    save_load_data(slot);
    func_800AC8D4(slot);
    t = state_word_a & 8;
    if (t == 0 || (t != 0 && D_80156994 != 0)) {
        func_800B0A88(slot, 0);
        func_800B0A88(slot, 1);
        func_800B08FC(slot, 0);
        func_800B08FC(slot, 1);
    }
    if (gameplay_mode == 6) func_8038c910(slot);
    StartHeadLights(slot);
    StartBrakeLights(slot);
    for (j = 0; j < 4; j++) StartTire(slot, j);
}

static void StartHeadLights(s16 slot)
{
    u8 *p = D_8013FEF4[slot].data;

    if (D_8014978C == 3 || D_8014978C == 5) {
        ((u16 *)(p + 0x140))[0] = D_8011B58C.uv[0];
        ((u16 *)(p + 0x140))[1] = D_8011B58C.uv[1];
        ((u16 *)(p + 0x140))[2] = D_8011B58C.uv[2];
        ((u16 *)(p + 0x140))[3] = D_8011B58C.uv[3];
        ((u16 *)(p + 0x140))[4] = D_8011B58C.uv[4];
        ((u16 *)(p + 0x140))[5] = D_8011B58C.uv[5];
        ((u16 *)(p + 0x140))[6] = D_8011B58C.uv[6];
        ((u16 *)(p + 0x140))[7] = D_8011B58C.uv[7];
        ((u16 *)(p + 0x150))[0] = D_8011B5AC.uv[0];
        ((u16 *)(p + 0x150))[1] = D_8011B5AC.uv[1];
        ((u16 *)(p + 0x150))[2] = D_8011B5AC.uv[2];
        ((u16 *)(p + 0x150))[3] = D_8011B5AC.uv[3];
        ((u16 *)(p + 0x150))[4] = D_8011B5AC.uv[4];
        ((u16 *)(p + 0x150))[5] = D_8011B5AC.uv[5];
        ((u16 *)(p + 0x150))[6] = D_8011B5AC.uv[6];
        ((u16 *)(p + 0x150))[7] = D_8011B5AC.uv[7];
    }
}

static void StartTire(s16 slot, u32 tire)
{
    tVisual *v;
    v = &player_array[slot].visuals[tire + 6];
    v->func = anim_state_update;
    v->slot = slot;
    v->data = 0;
    v->index = tire;
    v->timeStamp = D_801543CC;
    v->objnum = D_80139320[slot].tires[tire];
}

static void StartBrakeLights(s16 slot)
{
    tVisual *v;
    v = (tVisual *)((u32)&player_array[slot] + 0x110);
    v->func = func_8008BEA4;
    v->slot = slot;
}

