/* LLM-grown type context for auto-decompilation (autodecomp #1).
 *
 * m2c produces `void *` for globals it can't type and then dereferences them
 * (`g->unk8`), which won't compile. Each struct/typedef/extern added here
 * gives m2c real layouts, so every function that touches that symbol compiles
 * and can be searched. One definition typically unlocks a cluster.
 *
 * Rules: types/typedefs/struct defs and `extern` global declarations only, no
 * function bodies. Field names may be `unkN` (offset N) until known. Keep it
 * self-consistent; `autodecomp clusters` re-measures after every edit.
 */
#ifndef M2C_TYPES_H
#define M2C_TYPES_H

/* --- VI (video interface) context: matches libultra __OSViContext. -----
 * osViInit (work/libultra/vi/osViInit, func_8000C540) builds this struct at
 * D_8002C430/D_8002C400 and stores its address into __osViContext
 * (D_8002C464); offsets 0x0/0x2/0x4/0x8/0xC line up exactly with the real
 * SGI/Nintendo layout (state, retraceCount, framep, modep, control). */
typedef struct {
    u32 ctrl, width, burst, vSync, hSync, leap, hStart, xScale, vCurrent;
} OSViCommonRegs;

typedef struct {
    u32 origin, yScale, vStart, vBurst, vIntr;
} OSViFieldRegs;

typedef struct {
    u8 type;
    OSViCommonRegs comRegs;
    OSViFieldRegs fldRegs[2];
} OSViMode;

typedef struct {
    f32 factor;
    u16 offset;
    u32 scale;
} __OSViScale;

typedef struct {
    u16 state;
    u16 retraceCount;
    void *framep;
    OSViMode *modep;
    u32 control;
    OSMesgQueue *msgq;
    OSMesg msg;
    __OSViScale x;
    __OSViScale y;
} __OSViContext;

extern __OSViContext *__osViContext;

/* --- Thread scheduling internals (libultra osint.h), same signatures as
 * the real SDK; the symbols here are misattributed names left over from an
 * earlier auto-naming pass (__osCleanupThread's body is really
 * __osEnqueueAndYield; dll_remove's is __osDequeueThread) but the callers
 * only need a correct signature to compile. */
extern void __osCleanupThread(OSThread **queue);
void dll_remove(OSThread **queue, OSThread *thread);

/* --- Serial interface raw DMA (libultra os_internal_si.h). ------------- */
extern s32 __osSiRawStartDma(s32 direction, void *dramAddr);

/* --- SI/PFS staging buffers: raw PIF-RAM-sized scratch buffers that
 * controller code builds request/read blocks into before/after DMA.
 * Real libultra OSPifRam is a 64-byte, 0x10-aligned block (15 words +
 * pifstatus); __osPfsBuffer is used the same way by the PFS read path. */
typedef struct {
    u32 ramarray[15];
    u32 pifstatus;
} OSPifRam;

extern OSPifRam __osSiDmaBuffer;
extern u8 __osPfsBuffer[64];

/* __osContRamWrite: standard libultra Controller Pak raw write
 * (PRinternal/controller.h: s32 __osContRamWrite(OSMesgQueue*, int,
 * u16, u8*, int)). */
extern s32 __osContRamWrite(OSMesgQueue *mq, s32 channel, u16 address,
                             u8 *buffer, s32 force);

/* --- Viewport offset tables (src/game/display.c territory, but this
 * dynamic-code copy indexes them directly): gViewportOffsetX/Y are read
 * with the viewport index scaled by 8 bytes (4 s16s) per symbol_addrs.us.txt,
 * which also documents adjacent gViewportOffsetXExtra/YExtra 4 bytes
 * earlier in the same table. */
extern s16 gViewportOffsetX[32];
extern s16 gViewportOffsetY[32];

/* --- Decompression DMA sync helpers (src/util/dma.c, not visible to m2c's
 * type context since that's a .c file, not a header). */
extern s32 dma_wait(s32 blocking);
extern void dma_signal(void);

/* --- SI DMA retry counter: the last word of the __osSiDmaBuffer/OSPifRam
 * block (0x80037ADC = __osSiDmaBuffer + 0x3C, i.e. its pifstatus field),
 * but symbol_addrs.us.txt gives it its own symbol so m2c needs its own
 * extern too. */
extern s32 __osSiDmaRetry;

/* --- Real name for func_8000CFC4's body (see __osCleanupThread above;
 * ultralib's exceptasm.s confirms this is __osEnqueueAndYield, called
 * directly under its real name elsewhere too). */
