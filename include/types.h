/**
 * @file types.h
 * @brief Common type definitions for Rush 2049 N64
 *
 * Mirrors arcade source conventions where possible.
 * See reference/repos/rushtherock/game/typedefs.h
 */

#ifndef TYPES_H
#define TYPES_H

/* Basic integer types */
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

/* Volatile types (for hardware registers) */
typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8  vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

/* N64 Graphics types (from libultra) */
typedef u64 Gfx;
typedef u32 Mtx[4][4];

/* Arcade-style type definitions */
typedef f32 F32;          /* 32-bit float */
typedef s32 S32;          /* Signed 32-bit */
typedef s16 S16;          /* Signed 16-bit */
typedef s8  S08;          /* Signed 8-bit */
typedef u32 U32;          /* Unsigned 32-bit */
typedef u16 U16;          /* Unsigned 16-bit */
typedef u8  U08;          /* Unsigned 8-bit */

/* Boolean */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef s32 BOOL;

/* Vector types */
typedef f32 Vec3f[3];
typedef s16 Vec3s[3];
typedef s32 Vec3i[3];

/* Matrix types */
typedef f32 Mat3f[3][3];
typedef f32 Mat4f[4][4];

/* Fixed-point math (arcade uses 0x4000 = 1.0 scale) */
#define FRAC_BITS 14
#define FRAC_ONE (1 << FRAC_BITS)  /* 0x4000 = 16384 */

/* Common null pointer */
#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /* TYPES_H */
