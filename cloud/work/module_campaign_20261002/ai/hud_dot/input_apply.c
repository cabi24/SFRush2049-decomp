/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Config {s32 connected;u32 p4,p8;u16 u12;s16 s14,s16;u16 u18;s16 s20,s22;u8 u24,u25;s8 s26;u8 pad27;s16 bounds[4];u8 pad36[16];u16 index;} Config;
typedef struct Pad {u32 p0,p4;u16 u8;s16 s10,s12;u8 pad14[2];s16 s16,s18;u8 pad20[12];} Pad;
extern Pad D_80140BF0[];
void input_analog_write(u32,s32,s32,s32,s32);void input_status_update(u32,s32,s32,u16);void input_button_adjust(u32,s32);void input_connected_check(u32,s32);
void Input_ApplyPadConfig(Config *config) {
 Pad *pad=&D_80140BF0[config->index];
 s16 s22=config->s22,s20=config->s20,s16v=config->s16,s14=config->s14;
 u32 p4=config->p4,p8=config->p8;u16 u12=config->u12;
 pad->s18=s22;pad->s16=s20;pad->s12=s16v;pad->s10=s14;
 pad->p0=p4;pad->p4=p8;pad->u8=u12;
 input_analog_write(config->index,config->bounds[0],config->bounds[1],config->bounds[2],config->bounds[3]);
 input_status_update(config->index,config->s26,config->u24,config->u18);
 input_button_adjust(config->index,config->u25);
 input_connected_check(config->index,config->connected==-1);
}
