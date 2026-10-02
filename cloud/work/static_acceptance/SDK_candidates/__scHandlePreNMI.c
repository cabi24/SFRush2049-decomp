/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
#include "../src/rom/rom_tu.h"
/* Actual graphics RSP yield/preemption; existing misleading physical name.
 * Original SDK __scYield has a genuine logging-only else; no logging payload
 * remains in original native code, so retain the actual empty source block. */
void __scHandlePreNMI(OSSched *scheduler)
{
    if (scheduler->curRSPTask->type == 1) {
        scheduler->curRSPTask->state |= 0x10;
        osDpWait();
    } else {
    }
}
