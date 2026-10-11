/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* __osEPiRawWriteIo (static, boot 0x8000FE00, 352 B, segment 0x10a00 -> src/rom/lib_10a00.c, unconverted)
 * Conveyor inventory target_id: osEPiRawStartDma (stale pre-S12 label; same address and object).
 * Origin: reference/repos/ultralib (decompals e24c836) src/io/epirawwrite.c with
 *         PRinternal/piint.h EPI_SYNC for BUILD_VERSION >= VERSION_J (handle *type* compare,
 *         field copy into the current handle).
 * Project names: ultralib __osCurrentHandle[] is this project's __osPiDevList (0x8002C3A0).
 * IO_READ/IO_WRITE are rom_tu.h's (no PHYS_TO_K1); ultralib's PHYS_TO_K1 is spelled out where the
 * address is not a KSEG1 constant (the device address). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct OSPiHandle_s {
    struct OSPiHandle_s *next;
    u8 type;
    u8 latency;
    u8 pageSize;
    u8 relDuration;
    u8 pulse;
    u8 domain;
    u32 baseAddress;
    u32 speed;
} OSPiHandle;

#define IO_READ(addr) (*(volatile u32 *)(addr))
#define IO_WRITE(addr,value) (*(volatile u32 *)(addr)=(value))
#define PHYS_TO_K1(x) ((u32)(x)|0xA0000000)
#define PI_DOMAIN1 0
#define PI_STATUS_REG 0xA4600010
#define PI_STATUS_DMA_BUSY 1
#define PI_STATUS_IO_BUSY 2
#define PI_BSD_DOM1_LAT_REG 0xA4600014
#define PI_BSD_DOM1_PWD_REG 0xA4600018
#define PI_BSD_DOM1_PGS_REG 0xA460001C
#define PI_BSD_DOM1_RLS_REG 0xA4600020
#define PI_BSD_DOM2_LAT_REG 0xA4600024
#define PI_BSD_DOM2_PWD_REG 0xA4600028
#define PI_BSD_DOM2_PGS_REG 0xA460002C
#define PI_BSD_DOM2_RLS_REG 0xA4600030
#define WAIT_ON_IOBUSY(stat) { stat = IO_READ(PI_STATUS_REG); while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY)) stat = IO_READ(PI_STATUS_REG); } (void)0
#define UPDATE_REG(pihandle, reg, var) if (cHandle->var != pihandle->var) IO_WRITE(reg, pihandle->var)
#define EPI_SYNC(pihandle, stat, domain) WAIT_ON_IOBUSY(stat); domain = pihandle->domain; if (__osPiDevList[domain]->type != pihandle->type) { OSPiHandle *cHandle = __osPiDevList[domain]; if (domain == PI_DOMAIN1) { UPDATE_REG(pihandle, PI_BSD_DOM1_LAT_REG, latency); UPDATE_REG(pihandle, PI_BSD_DOM1_PGS_REG, pageSize); UPDATE_REG(pihandle, PI_BSD_DOM1_RLS_REG, relDuration); UPDATE_REG(pihandle, PI_BSD_DOM1_PWD_REG, pulse); } else { UPDATE_REG(pihandle, PI_BSD_DOM2_LAT_REG, latency); UPDATE_REG(pihandle, PI_BSD_DOM2_PGS_REG, pageSize); UPDATE_REG(pihandle, PI_BSD_DOM2_RLS_REG, relDuration); UPDATE_REG(pihandle, PI_BSD_DOM2_PWD_REG, pulse); } cHandle->type = pihandle->type; cHandle->latency = pihandle->latency; cHandle->pageSize = pihandle->pageSize; cHandle->relDuration = pihandle->relDuration; cHandle->pulse = pihandle->pulse; } (void)0

extern OSPiHandle *__osPiDevList[2];

s32 __osEPiRawWriteIo(OSPiHandle* pihandle, u32 devAddr, u32 data) {
    register u32 stat;
    register u32 domain;

    EPI_SYNC(pihandle, stat, domain);
    IO_WRITE(PHYS_TO_K1(pihandle->baseAddress | devAddr), data);

    return 0;
}
