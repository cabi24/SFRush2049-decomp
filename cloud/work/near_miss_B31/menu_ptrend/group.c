/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef int s32;
extern f32 D_801240FC,D_80124100,D_80124104;
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern void func_800A61B0(f32*,f32*,f32*);
#define FIELD(p,o) (*(f32*)((char*)(p)+(o)))
void menu_load_options(void *arg0,void *arg1,void *arg2,f32 *arg3) {
    f32 transformed[3];
    f32 normalized[3];
    f32 squared;
    f32 inverse;
    f32 *out,*in;
    void *other;
    other = arg0==arg1 ? arg2 : arg1;
    squared = arg3[2]*arg3[2]+(arg3[0]*arg3[0]+arg3[1]*arg3[1]);
    if (squared < D_801240FC) {
        normalized[1]=normalized[2]=0.0f;
        normalized[0]=D_80124100;
    } else {
        inverse = D_80124104 / sqrtf(squared);
        for(out=normalized,in=arg3;out!=normalized+3;out++,in++) *out=*in*inverse;
    }
    func_800A61B0(normalized,transformed,(f32*)((char*)arg0+0x7A0));
    FIELD(arg0,0x124)+=transformed[0];
    FIELD(arg0,0x128)+=transformed[1];
    FIELD(arg0,0x12C)+=transformed[2];
    func_800A61B0(normalized,transformed,(f32*)((char*)other+0x7A0));
    FIELD(other,0x124)-=transformed[0];
    FIELD(other,0x128)-=transformed[1];
    FIELD(other,0x12C)-=transformed[2];
}

void func_800A61B0(f32 *arg0, f32 *arg1, f32 *arg2) {
    arg1[0] = (arg0[0] * arg2[0] + arg0[1] * arg2[1]) + arg0[2] * arg2[2];
    arg1[1] = (arg0[0] * arg2[3] + arg0[1] * arg2[4]) + arg0[2] * arg2[5];
    arg1[2] = (arg0[0] * arg2[6] + arg0[1] * arg2[7]) + arg0[2] * arg2[8];
}
