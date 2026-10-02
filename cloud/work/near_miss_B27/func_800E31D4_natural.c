/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef signed short s16; typedef signed int s32; typedef unsigned char u8; typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern void func_800E313C(void *,f32,f32),func_800E30F4(void *),func_800E30AC(void *);
void func_800E31D4(void *arg0) {
 f32 factor,low,high;
 s16 gear;
 factor=(M2C_FIELD(arg0,f32*,0x3D0)+3.0f)*0.25f;
 low=M2C_FIELD(M2C_FIELD(arg0,void**,4),f32*,0x14)*M2C_FIELD(M2C_FIELD(arg0,void**,0),f32*,0xAC)*factor;
 high=M2C_FIELD(M2C_FIELD(arg0,void**,4),f32*,0x14)*M2C_FIELD(M2C_FIELD(arg0,void**,0),f32*,0xB0)*factor;
 gear=M2C_FIELD(arg0,s16*,0x3F6);
 if(gear==0 || gear==-1) { M2C_FIELD(arg0,s16*,0x3F4)=gear; return; }
 gear=M2C_FIELD(arg0,s16*,0x3F4);
 if(gear==0 || gear==-1) func_800E313C(arg0,low,high);
 if(low<M2C_FIELD(arg0,f32*,0x408)) func_800E30F4(arg0);
 if(M2C_FIELD(arg0,f32*,0x408)<high) func_800E30AC(arg0);
}
