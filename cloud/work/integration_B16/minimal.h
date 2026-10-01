#define PFS_CHECK_ID() if(osPfsReadWriteFile_pages(pfs)==PFS_ERR_NEW_PACK) return PFS_ERR_NEW_PACK
#define ROUND_UP_DIVIDE(n,d) (((n)+(d)-1)/(d))
#define PFS_DATA_FULL 7
#define PFS_DIR_FULL 8
#define DIR_STATUS_OCCUPIED 2
#define DIR_STATUS_EMPTY 0
extern s32 __osPfsDeclearPage(OSPfs*,__OSInode*,int,int*,u8,int*,int*);
#define PI_STATUS_IO_BUSY 2
#define PI_STATUS_DMA_BUSY 1
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


extern OSPiHandle *__osPiDevList[2];
