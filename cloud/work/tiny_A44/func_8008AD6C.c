/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
s32 func_8008AD6C(u32 *command) {
 u32 first=*command;
 u32 opcode=first&0xFF000000;
 if(opcode==0x05000000) {
  *command=(first&0xFFFF0000)|((first&0xFF00)>>8)|((first&0xFF)<<8);
 } else if(opcode==0x07000000 || opcode==0x06000000) {
  u32 second=command[1];
  *command=(first&0xFFFF0000)|((first&0xFF00)>>8)|((first&0xFF)<<8);
  command++;
  *command=(second&0xFFFF0000)|((second&0xFF00)>>8)|((second&0xFF)<<8);
 }
 return 2;
}
