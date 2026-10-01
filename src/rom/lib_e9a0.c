/* GENERATED ROM-aligned TU — segment 0xe9a0 (rom/lib_e9a0)
 * layout map 3babb96e2ff8b36483524b7acb5405673e5c98d295075fdfac8ed2bdda2fb775; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

typedef struct {
    u32      errStatus;         /* error status */
    void    *dramAddr;          /* RDRAM buffer address (DMA) */
    void    *C2Addr;            /* C2 buffer address */
    u32      sectorSize;        /* size of transfering sector */
    u32      C1ErrNum;          /* total # of C1 errors */
    u32      C1ErrSector[4];    /* error sectors */
} __OSBlockInfo;

typedef struct {
    u32             cmdType;        /* for disk only */
    u16             transferMode;   /* Block, Track, or sector?   */
    u16             blockNum;       /* which block is transfering */
    s32             sectorNum;      /* which sector is transfering */
    u32             devAddr;        /* Device buffer address */
    u32             bmCtlShadow;    /* asic bm_ctl(510) register shadow ram */
    u32             seqCtlShadow;   /* asic seq_ctl(518) register shadow ram */
    __OSBlockInfo   block[2];       /* bolck transfer info */
} __OSTranxInfo;


typedef struct __OSDiskPiHandle_s {
    struct __OSDiskPiHandle_s *next;          /* point to next handle on the table */
    u8                   type;          /* DEVICE_TYPE_BULK for disk */
    u8                   latency;       /* domain latency */
    u8                   pageSize;      /* domain page size */
    u8                   relDuration;   /* domain release duration */
    u8                   pulse;         /* domain pulse width */
    u8                   domain;        /* which domain */
    u32                  baseAddress;   /* Domain address */
    u32                  speed;         /* for roms only */
    /* The following are "private" elements" */
    __OSTranxInfo        transferInfo;  /* for disk only */
} __OSDiskPiHandle;
typedef struct {OSIoMesgHdr hdr;void *dramAddr;u32 devAddr,size;__OSDiskPiHandle *piHandle;} __OSDiskIoMesg;

typedef struct {s32 active;OSThread *thread;OSMesgQueue *cmdQueue,*evtQueue,*acsQueue;s32 (*dma)(s32,u32,void*,u32);s32 (*edma)(OSPiHandle*,s32,u32,void*,u32);} OSDevMgr;
#define DEVICE_TYPE_64DD 2
#define LEO_CMD_TYPE_0 0
#define LEO_CMD_TYPE_1 1
#define LEO_SECTOR_MODE 3
#define LEO_TRACK_MODE 2
#define LEO_ERROR_29 29
#define LEO_ERROR_4 4
#define LEO_ERROR_GOOD 0
#define LEO_BM_CTL 0x05000510
#define LEO_STATUS 0x05000508
#define LEO_BM_CTL_RESET 0x10000000
#define LEO_BM_CTL_CLR_MECHANIC_INTR 0x01000000
#define LEO_STATUS_MECHANIC_INTERRUPT 0x02000000
#define PI_STATUS_REG 0xA4600010
#define PI_CLR_INTR 2
#define OS_IM_PI 0x00100401
#define SR_IBIT4 0x800
#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
#define OS_MESG_TYPE_LOOPBACK 10
#define OS_MESG_TYPE_DMAREAD 11
#define OS_MESG_TYPE_DMAWRITE 12
#define OS_READ 0
#define OS_WRITE 1
#define OS_MESG_TYPE_EDMAREAD 15
#define OS_MESG_TYPE_EDMAWRITE 16
#define IO_WRITE(a,b) (*(volatile u32 *)(a)=(b))
extern void osEPiRawWriteIo(u32),__osPiGetCmdQueue(u32);
extern s32 osEPiRawStartDma(OSPiHandle*,u32,u32),osEPiRawReadIo(OSPiHandle*,u32,u32*);





