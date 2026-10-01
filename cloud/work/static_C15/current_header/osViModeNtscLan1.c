/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern OSTask *osViModeTableGet(OSTask*);
extern void __osSpSetStatus(u32);
extern s32 __osSpSetPc(u32),__osSpDma(s32,u32,void*,u32);
#define OS_TASK_LOADABLE 4
#define OS_YIELD_DATA_SIZE 0xC00
#define IO_READ(x) (*(volatile u32*)((u32)(x)|0xA0000000))
#define SP_CLR_YIELD 0x200
#define SP_CLR_YIELDED 0x800
#define SP_CLR_TASKDONE 0x2000
#define SP_SET_INTR_BREAK 0x100
#define SP_IMEM_START 0x04001000

void osViModeNtscLan1(OSTask* intp) {
    OSTask* tp;


    tp = osViModeTableGet(intp);

    if (tp->t.flags & OS_TASK_YIELDED) {
        tp->t.ucode_data = tp->t.yield_data_ptr;
        tp->t.ucode_data_size = tp->t.yield_data_size;
        intp->t.flags &= ~OS_TASK_YIELDED;
        if (tp->t.flags & OS_TASK_LOADABLE) {
            tp->t.ucode = (u64*)IO_READ((u32)intp->t.yield_data_ptr + OS_YIELD_DATA_SIZE - 4);
        }
    }

    osWritebackDCache(tp, sizeof(OSTask));
    __osSpSetStatus(SP_CLR_YIELD | SP_CLR_YIELDED | SP_CLR_TASKDONE | SP_SET_INTR_BREAK);

    while (__osSpSetPc(SP_IMEM_START) == -1) {}

    while (__osSpDma(1, (SP_IMEM_START - sizeof(*tp)), tp, sizeof(OSTask)) == -1) {}

    while (__osSpDeviceBusy()) {}

    while (__osSpDma(1, SP_IMEM_START, tp->t.ucode_boot, tp->t.ucode_boot_size) == -1) {}
}
