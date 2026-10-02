#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
extern OSThread gIdleThread,gGameThread;
extern u8 gStackIdle[];
extern void __osInitialize_common(void),idle_thread_entry(void *),game_init(void *);
extern OSMesgQueue gViModeTable;
extern OSMesg gViModeLan1[];
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
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

void idle_thread_entry(void *argument) {
    osCreatePiManager(150, &gViModeTable, gViModeLan1, 200);
    osCreateViManager(NULL, 0);
    osCreateThread(&gGameThread, 6, game_init, argument, gStackGame + 0x960, 4);
    osStartThread(&gGameThread);
    for (;;) {}
}

#pragma GLOBAL_ASM("build/C76/lib_2cf0/game_init.s")
void audio_thread_entry(s32 arg) { __setfpcsr(0x01000E00); while(1) { game_loop(); } }

