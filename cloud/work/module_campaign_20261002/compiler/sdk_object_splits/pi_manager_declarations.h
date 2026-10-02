#ifndef CAMPAIGN_PI_MANAGER_DECLARATIONS_H
#define CAMPAIGN_PI_MANAGER_DECLARATIONS_H
/* Existing storage, actual SDK OSDevMgr carrier; no object definition. */
typedef struct {s32 active; OSThread *thread; OSMesgQueue *cmdQueue, *evtQueue, *acsQueue; s32 (*dma)(s32,u32,void *,u32); s32 (*edma)(OSPiHandle *,s32,u32,void *,u32);} OSDevMgr;
extern OSMesgQueue __osPiDmaQueue;
extern OSMesg __osPiDmaMesg[1];
extern u8 gViModeMessage[];
extern void osSpTaskLoad_full(void *);
extern OSPri dll_get_priority(OSThread *);
#define OS_EVENT_PI 8
#endif
