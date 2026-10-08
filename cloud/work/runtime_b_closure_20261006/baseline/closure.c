/* flags: -g0 -O3 -mips2 -G 0 -non_shared */

/* One approved genuine-closure diagnostic. Research only, no original-TU claim. */

/* PROPOSED RESEARCH SCHEMA ONLY. Not compiled or approved as an original TU.
 * Every named field below is consumed by one of the eight genuine bodies.
 * Unknown ranges preserve established strides; they are not pressure fields.
 * See compatibility.md for source views, aliases and unresolved contracts.
 */
#ifndef RUNTIME_B_CLOSURE_PROPOSED_H
#define RUNTIME_B_CLOSURE_PROPOSED_H
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct BObject {
    struct BObject *next;
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
    u32 unknown38;
} BObject;                         /* 0x3C */
typedef struct BAttachedEffect {
    u8 unknown00[8];
    u32 flags;
    u32 unknown0C;
    f32 position[3];
} BAttachedEffect;                 /* 0x1C CONSUMED PREFIX, not allocation size */
typedef struct BStatusEffect {
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
} BStatusEffect;                   /* 0x34; F938 global stride */
typedef struct BRecord {
    struct BRecord *next;
    s8 owner;
    s8 hit_mask;                    /* +5: native signed byte in D498 */
    s8 kind;
    s8 flags;
    f32 lifetime;
    f32 timer;
    f32 collision_extent;           /* +0x10: former elapsed/radius labels */
    f32 velocity[3];
    f32 position[3];
    f32 previous_position[3];
    f32 uv[3][3];
    BAttachedEffect *attached_effect;
    BObject *primary;
    BObject *secondary;
} BRecord;                         /* 0x68 */
typedef struct BDebris {
    struct BDebris *next;
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
    f32 lifetime;
    f32 velocity[3];
} BDebris;                         /* 0x48 */
typedef struct BInput {
    u32 unknown00;
    u32 pressed;
    u32 held;
    u8 unknown0C[0x30-0x0C];
    u32 reset_mask;
    u8 unknown34[8];
    u32 fire_mask;
} BInput;                          /* 0x40 consumed view */
typedef struct BStatus {
    u32 flags;
    f32 transition_timer;
    f32 scale_timer;
    f32 scale;
    f32 pitch;
} BStatus;                         /* 0x14 */
typedef struct BPlayer {
    u8 unknown00[8];
    f32 position[3];
    f32 velocity[3];
    u8 unknown20[12];
    f32 uv[3][3];
    u8 unknown50[0x308-0x50];
    s8 active;
    u8 unknown309[0x34C-0x309];
    u32 color;
    u8 unknown350[0x359-0x350];
    s8 blocked;
    u8 unknown35A;
    s8 owner;
    u8 unknown35C[0x380-0x35C];
    BInput *input;
    s8 kind;
    s8 ammo;
    u8 unknown386[6];
    BStatus status;
    s8 action;
    u8 alpha;
    s8 transition;
    u8 unknown3A3;
    s32 latched;
    f32 transition_time;
    f32 cooldown;
    f32 pitch;
    f32 yaw;
} BPlayer;                         /* 0x3B8 */
typedef struct BVehicle {
    u8 unknown00[8];
    u8 model;
    u8 unknown09[0xFC-9];
    f32 extent_fc;
    u8 unknown100[8];
    f32 extent_108;
    u8 unknown10C[0x124-0x10C];
    f32 impulse[3];
    u8 unknown130[0x13C-0x130];
    f32 horizontal_impulse[3];
    u8 unknown148[0x640-0x148];
    s8 blocked;
    u8 unknown641[0x654-0x641];
    f32 radius;
    u8 unknown658[0x6C4-0x658];
    s16 state;
    u8 unknown6C6[0x808-0x6C6];
} BVehicle;                        /* 0x808 */
typedef struct BQuad BQuad;         /* A78BC returns a record pointer */
typedef struct BNameEntry BNameEntry;
typedef struct BRelease {
    struct BRelease *next;
    BQuad *quad;
} BRelease;                        /* +4 pointer, 0x08 consumed view */
typedef struct BPool {
    u8 header[16];
    void *head;
} BPool;                           /* 0x14 consumed prefix, not allocation size */
typedef struct BScene {
    u8 unknown00[12];
    f32 scale;
    u8 unknown10[0x44-0x10];
} BScene;                          /* 0x44 */
typedef struct BTexture { u8 bytes[36]; } BTexture;
typedef struct BTextureBank { BTexture *textures; u32 unknown04; } BTextureBank;
typedef union BColor { u32 word; u8 rgba[4]; } BColor;

/* Keep address-spelled symbols distinct. The linker/memory model records
 * byte aliases; it must not infer original C object identity from adjacency.
 */
