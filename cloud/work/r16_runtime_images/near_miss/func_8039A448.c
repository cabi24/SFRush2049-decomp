typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
typedef struct { u8 pad[8]; s32 f8; } Inner;
typedef struct { Inner *inner; } Outer;
typedef struct { u8 pad[72]; Outer *owner; } Player;
extern Player D_8014A118[];
extern u8 D_801543D4;
extern s8 D_803B3424;
extern s8 D_803B3428;

s32 func_8039A448(s32 mode)
{
    return (mode != 3 || D_8014A118[D_801543D4].owner->inner->f8 == 0)
        && (!(mode == 0) || D_803B3424 != 0)
        && (!(mode == 1) || D_803B3428 != 0);
}
