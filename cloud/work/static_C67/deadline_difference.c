/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
#include "rom_tu.h"
s32 viDeadlinePassed(void) { s32 difference; difference = gViAccumTime - gViTickCounter; return difference < 1; }
