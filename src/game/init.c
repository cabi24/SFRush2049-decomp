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

/* External game functions */
extern void func_80007E80(void);     /* OS initialization */
extern void func_800081D0(u32 addr, u32 *out);  /* Read PIF/EEPROM */
extern void func_80008210(s32 a0, void *a1, void *a2, s32 a3);  /* Video setup */
extern void func_80008380(s32 a0, s32 a1);  /* Video mode */
extern void *func_80001DFC(void);    /* Returns frame counter ptr */
extern void func_80000450(void *a0, void *a1, s32 a2, void *a3, s32 a4);  /* Timer/interrupt setup */
extern void func_800005D4(void *a0, void *a1, void *a2);  /* More setup */
extern void func_800075E0(OSMesgQueue *mq, OSMesg msg, s32 flags);  /* Send message */
extern void func_800EEA7C(void);     /* Late init */
extern void func_800A4934(void);     /* Sound init? */
extern void func_800A48C8(void);     /* Audio start? */
extern void func_800EE5DC(void);     /* Main game loop iteration */
extern void func_800FD464(void);     /* Main thread loop */

/* Forward declarations for functions in this file */
static void game_init(void *arg);
static void func_800024FC(void *arg);

/* Thread structures */
extern u8 D_80034BA0[];  /* Thread 1 struct (idle thread) */
extern u8 D_80034D50[];  /* Thread 6 struct (game_init thread) */
extern u8 D_80034690[];  /* Thread 8 struct */
extern u8 D_80034840[];  /* Thread 5 struct */
extern u8 D_800344E0[];  /* Thread 7 struct (main game thread) */

/* Thread stacks */
extern u8 D_8002F660[];  /* Stack for thread 1 */
extern u8 D_80030150[];  /* Stack for thread 6 */
extern u8 D_8002F7F0[];  /* Stack for thread 8 */
extern u8 D_80031D70[];  /* Stack for thread 5 */
extern u8 D_80030AB0[];  /* Stack for thread 7 */

/* Message queues */
extern OSMesgQueue D_80035410;   /* DMA message queue */
extern OSMesgQueue D_80035428;   /* Inflate I/O queue */
extern OSMesgQueue D_8002ECF8;   /* Retrace queue */
extern OSMesgQueue D_8002ECC0;   /* Unknown queue */
extern OSMesgQueue D_80035440;   /* Event queue */

/* Message queue message storage */
extern OSMesg D_8002F140[];  /* 8 messages for D_80035410 */
extern OSMesg D_8002F160[];  /* 8 messages for D_80035428 */
extern OSMesg D_8002ED10[];  /* 60 messages for D_8002ECF8 */
extern OSMesg D_8002ECD8[];  /* 8 messages for D_8002ECC0 */
extern OSMesg D_8002F180[];  /* 1 message for D_80035440 */

/* Memory regions */
extern u8 D_80086A50[];    /* Start of decompressed data */
extern u8 D_8010FD80[];    /* End of decompressed region 1 */
extern u8 D_801249F0[];    /* Start of BSS region */
extern u8 D_8017A640[];    /* End of BSS region */

/* ROM data */
#define ROM_COMPRESSED_DATA 0xB0CB10

/* State variables */
extern u8 D_8002B028;      /* Init flag */
extern u16 D_80142D90;     /* Magic value for sync */
extern u8 D_80035472;      /* Game state flag */

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
    func_80007E80();

    /* Read 16 words from PIF RAM */
    addr = 0xFFB000;
    for (i = 0; i < 16; i++) {
        func_800081D0(addr, &pif_data[i]);
        addr += 4;
    }

    /* Create and start idle thread (thread 1) */
    osCreateThread(D_80034BA0, 1, thread1_entry, arg,
                   D_8002F660 + 0x190, 2);
    osStartThread(D_80034BA0);

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
    func_80008210(0x96, (void *)0x8002EE08, (void *)0x8002EE20, 0xC8);
    func_80008380(0, 0);

    /* Create game_init thread (thread 6) */
    osCreateThread(D_80034D50, 6, game_init, arg,
                   D_80030150 + 0x960, 4);
    osStartThread(D_80034D50);

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
    osCreateMesgQueue(&D_80035410, D_8002F140, 8);     /* DMA queue */
    osCreateMesgQueue(&D_80035428, D_8002F160, 8);     /* Inflate I/O queue */
    osCreateMesgQueue(&D_8002ECF8, D_8002ED10, 60);    /* Retrace queue */
    osCreateMesgQueue(&D_8002ECC0, D_8002ECD8, 8);     /* Unknown queue */

    /* Get frame counter and set up timer interrupts */
    frame_counter = (void *)func_80001DFC();
    func_80000450((void *)0x8002E8E8, (void *)0x800344E0, 12, frame_counter, 1);

    /* Finalize DMA for first region */
    dma_finalize(D_80086A50, (u32)(D_8010FD80 - D_80086A50));

    /* Invalidate D-cache for region to be decompressed into */
    osInvalDCache(D_8010FD80, (u32)(D_801249F0 - D_8010FD80));

    /* Decompress main game data from ROM */
    inflate_entry((void *)ROM_COMPRESSED_DATA, D_80086A50, 0);

    /* Clear BSS section */
    bzero(D_801249F0, (u32)(D_8017A640 - D_801249F0));

    /* More initialization */
    func_800005D4((void *)0x8002E8E8, &msg, &D_8002ECF8);

    /* Create audio thread (thread 8) */
    osCreateThread(D_80034690, 8, (void *)0x800AC75C, NULL,
                   D_8002F7F0 + 0x960, 3);

    /* Set up event message queue */
    osCreateMesgQueue(&D_80035440, D_8002F180, 1);
    func_800075E0(&D_80035440, NULL, 1);

    /* Start audio thread */
    osStartThread(D_80034690);

    /* Create render thread (thread 5) */
    osCreateThread(D_80034840, 5, (void *)0x800E7808, NULL,
                   D_80031D70 + 0x960, 7);
    osStartThread(D_80034840);

    /* Late initialization */
    func_800EEA7C();
    func_800A4934();

    /* Create main game thread (thread 7) */
    osCreateThread(D_800344E0, 7, (void *)func_800024FC, arg,
                   D_80030AB0 + 0x12C0, 5);

    /* Write magic sync value */
    D_80142D90 = 0x0ABF;

    /* Start main game thread if not in some special mode */
    if (D_8002B028 == 0) {
        osStartThread(D_800344E0);
    }

    /* Wait for sync signal (0x0ABE) */
    do {
        osRecvMesg(&D_8002ECF8, &msg, 1);
    } while (*(s16 *)msg != 0x0ABE);

    /* Clear game state flag */
    D_80035472 = 0;

    /* Start audio */
    func_800A48C8();

    /* Main loop - runs game logic */
    for (;;) {
        func_800EE5DC();
    }
}

/**
 * Main game thread entry (thread 7)
 * func_800024FC - Runs main game rendering loop
 *
 * @param arg Argument passed through
 */
static void func_800024FC(void *arg) {
    /* Set FPU control/status register */
    __setfpcsr(0x1000E00);

    /* Main rendering loop */
    for (;;) {
        func_800FD464();
    }
}
