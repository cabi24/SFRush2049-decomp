typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
typedef struct Obj { s32 w[16]; } Obj;
extern Obj D_80146210[];
extern Obj *D_80149450[];
extern s32 D_80149788;
extern s32 game_state_flags;
void func_800A5488(void)
{
  s32 i;
  for (i = 0; i < 200; i++) D_80149450[i] = &D_80146210[i];
  D_80149788 = 0;
  game_state_flags = 0;
}
