#ifndef VISUAL_TIRE_TYPES_H
#define VISUAL_TIRE_TYPES_H
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef f32 Vec3[3];
typedef f32 Matrix3[3][3];
typedef struct Visual {
    u8 prefix[4];
    s16 tire,object,slot,reserved;
    f32 angle,time;
    void (*callback)(void);
} Visual;
typedef struct Tire92 {
    u8 prefix[64];
    f32 angular_velocity;
    u8 tail[24];
} Tire92;
typedef struct Model2056 {
    u8 prefix[8];
    u8 car_type;
    u8 to_body_type[6];
    s8 body_type;
    u8 to_steering[928];
    f32 steering;
    u8 to_tires[132];
    Tire92 tires[4];
    u8 to_suspension[36];
    f32 suspension[4];
    u8 to_visual_code[64];
    u16 visual_code[4];
    u8 to_player[418];
    s16 player;
    u8 to_mode[4];
    s8 mode;
    u8 tail[59];
} Model2056;
typedef struct Car952 {
    u8 prefix[232];
    u32 appearance;
    u8 to_mph[12];
    s16 mph;
    u8 to_controller[610];
    s8 controller,render_state,reserved,hidden;
    u8 tail[88];
} Car952;
typedef struct CarConfig {
    u8 prefix[96];
    f32 *radius[4];
    Vec3 tire_position[4];
} CarConfig;
typedef struct Control152 {
    u8 prefix[144];
    f32 spring_save;
    u8 tail[4];
} Control152;
typedef struct Object68 {
    u32 flags;
    u8 tail[64];
} Object68;
extern Model2056 D_8014A250[];
extern Car952 player_array[];
extern CarConfig *D_80110D08[];
extern Control152 D_80150B70[];
extern Object68 D_8012E700[];
extern s8 D_80140418,D_80156994,D_8015978C;
extern u8 D_80123264[];
extern s32 D_80143F68[];
extern f32 D_801112DC[][13],D_801113E0[][13];
extern f32 D_801543CC;
extern f32 D_8012393C,D_80123940,D_80123944,D_80123948;
extern s32 func_8008B2B4(void);
extern void model_data_load(s16,s32,s32);
extern void model_transform_setup(s16,s32,s32);
extern s32 model_bounds_calc(s32,Visual *);
extern s32 matrix_scale_apply(Visual *,s32,s32);
extern void func_8008D870(s16,s32,s32);
extern void euler_to_matrix(Matrix3,Vec3);
extern void func_8008B32C(Matrix3,Matrix3,f32);
extern void func_8008D6FC(s16,Vec3,Matrix3);
#endif
