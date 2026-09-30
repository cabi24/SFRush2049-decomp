s32 func_8008B000(u16 arg0, s16 arg1, u16 arg2)
{
  u8 *e;
  u8 *p;
  u8 *q;
  s32 i;

  e = (u8 *) (*(s32 *) ((u8 *) &D_801161F4 + ((arg0 >> 10) * 8)) + (arg0 & 0x3FF) * 0x58);
  if (arg1 >= 0) {
    q = e + arg1 * 0x10;
    if (arg1 >= *(s16 *) (e + 0x16)) {
      return 0;
    }
    *(s16 *) (q + 0x18) = arg2;
    *(u16 *) (q + 0x1A) |= 0x8000;
  } else {
    q = e + arg1 * 0x10;
    p = e;
    for (i = 0; i < *(s16 *) (e + 0x16); i++) {
      *(s16 *) (p + 0x18) = arg2;
      p += 0x10;
      *(u16 *) (q + 0x1A) |= 0x8000;
    }
  }
  return 1;
}
