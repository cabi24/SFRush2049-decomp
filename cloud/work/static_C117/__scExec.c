/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
#include "rom_tu.h"
extern u64 __osScAudioStartHi;
extern u64 __osScFrameTimeHi;
extern u64 __osScRetraceTimeHi;
extern OSScTask *__osScCurAudioTask;
extern void osViModeNtscLan1(void *);
extern void osViModeNtscLpn1(void *);
/* Actual three-input RCP task execution. Historical cache prototype is wrong:
 * native helper consumes no inputs; grounded explicit true signature cast
 * preserves physical symbol/current common header, with no invented args. */
void __scExec(OSSched *scheduler, OSScTask *sp, OSScTask *dp)
{
    int rv;
    if (scheduler->curRSPTask) { }
    if (sp) {
        if (sp->type == 2) {
            __osScAudioStartHi = osGetTime();
            __osScCurAudioTask = sp;
        } else {
            if (!(sp->state & 0x20))
                __osScRetraceTimeHi = __osScFrameTimeHi;
        }
        ((void (*)(void))osInvalICache)();
        sp->state &= ~0x30;
        osViModeNtscLan1(&sp->type);
        osViModeNtscLpn1(&sp->type);
        scheduler->curRSPTask = sp;
        if (sp == dp)
            scheduler->curRDPTask = dp;
    }
    if (dp && (dp != sp)) {
        if (!dp->unk38) { }
        rv = osDpSetNextBuffer(dp->unk38, *(u64 *)dp->unk3C);
        if (rv != 0) { }
        scheduler->curRDPTask = dp;
    }
}
