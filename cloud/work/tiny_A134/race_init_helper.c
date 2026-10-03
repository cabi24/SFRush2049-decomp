/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef int s32;
typedef signed short s16;
typedef unsigned char u8;
typedef float f32;
extern s32 D_8014A110;
extern s16 D_8014A108;
extern u8 D_80152BD0[];
extern u8 D_80152BD8[];
extern u8 D_80152C20[];
extern void func_800E847C(void);
extern void func_8008D6FC(s16 model, f32 *position, f32 *basis);
extern void func_800EB90C(void);
void race_init_helper(void)
{
    s32 i,j;
    func_800E847C();
    if(D_8014A110==2) {
        for(i=1,j=0;i<D_8014A108;i++,j++) {
            func_8008D6FC(*(s16 *)(D_80152BD0+j*952+246),
                         (f32 *)(D_80152BD8+j*952),
                         (f32 *)(D_80152C20+j*952));
        }
    }
    func_800EB90C();
}
