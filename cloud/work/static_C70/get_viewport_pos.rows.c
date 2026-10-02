/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern s16 gViewportX, gViewportOffsetX[], gViewportOffsetY[];
void get_viewport_pos(register s32 *x, register s32 *y) {
    *x = ((s16 (*)[4])gViewportOffsetX)[gViewportX][0];
    *y = ((s16 (*)[4])gViewportOffsetY)[gViewportX][0];
}
