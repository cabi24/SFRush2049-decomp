/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8001C508.c: file-local type names SampleBuffer suffixed _8001C508 so several bodies share one ROM TU; no other change. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct SampleBuffer_8001C508 {
    u8 mode;
    u8 unknown01[7];
    short *buffer;
    u32 samples;
    u8 unknown10[8];
} SampleBuffer_8001C508;
extern u8 D_8004FA18;
extern SampleBuffer_8001C508 D_8004FA50[];
extern void func_80014C60(short *, u32);

void func_8001C508(void)
{
    int i;
    for (i = 0; i < D_8004FA18; i++) {
        if (D_8004FA50[i].mode == 1) {
            func_80014C60(D_8004FA50[i].buffer, D_8004FA50[i].samples);
        }
    }
}
