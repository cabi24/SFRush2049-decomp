SoundState *sound_control(s16 arg0, s16 arg1, SoundClearRecord *arg2, s16 arg3)
{
  SoundClearRecord *rec;
  SoundState *first;
  SoundState *prev;
  SoundState *cur;
  s32 i;

  if (arg3 <= 0)
  {
    return (void *) 0;
  }
  first = 0;
  prev = 0;
  rec = arg2;
  for (i = 0; i < arg3; i++)
  {
    cur = func_800b3704(rec->unk0, rec->unk4 + arg0, rec->unk6 + arg1, rec->unk20 & 0x80000000);
    cur->unk18 = rec->unk18;
    cur->unk12 = rec->unk14;
    cur->unk20 = rec->unk10;
    cur->unk1C = rec->unkC;
    cur->unk22 = rec->unk12;
    cur->unk1E = rec->unkE;
    if (rec->unk8 >= 0)
    {
      cur->unk14 = rec->unk8;
    }
    if (rec->unkA >= 0)
    {
      cur->unk16 = rec->unkA;
    }
    Input_ApplyPadConfig(cur);
    if (rec->unk1C != 0)
    {
      cur->unk2C = rec->unk20 & 0x7FFFFFFF;
      if (rec->unk0 == -1)
      {
        cur->unk8 = rec->unk1C;
        cur->unk4 = cur;
        Input_ApplyPadConfig(cur);
      }
      else
      {
        cur->unk28 = rec->unk1C;
        if (rec->unk1C(cur) == 0)
        {
          sound_stop((s32) cur);
          return first;
        }
      }
    }
    if (i == 0)
    {
      first = cur;
    }
    else
    {
      prev->unk3C = cur;
    }
    prev = cur;
    rec++;
  }
  return first;
}
