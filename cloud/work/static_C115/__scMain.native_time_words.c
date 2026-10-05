/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
#include "rom_tu.h"
extern u32 __osScFrameTimeHi;
extern u32 __osScRetraceTimeHi;
extern u32 __osScFrameTimeResult;
extern void __scHandleRetrace(OSSched *), __scSchedule(OSSched *);
extern void __scHandleRSP(OSSched *), __scHandleRDP(OSSched *);
/* Complete original scheduler thread with all five actual event cases.
 * Existing native Hi/Lo word symbols describe adjacent64-bit timestamps;
 * these are extern references only. No logging-only SDK count is defined. */
void __scMain(void *arg)
{
    OSMesg message;
    OSSched *scheduler = (OSSched *)arg;
    OSScClient *client;
    while (1) {
        osRecvMesg(&scheduler->cmdQueue, &message, OS_MESG_BLOCK);
        switch ((s32)message) {
            case 666:
                *(u64 *)&__osScFrameTimeHi = osGetTime();
                __scHandleRetrace(scheduler);
                __scSchedule(scheduler);
                break;
            case 667:
                __scHandleRSP(scheduler);
                break;
            case 668:
                __osScFrameTimeResult = osGetTime() - *(u64 *)&__osScRetraceTimeHi;
                __scHandleRDP(scheduler);
                break;
            case 669:
                __scSchedule(scheduler);
                break;
            case 670:
                for (client = scheduler->clientList; client != 0; client = client->next)
                    osSendMesg(client->msgQueue, (OSMesg)&scheduler->priority, OS_MESG_BLOCK);
                break;
        }
    }
}
