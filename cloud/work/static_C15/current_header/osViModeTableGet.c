/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern OSTask gViModeTempBuffer;
#define _osVirtualToPhysical(ptr) if(ptr!=NULL) {ptr=(void*)osVirtualToPhysical(ptr);} (void)0

OSTask* osViModeTableGet(OSTask* intp) {
    OSTask* tp;
    tp = &gViModeTempBuffer;
    bcopy(intp, tp, sizeof(OSTask));

    _osVirtualToPhysical(tp->t.ucode);
    _osVirtualToPhysical(tp->t.ucode_data);
    _osVirtualToPhysical(tp->t.dram_stack);
    _osVirtualToPhysical(tp->t.output_buff);
    _osVirtualToPhysical(tp->t.output_buff_size);
    _osVirtualToPhysical(tp->t.data_ptr);
    _osVirtualToPhysical(tp->t.yield_data_ptr);
    return tp;
}
