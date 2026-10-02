
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
u8 *memchr(u8 *arg0, volatile unsigned int arg1, s32 arg2)
{
  u8 *sp4;
  s32 var_a2;
  int new_var;
  if (!new_var)
  {
  }
  sp4 = arg0;
  var_a2 = arg2 - 1;
  new_var = 0;
  if (arg2 != new_var)
  {
    loop_1:
    if ((*sp4) == arg1)
    {
      if (1)
      {
        return sp4;
      }
    }

    sp4 += 1;
    if (var_a2 == 0)
    {
      var_a2 -= 1;
      goto block_4;
    }
    var_a2 += 0;
    goto loop_1;
  }
  block_4:
  return (void *) new_var;

}
