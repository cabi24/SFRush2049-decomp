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

