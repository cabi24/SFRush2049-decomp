/**
 * @file init.c
 * @brief Game initialization and thread setup
 *
 * Decompiled from asm/us/2CF0.s
 * Contains main entry point and game initialization routines.
 *
 * Based on reference/repos/rushtherock/game/init.c
 * The arcade version has a simpler structure (single-threaded),
 * while the N64 version uses multiple threads for audio, rendering,
 * and game logic.
 *
 * Arcade flow: start() -> init() -> game_init() -> game_loop()
 * N64 flow: main() -> thread1_entry() -> game_init_thread() -> game_thread_entry()
 */

#include "types.h"
#include "PR/os_thread.h"
#include "PR/os_message.h"
#include "game/game.h"

/*===========================================================================*/
/*                          OS FUNCTION DECLARATIONS                         */
/*===========================================================================*/

extern void osSetEventMesg(s32 event, OSMesgQueue *mq, OSMesg msg);
extern void __setfpcsr(u32 csr);

/* External decompression functions */
extern s32 inflate_entry(void *src, void *dst, s32 use_heap);
extern void bzero(void *ptr, u32 size);
extern void osInvalDCache(void *addr, u32 size);
extern void dma_finalize(void *dst, u32 size);

/* libultra OS functions */
extern void osInitialize(void);
extern s32 osPiRawReadWord(u32 addr, u32 *out);
extern void osCreateScheduler(s32 a0, void *a1, void *a2, s32 a3);
extern void osScSetVideoMode(s32 a0, s32 a1);
extern void *osScGetFrameCount(void);
extern void osScCreateThread(void *a0, void *a1, s32 a2, void *a3, s32 a4);
extern void osScStartRetrace(void *a0, void *a1, void *a2);
extern void osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);

/*===========================================================================*/
/*                       ARCADE-COMPATIBLE CONSTANTS                         */
/*===========================================================================*/

/* Memory allocation sizes (from arcade init.c) */
#define NET_MEM_BYTES       (30 * 1024)
#define GUTS_STACK_BYTES    (32 * 1024)

/* ROM addresses for compressed data (N64 specific) */
#define ROM_COMPRESSED_DATA 0xB0CB10

/* FPU control/status register value */
#define FPCSR_INIT          0x1000E00

/* Model task timing (microseconds) */
#define MODEL_INITIAL_DELAY 50000   /* 50ms initial delay */
#define MODEL_PERIOD_USEC   5000    /* 5ms model update period */
#define MODEL_MIN_RESCHEDULE 500    /* 0.5ms minimum reschedule */

/*===========================================================================*/
/*                       ARCADE-COMPATIBLE GLOBALS                           */
/*===========================================================================*/

/* Time tracking variables (from arcade init.c) */
f32 gCurGameTime;           /* Current game time in seconds */
f32 gLastGameTime;          /* Last frame's game time */
f32 gGameLoopTime;          /* Time for one game loop iteration */
f32 gGameExecTime;          /* Execution time for game logic */

/* Frame tracking */
volatile u32 frames;        /* Frame counter */
volatile u32 gotvb;         /* VBlank received flag */

/* Memory pool */
u32 mem_pool_start;         /* Start of memory pool */
u32 net_mem_start;          /* Start of network memory */

/* Model task variables */
s32 game_block;             /* Model blocked by game task count */
u32 mast_period;            /* Model AST period */
u32 last_mast_time;         /* Last model AST time */

/* Track loaded index */
s16 last_track_loaded;      /* Last track loaded, -1 = none */

/*===========================================================================*/
/*                         GAME-SPECIFIC EXTERNALS                           */
/*===========================================================================*/

/* Game-specific initialization functions (in compressed game code) */
extern void game_late_init(void);       /* 0x800EEA7C */
extern void sound_init(void);           /* 0x800A4934 */
extern void audio_start(void);          /* 0x800A48C8 */
extern void game_frame_update(void);    /* 0x800EE5DC - per-frame game logic */
extern void game_loop(void);            /* 0x800FD464 - main game loop */

/*===========================================================================*/
/*                            FORWARD DECLARATIONS                           */
/*===========================================================================*/

static void game_init_thread(void *arg);
static void game_thread_entry(void *arg);
static void thread1_entry(void *arg);

/*===========================================================================*/
/*                            THREAD STRUCTURES                              */
/*===========================================================================*/

/* Thread structures (in BSS) */
extern u8 gIdleThread[];        /* Thread 1 struct (idle thread) */
extern u8 gInitThread[];        /* Thread 6 struct (game_init thread) */
extern u8 gAudioThread[];       /* Thread 8 struct (audio thread) */
extern u8 gRenderThread[];      /* Thread 5 struct (render thread) */
extern u8 gGameThread[];        /* Thread 7 struct (main game thread) */
extern u8 gScheduler[];         /* OSSched scheduler structure */

