/* Native flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
#include "scheduler_declarations.h"
extern OSScTask *__scTaskReady(OSSched *, OSScTask *);
s32 __scScheduleCore(OSSched *scheduler, OSScTask **sp, OSScTask **dp, s32 availRCP)
{
    s32 avail = availRCP;
    OSScTask *gfx = scheduler->rspTaskTail;
    OSScTask *audio = scheduler->rspTaskHead;
    if (scheduler->audioListPending && (avail & 2)) {
        if (gfx && (gfx->flags & 0x10)) {
            *sp = gfx;
            avail &= ~2;
        } else {
            *sp = audio;
            avail &= ~2;
            scheduler->audioListPending = 0;
            scheduler->rspTaskHead = scheduler->rspTaskHead->next;
            if (scheduler->rspTaskHead == 0)
                scheduler->rdpTaskHead = 0;
        }
    } else {
        if (__scTaskReady(scheduler, gfx)) {
            switch (gfx->flags & 7) {
                case 3:
                    if (gfx->state & 0x20) {
                        if (avail & 2) {
                            *sp = gfx;
                            avail &= ~2;
                            if (gfx->state & 1) {
                                *dp = gfx;
                                avail &= ~1;
                                if ((avail & 1) == 0)
                                    if (scheduler->curRDPTask != gfx) { }
                            }
                            scheduler->rspTaskTail = scheduler->rspTaskTail->next;
                            if (scheduler->rspTaskTail == 0)
                                scheduler->rdpTaskTail = 0;
                        }
                    } else {
                        if (avail == 3) {
                            *sp = *dp = gfx;
                            avail &= ~3;
                            scheduler->rspTaskTail = scheduler->rspTaskTail->next;
                            if (scheduler->rspTaskTail == 0)
                                scheduler->rdpTaskTail = 0;
                        }
                    }
                    break;
                case 7:
                case 6:
                case 2:
                    if (gfx->state & 2) {
                        if (avail & 2) {
                            *sp = gfx;
                            avail &= ~2;
                        }
                    } else if (gfx->state & 1) {
                        if (avail & 1) {
                            *dp = gfx;
                            avail &= ~1;
                            scheduler->rspTaskTail = scheduler->rspTaskTail->next;
                            if (scheduler->rspTaskTail == 0)
                                scheduler->rdpTaskTail = 0;
                        }
                    }
                    break;
                case 5:
                case 1:
                default:
                    break;
            }
        }
    }
    if (avail != availRCP)
        avail = __scScheduleCore(scheduler, sp, dp, avail);
    return avail;
}
