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
 * Get RDP performance counters
 * (0x800099B0 - osDpGetCounters)
 *
 * Reads the four RDP performance counters (clock, buffer busy, pipeline
 * busy, TMEM load) into a 4-word array, in register order.
 *
 * MATCHING: 0x800099B0 (asm/us/A5B0.s), IDO -O2
 * Source: ultralib src/io/dpctr.c:osDpGetCounters @ e24c8367 (score 0 via conveyor)
 *
 * @param array Destination for the four counter values
 */
void osDpGetCounters(u32 *array) {
    *array++ = DPC_CLOCK_REG;
    *array++ = DPC_BUFBUSY_REG;
    *array++ = DPC_PIPEBUSY_REG;
    *array++ = DPC_TMEM_REG;
}
