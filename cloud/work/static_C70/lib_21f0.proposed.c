#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
extern s16 gViewportX, gViewportScaleX, gViewportScaleY;
extern s16 gViewportOffsetX[], gViewportOffsetY[];
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
#pragma GLOBAL_ASM("build/C70/lib_21f0/display_update.s")
#pragma GLOBAL_ASM("build/C70/lib_21f0/viewport_setup.s")
#pragma GLOBAL_ASM("build/C70/lib_21f0/display_mode_tick.s")
s32 get_tv_offset(void) { s32 offset; if(osTvType==1) offset=0; else if(osTvType==0) offset=14; else offset=28; return offset; }

#pragma GLOBAL_ASM("build/C70/lib_21f0/apply_display_mode.s")
void get_viewport_pos(register s32 *x, register s32 *y) {
    *x = ((s16 (*)[4])gViewportOffsetX)[gViewportX][0];
    *y = ((s16 (*)[4])gViewportOffsetY)[gViewportX][0];
}

void get_viewport_offset(register s32 *x, register s32 *y) {
    *x = gViewportScaleX - ((s16 (*)[4])gViewportOffsetX)[gViewportX][0];
    *y = gViewportScaleY - ((s16 (*)[4])gViewportOffsetY)[gViewportX][0];
}

#pragma GLOBAL_ASM("build/C70/lib_21f0/update_viewport.s")
