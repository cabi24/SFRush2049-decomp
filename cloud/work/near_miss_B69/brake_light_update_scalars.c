/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;typedef unsigned char u8;typedef signed short s16;
typedef struct Transform48 {f32 basis[9],position[3];} Transform48;
typedef struct Descriptor72 {u8 opaque[28];f32 horizontal,vertical,width,height,offset_x,offset_y;u8 tail[20];} Descriptor72;
extern Descriptor72 D_8017A510[];
extern f32 D_80123BD4,D_80123BD8;
extern void func_800A61B0(f32 *,f32 *,Transform48 *);
void brake_light_update(int index,f32 *position,Transform48 *transform,f32 *view_output,s16 *screen_output) {
 f32 difference[3],view[3],screen_x,screen_y,inverse;
 Descriptor72 *descriptor;
 difference[0]=position[0]-transform->position[0];
 difference[1]=position[1]-transform->position[1];
 difference[2]=position[2]-transform->position[2];
 func_800A61B0(difference,view,transform);
 if(view[2]<2.5f)view[2]=2.5f;
 inverse=1.0f/view[2];
 descriptor=&D_8017A510[index];
 screen_x=view[0]*inverse*descriptor->horizontal*descriptor->width+descriptor->offset_x;
 screen_y=descriptor->offset_y-view[1]*inverse*descriptor->vertical*descriptor->height;
 if(screen_x<-32768.0f)screen_output[0]=-32768;
 else if(screen_x>D_80123BD4)screen_output[0]=32767;
 else screen_output[0]=(int)screen_x;
 if(screen_y<-32768.0f)screen_output[1]=-32768;
 else if(screen_y>D_80123BD8)screen_output[1]=32767;
 else screen_output[1]=(int)screen_y;
 if(view_output!=0) {
  view_output[0]=view[0];
  view_output[1]=view[1];
  view_output[2]=view[2];
 }
}
