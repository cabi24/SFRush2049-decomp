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
extern s32 osPiReadWord(u32, u32 *);

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
extern void __osContRamReset(u8);
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

/* Additional queue context for verified static packet C4. */
/* Required existing declaration corrections, not conflicting redeclarations:
 * include/PR/os_thread.h's priority-style osSetThreadPri prototype is historically
 * misattributed. Proven body at 0x80006D40 is actually SDK osViSetMode.
 * Delivered ABI signature: void osSetThreadPri(void *mode).
 * mode points to OSViMode; the body casts it to OSViMode* before assigning it.
 * The second old priority argument is unused and its spill causes a strict
 * mismatch, so the old 2-argument prototype cannot remain for this definition.
 * Preserve old ROM gates and audit any linked callers before changing it.
 * Existing __osActiveQueue global should be OSThread*, not OSThread**.
 * Canonical SDK run-queue head has current historical name __osActiveQueue;
 * canonical active-list head has current historical name __osRunQueue (C3).
 */
extern void __osDispatchThread(void);
extern OSThread *__osPopThread(OSThread **);
#define OS_STATE_WAITING 8
#define OS_STATE_RUNNABLE 2
#define OS_STATE_STOPPED 1
#define MQ_IS_EMPTY(mq) ((mq)->validCount == 0)
/* OS_MESG_NOBLOCK/BLOCK already exist from PR/os_message.h.
 * __osCleanupThread(OSThread**) already declared in m2c_types.h and is current
 * target name for SDK __osEnqueueAndYield.
 * __osRunningThread and __osEnqueueThread already declared.
 * Blocked O0 boot leads need extern s32 gScTimeEnabled, but don't promote them
 * into the locked O2 0x1050 TU until a supported flags/isolation route exists. */

/* SI raw I/O helpers retain the historical PI device-busy symbol. */
extern s32 __osPiDeviceBusy(void);

/* Verified static C6 SDK context; historical names retain retail linkage. */
extern s32 osPfsReadWriteFile_pages(OSPfs *);
extern s32 gAudioDmaBufferPtr;
extern u32 gViMgrEventCount;
extern s8 gSpTaskFlags0, gSpTaskFlags1, gSpTaskFlags2, gSpTaskFlags3, gSpTaskFlags4;
extern s8 gSpTaskResultA, gSpTaskResultB, gSpTaskResultC, gSpTaskResultD, gSpTaskResultE;
extern void guMtxIdentF(f32 [4][4]);
extern f32 cosf(f32);
extern f32 sinf(f32);
extern const f64 gOrthoScale;
#define VI_STATE_XSCALE_UPDATED 2
#define VI_STATE_YSCALE_UPDATED 4
#define VI_STATE_BLACK 0x20
#define VI_STATE_REPEATLINE 0x40
#define VI_STATE_FADE 0x80
#define VI_SCALE_MASK 0xFFF
#define VI_SUBPIXEL_SH 16
#define VI_2_10_FPART_MASK 0x3FF
#define IO_READ(addr) (*(volatile u32 *)(addr))
#define IO_WRITE(addr,value) (*(volatile u32 *)(addr)=(value))
#define VI_CURRENT_REG 0xa4400010U
#define VI_ORIGIN_REG 0xa4400004U
#define VI_WIDTH_REG 0xa4400008U
#define VI_BURST_REG 0xa4400014U
#define VI_V_SYNC_REG 0xa4400018U
#define VI_H_SYNC_REG 0xa440001cU
#define VI_LEAP_REG 0xa4400020U
#define VI_H_START_REG 0xa4400024U
#define VI_V_START_REG 0xa4400028U
#define VI_V_BURST_REG 0xa440002cU
#define VI_INTR_REG 0xa440000cU
#define VI_X_SCALE_REG 0xa4400030U
#define VI_Y_SCALE_REG 0xa4400034U
#define VI_CONTROL_REG 0xa4400000U
#define AI_MIN_DAC_RATE 132
#define AI_MAX_BIT_RATE 16
#define AI_DACRATE_REG 0xA4500010U
#define AI_BITRATE_REG 0xA4500014U
#define PFS_CHECK_STATUS() if ((pfs->status & 1)==0) return 5
#define ERRCK(fn) ret=fn; if(ret!=0) return ret
#define ARRLEN(a) ((int)(sizeof(a)/sizeof((a)[0])))
#define PFS_PAGE_NOT_USED 3
#define PFS_ONE_PAGE 8
#define BLOCKSIZE 32
#define FTOFIX32(x) ((long)((x)*(float)0x00010000))

