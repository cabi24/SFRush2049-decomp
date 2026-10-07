/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete accepted body; only its used declarations are carried here. */
typedef signed char s8; typedef unsigned char u8; typedef int s32;
extern s32 gameplay_mode;
extern s8 D_80152907;
extern u8 D_801543D4;
s32 func_800F84B0(s32 arg0)
{
  s32 result = 1;
  if (gameplay_mode == 3) {
    if (arg0 == 1 || arg0 == 2 || arg0 == 3 || arg0 == 4) {
      result = 0;
    }
    if (arg0 == 5 && (*((&D_80152907) + (D_801543D4 * 0x3B8))) == 0) {
      result = 0;
    }
  } else if (gameplay_mode == 1) {
    if (arg0 == 0 || arg0 == 1 || arg0 == 2 || arg0 == 5) {
      result = 0;
    }
  } else {
    if (arg0 == 5 || arg0 == 2) {
      result = 0;
    }
  }
  return result;
}

