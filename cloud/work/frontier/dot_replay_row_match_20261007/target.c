/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research only: base f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2.
 * Reconstructed from the protected D816C/D91A0 instruction boundaries and
 * existing accepted helper interfaces. No original N64 source/TU claim.
 * See README.md for private-ABI and decompiler-correction limits.
 */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef signed int s32;typedef unsigned int u32;typedef float f32;
void func_8008B32C(f32[3][3],f32[3][3],f32);
void func_8008D870(s16,s32,s32);
void *func_800B24EC(char *,s16 *,s8,s8,s32);
void func_800B5898(f32,f32[3][3]);
void func_800B5940(f32,f32[3][3]);
void *memcpy(void *,const void *,u32);
void model_data_load(s32,s32,u32);
void model_transform_setup(s32,s32,u32);
void func_800D816C(s8);
extern f32 D_8011418C[3][3];
extern s32 D_80154618[],D_80154630[];
extern s16 D_8014A0F8[];
extern f32 D_80154648[],D_80154FC0[];
extern u8 D_80111998[];
extern char D_80120270[],D_80120280[];
extern s32 D_8012E73C[];
typedef union Color {u32 word;u8 channel[4];} Color;

#define M2C_FIELD(p,t,o) (*(t)((u8 *)(p)+(o)))
f32 fabsf(f32);
#pragma intrinsic(fabsf)
extern f32 D_8002EB94;
extern f32 D_80112A9C;
extern f32 D_80112AA0;
extern f32 D_80112AA4;
extern f32 D_80112AA8;
extern f32 D_80112AAC;
extern f32 D_80112AB0;
extern f32 D_80112AB4;
extern s32 D_801140F0;
extern u8 D_80140BDC;

