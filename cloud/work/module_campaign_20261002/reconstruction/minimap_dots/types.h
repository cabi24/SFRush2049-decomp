#ifndef MINIMAP_DOT_TYPES_H
#define MINIMAP_DOT_TYPES_H
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Sprite {
    s32 handle;
    u32 word4,word8;
    u16 half12;
    s16 x,y;
    u16 half18;
    s16 width,height;
    u8 alpha,byte25;
    s8 hidden;
    u8 byte27;
    s16 bounds[4];
    u32 word36;
    s32 state;
    u32 selector,word48;
    u16 render_slot;
} Sprite;
typedef struct Car952 {
    u8 prefix[8];
    f32 position[3];
    u8 to_dead[836];
    s8 dead,active;
    u8 tail[94];
} Car952;
typedef struct Model2056 {
    u8 prefix[1600];
    s8 crash;
    u8 to_hit_target[131];
    s16 hit_target;
    u8 to_mode[262];
    s8 mode;
    u8 to_hide[18];
    s8 hide;
    u8 to_collidable[11];
    s8 collidable;
    u8 tail[28];
} Model2056;
typedef struct WorldPoint {s16 x,y,z;} WorldPoint;
typedef struct MapOrigin {s32 x,y;} MapOrigin;
typedef struct SpriteRender32 {u8 prefix[21],flags,tail[10];} SpriteRender32;
typedef struct ClockState {u8 prefix[636];u32 tick;} ClockState;
extern Car952 player_array[];
extern Model2056 D_8014A250[];
extern s8 D_80142DB4[],D_80156BDC,D_80140A04;
extern s16 D_80151AD0,D_801543CA;
extern WorldPoint D_801407B4,D_801407D4;
extern MapOrigin D_801160A8[];
extern s32 D_801161C4;
extern SpriteRender32 D_80140BF0[];
extern ClockState D_8002E8E8;
extern void Input_ApplyPadConfig(Sprite *);
extern void stat_race_update(Sprite *,s32,s32,s32);
extern s8 input_new_data_wrapper(s8 *,s32);
#endif
