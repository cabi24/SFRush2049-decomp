/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
void idle_thread_entry(void *argument) {
    osCreatePiManager(150, &gViModeTable, gViModeLan1, 200);
    osCreateViManager(NULL, 0);
    osCreateThread(&gGameThread, 6, game_init, argument, gStackGame + 0x960, 4);
    osStartThread(&gGameThread);
    for (;;) {}
}
