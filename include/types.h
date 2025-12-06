/**
 * @file types.h
 * @brief Common type definitions for Rush 2049 N64
 *
 * Mirrors arcade source conventions where possible.
 * See reference/repos/rushtherock/game/typedefs.h
 */

#ifndef TYPES_H
#define TYPES_H

#include <ultra64.h>

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
