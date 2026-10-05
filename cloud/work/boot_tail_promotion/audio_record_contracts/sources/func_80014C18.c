/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted source contract; original individually locked source is unchanged. */
#include "boot_tail_audio_record.h"


unsigned int func_80014C18(int index)
{
    return D_80038294[index].position;
}
