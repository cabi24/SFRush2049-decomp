/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern OSViMode *gViewportDataPtr;
extern s16 gViewportX, gViewportLeftEdge, gViewportTopEdge, gViewportRightEdge, gViewportBottomEdge;
extern s16 gViewportXOverflow, gViewportYOverflow, gViewportScaleX, gViewportScaleY;
extern s16 gViewportBoundsTable[][8], gViewportOffsetX[], gViewportOffsetY[];
extern void display_update(void);
void update_viewport(s32 x, s32 y) {
    s32 left, top, right, bottom;
    if (gViewportDataPtr == NULL) return;
    left = gViewportLeftEdge + x;
    top = gViewportTopEdge + y;
    right = gViewportRightEdge + x + gViewportXOverflow;
    bottom = gViewportBottomEdge + y + gViewportYOverflow;
    if (left > gViewportBoundsTable[gViewportX][0] && left < gViewportBoundsTable[gViewportX][1] && right > gViewportBoundsTable[gViewportX][2] && right < gViewportBoundsTable[gViewportX][3]) {
        gViewportScaleX = ((s16 (*)[4])gViewportOffsetX)[gViewportX][0] + x;
    }
    if (top > gViewportBoundsTable[gViewportX][4] && top < gViewportBoundsTable[gViewportX][5] && bottom > gViewportBoundsTable[gViewportX][6] && bottom < gViewportBoundsTable[gViewportX][7]) {
        gViewportScaleY = ((s16 (*)[4])gViewportOffsetY)[gViewportX][0] + y;
    }
    display_update();
}