/* Verified canonical controller/message context, packet C7. */
/* Minimal additions for seven eligible C7 bodies. Existing OSPifRam,
 * __OSContReadFormat, CHNL_ERR, ARRLEN, OS_STATE_WAITING, message constants,
 * __osPfsRequestType/2, __osCleanupThread and guMtxIdentF are sufficient. */
typedef struct {
    u8 dummy, txsize, rxsize, cmd, typeh, typel, status, dummy1;
} __OSContRequesFormat;
typedef struct {
    u8 txsize, rxsize, cmd, typeh, typel, status;
} __OSContRequesFormatShort;
#define CONT_CMD_EXE 1
#define CONT_CMD_NOP 0xFF
#define CONT_CMD_END 0xFE
#define CONT_CMD_READ_BUTTON 1
#define CONT_CMD_READ_BUTTON_TX 1
#define CONT_CMD_READ_BUTTON_RX 4
#define CONT_CMD_RESET_TX 1
#define CONT_CMD_RESET_RX 3
/* CONT_CMD_REQUEST_STATUS 0 already exists. */
#define CONT_CMD_REQUEST_STATUS_TX 1
#define CONT_CMD_REQUEST_STATUS_RX 3
#define MQ_IS_FULL(mq) ((mq)->validCount >= (mq)->msgCount)
extern void __osContBuildRequest(int, u8);
extern void __osContParseResponse(int, OSContStatus *);
extern f32 sqrtf(f32);

/* Verified canonical PFS/PI context, packet C8. */
/* Minimal context for C8, retaining existing C1-C7 types/macros. */
extern s32 __osContPifBuf;
extern OSMesgQueue __osContQueryMsgQ;
extern OSMesg __osContQueryMesg;
extern u8 __osSiChannelMask;
extern s32 __osInsertTimer(void); /* accepted pointer-valued s32 body */
extern s32 __osContBuildPacket(OSMesgQueue *, u8 *, OSContStatus *);
extern s32 osPfsGetFileStat(OSPfs *, u8 *, __OSInode *, __OSInodeUnit *);
#define CONT_BLOCK_DETECT 0x400
#define CHECK_IPAGE(p) (((p).ipage >= pfs->inode_start_page) && ((p).inode_t.bank < pfs->banks) && ((p).inode_t.page >= 0x01) && ((p).inode_t.page < 0x80))
/* C6 already supplies ERRCK, PFS_ERR_INVALID/INCONSISTENT, PFS_READ,
 * PFS_PAGE_NOT_USED and inode context. Existing PR headers supply PFS_EOF
 * and PFS_ERR_NOPACK/NEW_PACK/CONTRFAIL; avoid duplicate definitions.
 * OSPri/OS_STATE_STOPPED/OS_STATE_RUNNABLE and thread context already suffice.
 * Existing osCreateViManager(OSThread*,OSPri) is correct, despite its name. */


