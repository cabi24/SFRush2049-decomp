#ifndef NATIVE_CONTROLS_SYM_TYPES_H
#define NATIVE_CONTROLS_SYM_TYPES_H
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct Wheel92 {u8 other0[72];f32 velocity;u8 other76[16];} Wheel92;
typedef struct Physics {
    u8 other0[10];
    s8 autotrans;
    u8 other11[185];
    f32 BODYFORCE[4][3];
    u8 other244[48];
    f32 CENTERFORCE[3];
    f32 last_CENTERFORCE[3];
    f32 CENTERMOMENT[3];
    f32 peak_body_force[2][3];
    f32 peak_center_force[2][3];
    u8 other376[568];
    f32 steerangle;
    f32 torque[4];
    f32 steergain;
    u8 other968[4];
    f32 clutch;
    f32 throttle;
    f32 brake;
    u8 other984[4];
    f32 brakegain[4];
    u8 other1004[8];
    s16 gear;
    s16 commandgear;
    u8 other1016[16];
    f32 engangvel;
    u8 other1036[36];
    Wheel92 wheel[4];
    u8 other1440[148];
    f32 dt,idt;
    u8 other1596[4];
    s8 frozen;
    u8 other1601[27];
    s16 state;
    u8 other1630[178];
    s32 control_flags;
    u8 other1812[4];
    f32 modeltime;
    u8 other1820[4];
    f32 wheel_input;
    f32 clutch_input;
    f32 brake_input;
    f32 throttle_input;
    s8 gear_input;
    u8 other1841[149];
    s16 slot;
    u8 other1992[4];
    s8 mode;
    u8 other1997[3];
    s16 rpm;
    u8 other2002[54];
} Physics;
typedef struct Model952 {u8 other0[239];s8 place_locked;u8 other240[617];s8 state;u8 other858[94];} Model952;
extern Model952 player_array[];
extern s8 D_80152718,D_8013FECB;
extern void func_800E32CC(Physics *),object_update_full(Physics *),func_800E0B20(Physics *);

void func_800E3430(Physics *);

#endif
