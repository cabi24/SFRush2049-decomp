/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
extern const float D_8002C640[];
extern const float D_8002C840[];
extern int D_8004F800;

u32 func_8001E50C(u8 note, u32 encoded_value)
{
    u8 reference;
    float factor;
    float value;
    if (encoded_value == 0xFFFFFFFFU) {
        encoded_value = 0x40005622U;
    }
    reference = encoded_value >> 24;
    if (reference != note) {
        if (reference < note) {
            factor = D_8002C640[note - reference];
        } else {
            factor = D_8002C840[reference - note];
        }
        value = factor * (float)(encoded_value & 0xFFFFFFU);
    } else {
        value = (float)(encoded_value & 0xFFFFFFU);
    }
    return (u32)((value * 4096.0f) / (float)D_8004F800);
}
