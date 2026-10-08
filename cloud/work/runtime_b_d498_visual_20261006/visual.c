/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete semantic research adapter. Native entry consumes private s2;
 * ordinary-ABI compilation is not an original TU or matching claim. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct BObject {
    struct BObject *next;
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
    u32 unknown38;
} BObject;
typedef struct BRecord {
    struct BRecord *next;
    s8 owner;
    s8 hit_mask;
    s8 kind;
    s8 flags;
    f32 lifetime;
    f32 timer;
    f32 collision_extent;
    f32 velocity[3];
    f32 position[3];
    f32 previous_position[3];
    f32 uv[3][3];
    void *attached_effect;
    BObject *primary;
    BObject *secondary;
} BRecord;
typedef struct BPlayer {
    u8 unknown00[8];
    f32 position[3];
    u8 unknown14[0x2C-0x14];
    f32 uv[3][3];
    u8 unknown50[0x308-0x50];
    s8 active;
    u8 unknown309[0x35B-0x309];
    s8 owner;
    u8 unknown35C[0x3B8-0x35C];
} BPlayer;
typedef struct BVehicle {
    u8 unknown00[0x124];
    f32 impulse[3];
    u8 unknown130[0x13C-0x130];
    f32 horizontal_impulse[3];
    u8 unknown148[0x6C4-0x148];
    s16 state;
    u8 unknown6C6[0x808-0x6C6];
} BVehicle;
typedef struct BScene {
    u8 unknown00[12];
    f32 scale;
    u8 unknown10[0x44-0x10];
} BScene;
extern BPlayer D_80152818[];
extern BVehicle D_8014A250[];
extern BScene D_8012E700[];
extern s16 D_801543CA;
extern void func_8038D3A4(BPlayer *owner, BPlayer *target, s32 damage);
extern void func_800A61B0(f32 source[3], f32 dest[3], f32 basis[3][3]);
extern f32 sqrtf(f32 value);
#pragma intrinsic(sqrtf)

void func_8038D498(BRecord *record)
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
