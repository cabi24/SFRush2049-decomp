#ifndef BATTLE_SEMANTIC_BUS_H
#define BATTLE_SEMANTIC_BUS_H
/* Verification-only bus. Exact big-endian memory is supplied by the harness.
 * Addresses are unsigned integers; no cross-object C pointer is manufactured. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
extern u32 R32(u32);
extern u32 RU16(u32);
extern u32 RU8(u32);
#define RS16(a) ((s16)RU16(a))
#define RS8(a) ((s8)RU8(a))
extern f32 RF(u32);
extern void W32(u32,u32);
extern void W16(u32,u32);
extern void W8(u32,u32);
extern void WF(u32,f32);
extern void scene_remove(s16);
extern void scene_hide(s32,s32,u32);
extern void scene_show(s32,s32,u32);
extern void scene_color(s16,u32);
extern u32 scene_transform(s16);
extern void matrix_copy(u32,u32);
extern s32 scene_create(s32,u32,s16,u32);
extern f32 random_float(f32);
extern void scene_model(s16,u16);
extern void scene_position(s16,u32,u32);
extern void matrix_roll(f32,u32);
extern void matrix_scale(u32,u32,f32);
extern void scene_texture(s16,u32,s32);
extern void invalid_domain(s32);
#endif
