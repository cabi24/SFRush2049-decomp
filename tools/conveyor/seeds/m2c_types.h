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

#endif
