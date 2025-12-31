/**
 * @file macros.h
 * @brief Common macros for Rush 2049 N64 decompilation
 *
 * Includes GLOBAL_ASM for unmatched functions.
 * Based on SM64/MK64 patterns.
 */

#ifndef MACROS_H
#define MACROS_H

/* ============================================================
 * Compiler detection
 * ============================================================ */
#if defined(__sgi) || defined(__GNUC__)
#define COMPILER_IDO   defined(__sgi)
#define COMPILER_GCC   defined(__GNUC__)
#endif

/* ============================================================
 * GLOBAL_ASM - Embed assembly for non-matching functions
 * ============================================================
 * Usage:
 *   #ifdef NON_MATCHING
 *   void my_func(void) {
 *       /* Functional C implementation */
 *   }
 *   #else
 *   GLOBAL_ASM("asm/nonmatching/my_func.s")
 *   #endif
 */
#ifndef NON_MATCHING
#if defined(__sgi)
/* IDO compiler - use #pragma for assembly includes */
#define GLOBAL_ASM(file)
#else
/* GCC - use inline asm include */
#define GLOBAL_ASM(file) asm(".include \"" file "\"")
#endif
#else
#define GLOBAL_ASM(file)
#endif

/* ============================================================
 * Alignment macros
 * ============================================================ */
#if defined(__sgi)
/* IDO uses #pragma align */
#define ALIGNED8
#define ALIGNED16
#else
/* GCC */
#define ALIGNED8  __attribute__((aligned(8)))
#define ALIGNED16 __attribute__((aligned(16)))
#endif

/* ============================================================
 * Function attributes
 * ============================================================ */
#if defined(__sgi)
/* IDO doesn't have these */
#define UNUSED
#define NORETURN
#else
/* GCC */
#define UNUSED __attribute__((unused))
#define NORETURN __attribute__((noreturn))
#endif

/* ============================================================
 * Memory address conversion
 * ============================================================ */
#define SEGMENT_NUMBER(addr)    ((u32)(addr) >> 24)
#define SEGMENT_OFFSET(addr)    ((u32)(addr) & 0x00FFFFFF)

/* Physical to virtual address conversion */
#define PHYSICAL_TO_VIRTUAL(addr) ((void*)((u32)(addr) | 0x80000000))
#define VIRTUAL_TO_PHYSICAL(addr) ((u32)(addr) & 0x1FFFFFFF)

/* ============================================================
 * Array utilities
 * ============================================================ */
#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

/* ============================================================
 * Math helpers
 * ============================================================ */
#define ABS(x)    ((x) >= 0 ? (x) : -(x))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(x, lo, hi) (MIN(MAX((x), (lo)), (hi)))

/* Degrees to radians (N64 uses 0x10000 = 360 degrees) */
#define DEG_TO_BINANG(deg) ((s16)((deg) * (0x10000 / 360.0f)))

/* ============================================================
 * Fixed-point math (arcade compatibility)
 * ============================================================
 * Arcade uses 0x4000 = 1.0 for many calculations
 */
#define FTOFIX(f)  ((s32)((f) * 16384.0f))   /* Float to fixed (14-bit frac) */
#define FIXTOF(x)  ((f32)(x) / 16384.0f)     /* Fixed to float */
#define ITOFIX(i)  ((s32)(i) << 14)          /* Int to fixed */
#define FIXTOI(x)  ((s32)(x) >> 14)          /* Fixed to int (truncate) */

/* ============================================================
 * Static assertions (C89 fallback)
 * ============================================================ */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
/* C89/C99 fallback - generates a compile error on failure */
#define STATIC_ASSERT(cond, msg) typedef char static_assertion_##msg[(cond)?1:-1]
#endif

#endif /* MACROS_H */
