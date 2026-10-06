/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete research-only semantic adapter. Native inputs are private s1/s2,
 * not an ordinary ABI; genuine FCE0/E114 context is still required to match. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct BRecord {
    struct BRecord *next;
    s8 owner;
    u8 unknown05;
    s8 kind;
    s8 flags;
    f32 lifetime;
    f32 timer;
    f32 radius;
    f32 velocity[3];
    f32 position[3];
    f32 previous_position[3];
    f32 uv[3][3];
    void *attached_effect;
    void *primary;
    void *secondary;
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
    u8 unknown35C[0x384-0x35C];
    s8 kind;
    u8 unknown385[0x3B8-0x385];
} BPlayer;
typedef struct BVehicle {
    u8 unknown00[0x640];
    s8 blocked;
    u8 unknown641[0x6C4-0x641];
    s16 state;
    u8 unknown6C6[0x808-0x6C6];
} BVehicle;
extern BPlayer D_80152818[];
extern BVehicle D_8014A250[];
extern s16 D_801543CA;
extern void func_800A61B0(f32 source[3], f32 dest[3], f32 basis[3][3]);
extern f32 func_8008C768(f32 y, f32 x);
extern f32 fabsf(f32 value);
extern f32 sqrtf(f32 value);
#pragma intrinsic(fabsf, sqrtf)

BPlayer *func_8038DA78(BRecord *record, f32 previous[3])
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
        radius = record->radius;
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