extern BDebris *D_8039AE90;           /* aliases D_8039AE80 + 0x10 */
extern BRecord *D_8039A530;           /* aliases D_8039A520 + 0x10 */
extern BPool D_8039AE80, D_8039A520, D_8039A5F8, D_80394F70;
extern BPlayer D_80152818[];
extern BVehicle D_8014A250[];
extern BScene D_8012E700[];
extern BTextureBank D_80151AE8[];
extern BStatusEffect D_80394F90[4];
extern BColor D_8039438C;
extern s16 D_801543CA;
extern f32 D_8002EB94, D_80142764;
extern f32 D_8011F844[][4];
extern f32 D_80394358[13];
extern f32 D_803943A4[8][13][3];
extern f32 D_80394AFC[3], D_80394B08[][3];
extern f32 D_8011418C[3][3], D_801141B0[3][3];
extern s32 D_803942C0[];
extern s32 D_80399B18[];             /* indexed base; original extent unproven */
extern s32 D_80399B54;               /* physical byte alias of base + 15*4 */
extern s8 D_8012E67C[];
extern u16 D_80399AF8[], D_80394F88;
extern u8 D_80140BDC;
extern u32 D_80394390, D_803942E8;
extern u8 D_80394CFC[], D_80394D08[];

/* Actual external ABI data, using coherent logical pointee views. */
extern void math_utility(f32 source[3][3], f32 destination[3][3]);
extern void func_80090F44(f32 angle, f32 uv[3][3]);
extern void func_80090E9C(f32 angle, f32 uv[3][3]);
extern void func_8009EA68(f32 scale, f32 uv[3][3]);
extern f32 func_8008B2E4(f32 limit);
extern f32 func_8008C768(f32 y, f32 x);
extern void func_800A61B0(f32 source[3], f32 dest[3], f32 basis[3][3]);
extern void func_8009E820(f32 source[3], f32 dest[3], f32 basis[3][3]);
extern void func_8008B32C(f32 source[3][3], f32 dest[3][3], f32 scale);
extern void *func_8008E3C0(BPool *pool);
extern void func_800AFA84(BPool *pool, void *object);
extern void func_80090254(s16 scene_index);
extern s32 func_8008E398(s32 resource, f32 uv[3][3], s32 parent, u32 flags);
extern void func_8008E06C(s16 scene_index, u32 *color);
extern void func_8008D870(s16 scene_index, BTexture *texture, s32 index);
extern s32 func_800B61A8(s32 sound, s32 owner, s32 arg2, u8 arg3);
extern void func_8008D0C0(s16 *quad_active); /* call with (s16 *)release->quad */
extern BNameEntry *func_800B24EC(char *name, s16 *resource, s8 lo, s8 hi, s32 err);
extern BQuad *func_800A78BC(s32 count, f32 corners[4][3], u16 resource,
                           u32 *color, u16 flags, s32 indexed);
extern void func_8038F568(BPlayer *player);
extern BAttachedEffect *func_8038D054(s32 mode, f32 position[3], f32 homed_position[3]);
extern void func_8038CB20(void);
extern void func_8038DDDC(BRecord *record);
extern void func_8038D3A4(BPlayer *owner, BPlayer *target, s32 damage);
extern void func_8038D798(f32 position[3], f32 previous[3], s32 owner,
                         f32 strength, s32 damage);
extern s32 func_800ADD58(f32 previous[3], f32 position[3], f32 basis[3][3],
                        f32 extent, s32 mode, u32 mask);
extern void func_800ABCC8(void *effect, s32 mode, f32 position[3], s32 arg3,
                         s32 owner, s32 arg5, s32 arg6);
extern void func_800AF06C(f32 position[3], s32 mode, f32 scale, s32 arg3);
extern void func_800AEFE0(f32 position[3], f32 basis[3][3], f32 strength,
                         f32 arg3, f32 arg4, f32 arg5, s32 sound,
                         s32 owner, s32 arg8, s32 flags);
extern f32 fabsf(f32 value);
extern f32 sqrtf(f32 value);
#pragma intrinsic(fabsf, sqrtf)

/* Proposed private visibility, not recovered original signatures/order.
 * No body is supplied here; approved assembly must use every real body.
 * FCE0 is the sole externally kept genuine root, never E114.
 */
static void func_8038D200(BRecord *record, s32 mode);
static BObject *func_8038D328(s32 resource, s32 parent, u32 flags, s32 omit_uv);
static void func_8038D498(BRecord *record);
static BPlayer *func_8038DA78(BRecord *record, f32 previous[3]);
static void func_8038E088(BRecord *record);
static void func_8038E114(void);
static void func_8038F938(BStatus *status, BPlayer *player, BVehicle *vehicle, s32 owner);
void func_8038FCE0(void);
#endif


/* Complete D200 body; source-preserving approved reconciliation only. */
static void func_8038D200(BRecord *record, s32 mode)
{
    BObject *object;
    s32 flags;
    f32 scale;

    object = record->primary;
    object->position[0] = record->position[0];
    object->position[1] = record->position[1];
    object->position[2] = record->position[2];
    if (mode == 1) {
        math_utility(D_8011418C, object->uv);
    } else if (mode != 2) {
        math_utility(record->uv, object->uv);
    }
    flags = record->flags;
    if (flags & 0x70) {
        if (((flags & 0x80) && (flags & 0x10) &&
             (record->kind == 1 || record->kind == 8)) ||
            record->kind == 0) {
            if (flags & 0x80) {
                scale = 0.2f;
            } else {
                scale = 0.4f;
            }
            object->uv[2][0] *= scale;
            object->uv[2][1] *= scale;
            object->uv[2][2] *= scale;
        }
    }
    if (record->flags & 2) {
        func_80090F44(0.18f, object->uv);
    }
}

