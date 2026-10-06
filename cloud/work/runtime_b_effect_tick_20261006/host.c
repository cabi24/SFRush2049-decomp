#include "effect_tick.c"
EffectControl D_80399AE0;
EffectPlayer D_80152818[8];
s16 D_8014A108;
f32 D_8002EB94;
u16 D_80142A82, D_80142A84, D_80142A8E;
u32 (*boundary)(u32, u32, u32, u32);
f32 func_8008B2E4(f32 range) {
    union { f32 f; u32 u; } input, output;
    input.f = range;
    output.u = boundary(0, input.u, 0, 0);
    return output.f;
}
void func_80090770(s16 index, u16 value) { boundary(1, index, value, 0); }
void func_8008B0D8(s32 index, s32 mode, u32 viewports) { boundary(2, index, mode, viewports); }
