/* Common includes for ROM-aligned TUs (004). Passthrough slots need nothing;
 * promoted bodies add what they use here (keep it additive and collision-free
 * — one definition per hardware register across all ROM TUs). */
#include "types.h"

/* N64 MMIO registers referenced by promoted libultra bodies. IDO compiles
 * these #define'd KSEG1 addresses to literal immediates (no relocation) —
 * the same fact behind 003's KSEG1 de-symbolization. */
#define SP_STATUS_REG    (*(vu32 *)0xA4040010)
#define SP_PC_REG        (*(vu32 *)0xA4080000)
#define SP_STATUS_HALT     0x0001
#define SP_STATUS_DMA_BUSY 0x0004
#define SP_STATUS_IO_FULL  0x0010
#define DPC_CLOCK_REG    (*(vu32 *)0xA4100010)
#define DPC_BUFBUSY_REG  (*(vu32 *)0xA4100014)
#define DPC_PIPEBUSY_REG (*(vu32 *)0xA4100018)
#define DPC_TMEM_REG     (*(vu32 *)0xA410001C)
#define AI_STATUS_REG    (*(vu32 *)0xA450000C)
#define AI_STATUS_FIFO_FULL 0x80000000
