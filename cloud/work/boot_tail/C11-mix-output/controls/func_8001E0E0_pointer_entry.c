/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
typedef unsigned int u32;
extern const float D_8002CA40[129];
extern const float D_8002CC44[4];
extern const float D_8002D910;
extern const float D_8002D914;
extern const float D_8002D918;
extern const float D_8002D91C;
extern const float D_8002D920;

void func_8001E0E0(u16 *left, u16 *right, u32 volume, u32 pan,
                   u32 span, u16 *span_output, u32 aux, u16 *aux_output)
{
    float fraction;
    float gain;
    float factor;
    float scale;
    const float *entry;

    if (volume > 0x7F0000U) {
        volume = 0x7F0000U;
    }
    scale = D_8002D910;
    fraction = (float)(volume & 0xFFFFU) * (1.0f / 65536.0f);
    entry = &D_8002CA40[volume >> 16];
    gain = (1.0f - fraction) * entry[0] +
           entry[1] * fraction;
    fraction = (float)(span & 0x3FFFFFU) * (1.0f / 4194304.0f);
    entry = &D_8002CC44[span >> 22];
    factor = (1.0f - fraction) * entry[0] +
             entry[1] * fraction;
    *span_output = (int)(gain * factor * D_8002D914 * scale);
    span = 0x800000U - span;
    if (span >= 0x800000U) {
        span = 0x7F0000U;
    }
    fraction = (float)(span & 0x3FFFFFU) * (1.0f / 4194304.0f);
    entry = &D_8002CC44[span >> 22];
    factor = (1.0f - fraction) * entry[0] +
             entry[1] * fraction;
    gain *= factor;
    if (pan == 0x800000U) {
        *left = (int)(gain * D_8002D918 * scale);
        *right = (int)(gain * D_8002D91C * scale);
    } else {
        fraction = (float)(pan & 0x3FFFFFU) * (1.0f / 4194304.0f);
        entry = &D_8002CC44[pan >> 22];
        factor = (1.0f - fraction) * entry[0] +
                 entry[1] * fraction;
        *left = (int)(gain * factor * scale);
        pan = 0x800000U - pan;
        if (pan >= 0x800000U) {
            pan = 0x7F0000U;
        }
        fraction = (float)(pan & 0x3FFFFFU) * (1.0f / 4194304.0f);
        entry = &D_8002CC44[pan >> 22];
        factor = (1.0f - fraction) * entry[0] +
                 entry[1] * fraction;
        *right = (int)(gain * factor * scale);
    }
    if (aux > 0x7F0000U) {
        aux = 0x7F0000U;
    }
    fraction = (float)(aux & 0xFFFFU) * (1.0f / 65536.0f);
    entry = &D_8002CA40[aux >> 16];
    gain = (1.0f - fraction) * entry[0] +
           entry[1] * fraction;
    *aux_output = (int)(gain * D_8002D920 * scale);
}