/* Canonical EPi hardware protocol; existing IO_READ/IO_WRITE macros suffice. */
#define PI_STATUS_IO_BUSY 2
#define PI_STATUS_DMA_BUSY 1
#define PI_DOMAIN1 0
#define K1_TO_PHYS(x) ((u32)(x)&0x1FFFFFFF)
#define PI_STATUS_REG 0xA4600010
#define PI_DRAM_ADDR_REG 0xA4600000
#define PI_CART_ADDR_REG 0xA4600004
#define PI_WR_LEN_REG 0xA460000C
#define PI_RD_LEN_REG 0xA4600008
#define PI_BSD_DOM1_LAT_REG 0xA4600014
#define PI_BSD_DOM1_PWD_REG 0xA4600018
#define PI_BSD_DOM1_PGS_REG 0xA460001C
#define PI_BSD_DOM1_RLS_REG 0xA4600020
#define PI_BSD_DOM2_LAT_REG 0xA4600024
#define PI_BSD_DOM2_PWD_REG 0xA4600028
#define PI_BSD_DOM2_PGS_REG 0xA460002C
#define PI_BSD_DOM2_RLS_REG 0xA4600030
#define WAIT_ON_IOBUSY(stat)                                                                \
    {                                                                                       \
        stat = IO_READ(PI_STATUS_REG);                                                      \
        while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY))                             \
            stat = IO_READ(PI_STATUS_REG);                                                  \
    } (void)0

#define UPDATE_REG(pihandle, reg, var) \
    if (cHandle->var != pihandle->var) \
        IO_WRITE(reg, pihandle->var)


#define EPI_SYNC(pihandle, stat, domain)                             \
                                                                     \
    WAIT_ON_IOBUSY(stat);                                            \
                                                                     \
    domain = pihandle->domain;                                       \
    if (__osPiDevList[domain]->type != pihandle->type)           \
    {                                                                \
        OSPiHandle *cHandle = __osPiDevList[domain];             \
        if (domain == PI_DOMAIN1)                                    \
        {                                                            \
            UPDATE_REG(pihandle, PI_BSD_DOM1_LAT_REG, latency);      \
            UPDATE_REG(pihandle, PI_BSD_DOM1_PGS_REG, pageSize);     \
            UPDATE_REG(pihandle, PI_BSD_DOM1_RLS_REG, relDuration);  \
            UPDATE_REG(pihandle, PI_BSD_DOM1_PWD_REG, pulse);        \
        }                                                            \
        else                                                         \
        {                                                            \
            UPDATE_REG(pihandle, PI_BSD_DOM2_LAT_REG, latency);      \
            UPDATE_REG(pihandle, PI_BSD_DOM2_PGS_REG, pageSize);     \
            UPDATE_REG(pihandle, PI_BSD_DOM2_RLS_REG, relDuration);  \
            UPDATE_REG(pihandle, PI_BSD_DOM2_PWD_REG, pulse);        \
        }                                                            \
        cHandle->type = pihandle->type;                              \
        cHandle->latency = pihandle->latency;                        \
        cHandle->pageSize = pihandle->pageSize;                      \
        cHandle->relDuration = pihandle->relDuration;                \
        cHandle->pulse = pihandle->pulse;                            \
    }(void)0

