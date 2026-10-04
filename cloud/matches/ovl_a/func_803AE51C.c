/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u8 pad0[128]; void *text128; u8 pad132[808]; s32 value940; } Countdown;
void func_800B65B4(f32 value);
void func_800B669C(s32 a, s32 b);
void func_800B42F0(s32 id);
void func_800B74A0(s32 id);
void func_800BE078(s32 x, s32 y, s32 w, s32 h, s32 color, s32 flags, void *text);
void func_800B71D4(s32 x, s32 y, s32 value);
extern s8 D_801613E8;
extern u8 D_803B8D64[];
extern Countdown *D_8017A4E4;

s32 func_803AE51C(s32 arg0)
{
    func_800B65B4(0.0f);
    func_800B669C(1, 1);
    if (D_801613E8 == 1) {
        func_800B42F0(13);
        func_800B74A0(22);
        func_800BE078(160, 190, 320, 110, -1, 0, D_803B8D64);
    } else {
        func_800B42F0(11);
        func_800B74A0(22);
        func_800BE078(160, 185, 320, 110, -1, 0, D_8017A4E4->text128);
        func_800B74A0(0);
        func_800B42F0(12);
        func_800B71D4(160, 205, D_8017A4E4->value940);
    }
    func_800B669C(0, 3);
    func_800B65B4(-1.0f);
    return 1;
}
