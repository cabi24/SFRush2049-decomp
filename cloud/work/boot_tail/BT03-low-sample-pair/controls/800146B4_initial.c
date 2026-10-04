/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct SampleInfo {
    u32 frequency;
    void *data;
    u32 offset;
    u32 length;
    u32 loopStart;
    u32 loopLength;
    u8 format;
} SampleInfo;
#pragma pack()
typedef struct AudioState {
    u8 active, enabled;
    u16 flags, gain;
    u8 unknown06[2];
    double position08;
    u32 position10, position14;
    u8 unknown18[8];
    u16 ramp20, ramp22;
    float scale24;
    u16 steps28;
    u8 unknown2A[2];
    void *data2C;
    u32 length30, loopStart34, loopLength38, tail3C;
    u8 unknown40[29];
    u8 format5D;
    u8 unknown5E[10];
} AudioState;
extern AudioState *D_80038294;
extern void func_80011C1C(u16, u16);
void func_800146B4(u32 index, SampleInfo *sample, u8 reset)
{
    func_80011C1C(index, 0);
    if (reset) {
        D_80038294[index].ramp20 = 0;
        D_80038294[index].ramp22 = 0;
        D_80038294[index].scale24 = 1.0f;
        D_80038294[index].steps28 = 20;
    }
    D_80038294[index].data2C = sample->data;
    D_80038294[index].position08 = sample->offset;
    D_80038294[index].position14 = sample->offset;
    D_80038294[index].position10 = sample->offset;
    D_80038294[index].length30 = sample->length;
    D_80038294[index].loopStart34 = sample->loopStart;
    D_80038294[index].loopLength38 = sample->loopLength;
    D_80038294[index].format5D = sample->format;
    if (D_80038294[index].loopLength38 != 0) {
        D_80038294[index].tail3C = D_80038294[index].length30 -
            D_80038294[index].loopLength38 - D_80038294[index].loopStart34;
        if (D_80038294[index].tail3C < 10) D_80038294[index].tail3C = 0;
    } else {
        D_80038294[index].tail3C = 0;
    }
    if (((unsigned long)sample->data & 0xF0000000UL) != 0x80000000UL) {
        D_80038294[index].flags |= 1;
    }
}
