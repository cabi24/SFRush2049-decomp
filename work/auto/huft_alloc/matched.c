
typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef short S16;
typedef unsigned int U32;
typedef int S32;
typedef float F32;
typedef double F64;
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
typedef double f64;
typedef int BOOL;
extern void *memcpy(void *, const void *, unsigned int);
extern void *memset(void *, int, unsigned int);
extern int strcmp(const char *, const char *);
extern char *strcpy(char *, const char *);
extern unsigned int strlen(const char *);
extern int sprintf(char *, const char *, ...);
extern int printf(const char *, ...);
extern float fsqrt(float);
extern float fsin(float);
extern float fcos(float);
extern float fatan2(float, float);
extern double sqrt(double);
extern double sin(double);
extern double cos(double);
extern double atan2(double, double);
extern double fabs(double);
extern s32 gDisplayListHead;
extern volatile unsigned int gDisplayListSize;
s32 huft_alloc(s32 arg0)
{
  int new_var;
  gDisplayListSize += arg0;
  new_var = gDisplayListSize;
  new_var = (new_var - arg0) + gDisplayListHead;
  if (1)
  {
  }
  return new_var;
}
