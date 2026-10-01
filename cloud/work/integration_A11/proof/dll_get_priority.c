/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
s32 dll_get_priority(void *thread)
{
    if (thread == ((void *) 0))
    {
        thread = __osRunningThread;
    }
    return *(s32 *) ((u8 *) thread + 4);
}
