/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted source contract; original individually locked source is unchanged. */
#include "boot_tail_audio_record.h"
#pragma pack(1)
typedef struct SampleInfo_800146B4 {
    unsigned int frequency;
    void *data;
    unsigned int offset;
    unsigned int length;
    unsigned int loopStart;
    unsigned int loopLength;
    unsigned char format;
} SampleInfo_800146B4;
#pragma pack()
extern void func_80011C1C(unsigned short, unsigned short);
void func_800146B4(unsigned int index, SampleInfo_800146B4 *sample, unsigned char reset)
{
    func_80011C1C(index, 0);
    if (reset) {
        D_80038294[index].initial_count = 0;
        D_80038294[index].value22 = 0;
        D_80038294[index].value24 = 1.0f;
        D_80038294[index].release_count = 20;
    }
    D_80038294[index].data = sample->data;
    D_80038294[index].sample_position = sample->offset;
    D_80038294[index].current = D_80038294[index].position = sample->offset;
    D_80038294[index].length = sample->length;
    D_80038294[index].loop_start = sample->loopStart;
    D_80038294[index].loop_length = sample->loopLength;
    D_80038294[index].format = sample->format;
    if (D_80038294[index].loop_length != 0) {
        D_80038294[index].tail = D_80038294[index].length -
            D_80038294[index].loop_length - D_80038294[index].loop_start;
        if (D_80038294[index].tail < 10) D_80038294[index].tail = 0;
    } else {
        D_80038294[index].tail = 0;
    }
    if (((unsigned long)sample->data & 0xF0000000UL) != 0x80000000UL) {
        D_80038294[index].flags |= 1;
    }
}
