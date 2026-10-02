/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern OSViMode *gViewportDataPtr;
extern s16 gViewportX, gViewportLeftEdge, gViewportTopEdge, gViewportRightEdge, gViewportBottomEdge;
extern s16 gViewportXOverflow, gViewportYOverflow, gViewportScaleX, gViewportScaleY;
extern s16 gViewportBoundsTable[][8], gViewportOffsetX[], gViewportOffsetY[];
extern void display_update(void);
void update_viewport(s32 x, s32 y) {
    s32 left, top, right, bottom;
    register s16 *bounds;
    if (gViewportDataPtr == NULL) return;
    left = gViewportLeftEdge + x;
    top = gViewportTopEdge + y;
    right = gViewportRightEdge + x + gViewportXOverflow;
    bottom = gViewportBottomEdge + y + gViewportYOverflow;
    bounds = gViewportBoundsTable[gViewportX];
    if (bounds[0] < left && left < bounds[1] && bounds[2] < right && right < bounds[3]) {
        gViewportScaleX = ((s16 (*)[4])gViewportOffsetX)[gViewportX][0] + x;
    }
    bounds = gViewportBoundsTable[gViewportX];
    if (bounds[4] < top && top < bounds[5] && bounds[6] < bottom && bottom < bounds[7]) {
        gViewportScaleY = ((s16 (*)[4])gViewportOffsetY)[gViewportX][0] + y;
    }
    display_update();
}
