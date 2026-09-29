typedef signed char s8; typedef unsigned char u8; typedef int s32;
typedef struct { u8 pad0[5]; s8 sub; u8 id; u8 pad7[5]; void *ptr; u8 pad10[4]; } Voice;
extern Voice D_80156D38[64];
extern s32 audio_frame_sync(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void display_list_alloc(s32 a);

static s32 func_80097694(s32 arg0, s8 arg1)
{
  s32 i;

  for (i = 0; i < 64; i++)
  {
    if ((D_80156D38[i].ptr != 0) && (arg0 == D_80156D38[i].id) && ((arg1 < 0) || (arg1 == D_80156D38[i].sub)))
    {
      return i;
    }
  }
  return -1;
}

static void resource_slot_clear(s32 id)
{
  if (func_80097694(id, -1) < 0)
  {
    display_list_alloc(audio_frame_sync(id, 0, 0, 0, 0));
  }
}

void resource_slots_clear_multiple(void)
{
  resource_slot_clear(54);
  resource_slot_clear(58);
  resource_slot_clear(59);
}

void other_user(s32 x)
{
  resource_slot_clear(x);
}

s32 audio_user(s32 x, s32 y)
{
  return func_80097694(x, y) + func_80097694(y, x);
}
