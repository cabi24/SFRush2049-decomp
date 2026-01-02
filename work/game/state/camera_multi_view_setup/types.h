/*
 * Common types for N64 decompilation
 * Include this in your implementation
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

/* Floating point */
typedef float f32;
typedef double f64;

/* Volatile versions (for hardware registers) */
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

/* Pointer types */
typedef void* voidptr;

/* Boolean */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /* TYPES_H */
