/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
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
extern volatile f32 D_8002EB94;
extern f32 D_801244EC, D_801244F0, D_801244F4, D_801244F8, D_801244FC, D_80124500, D_80124504, D_80124508, D_8012450C, D_80124510, D_80124514;
extern s32 D_8014A110;


extern void math_utility(void *, void *);
extern void func_8008D6FC(s16, void *, void *);
extern f32 func_8008B424(f32 *);
extern f32 D_801106C0[];
extern s8 D_801613AB;
f32 func_8008B3C8(f32 *v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
void vector_copy_scale(f32 *a, f32 *b) {
    f32 s = func_8008B424(a);
    b[0] = a[0] * s;
    b[1] = a[1] * s;
    b[2] = a[2] * s;
}
void func_800CFDEC(f32 *a, f32 *b, s16 n, f32 lo, f32 hi, f32 t, f32 *out) {
    f32 k;
    s16 i;
    k = hi - lo;
    if (hi != lo) {
        k = (t - lo) / k;
    }
    for (i = 0; i < n; i++) {
        out[i] = (b[i] - a[i]) * k + a[i];
    }
}
void func_800E8CB8(void *car, void *vel, void *mat) {
    f32 m[9];
    math_utility(mat, m);
    if (D_801613AB != 0) {
        m[1] *= D_801106C0[D_801613AB];
        m[4] *= D_801106C0[D_801613AB];
        m[7] *= D_801106C0[D_801613AB];
    }
    if (*(s32 *)((u8 *)car + 244) >= 0) {
        func_8008D6FC(*(s32 *)((u8 *)car + 244), vel, m);
    }
}
#define car ((u8 *)arg0)
#define CF(off) (*(f32 *)(car + (off)))
#define CB(off) (*(s8 *)(car + (off)))
#define OF(off) (*(f32 *)(D_80150B70[idx] + (off)))
#define A1(off) (*(f32 *)((u8 *)arg1 + (off)))
void func_800EA3F4(void *arg0, void *arg1) {
    s32 idx;
    f32 len;
    f32 sp94;
    f32 sp90;
    s32 i;
    s32 unused;
    f32 sp84;
    f32 f2;
    f32 sp74[3];
    f32 sp68[3];
    f32 sp5C[3];
    f32 sp50[3];
    f32 sp44[3];
    f32 f12;

    idx = CB(0x35C);
    sp94 = D_801526F8[idx];
    sp90 = D_801526E0[idx];
    func_800E8CB8(arg0, arg1, car + 0x50);
    if (CB(0x35D) == 3) {
        sp94 -= 6.0f;
        sp90 -= 2.0f;
    }
    len = func_8008B3C8((f32 *)(car + 0x14));
    if (len > 100.0f) {
        f2 = 0.25f;
    } else {
        f2 = len * D_801244EC;
    }
    sp84 = sqrtf(sp94 * sp94 + sp90 * sp90) * (1.0f + f2);
    sp94 *= 1.0f + f2;
    if (D_80152574[idx] != 0 || D_8014A110 == 6) {
        f12 = 0.0;
    } else if (len < 140.0f) {
        f12 = D_801244F4 - (len / 140.0f) * D_801244F0;
    } else {
        f12 = D_801244F8;
    }
    D_80152574[idx] = 0;
    if (D_801525F8[idx] >= 3 && D_80152680[idx] > 0.5f) {
        D_801551D8[idx] = D_801551D8[idx] + D_8002EB94;
    } else if (len < 30.0f) {
        D_801551D8[idx] += D_8002EB94 * 10.0f;
    } else {
        D_801551D8[idx] = D_801551D8[idx] - D_8002EB94;
    }
    if (D_801551D8[idx] < 0.0f) {
        D_801551D8[idx] = 0.0f;
    } else if (D_801551D8[idx] > 1.0f) {
        D_801551D8[idx] = 1.0f;
    }
    if (D_801525F8[idx] < 3) {
        D_80155210[idx] -= D_8002EB94;
    } else {
        if (D_80152680[idx] > 1.5f) {
            D_80155210[idx] += D_8002EB94 * 10.0f;
        } else if (D_80152680[idx] > 0.5f) {
            D_80155210[idx] += D_8002EB94 / (D_80124500 + (1.5f - D_80152680[idx]) * D_801244FC);
        } else {
            D_80155210[idx] += D_8002EB94;
        }
    }
    if (D_80155210[idx] < 0.0f) {
        D_80155210[idx] = 0.0f;
    } else if (D_80155210[idx] > 1.0f) {
        D_80155210[idx] = 1.0f;
    }
    for (i = 0; i < 3; i++) {
        sp74[i] = -CF(0x44 + i * 4);
    }
    if (D_801551D8[idx] < 1.0f) {
        if (len < 1.0f) {
            sp68[0] = sp74[0];
            sp68[1] = sp74[1];
            sp68[2] = sp74[2];
        } else if (CF(0xAC) < 0 && D_801525F8[idx] >= 3) {
            sp68[0] = CF(0x14) * (1 / len);
            sp68[1] = CF(0x18) * (1 / len);
            sp68[2] = CF(0x1C) * (1 / len);
        } else {
            sp68[0] = CF(0x14) * (-1 / len);
            sp68[1] = CF(0x18) * (-1 / len);
            sp68[2] = CF(0x1C) * (-1 / len);
        }
    }
    if (D_801551D8[idx] == 0.0f) {
        sp5C[0] = sp68[0]; sp5C[1] = sp68[1]; sp5C[2] = sp68[2];
    } else if (D_801551D8[idx] == 1.0f) {
        sp5C[0] = sp74[0]; sp5C[1] = sp74[1]; sp5C[2] = sp74[2];
    } else {
        func_800CFDEC(sp68, sp74, 3, 0.0f, 1.0f, D_801551D8[idx], sp5C);
        len = func_8008B3C8(sp5C);
        if (len < D_80124504) {
            sp5C[0] = sp74[0]; sp5C[1] = sp74[1]; sp5C[2] = sp74[2];
        } else {
            sp5C[0] *= (1.0f / len); sp5C[1] *= (1.0f / len); sp5C[2] *= (1.0f / len);
        }
    }
    if (f12 > 0) {
        sp5C[0] *= (1.0f - f12); sp5C[1] *= (1.0f - f12); sp5C[2] *= (1.0f - f12);
        sp5C[0] += D_80155178[idx][0] * f12;
        sp5C[1] += D_80155178[idx][1] * f12;
        sp5C[2] += D_80155178[idx][2] * f12;
        len = func_8008B3C8(sp5C);
        if (len < D_80124508) {
            sp5C[0] = sp74[0]; sp5C[1] = sp74[1]; sp5C[2] = sp74[2];
        } else {
            sp5C[0] *= (1.0f / len); sp5C[1] *= (1.0f / len); sp5C[2] *= (1.0f / len);
        }
    }
    D_80155178[idx][0] = sp5C[0];
    D_80155178[idx][1] = sp5C[1];
    D_80155178[idx][2] = sp5C[2];
    for (i = 0; i < 3; i++) {
        sp74[i] = CF(0x38 + i * 4);
    }
    if (D_80155210[idx] < 1.0f) {
        sp68[0] = D_801141BC[0];
        sp68[1] = D_801141BC[1];
        sp68[2] = D_801141BC[2];
    }
    if (D_80155210[idx] == 0) {
        sp50[0] = sp68[0]; sp50[1] = sp68[1]; sp50[2] = sp68[2];
    } else if (D_80155210[idx] == 1.0f) {
        sp50[0] = sp74[0]; sp50[1] = sp74[1]; sp50[2] = sp74[2];
    } else {
        func_800CFDEC(sp68, sp74, 3, 0, 1.0f, D_80155210[idx], sp50);
        len = func_8008B3C8(sp50);
        if (len < D_8012450C) {
            sp50[0] = sp74[0]; sp50[1] = sp74[1]; sp50[2] = sp74[2];
        } else {
            sp50[0] *= (1.0f / len); sp50[1] *= (1.0f / len); sp50[2] *= (1.0f / len);
        }
    }
    if (f12 > 0) {
        sp50[0] *= (1.0f - f12); sp50[1] *= (1.0f - f12); sp50[2] *= (1.0f - f12);
        sp50[0] += D_801551A8[idx][0] * f12;
        sp50[1] += D_801551A8[idx][1] * f12;
        sp50[2] += D_801551A8[idx][2] * f12;
        len = func_8008B3C8(sp50);
        if (len < D_80124510) {
            sp50[0] = sp74[0]; sp50[1] = sp74[1]; sp50[2] = sp74[2];
        } else {
            sp50[0] *= (1.0f / len); sp50[1] *= (1.0f / len); sp50[2] *= (1.0f / len);
        }
    }
    D_801551A8[idx][0] = sp50[0];
    D_801551A8[idx][1] = sp50[1];
    D_801551A8[idx][2] = sp50[2];
    sp44[0] = sp5C[0] * sp94; sp44[1] = sp5C[1] * sp94; sp44[2] = sp5C[2] * sp94;
    sp44[0] += sp50[0] * sp90;
    sp44[1] += sp50[1] * sp90;
    sp44[2] += sp50[2] * sp90;
    vector_copy_scale(sp44, D_80150B70[idx] + 0x78);
    sp44[0] = OF(0x78) * sp84;
    sp44[1] = OF(0x7C) * sp84;
    sp44[2] = OF(0x80) * sp84;
    if (CB(0x35D) == 2) {
        sp44[0] += sp50[0] * 4.0f;
        sp44[1] += sp50[1] * 4.0f;
        sp44[2] += sp50[2] * 4.0f;
    } else if (CB(0x35D) == 3) {
        sp44[0] += sp50[0] * 4.0f;
        sp44[1] += sp50[1] * 4.0f;
        sp44[2] += sp50[2] * 4.0f;
    }
    OF(0x84) = A1(0) + sp44[0];
    OF(0x88) = A1(4) + sp44[1];
    OF(0x8C) = A1(8) + sp44[2];
    OF(0x78) *= -1.0f;
    OF(0x7C) *= -1.0f;
    OF(0x80) *= -1.0f;
    OF(0x60) = sp50[1] * OF(0x80) - sp50[2] * OF(0x7C);
    OF(0x64) = sp50[2] * OF(0x78) - sp50[0] * OF(0x80);
    OF(0x68) = sp50[0] * OF(0x7C) - sp50[1] * OF(0x78);
    len = func_8008B3C8((f32 *)(D_80150B70[idx] + 0x60));
    if (len < D_80124514) {
        OF(0x60) = D_801141C8[0];
        OF(0x64) = D_801141C8[1];
        OF(0x68) = D_801141C8[2];
    } else {
        OF(0x60) *= (1.0f / len);
        OF(0x64) *= (1.0f / len);
        OF(0x68) *= (1.0f / len);
    }
    OF(0x6C) = OF(0x7C) * OF(0x68) - OF(0x80) * OF(0x64);
    OF(0x70) = OF(0x80) * OF(0x60) - OF(0x78) * OF(0x68);
    OF(0x74) = OF(0x78) * OF(0x64) - OF(0x7C) * OF(0x60);
}

void __standin_func_800EA3F4(void)
{
    func_800EA3F4(0, 0);
}
