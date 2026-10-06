s32 func_800B61A8(s32 arg0, s32 arg1, s32 arg2, unsigned char arg3)
{
  unsigned int new_var;
  if (D_8010FFC0 == 0)
  {
    return -1;
  }
  if (arg0 == (-1))
  {
    return -1;
  }
  new_var = entity_flags_apply(arg0, arg1, arg2, arg3);
  return new_var;
}
