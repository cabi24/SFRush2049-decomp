/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B: turn toward the nearest eligible player in the forward cone. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef float f32;
typedef struct Player {
    u8 unknown000[8]; f32 position[3];
    u8 unknown014[756]; s8 active;
    u8 unknown309[80]; s8 blocked;
    u8 unknown35A; s8 index;
    u8 unknown35C[92];
} Player;
typedef struct Vehicle { u8 unknown000[1600]; s8 disabled; u8 unknown641[455]; } Vehicle;
typedef struct Object {
    struct Object *next;
    s8 owner, unknown05, kind, flags;
    f32 lifetime, timer, elapsed, velocity[3], position[3], previous[3], uv[3][3];
} Object;
extern Player D_80152818[];
extern Vehicle D_8014A250[];
extern s16 D_801543CA;
extern s8 D_8012E67C[];
extern void func_800A61B0(f32 *,f32 *,f32 *);
extern void func_80090E9C(f32,f32[3][3]);
extern void func_80090F44(f32,f32[3][3]);
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_8038DDDC(Object *object)
{
    f32 local[3];
    f32 delta[3];
    f32 best[3];
    f32 nearest, distance;
    Player *player;
    nearest = 2000.0f;
    for (player = D_80152818; player < D_80152818 + D_801543CA; player++) {
        if (object->owner != player->index &&
            D_8012E67C[object->owner] != D_8012E67C[player->index] &&
            !D_8014A250[player->index].disabled && player->active && !player->blocked) {
            delta[0] = player->position[0] - object->position[0];
            delta[1] = player->position[1] - object->position[1];
            delta[2] = player->position[2] - object->position[2];
            func_800A61B0(delta, local, &object->uv[0][0]);
            if (!(local[2] < 0.0f || local[0] < -local[2] || local[0] > local[2] ||
                local[1] < -local[2] || local[1] > local[2])) {
                distance = sqrtf(local[0]*local[0] + local[1]*local[1] + local[2]*local[2]);
                if (!(distance >= nearest)) {
                    nearest = distance;
                    best[0] = local[0];
                    best[1] = local[1];
                    best[2] = local[2];
                }
            }
        }
    }
    if (nearest < 2000.0f) {
        if (best[0] > 0.0f) func_80090E9C(-0.04f, object->uv);
        else func_80090E9C(0.04f, object->uv);
        if (best[1] > 0.0f) func_80090F44(0.03f, object->uv);
        else func_80090F44(-0.03f, object->uv);
    }
}
