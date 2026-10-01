/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Command {u32 word0,word1;} Command;
void func_800BD080(Command *list,u32 value) {
 s32 op,kind;
 while(1) {
 op=(u8)((list->word0&0xFF000000)>>24);kind=op&0xC0;
 if(kind==64||kind==128||(op>=9&&op<64)||(op>=192&&op<214)) {list++;continue;}
 if(op==253) {list->word1=value;return;}
 if(op==223)return;
 list++;
 }
}
