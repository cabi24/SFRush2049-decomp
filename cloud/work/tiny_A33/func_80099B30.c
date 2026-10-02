/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Gfx {u32 w0,w1;} Gfx;
extern u32 D_8017A4B0;extern s32 D_80161430;
void func_80099B30(Gfx **dl,void *texture) {
 Gfx *p=*dl,*cmd;u32 address=FIELD(texture,u32,20);
 if(D_8017A4B0!=address || D_80161430) {
 D_8017A4B0=address;
 cmd=p++;cmd->w0=0xFD100000;cmd->w1=FIELD(texture,u32,20);
 cmd=p++;cmd->w1=0;cmd->w0=0xE8000000;
 cmd=p++;cmd->w0=0xF5000000|((FIELD(texture,u8,16)+256)&511);cmd->w1=0x07000000;
 cmd=p++;cmd->w0=0xE6000000;cmd->w1=0;
 cmd=p++;cmd->w0=0xF0000000;cmd->w1=0x07000000|(((FIELD(texture,u8,17)-FIELD(texture,u8,16))&1023)<<14);
 cmd=p++;cmd->w0=0xE7000000;cmd->w1=0;
 *dl=p;
 }
}
