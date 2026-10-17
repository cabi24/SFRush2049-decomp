/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* CONTEXT ONLY (w22e): func_800D6160, physics_init_mode0 and physics_init_mode1 copied verbatim (function text byte-identical)
 * from src/blob/groups/codex_mode_init_a18/group.c lines 4137-4156 (func_800D6160, physics_init_mode0, physics_init_mode1), with the declarations they need. The whole
 * a18 file cannot be used as context here because it also defines speed_set and the speed wrappers, whose
 * locked definitions come from codex_vsync_a145 (the whole-program unit makes the same choice). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
typedef struct { u8 pad0[12]; s8 music; s8 sfx; s8 track; s8 mono; } AudioCfg;
extern AudioCfg D_80146108;
extern s32 D_8010FFD4[];
extern s32 D_8011735C;
extern s8 D_8014978C;
extern f32 D_801241A8;
void speed_set(f32,f32,s32,s32);
extern void sync_entry_register(s32, s32);
void func_800D6160(s32);
void func_800D6160(s32 mode) {
 s32 selected; s8 level; u32 seed; f32 amount;
 level=M2C_FIELD(&D_80146108,s8 *,12);
 if(level) {
  selected=D_8010FFD4[D_8014978C];
  if(selected == -1) {
   do {
    seed=(u32)D_8011735C*0x41C64E6D+12345;
    D_8011735C=seed;
    selected=(s32)((f32)(((s32)seed>>16)&0x7FFF)*12.0f/32768.0f);
   } while(selected == 5);
  }
  if(mode) amount=1.0f; else amount=D_801241A8;
  speed_set((f32)level/10.0f,amount,1,0);
  if(M2C_FIELD(&D_80146108,s8 *,14)==12) sync_entry_register(selected,1);
  else if(M2C_FIELD(&D_80146108,s8 *,14)<12) sync_entry_register(M2C_FIELD(&D_80146108,s8 *,14),1);
 }
}
void physics_init_mode0(void) {func_800D6160(0);}
void physics_init_mode1(void) {func_800D6160(1);}
