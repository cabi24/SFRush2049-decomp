#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
extern s16 gViewportX, gViewportScaleX, gViewportScaleY, gViewportPendingFrames;
extern s16 gViewportOffsetX[], gViewportOffsetY[];
extern OSViMode *gViewportStruct, *gViewportDataPtr;
extern int fcvt(char *,const char *,char *);
extern s32 lzss_decode(void *,void *);
extern s32 inflate_entry(void *,void *,s32);
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
#pragma GLOBAL_ASM("build/C72/lib_21f0/display_update.s")
#pragma GLOBAL_ASM("build/C72/lib_21f0/viewport_setup.s")
void display_mode_tick(void) {
    if (gViewportPendingFrames == 0) return;
    if (--gViewportPendingFrames == 0) {
        if ((gViewportDataPtr->fldRegs[0].yScale & 0xFFFF) == (gViewportStruct->fldRegs[0].yScale & 0xFFFF)) {
            osSetIntMask(0);
        }
        osSetThreadPri(gViewportDataPtr);
        osViSetSpecialFeatures(0xAA);
    }
}

s32 get_tv_offset(void) { s32 offset; if(osTvType==1) offset=0; else if(osTvType==0) offset=14; else offset=28; return offset; }

void apply_display_mode(void) { osSetThreadPri(gViewportStruct); }

void get_viewport_pos(register s32 *x, register s32 *y) {
    *x = ((s16 (*)[4])gViewportOffsetX)[gViewportX][0];
    *y = ((s16 (*)[4])gViewportOffsetY)[gViewportX][0];
}

void get_viewport_offset(register s32 *x, register s32 *y) {
    *x = gViewportScaleX - ((s16 (*)[4])gViewportOffsetX)[gViewportX][0];
    *y = gViewportScaleY - ((s16 (*)[4])gViewportOffsetY)[gViewportX][0];
}

#pragma GLOBAL_ASM("build/C72/lib_21f0/update_viewport.s")
