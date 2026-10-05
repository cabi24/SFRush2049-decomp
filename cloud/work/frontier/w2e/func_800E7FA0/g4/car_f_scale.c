/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct CarParams {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 scale;
} CarParams;

typedef struct Car {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ CarParams *params;
    /* 0x008 */ u8 pad8[0x480 - 0x8];
    /* 0x480 */ f32 f480;
    /* 0x484 */ f32 f484;
    /* 0x488 */ u8 pad488[0x4DC - 0x488];
    /* 0x4DC */ f32 f4DC;
    /* 0x4E0 */ f32 f4E0;
    /* 0x4E4 */ u8 pad4E4[0x538 - 0x4E4];
    /* 0x538 */ f32 f538;
    /* 0x53C */ f32 f53C;
    /* 0x540 */ u8 pad540[0x594 - 0x540];
    /* 0x594 */ f32 f594;
    /* 0x598 */ f32 f598;
    /* 0x59C */ u8 pad59C[0x61C - 0x59C];
    /* 0x61C */ u16 wheel[4];
    /* 0x624 */ u8 pad624[0x7D4 - 0x624];
    /* 0x7D4 */ u32 flags;
    /* 0x7D8 */ u8 pad7D8[0x7F8 - 0x7D8];
    /* 0x7F8 */ f32 level[4];
} Car; /* 0x808 */

typedef struct Curve {
    /* 0x00 */ s32 level[5];
    /* 0x14 */ f32 first;
    /* 0x18 */ f32 second;
} Curve; /* 0x1C */

extern Car D_8014A250[];
extern u32 D_80120EBC[4];
extern u32 D_80120ECC[4];
extern u32 D_80120EDC[4];
extern Curve D_80120EEC[4];
extern s8 D_8013FECB;
extern s32 D_8014A110;

f32 fabsf(f32);
#pragma intrinsic(fabsf)

void func_800E7FA0(s16 index)
{
    Car *car;
    f32 f;
    f32 scale;
    s32 sample;
    s32 blocked;
    s32 alternate;
    s32 i;
    s32 segment;
    f32 a;
    f32 b;
    f32 magnitude;
    Curve *curve;

    car = &D_8014A250[index];
    blocked = car->wheel[1] == 8 || car->wheel[2] == 8;
    alternate = car->wheel[1] == 2 || car->wheel[2] == 2;
    for (i = 0; i < 4; i++) {
        if (car->wheel[i] == 1) {
            car->flags |= D_80120ECC[i];
        } else {
            car->flags &= ~(D_80120EDC[i] | D_80120ECC[i]);
        }
    }
    if (blocked || alternate || D_8013FECB != 0) {
        for (i = 0; i < 4; i++) {
            car->level[i] = 0.0f;
            car->flags &= ~(D_80120EDC[i] | D_80120EBC[i]);
        }
    } else {
        curve = D_80120EEC;
        for (i = 0; i < 4; i++, curve++) {
            switch (i) {
            case 0:
                a = fabsf(car->f4DC);
                b = fabsf(car->f594);
                if (b < a) {
                    sample = a;
                } else {
                    sample = b;
                }
                break;
            case 1:
                a = fabsf(car->f480);
                b = fabsf(car->f538);
                if (b < a) {
                    sample = a;
                } else {
                    sample = b;
                }
                break;
            case 2:
                a = fabsf(car->f484);
                b = fabsf(car->f4E0);
                if (b < a) {
                    sample = a;
                } else {
                    sample = b;
                }
                break;
            case 3:
                sample = fabsf(car->f598) + fabsf(car->f53C);
                break;
            }
            magnitude = sample;
            if (D_8014A110 == 6) {
                scale = 3.0f;
            } else {
                scale = car->params->scale;
            }
            if (curve->level[0] * scale <= magnitude) {
                if (curve->level[4] < sample) {
                    sample = curve->level[4];
                }
                for (segment = 0; segment < 4; segment++) {
                    if (curve->level[segment + 1] >= sample) {
                        break;
                    }
                }
                f = ((f32)(sample - curve->level[segment]) / (f32)(curve->level[segment + 1] - curve->level[segment]) + segment) * 0.25f;
                car->level[i] = f;
                if (curve->first < f) {
                    car->flags |= D_80120EDC[i];
                }
                if (curve->second < car->level[i]) {
                    car->flags |= D_80120EBC[i];
                }
            } else if (car->level[i] != 0.0f) {
                if (curve->first < car->level[i]) {
                    car->flags |= D_80120EDC[i];
                }
                if (curve->second < car->level[i]) {
                    car->flags |= D_80120EBC[i];
                }
                car->level[i] -= 0.125f;
                if (car->level[i] <= 0.0f) {
                    car->level[i] = 0.0f;
                    car->flags &= ~(D_80120EDC[i] | D_80120EBC[i]);
                }
            }
        }
    }
}

void standin_caller(s32 n)
{
    s16 i;

    for (i = 0; i < n; i++) {
        if (D_8014A250[i].wheel[0] != 0) {
            func_800E7FA0(i);
        }
    }
    func_800E7FA0(0);
}
