/* flags: -g0 -O2 -mips2 -G 0 -non_shared ; repaired raw m2c seed, 632/725 words, NOT a match, IPA-blocked */

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
extern M2C_UNK camera_update_d();
extern f32 func_8008C768();
extern M2C_UNK func_8008E0B8();
extern M2C_UNK func_80099B30();
extern f32 func_8009C3F8();
extern M2C_UNK render_display_list();
extern u8 D_801161F4[];
extern f32 D_80123AFC;
extern f32 D_80124EE8;
extern u8 D_80124F78[];
extern s8 D_80124F84;
extern u8 D_80138670[];
extern u8 D_80150B70[];
extern u8 D_80151AE8[];
extern s32 D_8015B250;
extern s32 D_8017A4B0;
extern s32 D_8017A638;

s32 particle_system(s32 a0, void *arg1, s32 arg2, s32 arg3, void **arg0) {
    void *saved_reg_s7 = 0;
    void *spE4;
    s16 spDA;
    s16 spD8;
    s16 spD6;
    u16 spD4;
    f32 spC8;
    f32 spC4;
    f32 spC0;
    f32 spB8;
    f32 spB4;
    f32 spB0;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f10;
    f32 temp_f20;
    f32 temp_f4;
    f32 temp_f4_2;
    f32 var_f12;
    f32 var_f14;
    f32 var_f4;
    s16 temp_v0_2;
    s32 temp_a0_3;
    s32 temp_a1;
    s32 temp_s8;
    s32 temp_v1;
    s32 temp_v1_6;
    s32 var_a0_2;
    s32 var_s1;
    s32 var_t1;
    s32 var_t8;
    s32 var_t9;
    s32 var_v0;
    u16 *temp_v0_5;
    u16 *temp_v0_6;
    u16 temp_a1_2;
    u16 temp_v0;
    u8 var_a0;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_s0;
    void *temp_s0_2;
    void *temp_s0_3;
    void *temp_t6;
    void *temp_t6_2;
    void *temp_t6_3;
    void *temp_t6_4;
    void *temp_t6_5;
    void *temp_t6_6;
    void *temp_t7;
    void *temp_t7_2;
    void *temp_t8;
    void *temp_t8_2;
    void *temp_t8_3;
    void *temp_t9;
    void *temp_t9_2;
    void *temp_v0_10;
    void *temp_v0_3;
    void *temp_v0_4;
    void *temp_v0_7;
    void *temp_v0_8;
    void *temp_v0_9;
    void *temp_v1_2;
    void *temp_v1_3;
    void *temp_v1_4;
    void *temp_v1_5;
    void *var_s6;

    var_f12 = 0;
    var_f14 = 0;
    var_t1 = 0;
    spE4 = M2C_FIELD(arg0, void **, 0);
    temp_v0 = M2C_FIELD(saved_reg_s7, u16 *, 0x14);
    temp_v1 = M2C_FIELD(saved_reg_s7, s32 *, 0);
    temp_a0 = *((u8 *) &D_801161F4 + (((s32) temp_v0 >> 0xA) * 8)) + ((temp_v0 & 0x3FF) * 0x58);
    var_s1 = M2C_FIELD(temp_a0, s16 *, 0x16) - 1;
    if (temp_v1 & 4) {
        var_s1 = temp_v1 & 3;
        if (var_s1 == 0) {
            D_80124EE8 = 0.0f;
        } else {
            D_80124EE8 = M2C_FIELD(((u8 *) temp_a0 + (var_s1 * 0x10)), f32 *, 0xC);
        }
        var_v0 = 0x100 << M2C_ERROR(/* Read from unset register $t0 */);
    } else {
        var_v0 = 0x100 << M2C_ERROR(/* Read from unset register $t0 */);
        if (((M2C_FIELD(((u8 *) temp_a0 + (var_s1 * 0x10)), f32 *, 0x1C) != 0.0f) && !(temp_v1 & var_v0) && (arg3 == 0)) || (var_v0 = 0x100 << M2C_ERROR(/* Read from unset register $t0 */), ((temp_v1 & 8) != 0))) {
            if (arg2 < 2) {
                if (arg2 > 0) {
                    spC0 = M2C_FIELD(M2C_FIELD(saved_reg_s7, void **, 8), f32 *, 0x24) + M2C_FIELD(M2C_FIELD(arg1, void **, 8), f32 *, 0x24);
                    spC4 = M2C_FIELD(M2C_FIELD(saved_reg_s7, void **, 8), f32 *, 0x28) + M2C_FIELD(M2C_FIELD(arg1, void **, 8), f32 *, 0x28);
                    temp_s0 = (((((M2C_ERROR(/* Read from unset register $t0 */) * 4) + M2C_ERROR(/* Read from unset register $t0 */)) * 4) - M2C_ERROR(/* Read from unset register $t0 */)) * 8) + (u8 *) &D_80150B70;
                    spC8 = M2C_FIELD(M2C_FIELD(saved_reg_s7, void **, 8), f32 *, 0x2C) + M2C_FIELD(M2C_FIELD(arg1, void **, 8), f32 *, 0x2C);
                    spC0 -= M2C_FIELD(temp_s0, f32 *, 0x24);
                    spC4 -= M2C_FIELD(temp_s0, f32 *, 0x28);
                    spC8 -= M2C_FIELD(temp_s0, f32 *, 0x2C);
                } else {
                    temp_s0_2 = (((((M2C_ERROR(/* Read from unset register $t0 */) * 4) + M2C_ERROR(/* Read from unset register $t0 */)) * 4) - M2C_ERROR(/* Read from unset register $t0 */)) * 8) + (u8 *) &D_80150B70;
                    spC0 = M2C_FIELD(M2C_FIELD(saved_reg_s7, void **, 8), f32 *, 0x24) - M2C_FIELD(temp_s0_2, f32 *, 0x24);
                    spC4 = M2C_FIELD(M2C_FIELD(saved_reg_s7, void **, 8), f32 *, 0x28) - M2C_FIELD(temp_s0_2, f32 *, 0x28);
                    spC8 = M2C_FIELD(M2C_FIELD(saved_reg_s7, void **, 8), f32 *, 0x2C) - M2C_FIELD(temp_s0_2, f32 *, 0x2C);
                }
            }
            var_f12 = spC4;
            var_f14 = spC8;
            D_80124EE8 = sqrtf((spC0 * spC0) + (var_f12 * var_f12) + (var_f14 * var_f14));
        }
    }
    if (!(M2C_FIELD(saved_reg_s7, s32 *, 0) & var_v0) && ((var_s6 = (u8 *) temp_a0 + (var_s1 * 0x10), temp_f0 = M2C_FIELD(var_s6, f32 *, 0x1C), (temp_f0 == 0.0f)) || !((temp_f0 * M2C_FIELD(saved_reg_s7, f32 *, 0xC)) < D_80124EE8))) {
        if (M2C_FIELD(saved_reg_s7, s32 *, 0) & 4) {
            temp_v0_2 = M2C_FIELD(temp_a0, s16 *, 0x16);
            var_s1 = M2C_FIELD(saved_reg_s7, s32 *, 0) & 3;
            if (var_s1 >= temp_v0_2) {
                var_s1 = temp_v0_2 - 1;
            }
            var_s6 = (u8 *) temp_a0 + (var_s1 * 0x10);
        } else if ((var_s1 > 0) && (var_s1 != 0)) {
            temp_f0_2 = M2C_FIELD(saved_reg_s7, f32 *, 0xC);
            if (D_80124EE8 < (M2C_FIELD(var_s6, f32 *, 0xC) * temp_f0_2)) {
loop_24:
                var_s1 -= 1;
                var_s6 = (u8 *) var_s6 - 0x10;
                if (var_s1 != 0) {
                    if (D_80124EE8 < (M2C_FIELD(var_s6, f32 *, 0xC) * temp_f0_2)) {
                        goto loop_24;
                    }
                }
            }
        }
        temp_s8 = M2C_FIELD(var_s6, s32 *, 0x20);
        if (temp_s8 != 0) {
            if (M2C_FIELD(saved_reg_s7, s32 *, 0) & 0x80000) {
                temp_s0_3 = (((((M2C_ERROR(/* Read from unset register $t0 */) * 4) + M2C_ERROR(/* Read from unset register $t0 */)) * 4) - M2C_ERROR(/* Read from unset register $t0 */)) * 8) + (u8 *) &D_80150B70;
                camera_update_d(var_f12, var_f14, D_8015B250 + (D_80124F84 << 5) + 0x280, -M2C_FIELD(temp_s0_3, f32 *, 0x24), M2C_FIELD(temp_s0_3, f32 *, 0x28), M2C_FIELD(temp_s0_3, f32 *, 0x2C), -M2C_FIELD(&D_80124F78, f32 *, 0), M2C_FIELD(&D_80124F78, f32 *, 4), M2C_FIELD(&D_80124F78, f32 *, 8), 0.0f, 1.0f, 0.0f);
                temp_v0_3 = spE4;
                spE4 = (u8 *) temp_v0_3 + 8;
                M2C_FIELD(temp_v0_3, s32 *, 0) = 0xDC08000A;
                M2C_FIELD(temp_v0_3, s32 *, 4) = (s32) (D_8015B250 + (D_80124F84 << 5) + 0x280);
                temp_v1_2 = spE4;
                spE4 = (u8 *) temp_v1_2 + 8;
                M2C_FIELD(temp_v1_2, s32 *, 0) = 0xDC08030A;
                M2C_FIELD(temp_v1_2, s32 *, 4) = (s32) (D_8015B250 + (D_80124F84 << 5) + 0x290);
                temp_t9 = spE4;
                spE4 = (u8 *) temp_t9 + 8;
                M2C_FIELD(temp_t9, s32 *, 4) = 0x40000;
                M2C_FIELD(temp_t9, s32 *, 0) = 0xD9FFFFFF;
                temp_t8 = spE4;
                D_80124F84 += 1;
                spE4 = (u8 *) temp_t8 + 8;
                M2C_FIELD(temp_t8, s32 *, 0) = 0xD9FEFFFF;
                M2C_FIELD(temp_t8, s32 *, 4) = 0;
                temp_t6 = spE4;
                spE4 = (u8 *) temp_t6 + 8;
                M2C_FIELD(temp_t6, s32 *, 4) = 0;
                M2C_FIELD(temp_t6, s32 *, 0) = 0xE7000000;
                temp_t9_2 = spE4;
                spE4 = (u8 *) temp_t9_2 + 8;
                temp_v1_3 = (u8 *) saved_reg_s7 + (var_s1 * 4);
                M2C_FIELD(temp_t9_2, s32 *, 4) = 0;
                M2C_FIELD(temp_t9_2, s32 *, 0) = 0xE3000A01;
                temp_v0_4 = spE4;
                if (M2C_FIELD(temp_v1_3, void **, 0x20) != NULL) {
                    spE4 = (u8 *) temp_v0_4 + 8;
                    M2C_FIELD(temp_v0_4, s32 *, 0) = 0xD7000002;
                    temp_a0_2 = M2C_FIELD(temp_v1_3, void **, 0x20);
                    M2C_FIELD(temp_v0_4, s32 *, 4) = (s32) (((M2C_FIELD(temp_a0_2, u16 *, 0x12) << 6) & 0xFFFF) | (M2C_FIELD(temp_a0_2, u16 *, 0x10) << 0x17));
                }
                spB0 = M2C_FIELD(&D_80124F78, f32 *, 0) - M2C_FIELD(temp_s0_3, f32 *, 0x24);
                spB4 = M2C_FIELD(&D_80124F78, f32 *, 4) - M2C_FIELD(temp_s0_3, f32 *, 0x28);
                spB8 = M2C_FIELD(&D_80124F78, f32 *, 8) - M2C_FIELD(temp_s0_3, f32 *, 0x2C);
                func_8008E0B8(&spB0);
                temp_f20 = D_80123AFC;
                temp_f10 = (-func_8008C768(spB0, spB8) * temp_f20) + 6144.0f;
                if (M2C_ERROR(/* cfc1 */) & 0x78) {
                    if (!(M2C_ERROR(/* cfc1 */) & 0x78)) {
                        var_t8 = (s32) (temp_f10 - 2.1474836e9f) | 0x80000000;
                    } else {
                        goto block_33;
                    }
                } else {
                    var_t8 = (s32) temp_f10;
                    if (var_t8 < 0) {
block_33:
                        var_t8 = -1;
                    }
                }
                spD4 = (u16) var_t8;
                temp_f4 = (-func_8009C3F8(0) * temp_f20) + 2048.0f;
                if (M2C_ERROR(/* cfc1 */) & 0x78) {
                    if (!(M2C_ERROR(/* cfc1 */) & 0x78)) {
                        var_t9 = (s32) (temp_f4 - 2.1474836e9f) | 0x80000000;
                    } else {
                        goto block_38;
                    }
                } else {
                    var_t9 = (s32) temp_f4;
                    if (var_t9 < 0) {
block_38:
                        var_t9 = -1;
                    }
                }
                spD6 = (s16) var_t9;
                spD8 = spD4 + 0xFE0;
                spDA = var_t9 + 0x7E0;
                M2C_FIELD(saved_reg_s7, u16 **, 0x34) = &spD4;
            }
            temp_t6_2 = spE4;
            if (M2C_FIELD(var_s6, u16 *, 0x1A) & 0x10) {
                spE4 = (u8 *) temp_t6_2 + 8;
                M2C_FIELD(temp_t6_2, s32 *, 4) = 0x20000;
                M2C_FIELD(temp_t6_2, s32 *, 0) = 0xD9FFFFFF;
            }
            temp_a1 = M2C_FIELD(saved_reg_s7, s32 *, 0x38);
            if (temp_a1 != 0) {
                func_80099B30(&spE4, temp_a1);
            }
            temp_a1_2 = M2C_FIELD(var_s6, u16 *, 0x1A);
            if (temp_a1_2 & 1) {
                if (M2C_FIELD(saved_reg_s7, s32 *, 0x38) != 0) {

                }
                temp_a0_3 = M2C_FIELD(((u8 *) saved_reg_s7 + (var_s1 * 4)), s32 *, 0x20);
                if (temp_a0_3 != 0) {
                    render_display_list(temp_a0_3, temp_a1_2);
                } else if (temp_a1_2 & 0x8000) {
                    render_display_list(temp_a0_3, temp_a1_2);
                }
            } else {
                temp_v1_4 = spE4;
                if (M2C_FIELD(saved_reg_s7, u16 **, 0x34) != NULL) {
                    spE4 = (u8 *) temp_v1_4 + 8;
                    temp_v0_5 = M2C_FIELD(saved_reg_s7, u16 **, 0x34);
                    M2C_FIELD(temp_v1_4, s32 *, 0) = (s32) (((((s32) M2C_FIELD(temp_v0_5, u16 *, 0) >> 3) & 0xFFF) << 0xC) | 0xF2000000 | (((s32) M2C_FIELD(temp_v0_5, u16 *, 2) >> 3) & 0xFFF));
                    temp_v0_6 = M2C_FIELD(saved_reg_s7, u16 **, 0x34);
                    M2C_FIELD(temp_v1_4, s32 *, 4) = (s32) (((((s32) M2C_FIELD(temp_v0_6, u16 *, 4) >> 3) & 0xFFF) << 0xC) | (((s32) M2C_FIELD(temp_v0_6, u16 *, 6) >> 3) & 0xFFF));
                } else {
                    temp_v1_5 = spE4;
                    if (M2C_FIELD(saved_reg_s7, void **, 0x30) != NULL) {
                        spE4 = (u8 *) temp_v1_5 + 8;
                        temp_v0_7 = M2C_FIELD(saved_reg_s7, void **, 0x30);
                        M2C_FIELD(temp_v1_5, s32 *, 0) = (s32) (((((s32) M2C_FIELD(temp_v0_7, u16 *, 0) >> 3) & 0xFFF) << 0xC) | 0xF2000000 | (((s32) M2C_FIELD(temp_v0_7, u16 *, 2) >> 3) & 0xFFF));
                        temp_v0_8 = M2C_FIELD(saved_reg_s7, void **, 0x30);
                        M2C_FIELD(temp_v1_5, s32 *, 4) = (s32) (((((s32) M2C_FIELD(temp_v0_8, u16 *, 4) >> 3) & 0xFFF) << 0xC) | (((s32) M2C_FIELD(temp_v0_8, u16 *, 6) >> 3) & 0xFFF));
                    }
                }
            }
            temp_v1_6 = M2C_FIELD(saved_reg_s7, s32 *, 0);
            if ((M2C_FIELD(var_s6, u16 *, 0x1A) & 4) || (temp_v1_6 & 0x2000)) {
                var_a0 = M2C_FIELD(saved_reg_s7, u8 *, 0x3F);
                if ((temp_v1_6 & 0x200000) && (D_80124EE8 > 24.0f)) {
                    if (D_80124EE8 > 80.0f) {
                        var_a0 = 0;
                    } else {
                        var_f4 = (f32) var_a0;
                        if ((s32) var_a0 < 0) {
                            var_f4 += 4294967296.0f;
                        }
                        temp_f4_2 = (var_f4 * (80.0f - D_80124EE8)) / 56.0f;
                        if (M2C_ERROR(/* cfc1 */) & 0x78) {
                            if (!(M2C_ERROR(/* cfc1 */) & 0x78)) {
                                var_a0_2 = (s32) (temp_f4_2 - 2.1474836e9f) | 0x80000000;
                            } else {
                                goto block_69;
                            }
                        } else {
                            var_a0_2 = (s32) temp_f4_2;
                            if (var_a0_2 < 0) {
block_69:
                                var_a0_2 = -1;
                            }
                        }
                        var_a0 = var_a0_2 & 0xFF;
                    }
                }
                temp_v0_9 = spE4;
                spE4 = (u8 *) temp_v0_9 + 8;
                M2C_FIELD(temp_v0_9, s32 *, 0) = 0xFA000000;
                M2C_FIELD(temp_v0_9, s32 *, 4) = (s32) ((M2C_FIELD(saved_reg_s7, u8 *, 0x3E) << 8) | (M2C_FIELD(saved_reg_s7, u8 *, 0x3C) << 0x18) | (M2C_FIELD(saved_reg_s7, u8 *, 0x3D) << 0x10) | (var_a0 & 0xFF));
            }
            if (M2C_FIELD(saved_reg_s7, s32 *, 0) & 0x4000) {
                temp_v0_10 = spE4;
                spE4 = (u8 *) temp_v0_10 + 8;
                M2C_FIELD(temp_v0_10, s32 *, 0) = 0xFB000000;
                M2C_FIELD(temp_v0_10, s32 *, 4) = (s32) (M2C_FIELD(saved_reg_s7, u8 *, 0x43) | (M2C_FIELD(saved_reg_s7, u8 *, 0x40) << 0x18) | (M2C_FIELD(saved_reg_s7, u8 *, 0x41) << 0x10) | (M2C_FIELD(saved_reg_s7, u8 *, 0x42) << 8));
            }
            temp_t8_2 = spE4;
            spE4 = (u8 *) temp_t8_2 + 8;
            M2C_FIELD(temp_t8_2, s32 *, 4) = temp_s8;
            M2C_FIELD(temp_t8_2, s32 *, 0) = 0xDE000000;
            temp_t8_3 = spE4;
            if (D_8017A638 == 0) {
                spE4 = (u8 *) temp_t8_3 + 8;
                M2C_FIELD(temp_t8_3, s32 *, 4) = 0x8000;
                M2C_FIELD(temp_t8_3, s32 *, 0) = 0xE3001001;
                D_8017A638 = 1;
            }
            if (M2C_FIELD(var_s6, u16 *, 0x1A) & 2) {
                D_8017A4B0 = 0;
                if (arg2 != 0) {
                    func_80099B30(&spE4, arg2);
                }
            }
            temp_t7 = spE4;
            var_t1 = 1;
            if (M2C_FIELD(saved_reg_s7, s32 *, 0) & 0x80000) {
                spE4 = (u8 *) temp_t7 + 8;
                M2C_FIELD(temp_t7, s32 *, 0) = 0xE7000000;
                M2C_FIELD(temp_t7, s32 *, 4) = 0;
                temp_t6_3 = spE4;
                spE4 = (u8 *) temp_t6_3 + 8;
                M2C_FIELD(temp_t6_3, s32 *, 4) = 0x100000;
                M2C_FIELD(temp_t6_3, s32 *, 0) = 0xE3000A01;
                temp_t6_4 = spE4;
                spE4 = (u8 *) temp_t6_4 + 8;
                M2C_FIELD(temp_t6_4, s32 *, 4) = 0x10000;
                M2C_FIELD(temp_t6_4, s32 *, 0) = 0xD9FFFFFF;
                temp_t6_5 = spE4;
                spE4 = (u8 *) temp_t6_5 + 8;
                M2C_FIELD(temp_t6_5, s32 *, 4) = -1;
                M2C_FIELD(temp_t6_5, s32 *, 0) = 0xD7000002;
                temp_t6_6 = spE4;
                spE4 = (u8 *) temp_t6_6 + 8;
                M2C_FIELD(temp_t6_6, s32 *, 4) = 0;
                M2C_FIELD(temp_t6_6, s32 *, 0) = 0xD9FBFFFF;
                M2C_FIELD(saved_reg_s7, u16 **, 0x34) = NULL;
            }
            temp_t7_2 = spE4;
            if (M2C_FIELD(var_s6, u16 *, 0x1A) & 0x10) {
                spE4 = (u8 *) temp_t7_2 + 8;
                M2C_FIELD(temp_t7_2, s32 *, 0) = 0xD9FDFFFF;
                M2C_FIELD(temp_t7_2, s32 *, 4) = 0;
            }
        }
    }
    M2C_FIELD(arg0, void **, 0) = spE4;
    return var_t1;
}

