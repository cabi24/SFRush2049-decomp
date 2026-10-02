/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
void audio_thread_entry(s32 arg) { __setfpcsr(0x01000E00); while(1) { game_loop(); } }
