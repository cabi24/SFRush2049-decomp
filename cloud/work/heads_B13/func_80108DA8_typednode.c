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
typedef struct { s32 next; s16 flags,pad6; s32 pad8; s16 padC,p0,p1,pad12,p3,pad16; s8 b18,pad19,b1A,pad1B; s32 pad1C,pad20,pad24,word28,index; } MenuNode;
s32 func_80108DA8(MenuNode *arg0) {
    s32 sp24;
    s32 temp_a1;
    s32 temp_v0;
    s32 temp_t1;
    s32 var_v0;

    temp_a1 = arg0->index;
    if ((temp_a1 >= (s16) D_80151AD0) || (state_word_a & 8) || (D_801543CA < 2)) {
        arg0->word28 = 0;
        if (arg0->b1A != 1) {
            arg0->b1A = 1;
            Input_ApplyPadConfig(arg0);
        }
        return arg0->b1A;
    }
    temp_t1 = D_80156CE8 == 0;
    var_v0 = temp_t1;
    if (temp_t1 == 0) {
        var_v0 = *((s8 *) &D_80152907 + (temp_a1 * 0x3B8)) == 1;
    }
    if (var_v0 != arg0->b1A) {
        arg0->b1A = var_v0;
        sp24 = temp_a1;
        Input_ApplyPadConfig(arg0);
    }
    temp_v0 = temp_a1 * 8;
    if (arg0->b1A != 0) {
        return 1;
    }
    arg0->p0 = (s16) M2C_FIELD(((u8 *) &D_80116028 + ((s16) D_80151AD0 << 5) + temp_v0), s32 *, -0x20);
    arg0->p1 = (s16) M2C_FIELD(((u8 *) &D_80116028 + ((s16) D_80151AD0 << 5) + temp_v0), s32 *, -0x1C);
    if ((s16) D_80151AD0 >= 2) {
        func_800EF5B0(arg0, &D_80120E34, 0);
        arg0->b18 = 0x60;
    }
    Input_ApplyPadConfig(arg0);
    D_8011617C = (s32) arg0->p3;
    D_80116180 = (s32) arg0->pad16;
    return 1;
}

