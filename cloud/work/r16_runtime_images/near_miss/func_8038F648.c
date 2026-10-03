typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct {
    u8 count;
    u8 pad[3];
    u32 held[20];
    u32 pressed[20];
} Combo;
extern Combo *D_803B4278;
extern f32 D_803B427C;
extern s32 D_803B4280;
extern f32 D_8002EB90;
extern u32 D_8015694C;
extern u32 D_80156944;

s32 func_8038F648(void)
{
    Combo *combo = D_803B4278;
    s32 step;
    u32 pressed;
    u32 held;
    u32 other;

    if (combo == 0) {
        return 0;
    }
    step = D_803B4280;
    if (step != 0 && D_803B427C <= D_8002EB90) {
        D_803B4280 = 0;
        step = 0;
    }
    pressed = combo->pressed[step];
    held = combo->held[step];
    other = ~(pressed | held);
    if (other & D_8015694C) {
        D_803B4280 = 0;
        return 0;
    }
    if ((D_80156944 & held) != held) {
        return 0;
    }
    if ((D_8015694C & pressed) != pressed) {
        return 0;
    }
    if (other & D_80156944) {
        return 0;
    }
    D_803B427C = D_8002EB90 + 1.0f;
    D_803B4280 = step + 1;
    if (D_803B4280 != combo->count) {
        return 0;
    }
    return 1;
}
