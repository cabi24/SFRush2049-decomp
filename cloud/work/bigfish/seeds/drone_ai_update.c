/* flags: -g0 -O2 -mips2 -G 0 -non_shared ; repaired raw m2c seed, 692/820 words, NOT a match, IPA-blocked (entity_tick_main) */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef u32 uintptr_t;
typedef s32 intptr_t;
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;
typedef union 
{
  struct 
  {
    u32 w0;
    u32 w1;
  } words;
  u64 force_structure_alignment;
} Gfx;
typedef u32 Mtx[4][4];
typedef f32 F32;
typedef s32 S32;
typedef s16 S16;
typedef s8 S08;
typedef u32 U32;
typedef u16 U16;
#define NULL ((void*)0)
/*
 * This header contains macros emitted by m2c in "valid syntax" mode,
 * which can be enabled by passing `--valid-syntax` on the command line.
 *
 * In this mode, unhandled types and expressions are emitted as macros so
 * that the output is compilable without human intervention.
 */

#ifndef M2C_MACROS_H
#define M2C_MACROS_H

/* Unknown types */
typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

/* Unknown field access, like `*(type_ptr) &expr->unk_offset` */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* Bitwise (reinterpret) cast */
#define M2C_BITWISE(type, expr) ((type)(expr))

/* Unaligned reads */
#define M2C_LWL(expr) (expr)
#define M2C_FIRST3BYTES(expr) (expr)
#define M2C_UNALIGNED32(expr) (expr)

/* Unhandled instructions */
#define M2C_ERROR(desc) (0)
#define M2C_TRAP_IF(cond) (0)
#define M2C_BREAK() (0)
#define M2C_SYNC() (0)

#define GLUE_F64(a, b) (0.0)
#define MULT_HI(a, b) (0)
#define MULTU_HI(a, b) (0)
#define DMULT_HI(a, b) (0)
#define DMULTU_HI(a, b) (0)
#define CLZ(x) (0)
#define REVERSE_BITS(x) (0)
#define ROTATE_RIGHT(x, shift) (0)
#define ARM_RRX(x, carry) (0)
#define BSWAP32(x) (0)
#define BSWAP16(x) (0)
#define BSWAP16X2(x) (0)

/* Carry/overflow bits from partially-implemented instructions */
#define M2C_CARRY 0
#define M2C_OVERFLOW(a) (0)

/* Memcpy patterns */
#define M2C_MEMCPY_ALIGNED memcpy
#define M2C_MEMCPY_UNALIGNED memcpy
#define M2C_STRUCT_COPY memcpy

#endif
float sqrtf(float);
float fabsf(float);
#pragma intrinsic (sqrtf)
#pragma intrinsic (fabsf)
extern M2C_UNK entity_tick_main();
extern s16 func_80092B80();
extern M2C_UNK func_80092FE0();
extern M2C_UNK matrix_scale_apply();
extern M2C_UNK model_data_load();
extern M2C_UNK model_transform_setup();
extern u16 string_copy_format();
extern s32 D_801174B4;
extern u8 D_8011ADC0[];
extern u8 D_8011AF90[];
extern u8 D_8011B468[];
extern u8 D_801234A4[];
extern f32 D_80123A60;
extern u8 D_8012E700[];
extern u8 D_8012E714[];
extern u8 D_8012E73C[];
extern u8 D_80139320[];
extern s8 D_8013F1D8;
extern s8 D_8013FECD;
extern s8 D_80140418;
extern u8 D_80140BDC;
extern u8 D_801427C0[];
extern u16 D_80142998;
extern s8 D_8014978C;
extern s32 D_8014A110;
extern u8 D_8014A250[];
extern s16 D_80151AD0;
extern u8 D_80152818[];