/* Complete D328 body; source-preserving approved reconciliation only. */
static BObject *func_8038D328(s32 resource_index, s32 parent,
                      u32 flags, s32 omit_transform)
{
    BObject *object;
    object = func_8008E3C0(&D_80394F70);
    if (omit_transform) {
        object->scene_index = func_8008E398(D_80399B18[resource_index],
                                          0, parent, flags);
    } else {
        object->scene_index = func_8008E398(D_80399B18[resource_index],
                                          object->uv, parent, flags);
    }
    return object;
}

/* Complete D498 body; source-preserving approved reconciliation only. */
static void func_8038D498(BRecord *record)
{
    BPlayer *owner;
    BPlayer *player;
    BVehicle *vehicle;
    f32 scale;
    f32 strength;
    f32 center[3];
    f32 delta[3];
    f32 impulse[3];
    f32 distance_squared;
    f32 distance;
    f32 radius;
    s32 damage;
    s32 i;
    s32 j;

    owner = &D_80152818[record->owner];
    scale = D_8012E700[record->primary->scene_index].scale;
    if (scale <= 8.0f) {
        strength = 1.0f;
        damage = 800;
    } else if (scale <= 20.0f) {
        strength = 0.6f;
        damage = 400;
    } else {
        strength = 0.5f;
        damage = 200;
    }
    for (j = 0; j < 3; j++) center[j] = record->primary->position[j];
    for (i = 0, player = D_80152818; i < D_801543CA; i++, player++) {
        if (player->owner == record->owner) continue;
        if (D_8014A250[i].state != -1) continue;
        if (!player->active) continue;
        if ((u32)(s32)record->hit_mask & (1u << (player->owner & 31))) continue;
        for (j = 0; j < 3; j++) delta[j] = player->position[j] - center[j];
        distance_squared = delta[2] * delta[2] +
                           (delta[0] * delta[0] + delta[1] * delta[1]);
        radius = scale * 4.0f;
        if (distance_squared <= radius * radius) {
            record->hit_mask |= 1u << (player->owner & 31);
            func_8038D3A4(owner, player, damage);
            distance = sqrtf(distance_squared);
            impulse[2] = ((delta[2] / distance) * 330000.0f) * strength;
            impulse[1] = ((delta[1] / distance) * 330000.0f + 66000.0f) * strength;
            impulse[0] = ((delta[0] / distance) * 330000.0f) * strength;
            func_800A61B0(impulse, delta, player->uv);
            vehicle = &D_8014A250[player->owner];
            for (j = 0; j < 3; j++) vehicle->impulse[j] = delta[j] + vehicle->impulse[j];
            delta[1] = 0.0f;
            for (j = 0; j < 3; j++)
                vehicle->horizontal_impulse[j] = delta[j] + vehicle->horizontal_impulse[j];
        }
    }
}

/* Complete DA78 body; source-preserving approved reconciliation only. */
static BPlayer *func_8038DA78(BRecord *record, f32 previous[3])
{
    f32 delta[3];
    f32 normal[3];
    f32 relative[3];
    f32 point[3];
    f32 offset[3];
    f32 reverse[3];
    f32 denominator;
    f32 fraction;
    f32 nearest;
    f32 radius;
    BPlayer *result;
    BPlayer *player;
    s32 i;
    s32 j;

    if (record->kind == 3) return 0;
    for (j = 0; j < 3; j++) delta[j] = record->position[j] - previous[j];
    normal[0] = -delta[0];
    normal[1] = 0.0f;
    normal[2] = -delta[2];
    denominator = normal[2] * delta[2] +
                  (delta[0] * normal[0] + delta[1] * normal[1]);
    if (fabsf(denominator) < 0.001f) return 0;
    nearest = 1.0f;
    result = 0;
    for (i = 0, player = D_80152818; i < D_801543CA; i++, player++) {
        if (player->owner == record->owner) continue;
        if (D_8014A250[i].state != -1) continue;
        if (!player->active) continue;
        if (D_8014A250[i].blocked) continue;
        for (j = 0; j < 3; j++) relative[j] = player->position[j] - previous[j];
        fraction = (normal[2] * relative[2] +
                   (relative[0] * normal[0] + relative[1] * normal[1])) / denominator;
        if (fraction < 0.0f || nearest < fraction) continue;
        for (j = 0; j < 3; j++) point[j] = delta[j] * fraction + previous[j];
        for (j = 0; j < 3; j++) offset[j] = point[j] - player->position[j];
        radius = record->collision_extent;
        if (offset[1] < -radius || 3.5f + radius < offset[1]) continue;
        if ((radius + 6.75f) - 0.5f <
            sqrtf(offset[2] * offset[2] + offset[0] * offset[0])) continue;
        if (player->kind == 5) {
            for (j = 0; j < 3; j++) reverse[j] = previous[j] - record->position[j];
            func_800A61B0(reverse, offset, player->uv);
            if (fabsf(func_8008C768(offset[0], offset[2])) < 1.35f)
                record->flags |= 1;
        }
        result = player;
        nearest = fraction;
        for (j = 0; j < 3; j++) record->position[j] = point[j];
    }
    return result;
}

