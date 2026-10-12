/* Historical symbol names kept for locked C (hand-maintained; not generated).
 *
 * The S12 boot-tail census (cloud/work/s12_boot_tail_census, README "symbol_addrs
 * name conflicts") identified these boot-segment functions as exact masked
 * matches of decompals ultralib e24c836 under IDO 5.3, and symbol_addrs.us.txt
 * now labels them by their ultralib names. Locked, ROM-verified C bodies and the
 * context-pinned src/rom/rom_tu.h still call them by the historical labels, so
 * each historical name is an exact alias of the renamed function. Remove an
 * alias only after every caller has been renamed and re-locked.
 */
__osTLBLookup = __osProbeTLB;               /* 0x8000FCB0 */
__osPiDeviceBusy = __osSiDeviceBusy;        /* 0x8000FD70 */
osEPiRawWriteIo = __osResetGlobalIntMask;   /* 0x8000FDA0 */
osEPiRawStartDma = __osEPiRawWriteIo;       /* 0x8000FE00 */
osEPiRawReadIo = __osEPiRawReadIo;          /* 0x8000FF60 */
__osPiGetCmdQueue = __osSetGlobalIntMask;   /* 0x800100C0 */
__osPfsDataChecksum = __osContDataCrc;      /* 0x8000FAC0 */

/* w24s: the scheduler timestamps are one OSTime each (__osScRetraceTime, ...);
 * asm/us/1000.s (entry .bss clear, pinned by sdk_initialize's startup proof)
 * still names the start of .bss by the old high-half label. */
__osScRetraceTimeHi = __osScRetraceTime;    /* 0x8002E8E0 */
