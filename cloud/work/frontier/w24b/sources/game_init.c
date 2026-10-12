/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* game_init (static, boot 0x80002238, 708 B, segment 0x2cf0 -> src/rom/lib_2cf0.c, IDO 5.3 -g1 -O1): the game
 * thread's entry (created by idle_thread_entry). Message queues, scheduler (TV type from get_tv_offset), cache
 * invalidation + inflate of the compressed game image into RAM, bss clear, retrace client, the audio/render/
 * game-loop threads, sync magic 0xABF; waits for retrace message type 0xABE, then audio_start and the
 * world_trigger_activate loop. Rush code (no library ancestor). Names: symbol_addrs.us.txt.
 * w24b devices, all disclosed:
 *  - SIX UNUSED LOCALS unused0..unused5 declared before `client`: the exact frame residual. Retail frame 0x50;
 *    without them 0x38; 6 slots x 4 B = 0x18 (-O1 gives every local a slot; retail never touches 0x38..0x4F).
 *    Their real identity is unknown (any 24 B of locals declared first gives the same code).
 *  - `register s32 tvType` (retail keeps it in s0, saved at 0x20; -O1 keeps only `register` locals in regs).
 *  - the TV-type assignment and osCreateScheduler are ONE comma-expression statement: at -g1 as1 does not move
 *    code across statements; retail schedules `move s0,v0` after the stack-address `lui/addiu` and puts
 *    `li a2,12` in the jal delay slot, which only a single statement reproduces (two statements: 3 insns
 *    reordered). Probably a macro in the original.
 *  - `if (type == 0xABE) break;` in `while (1)` (retail `bne` + two `b`).
 *  - `(void (*)(void *))audio_thread_entry`: the TU defines it later as `void audio_thread_entry(s32)`; the cast
 *    (no code) keeps that definition's prototype. OSSched_2cf0 / OSScMsg_2cf0 are private incomplete/views
 *    (only addresses and the message's s16 type are used).
 * Retail calls osSendMesg (0x800075E0), osInvalICache (0x80008460), osInvalDCache (0x800084E0): w22g names; the
 * DB target object still carries pre-w22g names for some of them (see RESULTS.md). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef s32 OSPri;
typedef s32 OSId;
typedef struct OSThread_s OSThread;
typedef struct OSMesgQueue_s { OSThread *mtqueue; OSThread *fullqueue; s32 validCount; s32 first; s32 msgCount; OSMesg *msg; } OSMesgQueue;
typedef struct OSScClient_s { struct OSScClient_s *next; OSMesgQueue *msgQueue; } OSScClient;
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
