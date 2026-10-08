/* Host-only adapter for the semantic model; never a target closure member. */
#include "semantic_bus.h"
typedef u32 (*read_fn)(u32,u32);
typedef void (*write_fn)(u32,u32,u32);
typedef u32 (*call_fn)(u32,u32,u32,u32,u32);
static read_fn read_bus;
static write_fn write_bus;
static call_fn call_bus;
void set_bus(read_fn r, write_fn w, call_fn c) { read_bus=r; write_bus=w; call_bus=c; }
static u32 bits(f32 f) { union {f32 f; u32 u;} v; v.f=f; return v.u; }
static f32 value(u32 u) { union {f32 f; u32 u;} v; v.u=u; return v.f; }
u32 R32(u32 a) { return read_bus(a,4); }
u32 RU16(u32 a) { return read_bus(a,2); }
u32 RU8(u32 a) { return read_bus(a,1); }
f32 RF(u32 a) { return value(R32(a)); }
void W32(u32 a,u32 v) { write_bus(a,4,v); }
void W16(u32 a,u32 v) { write_bus(a,2,v); }
void W8(u32 a,u32 v) { write_bus(a,1,v); }
void WF(u32 a,f32 v) { W32(a,bits(v)); }
void scene_remove(s16 h) { call_bus(0x80090254u,h,0,0,0); }
void scene_hide(s32 h,s32 m,u32 mask) { call_bus(0x8008AE8Cu,h,m,mask,0); }
void scene_show(s32 h,s32 m,u32 mask) { call_bus(0x8008B0D8u,h,m,mask,0); }
void scene_color(s16 h,u32 color) { call_bus(0x8008E06Cu,h,color,0,0); }
u32 scene_transform(s16 h) { return call_bus(0x8008B3A0u,h,0,0,0); }
void matrix_copy(u32 src,u32 dst) { call_bus(0x8008D6B0u,src,dst,0,0); }
s32 scene_create(s32 model,u32 transform,s16 parent,u32 flags) { return (s32)call_bus(0x8008E398u,model,transform,parent,flags); }
f32 random_float(f32 max) { return value(call_bus(0x8008B2E4u,bits(max),0,0,0)); }
void scene_model(s16 h,u16 model) { call_bus(0x80090770u,h,model,0,0); }
void scene_position(s16 h,u32 xyz,u32 matrix) { call_bus(0x8008D6FCu,h,xyz,matrix,0); }
void matrix_roll(f32 angle,u32 matrix) { call_bus(0x8009EA68u,bits(angle),matrix,0,0); }
void matrix_scale(u32 src,u32 dst,f32 scale) { call_bus(0x8008B32Cu,src,dst,bits(scale),0); }
void scene_texture(s16 h,u32 texture,s32 slot) { call_bus(0x8008D870u,h,texture,slot,0); }
void invalid_domain(s32 reason) { call_bus(0u,reason,0,0,0); }
