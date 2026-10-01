/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
s32 func_800B66B0(u8 *text,s16 limit) {
 s32 count=0,offset;
 if(text[0]==255) {
 offset=1;
 do {
 offset+=2;count++;
 if(text[offset-1]==0 && text[offset-2]==0)break;
 }while(limit<0||count<=limit);
 }else{
 offset=0;
 do {
 u8 ch=*text++;offset++;count++;
 if(ch==0)break;
 }while(limit<0||count<=limit);
 }
 return offset;
}