/* Verified canonical PFS identity/motor context, packet C9. */
/* Minimal C9 context; existing OSPfs/__OSInode/OSPifRam/thread types suffice. */
typedef struct {
    u32 repaired, random;
    u64 serial_mid, serial_low;
    u16 deviceid;
    u8 banks, version;
    u16 checksum, inverted_checksum;
} __OSPackId;
typedef struct {
    u8 dummy, txsize, rxsize, cmd, addrh, addrl, data[32], datacrc;
} __OSContRamReadFormat;
extern u8 __osMotorPifBuf[256];
extern s32 __osIdCheckSum(u16 *, u16 *, u16 *);
extern s32 __osCheckId(OSPfs *, __OSPackId *);
extern s32 __osGetId(OSPfs *);
extern s32 __osRepairId(OSPfs *, __OSPackId *, __OSPackId *);
extern s32 __osTimerInterrupt(OSPfs *); /* actual SDK __osPfsCheckRamArea */
extern void __osMotorAccess(int, OSPifRam *); /* actual __osMakeMotorData */
extern u8 __osContAddressCrc(u16);
#define SELECT_BANK(pfs,bank) __osPfsSelectBank(pfs,bank)
#define SET_ACTIVEBANK_TO_ZERO() if(pfs->activebank!=0) { ERRCK(__osPfsSelectBank(pfs,0)); } (void)0
#define PFS_ID_0AREA 1
#define PFS_ID_1AREA 3
#define PFS_ID_2AREA 4
#define PFS_ID_3AREA 6
#define PFS_LABEL_AREA 7
#define DEF_DIR_PAGES 2
#define CONT_CMD_WRITE_PAK 3
#define CONT_CMD_WRITE_PAK_TX 35
#define CONT_CMD_WRITE_PAK_RX 1
#define CONT_BLOCK_RUMBLE 0x600
#define CHNL_ERR_MASK 0xC0
#define READFORMAT(ptr) ((__OSContRamReadFormat*)(ptr))
/* Existing C6/C7/C8 macros supply ERRCK, ARRLEN, BLOCKSIZE, CONT_CMD_*,
 * OS_READ/WRITE, PFS_ONE_PAGE. Public PR/os_pfs.h defines PFS_ERR_*,
 * PFS_INITIALIZED, PFS_ID_BROKEN, PFS_MOTOR_INITIALIZED.
 * C8 requires corrected osContStartReadData(OSMesgQueue*,int).
 * Existing C6 osPfsReadWriteFile_pages(OSPfs*) declaration remains correct. */


extern s32 osMotorInit(OSPfs *, s32); /* historical SDK __osMotorAccess */

/* Verified canonical directory, controller and event context, packet C10. */
extern s32 __osContLastChannel;
extern u8 __osPfsDataChecksum(u8 *); /* historical SDK data CRC helper */
extern s32 osPfsRename(OSPfs *, u16, u32, u8 *, u8 *);
extern s32 osMotorStart(OSMesgQueue *, OSPfs *, int);
#define PFS_ID_BANK_256K 0
#define CONT_CMD_READ_PAK 2
#define CONT_CMD_READ_PAK_TX 3
#define CONT_CMD_READ_PAK_RX 33
typedef u32 OSEvent;
typedef struct { OSMesgQueue *messageQueue; OSMesg message; } __OSEventState;
extern __OSEventState __osEventStateTab[16];
extern u32 __osShutdown;
extern u32 gEventTypeFlag;
extern void osSetEventMesgAlt(OSEvent, OSMesgQueue *, OSMesg);
#define OS_EVENT_PRENMI 14

/* Verified PFS checker/VI initialization context, packet C11. */
typedef struct { __OSInode inode; u8 bank; u8 map[256]; } __OSInodeCache;
#define PFS_INODE_DIST_MAP 256
#define PFS_SECTOR_SIZE 4
#define PFS_BANK_LAPPED_BY 8
extern u8 __osViMode[96], __osViModeTable[48];
extern OSViMode __osViModePending, __osViModeNext, __osViModeBuffer;
#define K0BASE 0x80000000
#define OS_TV_TYPE_PAL 0
#define OS_TV_TYPE_MPAL 2
extern u32 osTvType;
extern void __osViSwapContext(void);

/* Verified canonical timer interrupt context, packet C11. */
extern u32 __osTimerCounter;
extern u32 osGetCount(void);

/* Verified PFS allocation/checker and EPi DMA context, packet C12. */
extern s32 osPfsGetFileSize(OSPfs *, s32, u8, int, int, u8 *);
extern s32 __osPfsDeclearPage(OSPfs *, __OSInode *, int, int *, u8, int *, int *);
extern s32 osPiSetDeviceTiming(OSPiHandle *, s32, u32, void *, u32);
extern OSPiHandle *__osPiDevList[2];
#define PFS_CHECK_ID() if(osPfsReadWriteFile_pages(pfs)==PFS_ERR_NEW_PACK) return PFS_ERR_NEW_PACK
#define ROUND_UP_DIVIDE(n,d) (((n)+(d)-1)/(d))
#define PFS_DATA_FULL 7
#define PFS_DIR_FULL 8
#define DIR_STATUS_OCCUPIED 2
#define DIR_STATUS_EMPTY 0
