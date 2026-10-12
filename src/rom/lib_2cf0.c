/* GENERATED ROM-aligned TU — segment 0x2cf0 (rom/lib_2cf0)
 * layout map 2fee9198d70b169d9db10078865019d23807b48148a346fda9e88851da0e576b; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "static_debug_context.h"

/* PROMOTED 2026-10-02 — main
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/main.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/main.c:main (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void main(void *argument) {
    u32 index;
    u32 address;
    u32 header[16];
    __osInitialize_common();
    address = 0xFFB000;
    for (index = 0; index < 16; index++, address += 4) {
        osPiReadIo(address, &header[index]);
    }
    osCreateThread(&gIdleThread, 1, idle_thread_entry, argument, gStackIdle + 0x190, 2);
    osStartThread(&gIdleThread);
}

/* PROMOTED 2026-10-02 — idle_thread_entry
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/idle_thread_entry.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/idle_thread_entry.c:idle_thread_entry (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void idle_thread_entry(void *argument) {
    osCreatePiManager(150, &gViModeTable, gViModeLan1, 200);
    osSetThreadPri(NULL, 0);
    osCreateThread(&gGameThread, 6, game_init, argument, gStackGame + 0x960, 4);
    osStartThread(&gGameThread);
    for (;;) {}
}

/* PROMOTED 2026-10-12 — game_init
 * Source:   cloud/work/frontier/w24b/sources/game_init.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24b/sources/game_init.c:game_init (score0); rom_tu.h context: cloud/work/frontier/w24b/ctx53_gi.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct { s16 type; } OSScMsg_2cf0;
typedef struct OSSched_2cf0 OSSched_2cf0;
extern OSMesgQueue gDmaMesgQueue, gInflateMsgQueue, gRetraceMesgQueue, gEventMesgQueue, gSyncMesgQueue;
extern OSMesg gDmaMesgBuf[], gInflateMesgBuf[], gRetraceMesgBuf[], gEventMesgBuf[], gSyncMesgBuf[];
extern OSSched_2cf0 gScheduler;
extern u8 gSchedulerStack[], gStackAudio[], gRenderThreadStack[], gGameThreadStack[];
extern u8 gDecompressedDataStart[], gDecompressedDataEnd1[], gBssStart[], gBssEnd[], gRomCompressedData[];
extern OSThread gAudioThread, gSchedulerThread, D_800344E0;
extern s16 gSyncMagic;
extern s8 gInitFlag;
extern s8 gGameStateFlag;
extern void __setfpcsr(u32 value);
extern void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
extern void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
extern void osStartThread(OSThread *thread);
extern void osCreateScheduler(OSSched_2cf0 *, void *, OSPri, u8, u8);
extern void osScAddClient(OSSched_2cf0 *, OSScClient *, OSMesgQueue *);
extern void osInvalICache(void *vaddr, s32 nbytes);
extern void osInvalDCache(void *vaddr, s32 nbytes);
extern void bzero(void *s, s32 n);
extern s32 inflate_entry(void *, void *, s32);
extern s32 get_tv_offset(void);
extern void brake_force_apply(void *);
extern void render_thread_entry(void *);
extern void audio_thread_entry(s32);
extern void game_late_init(void);
extern void sound_init(void);
extern void audio_start(void);
extern void world_trigger_activate(void);
void game_init(void *arg) {
    s32 unused0, unused1, unused2, unused3, unused4, unused5; /* frame residual, see header */
    OSScClient client;
    OSMesg msg = 0;
    register s32 tvType;

    __setfpcsr(0x01000E00);
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, 8);
    osCreateMesgQueue(&gInflateMsgQueue, gInflateMesgBuf, 8);
    osCreateMesgQueue(&gRetraceMesgQueue, gRetraceMesgBuf, 60);
    osCreateMesgQueue(&gEventMesgQueue, gEventMesgBuf, 8);
    tvType = get_tv_offset(), osCreateScheduler(&gScheduler, gSchedulerStack + 0x640, 12, tvType, 1);
    osInvalICache(gDecompressedDataStart, gDecompressedDataEnd1 - gDecompressedDataStart);
    osInvalDCache(gDecompressedDataEnd1, gBssStart - gDecompressedDataEnd1);
    inflate_entry(gRomCompressedData, gDecompressedDataStart, 0);
    bzero(gBssStart, gBssEnd - gBssStart);
    osScAddClient(&gScheduler, &client, &gRetraceMesgQueue);
    osCreateThread(&gAudioThread, 8, brake_force_apply, 0, gStackAudio + 0x960, 3);
    osCreateMesgQueue(&gSyncMesgQueue, gSyncMesgBuf, 1);
    osSendMesg(&gSyncMesgQueue, 0, 1);
    osStartThread(&gAudioThread);
    osCreateThread(&gSchedulerThread, 5, render_thread_entry, 0, gRenderThreadStack + 0x960, 7);
    osStartThread(&gSchedulerThread);
    game_late_init();
    sound_init();
    osCreateThread(&D_800344E0, 7, (void (*)(void *))audio_thread_entry, arg, gGameThreadStack + 0x12C0, 5);
    gSyncMagic = 0xABF;
    if (!gInitFlag) {
        osStartThread(&D_800344E0);
    }
    while (1) {
        osRecvMesg(&gRetraceMesgQueue, &msg, 1);
        if (((OSScMsg_2cf0 *)msg)->type == 0xABE) {
            break;
        }
    }
    gGameStateFlag = 0;
    audio_start();
    while (1) {
        world_trigger_activate();
    }
}

/* PROMOTED 2026-10-02 — audio_thread_entry
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/audio_thread_entry.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/audio_thread_entry.c:audio_thread_entry (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void audio_thread_entry(s32 arg) { __setfpcsr(0x01000E00); while(1) { game_loop(); } }

