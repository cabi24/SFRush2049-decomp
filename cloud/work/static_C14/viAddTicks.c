/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern f32 gScaleTicksPerSecond;
void viAddTicks(f32 ticks) { gViAccumTime+=(s32)(ticks*gScaleTicksPerSecond); }