/* Complete E088 body; source-preserving approved reconciliation only. */
static void func_8038E088(BRecord *record)
{
    BObject *object;
    switch ((u8) record->kind) {
    case 3:
        object = record->secondary;
        func_80090254((s16) object->scene_index);
        func_800AFA84(&D_80394F70, object);
        /* The primary pointer is reread after both external calls. */
    case 0:
    case 1:
    case 2:
    case 4:
    case 6:
    case 7:
    case 8:
        object = record->primary;
        func_80090254((s16) object->scene_index);
        func_800AFA84(&D_80394F70, object);
        break;
    }
}

/* Complete E114 body; source-preserving approved reconciliation only. */
static void func_8038E114(void)
{
    BRecord *record;
    BRecord *next;
    BPlayer *player;
    BPlayer *hit;
    BVehicle *vehicle;
    BObject *object;
    BRelease *release;
    BInput *input;
    f32 previous[3];
    f32 offset[3];
    f32 local[3];
    f32 side[3];
    f32 basis[3][3];
    f32 corners[4][3];
    f32 extent;
    f32 radius; /* Kind 7 never defines or consumes this lazy value. */
    f32 distance;
    u32 color;
    u16 texture;
    s32 damage;
    s32 kind;
    s32 i;

    record = D_8039A530;
    color = D_80394390;
    while (record != 0) {
        next = record->next;
        if (record->kind == 3 && record->timer == 0.0f) {
            player = &D_80152818[record->owner];
            vehicle = &D_8014A250[record->owner];
            for (i = 0; i < 3; i++) {
                record->position[i] = player->position[i];
                offset[i] = D_803943A4[3][vehicle->model][i] + D_80394B08[3][i];
            }
            for (i = 0; i < 3; i++) record->position[i] += offset[2] * player->uv[2][i];
            for (i = 0; i < 3; i++) record->position[i] += offset[1] * player->uv[1][i];
            for (i = 0; i < 3; i++) record->position[i] += offset[0] * player->uv[0][i];
            math_utility(player->uv, record->uv);
            func_80090F44(2.07345128f, record->uv);
            if (player->latched == 1) {
                if (player->kind != 3 || vehicle->blocked || !player->active || player->blocked) {
                    player->latched = 0;
                    func_800AFA84(&D_8039A520, record);
                    record = next;
                    continue;
                }
                player->latched = 2;
                if (!record->primary) {
                    color = player->color;
                    if (player->status.flags & 1) {
                        color = (color & 0xFFFFFF00) | player->alpha;
                        if (player->alpha < 48) color = (color & 0xFFFFFF00) | 48;
                    }
                    record->primary = func_8038D328(4, -1, 0x2000, 0);
                    func_8008E06C((s16)record->primary->scene_index, &color);
                    record->secondary = func_8038D328(5, record->primary->scene_index, 0x2080, 1);
                    object = record->secondary;
                    func_8008E06C((s16)object->scene_index, &color);
                    texture = D_80399AF8[D_8012E67C[record->owner]];
                    func_8008D870((s16)object->scene_index,
                                 &D_80151AE8[texture >> 10].textures[texture & 0x3FF], -1);
                }
                record->collision_extent = 0.0f;
            }
            if (player->latched == 2) {
                if (player->kind != 3 || vehicle->blocked || !player->active || player->blocked) {
                    player->latched = 0;
                    func_8038E088(record);
                    func_800AFA84(&D_8039A520, record);
                    record = next;
                    continue;
                }
                object = record->primary;
                for (i = 0; i < 3; i++) object->position[i] = record->position[i];
                math_utility(record->uv, object->uv);
                color = player->color;
                if (player->status.flags & 1) {
                    color = (color & 0xFFFFFF00) | player->alpha;
                    if (player->alpha < 48) color = (color & 0xFFFFFF00) | 48;
                }
                func_8008E06C((s16)record->primary->scene_index, &color);
                func_8008E06C((s16)record->secondary->scene_index, &color);
                input = player->input;
                if (input && (input->fire_mask & input->pressed)) {
                    color = (color & 0xFFFFFF00) | 255;
                    func_8008E06C((s16)record->primary->scene_index, &color);
                    func_8008E06C((s16)record->secondary->scene_index, &color);
                    if (player->ammo > 0) player->ammo--;
                    player->action = 6;
                    func_800B61A8(D_803942C0[3], record->owner, 1, 1);
                    player->cooldown = 1.0f;
                    for (i = 0; i < 3; i++)
                        record->velocity[i] = record->uv[1][i] * 15.0f + player->velocity[i];
                    player->latched = 0;
                    record->lifetime = 5.0f;
                } else {
                    object = record->primary;
                    func_8008B32C(object->uv, object->uv, 0.5f);
                    record = next;
                    continue;
                }
            }
        }
        record->lifetime -= D_8002EB94;
        record->timer += D_8002EB94;
        if (record->lifetime <= 0.0f) {
            func_8038E088(record);
            func_800AFA84(&D_8039A520, record);
            record = next;
            continue;
        }
        for (i = 0; i < 3; i++) previous[i] = record->position[i];
        player = &D_80152818[record->owner];
        extent = record->collision_extent;
        switch ((u8)record->kind) {
        case 3:
            radius = 10.0f;
            damage = 960;
            record->velocity[1] -= D_80142764 * D_8002EB94;
            record->velocity[0] *= 0.9f;
            record->velocity[2] *= 0.9f;
            for (i = 0; i < 3; i++)
                record->position[i] = record->velocity[i] * D_8002EB94 + previous[i];
            break;
        case 2:
            radius = 10.0f;
            damage = 800;
            extent = 0.5f;
            record->velocity[1] -= (D_80142764 * D_8002EB94) * 2.5f;
            for (i = 0; i < 3; i++)
                record->position[i] = record->velocity[i] * D_8002EB94 + previous[i];
            break;
        case 4:
            radius = 12.0f;
            damage = 960;
            func_8038DDDC(record);
            record->velocity[2] += (100.0f - record->velocity[2]) * D_8002EB94;
            distance = record->velocity[2] * D_8002EB94;
            goto advance_forward;
        case 6:
            radius = 8.0f;
            damage = 520;
            for (i = 0; i < 3; i++)
                record->velocity[i] += record->uv[2][i] * (800.0f * D_8002EB94);
            for (i = 0; i < 3; i++)
                record->position[i] = record->velocity[i] * D_8002EB94 + previous[i];
            break;
        case 0:
            radius = 10.0f;
            damage = 400;
            distance = 1000.0f * D_8002EB94;
            goto advance_forward;
        case 1:
            radius = 3.0f;
            damage = 114;
            distance = 2000.0f * D_8002EB94;
            goto advance_forward;
        case 7:
            damage = 66;
            break;
        default:
            radius = 3.0f;
            damage = 80;
            distance = 2000.0f * D_8002EB94;
advance_forward:
            for (i = 0; i < 3; i++) record->position[i] = record->uv[2][i] * distance + previous[i];
            break;
        }
        if (record->attached_effect) {
            record->attached_effect->flags |= 0x10;
            for (i = 0; i < 3; i++) record->attached_effect->position[i] = record->position[i];
        }
        kind = record->kind;
        if (kind == 7) {
            if (record->primary) {
                func_8038D498(record);
                for (i = 0; i < 3; i++) record->primary->position[i] = player->position[i];
                if (record->timer >= 0.0333333015f) {
                    record->timer -= 0.0333333015f;
                    D_8012E700[record->primary->scene_index].scale += 1.0f;
                    record->primary->uv[0][0] = D_8012E700[record->primary->scene_index].scale;
                    record->primary->uv[1][1] = D_8012E700[record->primary->scene_index].scale;
                    record->primary->uv[2][2] = D_8012E700[record->primary->scene_index].scale;
                }
            }
            goto update_object;
        }
        if (kind == 4 || kind == 6) hit = func_8038DA78(record, record->previous_position);
        else hit = func_8038DA78(record, previous);
        if (hit) record->flags |= 0x80;
        if (func_800ADD58(previous, record->position, basis, extent, 0, 0xFFFF)) {
            record->flags |= 0x80;
            kind = record->kind;
            if (kind == 3) {
                if (record->flags & 2) {
                    if (fabsf(record->velocity[0]) < 10.0f && fabsf(record->velocity[2]) < 10.0f) {
                        record->lifetime = 0.0000001f;
                        func_800ABCC8(D_80394CFC, -2, record->position, 0, record->owner, 1, 0);
                        goto update_object;
                    }
                    func_800A61B0(record->velocity, local, basis);
                    local[1] = -local[1] * 0.4f;
                } else {
                    record->flags |= 2;
                    func_800A61B0(record->velocity, local, basis);
                    local[1] = 15.0f;
                }
                func_8009E820(local, record->velocity, basis);
                goto update_object;
            }
            if (kind == 4 || kind == 6) {
                func_8038D798(record->position, record->previous_position, record->owner, 900.0f, damage);
                func_800AF06C(record->position, 0, 0.5f, 1);
                record->lifetime = 0.0000001f;
                if (record->attached_effect) record->attached_effect->flags |= 0x1000;
                goto update_object;
            }
            if (kind == 2) {
                if (hit || record->lifetime <= D_8002EB94) goto hit_target;
                func_800A61B0(record->velocity, local, basis);
                local[1] = -local[1] * 0.9f;
                func_8009E820(local, record->velocity, basis);
                distance = func_8008B2E4(3.0f);
                func_800AEFE0(record->position, D_801141B0, 400.0f, 0.0f,
                              1.0f, 0.0f, (s32)(distance + 70.0f), record->owner, 0, 128);
                goto update_object;
            }
            goto emit_quad;
        }
        if (hit) {
            kind = record->kind;
hit_target:
            record->lifetime = 0.0000001f;
            if (kind == 2 || kind == 4 || kind == 6) {
                if (record->attached_effect) record->attached_effect->flags |= 0x1000;
                if (record->kind == 4 || record->kind == 6)
                    func_8038D798(record->position, record->previous_position, record->owner, 900.0f, damage);
                else func_8038D798(record->position, previous, record->owner, 900.0f, damage);
                func_800AF06C(record->position, 0, 0.5f, 1);
                goto update_object;
            }
            if (record->flags & 1) damage = (s32)((f32)damage * 0.35f);
            func_8038D3A4(player, hit, damage);
            goto emit_quad;
        }
        if (record->lifetime <= D_8002EB94) {
            kind = record->kind;
            if (kind == 2 || kind == 4 || kind == 6) {
                if (kind == 4 || kind == 6)
                    func_8038D798(record->position, record->previous_position, record->owner, 900.0f, damage);
                else func_8038D798(record->position, previous, record->owner, 900.0f, damage);
                func_800AF06C(record->position, 0, 0.5f, 1);
                if (record->attached_effect) record->attached_effect->flags |= 0x1000;
                record->lifetime = 0.0000001f;
            }
        }
        goto update_object;
emit_quad:
        record->lifetime = 0.0000001f;
        for (i = 0; i < 3; i++) side[i] = record->uv[0][i] * radius;
        for (i = 0; i < 3; i++) local[i] = record->uv[1][i] * radius;
        for (i = 0; i < 3; i++) corners[0][i] = record->position[i] - side[i];
        for (i = 0; i < 3; i++) corners[1][i] = side[i] + record->position[i];
        for (i = 0; i < 3; i++) corners[2][i] = side[i] + record->position[i];
        for (i = 0; i < 3; i++) corners[3][i] = record->position[i] - side[i];
        for (i = 0; i < 3; i++) corners[0][i] -= local[i];
        for (i = 0; i < 3; i++) corners[1][i] -= local[i];
        for (i = 0; i < 3; i++) corners[2][i] = local[i] + corners[2][i];
        for (i = 0; i < 3; i++) corners[3][i] = local[i] + corners[3][i];
        if (D_80394F88 == 0)
            func_800B24EC((char *)D_80394D08, (s16 *)&D_80394F88, 0, (s8)(D_80140BDC - 1), 1);
        release = func_8008E3C0(&D_8039A5F8);
        release->quad = func_800A78BC(4, corners, D_80394F88, &D_803942E8, 0x20F, 1);
        if (!release->quad) func_800AFA84(&D_8039A5F8, release);
update_object:
        if (!(record->flags & 0x70)) {
            for (i = 0; i < 3; i++) record->previous_position[i] = previous[i];
        }
        switch ((u8)record->kind) {
        case 0:
            if (!record->primary) record->primary = func_8038D328(2, -1, 0, 0);
            func_8038D200(record, 0);
            break;
        case 2:
            if (!record->primary) record->primary = func_8038D328(3, -1, 0, 0);
            func_8038D200(record, 0);
            break;
        case 3:
            func_8038D200(record, 2);
            break;
        case 4:
            if (!record->primary) record->primary = func_8038D328(7, -1, 0, 0);
            func_8038D200(record, 0);
            break;
        case 6:
            if (!record->primary) record->primary = func_8038D328(8, -1, 0, 0);
            func_8038D200(record, 0);
            break;
        case 7:
            if (!record->primary) {
                record->primary = func_8038D328(9, -1, 0x800000, 0);
                func_8038D200(record, 1);
            }
            break;
        case 1:
        case 8:
            if (!record->primary) record->primary = func_8038D328(1, -1, 0, 0);
            func_8038D200(record, 0);
            break;
        }
        if (record->flags & 0x10) {
            record->flags &= ~0x10;
            record->flags |= 0x20;
        } else if (record->flags & 0x20) {
            record->flags &= ~0x20;
            record->flags |= 0x40;
        } else if (record->flags & 0x40) record->flags &= ~0x40;
        record = next;
    }
}

