/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
void main(void *argument) {
    u32 index;
    u32 address;
    u32 header[16];
    __osInitialize_common();
    address = 0xFFB000;
    for (index = 0; index < 16; index++, address += 4) {
        osPiRawReadWord(address, &header[index]);
    }
    osCreateThread(&gIdleThread, 1, idle_thread_entry, argument, gStackIdle + 0x190, 2);
    osStartThread(&gIdleThread);
}
