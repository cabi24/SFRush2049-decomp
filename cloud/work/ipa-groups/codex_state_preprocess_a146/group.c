/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef int s32;
typedef struct { u8 pad0[5]; s8 sub; u8 id; u8 pad7[5]; void *ptr; u8 pad10[4]; } Voice;
extern Voice D_80156D38[64];
extern s32 audio_frame_sync(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void display_list_alloc(s32 a);

s32 func_80097694(s32 arg0, s8 arg1)
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

extern unsigned int D_801174B4;
extern void wheel_setup_initial(s32);
extern void tire_compound_set(void);
extern void player_cleanup_slots(void);
void state_change_preprocess(void)
{
    unsigned int state=D_801174B4;
    if(state&0x4000) wheel_setup_initial(57);
    else if(state&0x100) {
        wheel_setup_initial(56);
        tire_compound_set();
    } else if(state&0x80) wheel_setup_initial(60);
    else if(state&0x20000000) {
        wheel_setup_initial(55);
        resource_slot_clear(54);
        resource_slot_clear(58);
        resource_slot_clear(59);
    } else if(state&0x10000000) player_cleanup_slots();
}
