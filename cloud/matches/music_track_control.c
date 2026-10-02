/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef int s32;
extern f32 D_80123A6C;
extern void func_80020494(u8, u16, s32, s32);
void music_track_control(f32 value, u16 channel, s32 first, s32 second) {
    if (first) func_80020494((u8)(u32)(value * 127.0f * D_80123A6C), channel, 1, 0);
    if (second) func_80020494((u8)(u32)(value * 127.0f), channel, 0, 1);
}
