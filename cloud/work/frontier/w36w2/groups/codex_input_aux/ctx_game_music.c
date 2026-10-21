/* N64 _StartGameMusic and its two actual public wrappers.
 * MAT3 audio.c supplies the one song local, conditional track-table selection
 * and genuine music-volume/Random calls. The N64 twelve-song limit, retry for
 * song five, nonempty resume wrapper and queued audio backend remain native.
 * MAT3 source status is reported 100% fuzzy, not a fresh strict acceptance.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct { u8 pad0[12]; s8 music; s8 sfx; s8 track; s8 mono; u8 remaining[25]; } AudioCfg;
extern AudioCfg D_80146108;
extern s32 D_8010FFD4[19];
extern s8 D_8014978C;
extern void sync_entry_register(s32, s32);
f32 func_8008B2E4(f32);
void speed_mode0_wrapper(f32,f32);
void func_800D6160(s32);
void func_800D6160(s32 resume)
{
 s32 song;
 if(D_80146108.music) {
  if(D_8010FFD4[D_8014978C] == -1) {
   do { song=(s32)func_8008B2E4(12.0f); } while(song == 5);
  } else song=D_8010FFD4[D_8014978C];
  speed_mode0_wrapper(D_80146108.music / 10.0f,resume ? 1.0f : 0.05f);
  if(D_80146108.track == 12) sync_entry_register(song,1);
  else if(D_80146108.track < 12) sync_entry_register(D_80146108.track,1);
 }
}
void physics_init_mode0(void) {func_800D6160(0);}
void physics_init_mode1(void) {func_800D6160(1);}
