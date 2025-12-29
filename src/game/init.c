/**
 * @file init.c
 * @brief Game initialization and thread setup
 *
 * Decompiled from asm/us/2CF0.s
 * Contains main entry point and game initialization routines
 */

#include "types.h"
#include "PR/os_thread.h"
#include "PR/os_message.h"

/* OS function declarations (match os_thread.h and os_message.h) */
extern void osSetEventMesg(s32 event, OSMesgQueue *mq, OSMesg msg);
extern void __setfpcsr(u32 csr);

/* External decompression functions */
extern s32 inflate_entry(void *src, void *dst, s32 use_heap);
extern void bzero(void *ptr, u32 size);
extern void osInvalDCache(void *addr, u32 size);
extern void dma_finalize(void *dst, u32 size);

/* libultra OS functions */
extern void osInitialize(void);                                    /* func_80007E80 */
extern s32 osPiRawReadWord(u32 addr, u32 *out);                   /* func_800081D0 */
extern void osCreateScheduler(s32 a0, void *a1, void *a2, s32 a3); /* func_80008210 */
extern void osScSetVideoMode(s32 a0, s32 a1);                      /* func_80008380 */
extern void *osScGetFrameCount(void);                              /* func_80001DFC */
extern void osScCreateThread(void *a0, void *a1, s32 a2, void *a3, s32 a4);  /* func_80000450 */
extern void osScStartRetrace(void *a0, void *a1, void *a2);        /* func_800005D4 */
extern void osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);     /* func_800075E0 */

/* Game-specific initialization functions */
extern void game_late_init(void);    /* func_800EEA7C */
extern void sound_init(void);        /* func_800A4934 */
extern void audio_start(void);       /* func_800A48C8 */
extern void game_frame_update(void); /* func_800EE5DC - per-frame game logic */
extern void game_loop(void);         /* func_800FD464 - main game loop */

/* Forward declarations for functions in this file */
static void game_init(void *arg);
static void game_thread_entry(void *arg);

/* Thread structures */
extern u8 gIdleThread[];    /* Thread 1 struct (idle thread) */
extern u8 gInitThread[];    /* Thread 6 struct (game_init thread) */
extern u8 gAudioThread[];   /* Thread 8 struct (audio thread) */
extern u8 gRenderThread[];  /* Thread 5 struct (render thread) */
extern u8 gGameThread[];    /* Thread 7 struct (main game thread) */

/* Thread stacks */
extern u8 gIdleThreadStack[];    /* Stack for thread 1 (idle) */
extern u8 gInitThreadStack[];    /* Stack for thread 6 (init) */
extern u8 gAudioThreadStack[];   /* Stack for thread 8 (audio) */
extern u8 gRenderThreadStack[];  /* Stack for thread 5 (render) */
extern u8 gGameThreadStack[];    /* Stack for thread 7 (game) */

/* Message queues */
extern OSMesgQueue gDmaMesgQueue;       /* DMA message queue */
extern OSMesgQueue gInflateMesgQueue;   /* Inflate I/O queue */
extern OSMesgQueue gRetraceMesgQueue;   /* Retrace/VBlank queue */
extern OSMesgQueue gEventMesgQueue;     /* Event message queue */
extern OSMesgQueue gSyncMesgQueue;      /* Sync signal queue */

/* Message queue message storage */
extern OSMesg gDmaMesgBuf[];       /* 8 messages for gDmaMesgQueue */
extern OSMesg gInflateMesgBuf[];   /* 8 messages for gInflateMesgQueue */
extern OSMesg gRetraceMesgBuf[];   /* 60 messages for gRetraceMesgQueue */
extern OSMesg gEventMesgBuf[];     /* 8 messages for gEventMesgQueue */
extern OSMesg gSyncMesgBuf[];      /* 1 message for gSyncMesgQueue */

/* Memory regions */
extern u8 gDecompressedDataStart[];  /* Start of decompressed game code */
extern u8 gDecompressedDataEnd1[];   /* End of decompressed region 1 */
extern u8 gBssStart[];               /* Start of BSS region */
extern u8 gBssEnd[];                 /* End of BSS region */

/* ROM data */
#define ROM_COMPRESSED_DATA 0xB0CB10

/* State variables */
extern u8 gInitFlag;         /* Init mode flag (0 = normal boot) */
extern u16 gSyncMagic;       /* Magic value for sync (0x0ABF/0x0ABE) */
extern u8 gGameStateFlag;    /* Game state flag */

/* Thread entry points - forward declarations */
static void thread1_entry(void *arg);
static void thread6_entry(void *arg);

/**
 * Main entry point (called from entrypoint after BSS clear)
 * func_800020F0 - Sets up first thread and starts OS
 *
 * @param arg Argument passed from entrypoint
 */
