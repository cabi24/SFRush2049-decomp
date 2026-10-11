/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001C390 - lib_1cf90 (IDO 7.1). Clear the mode byte of every active sample buffer:
 * for i < D_8004FA18 (u8 buffer count), D_8004FA50[i].mode = 0 (SampleBuffer, 24 bytes,
 * include/boot_tail_sample_contract.h, the TU's own view used by func_8001C3CC).
 * w18c: natural indexed loop; uopt strength-reduces it to retail's pointer loop.
 * Prior: D10 BT03-high (pointer loop, 5.3 12 words; 7.1 true=110). No shaping devices. */
#include "boot_tail_sample_contract.h"
extern unsigned char D_8004FA18;
extern SampleBuffer D_8004FA50[];

void func_8001C390(void)
{
    int i;
    for (i = 0; i < D_8004FA18; i++) {
        D_8004FA50[i].mode = 0;
    }
}
