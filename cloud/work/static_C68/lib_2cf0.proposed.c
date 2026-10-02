#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
#pragma GLOBAL_ASM("build/C68/lib_2cf0/main.s")
#pragma GLOBAL_ASM("build/C68/lib_2cf0/idle_thread_entry.s")
#pragma GLOBAL_ASM("build/C68/lib_2cf0/game_init.s")
void audio_thread_entry(s32 arg) { __setfpcsr(0x01000E00); while(1) { game_loop(); } }

