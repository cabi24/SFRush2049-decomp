#ifndef RUNTIME_B_AA8C_ENTRY_LAYOUTS_H
#define RUNTIME_B_AA8C_ENTRY_LAYOUTS_H
/* Research-only observed layout. Neutral field names do not claim original declarations. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct BattleTransform {
    f32 matrix[3][3];
    f32 position[3];
} BattleTransform; /* 0x30 */

typedef struct BattleSlots {
    s32 handles[5];                 /* 0x000 */
    BattleTransform transforms[5];  /* 0x014 */
    s32 active;                     /* 0x104 */
    f32 timer;                      /* 0x108 */
} BattleSlots; /* 0x10C */

typedef struct BattleGroup {
    s32 handles[5];                 /* 0x000 */
    BattleTransform transforms[5];  /* 0x014 */
    s32 extra_handle;               /* 0x104 */
    BattleTransform extra_transform;/* 0x108 */
    s8 state138;                    /* 0x138, distinct from state139 */
    s8 state139;                    /* 0x139, cleared only for a live extra_handle */
    s16 state13A;
    f32 value13C;
    f32 value140;
    f32 value144;
} BattleGroup; /* 0x148 */

typedef struct BattleDescriptor {
    u8 unknown00[6];
    s16 handle;                     /* 0x06 */
    s16 owner;                      /* 0x08 */
    u8 unknown0A[0x18 - 0x0A];
} BattleDescriptor; /* 0x18; callback is in the still-opaque word at 0x14 */

typedef struct BattlePlayer {
    u8 unknown000[0x2F0];
    BattleDescriptor attachment;    /* 0x2F0 */
    u8 unknown308[0x34C - 0x308];
    u32 color;                      /* 0x34C */
    u8 unknown350[0x384 - 0x350];
    s8 mode;                        /* 0x384 */
    s8 selection;                   /* 0x385 */
    u8 unknown386[0x3B8 - 0x386];
} BattlePlayer; /* 0x3B8 */

typedef struct BattlePhysics {
    u8 unknown000[8];
    u8 model;                       /* 0x008 */
    u8 unknown009[0x640 - 9];
    s8 inhibit;                     /* 0x640 */
    u8 unknown641[0x808 - 0x641];
} BattlePhysics; /* 0x808 */

/* Externally owned storage. No initialized data or new storage is defined. */
extern BattleSlots D_80399120[];
extern BattleGroup D_80399550[];
extern s8 D_80399118[];
extern BattlePlayer D_80152818[];
extern BattlePhysics D_8014A250[];
extern u32 D_80394884;
extern void sound_call_minimal(s16 handle);
extern void model_data_load(s32 handle, s32 mode, u32 viewport_mask);
#endif
