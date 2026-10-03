/* flags: -g1 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern s32 __osAiDeviceBusy(void);
/* Genuine SDK file-local hardware workaround state. Original state byte is
   at8002C3C0; this private owner hypothesis is not a registered storage claim. */
static u8 __osAiNeedsAlign = 0;
s32 osAiSetNextBuffer(void *buffer,u32 size)
{
    char *adjusted;
    if (__osAiDeviceBusy()) return -1;
    adjusted = buffer;
    if (__osAiNeedsAlign) adjusted = (char *)buffer - 0x2000;
    if ((((u32)buffer + size) & 0x1FFF) == 0) __osAiNeedsAlign = 1;
    else __osAiNeedsAlign = 0;
    *(volatile u32 *)0xA4500000 = osVirtualToPhysical(adjusted);
    *(volatile u32 *)0xA4500004 = size;
    return 0;
}
