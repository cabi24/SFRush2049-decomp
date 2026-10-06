/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete accepted body with minimal declarations; raw O32 handle bits are
 * cast explicitly to the real sound_stop pointer argument at its call. */
typedef signed char s8; typedef int s32;
typedef struct Voice Voice;
extern s32 D_80116DE4, D_80116FE4;
void sound_stop(Voice *);
void sound_handles_clear(s32 arg0)
{
  s8 *entry;
  s8 *handle;
  s32 i;
  s32 h;

  do { entry = (s8 *) &D_80116DE4; do
  {
    if ((entry[0] != 0) && (((i = 0, arg0 != 0)) || (entry[1] == 0)))
    {
      handle = entry;
      do
      {
        h = *((s32 *) (handle + 0xC));
        if (h != 0)
        {
          sound_stop((Voice *)h);
          *((s32 *) (handle + 0xC)) = 0;
        }
        i += 4;
        handle += 4;
      } while (i != 0x14);
      entry[0] = 0;
    }
    entry += 0x20;
  } while (entry != (s8 *) &D_80116FE4); } while (0);
}

