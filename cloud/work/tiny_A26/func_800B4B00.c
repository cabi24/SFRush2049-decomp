/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
s32 func_800B4B00(void *owner,u8 selector) {
 void *fields=FIELD(FIELD(FIELD(owner,void *,0),void *,44),void *,0);
 switch(selector) {
 case 21:return FIELD(fields,s8,80);
 case 22:return FIELD(fields,s8,81);
 case 23:return FIELD(fields,s8,82);
 case 24:return FIELD(fields,s8,83);
 case 26:return FIELD(fields,s8,84);
 case 27:return FIELD(fields,s8,85);
 case 28:return FIELD(fields,s8,86);
 case 29:return FIELD(fields,s8,87);
 case 30:return FIELD(fields,s8,88);
 case 31:return FIELD(fields,s8,89);
 default:return 0;
 }
}
