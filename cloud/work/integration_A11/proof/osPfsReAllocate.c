/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
OSId osPfsReAllocate(OSThread* thread) {
    if (thread == NULL) {
        thread = __osRunningThread;
    }

    return thread->id;
}
