/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned int u32;typedef unsigned char u8;
typedef struct Gfx {u32 op,address;} Gfx;
void entity_cull_check(Gfx *start,Gfx *end,u32 delta) {
 Gfx *command;
 for(command=start;command<end;command++) {
  u8 opcode=(command->op & 0xFF000000)>>24;
  u32 category=opcode & 0xC0;
  if(category==0x40 || category==0x80)continue;
  if(opcode>=9 && opcode<64)continue;
  if(opcode>=192 && opcode<214)continue;
  {
   u8 next=(command[1].op & 0xFF000000)>>24;
   if(opcode==253) {
    u32 address=command->address;
    command->address=(address&0x0F000000)|((address+delta)&0x00FFFFFF);
   }
   if(opcode==225 && next==4) {
    u32 address=command->address;
    command->address=(address&0x0F000000)|((address+delta)&0x00FFFFFF);
   }
  }
 }
}