/* Complete F938 body; source-preserving approved reconciliation only. */
static void func_8038F938(BStatus *status, BPlayer *player,
                   BVehicle *vehicle, s32 owner)
{
    BColor color;
    BStatusEffect *effect;
    s32 i;
    s32 j;

    color = D_8039438C;
    if ((status->flags & 1) && player->transition != 2) {
        status->transition_timer -= D_8002EB94;
        if (status->transition_timer <= 0.0f || vehicle->blocked != 0) {
            status->transition_timer = 0.0f;
            player->transition = 2;
            player->alpha = 16;
            player->transition_time = 0.666667f;
        }
    }
    if (!(status->flags & 2)) {
        return;
    }
    effect = &D_80394F90[owner];
    if (vehicle->blocked != 0) {
        status->scale_timer = 0.0f;
        status->flags &= ~0x1E;
        if (effect->scene_index != -1) {
            func_80090254((s16) effect->scene_index);
            effect->scene_index = -1;
        }
        return;
    }
    for (i = 0; i < 3; i++) {
        effect->position[i] = player->position[i];
    }
    math_utility(player->uv, effect->uv);
    func_80090F44(status->pitch, effect->uv);
    status->pitch -= 0.005f;
    status->scale_timer -= D_8002EB94;
    if (status->scale_timer <= 0.0f) {
        if (status->flags & 8) {
            if (status->scale >= D_80394358[vehicle->model]) {
                status->flags &= ~8;
                status->flags |= 4;
                status->scale = D_80394358[vehicle->model];
                status->scale_timer = 30.0f;
            } else {
                status->scale += 0.05f;
                if (status->scale > D_80394358[vehicle->model]) {
                    status->scale = D_80394358[vehicle->model];
                }
                status->scale_timer = 0.0333333f;
            }
        } else if (status->flags & 0x10) {
            if (status->scale <= 0.06f) {
                status->scale_timer = 0.0f;
                status->flags &= ~0x1E;
                if (effect->scene_index != -1) {
                    func_80090254((s16) effect->scene_index);
                    effect->scene_index = -1;
                }
                return;
            }
            status->scale -= 0.05f;
            status->scale_timer = 0.0333333f;
        } else {
            status->flags &= ~4;
            status->flags |= 0x10;
            status->scale_timer = 0.0333333f;
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            effect->uv[i][j] *= status->scale;
        }
    }
    if (effect->scene_index == -1) {
        effect->scene_index = func_8008E398(D_80399B54, effect->uv, -1, 0x802000);
    }
    if (player->status.flags & 1) {
        color.rgba[3] = player->alpha;
        if (color.rgba[3] < 48) {
            color.rgba[3] = 48;
        }
    }
    func_8008E06C((s16) effect->scene_index, &color.word);
}

