f32 func_8008C768(f32 arg0, f32 arg1)
{
  f32 t;

  t = arg0;
  if (arg0 == (arg0 + arg1))
  {
    if (arg0 == 0.0f)
    {
      return 0.0f;
    }
    if (t > 0.0f)
    {
      return D_801238F4;
    }
    return D_801238F8;
  }
  if (arg1 < 0.0f)
  {
    if (t >= 0.0f)
    {
      return D_801238FC - func_8008C680((-t) / arg1);
    }
    return func_8008C680(t / arg1) + D_80123900;
  }
  if (t > 0.0f)
  {
    return func_8008C720(t / arg1);
  }
  return -func_8008C720((-t) / arg1);
}
