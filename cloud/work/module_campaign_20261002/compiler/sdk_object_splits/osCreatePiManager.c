/* Native flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
#include "pi_manager_declarations.h"
void osCreatePiManager(OSPri pri, OSMesgQueue* cmdQ, OSMesg* cmdBuf, s32 cmdMsgCnt) {
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;


    if ((*(OSDevMgr *)&__osPiMgrState).active) {
        return;
    }
    osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
    osCreateMesgQueue(&__osPiDmaQueue, (OSMesg*)__osPiDmaMesg, 1);

    if (!__osPiInitialized) {
        osPiInit();
    }

    osSetEventMesgAlt(OS_EVENT_PI, &__osPiDmaQueue, (OSMesg)0x22222222);
    oldPri = -1;
    myPri = dll_get_priority(NULL);

    if (myPri < pri) {
        oldPri = myPri;
        osCreateViManager(NULL, pri);
    }

    savedMask = __osDisableInt();
    (*(OSDevMgr *)&__osPiMgrState).active = 1;
    (*(OSDevMgr *)&__osPiMgrState).thread = &(*(OSThread *)&gViModeMessage);
    (*(OSDevMgr *)&__osPiMgrState).cmdQueue = cmdQ;
    (*(OSDevMgr *)&__osPiMgrState).evtQueue = &__osPiDmaQueue;
    (*(OSDevMgr *)&__osPiMgrState).acsQueue = &__osPiMesgQueue;
    (*(OSDevMgr *)&__osPiMgrState).dma = osPiStartDma;
    (*(OSDevMgr *)&__osPiMgrState).edma = osPiSetDeviceTiming;
    osCreateThread(&(*(OSThread *)&gViModeMessage), 0, osSpTaskLoad_full, &(*(OSDevMgr *)&__osPiMgrState), (void *)&__osPiDmaQueue, pri);
    osStartThread(&(*(OSThread *)&gViModeMessage));

    __osRestoreInt(savedMask);

    if (oldPri != -1) {
        osCreateViManager(NULL, oldPri);
    }
}
