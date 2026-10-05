/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct EffectObject {
    u8 prefix[8];
    s16 player;
} EffectObject;

typedef struct ParticleQuad {
    s32 handle;
    f32 matrix[4][3];
    f32 scale;
    f32 scale_velocity;
    f32 angle_x;
    f32 angle_x_velocity;
    f32 angle_z;
    f32 angle_z_velocity;
    u8 alpha;
    u8 alpha_step;
    u8 unused[2];
    f32 radius;
} ParticleQuad;

typedef struct PlayerEffect {
    ParticleQuad quad[4];
    s32 center_handle;
    u8 center_data[36];
    f32 position[3];
    u8 alpha;
    u8 phase;
    u8 unused[2];
    f32 lifetime;
    f32 update_timer;
} PlayerEffect;

typedef struct PlayerView {
    u8 prefix[8];
    f32 position[3];
    u8 remaining[932];
} PlayerView;

typedef struct RenderObject {
    u8 prefix[12];
    f32 opacity;
    u8 before_color[44];
    u32 color;
    u8 remaining[4];
} RenderObject;

extern s32 D_801170FC;
extern f32 D_8002EB94;
extern PlayerEffect D_80154660[];
extern u8 D_8014A250[][2056];
extern PlayerView player_array[];
extern s8 D_80156994;
extern s8 D_8014978C;
extern s16 D_8014A108;
extern s32 D_8011735C;
extern RenderObject D_8012E700[];
extern void entity_spawn_callback(s16, s32, s32);
extern void entity_transform_apply(EffectObject *, s32);
extern void func_80090F44(f32, f32 *);
extern void func_80090E9C(f32, f32 *);

#define MODEL_BYTE(offset) (*(s8 *)(D_8014A250[object->player] + (offset)))
#define MODEL_SHORT(offset) (*(s16 *)(D_8014A250[object->player] + (offset)))
#define EFFECT (D_80154660[object->player])
#define VIEW (player_array[object->player])
#define HANDLE_INDEX(handle) ((s16)(handle))

void entity_update_callback(EffectObject *object, s16 operation)
{
    ParticleQuad *quad;
    s32 i;
    s32 phase;
    f32 offset_x;
    f32 offset_z;
    u8 color[4];
    f32 scale;
    f32 radius;
    f32 velocity;
    f32 opacity;
    u8 alpha;
    u8 alpha_step;

    if (D_801170FC) {
        return;
    }
    EFFECT.lifetime -= D_8002EB94;
    if (EFFECT.lifetime <= 0.0f || MODEL_SHORT(1732) >= 0) {
        for (i = 0; i < 4; i++) {
            quad = &EFFECT.quad[i];
            if (quad->handle != -1) {
                entity_spawn_callback(quad->handle, 0, 0);
            }
            quad->handle = -1;
        }
    }
    if (!MODEL_BYTE(1600)) {
        if (D_80156994 || D_8014978C >= 6) {
            if (EFFECT.center_handle != -1) {
                entity_spawn_callback(EFFECT.center_handle, 0, 0);
                EFFECT.center_handle = -1;
            }
        }
        for (i = 0; i < 4; i++) {
            quad = &EFFECT.quad[i];
            if (quad->handle != -1) {
                entity_spawn_callback(quad->handle, 0, 0);
            }
            quad->handle = -1;
        }
        entity_transform_apply(object, 1);
        return;
    }
    if (!operation) {
        entity_transform_apply(object, 1);
        return;
    }
    EFFECT.position[0] = VIEW.position[0];
    EFFECT.position[1] = VIEW.position[1];
    EFFECT.position[2] = VIEW.position[2];
    EFFECT.update_timer -= D_8002EB94;
    if (!(EFFECT.update_timer <= 0.0f)) {
        return;
    }
    phase = EFFECT.phase;
    if (phase == 0) {
        EFFECT.alpha -= 31;
    } else if (phase == 1) {
        EFFECT.alpha += 31;
    } else if (phase >= 2) {
        D_8011735C = D_8011735C * 1103515245 + 12345;
        EFFECT.phase = -(s32)((f32)((D_8011735C >> 16) & 0x7fff) * 3.0f / 32768.0f);
        EFFECT.alpha = 95;
    }
    EFFECT.phase++;
    if (D_8014A108 < 4 && (D_80156994 || D_8014978C >= 6)) {
        color[0] = 255;
        color[1] = 255;
        color[2] = 255;
        color[3] = EFFECT.alpha;
        D_8012E700[HANDLE_INDEX(EFFECT.center_handle)].color = *(u32 *)color;
        opacity = D_8012E700[EFFECT.center_handle].opacity;
        if (opacity < 1.0f) {
            D_8012E700[EFFECT.center_handle].opacity = opacity + 0.05f;
        }
    }
    for (i = 0; i < 4; i++) {
        quad = &EFFECT.quad[i];
        if (quad->handle != -1) {
            scale = quad->scale;
            quad->matrix[2][0] = 0.0f;
            quad->matrix[1][0] = 0.0f;
            quad->matrix[0][1] = 0.0f;
            quad->matrix[2][1] = 0.0f;
            quad->matrix[1][2] = 0.0f;
            quad->matrix[0][2] = 0.0f;
            quad->matrix[2][2] = scale;
            quad->matrix[1][1] = scale;
            quad->matrix[0][0] = scale;
            switch (i) {
            case 0:
                offset_z = 0.0f;
                offset_x = 0.0f;
                break;
            case 1:
                offset_z = quad->radius * 0.5f;
                offset_x = offset_z;
                break;
            case 2:
                offset_z = quad->radius * -0.5f;
                offset_x = offset_z;
                break;
            case 3:
                offset_x = quad->radius * -0.5f;
                offset_z = quad->radius * 0.5f;
                break;
            }
            radius = quad->radius;
            quad->matrix[3][0] = VIEW.position[0] + offset_x;
            quad->matrix[3][2] = VIEW.position[2] + offset_z;
            quad->matrix[3][1] = VIEW.position[1] + radius - 3.0f;
            quad->radius = radius + 0.2f;
            func_80090F44(quad->angle_z, &quad->matrix[0][0]);
            func_80090E9C(quad->angle_x, &quad->matrix[0][0]);
            velocity = quad->scale_velocity;
            quad->scale += velocity;
            quad->angle_x += quad->angle_x_velocity;
            quad->scale_velocity = velocity - 0.002f;
            quad->angle_z += quad->angle_z_velocity;
            if (EFFECT.lifetime < 0.2f) {
                color[0] = 244;
                color[1] = 205;
                color[2] = 20;
                color[3] = quad->alpha;
                alpha = quad->alpha;
                alpha_step = quad->alpha_step;
                if (alpha_step < alpha) {
                    quad->alpha = alpha - alpha_step;
                }
                D_8012E700[HANDLE_INDEX(quad->handle)].color = *(u32 *)color;
            }
        }
    }
    EFFECT.update_timer = 0.0333333f;
}
