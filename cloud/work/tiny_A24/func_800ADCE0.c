typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
void func_800ADCE0(u8 *input,s32 count,u16 *front,u32 marker) {u16 *back=front+count-1;u32 word,value;s32 run;while(count>0){word=((u32)input[0]<<8)+input[1];input+=2;run=word>>13;value=word&0x1FFF;count-=run;do{count--;if(value==marker)*front++=value;else *back--=value;value++;}while(run-- >0);}}
