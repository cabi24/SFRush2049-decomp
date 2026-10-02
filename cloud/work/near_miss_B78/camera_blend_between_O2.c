/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef float f32;
typedef struct Basis {f32 m[9];} Basis;
typedef struct Record2056 {u8 prefix[1940];f32 position[3];Basis basis;u8 tail[68];} Record2056;
typedef struct Node64 {u8 opaque[40];f32 position[3];u8 tail[12];} Node64;
extern int D_80150F80;
extern Node64 *D_80150F38;
extern f32 D_80123E74;
extern void func_800A61B0(f32 *,f32 *,Basis *);
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
void camera_blend_between(Record2056 *record,f32 *output) {
 f32 difference[3],view[3],limit;
 Node64 *node;
 Basis *basis=&record->basis;
 if(D_80150F80!=0) {
  for(node=D_80150F38;node<D_80150F38+(D_80150F80-1);node++) {
   difference[0]=node->position[0]-record->position[0];
   difference[1]=node->position[1]-record->position[1];
   difference[2]=node->position[2]-record->position[2];
   func_800A61B0(difference,view,basis);
   if(view[2]<0.0f || view[2]>225.0f || fabsf(view[0])>14.0f)continue;
   if(view[2]<125.0f)limit=0.75f;
   else if(view[2]<225.0f)limit=1.0f-(225.0f-view[2])*D_80123E74;
   else limit=1.0f;
   if(limit<*output)*output=limit;
  }
 }
}
