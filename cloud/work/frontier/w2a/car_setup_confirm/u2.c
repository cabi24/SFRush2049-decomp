/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x000 */ char pad0[0x7C6];
    /* 0x7C6 */ s16 slot;
    /* 0x7C8 */ char pad7C8[4];
    /* 0x7CC */ s8 drone_type;
    /* 0x7CD */ char pad7CD[0x1B];
    /* 0x7E8 */ s8 unk7E8;
    /* 0x7E9 */ char pad7E9[0x1F];
} Model; /* 0x808 */

typedef struct {
    /* 0x000 */ char pad0[0xEE];
    /* 0x0EE */ s8 place;
    /* 0x0EF */ s8 place_locked;
    /* 0x0F0 */ f32 score;
    /* 0x0F4 */ char padF4[0xC];
    /* 0x100 */ f32 distance;
    /* 0x104 */ char pad104[0x255];
    /* 0x359 */ s8 unk359;
    /* 0x35A */ char pad35A[0x5E];
} GameCar; /* 0x3B8 */

extern s8 D_80152744;
extern s16 D_80152734;
extern Model D_8014A250[];
extern GameCar D_80152818[];
extern s32 D_8014A110;
extern s16 D_8014A108;
extern s8 D_80152718;
extern f32 D_801543AC;
extern s16 D_801543CA;

s32 w2a_helper(Model *m) {
    return D_8014A110 != 2 || m == D_8014A250;
}
