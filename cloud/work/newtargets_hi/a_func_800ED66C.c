typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
extern float D_80114748;
extern u32 D_80149B88;
void func_8008A38C(u16 a);
void func_800ED66C(float x) {
    D_80114748 = x;
    if (x < 0.0f || x >= 255.0f) {
        D_80149B88 &= ~0x20;
    } else {
        D_80149B88 |= 0x20;
        func_8008A38C((u32)x);
    }
}
