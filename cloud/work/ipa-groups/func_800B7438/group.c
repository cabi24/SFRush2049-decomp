typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
extern s8 D_801147C4;
s32 D_801551E8[10];
extern s32 D_80155210;
void func_800B73E4(void)
{
  s32 i;
  if (D_801147C4 == 0) {
    D_801147C4 = 1;
    for (i = 0; i < 10; i++)
      D_801551E8[i] = 0;
  }
}
void func_800B7438(s32 arg0)
{
  s32 *p;
  if (D_801147C4 == 0) {
    func_800B73E4();
  }
  for (p = D_801551E8; ; ) {
    if (*p == 0) { *p = arg0; break; }
    p++;
    if (p == &D_80155210) break;
  }
}
void __standin_input_init_flag_get(void)
{
  func_800B73E4();
}
