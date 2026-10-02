/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef int s32; typedef unsigned int u32; typedef float f32;
#define M2C_FIELD(expr,type,offset) (*(type)((char*)(expr)+(offset)))
extern f32 D_80110668[],D_80152720[];
extern f32 D_801543CC,D_801244D8,D_801244DC,D_801244E0,D_801244E4;
extern s8 D_80110680[];
extern f32 cosf(f32),sinf(f32);
extern void func_800E8D50(void*,s32,s32,f32*);
void func_800E9E2C(void *arg0, s32 arg1, s32 arg2) {
    f32 vector[3];
    s32 index;
    f32 *start;
    s32 mode;
    f32 angle;
    index = M2C_FIELD(arg0,s8*,0x35C);
    mode = D_80110680[M2C_FIELD(arg0,s8*,0x35B)];
    start = &D_80110668[M2C_FIELD(arg0,s8*,0x35B)];
    if (mode == 1 && *start+10.0f < D_801543CC) {
        *start = D_801543CC;
        D_80110680[M2C_FIELD(arg0,s8*,0x35B)] = 2;
        mode = D_80110680[M2C_FIELD(arg0,s8*,0x35B)];
    }
    if (mode == 2) {
        angle = (f32)((u32)(D_80110668[M2C_FIELD(arg0,s8*,0x35B)]*1000.0f) % 6000U) * D_801244D8 * D_801244DC;
    } else {
        angle = (f32)((u32)(D_801543CC*1000.0f) % 6000U) * D_801244E0 * D_801244E4;
    }
    vector[0] = cosf(angle)*15.0f;
    vector[1] = 10.0f;
    vector[2] = sinf(angle)*15.0f;
    D_80152720[index] = 0.0f;
    func_800E8D50(arg0,arg1,arg2,vector);
}
