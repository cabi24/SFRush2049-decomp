/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted source contract; original individually locked source is unchanged. */
#include "boot_tail_audio_record.h"

extern void func_8001E0E0(unsigned short *, unsigned short *, unsigned int, unsigned int, unsigned int, unsigned short *, unsigned int, unsigned short *);

void func_80014A74(int index, unsigned int volume, unsigned int pan, unsigned int span, unsigned int aux)
{
    AudioSlot *slot;
    slot = &D_80038294[index];
    func_8001E0E0(&slot->value42, &slot->value40, volume, pan, span,
                  &slot->value44, aux, &slot->value46);
}
