#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
#pragma GLOBAL_ASM("build/C68/lib_21f0/display_update.s")
#pragma GLOBAL_ASM("build/C68/lib_21f0/viewport_setup.s")
#pragma GLOBAL_ASM("build/C68/lib_21f0/display_mode_tick.s")
s32 get_tv_offset(void) { s32 offset; if(osTvType==1) offset=0; else if(osTvType==0) offset=14; else offset=28; return offset; }

#pragma GLOBAL_ASM("build/C68/lib_21f0/apply_display_mode.s")
#pragma GLOBAL_ASM("build/C68/lib_21f0/get_viewport_pos.s")
#pragma GLOBAL_ASM("build/C68/lib_21f0/get_viewport_offset.s")
#pragma GLOBAL_ASM("build/C68/lib_21f0/update_viewport.s")
