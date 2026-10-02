/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
void get_viewport_offset(register s32 *x, register s32 *y) {
    *x = gViewportScaleX - ((s16 (*)[4])gViewportOffsetX)[gViewportX][0];
    *y = gViewportScaleY - ((s16 (*)[4])gViewportOffsetY)[gViewportX][0];
}