/* Thread entry points */
extern void audio_thread_proc(void *);
extern void render_thread_entry(void *);

/* Thread stacks */
extern u8 gIdleThreadStack[];       /* Stack for thread 1 (idle) */
extern u8 gInitThreadStack[];       /* Stack for thread 6 (init) */
extern u8 gAudioThreadStack[];      /* Stack for thread 8 (audio) */
extern u8 gRenderThreadStack[];     /* Stack for thread 5 (render) */
extern u8 gGameThreadStack[];       /* Stack for thread 7 (game) */

/*===========================================================================*/
/*                            MESSAGE QUEUES                                 */
/*===========================================================================*/

extern OSMesgQueue gDmaMesgQueue;       /* DMA message queue */
extern OSMesgQueue gInflateMesgQueue;   /* Inflate I/O queue */
extern OSMesgQueue gRetraceMesgQueue;   /* Retrace/VBlank queue */
extern OSMesgQueue gEventMesgQueue;     /* Event message queue */
extern OSMesgQueue gSyncMesgQueue;      /* Sync signal queue */

/* Message queue message storage */
extern OSMesg gDmaMesgBuf[];        /* 8 messages for gDmaMesgQueue */
extern OSMesg gInflateMesgBuf[];    /* 8 messages for gInflateMesgQueue */
extern OSMesg gRetraceMesgBuf[];    /* 60 messages for gRetraceMesgQueue */
extern OSMesg gEventMesgBuf[];      /* 8 messages for gEventMesgQueue */
extern OSMesg gSyncMesgBuf[];       /* 1 message for gSyncMesgQueue */

/*===========================================================================*/
/*                            MEMORY REGIONS                                 */
/*===========================================================================*/

extern u8 gDecompressedDataStart[];     /* Start of decompressed game code */
extern u8 gDecompressedDataEnd1[];      /* End of decompressed region 1 */
extern u8 gBssStart[];                  /* Start of BSS region */
extern u8 gBssEnd[];                    /* End of BSS region */

/*===========================================================================*/
/*                            STATE VARIABLES                                */
/*===========================================================================*/

extern u8 gInitFlag;            /* Init mode flag (0 = normal boot) */
extern u16 gSyncMagic;          /* Magic value for sync (0x0ABF/0x0ABE) */
extern u8 gGameStateFlag;       /* Game state flag */

/* Video mode tables */
extern u8 gViModeTable[];       /* Video mode table */
extern u8 gViModeLan1[];        /* NTSC LAN1 mode */

/*===========================================================================*/
/*                      N64 ENTRY POINT AND THREADS                          */
/*===========================================================================*/

/**
 * Main entry point (called from entrypoint after BSS clear)
 * 0x800020F0 - Sets up first thread and starts OS
 *
 * This is the N64-specific entry point. The arcade version uses
 * start() which has a simpler single-threaded design.
 *
 * @param arg Argument passed from entrypoint
 */
void main(void *arg) {
    u32 pif_data[16];
    u32 addr;
    s32 i;

    /* Initialize OS */
    osInitialize();

    /* Read 16 words from PIF RAM (controller/EEPROM detection) */
    addr = 0xFFB000;
    for (i = 0; i < 16; i++) {
        osPiRawReadWord(addr, &pif_data[i]);
        addr += 4;
    }

    /* Create and start idle thread (thread 1) */
    osCreateThread(gIdleThread, 1, thread1_entry, arg,
                   gIdleThreadStack + 0x190, 2);
    osStartThread(gIdleThread);

    /* Never returns - boot thread becomes lowest priority */
}

/**
 * Idle thread entry (thread 1)
 * 0x800021A4 - Sets up video and creates game_init thread
 *
 * @param arg Argument from main
 */
static void thread1_entry(void *arg) {
    /* Initialize video system via scheduler */
    osCreateScheduler(0x96, gViModeTable, gViModeLan1, 0xC8);
    osScSetVideoMode(0, 0);

    /* Create game_init thread (thread 6) */
    osCreateThread(gInitThread, 6, game_init_thread, arg,
                   gInitThreadStack + 0x960, 4);
    osStartThread(gInitThread);

    /* Infinite loop - thread 1 becomes idle */
    for (;;) {
        /* Spin - lowest priority thread */
    }
}

/**
 * Main game initialization thread (thread 6)
 * 0x80002238 - Initializes all game subsystems
 *
 * This is the N64 equivalent of the arcade's init() + game_init() sequence.
 * The arcade version:
 *   1. init() - hardware/memory setup, model task init
 *   2. game_init() - NVRAM options, high scores, game state
 *
 * N64 version must also:
 *   - Decompress game code from ROM
 *   - Create multiple threads for audio/render/game
 *
 * @param arg Argument passed through from main
 */
