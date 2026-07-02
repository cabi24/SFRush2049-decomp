/*
 * conveyor_shim.h — compatibility shim so Rush The Rock arcade functions
 * compile in isolation under IDO 5.3 (C89) for match-matrix scoring.
 *
 * This header is the highest-leverage human touchpoint in the pipeline
 * (research R8): every type or macro added here can unlock hundreds of
 * candidate compilations. Keep it neutral — typedefs and stubs only,
 * no behavior. Codegen-relevant sizes must match the arcade's MIPS ABI
 * (ints/longs/pointers 32-bit, which IDO -mips2 already gives us).
 *
 * In C89, calls to undeclared functions are implicitly `int f()` — so
 * unknown *functions* never block compilation. Unknown *types*, *structs*
 * and *globals* do; that is what this file exists to fill in, iteratively,
 * guided by the candidate-compilation coverage report.
 */
#ifndef CONVEYOR_SHIM_H
#define CONVEYOR_SHIM_H

/* ---- Atari/TWI fixed-width scalar types (GUTS convention) ------------- */
typedef unsigned char   U8;
typedef signed char     S8;
typedef unsigned short  U16;
typedef short           S16;
typedef unsigned int    U32;
typedef int             S32;
typedef float           F32;
typedef double          F64;

typedef unsigned char   u8;
typedef signed char     s8;
typedef unsigned short  u16;
typedef short           s16;
typedef unsigned int    u32;
typedef int             s32;
typedef float           f32;
typedef double          f64;

#ifndef BOOL
typedef int             BOOL;
#endif
#ifndef VOID
#define VOID void
#endif

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void *)0)
#endif

/* ---- Common arcade macros --------------------------------------------- */
#ifndef MAX_LINKS
#define MAX_LINKS 8            /* max linked cabinets / players */
#endif
#ifndef MAX_CARS
#define MAX_CARS 12
#endif

/* One-frame time step used all over the game code. */
#ifndef ONE_FRAME
#define ONE_FRAME (1.0F / 60.0F)
#endif

/* ---- libc surface (IDO has these; declare, never define) -------------- */
extern void *memcpy(void *, const void *, unsigned int);
extern void *memset(void *, int, unsigned int);
extern int   strcmp(const char *, const char *);
extern char *strcpy(char *, const char *);
extern unsigned int strlen(const char *);
extern int   sprintf(char *, const char *, ...);
extern int   printf(const char *, ...);

/* ---- math surface ------------------------------------------------------ */
extern float  fsqrt(float);
extern float  fsin(float);
extern float  fcos(float);
extern float  fatan2(float, float);
extern double sqrt(double);
extern double sin(double);
extern double cos(double);
extern double atan2(double, double);
extern double fabs(double);

#endif /* CONVEYOR_SHIM_H */
