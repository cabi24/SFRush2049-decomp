typedef unsigned char u8; typedef signed char s8; typedef unsigned short u16; typedef unsigned int u32; typedef float f32;
typedef union Color4 {u8 c[4];u32 word;} Color4;
extern void f(void *);
void T(s8 p)
{
    int a; f32 pp[3]; f32 by; f32 sel; s8 cl=1; void *tx; u16 ti; Color4 col={{255,255,255,255}};
    f(&a); f(pp); f(&by); f(&sel); f(&cl); f(&tx); f(&ti); f(&col);
}
