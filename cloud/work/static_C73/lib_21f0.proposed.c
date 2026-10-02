#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
extern s16 gViewportX, gViewportScaleX, gViewportScaleY, gViewportPendingFrames;
extern s16 gViewportOffsetX[], gViewportOffsetY[];
extern OSViMode *gViewportStruct, *gViewportDataPtr;
extern int fcvt(char *,const char *,char *);
extern float gScaleTicksPerSecond,gScaleSecondsPerTick,gViewportFloatC,gViewportFloatD;
extern s16 gViewportY,gViewportLeftEdge,gViewportRightEdge,gViewportTopEdge,gViewportBottomEdge;
extern s16 gViewportXOverflow,gViewportYOverflow,gViewportScale,gViewportScaleYAlt;
extern s16 gViewportOffsetXExtra[],gViewportOffsetYExtra[];
extern OSViMode gViModeTableBase[];
extern void viewport_scale(float,float),display_update(void);
extern s32 lzss_decode(void *,void *);
extern s32 inflate_entry(void *,void *,s32);
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
#pragma GLOBAL_ASM("build/C73/lib_21f0/display_update.s")
void viewport_setup(s32 tv, s32 mode, s32 width, s32 height) {
    if (tv == 0) {
        gScaleTicksPerSecond = 50.0f;
        gScaleSecondsPerTick = gViewportFloatC;
    } else {
        gScaleTicksPerSecond = 60.0f;
        gScaleSecondsPerTick = gViewportFloatD;
    }
    gViewportX = tv;
    gViewportY = mode;
    gViewportStruct = &gViModeTableBase[mode];
    gViewportLeftEdge = (gViewportStruct->comRegs.hStart >> 16) & 0x3FF;
    gViewportRightEdge = gViewportStruct->comRegs.hStart & 0x3FF;
    gViewportTopEdge = (gViewportStruct->fldRegs[0].vStart >> 16) & 0x3FF;
    gViewportBottomEdge = gViewportStruct->fldRegs[0].vStart & 0x3FF;
    if (gViewportDataPtr == NULL) {
        gViewportXOverflow = ((s16 (*)[4])gViewportOffsetXExtra)[gViewportX][0];
        gViewportYOverflow = ((s16 (*)[4])gViewportOffsetYExtra)[gViewportX][0];
        gViewportScaleX = ((s16 (*)[4])gViewportOffsetX)[gViewportX][0];
        gViewportScaleY = ((s16 (*)[4])gViewportOffsetY)[gViewportX][0];
    } else {
        viewport_scale((float)width / (float)gViewportScale, (float)height / (float)gViewportScaleYAlt);
    }
    gViewportScale = width;
    gViewportScaleYAlt = height;
    display_update();
}

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

#pragma GLOBAL_ASM("build/C73/lib_21f0/update_viewport.s")
