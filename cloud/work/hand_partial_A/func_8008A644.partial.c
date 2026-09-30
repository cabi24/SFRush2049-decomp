void func_8008A644(u16 arg0)
{
  s32 temp_v1;
  if ((u16) D_8012E67A != arg0)
  {
    temp_v1 = D_80149438;
    D_80149438 = temp_v1 + 8;
    *((s32 *) (temp_v1 + 0)) = 0xEE000000;
    *((s32 *) (temp_v1 + 4)) = ((u32) arg0 << 16) | 0;
    D_8012E67A = arg0;
  }
  func_800878E0(0x10);
}
