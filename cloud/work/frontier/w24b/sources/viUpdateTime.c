/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* viUpdateTime (static, boot 0x800013F4, 252 B): per-frame: delta ticks since the last snapshot; accumulate when enabled, else elapsed
 * seconds since the accumulation base; clamp delta to 5 ticks; delta seconds.
 * Segment 0x1f50 (src/rom/lib_1050.c), IDO 5.3. Rush frame-timing code (VI tick counter -> scheduler
 * delta/elapsed seconds); no library ancestor. w24b: the whole 0x1f50 segment is `-g1 -O1 -Wab,-r4300_mul`
 * (the flagset of the adjacent lib_1050_sdk scheduler), not the -g0 -O2 the layout derives from the two O2
 * locks; at -g1 -O1 viDeadlinePassed is 48 B and absorbs the 20-B "viStub" slot (its two trailing `jr ra; nop`
 * pairs), see RESULTS.md. gViTickCounter / gViLastTick are `volatile` (interrupt-updated tick counter and
 * its snapshot): the retail load order in viUpdateTime's first statement needs both volatile (as1 keeps
 * volatile loads in order); TU integration needs rom_tu.h to declare them volatile (patch in RESULTS.md).
 * No shaping devices, no unused locals. */
typedef float f32;
extern volatile int gViTickCounter;
extern volatile int gViLastTick;
extern int gScTimeEnabled;
extern int gViFrameDelta;
extern int gViAccumTotal;
extern int gViAccumTime;
extern f32 gScaleSecondsPerTick;
extern f32 __osScDeltaTime;
extern f32 __osScElapsedTime;

void viUpdateTime(void) {
    gViFrameDelta = gViTickCounter - gViLastTick;
    gViLastTick = gViTickCounter;
    if (gScTimeEnabled) {
        gViAccumTotal += gViFrameDelta;
        gViAccumTime += gViFrameDelta;
    } else {
        __osScElapsedTime = (gViTickCounter - gViAccumTotal) * gScaleSecondsPerTick;
    }
    if (gViFrameDelta > 5) {
        gViFrameDelta = 5;
    }
    __osScDeltaTime = gViFrameDelta * gScaleSecondsPerTick;
}
