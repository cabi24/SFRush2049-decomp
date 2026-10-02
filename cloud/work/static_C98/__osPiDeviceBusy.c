/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
/* Actual SDK SI status check, retaining this cartridge's Pi-labelled symbol.
   Reconstructed from ultralib src/io/si.c and original nine instructions. */
s32 __osPiDeviceBusy(void)
{
    register u32 status = *(volatile u32 *)0xA4800018;
    if (status & 3) return 1;
    else return 0;
}
