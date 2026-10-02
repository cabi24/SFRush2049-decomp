/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;typedef unsigned int u32;typedef float f32;
extern f32 D_80114744;extern u32 D_80149B88;
void func_8008A644(u16);
void render_helper(f32 value) {
 D_80114744=value;
 if(value>0.0f) {
  D_80149B88|=0x10;
  func_8008A644((u16)(u32)value);
 }else D_80149B88&=~0x10;
}