static void game_init_thread(void *arg) {
    void *frame_counter;
    OSMesg msg;

    /* Set FPU control/status register (enable all exceptions) */
    __setfpcsr(FPCSR_INIT);

    /* Initialize message queues (N64-specific IPC) */
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, 8);
    osCreateMesgQueue(&gInflateMesgQueue, gInflateMesgBuf, 8);
    osCreateMesgQueue(&gRetraceMesgQueue, gRetraceMesgBuf, 60);
    osCreateMesgQueue(&gEventMesgQueue, gEventMesgBuf, 8);

    /* Get frame counter and set up timer interrupts */
    frame_counter = (void *)osScGetFrameCount();
    osScCreateThread(gScheduler, gGameThread, 12, frame_counter, 1);

    /* Finalize DMA for first region */
    dma_finalize(gDecompressedDataStart,
                 (u32)(gDecompressedDataEnd1 - gDecompressedDataStart));

    /* Invalidate D-cache for region to be decompressed into */
    osInvalDCache(gDecompressedDataEnd1,
                  (u32)(gBssStart - gDecompressedDataEnd1));

    /* Decompress main game data from ROM */
    inflate_entry((void *)ROM_COMPRESSED_DATA, gDecompressedDataStart, 0);

    /* Clear BSS section */
    bzero(gBssStart, (u32)(gBssEnd - gBssStart));

    /* Set up retrace handling */
    osScStartRetrace(gScheduler, &msg, &gRetraceMesgQueue);

    /* Create audio thread (thread 8) */
    osCreateThread(gAudioThread, 8, audio_thread_proc, NULL,
                   gAudioThreadStack + 0x960, 3);

    /* Set up sync message queue */
    osCreateMesgQueue(&gSyncMesgQueue, gSyncMesgBuf, 1);
    osJamMesg(&gSyncMesgQueue, NULL, 1);

    /* Start audio thread */
    osStartThread(gAudioThread);

    /* Create render thread (thread 5) */
    osCreateThread(gRenderThread, 5, render_thread_entry, NULL,
                   gRenderThreadStack + 0x960, 7);
    osStartThread(gRenderThread);

    /* Late initialization (arcade: part of init() sequence) */
    game_late_init();
    sound_init();

    /* Create main game thread (thread 7) */
    osCreateThread(gGameThread, 7, (void *)game_thread_entry, arg,
                   gGameThreadStack + 0x12C0, 5);

    /* Write magic sync value */
    gSyncMagic = 0x0ABF;

    /* Start main game thread if not in special mode */
    if (gInitFlag == 0) {
        osStartThread(gGameThread);
    }

    /* Wait for sync signal (0x0ABE) from game thread */
    do {
        osRecvMesg(&gRetraceMesgQueue, &msg, 1);
    } while (*(s16 *)msg != 0x0ABE);

    /* Clear game state flag */
    gGameStateFlag = 0;

    /* Start audio playback */
    audio_start();

    /* Main loop - runs per-frame game logic */
    for (;;) {
        game_frame_update();
    }
}

/**
 * Main game thread entry (thread 7)
 * 0x800024FC - Runs main game rendering loop
 *
 * This is equivalent to the arcade's game_loop() which runs
 * in an infinite loop calling game() each frame.
 *
 * @param arg Argument passed through
 */
static void game_thread_entry(void *arg) {
    /* Set FPU control/status register */
    __setfpcsr(FPCSR_INIT);

    /* Main rendering loop (arcade: game_loop()) */
    for (;;) {
        game_loop();
    }
}

/*===========================================================================*/
/*                     ARCADE-COMPATIBLE STUB FUNCTIONS                      */
/*===========================================================================*/

#ifdef NON_MATCHING

/**
 * Powerup initialization (arcade version)
 *
 * In the arcade version, this is called from start() and performs
 * all hardware and subsystem initialization. On N64, this is split
 * across game_init_thread() and the functions it calls.
 *
 * Arcade sequence:
 *   InitMemHandler2() -> InitializeTimerIRQ() -> InitError() ->
 *   InitStrings() -> HDStart() -> ZOID_Reset() -> InitSW() ->
 *   PotsInit() -> init_glob() -> loadInit() -> reinit_zoid() ->
 *   modelinit() -> InitTrkSel() -> ZeroCamera() -> InitBlits() ->
 *   InitAttract() -> InitMoobs() -> init_drone_handles() ->
 *   init_car_sound_handles() -> DBG_init_debug()
 */
