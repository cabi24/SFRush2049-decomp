/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete semantic reconstruction of image B [8038F938,8038FCD8).
 * Research only: the four explicit inputs model native s0/s2/s3/s5, not an
 * ordinary ABI claim. No original identifier or translation-unit claim.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct BStatus {
    u32 flags;
    f32 transition_timer;
    f32 scale_timer;
    f32 scale;
    f32 pitch;
} BStatus;
typedef struct BPlayer {
    u8 unknown00[8];
    f32 position[3];
    u8 unknown14[0x2C-0x14];
    f32 uv[3][3];
    u8 unknown50[0x38C-0x50];
    BStatus status;
    u8 unknown3A0;
    u8 alpha;
    s8 transition;
    u8 unknown3A3[5];
    f32 transition_time;
    u8 unknown3AC[0x3B8-0x3AC];
} BPlayer;
typedef struct BVehicle {
    u8 unknown00[8];
    u8 model;
    u8 unknown09[0x640-9];
    s8 blocked;
    u8 unknown641[0x808-0x641];
} BVehicle;
typedef struct BEffect {
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
} BEffect;
typedef union BColor {
    u32 word;
    u8 rgba[4];
} BColor;
extern f32 D_8002EB94;
extern f32 D_80394358[13];
extern BColor D_8039438C;
extern BEffect D_80394F90[4];
extern s32 D_80399B54;
extern void math_utility(f32 source[3][3], f32 destination[3][3]);
extern void func_80090F44(f32 angle, f32 uv[3][3]);
extern void func_80090254(s16 scene_index);
/* Native callers consume v0; the accepted void wrapper is left untouched. */
extern s32 func_8008E398(s32 resource, f32 uv[3][3], s32 parent, u32 flags);
extern void func_8008E06C(s16 scene_index, u32 *color);

void func_8038F938(BStatus *status, BPlayer *player,
                   BVehicle *vehicle, s32 owner)
{
    BColor color;
    BEffect *effect;
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