/* Complete FCE0 body; source-preserving approved reconciliation only. */
void func_8038FCE0(void)
{
    BDebris *debris;
    BDebris *next_debris;
    BRelease *release;
    BRelease *next_release;
    BPlayer *player;
    BVehicle *vehicle;
    BInput *input;
    BRecord *record;
    f32 *bounds;
    f32 width;
    f32 offset[3];
    f32 speed;
    s32 i;
    s32 j;
    s32 kind;
    s32 attach;
    s32 effect_mode;

    debris = D_8039AE90;
    while (debris != 0) {
        next_debris = debris->next;
        debris->lifetime -= D_8002EB94;
        if (debris->lifetime <= 0.0f) {
            func_80090254((s16) debris->scene_index);
            func_800AFA84(&D_8039AE80, debris);
        } else {
            debris->velocity[1] -= (D_80142764 * D_8002EB94) * 2.5f;
            debris->position[0] += debris->velocity[0] * D_8002EB94;
            debris->position[1] += debris->velocity[1] * D_8002EB94;
            debris->position[2] += debris->velocity[2] * D_8002EB94;
        }
        debris = next_debris;
    }
    player = D_80152818;
    for (i = 0; i < D_801543CA; i++, player++) {
        vehicle = &D_8014A250[player->owner];
        if (player->status.flags != 0) {
            func_8038F938(&player->status, player, vehicle, player->owner);
        }
        kind = player->kind;
        if (kind == 0 || kind == 1 || kind == 6 || kind == 8) {
            func_8038F568(player);
        }
        input = player->input;
        if ((input->reset_mask & input->pressed) != 0 || player->ammo == 0) {
            if ((input->reset_mask & input->pressed) == 0 && player->kind == 5) {
                bounds = D_8011F844[vehicle->model];
                vehicle->extent_108 = bounds[0];
                vehicle->extent_fc = bounds[0];
                width = bounds[1] < bounds[0] ? vehicle->extent_fc : bounds[1];
                vehicle->radius = sqrtf((width * width + bounds[2] * bounds[2]) +
                                        bounds[3] * bounds[3]);
            }
            player->kind = 8;
            player->ammo = -1;
            player->pitch = 0.0f;
            player->yaw = 0.0f;
        }
        if (player->cooldown > 0.0f) {
            player->cooldown -= D_8002EB94;
        }
        if (!(player->cooldown <= 0.0f) || player->ammo == 0) {
            continue;
        }
        if (player->kind == 3) {
            if (player->latched != 0) {
                continue;
            }
            player->latched = 1;
        } else {
            input = player->input;
            if (input == 0 || player->kind == 5) {
                continue;
            }
            if (!(input->fire_mask & input->pressed) &&
                !(player->kind == 1 && (input->fire_mask & input->held))) {
                continue;
            }
            if (vehicle->blocked != 0 || player->active == 0 || player->blocked != 0) {
                continue;
            }
            player->action = 6;
        }
        record = func_8008E3C0(&D_8039A520);
        if (record == 0) {
            continue;
        }
        record->hit_mask = 0;
        record->primary = 0;
        record->secondary = 0;
        record->flags = 0x10;
        record->kind = player->kind;
        record->velocity[2] = 0.0f;
        record->velocity[1] = 0.0f;
        record->velocity[0] = 0.0f;
        record->timer = 0.0f;
        record->owner = player->owner;
        for (j = 0; j < 3; j++) {
            record->position[j] = player->position[j];
        }
        for (j = 0; j < 3; j++) {
            record->previous_position[j] = player->position[j];
        }
        math_utility(player->uv, record->uv);
        attach = 0;
        if (record->kind != 3) {
            func_800B61A8(D_803942C0[player->kind], player->owner, 1, 1);
        }
        switch (record->kind) {
        case 2:
            effect_mode = 1;
            attach = 1;
            func_80090F44(0.5f, record->uv);
            player->cooldown = 0.1666667f;
            record->lifetime = 3.1666667f;
            record->collision_extent = 15.0f;
            for (j = 0; j < 3; j++) {
                record->velocity[j] = record->uv[2][j] * 75.0f + player->velocity[j];
            }
            break;
        case 4:
            effect_mode = 0;
            attach = 1;
            player->cooldown = 1.0f;
            record->lifetime = 15.0f;
            record->collision_extent = 2.0f;
            speed = player->velocity[0] * record->uv[2][0] +
                    player->velocity[1] * record->uv[2][1];
            record->velocity[2] = (player->velocity[2] * record->uv[2][2] + speed) + 165.0f;
            break;
        case 6:
            effect_mode = 2;
            attach = 1;
            player->cooldown = 0.1666667f;
            record->lifetime = 10.0f;
            record->collision_extent = 1.0f;
            func_80090F44(player->pitch, record->uv);
            for (j = 0; j < 3; j++) {
                record->velocity[j] = record->uv[2][j] * 50.0f + player->velocity[j];
            }
            break;
        case 8:
            player->cooldown = 0.1666667f;
            record->lifetime = 4.0f;
            record->collision_extent = 0.5f;
            func_80090E9C(-player->yaw, record->uv);
            func_80090F44(player->pitch, record->uv);
            break;
        case 0:
            player->cooldown = 0.1666667f;
            record->lifetime = 4.0f;
            record->collision_extent = 0.5f;
            func_80090F44(player->pitch, record->uv);
            break;
        case 1:
            player->cooldown = 0.0666667f;
            record->lifetime = 4.0f;
            record->collision_extent = 0.25f;
            speed = func_8008B2E4(0.025f);
            func_80090E9C((speed - 0.0125f) - player->yaw, record->uv);
            speed = func_8008B2E4(0.025f);
            func_80090F44((speed - 0.0125f) + player->pitch, record->uv);
            debris = func_8008E3C0(&D_8039AE80);
            if (debris != 0) {
                for (j = 0; j < 3; j++) {
                    offset[j] = D_80394AFC[j] + D_803943A4[1][vehicle->model][j];
                }
                for (j = 0; j < 3; j++) {
                    debris->position[j] = offset[2] * player->uv[2][j] + record->position[j];
                }
                for (j = 0; j < 3; j++) {
                    debris->position[j] += offset[1] * player->uv[1][j];
                }
                for (j = 0; j < 3; j++) {
                    debris->position[j] += offset[0] * player->uv[0][j];
                }
                math_utility(player->uv, debris->uv);
                speed = func_8008B2E4(0.23f);
                func_8009EA68(speed + 0.3f, debris->uv);
                debris->scene_index = func_8008E398(D_80399B18[0], debris->uv, -1, 0);
                debris->lifetime = func_8008B2E4(0.3f) + 0.5f;
                for (j = 0; j < 3; j++) {
                    debris->velocity[j] = debris->uv[1][j] * 28.0f + player->velocity[j];
                }
                debris->velocity[2] *= 0.88f;
                debris->velocity[1] *= 0.88f;
                debris->velocity[0] *= 0.88f;
            }
            break;
        case 7:
            player->cooldown = 1.0f;
            record->lifetime = 1.0f;
            record->collision_extent = 2.0f;
            break;
        }
        if (player->kind >= 8) {
            for (j = 0; j < 3; j++) {
                offset[j] = D_80394B08[player->kind][j];
            }
        } else {
            for (j = 0; j < 3; j++) {
                offset[j] = D_803943A4[player->kind][vehicle->model][j] +
                            D_80394B08[player->kind][j];
            }
        }
        for (j = 0; j < 3; j++) {
            record->position[j] += offset[2] * player->uv[2][j];
        }
        for (j = 0; j < 3; j++) {
            record->position[j] += offset[1] * player->uv[1][j];
        }
        for (j = 0; j < 3; j++) {
            record->position[j] += offset[0] * player->uv[0][j];
        }
        if (attach) {
            record->attached_effect = func_8038D054(effect_mode, record->position, record->position);
        } else {
            record->attached_effect = 0;
        }
        if (player->ammo > 0 && player->kind != 5 && player->kind != 3) {
            player->ammo--;
        }
    }
    release = D_8039A5F8.head;
    while (release != 0) {
        next_release = release->next;
        func_8008D0C0((s16 *)release->quad);
        func_800AFA84(&D_8039A5F8, release);
        release = next_release;
    }
    func_8038E114();
    func_8038CB20();
}
