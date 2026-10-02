/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Direct ancestor: rushtherock/game/drivetra.c:autoshift. */
typedef float f32;
typedef signed short s16;
typedef unsigned char u8;
typedef struct Parameters {u8 prefix[172]; f32 upshift,downshift;} Parameters;
typedef struct Tuning {u8 prefix[20]; f32 shift_gain;} Tuning;
typedef struct Model2056 {
    Parameters *parameters; Tuning *tuning;
    u8 to_throttle[968]; f32 throttle;
    u8 to_gear[32]; s16 gear,commandgear;
    u8 to_engangvel[16]; f32 engangvel;
    u8 tail[1020];
} Model2056;
extern void func_800E313C(Model2056 *,f32,f32);
extern void func_800E30F4(Model2056 *);
extern void func_800E30AC(Model2056 *);
void func_800E31D4(Model2056 *m)
{
    f32 modupshiftangvel,moddownshiftangvel,fact;
    fact=(m->throttle+(f32)3.0)*(f32).25;
    modupshiftangvel=m->tuning->shift_gain*m->parameters->upshift*fact;
    moddownshiftangvel=m->tuning->shift_gain*m->parameters->downshift*fact;
    if (m->commandgear==0 || m->commandgear==-1) {
        m->gear=m->commandgear;
    } else {
        if (m->gear==0 || m->gear==-1)
            func_800E313C(m,modupshiftangvel,moddownshiftangvel);
        if (m->engangvel>modupshiftangvel) func_800E30F4(m);
        if (m->engangvel<moddownshiftangvel) func_800E30AC(m);
    }
}