void main(void *arg) {
    u32 pif_data[16];
    u32 addr;
    s32 i;

    /* Initialize OS */
    osInitialize();

    /* Read 16 words from PIF RAM */
    addr = 0xFFB000;
    for (i = 0; i < 16; i++) {
        osPiRawReadWord(addr, &pif_data[i]);
        addr += 4;
    }

    /* Create and start idle thread (thread 1) */
    osCreateThread(gIdleThread, 1, thread1_entry, arg,
                   gIdleThreadStack + 0x190, 2);
    osStartThread(gIdleThread);

    /* Never returns - idle loop would be here */
}

/**
 * Idle thread entry (thread 1)
 * func_800021A4 - Sets up video and creates game_init thread
 *
 * @param arg Argument from main
 */
static void thread1_entry(void *arg) {
    /* Initialize video system */
    osCreateScheduler(0x96, (void *)0x8002EE08, (void *)0x8002EE20, 0xC8);
    osScSetVideoMode(0, 0);

    /* Create game_init thread (thread 6) */
    osCreateThread(gInitThread, 6, game_init, arg,
                   gInitThreadStack + 0x960, 4);
    osStartThread(gInitThread);

    /* Infinite loop - thread 1 becomes idle */
    for (;;) {
        /* Spin */
    }
}

/**
 * Main game initialization (thread 6)
 * func_80002238 - Initializes all game subsystems
 *
 * @param arg Argument passed through from main
 */
static void game_init(void *arg) {
    void *frame_counter;
    OSMesg msg;

    /* Set FPU control/status register (enable all exceptions) */
    __setfpcsr(0x1000E00);

    /* Initialize message queues */
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, 8);         /* DMA queue */
    osCreateMesgQueue(&gInflateMesgQueue, gInflateMesgBuf, 8); /* Inflate I/O queue */
    osCreateMesgQueue(&gRetraceMesgQueue, gRetraceMesgBuf, 60);/* Retrace queue */
    osCreateMesgQueue(&gEventMesgQueue, gEventMesgBuf, 8);     /* Event queue */

    /* Get frame counter and set up timer interrupts */
    frame_counter = (void *)osScGetFrameCount();
    osScCreateThread((void *)0x8002E8E8, (void *)0x800344E0, 12, frame_counter, 1);

    /* Finalize DMA for first region */
    dma_finalize(gDecompressedDataStart, (u32)(gDecompressedDataEnd1 - gDecompressedDataStart));

    /* Invalidate D-cache for region to be decompressed into */
    osInvalDCache(gDecompressedDataEnd1, (u32)(gBssStart - gDecompressedDataEnd1));

    /* Decompress main game data from ROM */
    inflate_entry((void *)ROM_COMPRESSED_DATA, gDecompressedDataStart, 0);

    /* Clear BSS section */
    bzero(gBssStart, (u32)(gBssEnd - gBssStart));

    /* More initialization */
    osScStartRetrace((void *)0x8002E8E8, &msg, &gRetraceMesgQueue);

    /* Create audio thread (thread 8) */
    osCreateThread(gAudioThread, 8, (void *)0x800AC75C, NULL,
                   gAudioThreadStack + 0x960, 3);

    /* Set up event message queue */
    osCreateMesgQueue(&gSyncMesgQueue, gSyncMesgBuf, 1);
    osJamMesg(&gSyncMesgQueue, NULL, 1);

    /* Start audio thread */
    osStartThread(gAudioThread);

    /* Create render thread (thread 5) */
    osCreateThread(gRenderThread, 5, (void *)0x800E7808, NULL,
                   gRenderThreadStack + 0x960, 7);
    osStartThread(gRenderThread);

    /* Late initialization */
    game_late_init();
    sound_init();

    /* Create main game thread (thread 7) */
    osCreateThread(gGameThread, 7, (void *)game_thread_entry, arg,
                   gGameThreadStack + 0x12C0, 5);

    /* Write magic sync value */
    gSyncMagic = 0x0ABF;

    /* Start main game thread if not in some special mode */
    if (gInitFlag == 0) {
        osStartThread(gGameThread);
    }

    /* Wait for sync signal (0x0ABE) */
    do {
        osRecvMesg(&gRetraceMesgQueue, &msg, 1);
    } while (*(s16 *)msg != 0x0ABE);

    /* Clear game state flag */
    gGameStateFlag = 0;

    /* Start audio */
    audio_start();

    /* Main loop - runs game logic */
    for (;;) {
        game_frame_update();
    }
}

/**
 * Main game thread entry (thread 7)
 * func_800024FC - Runs main game rendering loop
 *
 * @param arg Argument passed through
 */
static void game_thread_entry(void *arg) {
    /* Set FPU control/status register */
    __setfpcsr(0x1000E00);

    /* Main rendering loop */
    for (;;) {
        game_loop();
    }
}
