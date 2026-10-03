typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
extern s16 D_8014A96C;
void math_utility(void *arg0, void *arg1);
void func_800D11BC(void *arg0);
void func_800D1004(void *arg0);
void func_800CF06C(void *arg0);
void func_800D0424(void *arg0);
extern f32 D_8012416C;
extern f32 D_80124170;
void func_800D11BC(void *arg0)
{
    func_800D1004(arg0);
    *(f32 *)((s8 *)arg0 + 0x720) = 0.0f;
    *(f32 *)((s8 *)arg0 + 0x724) = 0.0f;
    *(f32 *)((s8 *)arg0 + 0x728) = 0.0f;
    *(f32 *)((s8 *)arg0 + 0x72C) = 1.0f;
    *(s8 *)((s8 *)arg0 + 0x730) = 1;
    func_800CF06C(arg0);
    func_800D0424(arg0);
    *(s16 *)((s8 *)arg0 + 0x7D0) = *(f32 *)((s8 *)arg0 + 0x408) * D_8012416C * D_80124170;
}