/* PROMOTED 2026-10-01 — osPiSetDeviceTiming
 * Source:   cloud/work/static_C12/osPiSetDeviceTiming.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C12/osPiSetDeviceTiming.c:osPiSetDeviceTiming (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPiSetDeviceTiming(OSPiHandle* pihandle, s32 direction, u32 devAddr, void* dramAddr, u32 size) {
    u32 stat;
    u32 domain;


    EPI_SYNC(pihandle, stat, domain);
    IO_WRITE(PI_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));
    IO_WRITE(PI_CART_ADDR_REG, K1_TO_PHYS(pihandle->baseAddress | devAddr));

    switch (direction) {
        case OS_READ:
            IO_WRITE(PI_WR_LEN_REG, size - 1);
            break;
        case OS_WRITE:
            IO_WRITE(PI_RD_LEN_REG, size - 1);
            break;
        default:
            return -1;
    }
    return 0;
}

/* PROMOTED 2026-10-01 — osSpTaskLoad_full
 * Source:   cloud/work/static_C12/pi_manager/osSpTaskLoad_full.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C12/pi_manager/osSpTaskLoad_full.c:osSpTaskLoad_full (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osSpTaskLoad_full(void* args) {
    __OSDiskIoMesg* mb;
    OSMesg em;
    OSMesg dummy;
    s32 ret;
    OSDevMgr* dm;
    s32 messageSend = 0;

    dm = (OSDevMgr*)args;
    mb = NULL;
    ret = 0;

    while (TRUE) {
        osRecvMesg(dm->cmdQueue, (OSMesg)&mb, OS_MESG_BLOCK);

        if (mb->piHandle != NULL && mb->piHandle->type == DEVICE_TYPE_64DD &&
            (mb->piHandle->transferInfo.cmdType == LEO_CMD_TYPE_0 ||
             mb->piHandle->transferInfo.cmdType == LEO_CMD_TYPE_1)) {
            __OSBlockInfo* blockInfo;
            __OSTranxInfo* info;
            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];
            info->sectorNum = -1;

            if (info->transferMode != LEO_SECTOR_MODE) {
                blockInfo->dramAddr = (void*)((u32)blockInfo->dramAddr - blockInfo->sectorSize);
            }

            if (info->transferMode == LEO_TRACK_MODE && mb->piHandle->transferInfo.cmdType == LEO_CMD_TYPE_0) {
                messageSend = 1;
            } else {
                messageSend = 0;
            }

            osRecvMesg(dm->acsQueue, &dummy, OS_MESG_BLOCK);
            osEPiRawWriteIo(OS_IM_PI);
            osEPiRawStartDma(mb->piHandle, LEO_BM_CTL, (info->bmCtlShadow | 0x80000000));

        readblock1:
            osRecvMesg(dm->evtQueue, &em, OS_MESG_BLOCK);
            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];

            if (blockInfo->errStatus == LEO_ERROR_29) {
                u32 stat;
                osEPiRawStartDma(mb->piHandle, LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_RESET);
                osEPiRawStartDma(mb->piHandle, LEO_BM_CTL, info->bmCtlShadow);
                osEPiRawReadIo(mb->piHandle, LEO_STATUS, &stat);

                if (stat & LEO_STATUS_MECHANIC_INTERRUPT) {
                    osEPiRawStartDma(mb->piHandle, LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_CLR_MECHANIC_INTR);
                }

                blockInfo->errStatus = LEO_ERROR_4;
                IO_WRITE(PI_STATUS_REG, PI_CLR_INTR);
                __osPiGetCmdQueue(OS_IM_PI | SR_IBIT4);
            }

            osJamMesg(mb->hdr.retQueue, mb, OS_MESG_NOBLOCK);

            if (messageSend == 1 && mb->piHandle->transferInfo.block[0].errStatus == LEO_ERROR_GOOD) {
                messageSend = 0;
                goto readblock1;
            }

            osJamMesg(dm->acsQueue, NULL, OS_MESG_NOBLOCK);
            if (mb->piHandle->transferInfo.blockNum == 1) {
                osYieldThread();
            }
        } else {
            switch (mb->hdr.type) {
                case OS_MESG_TYPE_DMAREAD:
                    osRecvMesg(dm->acsQueue, &dummy, OS_MESG_BLOCK);
                    ret = dm->dma(OS_READ, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case OS_MESG_TYPE_DMAWRITE:
                    osRecvMesg(dm->acsQueue, &dummy, OS_MESG_BLOCK);
                    ret = dm->dma(OS_WRITE, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case OS_MESG_TYPE_EDMAREAD:
                    osRecvMesg(dm->acsQueue, &dummy, OS_MESG_BLOCK);
                    ret = dm->edma(mb->piHandle, OS_READ, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case OS_MESG_TYPE_EDMAWRITE:
                    osRecvMesg(dm->acsQueue, &dummy, OS_MESG_BLOCK);
                    ret = dm->edma(mb->piHandle, OS_WRITE, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case OS_MESG_TYPE_LOOPBACK:
                    osJamMesg(mb->hdr.retQueue, mb, OS_MESG_NOBLOCK);
                    ret = -1;
                    break;
                default:
                    ret = -1;
                    break;
            }

            if (ret == 0) {
                osRecvMesg(dm->evtQueue, &em, OS_MESG_BLOCK);
                osJamMesg(mb->hdr.retQueue, mb, OS_MESG_NOBLOCK);
                osJamMesg(dm->acsQueue, NULL, OS_MESG_NOBLOCK);
            }
        }
    }
}

/* PROMOTED 2026-07-15 — __osInsertTimer
 * Source:   src/rom_auto/__osInsertTimer.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/rom_auto/__osInsertTimer.c:__osInsertTimer (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osInsertTimer(void) {
    if (__osPiMgrState.flag == 0) {
        return 0;
    }
    return __osPiMgrState.unk8;
}
