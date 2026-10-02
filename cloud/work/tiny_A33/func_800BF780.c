/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
void func_800BF780(f32 m[3][3],f32 src[3][3],f32 out[3][3]) {
 s32 i;
 for(i=0;i<3;i++) {
 out[i][0]=m[2][0]*src[i][2]+(src[i][0]*m[0][0]+src[i][1]*m[1][0]);
 out[i][1]=m[2][1]*src[i][2]+(src[i][0]*m[0][1]+src[i][1]*m[1][1]);
 out[i][2]=m[2][2]*src[i][2]+(src[i][0]*m[0][2]+src[i][1]*m[1][2]);
 }
}
