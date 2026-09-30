/* flags: -g0 -O2 -mips2 -G 0 -non_shared ; best hand seed, 732/762 words, NOT a match */

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
#pragma intrinsic (sqrtf)
extern f32 D_801526F8[];
extern f32 D_801526E0[];
extern s8 D_80152574[];
extern s8 D_801525F8[];
extern f32 D_80152680[];
extern f32 D_801551D8[];
extern f32 D_80155210[];
extern f32 D_80155178[][3];
extern f32 D_801551A8[][3];
extern u8 D_80150B70[][0x98];
extern f32 D_801141BC[3];
extern f32 D_801141C8[3];
extern f32 D_8002EB94;
extern f32 D_801244EC, D_801244F0, D_801244F4, D_801244F8, D_801244FC, D_80124500, D_80124504, D_80124508, D_8012450C, D_80124510, D_80124514;
extern s32 D_8014A110;
extern void func_800E8CB8(void *, void *, void *);
extern f32 func_8008B3C8(f32 *);
extern void func_800CFDEC(f32 *, f32 *, s16, f32, f32, f32, f32 *);
extern void vector_copy_scale(f32 *, void *);

#define car ((u8 *)arg0)
void func_800EA3F4(void *arg0, void *arg1) {
    s32 idx;
    f32 sp94, sp90, sp84;
    f32 sp74[3];
    f32 sp68[3];
    f32 sp5C[3];
    f32 sp50[3];
    f32 sp44[3];
    f32 len, f12, f2, scale, fl;
    f32 *p, *q;
    s8 st;
    u8 *obj;
    s32 i;

    idx = M2C_FIELD(car, s8 *, 0x35C);
    sp94 = D_801526F8[idx];
    sp90 = D_801526E0[idx];
    func_800E8CB8(arg0, arg1, car + 0x50);
    if (M2C_FIELD(car, s8 *, 0x35D) == 3) {
        sp94 -= 6.0f;
        sp90 -= 2.0f;
    }
    len = func_8008B3C8((f32 *)(car + 0x14));
    if (len > 100.0f) {
        f12 = 0.25f;
    } else {
        f12 = len * D_801244EC;
    }
    f2 = 1.0f + f12;
    sp84 = sqrtf(sp94 * sp94 + sp90 * sp90) * f2;
    sp94 *= f2;
    if (D_80152574[idx] != 0 || D_8014A110 == 6) {
        f12 = 0.0f;
    } else if (len < 140.0f) {
        f12 = D_801244F4 - (len / 140.0f) * D_801244F0;
    } else {
        f12 = D_801244F8;
    }
    D_80152574[idx] = 0;
    st = D_801525F8[idx];
    p = &D_801551D8[idx];
    if (st >= 3 && D_80152680[idx] > 0.5f) {
        *p = *p + D_8002EB94;
    } else if (len < 30.0f) {
        *p += D_8002EB94 * 10.0f;
    } else {
        *p = *p - D_8002EB94;
    }
    if (*p < 0.0f) {
        *p = 0.0f;
    } else if (*p > 1.0f) {
        *p = 1.0f;
    }
    q = &D_80155210[idx];
    if (st < 3) {
        *q -= D_8002EB94;
    } else {
        fl = D_80152680[idx];
        if (fl > 1.5f) {
            *q += D_8002EB94 * 10.0f;
        } else if (fl > 0.5f) {
            *q += D_8002EB94 / (D_80124500 + (1.5f - fl) * D_801244FC);
        } else {
            *q += D_8002EB94;
        }
    }
    if (*q < 0.0f) {
        *q = 0.0f;
    } else if (*q > 1.0f) {
        *q = 1.0f;
    }
    for (i = 0; i < 3; i++) {
        sp74[i] = -M2C_FIELD(car, f32 *, 0x44 + i * 4);
    }
    if (*p < 1.0f) {
        if (len < 1.0f) {
            sp68[0] = sp74[0];
            sp68[1] = sp74[1];
            sp68[2] = sp74[2];
        } else if (M2C_FIELD(car, f32 *, 0xAC) < 0.0f && st >= 3) {
            scale = 1.0f / len;
            sp68[0] = M2C_FIELD(car, f32 *, 0x14) * scale;
            sp68[1] = M2C_FIELD(car, f32 *, 0x18) * scale;
            sp68[2] = M2C_FIELD(car, f32 *, 0x1C) * scale;
        } else {
            scale = -1.0f / len;
            sp68[0] = M2C_FIELD(car, f32 *, 0x14) * scale;
            sp68[1] = M2C_FIELD(car, f32 *, 0x18) * scale;
            sp68[2] = M2C_FIELD(car, f32 *, 0x1C) * scale;
        }
    }
    if (*p == 0.0f) {
        sp5C[0] = sp68[0]; sp5C[1] = sp68[1]; sp5C[2] = sp68[2];
    } else if (*p == 1.0f) {
        sp5C[0] = sp74[0]; sp5C[1] = sp74[1]; sp5C[2] = sp74[2];
    } else {
        func_800CFDEC(sp68, sp74, 3, 0.0f, 1.0f, *p, sp5C);
        len = func_8008B3C8(sp5C);
        if (len < D_80124504) {
            sp5C[0] = sp74[0]; sp5C[1] = sp74[1]; sp5C[2] = sp74[2];
        } else {
            scale = 1.0f / len;
            sp5C[0] *= scale; sp5C[1] *= scale; sp5C[2] *= scale;
        }
    }
    if (f12 > 0.0f) {
        scale = 1.0f - f12;
        sp5C[0] = sp5C[0] * scale + D_80155178[idx][0] * f12;
        sp5C[1] = sp5C[1] * scale + D_80155178[idx][1] * f12;
        sp5C[2] = sp5C[2] * scale + D_80155178[idx][2] * f12;
        len = func_8008B3C8(sp5C);
        if (len < D_80124508) {
            sp5C[0] = sp74[0]; sp5C[1] = sp74[1]; sp5C[2] = sp74[2];
        } else {
            scale = 1.0f / len;
            sp5C[0] *= scale; sp5C[1] *= scale; sp5C[2] *= scale;
        }
    }
    D_80155178[idx][0] = sp5C[0];
    D_80155178[idx][1] = sp5C[1];
    D_80155178[idx][2] = sp5C[2];
    fl = *q;
    for (i = 0; i < 3; i++) {
        sp74[i] = M2C_FIELD(car, f32 *, 0x38 + i * 4);
    }
    if (fl < 1.0f) {
        sp68[0] = D_801141BC[0];
        sp68[1] = D_801141BC[1];
        sp68[2] = D_801141BC[2];
    }
    {
    f32 x, y, z;
    if (fl == 0.0f) {
        x = sp68[0]; y = sp68[1]; z = sp68[2];
    } else if (fl == 1.0f) {
        x = sp74[0]; y = sp74[1]; z = sp74[2];
    } else {
        func_800CFDEC(sp68, sp74, 3, 0.0f, 1.0f, fl, sp50);
        len = func_8008B3C8(sp50);
        x = sp74[0]; y = sp74[1]; z = sp74[2];
        if (len >= D_8012450C) {
            scale = 1.0f / len;
            x = sp50[0] * scale; y = sp50[1] * scale; z = sp50[2] * scale;
        }
    }
    if (f12 > 0.0f) {
        scale = 1.0f - f12;
        sp50[0] = x * scale + D_801551A8[idx][0] * f12;
        sp50[1] = y * scale + D_801551A8[idx][1] * f12;
        sp50[2] = z * scale + D_801551A8[idx][2] * f12;
        len = func_8008B3C8(sp50);
        x = sp74[0]; y = sp74[1];
        if (len >= D_80124510) {
            scale = 1.0f / len;
            x = sp50[0] * scale; y = sp50[1] * scale; z = sp50[2] * scale;
        }
    }
    D_801551A8[idx][0] = x;
    D_801551A8[idx][1] = y;
    D_801551A8[idx][2] = z;
    obj = D_80150B70[idx];
    sp44[2] = sp5C[2] * sp94 + z * sp90;
    sp44[1] = sp5C[1] * sp94 + y * sp90;
    sp44[0] = sp5C[0] * sp94 + x * sp90;
    sp50[0] = x; sp50[1] = y; sp50[2] = z;
    }
    vector_copy_scale(sp44, obj + 0x78);
    {
    f32 a0 = M2C_FIELD(obj, f32 *, 0x78);
    f32 a1 = M2C_FIELD(obj, f32 *, 0x7C);
    f32 a2 = M2C_FIELD(obj, f32 *, 0x80);
    s8 m;
    sp44[0] = a0 * sp84;
    sp44[1] = a1 * sp84;
    sp44[2] = a2 * sp84;
    m = M2C_FIELD(car, s8 *, 0x35D);
    if (m == 2 || m == 3) {
        sp44[0] += sp50[0] * 4.0f;
        sp44[1] += sp50[1] * 4.0f;
        sp44[2] += sp50[2] * 4.0f;
    }
    M2C_FIELD(obj, f32 *, 0x84) = sp44[0] + M2C_FIELD(arg1, f32 *, 0);
    M2C_FIELD(obj, f32 *, 0x88) = sp44[1] + M2C_FIELD(arg1, f32 *, 4);
    M2C_FIELD(obj, f32 *, 0x8C) = sp44[2] + M2C_FIELD(arg1, f32 *, 8);
    M2C_FIELD(obj, f32 *, 0x78) = a0 * -1.0f;
    M2C_FIELD(obj, f32 *, 0x7C) = a1 * -1.0f;
    M2C_FIELD(obj, f32 *, 0x80) = a2 * -1.0f;
    {
    f32 b1 = M2C_FIELD(obj, f32 *, 0x7C);
    f32 b2 = M2C_FIELD(obj, f32 *, 0x80);
    f32 b0 = M2C_FIELD(obj, f32 *, 0x78);
    M2C_FIELD(obj, f32 *, 0x60) = sp50[1] * b2 - b1 * sp50[2];
    M2C_FIELD(obj, f32 *, 0x64) = sp50[2] * b0 - b2 * sp50[0];
    M2C_FIELD(obj, f32 *, 0x68) = sp50[0] * b1 - b0 * sp50[1];
    }
    }
    len = func_8008B3C8((f32 *)(obj + 0x60));
    if (len < D_80124514) {
        M2C_FIELD(obj, f32 *, 0x60) = D_801141C8[0];
        M2C_FIELD(obj, f32 *, 0x64) = D_801141C8[1];
        M2C_FIELD(obj, f32 *, 0x68) = D_801141C8[2];
    } else {
        scale = 1.0f / len;
        M2C_FIELD(obj, f32 *, 0x60) = M2C_FIELD(obj, f32 *, 0x60) * scale;
        M2C_FIELD(obj, f32 *, 0x64) = M2C_FIELD(obj, f32 *, 0x64) * scale;
        M2C_FIELD(obj, f32 *, 0x68) = M2C_FIELD(obj, f32 *, 0x68) * scale;
    }
    {
    f32 c1 = M2C_FIELD(obj, f32 *, 0x7C);
    f32 c8 = M2C_FIELD(obj, f32 *, 0x68);
    f32 c4 = M2C_FIELD(obj, f32 *, 0x64);
    f32 c2 = M2C_FIELD(obj, f32 *, 0x80);
    f32 c0 = M2C_FIELD(obj, f32 *, 0x60);
    f32 cc = M2C_FIELD(obj, f32 *, 0x78);
    M2C_FIELD(obj, f32 *, 0x6C) = c1 * c8 - c4 * c2;
    M2C_FIELD(obj, f32 *, 0x70) = c2 * c0 - c8 * cc;
    M2C_FIELD(obj, f32 *, 0x74) = cc * c4 - c0 * c1;
    }
}
