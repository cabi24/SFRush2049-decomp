/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* COMPLETE semantic reconstruction, NOT a matching or private-ABI submission.
 * Image B [8038FCE0,803908D0). Original identifiers and TU are unproven.
 * Private helpers have explicit semantic interfaces for testing only; their
 * native registers are recorded in contracts.json, not replaced by O32 claims.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct BObject BObject;
typedef struct BRecord {
    struct BRecord *next;
    s8 owner;
    u8 unknown05;
    s8 kind;
    s8 flags;
    f32 lifetime;
    f32 timer;
    f32 elapsed;
    f32 velocity[3];
    f32 position[3];
    f32 previous_position[3];
    f32 uv[3][3];
    void *attached_effect;
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
    u8 unknown309[0x359-0x309];
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
    u8 unknown3A1[3];
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
typedef struct BRelease {
    struct BRelease *next;
    s32 handle;
} BRelease;
typedef struct BPool {
    u8 header[16];
    void *head;
} BPool;
extern BDebris *D_8039AE90;
extern BPool D_8039AE80, D_8039A520, D_8039A5F8;
extern f32 D_8002EB94, D_80142764;
extern s16 D_801543CA;
extern BPlayer D_80152818[];
extern BVehicle D_8014A250[];
extern f32 D_8011F844[][4];
extern s32 D_803942C0[];
extern f32 D_803943A4[8][13][3];
extern f32 D_80394AFC[3];
extern f32 D_80394B08[][3];
extern s32 D_80399B18[17];
extern void func_80090254(s16 scene_index);
extern void func_800AFA84(BPool *pool, void *object);
extern void *func_8008E3C0(BPool *pool);
extern void math_utility(f32 source[3][3], f32 destination[3][3]);
extern void func_80090F44(f32 angle, f32 uv[3][3]);
extern void func_80090E9C(f32 angle, f32 uv[3][3]);
extern void func_8009EA68(f32 scale, f32 uv[3][3]);
extern f32 func_8008B2E4(f32 limit);
extern s32 func_8008E398(s32 resource, f32 uv[3][3], s32 parent, u32 flags);
extern void func_800B61A8(s32 sound, s32 owner, s32 arg2, s32 arg3);
extern void func_8008D0C0(s32 handle);
extern void func_8038F568(BPlayer *player);
/* Third parameter is genuinely homed by D054, but not otherwise consumed. */
extern void *func_8038D054(s32 mode, f32 position[3], f32 unused_position[3]);
/* Semantic boundary only: native F938 uses s0/s2/s3/s5 and unsaved state. */
extern void private_F938(BStatus *status, BPlayer *player,
                         BVehicle *vehicle, s32 owner);
extern void private_E114(void);
extern void func_8038CB20(void);
extern f32 sqrtf(f32 value);
#pragma intrinsic(sqrtf)

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
            private_F938(&player->status, player, vehicle, player->owner);
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
        record->unknown05 = 0;
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
            record->elapsed = 15.0f;
            for (j = 0; j < 3; j++) {
                record->velocity[j] = record->uv[2][j] * 75.0f + player->velocity[j];
            }
            break;
        case 4:
            effect_mode = 0;
            attach = 1;
            player->cooldown = 1.0f;
            record->lifetime = 15.0f;
            record->elapsed = 2.0f;
            speed = player->velocity[0] * record->uv[2][0] +
                    player->velocity[1] * record->uv[2][1];
            record->velocity[2] = (player->velocity[2] * record->uv[2][2] + speed) + 165.0f;
            break;
        case 6:
            effect_mode = 2;
            attach = 1;
            player->cooldown = 0.1666667f;
            record->lifetime = 10.0f;
            record->elapsed = 1.0f;
            func_80090F44(player->pitch, record->uv);
            for (j = 0; j < 3; j++) {
                record->velocity[j] = record->uv[2][j] * 50.0f + player->velocity[j];
            }
            break;
        case 8:
            player->cooldown = 0.1666667f;
            record->lifetime = 4.0f;
            record->elapsed = 0.5f;
            func_80090E9C(-player->yaw, record->uv);
            func_80090F44(player->pitch, record->uv);
            break;
        case 0:
            player->cooldown = 0.1666667f;
            record->lifetime = 4.0f;
            record->elapsed = 0.5f;
            func_80090F44(player->pitch, record->uv);
            break;
        case 1:
            player->cooldown = 0.0666667f;
            record->lifetime = 4.0f;
            record->elapsed = 0.25f;
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
            record->elapsed = 2.0f;
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
        func_8008D0C0(release->handle);
        func_800AFA84(&D_8039A5F8, release);
        release = next_release;
    }
    private_E114();
    func_8038CB20();
}
