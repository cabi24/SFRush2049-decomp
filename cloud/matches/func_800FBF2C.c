/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed short s16;
typedef signed int s32;
extern float D_801247F8;
extern s16 D_80152032;
extern s32 state_word_b;
float viGetTimeToDeadline();
void func_800FBF2C(void)
{
  D_80152032 = viGetTimeToDeadline() + D_801247F8;
  if (D_80152032 <= 0) {
    state_word_b = 0x100000;
  }
}
