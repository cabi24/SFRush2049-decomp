/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern s16 gViewportX,gViewportOffsetX[],gViewportOffsetY[];
void get_viewport_pos(register s32 *x, register s32 *y) { *x=gViewportOffsetX[gViewportX*4]; *y=gViewportOffsetY[gViewportX*4]; }
