void func_800972C4(s32 arg0)
{
  s8 temp_v0;
  void *temp_v1;
  temp_v1 = (s32 *) ((arg0 * 0x14) + ((u8 *) (&D_80156D38)));
  temp_v0 = (&D_801234AC)[*((u8 *) (((s8 *) temp_v1) + 6))];
  *((s8 *) (((s8 *) temp_v1) + 3)) = (&D_801234AC)[*((u8 *) (((s8 *) temp_v1) + 6))];
  if ((temp_v0 != 0) && (arg0 >= ((s32) D_80140BDC)))
  {
    D_80140BDC = arg0 + 1;
  }
}