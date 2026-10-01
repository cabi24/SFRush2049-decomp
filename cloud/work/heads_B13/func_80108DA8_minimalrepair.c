/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#define NULL ((void *)0)
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern s16 D_80151AD0, D_801543CA;
extern s8 D_80156CE8, D_80152907;
extern s32 state_word_a, D_8011617C, D_80116180;
extern s32 D_80116028, D_80120E34;
extern void Input_ApplyPadConfig(void *);
extern void func_800EF5B0(void *,void *,s32);
s32 func_80108DA8(void *arg0) {
    s32 sp24;
    s32 temp_a1;
    s32 temp_v0;
    s32 temp_t1;
    s32 var_v0;

    temp_a1 = M2C_FIELD(arg0, s32 *, 0x2C);
    if ((temp_a1 >= (s16) D_80151AD0) || (state_word_a & 8) || (D_801543CA < 2)) {
        M2C_FIELD(arg0, s32 *, 0x28) = 0;
        if (M2C_FIELD(arg0, s8 *, 0x1A) != 1) {
            M2C_FIELD(arg0, s8 *, 0x1A) = 1;
            Input_ApplyPadConfig(arg0);
        }
        return M2C_FIELD(arg0, s8 *, 0x1A);
    }
    temp_t1 = D_80156CE8 == 0;
    var_v0 = temp_t1;
    if (temp_t1 == 0) {
        var_v0 = *((s8 *) &D_80152907 + (temp_a1 * 0x3B8)) == 1;
    }
    if (var_v0 != M2C_FIELD(arg0, s8 *, 0x1A)) {
        M2C_FIELD(arg0, s8 *, 0x1A) = var_v0;
        sp24 = temp_a1;
        Input_ApplyPadConfig(arg0);
    }
    temp_v0 = temp_a1 * 8;
    if (M2C_FIELD(arg0, s8 *, 0x1A) != 0) {
        return 1;
    }
    M2C_FIELD(arg0, s16 *, 0xE) = (s16) M2C_FIELD(((u8 *) &D_80116028 + ((s16) D_80151AD0 << 5) + temp_v0), s32 *, -0x20);
    M2C_FIELD(arg0, s16 *, 0x10) = (s16) M2C_FIELD(((u8 *) &D_80116028 + ((s16) D_80151AD0 << 5) + temp_v0), s32 *, -0x1C);
    if ((s16) D_80151AD0 >= 2) {
        func_800EF5B0(arg0, &D_80120E34, 0);
        M2C_FIELD(arg0, s8 *, 0x18) = 0x60;
    }
    Input_ApplyPadConfig(arg0);
    D_8011617C = (s32) M2C_FIELD(arg0, s16 *, 0x14);
    D_80116180 = (s32) M2C_FIELD(arg0, s16 *, 0x16);
    return 1;
}

