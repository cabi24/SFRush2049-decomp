/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef struct Vec3 {f32 x,y,z;} Vec3;
typedef struct Basis {f32 m[9];} Basis;
extern void *input_deadzone_apply(f32 *,f32 *,Basis *,f32,int,int);
extern f32 func_8008E0B8(f32 *);
int entity_iterate(f32 *first,f32 *second) {
 Basis basis;
 Vec3 difference;
 if(input_deadzone_apply(second,first,&basis,0.5f,1,6)) {
  difference.x=second[0]-first[0];
  difference.y=second[1]-first[1];
  difference.z=second[2]-first[2];
  func_8008E0B8(&difference.x);
  first[0]=difference.x*0.25f+first[0];
  first[1]=difference.y*0.25f+first[1];
  first[2]=difference.z*0.25f+first[2];
 }
 return 0;
}
