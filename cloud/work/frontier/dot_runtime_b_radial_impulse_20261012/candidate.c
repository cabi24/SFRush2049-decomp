/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete semantic research for image B [0x8038D498,0x8038D798).
 * The native private ABI receives effect in s2 and intentionally clobbers
 * s0/s1/s3..s8 and f20..f30; this ordinary C declaration is not an installable
 * replacement. Genuine E114/FCE0 context is still required. Empty claims.
 * Names are descriptive hypotheses; no original arcade donor is established.
 * Shared count qualification follows the already accepted D_801543CA view.
 * The three vectors are actual three-element values, not frame padding.
 */
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
typedef struct Effect {
    struct Effect *next;
    s8 owner;
    s8 hit_mask;
    u8 unknown06[90];
    BObject *primary;
    void *secondary;
} Effect;
typedef struct Player {
    u8 unknown000[8];
    f32 position[3];
    u8 unknown014[24];
    f32 uv[3][3];
    u8 unknown050[696];
    s8 active;
    u8 unknown309[82];
    s8 index;
    u8 unknown35c[92];
} Player;
typedef struct Vehicle {
    u8 unknown000[0x124];
    f32 velocity[3];
    u8 unknown130[12];
    f32 planar_velocity[3];
    u8 unknown148[0x57c];
    s16 attached;
    u8 unknown6c6[0x142];
} Vehicle;
typedef struct Scene {
    u8 unknown00[12];
    f32 radius;
    u8 unknown10[52];
} Scene;
extern Player D_80152818[];
extern Vehicle D_8014A250[];
extern Scene D_8012E700[];
extern volatile s16 D_801543CA;
extern void func_8038D3A4(Player *, Player *, s32);
extern void func_800A61B0(f32 *, f32 *, f32 *);
extern f32 sqrtf(f32);
#pragma intrinsic (sqrtf)

void func_8038D498(Effect *effect)
{
    Player *source;
    Player *player;
    Vehicle *vehicle;
    s32 i;
    s32 magnitude;
    f32 scale;
    f32 radius;
    f32 center[3];
    f32 impulse[3];
    f32 delta[3];
    f32 distance;

    source = &D_80152818[effect->owner];
    radius = D_8012E700[effect->primary->scene_index].radius;
    if (radius <= 8.0f) {
        scale = 1.0f;
        magnitude = 800;
    } else if (radius <= 20.0f) {
        scale = 0.6f;
        magnitude = 400;
    } else {
        scale = 0.5f;
        magnitude = 200;
    }
    center[0] = effect->primary->position[0];
    center[1] = effect->primary->position[1];
    center[2] = effect->primary->position[2];
    for (i = 0, player = D_80152818; i < D_801543CA; i++, player++) {
        if (player->index != effect->owner && D_8014A250[i].attached == -1 &&
            player->active && !(effect->hit_mask & (1 << player->index))) {
            delta[0] = player->position[0] - center[0];
            delta[1] = player->position[1] - center[1];
            delta[2] = player->position[2] - center[2];
            distance = delta[2]*delta[2] + (delta[0]*delta[0] + delta[1]*delta[1]);
            if (distance <= (radius*4.0f)*(radius*4.0f)) {
                effect->hit_mask |= 1 << player->index;
                func_8038D3A4(source, player, magnitude);
                distance = sqrtf(distance);
                impulse[2] = delta[2]/distance * 330000.0f * scale;
                impulse[1] = (delta[1]/distance * 330000.0f + 66000.0f) * scale;
                impulse[0] = delta[0]/distance * 330000.0f * scale;
                func_800A61B0(impulse, delta, &player->uv[0][0]);
                vehicle = &D_8014A250[player->index];
                vehicle->velocity[0] = delta[0] + vehicle->velocity[0];
                vehicle->velocity[1] = delta[1] + vehicle->velocity[1];
                vehicle->velocity[2] = delta[2] + vehicle->velocity[2];
                delta[1] = 0.0f;
                vehicle->planar_velocity[0] = delta[0] + vehicle->planar_velocity[0];
                vehicle->planar_velocity[1] = delta[1] + vehicle->planar_velocity[1];
                vehicle->planar_velocity[2] = delta[2] + vehicle->planar_velocity[2];
            }
        }
    }
}
