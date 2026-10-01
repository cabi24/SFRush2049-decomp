/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;typedef unsigned int u32;typedef float f32;
extern f32 D_80123DB4,D_80123DB8;f32 sinf(f32);f32 cosf(f32);
void func_800B5898(f32 angle,f32 *matrix) {
 f32 sine,cosine,x,y;s32 i;
 if(angle<D_80123DB4||angle>D_80123DB8) {
 sine=sinf(angle);cosine=cosf(angle);
 for(i=0;i<3;i++) {
 x=matrix[1];y=matrix[2];
 matrix[1]=x*cosine-y*sine;
 matrix[2]=x*sine+y*cosine;
matrix+=3;
 }
 }
}
