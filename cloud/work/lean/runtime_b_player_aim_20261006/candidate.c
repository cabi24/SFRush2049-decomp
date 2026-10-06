/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B: choose a forward-cone target and ease the player's aim angles. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef float f32;
typedef struct Player {
    u8 unknown000[8]; f32 position[3]; u8 unknown014[24]; f32 uv[3][3];
    u8 unknown050[696]; s8 active; u8 unknown309[80]; s8 blocked;
    u8 unknown35A; s8 index; u8 unknown35C[40]; s8 kind;
    u8 unknown385[43]; f32 pitch, yaw;
} Player;
typedef struct Vehicle { u8 unknown000[8]; u8 model; u8 unknown009[1591]; s8 disabled; u8 unknown641[455]; } Vehicle;
extern Player D_80152818[];
extern Vehicle D_8014A250[];
extern s16 D_801543CA;
extern s8 D_8012E67C[];
extern f32 D_803943A4[][13][3], D_80394B08[][3];
extern void func_800A61B0(f32 *,f32 *,f32 *);
extern f32 func_8008C768(f32,f32);

void func_8038F568(Player *source)
{
    f32 local[3], delta[3];
    f32 nearest, yaw, pitch, offset;
    Player *player;
    nearest = 2000.0f;
    yaw = 0.0;
    pitch = 0.0;
    for (player = D_80152818; player < D_80152818 + D_801543CA; player++) {
        if (source->index != player->index &&
            D_8012E67C[source->index] != D_8012E67C[player->index] &&
            !D_8014A250[player->index].disabled && player->active && !player->blocked) {
            delta[0] = player->position[0] - source->position[0];
            delta[1] = player->position[1] - source->position[1];
            delta[2] = player->position[2] - source->position[2];
            if (source->kind >= 8) {
                offset = D_80394B08[source->kind][1] * -1.0f;
            } else {
                offset = (D_803943A4[source->kind][D_8014A250[source->index].model][1] +
                          D_80394B08[source->kind][1]) * -1.0f;
            }
            delta[0] += source->uv[1][0] * offset;
            delta[1] += source->uv[1][1] * offset;
            delta[2] += source->uv[1][2] * offset;
            func_800A61B0(delta, local, &source->uv[0][0]);
            if (!(local[2] < 0.0 || local[0] < local[2] * -0.5f ||
                  local[0] > local[2] * 0.5f || local[1] < -local[2] ||
                  local[1] > local[2] || nearest < local[2])) {
                nearest = local[2];
                yaw = func_8008C768(local[0], local[2]);
                pitch = func_8008C768(local[1], local[2]);
            }
        }
    }
    if (source->yaw > yaw) source->yaw -= 0.01f;
    else if (source->yaw < yaw) source->yaw += 0.01f;
    if (source->pitch > pitch) source->pitch -= 0.01f;
    else if (source->pitch < pitch) source->pitch += 0.01f;
}
