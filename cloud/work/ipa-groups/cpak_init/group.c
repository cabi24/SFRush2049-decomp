/*
 * Per-wheel tyre effects (skid marks): four slots per player, each holding a
 * pooled display object. Not Controller Pak code (labels are historical).
 * Hand-written from the assembly (cloud Lane A).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

float sqrtf(float);
#pragma intrinsic (sqrtf)

typedef struct EffectObj {
    /* 0x00 */ struct EffectObj *next;   /* pool list link */
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ f32 f8;
    /* 0x0C */ void *dl;                 /* display handle */
    /* 0x10 */ f32 pos[3];
    /* 0x1C */ u8 color[4];
    /* 0x20 */ s32 f20;
} EffectObj;

typedef struct {
    /* 0x00 */ EffectObj *obj;
    /* 0x04 */ f32 start[3];
    /* 0x10 */ f32 pos[3];
    /* 0x1C */ f32 lenSq;
    /* 0x20 */ f32 dir[3];
    /* 0x2C */ f32 quad[6];             /* 0x2C..0x40: previous edge */
    /* 0x44 */ f32 left[3];
    /* 0x50 */ f32 right[3];
} WheelSlot;                            /* 0x5C */

typedef struct {
    /* 0x000 */ u8 pad0[0x74];
    /* 0x074 */ f32 wheelPos[4][3];
    /* 0x0A4 */ u8 padA4[0xE8 - 0xA4];
    /* 0x0E8 */ u32 wheelFlags;
    /* 0x0EC */ u8 padEC[0x3B8 - 0xEC];
} Car;                                  /* 0x3B8 */

typedef struct {
    /* 0x000 */ u8 pad0[0x5A0];
    /* 0x5A0 */ u16 surface[4];
    /* 0x5A8 */ u8 pad5A8[0x61C - 0x5A8];
    /* 0x61C */ u16 terrain[4];
    /* 0x624 */ u8 pad624[0x6C4 - 0x624];
    /* 0x6C4 */ s16 airborne[4];
    /* 0x6CC */ u8 pad6CC[0x75C - 0x6CC];
    /* 0x75C */ f32 wheelHeight[4];
    /* 0x76C */ u8 pad76C[0x808 - 0x76C];
} CarState;                             /* 0x808 */

typedef struct {
    /* 0x0 */ u8 pad0[2];
    /* 0x2 */ u16 flags;
    /* 0x4 */ u8 pad4[0x18 - 4];
} Surface;                              /* 0x18 */

extern Car player_array[8];
extern CarState D_8014A250[];
extern WheelSlot D_80155290[][4];
extern s16 active_player_count;
extern s32 state_word_a;
extern u32 D_8011743C[4];               /* per-wheel flag masks */
extern u8 D_80117438[];                 /* default mark colour */
extern u8 D_8011AD8C[];                 /* colour on terrain 0 */
extern f32 D_8011AD90[];
extern Surface *D_801497F8;
extern f32 D_80123C08;
extern f32 D_80123C0C;                  /* minimum travel before a mark starts */
extern f32 D_8002EB90[];
extern char D_80155220[];               /* EffectObj pool */
extern f32 D_80150B94[3];               /* camera position */
extern u16 D_80161378;

extern EffectObj *func_8008E3C0(void *pool);
extern void func_800AFA84(void *pool, EffectObj *obj);
extern void func_8008D0C0(void *dl);
extern void *func_800A78BC(s32, f32 *, u16, u8 *, s32, s32);
extern void func_8008C074(void *dl, s32 n, f32 *quad, s32, u8 *color, s32, s32);
extern void vector_copy_scale(f32 *in, f32 *out);
extern void func_800AF844(WheelSlot *slot);

void cpak_init(s16 player);
void func_800AF8C0(WheelSlot *slot, s16 player, s32 wheel, u8 *color);
void save_validate(s16 player, s32 wheel, u8 *color, WheelSlot *slot);

