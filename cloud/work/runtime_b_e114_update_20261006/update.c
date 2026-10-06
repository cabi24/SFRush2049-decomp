/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* COMPLETE semantic reconstruction of B:E114; research only, not a match.
 * This ordinary-ABI test entry is NOT an original exported/kept IPA root.
 * The actual ordinary root FCE0 and private DA78/D498 must join a separately
 * established genuine closure before any private-context matching attempt.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef unsigned short u16;
typedef struct BObject {
    struct BObject *next;
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
    u32 unknown38;
} BObject;
typedef struct BEffect {
    u8 unknown00[8];
    u32 flags;
    u32 unknown0C;
    f32 position[3];
} BEffect;
typedef struct BRecord {
    struct BRecord *next;
    s8 owner;
    u8 unknown05;
    s8 kind;
    s8 flags;
    f32 lifetime;
    f32 timer;
    f32 collision_extent;
    f32 velocity[3];
    f32 position[3];
    f32 previous_position[3];
    f32 uv[3][3];
    BEffect *attached_effect;
    BObject *primary;
    BObject *secondary;
} BRecord;
typedef struct BDebris {
    struct BDebris *next;
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
    f32 lifetime;
    f32 velocity[3];
} BDebris;
typedef struct BInput {
    u32 unknown00;
    u32 pressed;
    u32 held;
    u8 unknown0C[0x30-0x0C];
    u32 reset_mask;
    u8 unknown34[8];
    u32 fire_mask;
} BInput;
typedef struct BStatus {
    u32 flags;
    f32 values[4];
} BStatus;
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
    u8 unknown3A2[2];
    s32 latched;
    f32 unknown3A8;
    f32 cooldown;
    f32 pitch;
    f32 yaw;
} BPlayer;
typedef struct BVehicle {
    u8 unknown00[8];
    u8 model;
    u8 unknown09[0xFC-9];
    f32 extent_fc;
    u8 unknown100[8];
    f32 extent_108;
    u8 unknown10C[0x640-0x10C];
    s8 blocked;
    u8 unknown641[0x654-0x641];
    f32 radius;
    u8 unknown658[0x808-0x658];
} BVehicle;
typedef struct BQuad BQuad;
typedef struct BRelease {
    struct BRelease *next;
    BQuad *quad;
} BRelease;
typedef struct BPool {
    u8 header[16];
    void *head;
} BPool;
typedef struct BScene {
    u8 unknown00[12];
    f32 scale;
    u8 unknown10[0x44-0x10];
} BScene;
typedef struct BTexture { u8 bytes[36]; } BTexture;
typedef struct BTextureBank { BTexture *textures; u32 unknown04; } BTextureBank;
extern BRecord *D_8039A530;
extern BPool D_8039A520, D_8039A5F8;
extern BPlayer D_80152818[];
extern BVehicle D_8014A250[];
extern BScene D_8012E700[];
extern s8 D_8012E67C[];
extern BTextureBank D_80151AE8[];
extern u16 D_80399AF8[], D_80394F88;
extern u8 D_80140BDC;
extern u32 D_80394390, D_803942E8;
extern u8 D_80394CFC[], D_80394D08[];
extern f32 D_8002EB94, D_80142764;
extern f32 D_803943A4[8][13][3], D_80394B08[][3], D_801141B0[3][3];
extern s32 D_803942C0[];
extern void math_utility(f32 source[3][3], f32 destination[3][3]);
extern void func_80090F44(f32 angle, f32 uv[3][3]);
extern void func_8008B32C(f32 source[3][3], f32 dest[3][3], f32 scale);
extern void func_8008E06C(s16 scene, u32 *color);
extern void func_8008D870(s16 scene, BTexture *texture, s32 index);
extern void *func_8008E3C0(BPool *pool);
extern void func_800AFA84(BPool *pool, void *entry);
extern void func_800B61A8(s32 sound, s32 owner, s32 arg2, s32 arg3);
extern void func_8038DDDC(BRecord *record);
/* Semantic interfaces only. Native input-register contracts are separate. */
extern BPlayer *private_DA78(BRecord *record, f32 previous[3]);
extern void private_D498(BRecord *record);
extern BObject *private_D328(s32 resource, s32 parent, u32 flags, s32 omit_uv);
extern void private_D200(BRecord *record, s32 mode);
extern void private_E088(BRecord *record);
extern s32 func_800ADD58(f32 previous[3], f32 position[3], f32 basis[3][3],
                       f32 extent, s32 mode, u32 mask);
extern void func_800A61B0(f32 source[3], f32 local[3], f32 basis[3][3]);
extern void func_8009E820(f32 local[3], f32 dest[3], f32 basis[3][3]);
extern void func_800ABCC8(void *effect, s32 mode, f32 position[3], s32 arg3,
                         s32 owner, s32 arg5, s32 arg6);
extern void func_8038D798(f32 position[3], f32 previous[3], s32 owner,
                         f32 strength, s32 damage);
extern void func_800AF06C(f32 position[3], s32 mode, f32 scale, s32 arg3);
extern f32 func_8008B2E4(f32 limit);
extern void func_800AEFE0(f32 position[3], f32 basis[3][3], f32 strength,
                         f32 arg3, f32 arg4, f32 arg5, s32 sound,
                         s32 owner, s32 arg8, s32 flags);
extern void func_8038D3A4(BPlayer *owner, BPlayer *target, s32 damage);
extern void func_800B24EC(void *name, u16 *resource, s32 arg2, s8 count, s32 arg4);
extern BQuad *func_800A78BC(s32 count, f32 corners[4][3], u16 resource,
                        u32 *color, u16 flags, s32 arg5);
extern f32 fabsf(f32 value);
#pragma intrinsic(fabsf)

void func_8038E114(void)
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
                    record->primary = private_D328(4, -1, 0x2000, 0);
                    func_8008E06C((s16)record->primary->scene_index, &color);
                    record->secondary = private_D328(5, record->primary->scene_index, 0x2080, 1);
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
                    private_E088(record);
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
            private_E088(record);
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
                private_D498(record);
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
        if (kind == 4 || kind == 6) hit = private_DA78(record, record->previous_position);
        else hit = private_DA78(record, previous);
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
            func_800B24EC(D_80394D08, &D_80394F88, 0, (s8)(D_80140BDC - 1), 1);
        release = func_8008E3C0(&D_8039A5F8);
        release->quad = func_800A78BC(4, corners, D_80394F88, &D_803942E8, 0x20F, 1);
        if (!release->quad) func_800AFA84(&D_8039A5F8, release);
update_object:
        if (!(record->flags & 0x70)) {
            for (i = 0; i < 3; i++) record->previous_position[i] = previous[i];
        }
        switch ((u8)record->kind) {
        case 0:
            if (!record->primary) record->primary = private_D328(2, -1, 0, 0);
            private_D200(record, 0);
            break;
        case 2:
            if (!record->primary) record->primary = private_D328(3, -1, 0, 0);
            private_D200(record, 0);
            break;
        case 3:
            private_D200(record, 2);
            break;
        case 4:
            if (!record->primary) record->primary = private_D328(7, -1, 0, 0);
            private_D200(record, 0);
            break;
        case 6:
            if (!record->primary) record->primary = private_D328(8, -1, 0, 0);
            private_D200(record, 0);
            break;
        case 7:
            if (!record->primary) {
                record->primary = private_D328(9, -1, 0x800000, 0);
                private_D200(record, 1);
            }
            break;
        case 1:
        case 8:
            if (!record->primary) record->primary = private_D328(1, -1, 0, 0);
            private_D200(record, 0);
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
