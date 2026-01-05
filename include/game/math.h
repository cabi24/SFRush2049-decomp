/**
 * math.h - Fixed-point trigonometry and math utilities
 *
 * Based on arcade source files:
 *   - game/math.c: Fixed-point trig (ssin, scos, satan, possin, posdir, etc.)
 *   - game/mathsubs.c: Fixed-point multiply/divide (MUL, MULUV32, INTRP32, COPYUV)
 *   - game/trigtab.h: Sine, arcsine, arctangent lookup tables
 *
 * These routines are from Hard Drivin' (1985) adapted for arcade racing games.
 *
 * Fixed-point scaling conventions:
 *   - Angles: 16-bit signed where 0x8000 = -pi, 0x7FFF = nearly pi
 *   - Sine/Cosine output: -0x4000 to 0x4000 represents -1.0 to 1.0
 *   - 32-bit angles: 0x40000000 = 90 degrees, 0x80000000 = 180 degrees
 *
 * The square root routine takes unsigned 32-bit and returns largest 16-bit
 * integer which, when squared, is less than the argument.
 */

#ifndef MATH_H
#define MATH_H

#include "types.h"

/******* FIXED-POINT CONSTANTS *******/

/* Maximum sine value (represents 1.0) */
#define MAXSINE         0x4000

/* Number of entries in lookup tables */
#define NSINEVALUES     256
#define NASINEVALUES    256
#define NATANVALUES     256

/* Table scale factors */
#define STABSCALE       (0x4000 / NSINEVALUES)
#define ATABSCALE       (0x8000 / NATANVALUES)
#define ASTABSCALE      (0x8000 / NASINEVALUES)

/* 32-bit table scale factors */
#define STAB32SCL       0x400000L
#define ASTAB32SCL      0x400000L

/* Unit vector constants (0x4000 = 1.0 scale) */
extern const s16 unituvs16[3][3];

/* 32-bit unit vector constants (0x40000000 = 1.0 scale) */
extern const s32 unituvs32[3][3];

/******* LOOKUP TABLES (from trigtab.h) *******/

extern const s16 sintab[NSINEVALUES + 1];
extern const s16 asintab[NASINEVALUES + 1];
extern const s16 atantab[NATANVALUES + 1];

/******* 16-BIT FIXED-POINT TRIG *******/

/**
 * ssin - Fixed-point sine
 * Input: 16-bit angle (0x8000 = -pi, 0x7FFF = pi)
 * Output: -0x4000 to 0x4000 (-1.0 to 1.0)
 */
s16 ssin(u16 angle);

/**
 * scos - Fixed-point cosine
 * Input: 16-bit angle
 * Output: -0x4000 to 0x4000
 */
s16 scos(u16 angle);

/**
 * sincos - Calculate both sine and cosine at once
 */
void sincos(u16 angle, s16 *sina, s16 *cosa);

/**
 * satan - Fixed-point arctangent of y/x for any y and x
 * Returns: 16-bit angle
 */
s16 satan(s16 x, s16 y);

/**
 * sasin - Fixed-point arcsine
 * Input: value in range -0x4000 to 0x4000
 * Output: 16-bit angle
 */
s16 sasin(u16 value);

/**
 * possin - Sine for angles 0 to pi/2 (first quadrant)
 * Uses linear interpolation from lookup table
 */
s16 possin(u16 angle);

/**
 * posdir - Arctangent of y/x for positive x and y
 * Returns angle 0 to pi/4 (0 to 0x2000)
 */
u16 posdir(u16 x, u16 y);

/******* 32-BIT FIXED-POINT TRIG *******/

/**
 * ssin32 - 32-bit fixed-point sine
 * Input: 32-bit angle (0x80000000 = -pi)
 * Output: 32-bit scaled result
 */
s32 ssin32(u32 angle);

/**
 * scos32 - 32-bit fixed-point cosine
 */
s32 scos32(u32 angle);

/**
 * possin32 - 32-bit sine for angles 0 to pi/2
 */
s32 possin32(u32 angle);

/**
 * satan32 - 32-bit arctangent of y/x
 */
