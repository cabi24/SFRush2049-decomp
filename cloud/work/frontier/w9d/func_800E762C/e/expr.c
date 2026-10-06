/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef int s32;
typedef float f32;

typedef struct {
    char pad0[0x710];
    s32 unk710;
    f32 unk714;
    f32 unk718;
    char pad71C[0x808 - 0x71C];
} Model;

extern volatile s32 D_80143FF4;
extern f32 D_801543CC;
extern f32 D_8002AFB8;
extern s32 D_8014A110;
extern Model D_8014A250[6];

void func_800E762C(s32 arg0)
{
    s32 v;
    f32 q;
    s32 i;

    v = -arg0;
    D_80143FF4 = v;
    D_801543CC = v * D_8002AFB8;
    i = 0;
loop:
    if (D_8014A110 != 2 || i == 0 || D_8014A250[i].unk718 == 0.0f) {
        D_8014A250[i].unk710 = v;
        D_8014A250[i].unk714 = v * D_8002AFB8;
        D_8014A250[i].unk718 = D_8002AFB8;
    } else {
        q = v * D_8002AFB8 / D_8014A250[i].unk718;
        if (q < 0.0f) {
            D_8014A250[i].unk710 = q - 0.5f;
        } else {
            D_8014A250[i].unk710 = q + 0.5f;
        }
        D_8014A250[i].unk714 = v * D_8002AFB8;
    }
    i++;
    if (i < 6) goto loop;
}
