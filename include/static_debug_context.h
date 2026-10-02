#ifndef RUSH_STATIC_DEBUG_CONTEXT_H
#define RUSH_STATIC_DEBUG_CONTEXT_H
#include "../src/rom/rom_tu.h"

/* Baseline C68-C83 debug-recipe context:26 native bodies/8656 static bytes.
 * Declarations and genuine IEEE carrier only; no owned storage or ROM claims.
 * Existing SDK/m2c declarations remain unchanged. Final formatter bodies are
 * separately sourced BSD C81; complete notices travel with their sources. */
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
extern void dma_queue_init(void);
extern s32 lzss_decode(void *, void *);
extern s32 inflate_entry(void *, void *, s32);

typedef struct {
    unsigned sign:1;
    unsigned exponent:11;
    unsigned fraction:20;
    unsigned low;
} DoubleBits;
typedef union {
    double value;
    DoubleBits bits;
} DoubleUnion;

extern float gScaleTicksPerSecond, gScaleSecondsPerTick;
extern float gViewportFloatA, gViewportFloatB, gViewportFloatC, gViewportFloatD;
extern s16 gViewportX, gViewportY;
extern s16 gViewportLeftEdge, gViewportRightEdge;
extern s16 gViewportTopEdge, gViewportBottomEdge;
extern s16 gViewportXOverflow, gViewportYOverflow;
extern s16 gViewportScaleX, gViewportScaleY, gViewportScale, gViewportScaleYAlt;
extern s16 gViewportPendingFrames;
extern s16 gViewportOffsetXExtra[], gViewportOffsetYExtra[];
/* gViewportOffsetX/Y retain their accepted existing m2c declarations. */
extern s16 gViewportBoundsTable[][8];
extern OSViMode *gViewportStruct, *gViewportDataPtr;
extern OSViMode gViModeTableBase[], gViewportBuffer[];
extern void viewport_scale(float, float), display_update(void);
extern u32 osSetGlobalIntMask(u32);

extern OSThread gIdleThread, gGameThread;
extern u8 gStackIdle[];
extern void __osInitialize_common(void), idle_thread_entry(void *), game_init(void *);
extern OSMesgQueue gViModeTable;
extern OSMesg gViModeLan1[];
/* gStackGame retains its accepted existing m2c declaration. */

extern double modf(double, double *);
extern double gPerspFov, gPerspAspect;
extern u8 *__round_helper(double, s32 *, u8 *, u8 *, u8, u8 *);
extern u8 *__write_exponent(u8 *, s32, s32);
/* Historical fcvt is the real three-argument formatter worker. */
extern int fcvt(char *, const char *, char *);

#endif /* RUSH_STATIC_DEBUG_CONTEXT_H */
