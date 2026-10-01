/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
#include "context.h"
void osViInit(void) {
    bzero(__osViMode, 96);
    (*(__OSViContext **)&__osViModeInfo) = ((__OSViContext *)__osViMode);
    __osViContext = ((__OSViContext *)__osViModeTable);
    __osViContext->retraceCount = 1;
    (*(__OSViContext **)&__osViModeInfo)->retraceCount = 1;
    __osViContext->framep = (void*)K0BASE;
    (*(__OSViContext **)&__osViModeInfo)->framep = (void*)K0BASE;

    if (osTvType == OS_TV_TYPE_PAL) {
        __osViContext->modep = &__osViModePending;
    } else if (osTvType == OS_TV_TYPE_MPAL) {
        __osViContext->modep = &__osViModeNext;
    } else {
        __osViContext->modep = &__osViModeBuffer;
    }

    __osViContext->state = VI_STATE_BLACK;
    __osViContext->control = __osViContext->modep->comRegs.ctrl;

    while ((*(volatile u32 *)0xA4400010) > 10) { 
    }

    *(volatile u32 *)0xA4400000 = 0; 
    __osViSwapContext();
}