void drone_ai_update(void *arg0, s16 arg1) {
    s32 sp18C;
    s16 sp188;
    s32 sp17C;
    s32 sp174;
    s32 sp168;
    s32 sp164;
    s32 sp160;
    s32 sp15C;
    s32 sp14C;
    s8 sp137;
    u8 sp136;
    u8 sp135;
    u8 sp134;
    s32 sp12C;
    u8 sp12A;
    u8 sp129;
    u8 sp128;
    void *sp6C;
    void *sp68;
    s32 *sp64;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f10;
    f32 temp_f2;
    f32 temp_f4;
    f32 temp_f8;
    f32 var_f10;
    f32 var_f4;
    f32 var_f6;
    s16 temp_a2;
    s16 temp_a3;
    s32 *temp_ra;
    s32 *temp_t0;
    s32 *temp_v0_5;
    s32 *temp_v0_6;
    s32 *temp_v0_7;
    s32 temp_a3_2;
    s32 temp_t6_2;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_at;
    s32 var_t7;
    s32 var_t7_2;
    s32 var_t9;
    s32 var_v1;
    s8 temp_a3_3;
    u16 temp_a1_2;
    u32 temp_v1_5;
    u32 temp_v1_6;
    u32 temp_v1_7;
    u8 temp_v1_2;
    u8 temp_v1_3;
    u8 temp_v1_4;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a1;
    void *temp_a2_2;
    void *temp_t5;
    void *temp_t6;
    void *temp_v0;
    void *temp_v1;
    void *var_t5;
    void *var_t5_2;

    temp_a3 = M2C_FIELD(arg0, s16 *, 6);
    temp_a2 = M2C_FIELD(arg0, s16 *, 8);
    sp17C = (s32) *((u8 *) &D_8012E714 + (temp_a3 * 0x44));
    if (arg1 == 0) {
        if (temp_a3 >= 0) {
            model_data_load(temp_a3, 1, 0xF, temp_a3);
        }
        M2C_FIELD(arg0, s32 *, 0x14) = 0;
        M2C_FIELD(arg0, s16 *, 6) = -1;
        return;
    }
    temp_t5 = (temp_a2 * 0x3B8) + (u8 *) &D_80152818;
    sp188 = temp_a2;
    temp_t9 = (M2C_FIELD(temp_t5, s32 *, 0xE8) & 0x10) != 0;
    temp_ra = (temp_a2 << 6) + (u8 *) &D_80139320;
    sp168 = M2C_FIELD(temp_ra, s32 *, 0x14);
    sp174 = M2C_FIELD(temp_ra, s32 *, 0x10);
    temp_t6 = (temp_a2 * 0x808) + (u8 *) &D_8014A250;
    sp18C = temp_t9;
    sp6C = temp_t6;
    if (M2C_FIELD(temp_t6, u8 *, 8) == 2) {
        sp64 = temp_ra;
        sp188 = temp_a2;
        sp68 = temp_t5;
        entity_tick_main(temp_t9, temp_a2, temp_a3);
    }
    temp_t6_2 = D_8014978C * 4;
    temp_a3_2 = *((u8 *) &D_8011AF90 + temp_t6_2);
    temp_a0 = temp_a3_2 + (((s32) (M2C_FIELD(temp_t5, u16 *, 0x344) & 0xF800) >> 0xB) * 4);
    temp_v1 = temp_a3_2 + (((s32) (M2C_FIELD(temp_t5, u16 *, 0x34A) & 0xF800) >> 0xB) * 4);
    temp_a1 = temp_a3_2 + (((s32) (M2C_FIELD(temp_t5, u16 *, 0x346) & 0xF800) >> 0xB) * 4);
    temp_a2_2 = temp_a3_2 + (((s32) (M2C_FIELD(temp_t5, u16 *, 0x348) & 0xF800) >> 0xB) * 4);
    sp128 = (u8) ((u32) (M2C_FIELD(temp_v1, u8 *, 0) + M2C_FIELD(temp_a0, u8 *, 0) + M2C_FIELD(temp_a1, u8 *, 0) + M2C_FIELD(temp_a2_2, u8 *, 0)) >> 2);
    sp129 = (u8) ((u32) (M2C_FIELD(temp_v1, u8 *, 1) + M2C_FIELD(temp_a0, u8 *, 1) + M2C_FIELD(temp_a1, u8 *, 1) + M2C_FIELD(temp_a2_2, u8 *, 1)) >> 2);
    sp12A = (u8) ((u32) (M2C_FIELD(temp_v1, u8 *, 2) + M2C_FIELD(temp_a0, u8 *, 2) + M2C_FIELD(temp_a1, u8 *, 2) + M2C_FIELD(temp_a2_2, u8 *, 2)) >> 2);
    temp_f0 = M2C_FIELD(sp6C, f32 *, 0x5F4);
    if (temp_f0 >= 20.0f) {
        temp_f2 = (temp_f0 - 20.0f) * D_80123A60;
        if (temp_f2 > 1.0f) {
            var_at = *(s32 *)((u8 *)temp_a3_2 + (*((u8 *) &D_8011B468 + temp_t6_2) * 4));
        } else {
            if (temp_f2 > 0.0f) {
                temp_f0_2 = 1.0f - temp_f2;
                sp12C = *(s32 *)((u8 *)temp_a3_2 + (*((u8 *) &D_8011B468 + temp_t6_2) * 4));
                temp_v1_2 = M2C_FIELD(&sp12C, u8 *, 0);
                var_f6 = (f32) temp_v1_2;
                if ((s32) temp_v1_2 < 0) {
                    var_f6 += 4294967296.0f;
                }
                temp_f4 = ((f32) sp128 * temp_f0_2) + (var_f6 * temp_f2);
                if (0 & 0x78) {
                    if (!(0 & 0x78)) {
                        var_t7 = (s32) (temp_f4 - 2.1474836e9f) | 0x80000000;
                    } else {
                        goto block_15;
                    }
                } else {
                    var_t7 = (s32) temp_f4;
                    if (var_t7 < 0) {
block_15:
                        var_t7 = -1;
                    }
                }
                sp128 = (u8) var_t7;
                temp_v1_3 = M2C_FIELD(&sp12C, u8 *, 1);
                var_f4 = (f32) temp_v1_3;
                if ((s32) temp_v1_3 < 0) {
                    var_f4 += 4294967296.0f;
                }
                temp_f10 = ((f32) sp129 * temp_f0_2) + (var_f4 * temp_f2);
                if (0 & 0x78) {
                    if (!(0 & 0x78)) {
                        var_t9 = (s32) (temp_f10 - 2.1474836e9f) | 0x80000000;
                    } else {
                        goto block_22;
                    }
                } else {
                    var_t9 = (s32) temp_f10;
                    if (var_t9 < 0) {
block_22:
                        var_t9 = -1;
                    }
                }
                sp129 = (u8) var_t9;
                temp_v1_4 = M2C_FIELD(&sp12C, u8 *, 2);
                var_f10 = (f32) temp_v1_4;
                if ((s32) temp_v1_4 < 0) {
                    var_f10 += 4294967296.0f;
                }
                temp_f8 = ((f32) sp12A * temp_f0_2) + (var_f10 * temp_f2);
                if (0 & 0x78) {
                    if (!(0 & 0x78)) {
                        var_t7_2 = (s32) (temp_f8 - 2.1474836e9f) | 0x80000000;
                    } else {
                        goto block_29;
                    }
                } else {
                    var_t7_2 = (s32) temp_f8;
                    if (var_t7_2 < 0) {
block_29:
                        var_t7_2 = -1;
                    }
                }
                sp12A = (u8) var_t7_2;
            }
            goto block_32;
        }
    } else {
block_32:
        sp134 = sp128;
        sp137 = 0xFF;
        sp135 = sp129;
        sp136 = sp12A;
        var_at = sp134;
    }
    sp14C = var_at;
    sp164 = sp14C;
    M2C_FIELD(temp_t5, s32 *, 0x350) = (s32) M2C_FIELD(temp_t5, s32 *, 0x34C);
    M2C_FIELD(temp_t5, s32 *, 0x34C) = sp164;
    temp_v0 = (D_8014978C * 8) + (u8 *) &D_8011ADC0;
    sp160 = M2C_FIELD(temp_v0, s32 *, 0);
    sp15C = M2C_FIELD(temp_v0, s32 *, 4);
    if (M2C_FIELD(temp_t5, s8 *, 0x35A) != 0) {
        temp_v1_5 = M2C_FIELD(temp_t5, u32 *, 0x360);
        if (temp_v1_5 < 0x20U) {
            M2C_FIELD(temp_t5, u32 *, 0x360) = 0U;
        } else {
            M2C_FIELD(temp_t5, u32 *, 0x360) = (u32) (temp_v1_5 - 0x19);
        }
    } else {
        temp_v1_6 = M2C_FIELD(temp_t5, u32 *, 0x360);
        if (temp_v1_6 >= 0xE1U) {
            M2C_FIELD(temp_t5, u32 *, 0x360) = 0xFFU;
        } else {
            M2C_FIELD(temp_t5, u32 *, 0x360) = (u32) (temp_v1_6 + 0x19);
        }
    }
    M2C_FIELD(&sp160, u8 *, 3) = (u8) M2C_FIELD(temp_t5, u32 *, 0x360);
    M2C_FIELD(&sp15C, u8 *, 3) = (u8) M2C_FIELD(temp_t5, u32 *, 0x360);
    if ((D_8014A110 == 2) && (sp188 > 0)) {
        M2C_FIELD(&sp164, u8 *, 3) = 0x80U;
    } else if ((D_8014A110 == 6) && (M2C_FIELD(temp_t5, s32 *, 0x38C) & 1)) {
        M2C_FIELD(&sp164, u8 *, 3) = (u8) M2C_FIELD(temp_t5, u8 *, 0x3A1);
        M2C_FIELD(&sp160, u8 *, 3) = (u8) M2C_FIELD(temp_t5, u8 *, 0x3A1);
        M2C_FIELD(&sp15C, u8 *, 3) = (u8) M2C_FIELD(temp_t5, u8 *, 0x3A1);
    } else {
        M2C_FIELD(&sp164, u8 *, 3) = 0xFFU;
    }
    sp64 = temp_ra;
    sp68 = temp_t5;
    func_80092FE0(0x41A00000, sp188, &sp164, temp_a2_2, temp_a3_2);
    temp_v0_2 = M2C_FIELD(temp_ra, s32 *, 0x14);
    M2C_FIELD(((u8 *) &D_8012E700 + ((s16) temp_v0_2 * 0x44)), s32 *, 0x3C) = (s32) M2C_FIELD(&sp160, s32 *, 0);
    M2C_FIELD(((u8 *) &D_8012E700 + ((s16) temp_v0_2 * 0x44)), s32 *, 0x40) = (s32) M2C_FIELD(&sp15C, s32 *, 0);
    if (sp18C != 0) {
        M2C_FIELD(temp_t5, s8 *, 0x35F) = 0;
        sp68 = temp_t5;
        if (func_80092B80(sp188, 0xDU) != sp17C) {
            sp68 = temp_t5;
            *((u8 *) &D_8012E714 + (M2C_FIELD(arg0, s16 *, 6) * 0x44)) = func_80092B80(sp188, 0xDU);
        }
        sp68 = temp_t5;
        model_data_load((s16) sp174, 1, 0xF);
        model_data_load((s16) sp168, 1, 0xF);
        matrix_scale_apply();
        return;
    }
    temp_t8 = D_801174B4 & 0x100;
    var_v1 = temp_t8;
    if ((temp_t8 != 0) && (D_8013FECD != 0) && (M2C_FIELD(temp_t5, s8 *, 0x35F) == 0)) {
        sp64 = temp_ra;
        sp68 = temp_t5;
        D_80142998 = string_copy_format(&D_801234A4, 0, (s8) (D_80140BDC - 1), 0);
        M2C_FIELD(temp_t5, s8 *, 0x35F) = 1;
        var_v1 = D_801174B4 & 0x100;
    }
    if ((var_v1 != 0) && (D_8013FECD == 0) && (M2C_FIELD(temp_t5, s8 *, 0x35F) != 0)) {
        M2C_FIELD(temp_t5, s8 *, 0x35F) = 0;
    }
    if (M2C_FIELD(temp_t5, s8 *, 0x35F) != 0) {
        if (sp17C != D_80142998) {
            M2C_FIELD(((u8 *) &D_8012E700 + (M2C_FIELD(arg0, s16 *, 6) * 0x44)), u16 *, 0x14) = (u16) D_80142998;
        }
        sp68 = temp_t5;
        model_data_load((s16) sp174, 1, 0xF);
        model_data_load((s16) sp168, 1, 0xF);
        matrix_scale_apply();
        return;
    }
    if (!(D_801174B4 & 0x100)) {
        sp64 = temp_ra;
        sp68 = temp_t5;
        matrix_scale_apply();
        var_t5 = temp_t5;
        if (M2C_FIELD(var_t5, s8 *, 0x35A) != 0) {
            temp_v1_7 = M2C_FIELD(var_t5, u32 *, 0x360);
            if (temp_v1_7 < 0x11U) {
                sp68 = var_t5;
                model_data_load((s16) sp168, 1, 0xF);
                goto block_79;
            }
            temp_v0_3 = M2C_FIELD(temp_ra, s32 *, 0x14);
            M2C_FIELD(&sp160, u8 *, 3) = (u8) temp_v1_7;
            M2C_FIELD(&sp15C, u8 *, 3) = (u8) M2C_FIELD(var_t5, u32 *, 0x360);
            *((u8 *) &D_8012E73C + ((s16) temp_v0_3 * 0x44)) = M2C_FIELD(&sp160, s32 *, 0);
            M2C_FIELD((((s16) temp_v0_3 * 0x44) + (u8 *) &D_8012E700), s32 *, 0x40) = (s32) M2C_FIELD(&sp15C, s32 *, 0);
        } else {
            temp_v0_4 = D_8014A110;
            if (((temp_v0_4 == 6) || (temp_v0_4 == 4)) && (D_80151AD0 >= 3)) {
                sp68 = var_t5;
                model_data_load((s16) sp168, 1, 0xF);
            } else if ((D_801174B4 & 0x7C0000) && (D_8013F1D8 != 0)) {
                sp68 = var_t5;
                model_data_load((s16) sp168, 1, 0xF);
            } else {
                temp_a3_3 = M2C_FIELD(var_t5, s8 *, 0x35C);
                if ((temp_a3_3 < 0) || (M2C_FIELD(var_t5, s8 *, 0x35D) >= 2)) {
                    sp68 = var_t5;
                    model_transform_setup(sp168, 0, 0xF, temp_a3_3);
                } else {
                    sp68 = var_t5;
                    model_data_load((s16) sp168, 1, 1 << temp_a3_3, (s16) temp_a3_3);
                }
            }
block_79:
            var_t5 = sp68;
        }
        sp68 = var_t5;
        var_t5_2 = var_t5;
        if (func_80092B80(sp188, M2C_FIELD(sp6C, u8 *, 8)) != sp17C) {
            sp68 = var_t5_2;
            M2C_FIELD(((u8 *) &D_8012E700 + (M2C_FIELD(arg0, s16 *, 6) * 0x44)), s16 *, 0x14) = func_80092B80(sp188, M2C_FIELD(sp6C, u8 *, 8));
        }
        if ((D_8014A110 == 6) && (M2C_FIELD(sp6C, s16 *, 0x6C4) >= 0)) {
            temp_v0_5 = (u8 *) &D_8012E700 + (M2C_FIELD(arg0, s16 *, 6) * 0x44);
            *temp_v0_5 |= 0x80000000;
        } else if ((D_80140418 != 0) || (M2C_FIELD(var_t5_2, s32 *, 0xE8) & 8)) {
            temp_v0_6 = (u8 *) &D_8012E700 + (M2C_FIELD(arg0, s16 *, 6) * 0x44);
            *temp_v0_6 ^= 0x80000000;
        } else {
            temp_v0_7 = (u8 *) &D_8012E700 + (M2C_FIELD(arg0, s16 *, 6) * 0x44);
            *temp_v0_7 &= 0x7FFFFFFF;
        }
        if ((M2C_FIELD(var_t5_2, s8 *, 0x35D) != 1) || (D_80151AD0 >= 2)) {
            model_data_load((s16) sp174, 1, 0xF);
            return;
        }
        temp_t0 = (u8 *) &D_8012E700 + (sp174 * 0x44);
        temp_a0_2 = (u8 *) &D_8012E700 + ((s16) sp174 * 0x44);
        temp_a1_2 = *((u8 *) &D_801427C0 + ((s16) ((sp188 * 3) + 1) * 2));
        if (M2C_FIELD(temp_t0, u16 *, 0x14) != temp_a1_2) {
            M2C_FIELD(temp_a0_2, u16 *, 0x14) = temp_a1_2;
        }
        if ((0x100 << M2C_FIELD(var_t5_2, s8 *, 0x35C)) & M2C_FIELD(temp_a0_2, s32 *, 0)) {
            sp64 = temp_t0;
            sp68 = var_t5_2;
            model_transform_setup(sp174, 0, 1 << M2C_FIELD(var_t5_2, s8 *, 0x35C), M2C_FIELD(var_t5_2, s8 *, 0x35C));
        }
        if ((D_80140418 != 0) || (M2C_FIELD(var_t5_2, s32 *, 0xE8) & 8)) {
            M2C_FIELD(temp_t0, s32 *, 0) ^= 0x80000000;
            return;
        }
        M2C_FIELD(temp_t0, s32 *, 0) &= 0x7FFFFFFF;
    }
}

