void func_800D2054(s32 arg0, f32 arg1)
{
  f32 *temp_a2;
  f32 *temp_v0;
  f32 *temp_v0_2;
  f32 *var_t1;
  f32 temp_f0;
  f32 temp_f8;
  f32 *new_var2;
  s32 *new_var;
  s32 var_a3;
  u8 *temp_v1;
  long temp_a1;
  temp_a1 = 5;
  if (((!(state_word_a & 8)) && (gameplay_mode != 1)) && (arg0 < active_player_count))
  {
    temp_v1 = &(&D_80144018)[arg0];
    new_var = (s32 *) (((u8 *) &D_80149A78) + (arg0 << temp_a1));
    temp_a1 = *temp_v1;
    temp_v0 = new_var;
    var_a3 = 0;
    temp_a2 = temp_v0 + temp_a1;
    *temp_a2 = arg1;
    new_var2 = temp_v0;
    if (((s32) temp_a1) > ((arg0 << temp_a1) * 0))
    {
      var_t1 = new_var2;
      do
      {
        var_a3 += 1;
        temp_f8 = (*temp_a2) - (*var_t1);
        var_t1 += 1;
        *((0, temp_a2)) = temp_f8;
      }
      while (var_a3 < ((s32) temp_a1));
    }
    temp_v0_2 = &(&D_80144DA8)[arg0];
    temp_f8 = *temp_a2;
    temp_f0 = temp_f8;
    if (temp_f0 < (*temp_v0_2))
    {
      *temp_v0_2 = temp_f0;
    }
    *temp_v1 = temp_a1 + 1;
  }
}