s32 satan32(s32 x, s32 y);

/**
 * posdir32 - 32-bit arctangent for positive x and y
 */
u32 posdir32(u32 x, u32 y);

/******* SQUARE ROOT *******/

/**
 * sqrtf - Floating-point square root (standard C library function)
 */
extern f32 sqrtf(f32 x);

/**
 * lsqrt - Integer square root
 * Returns largest 16-bit integer whose square is <= input
 */
u32 lsqrt(u32 value);

/******* FIXED-POINT MULTIPLY/DIVIDE (from mathsubs.c) *******/

/**
 * MUL - Fixed-point multiply (0x4000 * 0x4000 = 0x4000)
 * Result = (a * b) >> 14
 */
s16 MUL(s16 a, s16 b);

/**
 * MULSUM - Fixed-point multiply and add
 * Result = ((a * b) + (c * d)) >> 14
 */
s16 MULSUM(s16 a, s16 b, s16 c, s16 d);

/**
 * MUL32 - Mixed 16x32 fixed-point multiply
 * Returns 32-bit result of (a * b) where a is 0x4000-scaled
 */
s32 MUL32(s16 a, s32 b);

/**
 * MULUV32 - 32-bit unit vector multiply
 * Normalizes result to 0x40000000 scale
 */
s32 MULUV32(s32 uva, s32 uvb);

/**
 * DIV32 - 32-bit integer division
 */
s32 DIV32(s32 a, s32 b);

/**
 * INTRP32 - 32-bit interpolation: (a * b) / c
 * With zero-check on divisor
 */
s32 INTRP32(s32 a, s32 b, s32 c);

/**
 * L_DIV_S - Unsigned long divided by unsigned short
 */
u32 L_DIV_S(u32 a, u16 b);

/******* UNIT VECTOR COPY *******/

/**
 * COPYUV - Copy 3x3 short unit vector matrix
 */
void COPYUV(const s16 suv[3][3], s16 duv[3][3]);

/**
 * COPYUV32 - Copy 3x3 long unit vector matrix
 */
void COPYUV32(s32 suv[3][3], s32 duv[3][3]);

/******* MEMORY/BYTE OPERATIONS *******/

/**
 * intswap - Swap high and low 16-bit halves of a 32-bit int
 */
void intswap(s32 *ip);

/**
 * MOVMEM - Copy memory (like memcpy but arcade-compatible)
 */
void MOVMEM(char *source, char *dest, s32 nbytes);

/**
 * swaparray - Swap bytes in array of 32-bit values
 */
void swaparray(u32 *ptr, u32 size);

/******* 16-BIT UV ROTATION (from math.c) *******/

void uyaw(s16 sint, s16 cost, s16 uv[3][3]);
void upitch(s16 sint, s16 cost, s16 uv[3][3]);
void uroll(s16 sint, s16 cost, s16 uv[3][3]);
void yaw(s16 sint, s16 cost, s16 uv[3][3]);
void pitch(s16 sint, s16 cost, s16 uv[3][3]);
void roll(s16 sint, s16 cost, s16 uv[3][3]);

/******* 32-BIT UV ROTATION (from math.c) *******/

void uyaw32(s32 sint, s32 cost, s32 uv[3][3]);
void upitch32(s32 sint, s32 cost, s32 uv[3][3]);
void uroll32(s32 sint, s32 cost, s32 uv[3][3]);
void yaw32(s32 sint, s32 cost, s32 uv[3][3]);
void pitch32(s32 sint, s32 cost, s32 uv[3][3]);
void roll32(s32 sint, s32 cost, s32 uv[3][3]);

/******* EULER ANGLE EXTRACTION *******/

void GetEuler(s16 uv[3][3], s16 rpy[3]);
void GetEuler32(s32 uv[3][3], s32 rpy[3]);
void GetZoidEuler(s32 uv[3][3], s32 rpy[3]);
void Get32Euler(s32 uv[3][3], s32 rpy[3]);

/******* UNIT VECTOR CONSTRUCTION *******/

