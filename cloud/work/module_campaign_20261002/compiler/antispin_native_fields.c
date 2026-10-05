/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32; typedef int s32; typedef short s16; typedef unsigned char u8;
typedef struct Parameters { u8 prefix[36]; f32 steerScale; } Parameters;
typedef struct AntiSpin {
    u8 prefix0[4]; Parameters *parameters;
    u8 gap8[56]; f32 velocity[3];
    u8 gap76[4]; f32 rotation[3];
    u8 gap92[224]; f32 centerMoment[3];
    u8 gap328[652]; f32 throttle;
    u8 gap984[24]; f32 speed;
    u8 gap1012[440]; f32 gain;
    u8 gap1456[92]; s32 road2,road3;
    u8 gap1556[268]; f32 damping;
    u8 gap1828[166]; s16 mode1994;
    u8 gap1996[8]; s32 flags;
} AntiSpin;
extern f32 D_801243C0;
void func_800E1AA0(AntiSpin *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f2;
    f32 var_f0;

    if (!(arg0->flags & 0x10)) {
        if ((arg0->road2 != 8) || (arg0->road3 != 8)) {
            var_f0 = arg0->gain;
            var_f0 += arg0->throttle * 0.5f;
            if (var_f0 > 1.0f) {
                var_f0 = 1.0f;
            }
            temp_f2 = arg0->speed;
            if (temp_f2 > 100.0f) {
                arg0->centerMoment[1] = (f32) (arg0->centerMoment[1] - (arg0->damping * D_801243C0 * var_f0));
            } else {
                arg0->centerMoment[1] = (f32) (arg0->centerMoment[1] - (arg0->damping * temp_f2 * 120.0f * var_f0));
            }
        }
        if (!(arg0->velocity[2] < 0.0f)) {
            if (arg0->mode1994 != 0) {
                temp_f0 = arg0->velocity[0];
                if ((temp_f0 * arg0->rotation[0]) > 0.0f) {
                    arg0->centerMoment[1] = (f32) (arg0->centerMoment[1] - (temp_f0 * 100.0f));
                }
            } else {
                temp_f0_2 = arg0->velocity[0];
                if (((temp_f0_2 * arg0->rotation[0]) > 0.0f) && (arg0->throttle < 0.5f)) {
                    arg0->centerMoment[1] = (f32) (arg0->centerMoment[1] - (temp_f0_2 * 100.0f * arg0->parameters->steerScale));
                }
            }
        }
    }
}
