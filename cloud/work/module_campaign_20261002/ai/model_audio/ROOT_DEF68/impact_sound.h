#ifndef ROOT_IMPACT_SOUND_H
#define ROOT_IMPACT_SOUND_H
typedef signed char s8;typedef unsigned char u8;
typedef signed short s16;typedef unsigned short u16;
typedef int s32;typedef unsigned int u32;typedef float f32;
typedef struct Impact24 {f32 force;f32 vector[3];f32 timestamp;s32 flag;} Impact24;
typedef struct Impact120 {Impact24 point[5];} Impact120;
typedef struct AudioModel2056 {
    u8 to_body_force[196];f32 body_force[4][3];
    u8 to_center_force[60];f32 center_force[3];
    u8 to_speed[692];f32 speed;
    u8 to_crash[588];s8 crash;u8 gap1601;
    s8 collision_kind,last_collision_kind;s16 collision_direction;
    u8 to_player[384];s16 player;u8 tail[64];
} AudioModel2056;
extern Impact120 D_80140808[];
extern f32 D_8002EB90,D_80140BE0[],D_80140B10[],D_80142518[];
extern s32 D_8011735C,D_80140AE0[],state_word_a;
extern s16 D_8011F020[],D_8011F040[],D_80140A08[];
extern f32 D_80124324,D_80124328,D_8012432C,D_80124330,D_80124350,D_8012436C;
void func_800DED78(s32,s16,void*,f32);
void best_times_display(s16);
u32 high_scores_display(u32,u32,u32,u8,f32,f32,f32);
void scheduler_recv(s32);
f32 fabsf(f32);
#pragma intrinsic(fabsf)
#endif
