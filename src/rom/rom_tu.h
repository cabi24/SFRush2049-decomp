/* Common includes for ROM-aligned TUs (004). Passthrough slots need nothing;
 * promoted bodies add what they use here (keep it additive and collision-free
 * — one definition per hardware register across all ROM TUs). */
#include "types.h"
#include "rom_auto.h"   /* auto-decomp type context (autodecomp promotion) */
#include "PR/os_message.h"
#include "PR/os_thread.h"

/* Data symbols referenced by promoted libultra bodies. */
extern OSThread *__osEmptyMesgQueue;   /* 0x8002C3D0 — empty mesg queue sentinel */

/* Referenced by huft_alloc (inflate's window allocator). C89 makes an
 * undeclared VARIABLE an error, so a promoted body that touches a global
 * needs it here even though the body compiles standalone with its own m2c
 * prelude. Types follow the body's use: a byte offset accumulated into a
 * base. The names come from symbol_addrs and read as display-list symbols in
 * inflate code, which is suspect — see docs/SYMBOL_MISATTRIBUTION.md. */
extern s32 gDisplayListHead;            /* 0x800354C4 */
extern volatile unsigned int gDisplayListSize;  /* 0x800354C8 */

/* N64 MMIO registers referenced by promoted libultra bodies. IDO compiles
 * these #define'd KSEG1 addresses to literal immediates (no relocation) —
 * the same fact behind 003's KSEG1 de-symbolization. */
#define SP_STATUS_REG    (*(vu32 *)0xA4040010)
#define SP_PC_REG        (*(vu32 *)0xA4080000)
#define SP_STATUS_HALT     0x0001
#define SP_STATUS_DMA_BUSY 0x0004
#define SP_STATUS_IO_FULL  0x0010
#define DPC_STATUS_REG   (*(vu32 *)0xA410000C)
#define DPC_CLOCK_REG    (*(vu32 *)0xA4100010)
#define DPC_BUFBUSY_REG  (*(vu32 *)0xA4100014)
#define DPC_PIPEBUSY_REG (*(vu32 *)0xA4100018)
#define DPC_TMEM_REG     (*(vu32 *)0xA410001C)
#define AI_STATUS_REG    (*(vu32 *)0xA450000C)
#define AI_STATUS_FIFO_FULL 0x80000000

/* Additive declarations for shared src/rom/rom_tu.h; existing types suffice. */
extern s32 __osViModeInfo;
extern s32 gDisplayListEnd;
extern s32 gViAccumTime;
extern s32 gViTickCounter;
extern s32 __osPiInitialized;
extern OSMesgQueue __osPiMesgQueue;
extern void *__osPiMesg;
extern s32 __osSiInitialized;
/* The SI names have historical swapped attribution; these match current source. */
extern OSMesgQueue __osSiMesg;
extern void *__osSiMesgQueue;
extern u8 __osPfsRequestType;
/* __osViContext and OSPifRam __osSiDmaBuffer already exist in m2c_types.h.
 * gDisplayListHead and volatile gDisplayListSize already exist in rom_tu.h.
 * No new struct/type definitions are needed. */

/* PI read wrapper uses the currently identified target signature. */
extern s32 osPiReadWord(s32, s32);

/* Additional libultra context for verified static packet C2. */
/* Existing OSTask, OSContStatus, OSContPad, __OSViContext, OSPifRam types suffice.
 * __osViModeInfo remains s32, as in C1; C2 explicitly casts its loaded value.
 * No change to accepted C1 declarations is needed. */
extern void __osContGetStatus(u8 *, OSContStatus *);
extern s32 bzero_alt(void);  /* historical target name for SDK __osSpGetStatus */
extern u32 __osTLBLookup(void *);
extern s32 __osSpDeviceBusy(void);
extern u8 __osPfsRequestType2;
typedef u32 OSYieldResult;
typedef struct {
    u8 dummy, txsize, rxsize, cmd;
    u16 button;
    s8 stick_x, stick_y;
} __OSContReadFormat;
#define CHNL_ERR(format) (((format).rxsize & 0xC0) >> 4)
#define SP_STATUS_YIELDED 0x100
#define SP_STATUS_YIELD 0x80
#define OS_TASK_YIELDED 1
#define OS_TASK_DP_WAIT 2
#define IS_KSEG0(x) ((u32)(x) >= 0x80000000U && (u32)(x) < 0xA0000000U)
#define IS_KSEG1(x) ((u32)(x) >= 0xA0000000U && (u32)(x) < 0xC0000000U)
#define K0_TO_PHYS(x) ((u32)(x) & 0x1FFFFFFF)
#define K1_TO_PHYS(x) ((u32)(x) & 0x1FFFFFFF)
/* OS_MESG_NOBLOCK and OS_MESG_BLOCK already available from PR/os_message.h.
 * C1 already supplies __osSiInitialized, __osSiMesg, osSiInit as needed.
 * __osSiDmaBuffer, __osViContext already provided in m2c_types.h. */

/* Additional libultra context for verified static packet C3. */
/* REQUIRED existing-prototype correction before osDpSetNextBuffer promotion:
 * include/PR/os.h currently says void osDpSetNextBuffer(void*, u32).
 * Proven target/SDK signature is s32 osDpSetNextBuffer(void*, u64).
 * Do not simply redeclare a conflicting prototype in rom_tu.h. */

typedef f32 Matrix[4][4];
extern void guPerspectiveF(f32 [4][4], u16 *, f32, f32, f32, f32, f32);
extern void guOrthoF(f32 [4][4], f32, f32, f32, f32, f32, f32, f32);
extern void guLookAtF(f32 [4][4], f32, f32, f32, f32, f32, f32, f32, f32, f32);
extern void guMtxF2L(f32 [4][4], Mtx *);
extern s32 osDpIsBusy(void);
extern void __osContRamReset(s32);
extern OSTime dll_insert(OSTimer *);
extern void dll_reschedule(OSTime);
extern void __osExceptionPanic(void);
extern OSThread *__osRunQueue;
typedef u32 OSIntMask;
#define OS_IM_ALL 0x003FFF01U
#define SR_IMASK 0x0000FF00
#define SR_IE 1
#define SR_EXL 2
#define RCP_IMASK 0x003F0000
#define RCP_IMASKSHIFT 16
#define FPCSR_FS 0x01000000
#define FPCSR_EV 0x800
#define FPCSR_RM_RN 0
#define OS_STATE_STOPPED 1
#define CONT_CMD_REQUEST_STATUS 0
#define OS_WRITE 1
#define OS_READ 0
/* OS_MESG_BLOCK already exists from PR/os_message.h.
 * __osPfsRequestType was supplied by C1; __osSiDmaBuffer exists in m2c_types.h.
 * Existing __osTimerList declaration is __OSTimerNode*. The delivered timer
 * body explicitly casts to OSTimer* before reading ->next, so that shared
 * declaration does not need to change. Both structs have next at offset0.
 * Literal volatile MMIO lvalues in delivered osDpSetNextBuffer need no
 * IO_READ/WRITE or DPC_* macro changes.
 * Delivered thread/timer bodies select the verified pre-K behavior directly;
 * no BUILD_VERSION/VERSION_K shared macros are needed. */