void cpak_init(s16 player)
{
    WheelSlot *slot;
    CarState *state;
    Car *car;
    u32 *mask;
    u8 *color;
    s32 i;
    s32 active;
    s32 has;
    f32 minLen;
    f32 d[3];
    f32 v[3];
    f32 lenSq;
    f32 len;
    f32 inv;

    if (active_player_count >= 3) {
        return;
    }
    slot = D_80155290[player];
    state = &D_8014A250[player];
    minLen = D_80123C0C;
    mask = D_8011743C;
    car = &player_array[player];
    for (i = 0; i < 4; i++) {
        color = D_80117438;
        active = (mask[i] & car->wheelFlags) != 0;
        if (active) {
            active = state->airborne[i] == -1;
            if (active) {
                active = state->terrain[i] != 8;
                if (active) {
                    active = (D_801497F8[state->surface[i]].flags & 0x30) == 0;
                }
            }
        }
        has = slot[i].obj != 0;
        if (state->terrain[i] == 0) {
            color = D_8011AD8C;
        }
        if (!has && active) {
            save_validate(player, i, color, &slot[i]);
        } else if (has && active) {
            func_800AF8C0(&slot[i], player, i, color);
            d[0] = car->wheelPos[i][0] - slot[i].start[0];
            d[1] = car->wheelPos[i][1] - slot[i].start[1];
            d[2] = car->wheelPos[i][2] - slot[i].start[2];
            lenSq = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
            if (slot[i].lenSq > 0.0f) {
                len = sqrtf(lenSq);
                v[0] = slot[i].dir[0] * len;
                v[1] = slot[i].dir[1] * len;
                v[2] = slot[i].dir[2] * len;
                d[0] = slot[i].pos[0] - v[0];
                d[1] = slot[i].pos[1] - v[1];
                d[2] = slot[i].pos[2] - v[2];
                if (d[0] * d[0] + d[1] * d[1] + d[2] * d[2] > 1.0f || lenSq < slot[i].lenSq) {
                    func_800AF844(&slot[i]);
                    save_validate(player, i, color, &slot[i]);
                }
            } else if (minLen < lenSq) {
                inv = 1.0f / sqrtf(lenSq);
                slot[i].dir[0] = d[0] * inv;
                slot[i].dir[1] = d[1] * inv;
                slot[i].lenSq = lenSq;
                slot[i].dir[2] = d[2] * inv;
            }
        } else if (has && !active) {
            func_800AF844(&slot[i]);
        }
    }
}

/* place the slot's leading edge across wheels (wheel|1) and (wheel&2) */
void func_800AF8C0(WheelSlot *slot, s16 player, s32 wheel, u8 *color)
{
    f32 axle[3];
    f32 across[3];
    Car *car;
    EffectObj *obj;
    s32 a;
    s32 b;
    s16 k;
    f32 half;
    f32 p;
    f32 lift;

    a = wheel | 1;
    b = wheel & 2;
    car = &player_array[player];
    obj = slot->obj;
    axle[0] = car->wheelPos[a][0] - car->wheelPos[b][0];
    axle[1] = car->wheelPos[a][1] - car->wheelPos[b][1];
    axle[2] = car->wheelPos[a][2] - car->wheelPos[b][2];
    axle[1] += D_8014A250[player].wheelHeight[a] - D_8014A250[player].wheelHeight[b];
    vector_copy_scale(axle, across);
    for (k = 0; k < 3; k++) {
        half = across[k] * 0.75f;
        slot->pos[k] = car->wheelPos[wheel][k];
        p = slot->pos[k];
        slot->left[k] = p - half;
        slot->right[k] = p + half;
    }
    lift = D_80123C08;
    slot->left[1] += lift;
    slot->right[1] += lift;
    obj->f8 = D_8002EB90[0];
    obj->color[0] = color[0];
    obj->color[1] = color[1];
    obj->color[3] = 0xC0;
    obj->color[2] = color[2];
    func_8008C074(obj->dl, 4, slot->quad, 0, obj->color, 0, 0);
}

/* start a new mark in `slot`, stealing the farthest pooled object if needed */
void save_validate(s16 player, s32 wheel, u8 *color, WheelSlot *slot)
{
    EffectObj *obj;
    EffectObj *far;
    EffectObj *p;
    f32 best;
    f32 dist;
    f32 t;
    s32 k;

    obj = func_8008E3C0(D_80155220);
    if (obj == 0) {
        best = -1.0f;
        far = p = ((EffectObj **) D_80155220)[4];
        for (; p != 0; p = p->next) {
            dist = 0.0f;
            for (k = 0; k < 3; k++) {
                t = p->pos[k] - D_80150B94[k];
                dist += t * t;
            }
            if (best < dist) {
                best = dist;
                far = p;
            }
        }
        func_8008D0C0(far->dl);
        func_800AFA84(D_80155220, far);
        obj = func_8008E3C0(D_80155220);
    }
    obj->f20 = 0;
    obj->color[0] = color[0];
    obj->color[1] = color[1];
    obj->color[3] = 0xC0;
    obj->color[2] = color[2];
    obj->dl = func_800A78BC(4, D_8011AD90, D_80161378, color,
                            ((state_word_a & 0x100) ? 1 : 15) | 0x1200, 1);
    if (obj->dl == 0) {
        func_800AFA84(D_80155220, obj);
        obj = 0;
    }
    if (obj != 0) {
        slot->obj = obj;
        slot->lenSq = 0.0f;
        func_800AF8C0(slot, player, wheel, color);
        slot->start[0] = slot->pos[0];
        slot->start[1] = slot->pos[1];
        slot->start[2] = slot->pos[2];
        slot->quad[1] = slot->right[1];
        slot->quad[0] = slot->right[0];
        slot->quad[2] = slot->right[2];
        slot->quad[3] = slot->left[0];
        slot->quad[4] = slot->left[1];
        slot->quad[5] = slot->left[2];
        func_8008C074(obj->dl, 4, slot->quad, 0, 0, 0, 0);
    }
}

/* stand-in caller: keeps func_800AF8C0 out of line under -O3 */
void __standin_func_800AF8C0(void)
{
    func_800AF8C0(0, 0, 0, 0);
}

/* stand-in caller: keeps save_validate out of line under -O3 */
void __standin_save_validate(void)
{
    save_validate(0, 0, 0, 0);
}
