/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
#include "rom_tu.h"
extern f32 gScaleSecondsPerTick;
/* Actual signed VI deadline minus current tick, converted/scaled to seconds. */
f32 viGetTimeToDeadline(void)
{
    return (gViAccumTime - gViTickCounter) * gScaleSecondsPerTick;
}
