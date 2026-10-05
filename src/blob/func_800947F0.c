/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Quirk: D_8002EB94 is read through a (s32) address cast, which makes IDO take its address in a
 * register (lui/addiu/lwc1 0(reg)) and load it before D_80118E30; the two written globals are volatile. */
typedef float f32;
typedef int s32;

extern f32 D_8002EB94;
extern volatile f32 D_80118E30;
extern volatile f32 D_8017A630;

void func_800947F0(void) {
    f32 t;
    f32 u;

    D_80118E30 += *(f32 *) (s32) &D_8002EB94;
    t = D_80118E30;
    D_8017A630 = t + t;
    u = D_8017A630;
    D_8017A630 = u - (s32) u;
    u = D_8017A630;
    if (u > 0.5f) {
        D_8017A630 = 1.0f - u;
        u = D_8017A630;
    }
    D_8017A630 = u + u;
}
