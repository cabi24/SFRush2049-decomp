/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
extern f32 D_80118E30,D_8002EB94,D_8017A630;
void func_800947F0(void) {
    D_80118E30+=D_8002EB94;
    D_8017A630=D_80118E30+D_80118E30;
    D_8017A630-=(int)D_8017A630;
    if (D_8017A630>0.5f) D_8017A630=1.0f-D_8017A630;
    D_8017A630+=D_8017A630;
}
