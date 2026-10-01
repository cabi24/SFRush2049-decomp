/* Canonical SDK SP-task context; keep shared IO_READ behavior unchanged. */
extern OSTask gViModeTempBuffer;
extern OSTask *osViModeTableGet(OSTask *);
extern void osViModeNtscLan1(OSTask *);
extern void __osSpSetStatus(u32);
extern s32 __osSpSetPc(u32);
extern s32 __osSpDma(s32, u32, void *, u32);
#define OS_TASK_LOADABLE 4
#define OS_YIELD_DATA_SIZE 0xC00
#undef IO_READ
#define IO_READ(addr) (*(volatile u32 *)((u32)(addr) | 0xA0000000))
#define _osVirtualToPhysical(ptr) if (ptr != NULL) { ptr = (void *)osVirtualToPhysical(ptr); } (void)0
#define SP_CLR_YIELD 0x200
#define SP_CLR_YIELDED 0x800
#define SP_CLR_TASKDONE 0x2000
#define SP_SET_INTR_BREAK 0x100
#define SP_IMEM_START 0x04001000

