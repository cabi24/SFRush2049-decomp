/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef int s32;
typedef unsigned int u32;
typedef unsigned short u16;
extern f32 D_80114750[][2];
extern f32 D_80114744;
extern u32 D_80149B88;
extern void func_8008A644(u16);
void func_800F6928(f32 value) {
    s32 index = 0;
    f32 (*table)[2] = D_80114750;
    f32 prevx, prevy;
    f32 (*cursor)[2];
    if (table[index][0] < value && table[index][0] < 32000.0f) { cursor = table + index; do { index++; cursor++; } while ((*cursor)[0] < value && (*cursor)[0] < 32000.0f); }
    if (index > 0) {
        prevx = table[index-1][0];
        prevy = table[index-1][1];
        value = (value - prevx) * (table[index][1] - prevy) / (table[index][0] - prevx) + prevy;
    }
    D_80114744 = value;
    if (value > 0.0f) {
        D_80149B88 |= 0x10;
        func_8008A644((u16)(u32)value);
    } else D_80149B88 &= ~0x10;
}
