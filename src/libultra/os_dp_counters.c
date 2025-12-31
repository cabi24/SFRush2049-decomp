/**
 * @file os_dp_counters.c
 * @brief RDP performance counter functions
 *
 * Decompiled from asm/us/A5B0.s
 */

#include "types.h"

/* RDP Counter registers (directly mapped) */
#define DPC_CLOCK_REG    (*(vu32 *)0xA4100010)
#define DPC_BUFBUSY_REG  (*(vu32 *)0xA4100014)
#define DPC_PIPEBUSY_REG (*(vu32 *)0xA4100018)
#define DPC_TMEM_REG     (*(vu32 *)0xA410001C)

/**
 * RDP counter structure
 */
typedef struct {
    u32 clock;      /* 0x00: Clock cycles */
    u32 bufbusy;    /* 0x04: Buffer busy cycles */
    u32 pipebusy;   /* 0x08: Pipeline busy cycles */
    u32 tmem;       /* 0x0C: TMEM load cycles */
} OSRdpCounters;

/**
 * Get RDP performance counters
 * (0x800099B0 - osDpGetCounters)
 *
 * Reads all RDP performance counters into a structure.
 *
 * @param counters Pointer to structure to receive counter values
 */
void osDpGetCounters(OSRdpCounters *counters) {
    counters->clock = DPC_CLOCK_REG;
    counters->bufbusy = DPC_BUFBUSY_REG;
    counters->pipebusy = DPC_PIPEBUSY_REG;
    counters->tmem = DPC_TMEM_REG;
}
