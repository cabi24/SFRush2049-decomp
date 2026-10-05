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
extern f32 D_801239E0, D_801239E4, D_801239E8, D_801239EC;
extern void entity_spawn_callback(s16, s32, s32);
extern void entity_transform_apply(EffectObject *, s32);
extern void func_80090F44(f32, f32 *);
extern void func_80090E9C(f32, f32 *);


void entity_update_callback(EffectObject *arg0, s16 arg1) {
    u8 color[4];
    f32 spE8;
    f32 spE4;
    PlayerEffect *temp_v0;
    PlayerEffect *temp_v0_5;
    PlayerEffect *var_v0_2;
    RenderObject *temp_v1_4;
    f32 *temp_s1;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f26;
    f32 var_f20;
    f32 var_f22;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 temp_v1_3;
    s32 *temp_s0;
    s32 *temp_s0_2;
    s32 temp_t2;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_s2;
    s32 var_s2_2;
    s32 var_s2_3;
    s32 var_s3;
    u8 *var_v0;
    u8 temp_v0_6;
    u8 temp_v1_5;
    u8 var_v1;
    ParticleQuad *temp_s0_3;

    if (D_801170FC == 0) {
        temp_v0 = &D_80154660[arg0->player];
        var_s2 = 0;
        temp_v0->lifetime -= D_8002EB94;
        temp_v1 = arg0->player;
        var_v0 = D_8014A250[temp_v1];
        if ((D_80154660[temp_v1].lifetime <= 0.0f) || (*(s16 *)(var_v0 + 1732) >= 0)) {
            do {
                temp_s0 = (s32 *)((u8 *)&D_80154660[arg0->player] + var_s2);
                temp_v0_2 = *temp_s0;
                if (temp_v0_2 != -1) {
                    entity_spawn_callback((s16) temp_v0_2, 0, 0);
                }
                var_s2 += 0x54;
                *temp_s0 = -1;
            } while (var_s2 != 0x150);
            var_v0 = D_8014A250[arg0->player];
        }
        if ((s8) var_v0[0x640] == 0) {
            if (D_80156994 == 0) {
                if (D_8014978C >= 6) {
                    goto block_12;
                }
            } else {
block_12:
                temp_v0_3 = D_80154660[arg0->player].center_handle;
                if (temp_v0_3 != -1) {
                    entity_spawn_callback((s16) temp_v0_3, 0, 0);
                    D_80154660[arg0->player].center_handle = -1;
                }
            }
            var_s2_2 = 0;
            do {
                temp_s0_2 = (s32 *)((u8 *)&D_80154660[arg0->player] + var_s2_2);
                temp_v0_4 = *temp_s0_2;
                if (temp_v0_4 != -1) {
                    entity_spawn_callback((s16) temp_v0_4, 0, 0);
                }
                var_s2_2 += 0x54;
                *temp_s0_2 = -1;
            } while (var_s2_2 != 0x150);
            goto block_20;
        }
        if (arg1 == 0) {
block_20:
            entity_transform_apply(arg0, 1);
            return;
        }
        var_s3 = 0;
        var_s2_3 = 0;
        D_80154660[arg0->player].position[0] = player_array[arg0->player].position[0];
        temp_v1_2 = arg0->player;
        D_80154660[temp_v1_2].position[1] = player_array[temp_v1_2].position[1];
        temp_v1_3 = arg0->player;
        D_80154660[temp_v1_3].position[2] = player_array[temp_v1_3].position[2];
        temp_v0_5 = &D_80154660[arg0->player];
        temp_v0_5->update_timer -= D_8002EB94;
        var_v0_2 = &D_80154660[arg0->player];
        if (var_v0_2->update_timer <= 0.0f) {
            var_v1 = var_v0_2->phase;
            if (var_v1 == 0) {
                var_v0_2->alpha -= 0x1F;
                var_v0_2 = &D_80154660[arg0->player];
                goto block_28;
            }
            if (var_v1 == 1) {
                var_v0_2->alpha += 0x1F;
                var_v0_2 = &D_80154660[arg0->player];
                goto block_28;
            }
            if ((s32) var_v1 >= 2) {
                temp_t2 = (D_8011735C * 0x41C64E6D) + 0x3039;
                D_8011735C = temp_t2;
                D_80154660[arg0->player].phase = (s32) (((f32) ((temp_t2 >> 0x10) & 0x7FFF) * 3.0f) / 32768.0f) * -1;
                D_80154660[arg0->player].alpha = 0x5F;
                var_v0_2 = &D_80154660[arg0->player];
block_28:
                var_v1 = var_v0_2->phase;
            }
            var_v0_2->phase = var_v1 + 1;
            if ((D_8014A108 < 4) && ((D_80156994 != 0) || (D_8014978C >= 6))) {
                color[0] = 0xFF;
                color[1] = 0xFF;
                color[2] = 0xFF;
                color[3] = D_80154660[arg0->player].alpha;
                D_8012E700[(s16)D_80154660[arg0->player].center_handle].color = *(u32 *)color;
                temp_v1_4 = &D_8012E700[D_80154660[arg0->player].center_handle];
                temp_f0 = temp_v1_4->opacity;
                if (temp_f0 < 1.0f) {
                    temp_v1_4->opacity = temp_f0 + D_801239E0;
                }
            }
            temp_f26 = D_801239E4;
            var_f22 = spE4;
            var_f20 = spE8;
            do {
                temp_s0_3 = (ParticleQuad *)((u8 *)&D_80154660[arg0->player] + var_s2_3);
                if (temp_s0_3->handle != -1) {
                    temp_f0_2 = temp_s0_3->scale;
                    temp_s0_3->matrix[2][0] = 0.0f;
                    temp_s0_3->matrix[1][0] = 0.0f;
                    temp_s0_3->matrix[0][1] = 0.0f;
                    temp_s0_3->matrix[2][1] = 0.0f;
                    temp_s0_3->matrix[1][2] = 0.0f;
                    temp_s0_3->matrix[0][2] = 0.0f;
                    temp_s0_3->matrix[2][2] = temp_f0_2;
                    temp_s0_3->matrix[1][1] = temp_f0_2;
                    temp_s0_3->matrix[0][0] = temp_f0_2;
                    switch (var_s3) {               /* irregular */
                    case 0:
                        var_f20 = 0.0f;
                        var_f22 = 0.0f;
                        break;
                    case 1:
                        var_f22 = temp_s0_3->radius * 0.5f;
                        var_f20 = var_f22;
                        break;
                    case 2:
                        var_f22 = temp_s0_3->radius * -0.5f;
                        var_f20 = var_f22;
                        break;
                    case 3:
                        temp_f0_3 = temp_s0_3->radius;
                        var_f20 = temp_f0_3 * -0.5f;
                        var_f22 = temp_f0_3 * 0.5f;
                        break;
                    }
                    temp_s1 = &temp_s0_3->matrix[0][0];
                    temp_s0_3->matrix[3][0] = (f32) (player_array[arg0->player].position[0] + var_f20);
                    temp_s0_3->matrix[3][2] = (f32) (player_array[arg0->player].position[2] + var_f22);
                    temp_s0_3->matrix[3][1] = (f32) ((player_array[arg0->player].position[1] + temp_s0_3->radius) - 3.0f);
                    temp_s0_3->radius = (f32) (temp_s0_3->radius + temp_f26);
                    func_80090F44(temp_s0_3->angle_z, temp_s1);
                    func_80090E9C(temp_s0_3->angle_x, temp_s1);
                    temp_f0_4 = temp_s0_3->scale_velocity;
                    temp_s0_3->scale = (f32) (temp_s0_3->scale + temp_f0_4);
                    temp_s0_3->angle_x = (f32) (temp_s0_3->angle_x + temp_s0_3->angle_x_velocity);
                    temp_s0_3->scale_velocity = (f32) (temp_f0_4 - D_801239E8);
                    temp_s0_3->angle_z = (f32) (temp_s0_3->angle_z + temp_s0_3->angle_z_velocity);
                    if (D_80154660[arg0->player].lifetime < temp_f26) {
                        color[0] = 0xF4;
                        color[1] = 0xCD;
                        color[2] = 0x14;
                        color[3] = (u8) temp_s0_3->alpha;
                        temp_v1_5 = temp_s0_3->alpha;
                        temp_v0_6 = temp_s0_3->alpha_step;
                        if ((s32) temp_v0_6 < (s32) temp_v1_5) {
                            temp_s0_3->alpha = (u8) (temp_v1_5 - temp_v0_6);
                        }
                        D_8012E700[(s16)temp_s0_3->handle].color = *(u32 *)color;
                    }
                }
                var_s3 += 1;
                var_s2_3 += 0x54;
            } while (var_s3 != 4);
            spE4 = var_f22;
            spE8 = var_f20;
            D_80154660[arg0->player].update_timer = D_801239EC;
        }
    }
}
