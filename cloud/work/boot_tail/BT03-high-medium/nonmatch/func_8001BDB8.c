/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: see README.md and verification.json. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct ChannelState {
    u32 unknown00[2];
    int flags08;
    u32 pending0C;
    u32 unknown10;
    u8 kind14;
    u8 unknown15[19];
} ChannelState;
extern ChannelState D_8004F300[];
int func_8001BDB8(u8 index)
{
    ChannelState *state;
    state = &D_8004F300[index];
    if (state->kind14 != 4 && state->pending0C != 0 && state->flags08 < 0) return 1;
    return 0;
}
