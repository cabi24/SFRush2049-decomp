/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
#pragma pack(1)
typedef struct RampState {
    u8 unknown00[36];
    u32 flags24;
    u8 unknown28[100];
    u32 start8C;
    u32 end90;
    u32 current94;
} RampState;
#pragma pack(0)
extern u32 D_8004BE90;
u32 func_80019AD4(RampState *state, u32 target)
{
    s32 duration;
    u32 previous;
    if (state->flags24 & 0x800) {
        duration = (state->end90 - state->start8C) >> 8;
        if (duration > 0) {
            previous = state->current94;
            state->current94 += (s32)(D_8004BE90 * (u32)((s32)(target - state->current94) >> 8)) / duration;
            if ((previous < target && state->current94 < target) ||
                (previous > target && state->current94 > target)) {
                target = state->current94;
                state->start8C += D_8004BE90;
            } else {
                state->start8C = state->end90;
            }
        }
    }
    return target;
}
