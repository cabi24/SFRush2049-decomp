/* Existing NextPathPt body, unchanged. Redundant generated declarations removed. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
extern u16 D_8012E5E8;
extern s32 D_80151CE8;
s16 func_800B9338(s16 arg0, s16 arg1)
{
  if ((arg0 + 1) == (*((u16 *) (((u8 *) (&D_8012E5E8)) + (arg1 * 8)))))
  {
    return *((s16 *) (((u8 *) (((*((s16 *) (((s8 *) (&D_80151CE8)) + 2))) * 0x50) + ((u8 *) ((0, &D_80151CE8))))) + (arg1 * 2) + 0x30));
  }
  return (s16) ((arg0 & 0xFFFFFFFFu) + 1);
}
