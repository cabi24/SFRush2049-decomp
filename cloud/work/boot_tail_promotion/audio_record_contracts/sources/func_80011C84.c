/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted source contract; original individually locked source is unchanged. */
#include "boot_tail_audio_record.h"
/* Reset and activate one 104-byte audio record. Unknown bytes retain observed record stride. */
void func_80011C84(unsigned short index)
{
    AudioState *audio = &D_80038294[index];
    func_80011A10(audio);
    audio->active = 1;
}