void Get32UVS(s32 rpy[3], s32 uv[3][3]);
void GetZoid32UVS(s32 rpy[3], s32 uv[3][3]);
void GetUVS(s16 rpy[3], s16 uv[3][3]);
void GetUnivZoidUVS(s32 zrpy[3], s16 uv[3][3]);
void GetZoidUVS(s32 zrpy[3], s16 uv[3][3]);
void GetZoidUVS16(s16 rpy[3], s16 uv[3][3]);
void UndoZoidUVS(s16 rpy[3], s16 uv[3][3]);
void GetZoidLongUVS(s32 zrpy[3], s32 uv[3][3]);

/******* VECTOR OPERATIONS *******/

s16 DotProduct14(s16 v1[3], s16 v2[3]);
s32 DotProduct16(s16 v1[3], s16 v2[3]);
s32 DotProduct32(s32 v1[3], s32 v2[3]);
void CrossProduct14(s16 v1[3], s16 v2[3], s16 res[3]);
void CrossProduct16(s16 v1[3], s16 v2[3], s16 res[3]);
void CrossProduct32(s32 v1[3], s32 v2[3], s32 res[3]);
void SubVector16(s16 v1[3], s16 v2[3], s16 res[3]);
void SubVector32(s32 v1[3], s32 v2[3], s32 res[3]);
void AddVector16(s16 v1[3], s16 v2[3], s16 res[3]);
void AddVector32(s32 v1[3], s32 v2[3], s32 res[3]);
void ScaleVector16(s16 v[3], s16 s);
void ScaleVector32(s32 v[3], s32 s);

/******* MATRIX OPERATIONS *******/

void Concat32(s32 *a, s32 *b, s32 *c);
s32 DotUV32(s32 v1[3], s32 v2[3]);
s16 GetQuadrant(s16 x, s16 y, s16 z);

/******* UV CONVERSION *******/

void ConvLongUVS(s32 *uv32, s16 *uv16);
void Conv16UVS(s32 uv32[3][3], s16 uv16[3][3]);
void ConvU16UVS(u32 uv32[3][3], s16 uv16[3][3]);

/******* AXIS ADJUSTMENT *******/

void adjust_x(s32 uv[3][3], s32 xmove, s32 movevec[3]);
void adjust_y(s32 uv[3][3], s32 ymove, s32 movevec[3]);
void adjust_z(s32 uv[3][3], s32 zmove, s32 movevec[3]);

/******* ANGLE CONVERSION *******/

u32 AddAngles(s32 angle, s32 delta);
s32 CvtAng32(s16 angle);
s16 CvtAng16(s32 angle);

/******* GEOMETRY UTILITIES *******/

void LookInDirection(s16 LookDir[3], s16 uvs[3][3]);
void MakeUVS(s16 LookDir[3], s16 uvs[3][3]);
void DistPtOnLine(s32 pt[3], s32 from[3], s32 to[3], s32 *zdist, s32 *xdist);
void ScaleUp(s16 original[3], s16 result[3]);
void ScaleDown(s32 original[3], s16 result[3]);
void GetEulerAngles(s32 LookDir[3], s16 rpy[3]);

/******* VECTOR TRANSFORMS *******/

void Transvec(s32 vec[3], s32 res[3], s16 uv[3][3]);
void UTransvec(s32 *vec, s32 *res, s16 uv[3][3]);

/******* FLOAT TRANSFORMS *******/

void ftransvec_math(f32 vec[3], f32 res[3], f32 uvs[3][3]);
void uftransvec(f32 vec[3], f32 res[3], f32 uvs[3][3]);

/******* QUICK DISTANCE *******/

u32 FastDst(s32 xdist, s32 zdist);

/******* COORDINATE SYSTEM CONVERSION *******/

void vecM2D(f32 v[3]);
void veccopyM2D(f32 m[3], f32 d[3]);
void vecD2M(f32 v[3]);
void veccopyD2M(f32 d[3], f32 m[3]);
void matcopyM2D(f32 m[3][3], f32 d[3][3]);
void matcopyD2M(f32 d[3][3], f32 m[3][3]);

#endif /* MATH_H */