void func_800D816C(s8 arg0) {
    s8 settled;
    f32 previous_position[3];
    s16 texture_index;
    Color color;
    s32 player_offset;
    s16 *selected;
    s32 *moving;
    s32 element_offset;
    void *position;
    f32 (*matrix)[3];
    f32 (*temp_s0)[3];
    f32 (*temp_s2)[3];
    f32 *temp_v0;
    f32 *temp_v0_2;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f24;
    f32 temp_f28;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f2_5;
    f32 temp_f2_6;
    f32 temp_f30;
    f32 temp_f4;
    f32 temp_f6;
    f32 var_f10;
    f32 var_f22;
    f32 var_f24;
    f32 var_f26;
    s16 *temp_t0;
    s32 *temp_v1;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_s0;
    s32 var_s3;
    s32 var_s8;
    s32 var_v1;
    u32 temp_s0_3;
    void *temp_s0_2;
    void *temp_s5;
    void *var_s1;
    void *var_v0;

    settled = 1;
    color.word = D_801140F0;
    if (D_80154618[arg0] != 0) {
        var_a1 = 0;
        var_a2 = 0;
        temp_t0 = &D_8014A0F8[arg0];
        var_v1 = 0;
        do {
            var_a0 = 1;
            if (var_v1 == 8) {
                var_a0 = 0;
            }
            if (var_a0 != 0) {
                var_a1 += 1;
                if (var_v1 < *temp_t0) {
                    var_a2 += 1;
                }
            }
            var_v1 += 1;
        } while (var_v1 != 0xC);
        temp_f2 = (f32) (var_a1 - 1);
        temp_f0 = D_80112AA8 - D_80112AAC;
        temp_s5 = (arg0 * 0x440) + D_80111998;
        if (temp_f0 < (temp_f2 * D_80112AA0 * D_80112A9C)) {
            var_f24 = (f32) var_a2;
            var_f26 = D_80112AA8 - ((var_f24 * temp_f0) / temp_f2);
        } else {
            var_f24 = (f32) var_a2;
            var_f26 = D_80112AA8 - (var_f24 * D_80112AA0 * D_80112A9C);
        }
        selected = temp_t0;
        player_offset = arg0 * 4;
        memcpy(&previous_position[0], (u8 *) temp_s5 + 0x3B0, 0xCU);
        M2C_FIELD(temp_s5, f32 *, 0x388) = (f32) (M2C_FIELD(temp_s5, f32 *, 0x388) + (6.2831855f * D_8002EB94));
        temp_f0_2 = M2C_FIELD(temp_s5, f32 *, 0x388);
        if (6.2831855f < temp_f0_2) {
            M2C_FIELD(temp_s5, f32 *, 0x388) = (f32) (temp_f0_2 - 6.2831855f);
        }
        temp_v1 = (s32 *) (player_offset + (u8 *) D_80154630);
        M2C_FIELD(temp_s5, f32 *, 0x3B8) = (f32) D_80112AB4;
        M2C_FIELD(temp_s5, f32 *, 0x3B0) = (f32) (((D_80112AA4 * D_80112AB4) / D_80112AB0) + (-110.0f * D_80112A9C));
        M2C_FIELD(temp_s5, f32 *, 0x3B4) = (f32) ((var_f26 * D_80112AB4) / D_80112AB0);
        if (*temp_v1 == 0) {
            temp_f16 = M2C_FIELD(temp_s5, f32 *, 0x3B4);
            temp_f12 = fabsf(temp_f16 - previous_position[1]);
            temp_v0 = (f32 *) (player_offset + (u8 *) D_80154648);
            if (*temp_v0 < ((temp_f12 * 60.0f * D_80112A9C) / 10.0f)) {
                temp_f2_2 = (temp_f12 * 60.0f * D_80112A9C) / 10.0f;
                temp_f14 = 540.0f * D_80112A9C;
                temp_f0_3 = 180.0f * D_80112A9C;
                *temp_v0 = temp_f2_2;
                if (temp_f2_2 < temp_f14) {
                    *temp_v0 = *temp_v0;
                } else {
                    *temp_v0 = temp_f14;
                }
                temp_f2_3 = *temp_v0;
                if (temp_f0_3 < temp_f2_3) {
                    *temp_v0 = temp_f2_3;
                } else {
                    *temp_v0 = temp_f0_3;
                }
            }
            if (((*temp_v0 * D_8002EB94) + previous_position[1]) < temp_f16) {
                M2C_FIELD(temp_s5, f32 *, 0x3B4) = (f32) ((*temp_v0 * D_8002EB94) + previous_position[1]);
            } else if (temp_f16 < (previous_position[1] - (*temp_v0 * D_8002EB94))) {
                M2C_FIELD(temp_s5, f32 *, 0x3B4) = (f32) (previous_position[1] - (*temp_v0 * D_8002EB94));
            } else {
                *temp_v0 = 180.0f * D_80112A9C;
            }
        }
        temp_s0 = (u8 *) temp_s5 + 0x38C;
        moving = temp_v1;
        func_8008B32C(D_8011418C, temp_s0, D_80112A9C * 0.4f);
        func_800B5940(M2C_FIELD(temp_s5, f32 *, 0x388), temp_s0);
        temp_f28 = 1.5707964f;
        func_800B5898(temp_f28, temp_s0);
        var_s1 = (arg0 * 0x440) + D_80111998;
        temp_f30 = 12.566371f;
        var_f22 = (var_f24 * D_80112AA0 * D_80112A9C) + var_f26;
        matrix = (u8 *) var_s1 + 0xC;
        position = (u8 *) var_s1 + 0x30;
        temp_f24 = 3.1415927f;
        var_s3 = 0;
        element_offset = 0;
        do {
            var_s0 = 1;
            var_s8 = 1;
            if (var_s3 == *selected) {
                temp_f0_4 = M2C_FIELD(var_s1, f32 *, 8);
                if (temp_f0_4 < temp_f24) {
                    M2C_FIELD(var_s1, f32 *, 8) = (f32) (temp_f0_4 + (temp_f30 * D_8002EB94));
                }
                if (temp_f24 < M2C_FIELD(var_s1, f32 *, 8)) {
                    M2C_FIELD(var_s1, f32 *, 8) = temp_f24;
                }
                if (*moving != 0) {
                    M2C_FIELD(var_s1, f32 *, 8) = temp_f24;
                }
            } else {
                temp_f0_5 = M2C_FIELD(var_s1, f32 *, 8);
                if (temp_f0_5 > 0.0f) {
                    M2C_FIELD(var_s1, f32 *, 8) = (f32) (temp_f0_5 - (temp_f30 * D_8002EB94));
                }
                if (M2C_FIELD(var_s1, f32 *, 8) < 0.0f) {
                    M2C_FIELD(var_s1, f32 *, 8) = 0.0f;
                }
                if (*moving != 0) {
                    M2C_FIELD(var_s1, f32 *, 8) = 0.0f;
                }
            }
            memcpy(&previous_position[0], position, 0xCU);
            M2C_FIELD(var_s1, f32 *, 0x34) = var_f22;
            M2C_FIELD(var_s1, f32 *, 0x30) = (f32) ((-40.0f * D_80112A9C) + D_80112AA4);
            M2C_FIELD(var_s1, f32 *, 0x38) = (f32) D_80112AB0;
            if (var_s3 == 0xA) {
                var_f10 = M2C_FIELD(var_s1, f32 *, 0x30) - (20.0f * D_80112A9C);
                goto block_45;
            }
            if (var_s3 == 0xB) {
                var_f10 = M2C_FIELD(var_s1, f32 *, 0x30) + (20.0f * D_80112A9C);
block_45:
                M2C_FIELD(var_s1, f32 *, 0x30) = var_f10;
            }
            if (*moving == 0) {
                temp_f12_2 = M2C_FIELD(var_s1, f32 *, 0x34);
                temp_f16_2 = fabsf(temp_f12_2 - previous_position[1]);
                temp_v0_2 = (f32 *) (player_offset + (u8 *) D_80154FC0);
                if (*temp_v0_2 < ((temp_f16_2 * 60.0f) / 10.0f)) {
                    temp_f14_2 = 540.0f * D_80112A9C;
                    temp_f2_4 = (temp_f16_2 * 60.0f) / 10.0f;
                    temp_f0_6 = 180.0f * D_80112A9C;
                    *temp_v0_2 = temp_f2_4;
                    if (temp_f2_4 < temp_f14_2) {
                        *temp_v0_2 = *temp_v0_2;
                    } else {
                        *temp_v0_2 = temp_f14_2;
                    }
                    temp_f2_5 = *temp_v0_2;
                    if (temp_f0_6 < temp_f2_5) {
                        *temp_v0_2 = temp_f2_5;
                    } else {
                        *temp_v0_2 = temp_f0_6;
                    }
                }
                if (((*temp_v0_2 * D_8002EB94) + previous_position[1]) < temp_f12_2) {
                    settled = 0;
                    M2C_FIELD(var_s1, f32 *, 0x34) = (f32) ((*temp_v0_2 * D_8002EB94) + previous_position[1]);
                } else if (temp_f12_2 < (previous_position[1] - (*temp_v0_2 * D_8002EB94))) {
                    settled = 0;
                    M2C_FIELD(var_s1, f32 *, 0x34) = (f32) (previous_position[1] - (*temp_v0_2 * D_8002EB94));
                }
            }
            func_8008B32C(D_8011418C, matrix, D_80112A9C);
            func_800B5898(M2C_FIELD(var_s1, f32 *, 8) - temp_f28, matrix);
            if (temp_f28 < M2C_FIELD(var_s1, f32 *, 8)) {
                var_v0 = func_800B24EC(D_80120270, &texture_index, 0, (s8) (D_80140BDC - 1), 1);
            } else {
                var_v0 = func_800B24EC(D_80120280, &texture_index, 0, (s8) (D_80140BDC - 1), 1);
            }
            func_8008D870(M2C_FIELD(var_s1, s16 *, 6), (s32) var_v0, -1);
            temp_f12_3 = M2C_FIELD(var_s1, f32 *, 0x34);
            temp_f2_6 = D_80112AA0 * D_80112A9C;
            if (((D_80112AA8 + temp_f2_6) < temp_f12_3) || (temp_f12_3 < (D_80112AAC - temp_f2_6))) {
                M2C_FIELD(var_s1, u8 *, 0x3C) = 0U;
            } else if (D_80112AA8 < temp_f12_3) {
                temp_f6 = 255.0f - (((temp_f12_3 - D_80112AA8) * 255.0f) / temp_f2_6);
                M2C_FIELD(var_s1, u8 *, 0x3C) = (u8) temp_f6;
            } else if (temp_f12_3 < D_80112AAC) {
                temp_f4 = 255.0f - (((D_80112AAC - temp_f12_3) * 255.0f) / temp_f2_6);
                M2C_FIELD(var_s1, u8 *, 0x3C) = (u8) temp_f4;
            } else {
                M2C_FIELD(var_s1, u8 *, 0x3C) = 0xFFU;
            }
            if (var_s3 == 8) {
                var_s0 = 0;
            }
            if (var_s0 == 0) {
                M2C_FIELD(var_s1, u8 *, 0x3C) = 0U;
            }
            color.channel[3] = M2C_FIELD(var_s1, u8 *, 0x3C);
            *((s32 *) ((u8 *) D_8012E73C + ((s16) M2C_FIELD(var_s1, s32 *, 4) * 0x44))) = (s32) color.word;
            if (var_s3 == 0xA) {
                temp_s0_2 = (u8 *) temp_s5 + element_offset;
                M2C_FIELD(temp_s5, f32 *, 0x330) = (f32) M2C_FIELD(temp_s0_2, f32 *, 0x30);
                M2C_FIELD(temp_s5, f32 *, 0x334) = (f32) M2C_FIELD(temp_s0_2, f32 *, 0x34);
                temp_s2 = (u8 *) temp_s5 + 0x30C;
                M2C_FIELD(temp_s5, f32 *, 0x338) = (f32) M2C_FIELD(temp_s0_2, f32 *, 0x38);
                M2C_FIELD(temp_s5, f32 *, 0x330) = (f32) (M2C_FIELD(temp_s5, f32 *, 0x330) + (180.0f * D_80112A9C));
                func_8008B32C(D_8011418C, temp_s2, D_80112A9C);
                func_800B5898(M2C_FIELD(temp_s0_2, f32 *, 8) - temp_f28, temp_s2);
                func_8008D870(M2C_FIELD(temp_s5, s16 *, 0x306), (s32) var_v0, -1);
                *((s32 *) ((u8 *) D_8012E73C + (M2C_FIELD(temp_s5, s16 *, 0x306) * 0x44))) = (s32) color.word;
            }
            if (var_s3 == 8) {
                var_s8 = 0;
            }
            if (var_s8 == 0) {
                model_data_load(M2C_FIELD(var_s1, s32 *, 4), 1, 0xFU);
                if (var_s3 == 0xA) {
                    model_data_load(M2C_FIELD(temp_s5, s32 *, 0x304), 1, 0xFU);
                }
            } else if (M2C_FIELD(var_s1, u8 *, 0x3C) == 0) {
                model_data_load(M2C_FIELD(var_s1, s32 *, 4), 1, 0xFU);
                if (var_s3 == 0xA) {
                    model_data_load(M2C_FIELD(temp_s5, s32 *, 0x304), 1, 0xFU);
                }
                var_f22 -= D_80112AA0 * D_80112A9C;
            } else {
                temp_s0_3 = 1 << arg0;
                model_transform_setup(M2C_FIELD(var_s1, s32 *, 4), 0, temp_s0_3);
                if (var_s3 == 0xA) {
                    model_transform_setup(M2C_FIELD(temp_s5, s32 *, 0x304), 0, temp_s0_3);
                }
                var_f22 -= D_80112AA0 * D_80112A9C;
            }
            var_s3 += 1;
            element_offset += 0x40;
            matrix = (f32 (*)[3]) ((u8 *) matrix + 0x40);
            position = (u8 *) position + 0x40;
            var_s1 = (u8 *) var_s1 + 0x40;
        } while (var_s3 != 0xC);
        if (settled == 1) {
            *((f32 *) ((u8 *) D_80154FC0 + player_offset)) = 0.0f;
        }
        *moving = 0;
    }
}

