/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * Historical label menu_load_options is misleading: this is the N64
 * descendant of arcade ForceApart (reference/repos/rushtherock/game/
 * collision.c): normalise dir, scale to 40000, rotate through each car's
 * matrix with func_800A61B0 and push `car` and the other car of the pair
 * (m == m1 ? m2 : m1) apart with equal and opposite forces at +0x124.
 *
 * Needs -O3: at -O2 IDO puts `car` in s0 (90/91 words differ); at -O3 it is
 * kept in t0 and spilled around the call like the target.
 * D_801240FC/D_80124100/D_80124104 are the function's own float literals
 * (0.0001f, 40000.0f, 40000.0f) in retail .rodata, referenced as externs so
 * the scorer can verify the addresses.
 * Quirk: `mag` is an unused local that supplies the last 8 bytes of the
 * 80-byte frame (arcade ForceApart has a `force` scalar in that position).
 */
typedef float f32;
typedef int s32;
typedef signed char s8;

typedef struct {
    char pad0[0x124];
    f32 unk124[3];
    char pad130[0x510];
    s8 unk640;
    char pad641[0x15F];
    f32 unk7A0[3][3];
} Car;

extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern f32 D_801240FC;
extern f32 D_80124100;
extern f32 D_80124104;

void func_800A61B0(f32 *arg0, f32 *arg1, f32 *arg2);

void menu_load_options(Car *car, Car *a, Car *b, f32 *dir) {
    Car *other;
    f32 len;
    f32 mag;
    f32 force[3];
    f32 out[3];
    s32 i;

    if (car == a) {
        other = b;
    } else {
        other = a;
    }
    len = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    if (len < D_801240FC) {
        force[0] = D_80124100;
        force[1] = 0.0f;
        force[2] = 0.0f;
    } else {
        len = D_80124104 / sqrtf(len);
        for (i = 0; i < 3; i++) {
            force[i] = dir[i] * len;
        }
    }
    func_800A61B0(force, out, car->unk7A0[0]);
    car->unk124[0] = car->unk124[0] + out[0];
    car->unk124[1] = car->unk124[1] + out[1];
    car->unk124[2] = car->unk124[2] + out[2];
    func_800A61B0(force, out, other->unk7A0[0]);
    other->unk124[0] = other->unk124[0] - out[0];
    other->unk124[1] = other->unk124[1] - out[1];
    other->unk124[2] = other->unk124[2] - out[2];
}
