/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern OSMesgQueue gDmaMesgQueue, gInflateMsgQueue, gRetraceMesgQueue, gEventMesgQueue, gSyncMesgQueue;
extern OSMesg gDmaMesgBuf[], gInflateMesgBuf[], gRetraceMesgBuf[], gEventMesgBuf[], gSyncMesgBuf[];
extern OSSched gScheduler;
extern u8 gSchedulerStack[], gDecompressedDataStart[], gDecompressedDataEnd1[], gBssStart[], gBssEnd[], gRomCompressedData[];
extern u8 gStackAudio[], gRenderThreadStack[], gGameThreadStack[];
extern OSThread gAudioThread, gSchedulerThread, D_800344E0;
extern s16 gSyncMagic;
extern s8 gInitFlag, gGameStateFlag;
extern s32 get_tv_offset(void);
extern void osCreateScheduler(OSSched *, void *, OSPri, s32, s32);
extern void osScAddClient(OSSched *, OSScClient *, OSMesgQueue *);
extern void osInvalICache_full(void *, s32), osInvalDCache(void *, s32), bzero(void *, s32);
extern s32 inflate_entry(void *, void *, s32);
extern void brake_force_apply(void *), render_thread_entry(void *), audio_thread_entry(s32);
extern void game_late_init(void), audio_start(void), world_trigger_activate(void);
void game_init(void *argument) {
    s16 *message = NULL;
    OSScClient client;
    register s32 tv;
    __setfpcsr(0x01000E00);
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, 8);
    osCreateMesgQueue(&gInflateMsgQueue, gInflateMesgBuf, 8);
    osCreateMesgQueue(&gRetraceMesgQueue, gRetraceMesgBuf, 60);
    osCreateMesgQueue(&gEventMesgQueue, gEventMesgBuf, 8);
    tv = get_tv_offset();
    osCreateScheduler(&gScheduler, gSchedulerStack + 0x640, 12, tv, 1);
    osInvalICache_full(gDecompressedDataStart, gDecompressedDataEnd1 - gDecompressedDataStart);
    osInvalDCache(gDecompressedDataEnd1, gBssStart - gDecompressedDataEnd1);
    inflate_entry(gRomCompressedData, gDecompressedDataStart, 0);
    bzero(gBssStart, gBssEnd - gBssStart);
    osScAddClient(&gScheduler, &client, &gRetraceMesgQueue);
    osCreateThread(&gAudioThread, 8, brake_force_apply, NULL, gStackAudio + 0x960, 3);
    osCreateMesgQueue(&gSyncMesgQueue, gSyncMesgBuf, 1);
    osJamMesg(&gSyncMesgQueue, NULL, OS_MESG_BLOCK);
    osStartThread(&gAudioThread);
    osCreateThread(&gSchedulerThread, 5, render_thread_entry, NULL, gRenderThreadStack + 0x960, 7);
    osStartThread(&gSchedulerThread);
    game_late_init();
    sound_init();
    osCreateThread(&D_800344E0, 7, (void (*)(void *))audio_thread_entry, argument, gGameThreadStack + 0x12C0, 5);
    gSyncMagic = 0xABF;
    if (gInitFlag == 0) osStartThread(&D_800344E0);
    for (;;) {
        osRecvMesg(&gRetraceMesgQueue, (OSMesg *)&message, OS_MESG_BLOCK);
        if (*message == 0xABE) break;
    }
    gGameStateFlag = 0;
    audio_start();
    for (;;) world_trigger_activate();
}
