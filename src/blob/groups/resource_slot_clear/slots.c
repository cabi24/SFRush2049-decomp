/* IPA group resource_slot_clear, file 2 of 2.
 * resource_slot_clear takes its argument in $t0 (IDO -O3 interprocedural
 * register allocation) and keeps it live across func_80097694, whose
 * temporaries are therefore confined to $t6-$t9. other_user is a stand-in for
 * the real second caller (state_change_preprocess).
 */
typedef signed char s8; typedef unsigned char u8; typedef int s32;
typedef struct { u8 pad0[5]; s8 sub; u8 id; u8 pad7[5]; void *ptr; u8 pad10[4]; } Voice;
extern Voice D_80156D38[64];
extern s32 audio_frame_sync(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void display_list_alloc(s32 a);

extern s32 func_80097694(s32 arg0, s8 arg1);
void resource_slot_clear(s32 id)
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

