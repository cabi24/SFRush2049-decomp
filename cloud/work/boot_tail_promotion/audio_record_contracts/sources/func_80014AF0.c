/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted source contract; original individually locked source is unchanged. */
#include "boot_tail_audio_record.h"
void func_80014AF0(int index)
{
    if (D_80038294[index].active) {
        D_80038294[index].active = 0;
        D_80038294[index].release_pending = 1;
    }
}
