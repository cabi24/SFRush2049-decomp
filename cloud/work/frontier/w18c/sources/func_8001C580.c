/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001C580 - lib_1cf90 (IDO 7.1). Start a streamed sample on a free voice: with the audio
 * system up (D_8002C630) and inside func_80014594/func_800145DC (lock/unlock), allocate a
 * voice for REQUEST (func_8001F898, -1 = none); set up its SampleBuffer slot (callback, mode 1,
 * context, position 0, sample count, buffer); describe the sample to func_800146B4 (frequency
 * with flag 0x400000, whole buffer as one loop); set the pitch to frequency / output rate
 * D_8004F800 in 4.12 fixed point (func_80014A0C) and the four 8-bit levels as 8.16 values
 * (func_80014A74); start it (func_800149BC). Returns the voice index or -1.
 * Caller: func_8002574C (lib_25bb0, locked) with func_80024FD4 as CALLBACK.
 * w18c: the slot stores are written callback, mode, context first (then position, ...):
 * that order gives retail's temporary numbering (D10's mode-first order: true=50).
 * Types: SampleBuffer/SampleCallback from boot_tail_sample_contract.h (the TU's include);
 * SampleInfo_800146B4 copied verbatim from func_800146B4's block (lib_11640); prototypes as
 * the locked definitions (func_8001F898, func_800146B4, func_80014A0C, func_800149BC) and the
 * TU's func_80014A74 declaration. `(unsigned int)(ratio * 4096.0f)` is the unsigned
 * float conversion retail performs (the u16 narrowing is the prototype's). No shaping devices.
 * Prior: D10 BT03-high-runtime-followon (5.3, 33 words; 7.1 true=50 reloc_blind=9). */
#include "boot_tail_sample_contract.h"
#pragma pack(1)
typedef struct SampleInfo_800146B4 { unsigned int frequency; void *data; unsigned int offset; unsigned int length; unsigned int loopStart; unsigned int loopLength; unsigned char format; } SampleInfo_800146B4;
#pragma pack()
extern unsigned char D_8002C630;
extern int D_8004F800;
extern SampleBuffer D_8004FA50[];
extern void func_80014594(void);
extern void func_800145DC(void);
extern int func_8001F898(unsigned char);
extern void func_800146B4(unsigned int, SampleInfo_800146B4 *, unsigned char);
extern void func_80014A0C(int, unsigned short);
extern void func_80014A74(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void func_800149BC(unsigned int);

int func_8001C580(unsigned char request, short *buffer, unsigned int samples, unsigned int frequency,
                  unsigned char volume, unsigned char pan, unsigned char span, unsigned char aux,
                  SampleCallback callback, unsigned int context)
{
    int index;
    SampleInfo_800146B4 info;
    float ratio;
    if (D_8002C630) {
        func_80014594();
        index = func_8001F898(request);
        if (index != -1) {
            D_8004FA50[index].callback = callback;
            D_8004FA50[index].mode = 1;
            D_8004FA50[index].context = context;
            D_8004FA50[index].position = 0;
            info.frequency = frequency | 0x400000;
            info.offset = 0;
            info.loopStart = 0;
            info.format = 0;
            D_8004FA50[index].samples = samples;
            info.length = samples;
            info.loopLength = samples;
            D_8004FA50[index].buffer = buffer;
            info.data = buffer;
            func_800146B4(index, &info, 1);
            ratio = (float)frequency / (float)D_8004F800;
            func_80014A0C(index, (unsigned int)(ratio * 4096.0f));
            func_80014A74(index, volume << 16, pan << 16, span << 16, aux << 16);
            func_800149BC(index);
        }
        func_800145DC();
        return index;
    }
    return -1;
}
