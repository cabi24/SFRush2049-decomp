/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted source contract; original individually locked source is unchanged. */
#include "boot_tail_audio_record.h"
extern unsigned short D_8003829C;
void func_80013C84(void)
{
    int i;
    for (i = 0; i < D_8003829C; i++) {
        if (D_80038294[i].active != 0) {
            D_80038294[i].position = D_80038294[i].current;
            D_80038294[i].current = (unsigned int) D_80038294[i].sample_position;
        }
    }
}
