/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;typedef unsigned char u8;
typedef struct Descriptor72 {u8 opaque[8];int orthographic;f32 horizontal,vertical,tan_horizontal,tan_vertical,inverse_horizontal,inverse_vertical,width,height,near_plane,far_plane,aspect;u8 tail[16];} Descriptor72;
extern Descriptor72 D_8017A510[];
extern int D_8002AFC4;
extern f32 D_80123BC8,D_80123BCC;
extern f32 func_800A557C(f32),func_8008C720(f32);
#define DEFAULT_ANGLE (*(const f32 *)((const u8 *)&D_80123BCC+4))
Descriptor72 *exhaust_smoke_effect(int index,f32 horizontal,f32 vertical,f32 width,f32 height,f32 near_plane,f32 far_plane) {
 Descriptor72 *entry=&D_8017A510[index];
 f32 angle_x,angle_y,pixels;
 entry->height=height;
 entry->width=width;
 entry->near_plane=near_plane;
 entry->far_plane=far_plane;
 if(D_8002AFC4>=241)entry->aspect=width/(height*0.5f);
 else entry->aspect=width/height;
 if(horizontal>0.0f) {
  angle_x=horizontal*D_80123BC8;
  if(vertical>0.0f)angle_y=vertical*D_80123BCC;
  else {
   if(D_8002AFC4>=241)pixels=height*0.5f;
   else pixels=height;
   angle_y=func_8008C720(func_800A557C(angle_x*0.5f)*pixels/width)*2.0f;
  }
  entry->orthographic=0;
 } else {
  angle_x=angle_y=DEFAULT_ANGLE;
  entry->orthographic=1;
 }
 entry->horizontal=angle_x;
 entry->vertical=angle_y;
 entry->tan_horizontal=func_800A557C(angle_x*0.5f);
 entry->tan_vertical=func_800A557C(entry->vertical*0.5f);
 entry->inverse_vertical=0.5f/entry->tan_vertical;
 entry->inverse_horizontal=0.5f/entry->tan_horizontal;
 return entry;
}
