/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState {
    u8 unknown00[36];
    u32 flags;
    u8 unknown28[34];
    u8 channel;
    u8 set;
    u8 unknown4C[64];
    u32 current8C;
    u32 stored90;
    u32 fixed94;
    u8 mode98;
    u8 unknown99[40];
    u8 lastC1;
} VoiceState;
#pragma pack(0)
extern void func_80020FDC(u8, u8, u8);
void func_80019BE4(VoiceState *state)
{
    if (!(state->flags & 0x80000)) {
        if (state->mode98 == 1) {
            if (!(state->flags & 0x2000)) state->current8C = 0;
            else state->current8C = state->stored90;
        } else {
            state->current8C = state->stored90;
        }
        state->fixed94 = (u32)state->lastC1 << 16;
    }
    if (state->channel != 255) func_80020FDC(state->channel, state->set, 1);
}
