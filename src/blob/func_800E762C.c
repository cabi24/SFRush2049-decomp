/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800E762C(n) -- per-model rescale over the six 0x808-byte MODELDAT records at D_8014A250.
 * Stores -n to D_80143FF4 and (-n) * D_8002AFB8 to D_801543CC. For each record: outside gameplay mode
 * 2, for record 0, or when its divisor (+0x718) is 0, it takes the new values directly (+0x710 = -n,
 * +0x714 = D_801543CC, +0x718 = the scale); otherwise +0x710 = round(D_801543CC / divisor) (round
 * half away from zero through `q -/+ 0.5f` truncation) and +0x714 = D_801543CC. No arcade ancestor
 * identified.
 *
 * Shaping (w13f; w9d's draft was 26/57):
 *  - the loop body reads the globals D_80143FF4 and D_801543CC back instead of locals holding the
 *    values just stored. The indexed stores into D_8014A250[] do not alias those globals, so uopt
 *    keeps them in registers; reading them back is what makes &D_80143FF4 a coloured web (retail's a3)
 *    and gives retail's FP colouring (D_801543CC's value in f12, the divisor load in f2). With locals
 *    v/f (w9d) both were wrong.
 *  - goto loop (a for/do-while loop is unrolled by -O3: 26 extra words) and the inverted condition
 *    order `!= 2 || i == 0 || divisor == 0.0f` are w9d's.
 * Also MATCH at -O2. Leaf, no frame.
 */
typedef int s32;
typedef float f32;

typedef struct {
    char pad0[0x710];
    s32 unk710;
    f32 unk714;
    f32 unk718;
    char pad71C[0x808 - 0x71C];
} Model;

extern s32 D_80143FF4;
extern f32 D_801543CC;
extern f32 D_8002AFB8;
extern s32 gameplay_mode;
extern Model D_8014A250[6];

void func_800E762C(s32 arg0)
{
    f32 q;
    s32 i;
    f32 scale;

    D_80143FF4 = -arg0;
    scale = D_8002AFB8;
    D_801543CC = D_80143FF4 * scale;
    i = 0;
loop:
    if (gameplay_mode != 2 || i == 0 || D_8014A250[i].unk718 == 0.0f) {
        D_8014A250[i].unk710 = D_80143FF4;
        D_8014A250[i].unk714 = D_801543CC;
        D_8014A250[i].unk718 = scale;
    } else {
        q = D_801543CC / D_8014A250[i].unk718;
        if (q < 0.0f) {
            D_8014A250[i].unk710 = q - 0.5f;
        } else {
            D_8014A250[i].unk710 = q + 0.5f;
        }
        D_8014A250[i].unk714 = D_801543CC;
    }
    i++;
    if (i < 6) goto loop;
}
