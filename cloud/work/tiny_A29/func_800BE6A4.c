/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
u8 *func_800BE6A4(u8 *dest,u8 *src) {
 u8 *out,*in;u8 ch;
 out=dest+1;ch=src[0];
 if(ch==255) {
 dest[0]=ch;in=src+1;out=dest+1;
 while(in[0]!=0||in[1]!=0) {out[0]=in[0];out[1]=in[1];out+=2;in+=2;}
 out[0]=0;out++;out[0]=0;
 }else{
 dest[0]=ch;in=src+1;
 while(ch!=0) {ch=*in++;*out++=ch;}
 }
 return dest;
}
