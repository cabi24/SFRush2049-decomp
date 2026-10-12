/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern void __osSpSetStatus(u32);
void osSpTaskYield(void) { __osSpSetStatus(0x400); }
