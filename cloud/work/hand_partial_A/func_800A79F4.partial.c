s32 func_800A79F4(s32 arg0, PadConfig *arg1, PadConfig *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6)
{
  PadConfig *c;
  s32 i;
  s32 n;

  n = D_801613AC;
  for (i = 0; i < n; i++) {
    if ((s8) (&pad_config)[i].unk16 == 2) {
      break;
    }
  }
  if (i >= 200) {
    return -1;
  }
  c = &(&pad_config)[i];
  if (i >= D_801613AC) {
    n = n + 1;
    D_801613AC = n;
  }
  if (D_8013C234 < n) {
    D_8013C234 = n;
  }
  c->unk4 = arg1;
  c->unk0 = arg2;
  c->unk8 = arg0;
  c->unkA = arg3;
  c->unkE = 0;
  c->unk14 = 0xFF;
  c->unk18 = 0;
  c->unk1A = 0;
  c->unk1C = arg6 - 1;
  c->unk1E = arg5 - 1;
  c->unk16 = 0;
  c->unk15 = 0;
  c->unk10 = arg5;
  c->unk12 = arg6;
  c->unkC = arg4;
  return i;
}