void init(void) {
    /* Arcade: Initialize memory handler */
    /* InitMemHandler2(mem_pool_start, GUTS_STACK_BYTES); */

    /* Arcade: Initialize timer IRQ */
    /* InitializeTimerIRQ(); */

    /* Arcade: Initialize error handling */
    /* InitError(); */

    /* Arcade: Initialize string system */
    /* InitStrings(); */

    /* Arcade: Initialize hard drive access */
    /* HDStart(); */

    /* Arcade: Reset graphics system */
    /* ZOID_Reset(); */

    /* Arcade: Initialize steering wheel */
    /* swforce = 0; */
    /* InitSW(); */
    /* freewheel(); */
    /* frictwheel(); */

    /* Arcade: Initialize potentiometers (analog inputs) */
    /* PotsInit(); */

    /* Arcade: Initialize global variables */
    /* init_glob(); */

    /* Arcade: Load initial data from storage */
    /* loadInit(); */

    /* Arcade: Reset graphics with game settings */
    /* reinit_zoid(); */

    /* Arcade: Initialize physics model */
    /* modelinit(); */

    /* Arcade: Initialize track selection */
    /* InitTrkSel(); */

    /* Arcade: Zero camera position */
    /* ZeroCamera(); */

    /* Arcade: Initialize 2D blitter */
    /* InitBlits(); */

    /* Arcade: Initialize attract mode */
    /* InitAttract(); */

    /* Arcade: Initialize mobile objects */
    /* InitMoobs(); */

    /* Arcade: Initialize sound handles */
    /* init_drone_handles(); */
    /* init_car_sound_handles(); */

    /* Arcade: Initialize debug features */
    /* DBG_init_debug(); */

    /* Set initial game state */
    gstate = ATTRACT;

    /* No track loaded yet */
    last_track_loaded = -1;

    /* Arcade: Initialize hint table and game stats */
    /* init_hint_table(); */
    /* init_game_stats(); */
}

/**
 * Game state initialization (arcade version)
 *
 * In the arcade version, this reads NVRAM settings, initializes
 * game options, loads high scores, and sets up the HUD.
 *
 * On N64, this functionality is spread across:
 *   - game_late_init() - late subsystem init
 *   - Controller Pak access for saves
 *   - game.c state machine initialization
 */
void arcade_game_init(void) {
    /* Arcade: Read NVRAM option settings */
    /* guts_opt  = eer_gets(EER_GUTS_OPT); */
    /* game_opt  = eer_gets(EER_GMOPT); */
    /* track_opt = eer_gets(EER_TRKOPT); */
    /* lap_opt   = eer_gets(EER_LAPOPT); */

    /* Arcade: Handle clear high scores option */
    /* Arcade: Handle factory reset option */
    /* Arcade: Set difficulty per track */
    /* Arcade: Set lap count per track */

    /* Arcade: Handle attract mode audio settings */
    /* Arcade: Initialize HUD */
    /* Arcade: Initialize display system */
    /* Arcade: Load high scores */
    /* Arcade: Load tourney settings */

    /* Clear steering wheel force */
    /* gSWforce = 0; */
}

/**
 * VBlank handler (arcade version)
 *
 * Called when vertical blank interrupt occurs.
 * Signals the main loop that a frame is complete.
 */
void frame_done(void) {
    gotvb = 1;
}

/**
 * Re-initialize graphics parameters (arcade version)
 *
 * Sets up view angle, ambient lighting, and light direction.
 * On arcade this uses the MBOX (Mathbox) and ZOID (3Dfx) APIs.
 * On N64, equivalent setup is done via RDP commands.
 */
void reinit_graphics(void) {
    /* Arcade: Set 90 degree field of view */
    /* win = MBOX_SetWindow(90.0, 0); */
    /* win->ambientlevel = 0.0; */

    /* Arcade: Set ambient light level */
    /* ZOID_SetAmbient(0, 0.3, 0); */

    /* Arcade: Set light direction (normalized) */
    /* lightdir[0] = 0.577; */
    /* lightdir[1] = -0.577; */
    /* lightdir[2] = 0.577; */
    /* ZOID_SetLightDir(lightdir, 1); */
}

/**
 * Initialize global variables (arcade version)
 *
 * Resets all global game state to default values.
 * Called from init() at powerup.
 */
void init_glob(void) {
    /* Arcade: Reset camera position and orientation */
    /* gCamPos[0] = gCamPos[1] = gCamPos[2] = 0.0; */

    /* Arcade: Reset game state */
    /* gstate = ATTRACT; */

    /* Arcade: Reset track state */
    /* trackno = 0; */
    /* last_track_loaded = -1; */

    /* Arcade: Reset car state */
    /* num_active_cars = 0; */
    /* num_humans = 0; */
    /* num_drones = 0; */

    /* Arcade: Reset timing */
    frames = 0;
    gotvb = 0;
    gCurGameTime = 0.0f;
    gLastGameTime = 0.0f;
    gGameLoopTime = 0.0f;
    gGameExecTime = 0.0f;
}

#endif /* NON_MATCHING */
