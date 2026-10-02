/* GENERATED ROM-aligned TU — segment 0x1050 (rom/lib_1050_sdk)
 * layout map af9176b4b4f60ff529afaec19f5236cce01f4a00e4b6a10f392108db260a7b70; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */

#include "rom_tu.h"
#include "static_scheduler_context.h"


#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/osCreateScheduler.s")
/* PROMOTED 2026-10-02 — osScAddClient
 * Source:   cloud/matches/osScAddClient.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm
 * Evidence: lock:cloud/matches/osScAddClient.c:osScAddClient (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osScAddClient(OSSched *scheduler, OSScClient *client, OSMesgQueue *queue) {
    u32 mask;
    mask = osSetGlobalIntMask(1);
    client->msgQueue = queue;
    client->next = scheduler->clientList;
    scheduler->clientList = client;
    osSetGlobalIntMask(mask);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scMain.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scSchedule.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scHandleRetrace.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scHandleRSP.s")
/* PROMOTED 2026-10-02 — __scHandleRDP
 * Source:   cloud/matches/__scHandleRDP.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm
 * Evidence: lock:cloud/matches/__scHandleRDP.c:__scHandleRDP (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __scHandleRDP(OSSched *scheduler)
{
    OSScTask *task, *sp = 0, *dp = 0;
    s32 state;
    if (!scheduler->curRDPTask) { }
    if (scheduler->curRDPTask->type != 1) { }
    task = scheduler->curRDPTask;
    scheduler->curRDPTask = 0;
    task->state &= ~1;
    __scExecTask(scheduler, task);
    state = ((scheduler->curRSPTask == 0) << 1) | (scheduler->curRDPTask == 0);
    if (__scScheduleCore(scheduler, &sp, &dp, state) != state)
        __scExec(scheduler, sp, dp);
}

/* PROMOTED 2026-10-02 — __scTaskReady
 * Source:   cloud/matches/__scTaskReady.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm
 * Evidence: lock:cloud/matches/__scTaskReady.c:__scTaskReady (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
OSScTask *__scTaskReady(OSSched *scheduler, OSScTask *task) {
    void *current;
    void *next;
    if (task != 0) {
        if ((current = osViGetCurrentFramebuffer()) !=
            (next = osViGetFramebuffer())) return 0;
        if (__osScPendingSwap != 0 && scheduler->retraceCount - __osScSwapCount < 2U)
            return 0;
        return task;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scExecTask.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scAppendList.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scExec.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scHandlePreNMI.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scScheduleCore.s")


