/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * NOT a strict match: score.py prints "MATCH (6 section-relative relocations
 * unverified: .data+0x0 ...)". Text words are identical.
 *
 * Per-car reset: clears fields at +0x314..+0x33E / +0xFA..+0xFE of the second
 * record and, for kind == 1 cars, hands out a round-robin id 0..3 from a
 * counter at 0x801161D0.
 *
 * The counter must be a function-local `static s32 ... = 0;` (retail .data,
 * initial value 0 at 0x801161D0). With any file-scope or extern declaration
 * (plain, volatile, array, struct member, defined in the unit) IDO -O2 hoists
 * the address into a register (lui/addiu + 0(reg)) and 7-26 words differ; the
 * target's lui/lw + two lui at/sw pairs only come from a function static.
 * The scorer cannot check a .data-relative relocation, hence "unverified".
 */
typedef float f32;
typedef short s16;
typedef int s32;
typedef signed char s8;

typedef struct {
    char pad0[0x7CC];
    s8 unk7CC;
    char pad7CD[0xF];
    s8 unk7DC;
} CarA;

typedef struct {
    char pad0[0xFA];
    s16 unkFA;
    s16 unkFC;
    s16 unkFE;
    char pad100[0x214];
    f32 unk314;
    f32 unk318;
    char pad31C[0x18];
    f32 unk334;
    s16 unk338;
    s16 unk33A;
    s16 unk33C;
    s16 unk33E;
} CarB;

extern f32 D_801543CC;
extern s16 D_80151CEE;

void func_800EC270(CarA *a, CarB *b) {
    static s32 D_801161D0 = 1;

    b->unk338 = 0;
    b->unk314 = 0.0f;
    b->unk318 = 0.0f;
    b->unk334 = D_801543CC;
    b->unk33A = -1;
    if (a->unk7CC == 1) {
        b->unk33C = D_801161D0;
        b->unk33E = D_801161D0;
        D_801161D0++;
        if (D_801161D0 >= 4) {
            D_801161D0 = 0;
        }
    } else {
        b->unk33C = 0;
    }
    if (D_80151CEE == 0) {
        a->unk7DC = 1;
    }
    b->unkFA = -1;
    b->unkFC = 0;
    b->unkFE = 0;
}
