/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* __osSiDeviceBusy (static, boot 0x8000FD70, 48 B, segment 0x10970 -> src/rom/lib_10970.c, unconverted)
 * Conveyor inventory target_id: __osPiDeviceBusy (stale pre-S12 label; same address and object;
 * rom_symbol_aliases.ld keeps __osPiDeviceBusy = __osSiDeviceBusy for locked callers).
 * Origin: reference/repos/ultralib (decompals e24c836) src/io/si.c (all versions). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define TRUE 1
#define FALSE 0
#define IO_READ(addr) (*(volatile u32 *)(addr))
#define SI_STATUS_REG 0xA4800018
#define SI_STATUS_DMA_BUSY (1 << 0)
#define SI_STATUS_RD_BUSY (1 << 1)

int __osSiDeviceBusy(void) {
    register u32 stat = IO_READ(SI_STATUS_REG);

    if (stat & (SI_STATUS_DMA_BUSY | SI_STATUS_RD_BUSY)) {
        return TRUE;
    } else {
        return FALSE;
    }
}
