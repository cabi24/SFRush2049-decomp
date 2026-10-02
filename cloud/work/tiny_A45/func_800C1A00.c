/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
typedef struct VecRecord68 {u8 p0[12];f32 x,y,z;u8 p24[44];} VecRecord68;
typedef struct Info32 {u8 p0[22];s16 id;u8 p24[4];VecRecord68 *vectors;} Info32;
typedef struct Ref20 {Info32 *info;u8 p4[8];s16 index;u16 flags;f32 scale;} Ref20;
typedef struct Object112 {u8 p0[108];Ref20 *ref;} Object112;
extern Object112 *D_8013C300[];extern s32 D_8013F1DC;
void func_800C1A00(s32 id,f32 *out) {
 Object112 **objects; s32 i,count;Ref20 *ref;Info32 *info;
 out[0]=0.0f;out[1]=0.0f;out[2]=0.0f;
 count=D_8013F1DC;objects=D_8013C300;i=0;
 if(count>0) {
  do {
   ++i;ref=(*objects)->ref;info=ref->info;
   if(id==info->id) {
    if(ref->flags&8) {
     out[0]=info->vectors[ref->index].x*(-ref->scale);
     out[1]=info->vectors[ref->index].y*(-ref->scale);
     out[2]=info->vectors[ref->index].z*(-ref->scale);
     return;
    }
    out[0]=info->vectors[ref->index].x*ref->scale;
    out[1]=info->vectors[ref->index].y*ref->scale;
    out[2]=info->vectors[ref->index].z*ref->scale;
    return;
   }
   ++objects;
  }while(i<count);
 }
}