extern void __osEnqueueAndYield(OSThread **queue);

/* --- Controller Pak raw read (PRinternal/controller.h counterpart to
 * __osContRamWrite above). */
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel, u16 address,
                            u8 *buffer);

/* --- Scheduler internals (src/libultra/os_scheduler.c has the same
 * layout reconstructed as SCHED_ and TASK_ offset macros operating on
 * void pointers; m2c needs real field types since it decompiles
 * __scAppendList/__scExec's own bodies directly from ROM asm, not through
 * those macros). */
typedef struct OSScTask_s {
    struct OSScTask_s *next;   /* 0x00 */
    s32 state;                  /* 0x04 */
    s32 flags;                  /* 0x08 */
    void *framebuffer;          /* 0x0C */
    s32 type;                   /* 0x10 */
    u8 pad14[0x38 - 0x14];
    void *unk38;                 /* 0x38 */
    s32 *unk3C;                  /* 0x3C */
    u8 pad40[0x50 - 0x40];
    OSMesgQueue *msgQueue;        /* 0x50 */
    OSMesg msg;                    /* 0x54 */
} OSScTask;

typedef struct OSScClient_s {
    struct OSScClient_s *next;  /* 0x00 */
    OSMesgQueue *msgQueue;       /* 0x04 */
} OSScClient;

typedef struct {
    s16 state;                        /* 0x00 */
    u8 pad02[0x20 - 0x02];
    s16 priority;                     /* 0x20 */
    u8 pad22[0x40 - 0x22];
    OSMesgQueue cmdQueue;              /* 0x40 */
    OSMesg cmdMsgs[8];                 /* 0x58 */
    OSMesgQueue retQueue;              /* 0x78 */
    OSMesg retMsgs[8];                 /* 0x90 */
    u8 padB0[0x260 - 0xB0];            /* thread lives here */
    OSScClient *clientList;            /* 0x260 */
    OSScTask *rspTaskHead;             /* 0x264 */
    OSScTask *rspTaskTail;             /* 0x268 */
    OSScTask *rdpTaskHead;             /* 0x26C */
    OSScTask *rdpTaskTail;             /* 0x270 */
    OSScTask *curRSPTask;              /* 0x274 */
    OSScTask *curRDPTask;              /* 0x278 */
    s32 retraceCount;                  /* 0x27C */
    s32 audioListPending;              /* 0x280 */
} OSSched;

extern void __scAppendList(OSSched *sc, OSScTask *task);
extern void __scExec(OSSched *sc, OSScTask *rspTask, OSScTask *rdpTask);

/* osViSetMode/display_mode_tick (symbol_addrs.us.txt names for these two
 * addresses); osViSetMode's caller here passes a task framebuffer pointer,
 * not necessarily a real OSViMode*, so keep the param generic. */
extern void osViSetMode(void *mode);
extern void display_mode_tick(void);

/* --- Inflate double-buffers + full PI DMA request (this ROM's
 * __osPiRawStartDma is the 7-arg osPiStartDma-shaped call: mb, priority,
 * direction, devAddr, dramAddr, size, mq). */
extern u8 gInflateBufferA[0x1000];
extern u8 gInflateBufferB[0x1000];
extern s32 __osPiRawStartDma(void *mb, s32 priority, s32 direction,
                              u32 devAddr, void *dramAddr, u32 size,
                              OSMesgQueue *mq);

/* --- PI manager (symbol_addrs.us.txt: osPiInit/osPiGetAccess/
 * osPiReleaseAccess/__osPiMgrState, standard libultra signatures). */
typedef struct {
    s32 flag;   /* 0x00 */
    u8 pad04[0x8 - 0x4];
    s32 unk8;   /* 0x08 */
    u8 pad0C[0x1C - 0xC];
} __OSPiMgrState;

extern __OSPiMgrState __osPiMgrState;
extern void osPiInit(void);
extern void osPiGetAccess(void);
extern void osPiReleaseAccess(void);
extern u32 osRomBase;

/* --- RCP/timer/scheduler internals confirmed against ultralib
 * (os_internal_reg.h, os_internal_rsp.h). */
extern void __osSetCompare(u32 compare);
extern void __osSpSetStatus(u32 status);
extern void osDpWait(void);

/* --- __setfpcsr/game_loop: audio_thread_entry sets the FPU control/status
 * register once then loops calling the dynamic per-frame game state
 * machine at 0x800FD464 (symbol_addrs.us.txt: game_loop, 704 bytes). */
extern void __setfpcsr(u32 value);
extern void game_loop(void);

#endif
